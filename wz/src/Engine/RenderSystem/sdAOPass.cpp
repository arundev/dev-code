#include "sdRenderSystemPCH.h"
#include "sdAOPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdAOPass::sdAOPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc), m_pkPostProcessParams(NULL)
{

}
//-------------------------------------------------------------------------------------------------
sdAOPass::~sdAOPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAOPass::Initialize(sdPostProcessParams& kParams)
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	//
	m_pkPostProcessParams = &kParams;

	//// 初始化材质
	//m_spDownSampleMaterial = pkRenderDevice->CreateMaterial("PostEffect_DOF_DownSample");
	//NIASSERT(m_spDownSampleMaterial);

	//m_spSeparableBlurMaterial = pkRenderDevice->CreateMaterial("PostEffect_DOF_SeparableBlur");
	//NIASSERT(m_spSeparableBlurMaterial);

	//// 初始化渲染属性
	//m_spTexProp  = NiNew NiTexturingProperty;
	//NIASSERT(m_spTexProp);
	//m_spTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	//m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdAOPass::Destroy()
{
	//m_spDownSampleMaterial = 0;
	//m_spSeparableBlurMaterial = 0;

	//m_kPropertyList.RemoveAll();
	//m_spTexProp = 0;

	m_pkPostProcessParams = NULL;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE