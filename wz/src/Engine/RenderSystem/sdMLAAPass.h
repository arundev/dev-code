//*************************************************************************************************
// 内容:	Jimenez's MLAA抗锯齿
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-13
// 最后修改:
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

	// 虚函数继承
	virtual void Draw(){};

	// 初始化与销毁
	bool Initialize();
	void Destroy();

	// MLAA各个步骤的Pass处理
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

	// Geometry PostWeight各个步骤Pass处理
	// @{
	// Post Weight Calc
	void SetPostWeightCalculationInputTexture(NiTexture* spGeometryTexture);
	void PostWeightCalculation();

	//// Post Weight Blending(貌似木有使用)
	//void SetPostWeightBlendInputTexture(NiTexture* spColorTexture, NiTexture* spBlendTexture);
	//void PostWeightBlend();
	// @}

protected:
	// 材质
	NiMaterialPtr	m_spColorEdgeDetectMaterial;
	NiMaterialPtr	m_spDepthEdgeDetectMaterial;
	NiMaterialPtr	m_spWeightCalcMaterial;
	NiMaterialPtr	m_spNeighborBlendMaterial;

	NiMaterialPtr	m_spPostWeightCalculationMaterial;
	NiMaterialPtr	m_spPostWeightBlendingMaterial;

	// 渲染属性
	NiPropertyList	m_kColorEdgeDetectPropertyList;
	NiPropertyList	m_kDepthEdgeDetectPropertyList;
	NiPropertyList	m_kWeightCalcPropertyList;
	NiPropertyList	m_kNeighborBlendPropertyList;
	NiPropertyList	m_kPostWeightCalculationPropertyList;
	NiPropertyList	m_kPostWeightBlendingPropertyList;

	// 纹理属性
	NiTexturingPropertyPtr	m_spColorEdgeDetectTexProp;
	NiTexturingPropertyPtr	m_spDepthEdgeDetectTexProp;
	NiTexturingPropertyPtr	m_spWeightCalcTexProp;
	NiTexturingPropertyPtr	m_spNeighborBlendTexProp;
	NiTexturingPropertyPtr	m_spPostWeightCalculationTexProp;
	NiTexturingPropertyPtr	m_spPostWeightBlendingTexProp;

	// MLAA的Area纹理,用于混合权值查询
	NiTexturePtr	m_spAreaTexture;
};
NiSmartPointer(sdMLAAPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif