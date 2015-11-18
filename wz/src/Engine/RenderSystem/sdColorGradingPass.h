//*************************************************************************************************
// ����:	Color Grading
//---------------------------------------------------------
// ����:		
// ����:		2012-10-13
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_COLORGRADING_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_COLORGRADING_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdColorGradingPass : public sdRenderPass
{
public:
	sdColorGradingPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdColorGradingPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// ��Ⱦ����
	void	SetInputSceneTexture(NiTexture* spTexture);
	void	SetInputGeomTexture(NiTexture* spTexture);

protected:
	// ����
	NiMaterialPtr			m_spColorGradingMaterial;	

	// ��Ⱦ����
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;

	// ��������
	NiTexturePtr	m_spSceneTexture;
	NiTexturePtr	m_spGeomTexture;
	NiTexturePtr	m_spBackgroundLUTTexture;	// ����LUT��������
	NiTexturePtr	m_spForegroundLUTTexture;	// ǰ��LUT��������
};
NiSmartPointer(sdColorGradingPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif