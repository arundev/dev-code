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
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT | PROP_ATTR_FILEPATH, sdBuilding, NIF)	///< NIF路径
	END_PROPFUNC_MAP(sdBuilding)

	SetName("Building");			///< 对象名称
	SetType(E_ET_BUILDING);			///< 设置Building类型
	SetLoadingStatus(E_LS_WAIT);	///< 设置加载状态
}
//-------------------------------------------------------------------------------------------------
sdBuilding::~sdBuilding()
{

}
//-------------------------------------------------------------------------------------------------
void sdBuilding::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	__super::Cull(kCamera, kRenderPath);

	// 进行包围盒裁剪
	// \TODO

	// 得到可见Mesh列表
	typedef std::vector<NiMesh*> MeshVec;
	typedef std::vector<NiMesh*>::iterator MeshVecItr;
	MeshVec kMeshVec;
	__super::Cull(kCamera, kMeshVec);

	// 根据当前状态进行分发处理
	bool bNeedEdgeEnhancement = false;
	if (m_bDynamic)
	{
		MeshVecItr kItr = kMeshVec.begin();
		MeshVecItr kItrEnd = kMeshVec.end();
		for (; kItr != kItrEnd; ++kItr)
		{
			NiMesh* pkMesh = *kItr;
			NIASSERT(pkMesh);

			// 提交更新
			//spMesh->OnVisible()

			// 跳过带蒙皮的对象
			if (pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED()))
				continue;

			//
			NiPropertyState* pkPropState = pkMesh->GetPropertyState();
			if (!pkPropState)
				continue;

			// 处理勾边
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

			// 处理半透明和屏幕扭曲
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

			// 提交更新
			//pkMesh->OnVisible()

			// 跳过带蒙皮的对象
			if (pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED()))
				continue;

			// 处理树叶Billboard和淡入淡出


			// 
			NiPropertyState* pkPropState = pkMesh->GetPropertyState();
			if (!pkPropState)
				continue;

			// 处理勾边
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

			// 处理半透明和屏幕扭曲
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

	// 勾边增强
	if (m_bEdge && bNeedEdgeEnhancement)
	{
		kRenderPath.RenderMeshEdgeEnhancement(m_kWorldBound, m_kEdgeColor, 4);
	}

//	// 渲染资源自带的点光源
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