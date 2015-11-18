//*************************************************************************************************
// 内容:	
//		Hermite插值
//
// 资料:
//		0.这里使用的是Cubic Hermite Spline
//		1.[0,1]区间的内插
//		 已知f(t), 其中
//				f(0)  = p0,		f(1)  = p1, 
//				f`(0) = m0,		f`(1) = m1,
//		 则,
//				f(t) = (2 * t * t * t - 3 * t * t + 1) * p0 + 
//					   (t * t * t - 2 * t * t + t) * m0 +
//					   (-2 * t * t * t + 3 * t * t) * p1 +
//					   (t * t * t - t * t) * m1
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Hermite_form
//		https://en.wikipedia.org/wiki/Hermite_interpolation
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-27
// 最后修改:	2012-12-27
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_HERMITE_SHAPE_H__
#define _SD_ENGINE_UTIL_HERMITE_SHAPE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL

class sdHermiteShape : public NiRefObject
{
public:
	sdHermiteShape();
	~sdHermiteShape();

	// 控制点数组编辑
	inline int AddPoint(const NiPoint3& kPos);
	inline void	SetPoint(int iIndex, const NiPoint3& kPos);
	inline void	SetPoints(const std::vector<NiPoint3>& kPoint3Vec);
	inline void	RemovePoint(int iIndex);
	inline void	RemoveAllPoints();
	inline NiPoint3& GetPoint(int iIndex);
	inline std::vector<NiPoint3>& GetPoints();
	inline const std::vector<NiPoint3>& GetPoints() const;

	// 插值点数组访问
	inline std::vector<NiPoint3>& GetInterPoints();
	inline const std::vector<NiPoint3>& GetInterPoints() const;

	// 扩展点数组访问
	inline std::vector<NiPoint3>& GetExtendPoints();
	inline const std::vector<NiPoint3>& GetExtendPoints() const;

	// 插值点扩展半径
	inline void SetExtendWidth(float fWidth);
	inline float GetExtendWidth();

	// 两控制点之间最大插值点数量
	inline void SetMaxInterNum(int iNum);
	inline int GetMaxInterNum();

	// 从控制点生成插值点
	bool Interpolation();

	// 扩展插值点
	bool ExtentInterPoint();
	
	// 获取使用的内存的尺寸
	//virtual uint GetMemSize();

protected:
	// (vector在使用下标访问时会检查越界,考虑使用时用指针来访问)
	typedef std::vector<NiPoint3> Point3Vec;
	typedef std::vector<NiPoint3>::iterator Point3VecItr;
	Point3Vec	m_kCtrlPointVec;	///< 原始控制点
	Point3Vec	m_kInterPointVec;	///< 插值后的中心点
	Point3Vec	m_kExtendPointVec;	///< 完整扩展后的顶点

	float m_fExtendWidth;	///< 插值点扩展宽度
	int	m_iMaxInterNum;		///< 两控制点之间最大插值点数量,为-1则不限制
};
NiSmartPointer(sdHermiteShape);

#include "sdHermiteShape.inl"

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif