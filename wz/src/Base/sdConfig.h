//*************************************************************************************************
// 内容: 用于定义一些宏定义来调整整个Base库的编译配置(本文件抽取自Ogre/OgreConfig.h)
//---------------------------------------------------------
// 作者:	
// 创建:		2012-08-04
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_CONFIG_H__
#define _SD_BASE_CONFIG_H__

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
// 控制是否进行profile
//	0 - 剖分宏定义为空
//	1 - 剖分宏定义为真正的剖分代码
#define SD_PROFILING 0

// 控制捕捉assert的方式
//	0 - Debug模式为Assert，Release不显示
//	1 - Debug模式为Assert，Release模式为Exception
//	2 - Debug模式为Exception， Release模式亦为Exception
#define SD_ASSERT_MODE 0

// 控制是否记录log
#define SD_LOG 1

// 控制内存分配器使用
//#define SD_MEMORY_ALLOCATOR

// 控制内存使用跟踪
//#define SD_MEMORY_TRACKER

// 控制多线程是否开启
#define SD_MULTITHREADED
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE
#endif