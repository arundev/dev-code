//*************************************************************************************************
// ����:	Jimenez's MLAA�����
//---------------------------------------------------------
// ����:		
// ����:		2012-10-13
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_MLAA_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_MLAA_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdMLAAPass : public sdRenderPass
{
public:
	sdMLAAPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdMLAAPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// MLAA���������Pass����
	// @{
	// 1.Edge Detect
	void SetColorEdgeDetectInputTexture(NiTexture* spColorTexture);
	void ColorEdgeDetect();

	void SetDepthEdgeDetectInputTexture(NiTexture* spDepthTexture);
	void DepthEdgeDetect();

	// 2.Blend Weight Calc
	void SetBlendWeightCalcInputTexture(NiTexture* spEdgeTexture);
	void BlendWeightCalc();

	// 3.Neighborhood Blend
	void SetNeighborhoodBlendInputTexture(NiTexture* spColorTexture, NiTexture* spBlendTexture);
	void NeighborhoodBlend();
	// @}

	// Geometry PostWeight��������Pass����
	// @{
	// Post Weight Calc
	void SetPostWeightCalculationInputTexture(NiTexture* spGeometryTexture);
	void PostWeightCalculation();

	//// Post Weight Blending(ò��ľ��ʹ��)
	//void SetPostWeightBlendInputTexture(NiTexture* spColorTexture, NiTexture* spBlendTexture);
	//void PostWeightBlend();
	// @}

protected:
	// ����
	NiMaterialPtr	m_spColorEdgeDetectMaterial;
	NiMaterialPtr	m_spDepthEdgeDetectMaterial;
	NiMaterialPtr	m_spWeightCalcMaterial;
	NiMaterialPtr	m_spNeighborBlendMaterial;

	NiMaterialPtr	m_spPostWeightCalculationMaterial;
	NiMaterialPtr	m_spPostWeightBlendingMaterial;

	// ��Ⱦ����
	NiPropertyList	m_kColorEdgeDetectPropertyList;
	NiPropertyList	m_kDepthEdgeDetectPropertyList;
	NiPropertyList	m_kWeightCalcPropertyList;
	NiPropertyList	m_kNeighborBlendPropertyList;
	NiPropertyList	m_kPostWeightCalculationPropertyList;
	NiPropertyList	m_kPostWeightBlendingPropertyList;

	// ��������
	NiTexturingPropertyPtr	m_spColorEdgeDetectTexProp;
	NiTexturingPropertyPtr	m_spDepthEdgeDetectTexProp;
	NiTexturingPropertyPtr	m_spWeightCalcTexProp;
	NiTexturingPropertyPtr	m_spNeighborBlendTexProp;
	NiTexturingPropertyPtr	m_spPostWeightCalculationTexProp;
	NiTexturingPropertyPtr	m_spPostWeightBlendingTexProp;

	// MLAA��Area����,���ڻ��Ȩֵ��ѯ
	NiTexturePtr	m_spAreaTexture;
};
NiSmartPointer(sdMLAAPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif