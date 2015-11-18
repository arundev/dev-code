//*************************************************************************************************
// 内容:	EffectSystem库类型
//---------------------------------------------------------
// 作者:	
// 创建:		2013-01-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_MOVIESYSTEM_EXPORT
	#define MOVIESYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_MOVIESYSTEM_IMPORT
		#define MOVIESYSTEM_ENTRY __declspec(dllimport)
	#else
		#define MOVIESYSTEM_ENTRY
	#endif
#endif

#endif