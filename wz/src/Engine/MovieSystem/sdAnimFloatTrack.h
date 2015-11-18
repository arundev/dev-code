//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-22
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_FLOAT_TRACK_H__
#define _MOVIE_SYSTEM_ANIM_FLOAT_TRACK_H__

#include "sdAnimTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
class sdAnimFloatTrack : public TAnimTrack<sdFloatKey>
{
public:
	sdAnimFloatTrack(){};
	virtual ~sdAnimFloatTrack(){};

	// 虚函数继承
	virtual inline EAnimTrackType GetType() { return E_ANIM_TRACK_FLOAT; }

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);

	virtual void GetValue(float fTime, float& fValue);
	virtual void GetValue(int iIndex, float& fValue);
	virtual void SetValue(float fTime, float fValue);
	virtual void SetValue(int iIndex, float fValue);
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif