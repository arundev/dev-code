#include "sdRenderSystemPCH.h"
#include "sdRenderPath_DX9.h"

//
#include <sdMatrix3.h>
#include <sdMatrix4.h>
#include <sdTimeMgr.h>

#include <sdMap.h>
#include <sdLight.h>

using namespace Base;
using namespace Base::Math;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::Draw()
{
	// ������Ⱦ��ֱ�ӷ���
	if (!m_bEnableRendering)
		return;

	// ����������Ⱦ״̬
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);
	pkRenderDevice->SaveAllRenderStates();

	// ���ó�ʼ��Ⱦ״̬
	PrepareRenderStates();

	// ���ñ任����(ò����������ӳ�һ�µȴ�RenderTarget���������)
	pkRenderDevice->SetCameraData(m_pkCurCam);

	// ������ɫ��ȫ�ֱ���
	PrepareShaderConstants();

	// ��Ⱦ
	//@{
	// Ԥ������ȵ�DepthStencilBuffer
	//DrawEarlyZPass();

	// ���Ƶ���Depth���ض�����,����Ⱦ��ģ�建��
	DrawTerrainDepthPass();

	// ���������GeometryBuffer,����Ⱦ��ģ�建��(����Building/Tree/Doodads/Terrain)
	DrawGeometryPasses();

	// ����ֲ�����
	DrawPreLightPass();

	// ��ɫ�ϳ�
	DrawShadingPasses();

	// AO?

	// ȫ����
	

	// ˮ��͸�����

	// ˮ

	// ˮ��͸�����

	// ����

	// ������Ч
	DrawPostProcessPasses();

	// ����
	DrawEdge();

	// ������Ϣ
	m_spDebugGizmoPass->Draw();
	//@}

	// �ָ�������Ⱦ״̬
	pkRenderDevice->RestoreAllRenderStates();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::PrepareRenderStates()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// �ü�ģʽ
	// D3DCULL_NONE
	// D3DCULL_CW
	// D3DCULL_CCW
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ��դ��ģʽ
	// D3DFILL_POINT
	// D3DFILL_WIREFRAME
	// D3DFILL_SOLID
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// ȫ����
	// @{
	// Ӳ��ȫ����
	// D3DRS_FOGENABLE
	// D3DRS_RANGEFOGENABLE
	// D3DRS_FOGCOLOR
	// D3DRS_FOGTABLEMODE
	// D3DRS_FOGSTART
	// D3DRS_FOGEND
	// D3DRS_FOGDENSITY
	pkRenderDevice->SetRenderState(D3DRS_FOGENABLE, false);
	// @}

	// ����Ӳ��sRGB����(ȫ��ֵ)
	pkRenderDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);

	// ���
	// @{
	// ��Ȼ���(��ʼֵ)
	//pkRenderDevice->SetRenderState(D3DRS_ZENABLE,		true);
	//pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,	true);
	//pkRenderDevice->SetRenderState(D3DRS_ZFUNC,			D3DCMP_LESSEQUAL);

	// ���ƫ��(��ʼֵ)
	// D3DRS_DEPTHBIAS
	// D3DRS_SLOPESCALEDEPTHBIAS
	//pkRenderDevice->SetRenderState(D3DRS_DEPTHBIAS,				0);
	//pkRenderDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,	0)
	// @}

	// ģ��
	// @{
	// ģ�����
	// D3DRS_STENCILENABLE
	// D3DRS_STENCILFAIL
	// D3DRS_STENCILZFAIL
	// D3DRS_STENCILPASS
	// D3DRS_STENCILFUNC
	// D3DRS_STENCILREF
	// D3DRS_STENCILMASK
	// D3DRS_STENCILWRITEMASK
	//
	// D3DRS_TWOSIDEDSTENCILMODE
	// D3DRS_CCW_STENCILFAIL
	// D3DRS_CCW_STENCILZFAIL
	// D3DRS_CCW_STENCILPASS
	// D3DRS_CCW_STENCILFUNC
	//pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// ͸��
	// @{
	// ͸������
	// D3DRS_ALPHATESTENABLE
	// D3DRS_ALPHAREF
	// D3DRS_ALPHAFUNC
	//pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	// ͸�����
	// D3DRS_ALPHABLENDENABLE
	// D3DRS_BLENDOP
	// D3DRS_SRCBLEND
	// D3DRS_DESTBLEND
	//
	// D3DRS_SEPARATEALPHABLENDENABLE
	// D3DRS_BLENDOPALPHA
	// D3DRS_SRCBLENDALPHA
	// D3DRS_DESTBLENDALPHA
	//pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	// @}

	// ��ɫ
	// @{
	// ������ɫ���(��ʼֵ)
	// D3DRS_COLORWRITEENABLE
	// D3DRS_COLORWRITEENABLE1
	// D3DRS_COLORWRITEENABLE2
	// D3DRS_COLORWRITEENABLE3
	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::PrepareShaderConstants()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	uint uiWidth = pkRenderDevice->GetDefaultRenderTargetGroup()->GetWidth(0);
	uint uiHeight = pkRenderDevice->GetDefaultRenderTargetGroup()->GetHeight(0);

	// ���
	// @{
	// �����̬
	NiPoint3 kCamDir = m_pkCurCam->GetWorldDirection();
	NiPoint3 kCamUp	 = m_pkCurCam->GetWorldUpVector();
	NiPoint3 kCamRight = m_pkCurCam->GetWorldRightVector();
	kCamDir.Unitize();
	kCamUp.Unitize();
	kCamRight.Unitize();
	pkRenderDevice->SetGlobalShaderConstant("g_vCameraUp", sizeof(NiPoint3), &kCamUp);
	pkRenderDevice->SetGlobalShaderConstant("g_vCameraRight", sizeof(NiPoint3), &kCamRight);

	// �����׶��(��׶�������ǶԳƵ�)
	const NiFrustum& kCamFrustum = m_pkCurCam->GetViewFrustum();
	NiPoint4 kFrustumData(kCamFrustum.m_fRight, kCamFrustum.m_fTop, kCamFrustum.m_fNear, kCamFrustum.m_fFar);
	pkRenderDevice->SetGlobalShaderConstant("g_fCameraFarClip", sizeof(kCamFrustum.m_fFar), &kCamFrustum.m_fFar);
	pkRenderDevice->SetGlobalShaderConstant("g_vCameraFrustum", sizeof(NiPoint4), &kFrustumData);
	
	// �۲����
	const NiPoint3& kCamPos = m_pkCurCam->GetWorldLocation();
	sdMatrix4 kViewMatrix(
		kCamRight.x, kCamUp.x, kCamDir.x, 0.0f,
		kCamRight.y, kCamUp.y, kCamDir.y, 0.0f,
		kCamRight.z, kCamUp.z, kCamDir.z, 0.0f,
		-kCamPos.Dot(kCamRight), -kCamPos.Dot(kCamUp), -kCamPos.Dot(kCamDir), 1.0f);

	// �����任����
	NiPoint3 kRight, kUp, kDir;
	if (kCamFrustum.m_bOrtho)
	{
		kRight = kCamRight * kCamFrustum.m_fRight;
		kUp = kCamUp * kCamFrustum.m_fTop;
		kDir = kCamDir * kCamFrustum.m_fFar;
	}
	else
	{
		kRight = kCamRight * kCamFrustum.m_fRight * kCamFrustum.m_fFar;
		kUp = kCamUp * kCamFrustum.m_fTop * kCamFrustum.m_fFar;
		kDir = kCamDir * kCamFrustum.m_fFar;
	}

	sdMatrix4 kDepthToWorldMatrix(
		kRight.x,	kRight.y,	kRight.z,	0.0f,
		kUp.x,		kUp.y,		kUp.z,		0.0f,
		kDir.x,		kDir.y,		kDir.z,		0.0f,
		kCamPos.x,	kCamPos.y,	kCamPos.z,	1.0f);
	
	sdMatrix4 kDepthToViewMatrix = kDepthToWorldMatrix * kViewMatrix;

	pkRenderDevice->SetGlobalShaderConstant("g_mDepthToWorld", sizeof(kDepthToWorldMatrix), &kDepthToWorldMatrix);
	pkRenderDevice->SetGlobalShaderConstant("g_mDepthToView", sizeof(kDepthToViewMatrix), &kDepthToViewMatrix);
	// @}

	
	// �ƹ�
	// @{
	bool bMainLight		= m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_MAINLIGHT);
	bool bAssistLight	= m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_ASSISTLIGHT);
	bool bAmbientLight	= m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_AMBIENTLIGHT);
	bool bLocalLight	= m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_LOCALLIGHT);

	// �۲�������ת����(D3D�۲�����ϵ,����ΪX,����ΪY,��ǰΪZ)
	sdMatrix3 kViewRotationMatrix(
		kCamRight.x, kCamUp.x, kCamDir.x,
		kCamRight.y, kCamUp.y, kCamDir.y,
		kCamRight.z, kCamUp.z, kCamDir.z);

	// ����
	if (bMainLight)
	{
		sdLight* pkMainLight = m_pkCurMap->GetMainLight();
		NIASSERT(pkMainLight);

		NiPoint3 kDir;
		const NiMatrix3& kRotate = pkMainLight->GetWorldRotate();
		kRotate.GetCol(0, kDir);

		sdVector3 kMainLightDir(kDir.x, kDir.y, kDir.z);
		sdVector3 kMainLightViewDir;
		sdMath::Mult(kMainLightViewDir, kMainLightDir, kViewRotationMatrix);
		sdMath::Normalize(kMainLightViewDir);
		pkRenderDevice->SetGlobalShaderConstant("g_vMainLightDir", sizeof(kMainLightViewDir), &kMainLightViewDir);
		
		NiColor kMainLightColor = pkMainLight->GetDiffuseColor() * pkMainLight->GetDimmer();
		NiColor kMainLightAmbient = pkMainLight->GetAmbientColor() * pkMainLight->GetDimmer();
		pkRenderDevice->SetGlobalShaderConstant("g_vMainLightColor", sizeof(kMainLightColor), &kMainLightColor);
		pkRenderDevice->SetGlobalShaderConstant("g_vMainLightAmbientColor", sizeof(kMainLightAmbient), &kMainLightAmbient);
	}

	// ������
	if (bAssistLight)
	{
		sdLight* pkAssistLight = m_pkCurMap->GetAssistantLight();
		NIASSERT(pkAssistLight);

		NiPoint3 kDir;
		const NiMatrix3& kRotate = pkAssistLight->GetWorldRotate();
		kRotate.GetCol(0, kDir);

		sdVector3 kAssistLightDir(kDir.x, kDir.y, kDir.z);
		sdVector3 kAssistLightViewDir;
		sdMath::Mult(kAssistLightViewDir, kAssistLightDir, kViewRotationMatrix);
		sdMath::Normalize(kAssistLightViewDir);
		pkRenderDevice->SetGlobalShaderConstant("g_vAssistLightDir", sizeof(kAssistLightViewDir), &kAssistLightViewDir);
		
		NiColor kAssistLightColor = pkAssistLight->GetDiffuseColor() * pkAssistLight->GetDimmer();
		NiColor kAssistLightAmbient = pkAssistLight->GetAmbientColor() * pkAssistLight->GetDimmer();	
		pkRenderDevice->SetGlobalShaderConstant("g_vAssistLightColor", sizeof(kAssistLightColor), &kAssistLightColor);
		pkRenderDevice->SetGlobalShaderConstant("g_vAssistLightAmbientColor", sizeof(kAssistLightAmbient), &kAssistLightAmbient);
	}

	// ������
	if (bAmbientLight)
	{
		sdLight* pkAmbientLight = m_pkCurMap->GetAmbientLight();
		NIASSERT(pkAmbientLight);

		NiColor kTerrainAmbientColor = pkAmbientLight->GetDiffuseColor() * pkAmbientLight->GetDimmer();
		NiColor kSkyAmbientColor = pkAmbientLight->GetAmbientColor() * pkAmbientLight->GetDimmer();
		pkRenderDevice->SetGlobalShaderConstant("g_vTerraimAmbientColor", sizeof(kTerrainAmbientColor), &kTerrainAmbientColor);
		pkRenderDevice->SetGlobalShaderConstant("g_vSkyAmbientColor", sizeof(kSkyAmbientColor), &kSkyAmbientColor);
	}

	//
	NiPoint4 kLightFactor(1.0f, 1.0f, 1.0f, 1.0f);
	pkRenderDevice->SetGlobalShaderConstant("g_vLightFactor", sizeof(kLightFactor), &kLightFactor);
	// }@

	// ���β���
	// @{
	// ���γߴ���Ϣ
	float fTerrainSize[4] = {(float)m_kTerrainParams.terrainSize, (float)m_kTerrainParams.blendMapSize, (float)m_kTerrainParams.tileMapSize, 0.f};
	pkRenderDevice->SetGlobalShaderConstant("g_vTerrainMapSize", sizeof(fTerrainSize), &(fTerrainSize[0]));
	
	float fRecipTerrainSize[4] = {1.f/m_kTerrainParams.terrainSize, 1.f/m_kTerrainParams.blendMapSize, 1.f/m_kTerrainParams.tileMapSize, 0.f};
	pkRenderDevice->SetGlobalShaderConstant("g_vRecipTerrainMapSize", sizeof(fRecipTerrainSize), &(fRecipTerrainSize[0]));

	// ���θ߶�ͼ�߶ȷ�Χ
	float fTerrainHeightRange[2] = { m_kTerrainParams.minHeight, m_kTerrainParams.maxHeight};
	pkRenderDevice->SetGlobalShaderConstant("g_vTerrainHeightRange", sizeof(fTerrainHeightRange), &(fTerrainHeightRange[0]));

	// ���β���
	pkRenderDevice->SetGlobalShaderConstant("g_vTerrainDiffuseMaterial", sizeof(m_kTerrainParams.diffuseMaterial), &(m_kTerrainParams.diffuseMaterial.m_fX));
	pkRenderDevice->SetGlobalShaderConstant("g_vTerrainSpecularMaterial", sizeof(m_kTerrainParams.specularMaterial), &(m_kTerrainParams.specularMaterial.m_fX));
	pkRenderDevice->SetGlobalShaderConstant("g_fTerrainShiness", sizeof(m_kTerrainParams.shiness), &(m_kTerrainParams.shiness));
	
	// ����DiffuseAtlas/NormalAtlas��Ϣ
	pkRenderDevice->SetGlobalShaderConstant("g_vDiffuseAtlasTableParam", sizeof(m_kTerrainParams.diffuseAtlasTableParam), &(m_kTerrainParams.diffuseAtlasTableParam));
	pkRenderDevice->SetGlobalShaderConstant("g_vNormalAtlasTableParam", sizeof(m_kTerrainParams.normalAtlasTableParam), &(m_kTerrainParams.normalAtlasTableParam));
	
	// ��Ļÿ�����ض�Ӧ������ռ���Զ�ü���ĳߴ�(��˫������Ƶ�ƽ��)
	sdVector2 kFarPlanePixelSize;
	if (kCamFrustum.m_bOrtho)
	{
		kFarPlanePixelSize.m_fX = (kCamFrustum.m_fRight - kCamFrustum.m_fLeft) / (float)uiWidth;
		kFarPlanePixelSize.m_fY = (kCamFrustum.m_fTop - kCamFrustum.m_fBottom) / (float)uiHeight;
	}
	else
	{
		kFarPlanePixelSize.m_fX = (kCamFrustum.m_fRight - kCamFrustum.m_fLeft) * kCamFrustum.m_fFar / (float)uiWidth;
		kFarPlanePixelSize.m_fY = (kCamFrustum.m_fTop - kCamFrustum.m_fBottom) * kCamFrustum.m_fFar / (float)uiHeight;
	}
	pkRenderDevice->SetGlobalShaderConstant("g_vFarPixelSize", sizeof(kFarPlanePixelSize), &kFarPlanePixelSize);
	
	// ���ν�Զƽ��ֽ����
	pkRenderDevice->SetGlobalShaderConstant("g_fTerrainFarStart", sizeof(m_kTerrainParams.terrainFarStart), &m_kTerrainParams.terrainFarStart);
	
	// ���ε�����Ϣ
	if (true)	///< ���ǽ����ڱ༭״̬������
	{
		sdVector4 kTerrainDebugParam = sdVector4::ZERO;
		if (m_kTerrainParams.showInvisiableLayers)
			kTerrainDebugParam.m_fX = 1.f;
		if (m_kTerrainParams.showTileGrid)
			kTerrainDebugParam.m_fY = 1.f;
		if (m_kTerrainParams.showCurrentLayer)
			kTerrainDebugParam.m_fZ = 1.f;
		kTerrainDebugParam.m_fW = (float)m_kTerrainParams.currentLauerIndex;

		pkRenderDevice->SetGlobalShaderConstant("g_vTerrainDebugParams", sizeof(kTerrainDebugParam), &kTerrainDebugParam);
	}
	// @}


	// ����
	// @{


	// @}

	// ������Ч
	// @{
	// MLAA
	if (m_kPostProcessParams.enableMLAA)
	{
		pkRenderDevice->SetGlobalShaderConstant("g_fEdgeThreshold", sizeof(float), &m_kPostProcessParams.edgeThreshold);
	}

	//if (m_kPostProcessParams.enableColorGrading)
	{
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAAColorForegroundDepth", sizeof(float), &m_kPostProcessParams.colorForegroundDepth);
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAAColorBackgroundDepth", sizeof(float), &m_kPostProcessParams.colorBackgroundDepth);
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAAColorSkyDepth", sizeof(float), &m_kPostProcessParams.colorSkyDepth);
	}

	//if (m_kPostProcessParams.enableDof)
	{
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAANoBlurDepth", sizeof(float), &m_kPostProcessParams.dofNoBlurDepth);
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAAFullBlurDepth", sizeof(float), &m_kPostProcessParams.dofFullBlurDepth);
		pkRenderDevice->SetGlobalShaderConstant("g_fMLAADofSkyDepth", sizeof(float), &m_kPostProcessParams.dofSkyDepth);
	}

	//if (m_kPostProcessParams.enableGodRay)
	{
		pkRenderDevice->SetGlobalShaderConstant("g_vGodRayDirection", sizeof(NiPoint3), &m_kPostProcessParams.godrayDirection);
		pkRenderDevice->SetGlobalShaderConstant("g_vGodRayColor", sizeof(NiColor), &m_kPostProcessParams.godrayColor);
		pkRenderDevice->SetGlobalShaderConstant("g_vGodRaySize", sizeof(NiPoint2), &m_kPostProcessParams.godraySize);
		pkRenderDevice->SetGlobalShaderConstant("g_fGodRayIntensity", sizeof(float), &m_kPostProcessParams.godrayIntensity);
	}

	// 
	pkRenderDevice->SetGlobalShaderConstant("g_bDof", sizeof(bool), &m_kPostProcessParams.enableDof);	
	pkRenderDevice->SetGlobalShaderConstant("g_bGodRay", sizeof(bool), &m_kPostProcessParams.enableGodRay);
	pkRenderDevice->SetGlobalShaderConstant("g_bRedAlert", sizeof(bool), &m_kPostProcessParams.enableRedAlert);
	pkRenderDevice->SetGlobalShaderConstant("g_bDeadEffect", sizeof(bool), &m_kPostProcessParams.enableDeadEffect);
	// @}

	// @{
	//
	NiPoint4 kNormalScale(1.0f, 1.0f, 1.0f, 1.0f);
	pkRenderDevice->SetGlobalShaderConstant("g_vNormalScale", sizeof(kNormalScale), &kNormalScale);

	// �༭��״̬�µ�һЩĬ��ֵ
	sdVector3 kVertexColorMask = sdVector3::ZERO;
	sdVector3 kDiffuseMask = sdVector3::ZERO;
	float fGlossMapMask = 1.0f;
	float fLightMapMask = 1.0f;
	
	if (!m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_DIFFUSEMAP))	kDiffuseMask = sdVector3::UNIT_SCALE;
	if (!m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_GLOSSMAP))	fGlossMapMask = 1.0f;
	if (!m_kRenderParams.IsEnableChannel(sdRenderParams::E_BUILDING, sdRenderParams::E_LIGHTMAP))	fLightMapMask = 1.0f;

	pkRenderDevice->SetGlobalShaderConstant("g_vVertexColorMask", sizeof(kVertexColorMask), &kVertexColorMask);
	pkRenderDevice->SetGlobalShaderConstant("g_vDiffuseMapMask", sizeof(kDiffuseMask), &kDiffuseMask);
	pkRenderDevice->SetGlobalShaderConstant("g_fGlossMapMask", sizeof(fGlossMapMask), &fGlossMapMask);
	pkRenderDevice->SetGlobalShaderConstant("g_fLightMapMask", sizeof(fLightMapMask), &fLightMapMask);
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawEarlyZPass()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ����Ŀ�껺��
	pkRenderDevice->SetRenderTargetGroup(m_pkCurFinalRenderTarget);

	// ��ջ���(��Ȼ����ģ�建��)
	float	fFarDepth		= 1.0f;
	uint	uiBlackStencil	= 0x0;
	pkRenderDevice->Clear(NULL, &fFarDepth, &uiBlackStencil);

	// ����
	m_spEarlyZPass->Draw();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawTerrainDepthPass()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ����Ŀ�껺��(������Ȼ���)
	pkRenderDevice->SetRenderTargetGroup(m_spTerrainDepthTarget);

	// ��ջ���(��ɫ����,��������Ȼ���)
	uint	uiBlackColor	= 0x00000000;
	float	fFarDepth		= 1.0f;
	uint	uiBlackStencil	= 0x0;
	pkRenderDevice->Clear(&uiBlackColor, &fFarDepth, &uiBlackStencil);

	// ����TerrainMesh
	m_spTerrainDepthPass->Draw();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawGeometryPasses()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ����Ŀ�껺��(���λ���)
	pkRenderDevice->SetRenderTargetGroup(m_spGeometryAlbedoMRT);

	// ��ջ���(��ɫ����)
	uint uiBlackColor = 0x00000000;
	pkRenderDevice->Clear(&uiBlackColor, NULL, NULL);

	// ����StaticMesh
	m_spMRTGeometryPass->Draw();
	
	// ����SkinnedMesh

	// ����Doodads

	// ����Terrain
	m_spTerrainAtlasGeometryPass->Draw();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawPreLightPass()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ����Ŀ�껺��(���ջ���)
	pkRenderDevice->SetRenderTargetGroup(m_spLightTarget);

	// ��ջ���(��ɫ����)
	uint uiBlackColor = 0x00000000;
	pkRenderDevice->Clear(&uiBlackColor, NULL, NULL);

	// ���㱾�ع���
	m_spPreLightPass->Draw();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawShadingPasses()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ����Ŀ�껺��(���λ���)
	pkRenderDevice->SetRenderTargetGroup(m_spShadingTarget);

	// ��ջ���(��ɫ����)
	NiColor& kFogColor = m_kEnvironmentParams.fogColor;
	uint uiFogR = (uint)(kFogColor.r * 255);
	uint uiFogG = (uint)(kFogColor.g * 255);
	uint uiFogB = (uint)(kFogColor.b * 255);
	uint uiD3DFogColor = (uiFogR << 16) | (uiFogG << 8) | uiFogB;
	pkRenderDevice->Clear(&uiD3DFogColor, NULL, NULL);

	// ��ɫ����
	m_spTerrainAtlasShadingPass->Draw();

	// ��ɫ��·

	// ��ɫStaticMesh
	m_spMRTShadingPass->Draw();

	// ��ɫSkinnedMesh

	// ��ɫDecal
	m_spProjectorPass->Draw();

	// ��ɫDecalShadow

	// ��ɫ�ر�Doodads

	// ��ɫ����
	// @{
	// ��ɫ��պ�
	//sdSkyBox* pkSkyBox = m_pkCurMap->GetSkyBox();
	//NIASSERT(pkSkyBox);
	//pkSkyBox->Render();

	// ��ɫ��������
	//sdBGScroll* pkBGScroll = m_pkCurMap->GetBGScroll();
	//NIASSERT(pkBGScroll);
	//pkBGScroll->Render();

	// ��պ���
	//sdCloud* pkCloudDome = m_pkCurMap->GetCloudDome();
	//NIASSERT(pkCloudDome);
	//pkCloudDome->Render();

	// ���ƻ�����Ч
	//Rain
	//Snow
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawPostProcessPasses()
{
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderedTexture* spColorBuffer = m_spShadingTexture;

	///< 1.MLAA Pass
	///<	- ��ColorBuffer����AA
	///<	- ��Dof/ColorGradingȨֵ����AA
	NiTexture* spWeightBuffer = pkRenderSystem->GetDefaultTexture(IRenderSystem::E_DT_ERROE_TEXTURE);  
	if (m_kPostProcessParams.enableMLAA)
	{
		// Color Copy
		// (����Ҫ���Target)
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spLightTarget);
		m_spFinalPass->SetCopyInputTexture(m_spShadingTexture);
		m_spFinalPass->Copy();
		// @}

		// MLAA Edge Detect
		// @{
		uint uiD3DColor = 0x0;
		pkRenderDevice->SetRenderTargetGroup(m_spAlbedoTarget);
		pkRenderDevice->Clear(&uiD3DColor, NULL, &uiD3DColor);
		//m_pkMLAAPass->SetColorEdgeDetectInputTexture(m_spShadingTexture);
		//m_pkMLAAPass->ColorEdgeDetect();
		m_spMLAAPass->SetDepthEdgeDetectInputTexture(m_spGeometryTexture);
		m_spMLAAPass->DepthEdgeDetect();
		// @}

		// MLAA Belnd-Weight Calc
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spGlossTarget);
		pkRenderDevice->Clear(&uiD3DColor, NULL, NULL);
		m_spMLAAPass->SetBlendWeightCalcInputTexture(m_spAlbedoTexture);
		m_spMLAAPass->BlendWeightCalc();
		// @}

		// MLAA Neighborhood Blend
		// (ע�ⲻҪ���Target,MLAAֻ�����˱�Ե)
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spLightTarget);
		m_spMLAAPass->SetNeighborhoodBlendInputTexture(m_spShadingTexture, m_spGlossTexture);
		m_spMLAAPass->NeighborhoodBlend();
		// @}

		// Post-Weight Calc
		// (����Ҫ���Target)
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spShadingTarget);
		pkRenderDevice->Clear(&uiD3DColor, NULL, NULL);
		m_spMLAAPass->SetPostWeightCalculationInputTexture(m_spGeometryTexture);
		m_spMLAAPass->PostWeightCalculation();
		// @}

		// Post-Weight Copy
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spGeometryTarget);
		m_spFinalPass->SetCopyInputTexture(m_spShadingTexture);
		m_spFinalPass->Copy();
		// @}

		// Post-Weight Neighborhood Blend
		// (ע�ⲻҪ���Target,AAֻ�����˱�Ե)
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spShadingTarget);
		m_spMLAAPass->SetNeighborhoodBlendInputTexture(m_spGeometryTexture, m_spGlossTexture);
		m_spMLAAPass->NeighborhoodBlend();

		spColorBuffer = m_spLightTexture;
		spWeightBuffer = m_spShadingTexture;
		// @}
	}
	else
	{
		// Post-Weight Calc
		// @{
		uint uiD3DColor = 0x0;
		pkRenderDevice->SetRenderTargetGroup(m_spLightTarget);
		pkRenderDevice->Clear(&uiD3DColor, NULL, NULL);
		m_spMLAAPass->SetPostWeightCalculationInputTexture(m_spGeometryTexture);
		m_spMLAAPass->PostWeightCalculation();
		// @}

		spColorBuffer = m_spShadingTexture;
		spWeightBuffer = m_spLightTexture;
	}

	///< 2.Color Grading Pass
	if (m_kPostProcessParams.enableColorGrading)
	{
		pkRenderDevice->SetRenderTargetGroup(m_spAlbedoTarget);
		m_spColorGradingPass->SetInputGeomTexture(spWeightBuffer);
		m_spColorGradingPass->SetInputSceneTexture(spColorBuffer);		
		m_spColorGradingPass->Draw();

		spColorBuffer = m_spAlbedoTexture;
	}

	///< 3.Dof Pass
	NiTexture* spDofBuffer = pkRenderSystem->GetDefaultTexture(IRenderSystem::E_DT_ERROE_TEXTURE); 
	if (m_kPostProcessParams.enableDof)
	{
		// Down-Sample
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spHalfSizeTarget);
		m_spDofPass->SetDownSampleInputTexture(spColorBuffer);
		m_spDofPass->DownSample();
		// @}
		
		// BlurX
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spDofBlurXTarget);
		m_spDofPass->SetDownSampleInputTexture(m_spHalfSizeTexture);
		m_spDofPass->BlurX();
		// @}

		// BlurY
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spDofBlurYTarget);
		m_spDofPass->SetDownSampleInputTexture(m_spDofBlurXTexture);
		m_spDofPass->BlurY();
		// @}

		spDofBuffer = m_spDofBlurYTexture;
	}

	///< 4.GodRay Pass
	NiTexture* spGodRayBuffer = pkRenderSystem->GetDefaultTexture(IRenderSystem::E_DT_ERROE_TEXTURE); 
	if (m_kPostProcessParams.enableGodRay)
	{
		// Sun
		// @{
		sdSun* pkSun = m_pkCurMap->GetSun();
		NIASSERT(pkSun);

		uint uiD3DColor = 0x0;
		pkRenderDevice->SetRenderTargetGroup(m_spQuaterSizeTarget[0]);
		pkRenderDevice->Clear(&uiD3DColor, NULL, NULL);
		pkSun->SetDepthTexture(spWeightBuffer);
		pkSun->Render();
		// @}

		// Radial Blur
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spQuaterSizeTarget[1]);
		m_spGodRayPass->SetRadialBlurInputTexture(m_spQuaterSizeTexture[0]);
		m_spGodRayPass->RadialBlur();

		pkRenderDevice->SetRenderTargetGroup(m_spQuaterSizeTarget[0]);
		m_spGodRayPass->SetRadialBlurInputTexture(m_spQuaterSizeTexture[1]);
		m_spGodRayPass->RadialBlur();

		pkRenderDevice->SetRenderTargetGroup(m_spQuaterSizeTarget[1]);
		m_spGodRayPass->SetRadialBlurInputTexture(m_spQuaterSizeTexture[0]);
		m_spGodRayPass->RadialBlur();
		// @}

		// Flare
		// @{
		sdFlare* pkFlare = m_pkCurMap->GetFlare();
		NIASSERT(pkFlare);
		pkFlare->SetDepthTexture(spWeightBuffer);
		pkFlare->Render();
		// @}

		spGodRayBuffer = m_spQuaterSizeTexture[1];
	}

	///< 5.Compose Pass
	NiTexture* spBloomBuffer = pkRenderSystem->GetDefaultTexture(IRenderSystem::E_DT_ERROE_TEXTURE);
	{
		pkRenderDevice->SetRenderTargetGroup(m_spGlossTarget);
		m_spFinalPass->SetCompositeInputtexture_ColorTexture(spColorBuffer);
		m_spFinalPass->SetCompositeInputtexture_WeightTexture(spWeightBuffer);
		m_spFinalPass->SetCompositeInputtexture_DofTexture(spDofBuffer);
		m_spFinalPass->SetCompositeInputtexture_GodRayTexture(spGodRayBuffer);
		m_spFinalPass->SetCompositeInputtexture_BloomTexture(spBloomBuffer);
		m_spFinalPass->Composite();

		spColorBuffer = m_spGlossTexture;
	}

	///< 6.HDR Pass, Bloom and Tone Mapping
	if (m_kPostProcessParams.enableHDR)
	{
		// Down-Scale, 1-->1/2-->1/4-->1/8
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spHDRHalfSizeScaleTarget);
		m_spHDRPass->SetDownScaleInputTexture(spColorBuffer);
		m_spHDRPass->DownScale();

		pkRenderDevice->SetRenderTargetGroup(m_spHDRQuaterSizeScaleTarget);
		m_spHDRPass->SetDownScaleInputTexture(m_spHDRHalfSizeScaleTexture);
		m_spHDRPass->DownScale();

		pkRenderDevice->SetRenderTargetGroup(m_spHDROctantSizeScaleTarget);
		m_spHDRPass->SetDownScaleInputTexture(m_spHDRQuaterSizeScaleTexture);
		m_spHDRPass->DownScale();
		// @}

		// Scene-Luminance, 1/8-->16x16
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_kHDRIntermediateLuminanceTarget[0]);
		m_spHDRPass->SetSceneLuminanceInputTexture(m_spHDROctantSizeScaleTexture);
		m_spHDRPass->SceneLuminance();
		// @}

		// Down-Scale Scene-Luminance, 16x16-->4x4-->1x1
		// @{
		for (uint i = 0; i < m_kHDRIntermediateLuminanceTarget.size(); i++)
		{
			if (i == m_kHDRIntermediateLuminanceTarget.size()-1)
			{
				pkRenderDevice->SetRenderTargetGroup(m_spHDRCurLuminanceTarget);
				m_spHDRPass->SetDownScaleSceneLuminanceInputTexture(m_kHDRIntermediateLuminanceTexture[i]);
				m_spHDRPass->DownScaleSceneLuminance();
			}
			else
			{
				pkRenderDevice->SetRenderTargetGroup(m_kHDRIntermediateLuminanceTarget[i+1]);
				m_spHDRPass->SetDownScaleSceneLuminanceInputTexture(m_kHDRIntermediateLuminanceTexture[i]);
				m_spHDRPass->DownScaleSceneLuminance();
			}
		}
		// @}

		// Adapted Luminance
		// @{
		static bool bReset = true;
		if (bReset)
		{
			pkRenderDevice->SetRenderTargetGroup(m_spHDRCurAdaptedLuminanceTarget);
			m_spHDRPass->SetInitLuminanceInputTexture(m_spHDRCurLuminanceTexture);
			m_spHDRPass->InitLuminance();

			bReset = false;
		}
		else
		{
			pkRenderDevice->SetRenderTargetGroup(m_spHDRCurAdaptedLuminanceTarget);
			m_spHDRPass->SetAdaptedLuminanceInputTexture_Cur(m_spHDRCurLuminanceTexture);
			m_spHDRPass->SetAdaptedLuminanceInputTexture_Last(m_spHDRLastLuminanceTexture);
			m_spHDRPass->AdaptedLuminance(sdTimeMgr::Instance().GetFrameTime());
		}
		// @}

		// Bloom Threshold
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spHDRBloomThresholdTarget);
		m_spHDRPass->SetBloomThresholdInputTexture_Color(m_spHDROctantSizeScaleTexture);
		m_spHDRPass->SetBloomThresholdInputTexture_Luminance(m_spHDRCurAdaptedLuminanceTexture);
		m_spHDRPass->BloomThreshold();
		// @}

		// Bloom Blur X
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spHDRBloomBlurXTarget);
		m_spHDRPass->SetBloomBlurXInputTexture(m_spHDRBloomThresholdTexture);
		m_spHDRPass->BloomBlurX();
		// @}

		// Bloom Blur Y
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spHDRBloomBlurYTarget);
		m_spHDRPass->SetBloomBlurXInputTexture(m_spHDRBloomBlurXTexture);
		m_spHDRPass->BloomBlurY();
		// @}

		// Tone Mapping
		// @{
		pkRenderDevice->SetRenderTargetGroup(m_spGeometryTarget);
		m_spHDRPass->SetToneMappingInputTexture_Color(spColorBuffer);
		m_spHDRPass->SetToneMappingInputTexture_Bloom(m_spHDRBloomBlurYTexture);
		m_spHDRPass->SetToneMappingInputTexture_Luminance(m_spHDRCurAdaptedLuminanceTexture);
		m_spHDRPass->ToneMapping();
		// @}

		// �л�
		// @{
		NiRenderTargetGroupPtr spTempTarget = m_spHDRCurAdaptedLuminanceTarget;
		m_spHDRCurAdaptedLuminanceTarget = m_spHDRLastLuminanceTarget;
		m_spHDRLastLuminanceTarget = spTempTarget;

		NiRenderedTexturePtr spTempTexture = m_spHDRCurAdaptedLuminanceTexture;
		m_spHDRCurAdaptedLuminanceTexture = m_spHDRLastLuminanceTexture;
		m_spHDRLastLuminanceTexture = spTempTexture;
		// @}

		spColorBuffer = m_spGeometryTexture;
	}

	///< 7.HUD Pass


	///< 8.Final Pass
	// @{
	uint uiD3DColor = 0x0;
	pkRenderDevice->SetRenderTargetGroup(m_pkCurFinalRenderTarget);
	pkRenderDevice->Clear(&uiD3DColor, NULL, NULL);
	m_spFinalPass->SetCopyToBackBufferInputTexture_ColorTexture(spColorBuffer);
	m_spFinalPass->CopyToBackBuffer();
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::DrawEdge()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

//	// ����Ŀ�껺��
//	pkRenderDevice->SetRenderTargetGroup(m_spAlbedoTarget);

//	// ��ջ���(��ɫ����)
//	uint uiBlackColor = 0x00000000;
//	pkRenderDevice->Clear(&uiBlackColor, NULL, NULL);

//	// ����
//	m_spEdgePass->Draw();

//	// ����Ŀ�껺��
//	pkRenderDevice->SetRenderTargetGroup(m_pkCurFinalRenderTarget);
//	pkRenderDevice->Clear(&uiBlackColor, NULL, NULL);

//	// ������ǿ
//	m_spEdgeEnhancementPass->Draw();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE