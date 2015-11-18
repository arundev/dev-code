//*************************************************************************************************
// 内容:	过场动画基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_MOVIE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_MOVIE_H__

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class IAnimNode;

class IMovie : public NiRefObject
{
public:
	// 过场动画播放方式
	enum EMoviePlayMode
	{
		E_MOVIE_PLAYONCE,		///< 一次播放,播放完后,回到第一帧
		E_MOVIE_PLAYONCEEND,	///< 一次播放,播放完后,停在最后一帧
		E_MOVIE_PLAYLOOP,		///< 循环播放,需要手动停止
	};

public:
	virtual ~IMovie() = 0;

	// 过场动画文件加载与销毁
	virtual bool Load(const std::string& strMovieFile) = 0;
	virtual bool Save(const std::string& strMovieFile) = 0;

	// 过场动画更新
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// 过场动画播放
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	// 当前是否处于播放状态
	virtual bool IsPlaying() = 0;

	// 获取动画根节点
	virtual IAnimNode*	GetAnimNode() = 0;

	// 获取过场动画总时长
	virtual float GetTotalTime() = 0;

	// 获取过场动画当前播放位置
	virtual float GetCurrentTime() = 0;

	// 跳转到过场动画指定时间点
	virtual void GotoTime(float fTime) = 0;

	// 跳转到指定帧
	virtual void GotoFrame(uint uiFrameId) = 0;

	// 获取与设置过场动画播放方式
	virtual void SetPlayMode(EMoviePlayMode ePlayMode) = 0;
	virtual EMoviePlayMode GetPlayMode() = 0;

	// 获取与设置过场动画相机
	virtual void SetCamera(NiCamera* spCamera) = 0;
	virtual NiCamera* GetCamera() = 0;

	// 获取与设置过场动画参数
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;
};
NiSmartPointer(IMovie);

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif