//*************************************************************************************************
// ����:	���������������뱣��
//---------------------------------------------------------
// ����:		
// ����:		2012-09-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TERRAIN_STREAM_H__
#define _SD_ENGINE_TERRAIN_STREAM_H__
#include "sdHeightMap.h"
#include "sdNormalMap.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdDynamicTexture;
class sdTextureAtlas;
ENGINE_NAMESPACE_END_ENGINE

class sdTerrain;
//-------------------------------------------------------------------------------------------------
// ���ڶ����ı�������ص�������
//  \TODO	���ǸĽ�ΪGB��Stream��ʽ
//	\TODO	���ǸĽ�Ϊ�������ʽ�����Ǿ�̬��������
//	\TODO	�ֶ��洢ΪDDS��ʽ
//	\TODO	�м�����ѹ��ΪDDS��ʽ(DetailAlpha/LightMap)
//	\TODO	��̬�ؽ�AtlasMap
//-------------------------------------------------------------------------------------------------
class sdTerrainStream : public NiRefObject
{
public:
	sdTerrainStream(const char* szSceneFullPath);

	// �����뱣��������Ϣ
	bool SaveConfig(sdTerrain* pkTerrain);
	bool LoadConfig(sdTerrain* pkTerrain);

	// �����뱣��߶�ͼ
	bool SaveHeightMapConfig(sdHeightMap* pkHeightMap);
	sdHeightMapPtr LoadHeightMapConfig();

	bool SaveHeightMap(sdHeightMap* pkHeightMap);
	bool LoadHeightMap(sdHeightMap* pkHeightMap);

	// �����뱣�淨����ͼ
	bool SaveNormalMap(sdNormalMap* pkNormalMap);
	sdNormalMapPtr LoadNormalMap();

	// ���غͱ���ͼ���ļ�
	bool SaveLayerMapConfig(sdTerrain* pkTerrain);
	bool LoadLayerMapConfig(sdTerrain* pkTerrain);

	bool SaveLayerMap(sdTerrain* pkTerrain);
	bool LoadLayerMap(sdTerrain* pkTerrain);

	// ���غͱ�������ͼ
	// (����ʹ��D3DX������,WZԭʼ��ʽ���Լ��ֹ�����)
	bool SaveBlendMap(Engine::sdDynamicTexture* pkDynamicBlendMap);
	NiTexturePtr LoadBlendMap();

	// ���غͱ�����������ͼ
	// (����ʹ��D3DX������,WZԭʼ��ʽ���Լ��ֹ�����)
	bool SaveTileMap(Engine::sdDynamicTexture* pkDynamicTileMap);
	NiTexturePtr LoadTileMap();

	// �����뱣����������ͼ���뷨����ͼ������
	bool SaveAtlasConfig(Engine::sdTextureAtlas* pkDiffuseAtlas, Engine::sdTextureAtlas* pkNormalAtlas);
	bool LoadAtlasConfig(sdTerrain* pkTerrain);

	// ���غͱ�����������ͼ���뷨����ͼ������
	bool SaveDiffuseAtlasMap(NiTexture* spDiffuseMap);
	bool SaveDiffuseAtlasTableMap(NiTexture* spDiffuseTableMap);
	NiTexturePtr LoadDiffuseAtlasMap();
	NiTexturePtr LoadDiffuseAtlasTableMap();

	// ���غͱ�������ͼ������ͼ����
	bool SaveNormalAtlasMap(NiTexture* spNormalMap);
	bool SaveNormalAtlasTableMap(NiTexture* spNormalTableMap);
	NiTexturePtr LoadNormalAtlasMap();
	NiTexturePtr LoadNormalAtlasTableMap();

protected:
	char m_szSceneFullPath[MAX_PATH];	///< ����ȫ·��
	bool m_bCompactHeightMap;			///< �Ƿ�ʹ��ѹ���߶�ͼ(�ڲ����ݱ���)
};
NiSmartPointer(sdTerrainStream);
//-------------------------------------------------------------------------------------------------
#endif