//*************************************************************************************************
// ����:	Terrain��Geometry-Buffer����Ⱦ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-06
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_GEOMETRY_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_GEOMETRY_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ������Ⱦ�����λ���
//-------------------------------------------------------------------------------------------------
class sdTerrainAtlasGeometryPass : public sdRenderPass
{
public:
	sdTerrainAtlasGeometryPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdTerrainAtlasGeometryPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(
		uint uiTerrainStencilID, 
		uint uiTerrainStencilIDMask,
		uint uiFarTerrainStencilIDShift,
		NiRenderedTexture* spDepthTexture);
	void Destroy();

	// ���õ�����Ⱦ��Ϣ(���ڸ����ڲ���һЩ��ͼ��Ϣ)
	void SetTerrainParams(const sdTerrainParams& kTerrainParams);

protected:
	// ģ�����
	uint	m_uiTerrainStencilID;
	uint	m_uiTerrainStencilIDMask;
	uint	m_uiFarTerrainStencilIDShift;

	// ֻ��Ⱦ�������߲���
	NiMaterialPtr	m_spBaseNormalOnlyMaterial;

	// ͸��ͶӰ�������:
	//	1.������Ⱦ�������ߺͷ�����ͼ�Ĳ���
	//	2.Զ����Ⱦ�������ߵĲ���
	NiMaterialPtr	m_spFarBaseNormalMaterial;
	NiMaterialPtr	m_spNearNormalMapMaterial;

	// ����ͶӰ�������:
	//	1.������Ⱦ�������ߺͷ�����ͼ�Ĳ���
	//	2.Զ����Ⱦ�������ߵĲ���
	NiMaterialPtr	m_spOrthoFarBaseNormalMaterial;
	NiMaterialPtr	m_spOrthoNearNormalMapMaterial;

	// �����������
	NiRenderedTexturePtr	m_spDepthTexture;

	// ��������ͼ,����ȷ���ر���ͼͶӰ����
	NiSourceCubeMapPtr		m_spUVTableCubeMap;

	// ������Ⱦ����
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;
};
NiSmartPointer(sdTerrainAtlasGeometryPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif