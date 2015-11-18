//*************************************************************************************************
// ����:	����
//---------------------------------------------------------
// ����:		
// ����:		2013-05-30
// ����޸�:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_EDGE_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_EDGE_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdEdgePass : public sdRenderPass
{
public:
	sdEdgePass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdEdgePass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, uint uiEdgeStencilIDWriteMask);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertEdgeMesh(NiMesh* pkMesh);

protected:
	uint m_uiEdgeStencilID;				///< ����ģ��ο�ֵ
	uint m_uiEdgeStencilIDMask;			///< ����ģ������
	uint m_uiEdgeStencilIDWriteMask;	///< ����ģ��д������
	
	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kMaterialVec;			///< ����
};
NiSmartPointer(sdEdgePass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif