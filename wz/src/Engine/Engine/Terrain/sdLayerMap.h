//*************************************************************************************************
// 内容:	地表资源图层
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_LAYER_MAP_H__
#define _TERRAIN_LAYER_MAP_H__
#include "sdMemoryTexture.h"

// 最大图层名长度
#define MAX_LAYER_NAME_LEN 32
//-------------------------------------------------------------------------------------------------
// 表示地表一层资源贴图(仅仅在编辑器状态下使用,相当于Cry的SSurfaceType)
//	\TODO 统一纹理加载到ResourceSystem
//	\TODO 记得BigWorld有UVMapType\Rotate\Translate\Scale参数
//	\TODO WZ中把贴图在加载时动态转换为DXT1格式以减少内存使用
//-------------------------------------------------------------------------------------------------
class sdLayerMap : public NiRefObject
{
public:
	sdLayerMap(const char* szName, const char* szDiffuseMap, const char* szNormalMap, 
		float fRepeatSize = 1.0f, float fMipmapBias = 0.0f);
	~sdLayerMap();

	const char*	GetName() const { return m_szName;}
	void		SetName(const char* szName) { NIASSERT(szName); strncpy(m_szName, szName, MAX_LAYER_NAME_LEN);}

	const char*	GetDiffuseMapPath() const { return m_szDiffuseMap;}
	void		SetDiffuseMapPath(const char* szName);

	const char*	GetNormalMapPath() const { return m_szNormalMap;}
	void		SetNormalMapPath(const char* szName);

	NiTexture*	GetDiffuseMap() { return m_pkDiffuseMap ? m_pkDiffuseMap->GetGBTexture() : NULL;}
	NiTexture*	GetNormalMap() { return m_pkNormalMap ? m_pkNormalMap->GetGBTexture() : NULL;}

	float	GetRepeatSize() const { return m_fRepeatSize;}
	void	SetRepeatSize(float fRepeatSize) { m_fRepeatSize = fRepeatSize; }

	float	GetMipmapBias() const { return m_fMipmapBias;};
	void	SetMipmapBias(float fMipmapBias) { m_fMipmapBias = fMipmapBias; }

protected:
	// 加载数据到Texture
	Engine::sdMemoryTexturePtr LoadPixelDataToD3DTexture(const char* szName);

protected:
	// 图层名
	char m_szName[MAX_LAYER_NAME_LEN];			

	// 图层纹理
	char m_szDiffuseMap[MAX_PATH_LEN];	
	char m_szNormalMap[MAX_PATH_LEN];
	Engine::sdMemoryTexturePtr m_pkDiffuseMap;
	Engine::sdMemoryTexturePtr m_pkNormalMap;

	// 图层信息
	float	m_fRepeatSize;
	float	m_fMipmapBias;
};
NiSmartPointer(sdLayerMap);
//-------------------------------------------------------------------------------------------------
#endif