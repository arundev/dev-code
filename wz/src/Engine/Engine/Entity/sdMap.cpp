#include "sdEnginePCH.h"
#include "sdMap.h"
#include "sdWorld.h"

#include "sdBuilding.h"
#include "sdDecal.h"

using namespace Engine::RenderSystem;

NiImplementRTTI(sdMap, sdGameObject);
sdImplementDefaultStream(sdMap);

IMPL_PROPFUNC_MAP(sdMap, sdGameObject)
IMPL_DYNAMIC(sdMap, sdGameObject)
//-------------------------------------------------------------------------------------------------
sdMap::sdMap()
: m_pkTerrain(NULL)
{
	BEGIN_PROPFUNC_MAP(sdMap)

		// 
		REGISTER_PROPFUNC(PROP_SDOBJECT, PROP_ATTR_DEFAULT, sdMap, TerrainProp)		///< 地形
//		REGISTER_PROPFUNC(PROP_SDOBJECT, PROP_ATTR_DEFAULT, sdMap, SkyProp)			///< 天空盒
		REGISTER_PROPFUNC(PROP_SDOBJECT_C, PROP_ATTR_DEFAULT, sdMap, Light)			///< 灯光
//		REGISTER_PROPFUNC(PROP_SDOBJECT_C, PROP_ATTR_DEFAULT, sdMap, Building)		///< 场景对象
	END_PROPFUNC_MAP(sdMap)

	SetName("Map");


	// @{
	m_kStaticObjectVec.reserve(10000);

	m_spDebugNode = NiNew NiNode;
	NIASSERT(m_spDebugNode);
	m_spDebugNode->SetName("DebugNode");

	m_spLightNode = NiNew NiNode;
	NIASSERT(m_spLightNode);
	m_spLightNode->SetName("LightNode");

	m_spDecalNode = NiNew NiNode;
	NIASSERT(m_spDecalNode);
	m_spDecalNode->SetName("DecalNode");

	m_spBuildingNode = NiNew NiNode;
	NIASSERT(m_spBuildingNode);
	m_spBuildingNode->SetName("BuildingNode");
	// @}
}
//-------------------------------------------------------------------------------------------------
sdMap::~sdMap()
{

}
//-------------------------------------------------------------------------------------------------
/*
bool sdMap::CreateScene(const char* szMapName)
{
	NIASSERT(szMapName);

	//*************************************************
	// 临时代码
	//NiStream kStream;
	//kStream.Load("E:\\project_game\\dependence\\engine\\TheClockAndTheOcularis.nif");
	//NiAVObjectPtr pAVObject = (NiAVObject*)kStream.GetObjectAt(0);
	////pAVObject->SetScale(0.001f);
	//pAVObject->Update(0.0f);
	//pAVObject->UpdateProperties();
	//pAVObject->UpdateEffects();
	//m_pkRoot = pAVObject;
	//*************************************************
	
	// 加载地形
	char szBuff[MAX_PATH_LEN];
	strncpy(szBuff, sdWorldManager::Instance().GetGameRootPath(), MAX_PATH_LEN);
	strncat(szBuff, "edata\\", MAX_PATH_LEN);
	strncat(szBuff, sdWorldManager::Instance().GetCurrentWorld()->GetPath(), MAX_PATH_LEN);
	strncat(szBuff, "\\scene\\", MAX_PATH_LEN);
	strncat(szBuff, szMapName, MAX_PATH_LEN);

	m_kTerrain.SetEnableEditHeight(true);
	m_kTerrain.SetEnableEditMaterial(true);
	m_kTerrain.CreateScene(512, 1024);

	//m_kTerrain.CreateScene(512,1024);
	//sdLayerMap* pkLayerMap = NiNew sdLayerMap("Default", 
	//	"E:/project_game/edata/world0/texture/terrain/road_soil_small_pepples.dds",
	//	"E:/project_game/edata/world0/texture/terrain/road_soil_small_pepples_ddn.dds", 10.0);
	//m_kTerrain.AppendLayer(pkLayerMap);

	//pkLayerMap = NiNew sdLayerMap("Default1", 
	//	"E:/project_game/edata/world0/texture/terrain/plains_ferns_green.dds",
	//	"E:/project_game/edata/world0/texture/terrain/plains_ferns_green_ddn.dds");
	//m_kTerrain.AppendLayer(pkLayerMap);

	//pkLayerMap = NiNew sdLayerMap("Default2", 
	//	"E:/project_game/edata/world0/texture/terrain/ground_mossy.dds",
	//	"E:/project_game/edata/world0/texture/terrain/ground_mossy_ddn.dds");
	//m_kTerrain.AppendLayer(pkLayerMap);

	//pkLayerMap = NiNew sdLayerMap("Default3", 
	//	"E:/project_game/edata/world0/texture/terrain/plains_grass_green_and_dry.dds",
	//	"E:/project_game/edata/world0/texture/terrain/plains_grass_green_and_dry_ddn.dds");
	//m_kTerrain.AppendLayer(pkLayerMap);

	//pkLayerMap = NiNew sdLayerMap("Default4", 
	//	"E:/project_game/edata/world0/texture/terrain/cliff_is2.dds",
	//	"E:/project_game/edata/world0/texture/terrain/cliff_is2_ddn.dds");
	//m_kTerrain.AppendLayer(pkLayerMap);

	//pkLayerMap = NiNew sdLayerMap("Default5", 
	//	"E:/project_game/edata/world0/texture/terrain/cliff_more_pieces.dds",
	//	"E:/project_game/edata/world0/texture/terrain/cliff_more_pieces_ddn.dds");
	//m_kTerrain.AppendLayer(pkLayerMap);
	
	m_kTerrain.UpdateBlendMap(0, 0, 10000);
	m_kTerrain.RepackMaps();
	//*************************************************

	// 灯光
	// @{
	if (!m_pkAmbientLight)
	{
		m_pkAmbientLight = NiNew sdLight();
		NIASSERT(m_pkAmbientLight);

		m_pkAmbientLight->SetName("AmbientLight");
		m_pkAmbientLight->SetLightType(sdLight::E_LT_AMBIENT);
		m_pkAmbientLight->SetAmbientColor(NiColor::BLACK);
		m_pkAmbientLight->SetDiffuseColor(NiColor::BLACK);
		m_pkAmbientLight->SetDimmer(1.f);

		m_pkAmbientLight->Update(0.f);
		m_pkAmbientLight->UpdateProperties();
		m_pkAmbientLight->UpdateEffects();
	}

	if (!m_pkMainLight)
	{
		m_pkMainLight = NiNew sdLight();
		NIASSERT(m_pkMainLight);

		m_pkMainLight->SetName("MainLight");
		m_pkMainLight->SetLightType(sdLight::E_LT_DIR);
		m_pkMainLight->SetAmbientColor(NiColor::BLACK);
		m_pkMainLight->SetDiffuseColor(NiColor::BLACK);
		m_pkMainLight->SetSpecularColor(NiColor::BLACK);
		m_pkMainLight->SetDimmer(1.0f);

		NiMatrix3 kRotation;
		kRotation.FromEulerAnglesXYZ(0, -NI_PI/4.0f, -NI_PI/4.0f);
		m_pkMainLight->SetRotate(kRotation);

		m_pkMainLight->Update(0.0f);
		m_pkMainLight->UpdateProperties();
		m_pkMainLight->UpdateEffects();
	}

	if (!m_pkAmbientLight)
	{
		m_pkAssistantLight = NiNew sdLight();
		NIASSERT(m_pkAssistantLight);

		m_pkAssistantLight->SetName("AssistantLight");
		m_pkAssistantLight->SetLightType(sdLight::E_LT_DIR);
		m_pkAssistantLight->SetAmbientColor(NiColor::BLACK);
		m_pkAssistantLight->SetDiffuseColor(NiColor::BLACK);
		m_pkAssistantLight->SetSpecularColor(NiColor::BLACK);
		m_pkAssistantLight->SetDimmer(1.0f);

		m_pkAssistantLight->Update(0.0f);
		m_pkAssistantLight->Update(0.0f);
		m_pkAssistantLight->UpdateProperties();
		m_pkAssistantLight->UpdateEffects();
	}
	// @}

	// 环境
	// @{
	m_pkFlare = NiNew sdFlare;
	NIASSERT(m_pkFlare);

	m_pkSun = NiNew sdSun;
	NIASSERT(m_pkSun);

	m_pkSkyBox = NiNew sdSkyBox;
	NIASSERT(m_pkSkyBox);

	m_pkBGScroll = NiNew sdBGScroll;
	NIASSERT(m_pkBGScroll);

	m_pkCloudDome = NiNew sdCloud;
	NIASSERT(m_pkCloudDome);
	// @}

	// 更新Debug节点
	m_spDebugNode->Update(0.0f);



	// 
	m_kRenderParams.Reset();

	return true;
}*/
//-------------------------------------------------------------------------------------------------
void sdMap::Cull(const NiCamera& kCamera, EntityVec& kEntityVec)
{
	EntityVecItr itr = m_kStaticObjectVec.begin();
	EntityVecItr itr_end = m_kStaticObjectVec.end();
	for (; itr != itr_end; ++itr)
		kEntityVec.push_back(*itr);
}
//-------------------------------------------------------------------------------------------------
void sdMap::UpdateGame()
{
	// 更新场景图
	

	// 更新加载管理器
	m_kSceneLoadMgr.Update();
}
//-------------------------------------------------------------------------------------------------
bool sdMap::AddEntity(sdEntity* pkEntity)
{
	NIASSERT(pkEntity);

	// 检查HashCode
	uint uiHashCode = pkEntity->GetHashCode();
	bool bExist = ms_kHashCodeGen.IsCodeExist(uiHashCode);
	if (uiHashCode == INVALID_HASHCODE)
	{
		uint uiNewHashCode = ms_kHashCodeGen.GetCode();
		if (uiNewHashCode == INVALID_HASHCODE)
			return false;
		pkEntity->SetHashCode(uiNewHashCode);
	}
	else
	{
		ms_kHashCodeGen.AddCode(uiHashCode);
	}

	// 添加到对应队列
	if (pkEntity->GetType() == E_ET_BUILDING || 
		pkEntity->GetType() == E_ET_LIGHT ||
		pkEntity->GetType() == E_ET_DECAL)
	{
		m_kStaticObjectVec.push_back(pkEntity);
	}

	// 加入场景管理器
	//m_pkOctree->AddObject();
	//m_pkEntityMatrix->AddObject();

	// 设置所属地图
	pkEntity->SetMap(this);

	// 分类处理
	if (NiIsKindOf(sdTerrain, pkEntity))
	{
		NIASSERT(!m_pkTerrain);
		m_pkTerrain = (sdTerrain*)pkEntity;
	}
	else if (NiIsKindOf(sdBuilding, pkEntity))
	{
		m_spBuildingNode->AttachChild(pkEntity);
	}
	else if (NiIsKindOf(sdLight, pkEntity))
	{
		m_spLightNode->AttachChild(pkEntity);

		sdLight* pkLight = (sdLight*)pkEntity;
		if (pkLight->GetName().Equals("AmbientLight"))
		{
			if (!m_pkAmbientLight)
				m_pkAmbientLight = pkLight;
		}
		else if (pkLight->GetName().Equals("MainLight"))
		{
			if (!m_pkMainLight)
				m_pkMainLight = pkLight;
		}
		else if (pkLight->GetName().Equals("AssistantLight"))
		{
			if (!m_pkAssistantLight)
				m_pkAssistantLight = pkLight;
		}
	}
	else if (NiIsKindOf(sdDecal, pkEntity))
	{
		m_spDecalNode->AttachChild(pkEntity);
	}

	// 加入加载管理器
	m_kSceneLoadMgr.AddEntity(pkEntity);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdMap::RemoveEntity(sdEntity* pkEntity)
{
	

	return true;
}
//-------------------------------------------------------------------------------------------------