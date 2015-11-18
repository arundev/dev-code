//*************************************************************************************************
// ����:	�����ɫ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-14
// ����޸�:	2014-05-05
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_MRT_SHADING_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_MRT_SHADING_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdMRTShadingPass : public sdRenderPass
{
public:
	sdMRTShadingPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdMRTShadingPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	//	@param[in]	uiStaticMeshStencilID		����Ⱦ����ģ��ID
	//	@param[in]	uiStaticMeshStencilIDMask	����Ⱦ����ģ��ID����
	//	@param[in]	spLightTexture				��Ļ�ֲ����ջ���
	//	@param[in]	spGeomTexture				��Ļ��������Ȼ���
	//	@param[in]	spAlbedoTexture				��Ļ�����仺��
	//	@param[in]	spGlossTexture				��Ļ�߹⻺��
	bool Initialize(uint uiStaticMeshStencilID, 
		uint uiStaticMeshStencilIDMask,
		NiRenderedTexture* pkLightTexture, 
		NiRenderedTexture* pkGeomTexture,
		NiRenderedTexture* pkAlbedoTexture,	
		NiRenderedTexture* pkGlossTexture, 
		NiRenderedTexture* pkGlowTexture);
	void Destroy();

protected:
	uint m_uiStaticMeshStencilID;
	uint m_uiStaticMeshStencilIDMask;

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kStaticShadingMaterials;		///< ��ɫ����

	NiRenderedTexturePtr m_spLightTexture;
	NiRenderedTexturePtr m_spGeomTexture;
	NiRenderedTexturePtr m_spAlbedoTexture;
	NiRenderedTexturePtr m_spGlossTexture;
	NiRenderedTexturePtr m_spGlowTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdMRTShadingPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif