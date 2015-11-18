//*************************************************************************************************
// 内容:	地表法线图
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-08
// 最后修改:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_NORMAL_MAP_H__
#define _TERRAIN_NORMAL_MAP_H__
#include "sdDynamicTexture.h"

class sdHeightMap;

//-------------------------------------------------------------------------------------------------
// 地表法线图,与HeightMap密度相同,所以在上边缘处的法线是没有存储的
//	1.可以避免LOD变化引起的光照变化
//-------------------------------------------------------------------------------------------------
class sdNormalMap : public NiRefObject
{
public:
	sdNormalMap(uint uiSize, sdHeightMap* pkHeightMap);
	sdNormalMap(NiTexture* spNormalTexture);
	~sdNormalMap();

	// 更新,分别是整体更新/更新指定区域(用于编辑状态)
	void	Update();
	void	Update(int iX, int iY, int iW, int iH);

	// 属性访问 
	NiTexture*	GetGBTexture() const;

protected:
	// 高度图
	sdHeightMap* m_pkHeightMap;	

	// NormalMap的纹理
	Engine::sdDynamicTexturePtr m_pkDynamicNormalTexture;	///< 用于编辑状态,以提供动态更新
	NiTexturePtr m_spNormalTexture;							///< 用于游戏状态,仅供渲染

	// 坡度阈值
	NiPoint3	m_kPlanarThreshold;
};
NiSmartPointer(sdNormalMap);
//-------------------------------------------------------------------------------------------------
#endif