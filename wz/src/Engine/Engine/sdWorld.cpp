#include "sdEnginePCH.h"
#include "sdWorld.h"
#include "sdMap.h"
#include "sdTerrain.h"
#include "sdBuilding.h"
#include "sdDecal.h"
#include "sdSceneStream.h"

#include <sdFileUtil.h>

using namespace Base::Util;
//-------------------------------------------------------------------------------------------------
sdWorld::sdWorld(const char* szName, const char* szPath)
{
	NIASSERT(szName);
	NIASSERT(szPath);

	strncpy(m_szName, szName, MAX_WORLD_NAME_LEN);
	strncpy(m_szPath, szPath, MAX_WORLD_NAME_LEN);
}
//-------------------------------------------------------------------------------------------------
sdWorld::~sdWorld()
{

}
//-------------------------------------------------------------------------------------------------
sdWorldManager::sdWorldManager()
: m_pkWorld(NULL)
{
	// 游戏根目录
	sdFileUtil::GetModulePath(m_szGameRootPath);
	strncat(m_szGameRootPath, "\\..\\", MAX_PATH_LEN);
	sdFileUtil::NormalizePath(m_szGameRootPath);
}
//-------------------------------------------------------------------------------------------------
sdWorldManager::~sdWorldManager()
{

}
//-------------------------------------------------------------------------------------------------
sdMap* sdWorldManager::LoadScene(const char* szMapName, LoadCallback pfnCallback)
{
	NIASSERT(szMapName);

	// 获取场景路径
	char szBuff[MAX_PATH_LEN];
	strncpy(szBuff, GetGameRootPath(), MAX_PATH_LEN);
	strncat(szBuff, GetCurrentWorld()->GetPath(), MAX_PATH_LEN);
	strncat(szBuff, "scene\\", MAX_PATH_LEN);
	strncat(szBuff, szMapName, MAX_PATH_LEN);

	if (pfnCallback)
		pfnCallback(100, 2, "场景加载开始...");

	// 加载场景
	sdSceneStream kSceneStream(szBuff);
	sdMap* pkMap = kSceneStream.LoadScene(szBuff, pfnCallback);
	NIASSERT(pkMap);

	// 加载地形
	sdTerrain* pkTerrain = pkMap->GetTerrain();
	if (pkTerrain)
	{
		//m_kTerrain.SetEnableEditHeight(false);
		//m_kTerrain.SetEnableEditMaterial(false);
		pkTerrain->SetEnableEditHeight(true);
		pkTerrain->SetEnableEditMaterial(true);
		pkTerrain->LoadScene(szBuff);

		pkTerrain->UpdateBlendMap(0, 0, 10000);
		pkTerrain->RepackMaps();
	}
	

	//*************************
	// 测试
	//*************************
	//sdBuildingPtr pkBuilding = NiNew sdBuilding();
	//NIASSERT(pkBuilding);
	//pkBuilding->SetNIFFile("E:\\project_game\\edata\\world0\\model\\scene\\tongyong\\taijie\\c_chanzong_tutai_002.nif");
	//pkMap->AddEntity(pkBuilding);
	//pkBuilding->SyncLoadNIF();
	//pkBuilding->UpdateLoad();

	{
		sdLightPtr spLight = NiNew sdLight();
		NIASSERT(spLight);

		spLight->SetName("PointLight");
		spLight->SetLightType(sdLight::E_LT_POINT);

		spLight->SetAmbientColor(NiColor::WHITE);
		spLight->SetDiffuseColor(NiColor::WHITE);
		spLight->SetSpecularColor(NiColor::WHITE);
		spLight->SetDimmer(10.0f);

		spLight->SetTR(NiPoint3(40.f, 40.f, 0.f));

		spLight->SetAttenuRange(10.f);
		spLight->SetAttenuSoftness(1.f);

		spLight->Update(0.f);
		spLight->UpdateProperties();
		spLight->UpdateEffects();

		pkMap->AddEntity(spLight);
	}

	{
		sdLightPtr spLight = NiNew sdLight();
		NIASSERT(spLight);

		spLight->SetName("SpotLight");
		spLight->SetLightType(sdLight::E_LT_SPOT);

		spLight->SetAmbientColor(NiColor::WHITE);
		spLight->SetDiffuseColor(NiColor::WHITE);
		spLight->SetSpecularColor(NiColor::WHITE);
		spLight->SetDimmer(10.0f);

		spLight->SetTR(NiPoint3(10.f, 10.f, 5.f));
		spLight->SetRT(NiPoint3(0.f, -45.f, 0.f));

		spLight->SetAttenuRange(10.f);
		spLight->SetAttenuSoftness(1.f);
		spLight->SetSpotFovX(30.f);
		spLight->SetSpotFovY(30.f);
		spLight->SetSpotSoftness(1.f);

		spLight->Update(0.f);
		spLight->UpdateProperties();
		spLight->UpdateEffects();

		pkMap->AddEntity(spLight);
	}

	{
		sdDecalPtr spDecal = NiNew sdDecal();
		NIASSERT(spDecal);

		spDecal->SetName("Decal");

		spDecal->SetTR(NiPoint3(5.f, 5.f, 0.f));
		spDecal->SetRT(NiPoint3(0.f, -30.f, 0.f));

//		spDecal->SetWidth(4.f);
//		spDecal->SetHeight(4.f);
//		spDecal->SetLength(10.f);

//		spDecal->SetDiffuseMaterial(NiColor::WHITE);
//		spDecal->SetSpecularMaterial(NiColor::WHITE);

//		spDecal->SetUVScale(NiPoint2(1.f, 1.f));
//		spDecal->SetUVOffset(NiPoint2::ZERO);
//		spDecal->SetUVSpeed(NiPoint2::ZERO);

//		spDecal->SetDiffuseMap(30.f);
//		spDecal->SetNormalMap(30.f);
//		spDecal->SetFilterMap(1.f);

//		spDecal->SetRecvLighting(true);
//		spDecal->SetAlpha(0.5f);
//		spDecal->SetBlendType(0);

		spDecal->Update(0.f);
		spDecal->UpdateProperties();
		spDecal->UpdateEffects();

		pkMap->AddEntity(spDecal);
	}

	{
		sdBuildingPtr spBuilding = NiNew sdBuilding();

		spBuilding->SetName("Building");

		spBuilding->SetNIF("world0:model\\scene\\tongyong\\chezi\\sb_ty_zhanche_001.nif");
		spBuilding->SyncLoadNIF();

		spBuilding->SetTR(NiPoint3(10.f, 5.f, 0.f));

		spBuilding->Update(0.f);
		spBuilding->UpdateProperties();
		spBuilding->UpdateEffects();

		pkMap->AddEntity(spBuilding);
	}


/*
	//************************

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
	//*****************************************************

	return true;
*/
	return pkMap;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::SaveScene(sdMap* pkMap, const char* szMapName, LoadCallback pfnCallback)
{
	NIASSERT(pkMap);
	NIASSERT(szMapName);

	// 获取场景路径
	char szBuff[MAX_PATH_LEN];
	strncpy(szBuff, GetGameRootPath(), MAX_PATH_LEN);
	strncat(szBuff, GetCurrentWorld()->GetPath(), MAX_PATH_LEN);
	strncat(szBuff, "scene\\", MAX_PATH_LEN);
	strncat(szBuff, szMapName, MAX_PATH_LEN);

	// 保存地形
	sdTerrain* pkTerrain = pkMap->GetTerrain();
	if (pkTerrain)
		pkTerrain->SaveScene(szBuff);

	// 保存场景
	sdSceneStream kSceneStream(szBuff);
	kSceneStream.SaveScene(pkMap, NULL);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::Convert2ResPath(const char* szPath, char* szNewPath)
{
	NIASSERT(szPath);
	NIASSERT(szNewPath);
	NIASSERT(szPath != szNewPath);

	if (IsPhyPath(szPath))
	{
		// 物理路径
		strncpy(szNewPath, szPath, MAX_PATH_LEN);
	}
	else if (IsResPath(szPath))
	{
		// 资源路径
		const char* szCurWorldName = NULL;
		const char* szCurWorldPath = NULL;
		if (m_pkWorld)
		{
			szCurWorldName = m_pkWorld->GetName();
			szCurWorldPath = m_pkWorld->GetPath();
		}

		char szPathWorldName[MAX_WORLD_NAME_LEN];
		GetWorldName(szPath, szPathWorldName);

		char szPathRelPath[MAX_PATH_LEN];
		GetRelativePath(szPath, szPathRelPath);

		if (!strncmp(szPathWorldName, "dependence", MAX_WORLD_NAME_LEN))
		{
			// "dependence:path"类型,dependence的世界特殊处理
			strncpy(szNewPath, m_szGameRootPath, MAX_PATH_LEN);
			strncat(szNewPath, "dependence\\", MAX_PATH_LEN);
			strncat(szNewPath, szPathRelPath, MAX_PATH_LEN);
		}
		else if (!strncmp(szPathWorldName, "worldt", MAX_WORLD_NAME_LEN))
		{
			// "worldt:path"类型,worldt的世界特殊处理
			strncpy(szNewPath, m_szGameRootPath, MAX_PATH_LEN);
			strncat(szNewPath, "idata\\worldt\\", MAX_PATH_LEN);
			strncat(szNewPath, szPathRelPath, MAX_PATH_LEN);
		}
		//else if (!strncmp(szPathWorldName, "world0", MAX_WORLD_NAME_LEN))
		//{
		//	 "world0:path"类型,world0的世界特殊处理(world0表示当前世界)
		//	strncpy(szNewPath, m_szGameRootPath, MAX_PATH_LEN);
		//	strncat(szNewPath, "edata\\", MAX_PATH_LEN);
		//	strncat(szNewPath, szPathRelPath, MAX_PATH_LEN);
		//}
		else if (!strncmp(szPathWorldName, szCurWorldName, MAX_WORLD_NAME_LEN))
		{
			// 当前world
			strncpy(szNewPath, m_szGameRootPath, MAX_PATH_LEN);
			strncat(szNewPath, szCurWorldPath, MAX_PATH_LEN);
			strncat(szNewPath, szPathRelPath, MAX_PATH_LEN);
		}
		else
		{

		}

	}
	else if (IsRelPath(szPath))
	{
		// 相对路径
	}
	else
	{
		// 其余情况暂时作为相对路径处理
		strncpy(szNewPath, m_szGameRootPath, MAX_PATH_LEN);
		strncat(szNewPath, szPath, MAX_PATH_LEN);
	}

	// 标准化路径
	NiPath::Standardize(szNewPath);

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::IsPhyPath(const char* szPath)
{
	NIASSERT(szPath);

	// 路径中有且只能有一个':\\'或':/'
	// (strstr(szPath, ":\\"))
	const char* pcChar = szPath;
	bool bFind = false;
	for (; *pcChar; ++pcChar)
	{
		if (*pcChar == ':' && (*(pcChar+1) == '\\' || *(pcChar+1) == '/'))
		{
			if (bFind)
				return false;
			else
				bFind = true;
		}
	}
	return bFind;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::IsResPath(const char* szPath)
{
	NIASSERT(szPath);

	// 路径中有且只能有一个':',且':'后面没有'\\'或'/'
	const char* pcChar = szPath;
	bool bFind = false;
	for (; *pcChar; ++pcChar)
	{
		if (*pcChar == ':' && *(pcChar+1) != '\\' && *(pcChar+1) != '/')
		{
			if (bFind)
				return false;
			else
				bFind = true;
		}
	}
	return bFind;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::IsRelPath(const char* szPath)
{
	NIASSERT(szPath);

	if (szPath[0] == '\\' || szPath[0] == '／')
	{
		return true;
	}
	else if (szPath[0] == '.')
	{
		if (szPath[1] == '\\' || szPath[1] == '／')
		{
			return true;
		}
		else if (szPath[1] == '.')
		{
			if (szPath[2] == '\\' || szPath[2] == '／')
			{
				return true;
			}
		}
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::GetWorldName(const char* szPath, char* szWorldName)
{
	NIASSERT(szPath);
	NIASSERT(szWorldName);

	if (!IsResPath(szPath))
		return false;

	const char* pcChar = strpbrk(szPath, ":");
	int iLength = min(pcChar - szPath, MAX_WORLD_NAME_LEN);
	strncpy(szWorldName, szPath, iLength);
	szWorldName[iLength] = '\0';			///< 末尾追加结束符

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdWorldManager::GetRelativePath(const char* szPath, char* szRelPath)
{
	NIASSERT(szPath);
	NIASSERT(szRelPath);

	char szWorldName[MAX_WORLD_NAME_LEN];
	if (!GetWorldName(szPath, szWorldName))
		return false;

	uint uiLength = strlen(szWorldName);
	strncpy(szRelPath, szPath+uiLength+1, MAX_PATH_LEN);	///< +1是为了跳过":"符号
	sdFileUtil::ConfimNoSlash(szRelPath);

	return true;
}
//-------------------------------------------------------------------------------------------------