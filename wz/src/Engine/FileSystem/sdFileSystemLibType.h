//*************************************************************************************************
// 内容:	文件系统库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2014-01-14
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_FILE_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_DYNAMCPOOL_EXPORT
	#define FILESYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_DYNAMCPOOL_IMPORT
		#define FILESYSTEM_ENTRY __declspec(dllimport)
	#else
		#define FILESYSTEM_ENTRY
	#endif
#endif

#endif