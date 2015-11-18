//*************************************************************************************************
// 内容:	插值器接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-22
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIM_INTERPOLATOR_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIM_INTERPOLATOR_H__

#include "IAnimSpline.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// IInterpolator
//-----------------------------------------------------------------------------
class IInterpolator : public NiRefObject
{
public:
	virtual ~IInterpolator() {};
};
NiSmartPointer(IInterpolator);

//-----------------------------------------------------------------------------
// ISplineInterpolator
//-----------------------------------------------------------------------------
class ISplineInterpolator : public IInterpolator
{
public:
	typedef float ElemType;
	typedef ElemType ValueType[4];

public:
	virtual ~ISplineInterpolator() {};

	// Dimension of the spline from 0 to 3, number of parameters used in ValueType.
	virtual int GetNumDimensions() = 0;

	// Get Spline type 
	virtual ESplineType GetSplineType() = 0;

	// 键操作
	virtual int  InsertKey(float fTime, ValueType kValue) = 0;
	virtual void RemoveKey(int iKey) = 0;
	virtual void Clear() = 0;

	virtual void FindKeysInRange(float fStartTime, float fEndTime, int& iFirstFoundKey, int& iNumFoundKeys) = 0;
	virtual void RemoveKeysInRange(float fStartTime, float fEndTime) = 0;

	virtual int   GetKeyCount() = 0;
	virtual void  SetKeyTime(int iKey, float fTime) = 0;
	virtual float GetKeyTime(int iKey) = 0;
	virtual void  SetKeyValue(int iKey, ValueType kValue) = 0;
	virtual bool  GetKeyValue(int iKey, ValueType &kValue) = 0;

	virtual void  SetKeyInTangent(int iKey, ValueType kTin) = 0;
	virtual void  SetKeyOutTangent(int iKey, ValueType kTout) = 0;
	virtual void  SetKeyTangents(int iKey, ValueType kTin, ValueType kTout) = 0;
	virtual bool  GetKeyTangents(int iKey, ValueType &kTin, ValueType &kTout) = 0;

	virtual void  SetKeyFlags(int iKey, int iFlags) = 0;
	virtual int   GetKeyFlags(int iKey) = 0;

	// 获取插值
	virtual void Interpolate(float fTime, ValueType &kValue) = 0;
};
NiSmartPointer(ISplineInterpolator);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif