//*************************************************************************************************
// 内容:	过场动画系统基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_MOVIE_SYSTEM_H__
#define _SD_ENGINE_MOVIE_SYSTEM_MOVIE_SYSTEM_H__

#include <sdSingleton.h>

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class IMovie;

class IMovieSystem : public Base::sdTSingleton<IMovieSystem>
{
public:
	virtual ~IMovieSystem() {};

	// 加载创建与销毁过场动画
	virtual IMovie* CreateMovie() = 0;
	virtual IMovie* LoadMovie(const std::string& strMovieFile) = 0;
	virtual void DestroyMovie(IMovie* pkMovie) = 0;

	// 过场动画更新
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// 获取与设置过场动画全局参数
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// 获取管理器接口
	static IMovieSystem* GetMovieSystem(){ return InstancePtr();}
};

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif