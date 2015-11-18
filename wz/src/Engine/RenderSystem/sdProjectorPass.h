//*************************************************************************************************
// ����:	Projector��Ⱦ
//---------------------------------------------------------
// ����:		
// ����:		2012-12-07
// ����޸�:	2014-04-20
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_PROJECTOR_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_PROJECTOR_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdProjectorPass : public sdRenderPass
{
public:
	sdProjectorPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdProjectorPass();

	// �麯���̳�(�̳���sdRenderPass)
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(
		uint uiProjectorMeshStencilID, 
		uint uiProjectorMeshStencilIDMask,
		NiRenderedTexture* spGeometryTexture,
		NiRenderedTexture* spLightTexture);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertProjector(sdProjectorSceneObject* pkProjector);

protected:
	uint m_uiProjectorMeshStencilID;			///< Ͷ����ģ��ģ��ο�ֵ
	uint m_uiProjectorMeshStencilIDMask;		///< Ͷ����ģ��ģ������

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kMaterialVec;					///< ����

	NiRenderedTexturePtr m_spGeometryTexture;
	NiRenderedTexturePtr m_spLightTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdProjectorPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif