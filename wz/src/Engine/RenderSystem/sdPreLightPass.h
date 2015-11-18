//*************************************************************************************************
// ����:	�ӳ�Light����Ⱦ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-20
// ����޸�:	2014-04-11
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_PRELIGHT_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_PRELIGHT_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPreLightPass : public sdRenderPass
{
public:
	sdPreLightPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdPreLightPass();

	// �麯���̳�(�̳���sdRenderPass)
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(
		uint uiLightVolumeStencilID, 
		uint uiLightVolumeStencilIDMask,
		uint uiLightVolumeStencilIDWriteMask,
		NiRenderedTexture* pkGeomTexture);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertLight(sdLightSceneObject* pkLight);

protected:
	uint m_uiLightVolumeStencilID;				///< �ƹ���ģ��ģ��ο�ֵ
	uint m_uiLightVolumeStencilIDMask;			///< �ƹ���ģ��ģ������
	uint m_uiLightVolumeStencilIDWriteMask;		///< �ƹ���ģ��ģ��д������

	NiMaterialPtr	m_spPointLightMaterial;		///< ���Դ����
	NiMaterialPtr	m_spSpotLightMaterial;		///< �۹�Ʋ���
	NiMaterialPtr	m_spRectSpotLightMaterial;	///< �۹�Ʋ���

	NiRenderedTexturePtr m_spGeometryTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdPreLightPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif