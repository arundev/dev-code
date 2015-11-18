//*************************************************************************************************
// ����:	������ǿ
//---------------------------------------------------------
// ����:		
// ����:		2014-05-07
// ����޸�:	
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

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, NiRenderedTexture* pkEdgeTexture);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertEdgeEnhancementArea(const NiBound& kBound, const NiColor& kColor, uint uiWidth);

protected:
	// ����������Ⱦ����
	void CreateProxyMesh();

	// ������Ļ�ߴ�����������
	void SetShaderParams(uint uiScreenWidth, uint uiScreenHeight);

	// �����˹�ֲ�
	float GaussianDistribution(float x, float y, float rho);

protected:
	uint m_uiEdgeStencilID;					///< ����ģ��ο�ֵ
	uint m_uiEdgeStencilIDMask;				///< ����ģ������
	
	NiMaterialPtr m_spEdgeMaterial;			///< ����
	NiRenderedTexturePtr m_spEdgeTexture;	///< ����
	NiMeshPtr m_spProxyMesh;				///< ����Mesh

//	NiFloatsExtraDataPtr m_spEdgeParam;				///< ���߲�������
//	NiFloatsExtraDataPtr m_spSampleOffsetsParam;	///< ����ƫ������
//	NiFloatsExtraDataPtr m_spSampleWeightsParam;	///< ����Ȩ������

	NiPoint2 m_akSampleOffsets[13];			///< ����ƫ��
	float	 m_afSampleWeights[13];			///< ����Ȩ��
};
NiSmartPointer(sdEdgeEnhancementPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif