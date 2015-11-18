//*************************************************************************************************
// ����:	Depth of Field
//---------------------------------------------------------
// ����:		
// ����:		2012-10-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_AO_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_AO_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdAOPass : public sdRenderPass
{
public:
	sdAOPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdAOPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize(sdPostProcessParams& kParams);
	void Destroy();

	// ���ܲ���
	// @{

	// @}
protected:
	// ����
	NiMaterialPtr	m_spDownSampleMaterial;		
	NiMaterialPtr	m_spOcclusionMaterial;
	NiMaterialPtr	m_spBlurMaterial;
	NiMaterialPtr	m_spApplyMaterial;

	// ��Ⱦ����
	NiPropertyList	m_kDownSamplePropertyList;
	NiPropertyList	m_kOcclusionPropertyList;
	NiPropertyList	m_kBlurPropertyList;
	NiPropertyList	m_kApplyPropertyList;

	// ��������
	NiTexturingPropertyPtr	m_spDownSampleTexProp;
	NiTexturingPropertyPtr	m_spOcclusionTexProp;
	NiTexturingPropertyPtr	m_spBlurTexProp;
	NiTexturingPropertyPtr	m_spApplyTexProp;

	// ������Ч����
	sdPostProcessParams*	m_pkPostProcessParams;
};
NiSmartPointer(sdAOPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif