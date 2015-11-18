//*************************************************************************************************
// 内容:	Movie的WZ实现
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_MOVIE_H__
#define _MOVIE_SYSTEM_MOVIE_H__

#include <IMovie.h>

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class sdMovie : public IMovie
{
public:
	sdMovie();
	virtual ~sdMovie();

	// 虚函数继承
	virtual bool Load(const std::string& strMovieFile);
	virtual bool Save(const std::string& strMovieFile);

	virtual void UpdateFrame(float fAccumTime, float fFrameTime);

	virtual void Start(){};
	virtual void Stop(){};
	virtual void Pause(){};
	virtual void Resume(){};

	virtual bool IsPlaying(){ return false;};

	IAnimNode*	 GetAnimNode(){ return NULL;};

	virtual float GetTotalTime(){ return 0.f;};
	virtual float GetCurrentTime(){ return 0.f;};

	virtual void GotoTime(float fTime){};
	virtual void GotoFrame(uint uiFrameId){};

	virtual inline void SetPlayMode(EMoviePlayMode ePlayMode);
	virtual inline EMoviePlayMode GetPlayMode();

	virtual inline void SetCamera(NiCamera* spCamera){};
	virtual inline NiCamera* GetCamera(){ return NULL;};

	virtual inline void SetParam(const std::string& key, const std::string& value){};
	virtual inline void GetParam(const std::string& key, std::string& value){};

protected:
	// 播放方式
	EMoviePlayMode m_ePlayMode;
};
NiSmartPointer(sdMovie);

#include "sdMovie.inl"

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif