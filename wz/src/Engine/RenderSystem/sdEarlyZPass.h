//*************************************************************************************************
// ����:	EarlyZ����Ⱦ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:	2012-12-12
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_EARLYZ_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_EARLYZ_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ����д��Depth-Stencil Buffer,���Ի�ȡ˫��д���ٶ�
// 
// \class sdEarlyZPass
// 
// The Early-Z Pass only writes depth-stencil buffer of the specified render target group.
//-------------------------------------------------------------------------------------------------
class sdEarlyZPass : public sdRenderPass
{
public:
	sdEarlyZPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdEarlyZPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(uint uiStaticMeshStencilID, uint uiStaticMeshStencilIDWriteMask);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertStaticMesh(NiMesh* spMesh);

protected:
	//@{
	// �麯���̳�
	virtual RenderObjectComparator GetComparator();

	// ���Ƶ�����Ƚ���
	static bool	Comparator(sdRenderObject* lhs, sdRenderObject* rhs);
	//@}

protected:
	// ģ�����
	uint	m_uiStaticMeshStencilID;
	uint	m_uiStaticMeshStencilIDMask;

	// ����
	NiMaterialPtr	m_spSolidMeshZMaterial;
	NiMaterialPtr	m_spNormalMapAlphaTestZMaterial;
	NiMaterialPtr	m_spDiffuseMapAlphaTestZMaterial;	
};
NiSmartPointer(sdEarlyZPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif