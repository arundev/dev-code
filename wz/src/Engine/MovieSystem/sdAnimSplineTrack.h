//*************************************************************************************************
// 内容:	AnimKey与AnimSplineTrack(貌似参考自CryEngine的CryMovieSystem)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-07
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_SPLINE_TRACK_H__
#define _MOVIE_SYSTEM_ANIM_SPLINE_TRACK_H__

#include <IAnimTrack.h>
#include <sdLuaFile.h>

#include "sdSplineInterpolator.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
//class I2DBezierKey : public IKey
//class ITcbKey : public IKey
//-----------------------------------------------------------------------------
class sdBezierKey : public IKey
{
	
};
//-----------------------------------------------------------------------------
// TAnimSplineTrack
//-----------------------------------------------------------------------------
template<typename KeyType>
class TAnimSplineTrack : public IAnimTrack
{
public:
	TAnimSplineTrack();
	virtual ~TAnimSplineTrack();

	// 虚函数继承
	virtual inline EAnimTrackType GetType(){ return E_ANIM_TRACK_DEFAULT;}

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream){	return true; }
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream){ return true; }
	virtual inline void Clear();

	virtual inline int CreateKey(float fTime);
	virtual inline int CloneKey(int iFromKey);
	virtual inline int CopyKey(IAnimTrack* pkFromTrack, int iFromKey);


protected:
	typedef TTrackSplineInterpolator<KeyType> SplineInterpolator;
	SplineInterpolator*	m_pkSplineInterpolator;	///< 样条插值器
	KeyType m_kDefaultValue;
	int		m_iFlags;
};
//-----------------------------------------------------------------------------
#include "sdAnimSplineTrack.inl"
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif