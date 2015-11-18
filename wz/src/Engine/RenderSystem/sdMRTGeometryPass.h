//*************************************************************************************************
// ����:	���Geometry-Buffer����Ⱦ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-14
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_MRT_GEOMETRY_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_MRT_GEOMETRY_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdMRTGeometryPass : public sdRenderPass
{
public:
	sdMRTGeometryPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdMRTGeometryPass();

	// �麯���̳�
	virtual void Draw();

	// ��ʼ��������
	bool Initialize(uint uiStencilID, uint uiStencilIDWriteMask);
	void Destroy();

	// ��ӻ��ƶ���
	void InsertStaticMesh(NiMesh* spMesh);

protected:
	uint	m_uiStencilID;
	uint	m_uiStencilIDWriteMask;

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kStaticGeometryMaterials;		///< ��̬Mesh����
	MaterialVec	m_kSkinnedGeometryMaterials;	///< ������Mesh����

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdMRTGeometryPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif