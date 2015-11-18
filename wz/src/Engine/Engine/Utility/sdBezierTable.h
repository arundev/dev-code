//*************************************************************************************************
// 内容:	
//		Bezier插值
//
// 资料:
//		0.这里使用的是Cubic Bezier Spline
//		1.[0,1]区间的内插
//		 已知f(t), 其中
//				f(0)  = p0,		f(1)  = p3, 
//		 则,
//				f(t) = (1 - t) * (1 - t) * (1 - t) * p0 + 
//					   (1 - t) * (1 - t) * t * 3 * p1 +
//					   (1 - t) * t * t * 3 * p2 +
//					   t * t * t * p3;
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Bézier curve
//		https://en.wikipedia.org/wiki/De Casteljau's algorithm	这里没有使用该算法
//---------------------------------------------------------
// 作者:		
// 创建:		2012-01-17
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_BEZIER_SHAPE_H__
#define _SD_ENGINE_UTIL_BEZIER_SHAPE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL

class sdBezierTable : public NiRefObject
{
public:

protected:
	
	// 从控制点生成插值点
	void UpdateTable(int iLockHandle);

protected:
	std::vector<uchar> m_kTable;
	std::vector<uchar> m_kLockedIndiceTable;
};
NiSmartPointer(sdBezierTable);

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif