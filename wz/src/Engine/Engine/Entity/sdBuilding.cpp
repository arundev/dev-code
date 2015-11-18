#include "sdEnginePCH.h"
#include "sdBuilding.h"
#include "IRenderPath.h"

using namespace Engine::RenderSystem;

NiImplementRTTI(sdBuilding, sdEntity);
sdImplementDefaultStream(sdBuilding);

IMPL_PROPFUNC_MAP(sdBuilding, sdEntity)
IMPL_DYNAMIC(sdBuilding, sdEntity)
//-------------------------------------------------------------------------------------------------
sdBuilding::sdBuilding()
{
	BEGIN_PROPFUNC_MAP(sdBuilding)
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT | PROP_ATTR_FILEPATH, sdBuilding, NIF)	///< NIF·��
	END_PROPFUNC_MAP(sdBuilding)

	SetName("Building");			///< ��������
	SetType(E_ET_BUILDING);			///< ����Building����
	SetLoadingStatus(E_LS_WAIT);	///< ���ü���״̬
}
//-------------------------------------------------------------------------------------------------
sdBuilding::~sdBuilding()
{

}
//-------------------------------------------------------------------------------------------------
void sdBuilding::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	__super::Cull(kCamera, kRenderPath);

	// ���а�Χ�вü�
	// \TODO

	// �õ��ɼ�Mesh�б�
	typedef std::vector<NiMesh*> MeshVec;
	typedef std::vector<NiMesh*>::iterator MeshVecItr;
	MeshVec kMeshVec;
	__super::Cull(kCamera, kMeshVec);

	// ���ݵ�ǰ״̬���зַ�����
	bool bNeedEdgeEnhancement = false;
	if (m_bDynamic)
	{
		MeshVecItr kItr = kMeshVec.begin();
		MeshVecItr kItrEnd = kMeshVec.end();
		for (; kItr != kItrEnd; ++kItr)
		{
			NiMesh* pkMesh = *kItr;
			NIASSERT(pkMesh);

			// �ύ����
			//spMesh->OnVisible()

			// ��������Ƥ�Ķ���
			if (pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED()))
				continue;

			//
			NiPropertyState* pkPropState = pkMesh->GetPropertyState();
			if (!pkPropState)
				continue;

			// ������
			NiAlphaProperty* pkAlphaProp = pkPropState->GetAlpha();
			bool bAlphaBlend = pkAlphaProp && pkAlphaProp->GetAlphaBlending();
			if (m_bEdge)
			{
				if (!bAlphaBlend)
				{
					kRenderPath.RenderMeshEdge(pkMesh);
					bNeedEdgeEnhancement = true;
				}
			}

			// �����͸������ĻŤ��
			bool bShimmer = pkMesh->GetName().EqualsNoCase("shimmer");
			if (bAlphaBlend || bShimmer)
			{

			}
			else
			{
				kRenderPath.RenderStaticMesh(pkMesh);
			}
		}
	}
	else
	{
		MeshVecItr kItr = kMeshVec.begin();
		MeshVecItr kItrEnd = kMeshVec.end();
		for (; kItr != kItrEnd; ++kItr)
		{
			NiMesh* pkMesh = *kItr;
			NIASSERT(pkMesh);

			// �ύ����
			//pkMesh->OnVisible()

			// ��������Ƥ�Ķ���
			if (pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED()))
				continue;

			// ������ҶBillboard�͵��뵭��


			// 
			NiPropertyState* pkPropState = pkMesh->GetPropertyState();
			if (!pkPropState)
				continue;

			// ������
			NiAlphaProperty* pkAlphaProp = pkPropState->GetAlpha();
			bool bAlphaBlend = pkAlphaProp && pkAlphaProp->GetAlphaBlending();
			if (m_bEdge)
			{
				if (!bAlphaBlend)
				{
					kRenderPath.RenderMeshEdge(pkMesh);
					bNeedEdgeEnhancement = true;
				}
			}

			// �����͸������ĻŤ��
			bool bShimmer = pkMesh->GetName().EqualsNoCase("shimmer");
			if (bAlphaBlend || bShimmer)
			{

			}
			else
			{
				kRenderPath.RenderStaticMesh(pkMesh);
			}
		}
	}

	// ������ǿ
	if (m_bEdge && bNeedEdgeEnhancement)
	{
		kRenderPath.RenderMeshEdgeEnhancement(m_kWorldBound, m_kEdgeColor, 4);
	}

//	// ��Ⱦ��Դ�Դ��ĵ��Դ
//	PointLightVecItr kItr = m_kLights.begin();
//	PointLightVecItr kItrEnd = m_kLights.end();
//	for (; kItr != kItrEnd; ++kItr)
//	{
//		NiPointLight* pkPointLight = *kItr;
//		NIASSERT(pkPointLight);
//
//		kRenderPath->RenderLight();
//	}
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::UpdateGame()
{
	__super::UpdateGame();
}
//-------------------------------------------------------------------------------------------------