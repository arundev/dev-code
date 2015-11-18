//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-07
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_BOOL_TRACK_H__
#define _MOVIE_SYSTEM_ANIM_BOOL_TRACK_H__

#include "sdAnimTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
class sdAnimBoolTrack : public TAnimTrack<sdBoolKey>
{
public:
	sdAnimBoolTrack(){};
	virtual ~sdAnimBoolTrack(){};

	// 虚函数继承
	virtual inline EAnimTrackType GetType() { return E_ANIM_TRACK_BOOL; }

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);

	virtual void GetValue(float fTime, bool& bValue);
	virtual void GetValue(int iIndex, bool& bValue);
	virtual void SetValue(float fTime, bool bValue);
	virtual void SetValue(int iIndex, bool bValue);
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif