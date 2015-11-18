//*************************************************************************************************
// 内容:	地形拾取
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-29
// 最后修改:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TERRAIN_PICK_H__
#define _SD_ENGINE_TERRAIN_PICK_H__
#include "sdHeightMap.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
class sdVector3;
class sdRay;
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE

//
// 地形拾取,基本原理是对射线经过的每一个高度格子作相交判断
// 常用方法有:
//	1.不进行预处理,从Ray与AABB起始交点到终止交点进行固定步长微分查找
//	2.不进行预处理,从Ray与AABB起始交点到终止交点进行Bresenham微分查找
//	3.预处理为QuadTree的AABB
//	4.预处理为粗粒度HeightMap
// 
// 这里使用第一种--土法,类设计上参考了NiPick
//
class sdTerrainPick : public NiRefObject
{
public:
	sdTerrainPick();
	sdTerrainPick(sdHeightMap* pkHeightMap);

	// 拾取,输入坐标系是HeightMap坐标系的浮点形式
	bool Pick(const Base::Math::sdRay& kRay, Base::Math::sdVector3& kIntersect);

	// 拾取设置
	void SetHeightMap(sdHeightMap* pkHeightMap){NIASSERT(pkHeightMap);m_pkHeightMap = pkHeightMap;}
	void SetCullBack(bool bCullBack){ m_bCullBack = bCullBack;}
	void SetMaxDistance(float fMaxDistance){ NIASSERT(fMaxDistance >= 0); m_fMaxDistance = fMaxDistance;}

protected:
	// 射线与矩形相交
	bool Intersect(uint uiX, uint uiY, const Base::Math::sdRay& kRay, Base::Math::sdVector3& kIntersect);
	
	// 射线与三角形求交
	bool Intersect(const Base::Math::sdVector3& kV1, 
		const Base::Math::sdVector3& kV2, 
		const Base::Math::sdVector3& kV3,
		const Base::Math::sdRay& kRay,
		Base::Math::sdVector3& kIntersect);

protected:
	sdHeightMapPtr m_pkHeightMap;
	bool	m_bCullBack;			///< 是否拾取背面
	float	m_fMaxDistance;			///< 最大拾取距离
};
NiSmartPointer(sdTerrainPick);
#endif