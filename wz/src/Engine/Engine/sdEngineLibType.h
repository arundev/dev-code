//*************************************************************************************************
// 内容:	引擎库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-18
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_LIBTYPE_H__
#define _SD_ENGINE_LIBTYPE_H__

#ifdef SD_ENGINE_EXPORT
	#define ENGINE_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_IMPORT
		#define ENGINE_ENTRY __declspec(dllimport)
	#else
		#define ENGINE_ENTRY
	#endif
#endif

#endif
