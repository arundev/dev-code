//*************************************************************************************************
// 内容:	勾边增强
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-07
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_EDGE_ENHANCEMENT_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_EDGE_ENHANCEMENT_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdEdgeEnhancementPass : public sdRenderPass
{
public:
	sdEdgeEnhancementPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdEdgeEnhancementPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, NiRenderedTexture* pkEdgeTexture);
	void Destroy();

	// 添加绘制对象
	void InsertEdgeEnhancementArea(const NiBound& kBound, const NiColor& kColor, uint uiWidth);

protected:
	// 创建代理渲染对象
	void CreateProxyMesh();

	// 根据屏幕尺寸计算过滤算子
	void SetShaderParams(uint uiScreenWidth, uint uiScreenHeight);

	// 计算高斯分布
	float GaussianDistribution(float x, float y, float rho);

protected:
	uint m_uiEdgeStencilID;					///< 勾边模板参考值
	uint m_uiEdgeStencilIDMask;				///< 勾边模板掩码
	
	NiMaterialPtr m_spEdgeMaterial;			///< 材质
	NiRenderedTexturePtr m_spEdgeTexture;	///< 纹理
	NiMeshPtr m_spProxyMesh;				///< 代理Mesh

//	NiFloatsExtraDataPtr m_spEdgeParam;				///< 勾边参数数据
//	NiFloatsExtraDataPtr m_spSampleOffsetsParam;	///< 采样偏移数据
//	NiFloatsExtraDataPtr m_spSampleWeightsParam;	///< 采样权重数据

	NiPoint2 m_akSampleOffsets[13];			///< 采样偏移
	float	 m_afSampleWeights[13];			///< 采样权重
};
NiSmartPointer(sdEdgeEnhancementPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif