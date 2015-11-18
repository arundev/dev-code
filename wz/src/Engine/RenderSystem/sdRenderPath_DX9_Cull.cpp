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

	// �ü���̬���
	OctreeCull();

	// �ü���̬���


	// �ü�����
	RenderTerrain();

	// �ü����Զ���
	RenderDebugGizmo();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::OctreeCull()
{
	NIASSERT(m_pkCurMap);
	NIASSERT(m_pkCurCam);

	//sdPF_Func_Auto;

	// �ü�Map�õ�����ȾEntity����
	EntityVec kEntityvec;
	m_pkCurMap->Cull(*m_pkCurCam, kEntityvec);

	// �ü�Entity����õ�����ȾMesh,����RenderObject����,��䵽RenderPath
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
					// wzԭ���õ�IsMesh,�����޸���GBԴ��,����IsNode
					kMeshVec.push_back((NiMesh*)spAVObject);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPath_DX9::RenderTerrain()
{
	// ȷ��Camera/Map/Terrain��Ч��
	if (!m_pkCurCam || !m_pkCurMap || !m_pkCurMap->GetTerrain())
		return;

	sdTerrain* pkTerrain = m_pkCurMap->GetTerrain();
	NIASSERT(pkTerrain);

	// �ü�
	vector<NiMesh*> kMeshVec;
	pkTerrain->Cull(*m_pkCurCam, kMeshVec);

	// ��ӵ���Ⱦ·��
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
	// ȷ��Camera/Map��Ч��
	if (!m_pkCurCam || !m_pkCurMap)
		return;

	// ȷ������Pass�ǿ�
	if (!m_spDebugGizmoPass)
		return;

	// �ռ�DebugNode��Mesh(���ڱ༭��)
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