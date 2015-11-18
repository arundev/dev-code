//*************************************************************************************************
// ����:	Depth of Field
//---------------------------------------------------------
// ����:		
// ����:		2012-10-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_DOF_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_DOF_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdDOFPass : public sdRenderPass
{
public:
	sdDOFPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdDOFPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize(sdPostProcessParams& kParams);
	void Destroy();

	// ���ܲ���
	// @{
	// Down Sample
	void SetDownSampleInputTexture(NiTexture* spTexture);
	void DownSample();

	// BlurX
	void SetBlurXInputTexture(NiTexture* spTexture);
	void BlurX();

	// BlurY
	void SetBlurYInputTexture(NiTexture* spTexture);
	void BlurY();
	// @}
protected:
	// ����
	NiMaterialPtr	m_spDownSampleMaterial;		// 1/2���²���
	NiMaterialPtr	m_spSeparableBlurMaterial;	// һά����

	// ��Ⱦ����
	NiPropertyList	m_kPropertyList;

	// ��������
	NiTexturingPropertyPtr	m_spTexProp;

	// ������Ч����
	sdPostProcessParams*	m_pkPostProcessParams;
};
NiSmartPointer(sdDOFPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif