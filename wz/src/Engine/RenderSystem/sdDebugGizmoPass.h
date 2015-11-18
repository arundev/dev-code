//*************************************************************************************************
// ����:	ʹ��Ĭ�ϲ���ǰ����ɫ����,���ڻ��Ƶ��Զ���
//---------------------------------------------------------
// ����:		
// ����:		2012-09-03
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_DEBUG_GIZMO_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_DEBUG_GIZMO_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// �Բ����Mesh,ʹ��GB��Ĭ�϶��������Ƶ����ջ���
//-------------------------------------------------------------------------------------------------
class sdDebugGizmoPass : public sdRenderPass
{
public:
	sdDebugGizmoPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdDebugGizmoPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// ��ӻ��ƶ���
	void InsertStaticMesh(NiMesh* spMesh);
};
NiSmartPointer(sdDebugGizmoPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif