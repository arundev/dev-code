//*************************************************************************************************
// ����:	��̬�ڴ�ؿ�����
//---------------------------------------------------------
// ����:	
// ����:		2012-12-18
// ����޸�:
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