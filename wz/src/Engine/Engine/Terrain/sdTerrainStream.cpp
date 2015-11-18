#include "sdEnginePCH.h"
#include "sdTerrainStream.h"
#include "sdTerrain.h"

#include "sdDynamicTexture.h"
#include "sdTextureAtlas.h"
#include "sdTextureUtility.h"

//
#include <sdFileUtil.h>
#include <sdLuaFile.h>

//
#include <NiDX9TextureData.h>

#define TERRAIN_DIRECTORY					"terrain"				///< �����������Ŀ¼
#define TERRAIN_CONFIG_FILENAME				"terrain.lua"			///< ���������ļ�
#define TERRAIN_HEIGHTMAP_FILENAME			"heightmap.raw"			///< �ر�߶�ͼ
#define TERRAIN_HEIGHTMAP_CONFIG_FILENAME	"heightmap.lua"			///< �ر�߶�ͼ�����ļ�
#define TERRAIN_NORMALMAP_FILENAME			"normalmap.dds"			///< �ر���ͼ(��������Ϸ)
#define TERRAIN_LAYERMAP_FILENAME			"layermap.raw"			///< �ر���������Ϣ(�����ڱ༭��)
#define	TERRAIN_LAYERMAP_LIST_FILENAME		"layermap.lua"			///< �ر������б�	
#define TERRAIN_BLENDMAP_FILENAME			"blendmap.dds"			///< �ر�����ͼ(��������Ϸ)
#define TERRAIN_TILEMAP_FILENAME			"tilemap.dds"			///< �ر���������ͼ(��������Ϸ)
#define	TERRAIN_ATLAS_CONFIG_FILENAME		"atlas.lua"				///< ��ͼ������(��������Ϸ)
#define	TERRAIN_DIFFUSE_ATLASMAP_FILENAME	"diffuseatlasmap.dds"	///< ��ͼ��(��������Ϸ)
#define TERRAIN_DIFFUSE_ATLASTABLE_FILENAME	"diffuseatlastable.dds"	///< ��ͼ��(��������Ϸ)
#define	TERRAIN_NORMAL_ATLASMAP_FILENAME	"normalatlasmap.dds"	///< ��ͼ��(��������Ϸ)
#define TERRAIN_NORMAL_ATLASTABLE_FILENAME	"normalatlastable.dds"	///< ��ͼ��(��������Ϸ)

