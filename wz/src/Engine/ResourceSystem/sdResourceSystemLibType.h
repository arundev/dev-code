//*************************************************************************************************
// 内容:	资源加载与管理库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-18
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_RESOURCESYSTEM_EXPORT
	#define RESOURCESYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_RESOURCESYSTEM_IMPORT
		#define RESOURCESYSTEM_ENTRY __declspec(dllimport)
	#else
		#define RESOURCESYSTEM_ENTRY
	#endif
#endif

#endif