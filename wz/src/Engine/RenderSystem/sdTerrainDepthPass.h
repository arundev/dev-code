//*************************************************************************************************
// ����:	Terrain���Ԥ��Ⱦ��ZBuffer
//---------------------------------------------------------
// ����:		
// ����:		2012-08-04
// ����޸�:	2012-12-12
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_TERRAIN_DEPTH_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_TERRAIN_DEPTH_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 1.��Ⱦ������ȵ�һ������
// 2.��Ⱦ���ε�ģ�建��
//-------------------------------------------------------------------------------------------------
class sdTerrainDepthPass : public sdRenderPass
{
public:
	sdTerrainDepthPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdTerrainDepthPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(uint uiTerraiStencilID, uint uiTerrainStencilIDWriteMask);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertTerrainMesh(NiMesh* spMesh);

protected:
	//@{
	// �麯���̳�
	virtual RenderObjectComparator GetComparator();

	// ���Ƶ�����Ƚ���
	static bool	Comparator(sdRenderObject* lhs, sdRenderObject* rhs);
	//@}

protected:
	// ģ�����
	uint	m_uiTerrainStencilID;
	uint	m_uiTerrainStencilIDWriteMask;

	// ����
	NiMaterialPtr	m_spTerrainMaterial;
};
NiSmartPointer(sdTerrainDepthPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif