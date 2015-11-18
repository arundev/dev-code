//*************************************************************************************************
// 内容:	动态内存池库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-18
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_DYNAMIC_POOL_LIBTYPE_H__
#define _SD_ENGINE_DYNAMIC_POOL_LIBTYPE_H__

#ifdef SD_ENGINE_DYNAMCPOOL_EXPORT
	#define DYNAMCPOOL_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_DYNAMCPOOL_IMPORT
		#define DYNAMCPOOL_ENTRY __declspec(dllimport)
	#else
		#define DYNAMCPOOL_ENTRY
	#endif
#endif

#endif