//*************************************************************************************************
// 内容:	性能分析工具
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-16
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_PROFILE_H__
#define _SD_BASE_DIAGNOSTICS_PROFILE_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS





// 导出为宏定义,便于处理
#if (SD_PROFILING == 1)
	//
	//
	//
#else
	//#define OgreProfile( a )
	//#define OgreProfileBegin( a )
	//#define OgreProfileEnd( a )
#endif

BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif