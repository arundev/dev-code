//*************************************************************************************************
// 内容: 用于定义一些体系结构相关宏定义,本文件抽取自Ogre/OgrePlatform.h)
//---------------------------------------------------------
// 作者:	
// 创建:		2012-08-04
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_PLATFORM_H__
#define _SD_BASE_PLATFORM_H__

BASE_NAMESPACE_BEGIN_BASE

// 定义关于导出静态库和动态库的宏定义
#if defined(SD_STATIC_LIB)
	#define _SDExport
	#define _SDPrivate
#else
	#define _SDExport	__declspec(dllimport)
	#define _SDPrivate
#endif

// 区分Debug/Release模式
#ifdef _DEBUG
	#define SD_DEBUG_MODE 1
#else
	#define SD_DEBUG_MODE 0
#endif

// 
#define SD_STATIC			static

#define SD_INLINE			inline
#define SD_FORCE_INLINE		__forceinline

#define SD_STATIC_INLINE	static inline

BASE_NAMESPACE_END_BASE
#endif