using namespace Base::Util;
using namespace Base::Math;
using namespace Engine;
using namespace Engine::Util;
using namespace Engine::RenderSystem;
//-------------------------------------------------------------------------------------------------
sdTerrainStream::sdTerrainStream(const char* szSceneFullPath)
: m_bCompactHeightMap(true)
{
	NIASSERT(szSceneFullPath);
	::strncpy(m_szSceneFullPath, szSceneFullPath, MAX_PATH);
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveConfig(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_CONFIG_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ���ļ�
	sdLuaWriteUtil kConfigFile;
	if (!kConfigFile.OpenFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// д���ļ�
	sdTerrainParams& kTerrainParams = pkTerrain->GetTerrainParams();
	kConfigFile.WriteRootNodeBegin("Terrain");
		kConfigFile.WriteData("TerrainSize", pkTerrain->GetTerrainSize(), 15);
		kConfigFile.WriteData("BlendTexSize", pkTerrain->GetBlendMapSize(), 15);

		kConfigFile.WriteNodeBegin("Material");
			kConfigFile.WriteData("AmbientMaterial", kTerrainParams.ambientMaterial, 15);
			kConfigFile.WriteData("DiffuseMaterial", kTerrainParams.diffuseMaterial, 15);
			kConfigFile.WriteData("SpecularMaterial", kTerrainParams.specularMaterial, 15);
		kConfigFile.WriteNodeEnd();

	kConfigFile.WriteRootNodeEnd();

	// �����ļ�
	kConfigFile.CloseFile();

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::LoadConfig(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szConfigFullPath[MAX_PATH];
	strcpy(szConfigFullPath, szBuffer);
	strcat(szConfigFullPath, "\\");
	strcat(szConfigFullPath, TERRAIN_CONFIG_FILENAME);

	// ���ļ�
	sdLuaReadUtil kConfigFile;
	if (!kConfigFile.LoadFile(szConfigFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// ��ȡ�ļ�����
	kConfigFile.ReadRootNode("Terrain");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	kConfigFile.ReadData("TerrainSize", pkTerrain->m_uiTerrainSize);
	kConfigFile.ReadData("BlendTexSize", pkTerrain->m_uiBlendTexSize);

	{
		kConfigFile.BeginReadNode("Material");
		if (kConfigFile.IsNodeValid())
		{
			sdTerrainParams& kTerrainParams = pkTerrain->GetTerrainParams();
			kConfigFile.ReadData("AmbientMaterial", kTerrainParams.ambientMaterial);
			kConfigFile.ReadData("DiffuseMaterial", kTerrainParams.diffuseMaterial);
			kConfigFile.ReadData("SpecularMaterial", kTerrainParams.specularMaterial);

			kConfigFile.EndReadNode();
		}	
	}
	
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveHeightMapConfig(sdHeightMap* pkHeightMap)
{
	NIASSERT(pkHeightMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_HEIGHTMAP_CONFIG_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ���ļ�
	sdLuaWriteUtil kConfigFile;
	if (!kConfigFile.OpenFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// д���ļ��汾
	kConfigFile.WriteRootNodeBegin("Version");
	kConfigFile.WriteData("ID", 1, 15);
	kConfigFile.WriteRootNodeEnd();

	// д���ļ�
	kConfigFile.WriteRootNodeBegin("Terrain");
	kConfigFile.WriteNodeBegin("HeightMap");
		kConfigFile.WriteData("Size", pkHeightMap->GetSize(), 15);
		kConfigFile.WriteData("MinHeight", pkHeightMap->GetMinHeight(), 15);
		kConfigFile.WriteData("MaxHeight", pkHeightMap->GetMaxHeight(), 15);
		kConfigFile.WriteData("Compacted", true, 15);
	kConfigFile.WriteNodeEnd();
	kConfigFile.WriteRootNodeEnd();

	// �����ļ�
	kConfigFile.CloseFile();

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
sdHeightMapPtr sdTerrainStream::LoadHeightMapConfig()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szConfigFullPath[MAX_PATH];
	strcpy(szConfigFullPath, szBuffer);
	strcat(szConfigFullPath, "\\");
	strcat(szConfigFullPath, TERRAIN_HEIGHTMAP_CONFIG_FILENAME);

	// ���ļ�
	sdLuaReadUtil kConfigFile;
	if (!kConfigFile.LoadFile(szConfigFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// ��ȡ�ļ��汾����
	kConfigFile.ReadRootNode("Version");
	if (kConfigFile.IsNodeValid())
	{
		uint uiID = 0;
		kConfigFile.ReadData("ID", uiID);
	}

	// ��ȡ�ļ�����
	kConfigFile.ReadRootNode("Terrain");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	kConfigFile.BeginReadNode("HeightMap");

	uint uiSize = 0;
	kConfigFile.ReadData("Size", uiSize);

	float fMinHeight = 0.f;
	kConfigFile.ReadData("MinHeight", fMinHeight);

	float fMaxHeight = 0.f;
	kConfigFile.ReadData("MaxHeight", fMaxHeight);

	m_bCompactHeightMap = false; 
	kConfigFile.ReadData("Compacted", m_bCompactHeightMap);

	kConfigFile.EndReadNode();

	// �������θ߶�ͼ
	sdHeightMapPtr pkHeightMap = NiNew sdHeightMap(uiSize, fMinHeight, fMaxHeight);
	NIASSERT(pkHeightMap);

	return pkHeightMap;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveHeightMap(sdHeightMap* pkHeightMap)
{
	NIASSERT(pkHeightMap);;

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// ·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_HEIGHTMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// �����ļ�
	NiFile* pkFile = NiFile::GetFile(szFileFullPath, NiFile::WRITE_ONLY);
	NIASSERT(pkFile)

	// д���ļ�
	if (m_bCompactHeightMap)
	{
		// ����������ȡ����, ����ѹ����д���ļ�
		uint uiSize = pkHeightMap->GetSize();
		float fMinHeight = pkHeightMap->GetMinHeight();
		float fMaxHeight = pkHeightMap->GetMaxHeight();
		float fHeightRangeDive = 1.f / (fMaxHeight - fMinHeight);
		ushort* pusHeight = NiAlloc2(ushort, uiSize * uiSize, NiMemHint::NONE);
		ushort* pusCurHeight = pusHeight;
		for (uint uiY = 0; uiY < uiSize; ++uiY)
		{
			for (uint uiX = 0; uiX < uiSize; ++uiX)
			{
				*pusCurHeight++ = (ushort)((pkHeightMap->GetRawHeight(uiX, uiY) - fMinHeight) * fHeightRangeDive * 65535.f);
			}
		}
		
		pkFile->Write(pusHeight, uiSize * uiSize * sizeof(ushort));
		pkFile->Flush();
		NiFree(pusHeight);
	}
	else
	{
		// ����һ����ȡ����, ֱ��д���ļ�
		uint uiSize = pkHeightMap->GetSize();
		float* pfHeight = NiAlloc2(float, uiSize * uiSize, NiMemHint::NONE);
		float* pfCurHeight = pfHeight;
		for (uint uiY = 0; uiY < uiSize; ++uiY)
		{
			for (uint uiX = 0; uiX < uiSize; ++uiX)
			{
				*pfCurHeight++ = pkHeightMap->GetRawHeight(uiX, uiY);
			}
		}

		pkFile->Write(pfHeight, uiSize * uiSize * sizeof(float));
		pkFile->Flush();
		NiFree(pfHeight);
	}

	// �����ļ�
	NiDelete pkFile;
	pkFile = NULL;

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::LoadHeightMap(sdHeightMap* pkHeightMap)
{
	NIASSERT(pkHeightMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_HEIGHTMAP_FILENAME);

	// ���ļ�
	NiFile* pkFile = NiFile::GetFile(szFileFullPath, NiFile::READ_ONLY);
	NIASSERT(pkFile)

	// ��ȡ�ļ�����
	if (m_bCompactHeightMap)
	{
		// ����������ȡ���ݣ���ѹ��
		uint uiSize = pkHeightMap->GetSize();
		ushort* pusHeight = NiAlloc2(ushort, uiSize * uiSize, NiMemHint::NONE);
		pkFile->Read(pusHeight, uiSize * uiSize * sizeof(ushort));

		float fMinHeight = pkHeightMap->GetMinHeight();
		float fMaxHeight = pkHeightMap->GetMaxHeight();
		float fFactor = (fMaxHeight - fMinHeight) / 65535.f;
		ushort* pusCurHeight = pusHeight;
		for (uint uiY = 0; uiY < uiSize; ++uiY)
		{
			for (uint uiX = 0; uiX < uiSize; ++uiX)
			{
				float fHeight = *pusCurHeight++ * fFactor + fMinHeight;
				pkHeightMap->SetRawHeight(uiX, uiY, fHeight);
			}
		}

		NiFree(pusHeight);
		pusHeight = NULL;
	}
	else
	{
		// ����һ��ֱ����ȡ����
		uint uiSize = pkHeightMap->GetSize();
		float* pfHeight = NiAlloc2(float, uiSize * uiSize, NiMemHint::NONE);
		pkFile->Read(pfHeight, uiSize * uiSize * sizeof(float));

		float* pfCurHeight = pfHeight;
		for (uint uiY = 0; uiY < uiSize; ++uiY)
		{
			for (uint uiX = 0; uiX < uiSize; ++uiX)
			{
				pkHeightMap->SetRawHeight(uiX, uiY, *pfCurHeight++);
			}
		}

		NiFree(pfHeight);
		pfHeight = NULL;
	}

	// �ر��ļ�
	NiDelete pkFile;
	pkFile = NULL;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveNormalMap(sdNormalMap* pkNormalMap)
{
	NIASSERT(pkNormalMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMALMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����
	NiTexture* spGBNormalMap = pkNormalMap->GetGBTexture();
	NIASSERT(spGBNormalMap);

	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spGBNormalMap->GetRendererData();
	NIASSERT(spDX9TextureData);

	LPDIRECT3DBASETEXTURE9 spD3DBaseTexture = (LPDIRECT3DBASETEXTURE9)spDX9TextureData->GetD3DTexture();
	NIASSERT(spD3DBaseTexture);

	// ����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
sdNormalMapPtr sdTerrainStream::LoadNormalMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMALMAP_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	sdNormalMapPtr pkNormalMap = NiNew sdNormalMap(spTexture);
	NIASSERT(pkNormalMap);

	return pkNormalMap;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveLayerMapConfig(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_LAYERMAP_LIST_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ���ļ�
	sdLuaWriteUtil kConfigFile;
	if (!kConfigFile.OpenFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// д���ļ�
	sdTerrain::LayerMapVec& kLayerMapVec = pkTerrain->m_kLayerMapVec;
	sdTerrain::LayerMapVecItr itr = kLayerMapVec.begin();
	sdTerrain::LayerMapVecItr itr_end = kLayerMapVec.end();

	kConfigFile.WriteRootNodeBegin("Terrain");
	kConfigFile.WriteNodeBegin("LayerMap");
	kConfigFile.WriteData("Count", kLayerMapVec.size(), 15);
	kConfigFile.WriteNodeBegin("Layer");	
	for (; itr != itr_end; ++itr)
	{
		kConfigFile.LoopInnerBegin();
		kConfigFile.WriteData("Name", (*itr)->GetName(), 15);
		kConfigFile.WriteData("DiffuseMap", (*itr)->GetDiffuseMapPath(), 15, true);
		kConfigFile.WriteData("NormalMap", (*itr)->GetNormalMapPath(), 15, true);
		kConfigFile.WriteData("RepeatSize", (*itr)->GetRepeatSize(), 15);
		kConfigFile.WriteData("MipmapBias", (*itr)->GetMipmapBias(), 15);
		kConfigFile.LoopInnerEnd();
	}	
	kConfigFile.WriteNodeEnd();
	kConfigFile.WriteNodeEnd();
	kConfigFile.WriteRootNodeEnd();

	// �����ļ�
	kConfigFile.CloseFile();

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::LoadLayerMapConfig(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_LAYERMAP_LIST_FILENAME);

	// ���ļ�
	sdLuaReadUtil kConfigFile;
	if (!kConfigFile.LoadFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// ��ȡ�ļ�����
	kConfigFile.ReadRootNode("Terrain");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	kConfigFile.BeginReadNode("LayerMap");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	uint uiLayerCount = 0;
	kConfigFile.ReadData("Count", uiLayerCount);

	kConfigFile.BeginReadNode("Layer");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	float fRepeatSize;
	float fMipmapBias;
	char szName[MAX_LAYER_NAME_LEN];
	char szDiffuseMapName[MAX_PATH_LEN];
	char szNormalMapName[MAX_PATH_LEN];
	int iIndex = kConfigFile.LoopReset();
	while (kConfigFile.LoopNext(iIndex))
	{
		szName[0] = '\0';
		kConfigFile.ReadData("Name", szName, MAX_LAYER_NAME_LEN);

		szDiffuseMapName[0] = '\0';
		kConfigFile.ReadData("DiffuseMap", szDiffuseMapName, MAX_PATH_LEN);

		szNormalMapName[0] = '\0';
		kConfigFile.ReadData("NormalMap", szNormalMapName, MAX_PATH_LEN);

		fRepeatSize = 1.f;
		kConfigFile.ReadData("RepeatSize", fRepeatSize);

		fMipmapBias = 1.f;
		kConfigFile.ReadData("MipmapBias", fMipmapBias);

		kConfigFile.LoopInnerEnd();

		sdLayerMapPtr pkLayerMap = NiNew sdLayerMap(szName, szDiffuseMapName, szNormalMapName, fRepeatSize, fMipmapBias);
		NIASSERT(pkLayerMap);
		pkTerrain->AppendLayerMap(pkLayerMap);
	}

	kConfigFile.EndReadNode();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveLayerMap(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_LAYERMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// �����ļ�
	NiFile* pkFile = NiFile::GetFile(szFileFullPath, NiFile::WRITE_ONLY);
	NIASSERT(pkFile)

	// ��ȡ����,д���ļ�
	uint uiBlendTexSize = pkTerrain->GetBlendMapSize();
	sdTerrain::LayerAlphaVec& kLayerAlphaVec = pkTerrain->m_kLayerAlphaVec;
	sdTerrain::LayerAlphaVecItr itr = kLayerAlphaVec.begin();
	sdTerrain::LayerAlphaVecItr itr_end = kLayerAlphaVec.end();
	for (; itr != itr_end; ++itr)
	{
		pkFile->Write((*itr), uiBlendTexSize * uiBlendTexSize);
	}

	// �����ļ�
	NiDelete pkFile;
	pkFile = NULL;

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::LoadLayerMap(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_LAYERMAP_FILENAME);

	// ���ļ�,��ȡ�ļ�����
	NiFile* pkFile = NiFile::GetFile(szFileFullPath, NiFile::READ_ONLY);
	NIASSERT(pkFile)

	uint uiBlendTexSize = pkTerrain->GetBlendMapSize();
	sdTerrain::LayerAlphaVec& kLayerAlphaVec = pkTerrain->m_kLayerAlphaVec;
	sdTerrain::LayerAlphaVecItr itr = kLayerAlphaVec.begin();
	sdTerrain::LayerAlphaVecItr itr_end = kLayerAlphaVec.end();
	for (; itr != itr_end; ++itr)
	{
		pkFile->Read((*itr), uiBlendTexSize * uiBlendTexSize);
	}
	
	NiDelete pkFile;
	pkFile = NULL;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveBlendMap(sdDynamicTexture* pkDynamicBlendMap)
{
	NIASSERT(pkDynamicBlendMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_BLENDMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����(ʹ��ϵͳ�ڴ濽������ֹ����߳̿�Ĳ�����)
	LPDIRECT3DTEXTURE9 d3dBaseTexture = pkDynamicBlendMap->GetSysMemTexture();
	NIASSERT(d3dBaseTexture);

	// ����һ��ֱ��ʹ��D3DX����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, d3dBaseTexture, NULL);

//	// ���������ֹ�����
//	// @{
//	D3DSURFACE_DESC d3dSurfaceDesc;
//	d3dBaseTexture->GetLevelDesc(0, &d3dSurfaceDesc);
//
//	DDSHeader kDDSHeader;
//	kDDSHeader.SetWidth(d3dSurfaceDesc.Width);
//	kDDSHeader.SetHeight(d3dSurfaceDesc.Height);
//	kDDSHeader.SetMipmapCount(0);
//	kDDSHeader.pf.size = 32;
//	kDDSHeader.pf.flags = 65;
//	kDDSHeader.pf.bitCount = 16;	///< A4R4G4B4
//	kDDSHeader.pf.rmask = 0x0f00;
//	kDDSHeader.pf.gmask = 0x00f0;
//	kDDSHeader.pf.bmask = 0x000f;
//	kDDSHeader.pf.amask = 0xf000;
//	kDDSHeader.caps1 = 0x1002;		///< DDSCAPS_TEXTURE | DDSCAPS_ALPHA
//
//	FILE* fp = fopen(szFileFullPath, "wb");
//	fwrite(&kDDSHeader.fourcc, sizeof(unsigned int), 1, fp);	///< Magic Number
//
//	fwrite(&kDDSHeader.size, sizeof(unsigned int), 1, fp);		///< DDSURFACEDESC2
//	fwrite(&kDDSHeader.flags, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.height, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.width, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.pitch, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.depth, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.mipmapCount, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.reserved[0], sizeof(unsigned int), 11, fp);
//	fwrite(&kDDSHeader.width, sizeof(unsigned int), 1, fp);
//
//	fwrite(&kDDSHeader.pf.flags, sizeof(unsigned int), 1, fp);	///< DDSURFACEDESC2��ǶDDPIXELFORMAT
//	fwrite(&kDDSHeader.pf.size, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.flags, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.fourcc, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.bitCount, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.rmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.gmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.bmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.amask, sizeof(unsigned int), 1, fp);	
//
//	fwrite(&kDDSHeader.caps1, sizeof(unsigned int), 1, fp);		///< DDSURFACEDESC2��ǶDDSCAPS2
//	fwrite(&kDDSHeader.caps2, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.caps3, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.caps4, sizeof(unsigned int), 1, fp);	
//
//	fwrite(&kDDSHeader.reserved2, sizeof(unsigned int), 1, fp);	///< DDSURFACEDESC2
//
//	D3DLOCKED_RECT d3dLockedRect;
//	d3dBaseTexture->LockRect(0, &d3dLockedRect, NULL, D3DLOCK_READONLY);
//
//	uchar* pucSrcData = (uchar*)(d3dLockedRect.pBits);
//	for (uint y = 0; y < d3dSurfaceDesc.Height; ++y)
//		fwrite(pucSrcData + y * d3dLockedRect.Pitch, d3dSurfaceDesc.Width * 2, 1, fp);
//
//	d3dBaseTexture->UnlockRect(0);
//	fclose(fp);
//	fp = NULL;
//	// @}

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadBlendMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_BLENDMAP_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveTileMap(sdDynamicTexture* pkDynamicTileMap)
{
	NIASSERT(pkDynamicTileMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_TILEMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����(ʹ��ϵͳ�ڴ濽������ֹ����߳̿�Ĳ�����)
	LPDIRECT3DTEXTURE9 spD3DBaseTexture = pkDynamicTileMap->GetSysMemTexture();
	NIASSERT(spD3DBaseTexture);

	// ����һ������D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

//	// ���������ֹ�����
//	// @{
//	D3DSURFACE_DESC d3dSurfaceDesc;
//	d3dBaseTexture->GetLevelDesc(0, &d3dSurfaceDesc);
//
//	DDSHeader kDDSHeader;
//	kDDSHeader.SetWidth(d3dSurfaceDesc.Width);
//	kDDSHeader.SetHeight(d3dSurfaceDesc.Height);
//	kDDSHeader.SetMipmapCount(0);
//	kDDSHeader.pf.size = 32;
//	kDDSHeader.pf.flags = 65;
//	kDDSHeader.pf.bitCount = 32;	///< A8R8G8B8
//	kDDSHeader.pf.rmask = 0x00ff0000;
//	kDDSHeader.pf.gmask = 0x0000ff00;
//	kDDSHeader.pf.bmask = 0x000000ff;
//	kDDSHeader.pf.amask = 0xff000000;
//	kDDSHeader.caps1 = 0x1002;		///< DDSCAPS_TEXTURE | DDSCAPS_ALPHA
//
//	FILE* fp = fopen(szFileFullPath, "wb");
//	fwrite(&kDDSHeader.fourcc, sizeof(unsigned int), 1, fp);	///< Magic Number
//
//	fwrite(&kDDSHeader.size, sizeof(unsigned int), 1, fp);		///< DDSURFACEDESC2
//	fwrite(&kDDSHeader.flags, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.height, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.width, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.pitch, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.depth, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.mipmapCount, sizeof(unsigned int), 1, fp);
//	fwrite(&kDDSHeader.reserved[0], sizeof(unsigned int), 11, fp);
//	fwrite(&kDDSHeader.width, sizeof(unsigned int), 1, fp);
//
//	fwrite(&kDDSHeader.pf.flags, sizeof(unsigned int), 1, fp);	///< DDSURFACEDESC2��ǶDDPIXELFORMAT
//	fwrite(&kDDSHeader.pf.size, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.flags, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.fourcc, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.bitCount, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.rmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.gmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.bmask, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.pf.amask, sizeof(unsigned int), 1, fp);	
//
//	fwrite(&kDDSHeader.caps1, sizeof(unsigned int), 1, fp);		///< DDSURFACEDESC2��ǶDDSCAPS2
//	fwrite(&kDDSHeader.caps2, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.caps3, sizeof(unsigned int), 1, fp);	
//	fwrite(&kDDSHeader.caps4, sizeof(unsigned int), 1, fp);	
//
//	fwrite(&kDDSHeader.reserved2, sizeof(unsigned int), 1, fp);	///< DDSURFACEDESC2
//
//	D3DLOCKED_RECT d3dLockedRect;
//	d3dBaseTexture->LockRect(0, &d3dLockedRect, NULL, D3DLOCK_READONLY);
//
//	uchar* pucSrcData = (uchar*)(d3dLockedRect.pBits);
//	for (uint y = 0; y < d3dSurfaceDesc.Height; ++y)
//		fwrite(pucSrcData + y * d3dLockedRect.Pitch, d3dSurfaceDesc.Width * 2, 1, fp);
//
//	d3dBaseTexture->UnlockRect(0);
//	fclose(fp);
//	fp = NULL;
//	// @}

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadTileMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_TILEMAP_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveAtlasConfig(Engine::sdTextureAtlas* pkDiffuseAtlas, Engine::sdTextureAtlas* pkNormalAtlas)
{
	NIASSERT(pkDiffuseAtlas);
	NIASSERT(pkNormalAtlas);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_ATLAS_CONFIG_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ���ļ�
	sdLuaWriteUtil kConfigFile;
	if (!kConfigFile.OpenFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// д���ļ�
	kConfigFile.WriteRootNodeBegin("Terrain");

	kConfigFile.WriteNodeBegin("DiffuseAtlas");
	pkDiffuseAtlas->Save(kConfigFile);
	kConfigFile.WriteNodeEnd();

	kConfigFile.WriteNodeBegin("NormalAtlas");
	pkNormalAtlas->Save(kConfigFile);
	kConfigFile.WriteNodeEnd();
	
	kConfigFile.WriteRootNodeEnd();

	// �����ļ�
	kConfigFile.CloseFile();

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::LoadAtlasConfig(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuffer);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_ATLAS_CONFIG_FILENAME);

	// ���ļ�
	sdLuaReadUtil kConfigFile;
	if (!kConfigFile.LoadFile(szFileFullPath))
	{
		NIASSERT(0);
		return false;
	}

	// ��ȡ�ļ�����
	kConfigFile.ReadRootNode("Terrain");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	// DiffuseMap
	kConfigFile.BeginReadNode("DiffuseAtlas");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	kConfigFile.BeginReadNode("TableInfo");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	NiPoint2 kDiffuseTexId2U, kDiffuseLevel2V;
	kConfigFile.ReadData("TexId2U", kDiffuseTexId2U);
	kConfigFile.ReadData("Level2V", kDiffuseLevel2V);
	
	kConfigFile.EndReadNode();
	kConfigFile.EndReadNode();

	// NormalMap
	kConfigFile.BeginReadNode("NormalAtlas");
	if (!kConfigFile.IsNodeValid())
	{
		NIASSERT(0);
		return false;
	}

	NiPoint2 kNormalTexId2U, kNormalLevel2V;
	kConfigFile.ReadData("TexId2U", kNormalTexId2U);
	kConfigFile.ReadData("Level2V", kNormalLevel2V);
	
	kConfigFile.EndReadNode();
	kConfigFile.EndReadNode();

	//
	pkTerrain->m_kRenderParams.diffuseAtlasTableParam.m_fX = kDiffuseTexId2U.GetX();
	pkTerrain->m_kRenderParams.diffuseAtlasTableParam.m_fY = kDiffuseTexId2U.GetY();
	pkTerrain->m_kRenderParams.diffuseAtlasTableParam.m_fZ = kDiffuseLevel2V.GetX();
	pkTerrain->m_kRenderParams.diffuseAtlasTableParam.m_fW = kDiffuseLevel2V.GetY();

	pkTerrain->m_kRenderParams.normalAtlasTableParam.m_fX = kNormalTexId2U.GetX();
	pkTerrain->m_kRenderParams.normalAtlasTableParam.m_fY = kNormalTexId2U.GetY();
	pkTerrain->m_kRenderParams.normalAtlasTableParam.m_fZ = kNormalLevel2V.GetX();
	pkTerrain->m_kRenderParams.normalAtlasTableParam.m_fW = kNormalLevel2V.GetY();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveDiffuseAtlasMap(NiTexture* spDiffuseMap)
{
	NIASSERT(spDiffuseMap);;

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// ·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_DIFFUSE_ATLASMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����
	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spDiffuseMap->GetRendererData();
	NIASSERT(spDX9TextureData);

	LPDIRECT3DBASETEXTURE9 spD3DBaseTexture = (LPDIRECT3DBASETEXTURE9)spDX9TextureData->GetD3DTexture();
	NIASSERT(spD3DBaseTexture);

	// ����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveDiffuseAtlasTableMap(NiTexture* spDiffuseTableMap)
{
	NIASSERT(spDiffuseTableMap);;

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// ·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_DIFFUSE_ATLASTABLE_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����
	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spDiffuseTableMap->GetRendererData();
	NIASSERT(spDX9TextureData);

	LPDIRECT3DBASETEXTURE9 spD3DBaseTexture = (LPDIRECT3DBASETEXTURE9)spDX9TextureData->GetD3DTexture();
	NIASSERT(spD3DBaseTexture);

	// ����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadDiffuseAtlasMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_DIFFUSE_ATLASMAP_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadDiffuseAtlasTableMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_DIFFUSE_ATLASTABLE_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveNormalAtlasMap(NiTexture* spNormalMap)
{
	NIASSERT(spNormalMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// ·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMAL_ATLASMAP_FILENAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����
	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spNormalMap->GetRendererData();
	NIASSERT(spDX9TextureData);

	LPDIRECT3DBASETEXTURE9 spD3DBaseTexture = (LPDIRECT3DBASETEXTURE9)spDX9TextureData->GetD3DTexture();
	NIASSERT(spD3DBaseTexture);

	// ����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainStream::SaveNormalAtlasTableMap(NiTexture* spNormalTableMap)
{
	NIASSERT(spNormalTableMap);

	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// ȷ��·������
	sdFileUtil::ConfimDir(szBuffer);

	// ·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMAL_ATLASTABLE_FILENAME);


	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// ��ȡD3D����
	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spNormalTableMap->GetRendererData();
	NIASSERT(spDX9TextureData);

	LPDIRECT3DBASETEXTURE9 spD3DBaseTexture = (LPDIRECT3DBASETEXTURE9)spDX9TextureData->GetD3DTexture();
	NIASSERT(spD3DBaseTexture);

	// ����D3D����
	D3DXSaveTextureToFile(szFileFullPath, D3DXIFF_DDS, spD3DBaseTexture, NULL);

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadNormalAtlasMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMAL_ATLASMAP_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdTerrainStream::LoadNormalAtlasTableMap()
{
	// �����ļ���·��
	char szBuffer[MAX_PATH];
	memset(szBuffer, 0, MAX_PATH * sizeof(char));
	strcpy(szBuffer, m_szSceneFullPath);
	strcat(szBuffer, "\\");
	strcat(szBuffer, TERRAIN_DIRECTORY);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuffer);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, TERRAIN_NORMAL_ATLASTABLE_FILENAME);

	// �����ļ�
	NiDevImageConverter kDevImageConverter;
	NiPixelDataPtr spPixelData = kDevImageConverter.ReadImageFile(szFileFullPath, 0);
	NIASSERT(spPixelData);

	// �����������
	NiTexture::FormatPrefs kPerfs;
	kPerfs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPerfs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiTexturePtr spTexture = NiSourceTexture::Create(spPixelData);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------