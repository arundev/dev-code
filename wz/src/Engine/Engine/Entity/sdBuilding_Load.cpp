#include "sdEnginePCH.h"
#include "sdBuilding.h"
#include "IResourceSystem.h"
#include "sdWorld.h"

using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
uint GetLoadTexLodLevel()
{
	static float fDiffuseMapPixel = 0.f;
	static float fGlossMapPixel = 0.f;
	static float fNormalMapPixel = 92.f;
	static float fLightMapPixel = 0.f;
	static float fDetailNormalMapPixel = 162.f;
	static float fEnvironmentMapPixel = 162.f;

	return 0xffffffff;
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::UpdateLoad()
{
	// 加载模型
	// @{
	ELoadingState eLoadingStatus = GetLoadingStatus();
	if (eLoadingStatus == E_LS_WAIT)
	{
		// 当前处于等待状态,则检查距离,距离足够近则转入加载状态
		bool bNeedLoad = true;	// sdOctree::IsNeedLoad();
		if (bNeedLoad)
		{
			SetLoadingStatus(E_LS_LOADING);
			AsyncLoadNIF();
		}
	}
	else if (eLoadingStatus == E_LS_LOADING)
	{
		// 当前处于加载状态,则检查距离,距离足够近则提升加载优先级
		float fSqlDistance = 10.f;	
		if (GetViewDistance() >= 30.f)	
			UpdatePrior(E_RES_PRIOR_HIGHEST, fSqlDistance);	
		else
			UpdatePrior(E_RES_PRIOR_ATONCE, fSqlDistance);	///< 30m以内立刻加载
	}
	else if (eLoadingStatus == E_LS_VALID)
	{
		// 
	}
	// @}

	// 加载纹理
	// @{
	eLoadingStatus = GetLoadingStatus();		///< 上面可能更新过加载状态
	if (GetLoadingStatus() == E_LS_VALID)
	{
		uint uiLODBit = GetLoadTexLodLevel();	///< 计算纹理LOD
		if ((uiLODBit & E_ETL_LIGHTMAP) != 0 && m_eLightMapLoadingState == E_LS_WAIT)
		{
			//
			m_eLightMapLoadingState = E_LS_VALID;
		}
		if ((uiLODBit & E_ETL_DETAILNORMALMAP) != 0 && m_eDetaiNormalMapLoadingState == E_LS_WAIT)
		{
			//
			m_eDetaiNormalMapLoadingState = E_LS_VALID;
		}
		if ((uiLODBit & E_ETL_ENVIRONMENTMAP) != 0 && m_eEnvironmentMapLoadingState == E_LS_WAIT)
		{
			//
			m_eEnvironmentMapLoadingState = E_LS_VALID;
		}
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::UpdateUnload()
{
	
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::UpdateDelayUnload()
{

}

//-------------------------------------------------------------------------------------------------
void sdBuilding::SyncLoadNIF()
{
	if (m_spNIFRoot)
	{
		DetachChild(m_spNIFRoot);
		m_spNIFRoot = 0;
	}

	NiNodePtr spNIFRoot = IResourceSystem::Instance().SyncLoadNif((const char*)m_kNIFPath);
	NIASSERT(spNIFRoot);

	AttachGraphic(spNIFRoot);
	GraphicLoadOver();

	SetLoadingStatus(E_LS_VALID);
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::AsyncLoadNIF()
{
	// 转换到物理路径
	char szResPath[MAX_PATH_LEN];
	sdWorldManager::Instance().Convert2ResPath((const char*)m_kNIFPath, szResPath);

	// 请求加载
	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	sdResourceParam kResParam(E_RES_TYPE_NIF, E_RES_PRIOR_HIGHEST, E_RES_USAGE_NIF_BUILDING, szResPath);
	m_pkResource = pkResourceSystem->AsyncLoadResource(kResParam, m_uiResourceTick, this);
}
//-------------------------------------------------------------------------------------------------
bool sdBuilding::UpdatePrior(EResReqPrior ePrior, float fSqlDistance)
{
	if (m_pkResource)
	{
		m_pkResource->UpdatePrior(ePrior, fSqlDistance);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::OnLoaded(IResource* pkResource)
{
	if (m_pkResource == pkResource)
		m_pkResource = NULL;	///< 清除本地加载指针

	NiNode* spRootNode = pkResource->GetRootNode();
	if (spRootNode)
	{
		if (GetLoadingStatus() == E_LS_LOADING)
		{
			SetLoadingStatus(E_LS_VALID);
			AttachGraphic(spRootNode);
			GraphicLoadOver();

			m_uiResourceTick = pkResource->GetTicket();
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::AttachGraphic(NiNode* pkNode)
{
	NIASSERT(pkNode);

	if (m_spNIFRoot == pkNode)
		return;

	DetachGraphic();

	if (pkNode)
	{
		m_spNIFRoot = pkNode;
		AttachChild(pkNode);
	}
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::DetachGraphic()
{
	m_kLights.clear();

	if (m_spNIFRoot)
	{
		DetachChild(m_spNIFRoot);
		m_spNIFRoot = 0;
	}

	if (m_uiResourceTick != IResource::INVALID_RES_TICKET)
	{
		IResourceSystem::Instance().AsyncFreeResource(m_uiResourceTick);
		m_uiResourceTick = IResource::INVALID_RES_TICKET;
	}

	if (m_pkResource)
	{
		IResourceSystem::Instance().AsyncFreeResource(m_pkResource->GetTicket());
		//IResourceSystem::Instance().UnRegister(this);
		m_pkResource = 0;
	}
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::GraphicLoadOver()
{
	if (!m_spNIFRoot)
		return;

	//	// 通知场景管理器
	//	sdEntityMatrix* pkEntityMatrix = m_pkMap->GetEntityMatrix();
	//	if (pkEntityMatrix)
	//	{
	//		pkEntityMatrix->BuildingLoaded();
	//	}

	// 处理Shimmer信息
	ProcessShimmer(this);

	// 处理TreeLeaf信息
	ProcessTreeLeaf(this);

	// 处理灯光
	ProcessLight(this);

	// 处理特殊材质

	// 处理贴花

	// 处理勾边
	if (m_bEdge)
		ProcessEdge(this);

	// 更新包围盒
	//UpdateWorldBound();
	//UpdateNodeBound();

	// 更新
	Update(0.f);
	UpdateProperties();
	UpdateEffects();
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::ProcessShimmer(NiNode* pkNode)
{
	if (!pkNode)
		return;


}
//-------------------------------------------------------------------------------------------------
void sdBuilding::ProcessTreeLeaf(NiNode* pkNode)
{
	if (!pkNode)
		return;
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::ProcessLight(NiNode* pkNode)
{
	if (!pkNode)
		return;


}
//-------------------------------------------------------------------------------------------------