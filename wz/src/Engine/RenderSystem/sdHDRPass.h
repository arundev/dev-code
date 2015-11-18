//*************************************************************************************************
// ����:	High Dynamic Range
//---------------------------------------------------------
// ����:		
// ����:		2012-10-24
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_HDR_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_HDR_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdHDRPass : public sdRenderPass
{
public:
	sdHDRPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdHDRPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize(sdPostProcessParams& kParams);
	void Destroy();

	// ���ܲ���
	// @{
	// Down Scale
	void SetDownScaleInputTexture(NiTexture* spTexture);
	void DownScale();

	// Scene Luminance
	void SetSceneLuminanceInputTexture(NiTexture* spTexture);
	void SceneLuminance();

	// Down Scale Scene Luminance
	void SetDownScaleSceneLuminanceInputTexture(NiTexture* spTexture);
	void DownScaleSceneLuminance();

	// Final Down Scale Scene Luminance
	void SetFinalDownScaleSceneLuminanceInputTexture(NiTexture* spTexture);
	void FinalDownScaleSceneLuminance();

	// Init Luminance
	void SetInitLuminanceInputTexture(NiTexture* spTexture);
	void InitLuminance();

	// Adapted Luminance
	void SetAdaptedLuminanceInputTexture_Cur(NiTexture* spTexture);
	void SetAdaptedLuminanceInputTexture_Last(NiTexture* spTexture);
	void AdaptedLuminance(float fFrameTime);

	// Bloom Threshold
	void SetBloomThresholdInputTexture_Color(NiTexture* spTexture);
	void SetBloomThresholdInputTexture_Luminance(NiTexture* spTexture);
	void BloomThreshold();

	// Bloom BlurX
	void SetBloomBlurXInputTexture(NiTexture* spTexture);
	void BloomBlurX();

	// Bloom BlurY
	void SetBloomBlurYInputTexture(NiTexture* spTexture);
	void BloomBlurY();

	// Tone Mapping
	void SetToneMappingInputTexture_Color(NiTexture* spTexture);
	void SetToneMappingInputTexture_Bloom(NiTexture* spTexture);
	void SetToneMappingInputTexture_Luminance(NiTexture* spTexture);
	void ToneMapping();
	// @}
protected:
	// ����
	NiMaterialPtr	m_spDownScaleMaterial;
	NiMaterialPtr	m_spSceneLuminanceMaterial;
	NiMaterialPtr	m_spDownScaleLuminanceMaterial;
	NiMaterialPtr	m_spFinalDownScaleLuminanceMaterial;
	NiMaterialPtr	m_spInitLuminanceMaterial;
	NiMaterialPtr	m_spAdaptedLuminanceMaterial;
	NiMaterialPtr	m_spBloomThresholdMaterial;
	NiMaterialPtr	m_spSeparableBlurMaterial;
	NiMaterialPtr	m_spToneMapMaterial;

	// ��Ⱦ����
	NiPropertyList	m_kDownScalePropertyList;
	NiPropertyList	m_kSceneLuminancePropertyList;
	NiPropertyList	m_kDownScaleLuminancePropertyList;
	NiPropertyList	m_kFinalDownScaleLuminancePropertyList;
	NiPropertyList	m_kInitLuminancePropertyList;
	NiPropertyList	m_kAdaptedLuminancePropertyList;
	NiPropertyList	m_kBloomThresholdPropertyList;
	NiPropertyList	m_kSeparableBlurPropertyList;
	NiPropertyList	m_kToneMapPropertyList;

	// ��������
	NiTexturingPropertyPtr	m_spDownScaleTexProp;
	NiTexturingPropertyPtr	m_spSceneLuminanceTexProp;
	NiTexturingPropertyPtr	m_spDownScaleLuminanceTexProp;
	NiTexturingPropertyPtr	m_spFinalDownScaleLuminanceTexProp;
	NiTexturingPropertyPtr	m_spInitLuminanceTexProp;
	NiTexturingPropertyPtr	m_spAdaptedLuminanceTexProp;
	NiTexturingPropertyPtr	m_spBloomThresholdTexProp;
	NiTexturingPropertyPtr	m_spSeparableBlurTexProp;
	NiTexturingPropertyPtr	m_spToneMapTexProp;

	// ������Ч����
	sdPostProcessParams*	m_pkPostProcessParams;
};
NiSmartPointer(sdHDRPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif