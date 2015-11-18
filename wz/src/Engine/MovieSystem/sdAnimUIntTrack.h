//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-08
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_UINT_TRACK_H__
#define _MOVIE_SYSTEM_ANIM_UINT_TRACK_H__

#include "sdAnimTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
class sdAnimUIntTrack : public TAnimTrack<sdUIntKey>
{
public:
	sdAnimUIntTrack(){};
	virtual ~sdAnimUIntTrack(){};

	// 虚函数继承
	virtual inline EAnimTrackType GetType() { return E_ANIM_TRACK_UINT; }

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);

	virtual void GetValue(float fTime, uint& bValue);
	virtual void GetValue(int iIndex, uint& bValue);
	virtual void SetValue(float fTime, uint bValue);
	virtual void SetValue(int iIndex, uint bValue);
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif