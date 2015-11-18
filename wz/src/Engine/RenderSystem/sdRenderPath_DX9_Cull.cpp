#include "sdRenderSystemPCH.h"
#include "sdRenderPath_DX9.h"

#include <sdMap.h>
#include <sdTerrainSystem.h>

using namespace std;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::Render()
{
	// RenderShadow();
	// RenderReflection();
	RenderMainView();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::RenderMainView()
{
	//sdPF_Func_Auto;

	//********************************************************
	//using namespace std;
	//vector<NiMesh*> kMeshVec;
	//CollectMeshes((NiNode*)(NiAVObject*)m_pkCurMap->m_pkRoot, kMeshVec);
	//
	//vector<NiMesh*>::iterator itr = kMeshVec.begin();
	//vector<NiMesh*>::iterator itr_end = kMeshVec.end();
	//for (; itr!=itr_end; ++itr)
	//{
	//	RenderStaticMesh(*itr, -1);
	//}
	//********************************************************

	// 裁剪静态物件
	OctreeCull();

	// 裁剪动态物件


	// 裁剪地形
	RenderTerrain();

	// 裁剪调试对象
	RenderDebugGizmo();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::OctreeCull()
{
	NIASSERT(m_pkCurMap);
	NIASSERT(m_pkCurCam);

	//sdPF_Func_Auto;

	// 裁剪Map得到待渲染Entity数组
	EntityVec kEntityvec;
	m_pkCurMap->Cull(*m_pkCurCam, kEntityvec);

	// 裁剪Entity数组得到待渲染Mesh,构建RenderObject队列,填充到RenderPath
	EntityVecItr itr = kEntityvec.begin();
	EntityVecItr itr_end = kEntityvec.end();
	for (; itr != itr_end; ++itr)
	{
		sdEntity* pkEntity = *itr;
		NIASSERT(pkEntity);

		pkEntity->Cull(*m_pkCurCam, *this);
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::UmbraCull()
{

}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::CollectMeshes(NiNode* spNode, std::vector<NiMesh*>& kMeshVec)
{
	NIASSERT(spNode);

	if (spNode->GetAppCulled())
		return;

	using namespace std;
	stack<NiNode*> kNodeStack;
	kNodeStack.push(spNode);
	while(!kNodeStack.empty())
	{
		NiNode* spCurNode = kNodeStack.top();
		kNodeStack.pop();

		int iCount = spCurNode->GetChildCount();
		for (int i = 0; i < iCount; ++i)
		{
			NiAVObject* spAVObject = spCurNode->GetAt(i);
			if (spAVObject && !spAVObject->GetAppCulled())
			{
				if (spAVObject->IsNode())
				{
					kNodeStack.push((NiNode*)spAVObject);
				}
				else if (NiIsKindOf(NiMesh, spAVObject))
				{
					// wz原文用的IsMesh,他们修改了GB源码,类似IsNode
					kMeshVec.push_back((NiMesh*)spAVObject);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::RenderTerrain()
{
	// 确信Camera/Map/Terrain有效性
	if (!m_pkCurCam || !m_pkCurMap || !m_pkCurMap->GetTerrain())
		return;

	sdTerrain* pkTerrain = m_pkCurMap->GetTerrain();
	NIASSERT(pkTerrain);

	// 裁剪
	vector<NiMesh*> kMeshVec;
	pkTerrain->Cull(*m_pkCurCam, kMeshVec);

	// 添加到渲染路径
	vector<NiMesh*>::iterator itr = kMeshVec.begin();
	vector<NiMesh*>::iterator itr_end = kMeshVec.end();
	for (; itr != itr_end; ++itr)
	{
		RenderTerrainMesh(*itr);
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::RenderDebugGizmo()
{
	// 确信Camera/Map有效性
	if (!m_pkCurCam || !m_pkCurMap)
		return;

	// 确定绘制Pass非空
	if (!m_spDebugGizmoPass)
		return;

	// 收集DebugNode的Mesh(用于编辑器)
	vector<NiMesh*> kMeshVec;
	CollectMeshes((NiNode*)(NiAVObject*)m_pkCurMap->GetDebugNode(), kMeshVec);

	vector<NiMesh*>::iterator itr = kMeshVec.begin();
	vector<NiMesh*>::iterator itr_end = kMeshVec.end();
	for (; itr!=itr_end; ++itr)
	{
		m_spDebugGizmoPass->InsertStaticMesh(*itr);
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE