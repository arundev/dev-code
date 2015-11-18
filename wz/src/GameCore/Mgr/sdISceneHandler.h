//*************************************************************************************************
// 内容:	场景事件回调接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-06	
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_ISCENE_HANDLER_H__
#define _SD_GAMECORE_ISCENE_HANDLER_H__

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 一些常用场景事件的回调接口(用以一些保证调用先后顺序的情况)
class sdISceneHandler : public NiMemObject
{
public:
	virtual ~sdISceneHandler() = 0;

	// 更新
	virtual void Update() = 0;

	// 地图加载与卸载
	// @{
	virtual void OnMapBeforeLoad() = 0;
	virtual void OnMapAfterLoad() = 0;
	virtual void OnMapBeforeUnload() = 0;
	virtual void OnMapBeforeUnload() = 0;
	// @}
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif
