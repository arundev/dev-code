//*************************************************************************************************
// ����:	Post Fog
//---------------------------------------------------------
// ����:		
// ����:		2012-10-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_POSTFOG_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_POSTFOG_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPostFogPass : public sdRenderPass
{
public:
	sdPostFogPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdPostFogPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(NiTexture* spGeometryTexture);
	void Destroy();

protected:
	// ����
	NiMaterialPtr	m_spFogMaterial;
	NiMaterialPtr	m_spCurveFogMaterial;
	NiMaterialPtr	m_spAOAndFogMaterial;
	NiMaterialPtr	m_spAODebugMaterial;

	// ��Ⱦ����
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;
};
NiSmartPointer(sdPostFogPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif