//*************************************************************************************************
// 内容:	样条曲线(来自CryEngine/Common/ISpline.h)
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Interpolation
//		https://en.wikipedia.org/wiki/Spline_interpolation
//		https://en.wikipedia.org/wiki/Spline_(mathematics)
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//		https://en.wikipedia.org/wiki/Kochanek–Bartels spline
//		https://en.wikipedia.org/wiki/Bézier curve
//		https://en.wikipedia.org/wiki/B-spline
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-07
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_SPLINE_INTERPOLATOR_H__
#define _MOVIE_SYSTEM_ANIM_SPLINE_INTERPOLATOR_H__

#include <IAnimInterpolator.h>
#include "sdSpline.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// TBaseSplineInterpolator, 样条插值器
//-----------------------------------------------------------------------------
template <class value_type, class spline_type>
class TBaseSplineInterpolator : public ISplineInterpolator, public spline_type
{
protected:
	static const int DIM = sizeof(value_type) / sizeof(ElemType);	///< 数据维度

public:
	//
	inline void ToValueType(const value_type& kLocalValue, ValueType &kValue) { *(value_type*)v = kLocalValue; }
	inline void FromValueType(ValueType v, value_type& t) { t = *(value_type*)v; }
	
	// 虚函数继承
	virtual int GetNumDimensions();
	virtual ESplineType GetSplineType();

	virtual int InsertKey(float fTime, ValueType kValue);
	virtual void RemoveKey(int iKey);
	virtual void Clear();

	virtual void FindKeysInRange(float fStartTime, float fEndTime, int& iFirstFoundKey, int& iNumFoundKeys);
	virtual void RemoveKeysInRange(float fStartTime, float fEndTime);

	virtual int		GetKeyCount();
	virtual float	GetKeyTime(int iKey);
	virtual bool	GetKeyValue(int iKey,ValueType &kValue);
	virtual void	SetKeyValue(int iKey,ValueType &kValue);
	virtual void	SetKeyTime(int iKey, float fTime);

	virtual void	SetKeyInTangent(int iKey, ValueType kTin);
	virtual void	SetKeyOutTangent(int iKey, ValueType kTout);
	virtual void	SetKeyTangents(int iKey, ValueType kTin, ValueType kTout);
	virtual bool	GetKeyTangents(int iKey, ValueType &kTin, ValueType &kTout);

	virtual void	SetKeyFlags(int iKey, int iFlags);
	virtual int		GetKeyFlags(int iKey);

	virtual void	Interpolate(float fTime, ValueType &kValue);
};
//-----------------------------------------------------------------------------
// TBaseSplineInterpolator, 样条插值器
//-----------------------------------------------------------------------------
template <class T>
class TTrackSplineInterpolator : public TBaseSplineInterpolator<T, TBezierSpline<T, TBezierSplineKey<T> > >
{


};
//-----------------------------------------------------------------------------
#include "sdSplineInterpolator.h"
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif