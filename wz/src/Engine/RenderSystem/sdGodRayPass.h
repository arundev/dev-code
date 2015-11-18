//*************************************************************************************************
// ����:	GodRay
//---------------------------------------------------------
// ����:		
// ����:		2012-10-22
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_GODRAY_H__
#define _SD_ENGINE_RENDER_SYSTEM_GODRAY_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdGodRayPass : public sdRenderPass
{
public:
	sdGodRayPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdGodRayPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// ���ܲ���
	// @{
	// Radial Blur
	void SetRadialBlurInputTexture(NiTexture* spTexture);
	void RadialBlur();
	// @}

protected:
	// ����
	NiMaterialPtr	m_spRadialBlurMaterial;

	// ��Ⱦ����
	NiPropertyList	m_kPropertyList;

	// ��������
	NiTexturingPropertyPtr	m_spTexProp;
};
NiSmartPointer(sdGodRayPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif