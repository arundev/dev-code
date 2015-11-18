//*************************************************************************************************
// 内容:	资源系统一些常规公共定义
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_DEFINE_H__
#define _SD_ENGINE_RESOURCE_DEFINE_H__

namespace ResourceSystem
{
	// 资源加载任务类型
	enum eResTaskType
	{
		E_RES_TASK_MODEL	= 0x0001,	// nif文件
		E_RES_TASK_KEYFRAME	= 0x0002,	// kf文件
		E_RES_TASK_ACTOR	= 0x0004,	// kfm文件
		E_RES_TASK_TEXTURE	= 0x0008,	// texture文件
	};

	// 资源加载任务状态
	enum eResTaskStatus
	{
		E_RES_TASK_WAIT,
		E_RES_TASK_LOADING,
	};

	// 资源加载优先级
	enum eResTaskPrior
	{
		E_RES_TASK_NORMAL	= 0,
		E_RES_TASK_HIGH		= 1,
		E_RES_TASK_HIGHEST	= 2,
	};

	// 资源生命周期
	enum eResLifeTime
	{
		E_RES_LT_APP,
		E_RES_LT_SCENE,
	};
}
#endif