//*************************************************************************************************
// 内容:	物理系统库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-18
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_PHYSICSSYSTEM_EXPORT
	#define PHYSICSSYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_PHYSICSSYSTEM_IMPORT
		#define PHYSICSSYSTEM_ENTRY __declspec(dllimport)
	#else
		#define PHYSICSSYSTEM_ENTRY
	#endif
#endif

#endif