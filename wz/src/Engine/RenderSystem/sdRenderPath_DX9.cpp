#include "sdRenderSystemPCH.h"
#include "sdRenderPath_DX9.h"
#include "sdRenderObject.h"
#include "sdRenderObjectAlloc.h"

#include <sdMap.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdRenderPath_DX9::sdRenderPath_DX9()
: m_pkCurMap(NULL)
, m_pkCurCam(NULL)
, m_pkCurFinalRenderTarget(NULL)
, m_bEnableRendering(true)
, m_bEnableStaticMeshRendering(true)
, m_bEnableSkinnedMeshRendering(true)
, m_bUseLocalLight(true)
, m_bUseEarlyZ(false)
, m_bUseMRT(true)
{
	// 创建RenderObject分配器缓存
	m_spRenderObjectAlloc = NiNew sdRenderObjectAlloc(1024 * 1024 * 5, 1024 * 1024);
	NIASSERT(m_spRenderObjectAlloc);

	// 创建RenderPass
	//@{
	m_spEarlyZPass = NiNew sdEarlyZPass(m_spRenderObjectAlloc);
	NIASSERT(m_spEarlyZPass);

	m_spMRTGeometryPass = NiNew sdMRTGeometryPass(m_spRenderObjectAlloc);
	NIASSERT(m_spMRTGeometryPass);

	m_spMRTShadingPass = NiNew sdMRTShadingPass(m_spRenderObjectAlloc);
	NIASSERT(m_spMRTShadingPass);

	m_spPreLightPass = NiNew sdPreLightPass(m_spRenderObjectAlloc);
	NIASSERT(m_spPreLightPass);

	m_spProjectorPass = NiNew sdProjectorPass(m_spRenderObjectAlloc);
	NIASSERT(m_spProjectorPass);

	m_spTerrainDepthPass = NiNew sdTerrainDepthPass(m_spRenderObjectAlloc);
	NIASSERT(m_spTerrainDepthPass);

	m_spTerrainAtlasGeometryPass = NiNew sdTerrainAtlasGeometryPass(m_spRenderObjectAlloc);
	NIASSERT(m_spTerrainAtlasGeometryPass);

	m_spTerrainAtlasShadingPass = NiNew sdTerrainAtlasShadingPass(m_spRenderObjectAlloc);
	NIASSERT(m_spTerrainAtlasShadingPass);

	m_spMLAAPass = NiNew sdMLAAPass(m_spRenderObjectAlloc);
	NIASSERT(m_spMLAAPass);

	m_spColorGradingPass = NiNew sdColorGradingPass(m_spRenderObjectAlloc);
	NIASSERT(m_spColorGradingPass);

	m_spDofPass = NiNew sdDOFPass(m_spRenderObjectAlloc);
	NIASSERT(m_spDofPass);

	m_spGodRayPass = NiNew sdGodRayPass(m_spRenderObjectAlloc);
	NIASSERT(m_spGodRayPass);

	m_spHDRPass = NiNew sdHDRPass(m_spRenderObjectAlloc);
	NIASSERT(m_spHDRPass);

	m_spFinalPass = NiNew sdFinalPass(m_spRenderObjectAlloc);
	NIASSERT(m_spFinalPass);

	m_spEdgePass = NiNew sdEdgePass(m_spRenderObjectAlloc);
	NIASSERT(m_spEdgePass);

	m_spEdgeEnhancementPass = NiNew sdEdgeEnhancementPass(m_spRenderObjectAlloc);
	NIASSERT(m_spEdgeEnhancementPass);

	m_spDebugGizmoPass = NiNew sdDebugGizmoPass(m_spRenderObjectAlloc);
	NIASSERT(m_spDebugGizmoPass);
	//@}

	// 初始化渲染参数
	m_kRenderParams.Reset();
	m_kEnvironmentParams.Reset();
	m_kPostProcessParams.Reset();
	m_kTerrainParams.Reset();
}
//-------------------------------------------------------------------------------------------------
sdRenderPath_DX9::~sdRenderPath_DX9()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}

	// 清除Params
	// @{
	m_kRenderParams.Reset();
	m_kEnvironmentParams.Reset();
	m_kPostProcessParams.Reset();
	m_kTerrainParams.Reset();
	// @}
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::Initialize()
{
	// 已经初始化则直接返回
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderTargetGroup* spRenderTarget = pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(spRenderTarget);
	uint uiWidth = spRenderTarget->GetWidth(0);
	uint uiHeight = spRenderTarget->GetHeight(0);

	// 初始化Shader变量(主要是屏幕尺寸相关)
	// @{
	// 屏幕尺寸
	NiPoint2 kWindowSize((float)uiWidth, (float)uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vWindowSize", sizeof(NiPoint2), &kWindowSize);

	// 屏幕半像素偏移
	NiPoint2 kHalfPixelOffset(0.5f / (float)uiWidth, 0.5f / (float)uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHalfPixelOffset", sizeof(NiPoint2), &kHalfPixelOffset);

	// 投影坐标到屏幕纹理坐标的偏移
	NiPoint2 kPixelToTexelOffset(0.5f + 0.5f / (float)uiWidth, 0.5f + 0.5f / (float)uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vPixToTexOffset", sizeof(NiPoint2), &kPixelToTexelOffset);
	// @}

	// 创建Texture
	CreateAllRenderedTexture();

	// 创建RenderTarget
	CreateAllRenderTarget();

	// 初始化RenderPass
	// @{
	m_spEarlyZPass->Initialize(E_SID_STATIC_MESH, E_SID_MASK);

	m_spMRTGeometryPass->Initialize(E_SID_STATIC_MESH, E_SID_MASK);
	m_spMRTShadingPass->Initialize(E_SID_STATIC_MESH, E_SID_MASK, 
		m_spLightTexture, m_spGeometryTexture, m_spAlbedoTexture, m_spGlossTexture,
		m_spGlossTexture/* m_spGlowTexture*/);
	
	uint uiLightVolumeStencilID = 0x1 << E_PSB_DEFERED_LIGHT_SHIFT;
	uint uiLightVolumeStencilIDMask = 0x1 << E_PSB_DEFERED_LIGHT_SHIFT;
	uint uiLightVolumeStencilIDWriteMask = uiLightVolumeStencilIDMask;
	m_spPreLightPass->Initialize(uiLightVolumeStencilID, uiLightVolumeStencilIDMask,
		uiLightVolumeStencilIDWriteMask, m_spGeometryTexture);

	uint uiProjectorVolumeStencilID = 0x1 << E_PSB_RECEIVE_DECAL_SHIFT;
	uint uiProjectorVolumeStencilIDMask = 0x1 << E_PSB_RECEIVE_DECAL_SHIFT;
	m_spProjectorPass->Initialize(uiProjectorVolumeStencilID, 
		uiProjectorVolumeStencilIDMask, m_spGeometryTexture, m_spLightTexture);

	uint uiTerrainDepthStencilID = E_SID_TERRAIN | (0x1 << E_PSB_RECEIVE_DECAL_SHIFT);
	uint uiTerrainDepthStencilIDMask = E_SID_MASK | (0x1 << E_PSB_RECEIVE_DECAL_SHIFT);
	m_spTerrainDepthPass->Initialize(uiTerrainDepthStencilID, uiTerrainDepthStencilIDMask);
	m_spTerrainAtlasGeometryPass->Initialize(E_SID_TERRAIN, E_SID_MASK, E_PSB_DISTANCE_TERRAIN_SHIFT, m_spLightTexture);
	m_spTerrainAtlasShadingPass->Initialize(E_SID_TERRAIN, E_SID_MASK, m_spLightTexture, m_spGeometryTexture);
	
	m_spMLAAPass->Initialize();
	m_spColorGradingPass->Initialize();
	m_spDofPass->Initialize(m_kPostProcessParams);
	m_spGodRayPass->Initialize();
	m_spHDRPass->Initialize(m_kPostProcessParams);
	m_spFinalPass->Initialize();

	uint uiEdgeStencilID = 0x1 << E_PSB_EDGE_SHIFT;
	uint uiEdgeStencilIDMask = 0x1 << E_PSB_EDGE_SHIFT;
	uint uiEdgeStencilIDWriteMask = 0x1 << E_PSB_EDGE_SHIFT;
	m_spEdgePass->Initialize(uiEdgeStencilID, uiEdgeStencilIDMask, uiEdgeStencilIDWriteMask);
	m_spEdgeEnhancementPass->Initialize(uiEdgeStencilID, uiEdgeStencilIDMask, m_spAlbedoTexture);

	m_spDebugGizmoPass->Initialize();
	// @}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::Destroy()
{
	// 已经被销毁则直接返回
	if (!m_bInitialized)
		return;

	// 销毁RenderPass
	m_spEarlyZPass->Destroy();
	m_spMRTGeometryPass->Destroy();
	m_spMRTShadingPass->Destroy();
	m_spPreLightPass->Destroy();
	m_spProjectorPass->Destroy();
	m_spTerrainDepthPass->Destroy();
	m_spTerrainAtlasGeometryPass->Destroy();
	m_spTerrainAtlasShadingPass->Destroy();
	m_spMLAAPass->Destroy();
	m_spColorGradingPass->Destroy();
	m_spDofPass->Destroy();
	m_spGodRayPass->Destroy();
	m_spHDRPass->Destroy();
	m_spFinalPass->Destroy();
	m_spEdgePass->Destroy();
	m_spEdgeEnhancementPass->Destroy();
	m_spDebugGizmoPass->Destroy();

	// 销毁Textute
	ReleaseAllRenderTexture();

	// 销毁RenderTarget
	ReleaseAllRenderTarget();
	
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::CreateAllRenderedTexture()
{
	typedef IRenderDevice::TextureSpec TextureSpec;

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 获取屏幕尺寸
	NiRenderTargetGroup* spRenderTarget = pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(spRenderTarget);
	uint uiWidth = spRenderTarget->GetWidth(0);
	uint uiHeight = spRenderTarget->GetHeight(0);

	//
	NiTexture::FormatPrefs kFormatARGB8;
	kFormatARGB8.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kFormatARGB8.m_eAlphaFmt	= NiTexture::FormatPrefs::SMOOTH;
	kFormatARGB8.m_eMipMapped	= NiTexture::FormatPrefs::NO;

	NiTexture::FormatPrefs kFormatARGB16;
	kFormatARGB16.m_ePixelLayout = NiTexture::FormatPrefs::FLOAT_COLOR_64;
	kFormatARGB16.m_eAlphaFmt	= NiTexture::FormatPrefs::SMOOTH;
	kFormatARGB16.m_eMipMapped	= NiTexture::FormatPrefs::NO;

	NiTexture::FormatPrefs kFormatA32;
	kFormatA32.m_ePixelLayout = NiTexture::FormatPrefs::SINGLE_COLOR_32;
	kFormatA32.m_eAlphaFmt	= NiTexture::FormatPrefs::SMOOTH;
	kFormatA32.m_eMipMapped	= NiTexture::FormatPrefs::NO;

	NiTexture::FormatPrefs kFormatHightColor16;
	kFormatHightColor16.m_ePixelLayout = NiTexture::FormatPrefs::HIGH_COLOR_16;
	kFormatHightColor16.m_eAlphaFmt	= NiTexture::FormatPrefs::NONE;
	kFormatHightColor16.m_eMipMapped	= NiTexture::FormatPrefs::NO;

	TextureSpec kFullTextureARGB8Spec		= {uiWidth,		uiHeight,	kFormatARGB8};
	TextureSpec kHalfTextureARGB8Spec		= {uiWidth/2,	uiHeight/2, kFormatARGB8};
	TextureSpec kQuaterTextureARGB8Spec		= {uiWidth/4,	uiHeight/4, kFormatARGB8};

	TextureSpec kTextureARGB8Spec			= {uiWidth,		uiHeight,	kFormatARGB8};
	TextureSpec kTextureA32Spec				= {uiWidth,		uiHeight,	kFormatA32};
	TextureSpec kTextureARGB16Spec			= {uiWidth,		uiHeight,	kFormatARGB16};
	TextureSpec kFullTextureHighColorSpec	= {uiWidth,		uiHeight,	kFormatHightColor16};

	// 创建纹理(WZ给NiRenderedTexture添加了一个Resize函数,省事了许多)
	// @{
	m_spGeometryTexture = pkRenderDevice->CreateRenderTexture(kFullTextureARGB8Spec);
	NIASSERT(m_spGeometryTexture);

	m_spAlbedoTexture = pkRenderDevice->CreateRenderTexture(kFullTextureARGB8Spec);
	NIASSERT(m_spAlbedoTexture);

	m_spGlossTexture = pkRenderDevice->CreateRenderTexture(kFullTextureARGB8Spec);
	NIASSERT(m_spGlossTexture);

	m_spLightTexture = pkRenderDevice->CreateRenderTexture(kFullTextureARGB8Spec);
	NIASSERT(m_spLightTexture);

	m_spShadingTexture  = pkRenderDevice->CreateRenderTexture(kFullTextureARGB8Spec);
	NIASSERT(m_spShadingTexture);

	m_spMLAAEdgeDetectTexture = pkRenderDevice->CreateRenderTexture(kFullTextureHighColorSpec);
	NIASSERT(m_spMLAAEdgeDetectTexture);

	m_spDofBlurXTexture  = pkRenderDevice->CreateRenderTexture(kHalfTextureARGB8Spec);
	NIASSERT(m_spDofBlurXTexture);

	m_spDofBlurYTexture  = pkRenderDevice->CreateRenderTexture(kHalfTextureARGB8Spec);
	NIASSERT(m_spDofBlurYTexture);

	// HDR Scale
	kTextureARGB16Spec.uiWidth	= uiWidth/2;
	kTextureARGB16Spec.uiHeight = uiHeight/2;
	m_spHDRHalfSizeScaleTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB16Spec);
	NIASSERT(m_spHDRHalfSizeScaleTexture);

	kTextureARGB16Spec.uiWidth	= uiWidth/4;
	kTextureARGB16Spec.uiHeight = uiHeight/4;
	m_spHDRQuaterSizeScaleTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB16Spec);
	NIASSERT(m_spHDRQuaterSizeScaleTexture);

	kTextureARGB16Spec.uiWidth	= uiWidth/8;
	kTextureARGB16Spec.uiHeight = uiHeight/8;
	m_spHDROctantSizeScaleTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB16Spec);
	NIASSERT(m_spHDROctantSizeScaleTexture);

	// HDR Luminance
	kTextureA32Spec.uiWidth = 1;
	kTextureA32Spec.uiHeight = 1;
	m_spHDRCurLuminanceTexture = pkRenderDevice->CreateRenderTexture(kTextureA32Spec);
	NIASSERT(m_spHDRCurLuminanceTexture);

	kTextureA32Spec.uiWidth = 1;
	kTextureA32Spec.uiHeight = 1;
	m_spHDRLastLuminanceTexture = pkRenderDevice->CreateRenderTexture(kTextureA32Spec);
	NIASSERT(m_spHDRLastLuminanceTexture);

	kTextureA32Spec.uiWidth = 1;
	kTextureA32Spec.uiHeight = 1;
	m_spHDRCurAdaptedLuminanceTexture = pkRenderDevice->CreateRenderTexture(kTextureA32Spec);
	NIASSERT(m_spHDRCurAdaptedLuminanceTexture);

	uint uiStartSize = min(uiWidth / 16, uiHeight / 16);
	uint uiChainLength = 1;
	uint uiSize = 16;
	for (; uiSize < uiStartSize; uiSize *= 4)	++uiChainLength;
	uiSize /= 4;
	for (uint ui = 0; ui < uiChainLength; ++ui, uiSize/=4)
	{
		kTextureA32Spec.uiWidth		= uiSize;
		kTextureA32Spec.uiHeight	= uiSize;

		NiRenderedTexturePtr spIntermediateLuminanceTexture = pkRenderDevice->CreateRenderTexture(kTextureA32Spec);
		NIASSERT(spIntermediateLuminanceTexture);
		m_kHDRIntermediateLuminanceTexture.push_back(spIntermediateLuminanceTexture);
	}

	// HDR Bloom
	kTextureARGB8Spec.uiWidth	= uiWidth/8;
	kTextureARGB8Spec.uiHeight	= uiHeight/8;
	m_spHDRBloomThresholdTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB8Spec);
	NIASSERT(m_spHDRBloomThresholdTexture);

	kTextureARGB8Spec.uiWidth	= uiWidth/8;
	kTextureARGB8Spec.uiHeight	= uiHeight/8;
	m_spHDRBloomBlurXTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB8Spec);
	NIASSERT(m_spHDRBloomBlurXTexture);

	kTextureARGB8Spec.uiWidth	= uiWidth/8;
	kTextureARGB8Spec.uiHeight	= uiHeight/8;
	m_spHDRBloomBlurYTexture = pkRenderDevice->CreateRenderTexture(kTextureARGB8Spec);
	NIASSERT(m_spHDRBloomBlurYTexture);

	// Back Buffer
	m_spHalfSizeTexture  = pkRenderDevice->CreateRenderTexture(kHalfTextureARGB8Spec);
	NIASSERT(m_spHalfSizeTexture);

	for (int i = 0; i < 3; ++i)
	{
		m_spQuaterSizeTexture[i] = pkRenderDevice->CreateRenderTexture(kQuaterTextureARGB8Spec);
		NIASSERT(m_spQuaterSizeTexture[i]);
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::ReleaseAllRenderTexture()
{
	m_spGeometryTexture = 0;
	m_spAlbedoTexture = 0;
	m_spGeometryTexture = 0;
	m_spLightTexture = 0;
	m_spShadingTexture = 0;
	m_spMLAAEdgeDetectTexture = 0;
	m_spDofBlurXTexture = 0;
	m_spDofBlurYTexture = 0;
	m_spHalfSizeTexture = 0;
	m_spQuaterSizeTexture[0] = 0;
	m_spQuaterSizeTexture[1] = 0;
	m_spQuaterSizeTexture[2] = 0;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::CreateAllRenderTarget()
{

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 获取屏幕尺寸
	NiRenderTargetGroup* spRenderTarget = pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(spRenderTarget);
	uint uiWidth = spRenderTarget->GetWidth(0);
	uint uiHeight = spRenderTarget->GetHeight(0);

	// 创建渲染目标
	// @{
	m_spTerrainDepthTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spTerrainDepthTarget);

	m_spGeometryAlbedoMRT = pkRenderDevice->CreateRenderTarget(3);
	NIASSERT(m_spGeometryAlbedoMRT);

	m_spGeometryTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spGeometryTarget);

	m_spAlbedoTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spAlbedoTarget);

	m_spGlossTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spGlossTarget);

	m_spLightTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spLightTarget);

	m_spShadingTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spShadingTarget);

	m_spMLAAEdgeDetectTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spMLAAEdgeDetectTarget);

	m_spDofBlurXTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spDofBlurXTarget);

	m_spDofBlurYTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spDofBlurYTarget);

	m_spHDRHalfSizeScaleTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRHalfSizeScaleTarget);

	m_spHDRQuaterSizeScaleTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRQuaterSizeScaleTarget);

	m_spHDROctantSizeScaleTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDROctantSizeScaleTarget);

	m_spHDRCurLuminanceTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRCurLuminanceTarget); 

	m_spHDRLastLuminanceTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRLastLuminanceTarget); 

	m_spHDRCurAdaptedLuminanceTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRCurAdaptedLuminanceTarget); 

	uint uiStartSize = min(uiWidth / 16, uiHeight / 16);
	uint uiChainLength = 1;
	uint uiSize = 16;
	for (; uiSize < uiStartSize; uiSize *= 4)	++uiChainLength;
	uiSize /= 4;
	for (uint ui = 0; ui < uiChainLength; ++ui, uiSize/=4)
	{
		NiRenderTargetGroupPtr spIntermediateLuminanceTarget = pkRenderDevice->CreateRenderTarget(1);
		NIASSERT(spIntermediateLuminanceTarget);
		m_kHDRIntermediateLuminanceTarget.push_back(spIntermediateLuminanceTarget);
	}

	m_spHDRBloomThresholdTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRBloomThresholdTarget); 

	m_spHDRBloomBlurXTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRBloomBlurXTarget); 

	m_spHDRBloomBlurYTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHDRBloomBlurYTarget); 

	m_spHalfSizeTarget = pkRenderDevice->CreateRenderTarget(1);
	NIASSERT(m_spHalfSizeTarget);

	for (int i = 0; i < 3; ++i)
	{
		m_spQuaterSizeTarget[i] = pkRenderDevice->CreateRenderTarget(1);
		NIASSERT(m_spQuaterSizeTarget[i]);
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::ReleaseAllRenderTarget()
{
	m_spTerrainDepthTarget = 0;
	m_spGeometryAlbedoMRT	= 0;
	m_spGeometryTarget = 0;
	m_spAlbedoTarget = 0;
	m_spGlossTarget = 0;
	m_spLightTarget	= 0;
	m_spShadingTarget = 0;
	m_spMLAAEdgeDetectTarget = 0;
	m_spDofBlurXTarget = 0;
	m_spDofBlurYTarget = 0;
	m_spHDRHalfSizeScaleTarget = 0;
	m_spHDRQuaterSizeScaleTarget = 0;
	m_spHDROctantSizeScaleTarget = 0;
	m_spHalfSizeTarget = 0;
	m_spQuaterSizeTarget[0] = 0;
	m_spQuaterSizeTarget[1] = 0;
	m_spQuaterSizeTarget[2] = 0;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UpdateRenderParams(const sdRenderParams& kRenderParams)
{
	m_kRenderParams = kRenderParams;

	// 更新到所有RenderPass(不明白为什么用拷贝)
	m_spEarlyZPass->SetRenderParams(&kRenderParams);
	m_spMRTGeometryPass->SetRenderParams(&kRenderParams);
	m_spMRTShadingPass->SetRenderParams(&kRenderParams);
	m_spPreLightPass->SetRenderParams(&m_kRenderParams);
	m_spProjectorPass->SetRenderParams(&kRenderParams);
	m_spTerrainDepthPass->SetRenderParams(&kRenderParams);
	m_spTerrainAtlasGeometryPass->SetRenderParams(&kRenderParams);
	m_spTerrainAtlasShadingPass->SetRenderParams(&m_kRenderParams);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UpdateEnvironmentParams(const sdEnvironmentParams& kEnvParams)
{
	m_kEnvironmentParams = kEnvParams;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UpdatePostProcessParams(const sdPostProcessParams& kPostProcessParams)
{
	m_kPostProcessParams = kPostProcessParams;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UpdateTerrainParams(const sdTerrainParams& kTerrainParams)
{
	// 快速检查参数是否发生改变
	// (由于sdTerrainParams继承自NiMemObject故可以使用memcmp函数)
	if (0 != memcmp(&m_kTerrainParams, &kTerrainParams, sizeof(sdTerrainParams)))
	{
		m_kTerrainParams = kTerrainParams;

		// 更新到地形RenderPass
		m_spTerrainAtlasGeometryPass->SetTerrainParams(kTerrainParams);
		m_spTerrainAtlasShadingPass->SetTerrainParams(kTerrainParams);
	}
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderStaticMesh(NiMesh* pkMesh)
{
	NIASSERT(pkMesh);

	if (!m_bEnableStaticMeshRendering)
		return false;

//	// 加入深度预渲染通道
//	if (m_bUseEarlyZ)
//		m_spEarlyZPass->InsertStaticMesh(pkMesh);


	//***************************************
	// 屏蔽DarkMap
	NiPropertyStatePtr spPropState = pkMesh->GetPropertyState();
	NiTexturingPropertyPtr spTexProp = spPropState->GetTexturing();
	spTexProp->SetDarkMap(NULL);
	spTexProp->SetParallaxMap(NULL);
	//***************************************

	// 加入G-Buffer生成通道
	m_spMRTGeometryPass->InsertStaticMesh(pkMesh);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderSkinnedMesh(NiMesh* pkMesh)
{
	NIASSERT(pkMesh);

	if (!m_bEnableSkinnedMeshRendering)
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderTerrainMesh(NiMesh* pkMesh)
{
	NIASSERT(pkMesh);

	// 加入地形深度渲染通道
	m_spTerrainDepthPass->InsertTerrainMesh(pkMesh);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderLight(Engine::sdLightSceneObject* pkLight)
{
	NIASSERT(pkLight);

	bool bEnableLightChannel = m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_LOCALLIGHT);
	if (m_bUseLocalLight && bEnableLightChannel)
	{
		NiColor kDiffuseColor =  pkLight->GetDiffuseColor() * pkLight->GetDimmer();
		if (kDiffuseColor.r < 0.001f && kDiffuseColor.g < 0.001f && kDiffuseColor.g < 0.001f)	
			return true;

		m_spPreLightPass->InsertLight(pkLight);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderProjector(sdProjectorSceneObject* pkProjector)
{
	NIASSERT(pkProjector);

	if (!pkProjector->GetVisible())
		return true;

	m_spProjectorPass->InsertProjector(pkProjector);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderMeshEdge(NiMesh* pkMesh)
{
	NIASSERT(pkMesh);

	m_spEdgePass->InsertEdgeMesh(pkMesh);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderPath_DX9::RenderMeshEdgeEnhancement(const NiBound& kBound, const NiColor& kColor, uint uiWidth)
{
	m_spEdgeEnhancementPass->InsertEdgeEnhancementArea(kBound, kColor, uiWidth);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::RenderScene(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget)
{
	NIASSERT(pkMap);
	NIASSERT(pkCamera);
	NIASSERT(pkRenderTarget);

	BeginRenderScene(pkMap, pkCamera, pkRenderTarget);

	// 裁剪
	Render();

	// 渲染
	Draw();

	EndRenderScene();	
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::BeginRenderScene(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget)
{
	// 清空渲染缓存
	m_spRenderObjectAlloc->DeallocAll();

	// 更新
	Update(pkMap, pkCamera, pkRenderTarget);

	// 开始RenderPass处理
	m_spEarlyZPass->Begin();
	m_spMRTGeometryPass->Begin();
	m_spMRTShadingPass->Begin();
	m_spProjectorPass->Begin();
	m_spProjectorPass->Begin();
	m_spTerrainDepthPass->Begin();
	m_spTerrainAtlasGeometryPass->Begin();
	m_spTerrainAtlasShadingPass->Begin();
	m_spMLAAPass->Begin();
	m_spColorGradingPass->Begin();
	m_spDofPass->Begin();
	m_spGodRayPass->Begin();
	m_spHDRPass->Begin();
	m_spFinalPass->Begin();
	m_spEdgePass->Begin();
	m_spEdgeEnhancementPass->Begin();
	m_spDebugGizmoPass->Begin();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::EndRenderScene()
{
	// 结束RenderPass处理
	m_spEarlyZPass->End();
	m_spMRTGeometryPass->End();
	m_spMRTShadingPass->End();
	m_spPreLightPass->End();
	m_spProjectorPass->End();
	m_spTerrainDepthPass->End();
	m_spTerrainAtlasGeometryPass->End();
	m_spTerrainAtlasShadingPass->End();
	m_spMLAAPass->End();
	m_spColorGradingPass->End();
	m_spDofPass->End();
	m_spGodRayPass->End();
	m_spHDRPass->End();
	m_spFinalPass->End();
	m_spEdgePass->End();
	m_spEdgeEnhancementPass->End();
	m_spDebugGizmoPass->End();

	// 更新
	Update(NULL, NULL, NULL);

	// 清空渲染缓存
	m_spRenderObjectAlloc->DeallocAll();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::Update(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget)
{
	m_pkCurMap = pkMap;
	m_pkCurCam = pkCamera;

	// 
	UpdateRenderTargets(pkRenderTarget);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UpdateRenderTargets(NiRenderTargetGroup* pkRenderTarget)
{
	m_pkCurFinalRenderTarget = pkRenderTarget;

	if (pkRenderTarget)
	{
		// Depth/Stencil Buffer
		NiDepthStencilBuffer* pkDSBuffer = pkRenderTarget->GetDepthStencilBuffer();
		NIASSERT(pkDSBuffer);

		// Terrain-Depth-Buffer
		m_spTerrainDepthTarget->AttachBuffer(m_spLightTexture->GetBuffer(),	0);
		m_spTerrainDepthTarget->AttachDepthStencilBuffer(pkDSBuffer);

		// Geometry-Buffer
		m_spGeometryAlbedoMRT->AttachBuffer(m_spGeometryTexture->GetBuffer(),	0);
		m_spGeometryAlbedoMRT->AttachBuffer(m_spAlbedoTexture->GetBuffer(),		1);
		m_spGeometryAlbedoMRT->AttachBuffer(m_spGlossTexture->GetBuffer(),		2);
		m_spGeometryAlbedoMRT->AttachDepthStencilBuffer(pkDSBuffer);

		m_spGeometryTarget->AttachBuffer(m_spGeometryTexture->GetBuffer(),	0);
		m_spGeometryTarget->AttachDepthStencilBuffer(pkDSBuffer);

		m_spAlbedoTarget->AttachBuffer(m_spAlbedoTexture->GetBuffer(),	0);
		m_spAlbedoTarget->AttachDepthStencilBuffer(pkDSBuffer);

		m_spGlossTarget->AttachBuffer(m_spGlossTexture->GetBuffer(),	0);
		m_spGlossTarget->AttachDepthStencilBuffer(pkDSBuffer);

		// Light-Buffer
		m_spLightTarget->AttachBuffer(m_spLightTexture->GetBuffer(), 0);
		m_spLightTarget->AttachDepthStencilBuffer(pkDSBuffer);

		// Shading-Buffer
		m_spShadingTarget->AttachBuffer(m_spShadingTexture->GetBuffer(), 0);
		m_spShadingTarget->AttachDepthStencilBuffer(pkDSBuffer); 

		// MLAA-Edge-Buffer
		m_spMLAAEdgeDetectTarget->AttachBuffer(m_spMLAAEdgeDetectTexture->GetBuffer(), 0);

		// DOF-Buffer
		m_spDofBlurXTarget->AttachBuffer(m_spDofBlurXTexture->GetBuffer(), 0);
		m_spDofBlurYTarget->AttachBuffer(m_spDofBlurYTexture->GetBuffer(), 0);

		// HDR-Scale-Buffer
		m_spHDRHalfSizeScaleTarget->AttachBuffer(m_spHDRHalfSizeScaleTexture->GetBuffer(), 0);
		m_spHDRQuaterSizeScaleTarget->AttachBuffer(m_spHDRQuaterSizeScaleTexture->GetBuffer(), 0);
		m_spHDROctantSizeScaleTarget->AttachBuffer(m_spHDROctantSizeScaleTexture->GetBuffer(), 0);

		// HDR-Luminance-Buffer
		m_spHDRCurLuminanceTarget->AttachBuffer(m_spHDRCurLuminanceTexture->GetBuffer(), 0);
		m_spHDRLastLuminanceTarget->AttachBuffer(m_spHDRLastLuminanceTexture->GetBuffer(), 0);
		m_spHDRCurAdaptedLuminanceTarget->AttachBuffer(m_spHDRCurAdaptedLuminanceTexture->GetBuffer(), 0);
		for (uint i = 0; i < m_kHDRIntermediateLuminanceTexture.size(); ++i)
			m_kHDRIntermediateLuminanceTarget[i]->AttachBuffer(m_kHDRIntermediateLuminanceTexture[i]->GetBuffer(), 0);

		// HDR-Bloom-Buffer
		m_spHDRBloomThresholdTarget->AttachBuffer(m_spHDRBloomThresholdTexture->GetBuffer(), 0);
		m_spHDRBloomBlurXTarget->AttachBuffer(m_spHDRBloomBlurXTexture->GetBuffer(), 0);
		m_spHDRBloomBlurYTarget->AttachBuffer(m_spHDRBloomBlurYTexture->GetBuffer(), 0);

		// Back-Buffer
		m_spHalfSizeTarget->AttachBuffer(m_spHalfSizeTexture->GetBuffer(), 0);
		m_spQuaterSizeTarget[0]->AttachBuffer(m_spQuaterSizeTexture[0]->GetBuffer(), 0);
		m_spQuaterSizeTarget[1]->AttachBuffer(m_spQuaterSizeTexture[1]->GetBuffer(), 0);
		m_spQuaterSizeTarget[2]->AttachBuffer(m_spQuaterSizeTexture[2]->GetBuffer(), 0);
	}
	else
	{
		
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE