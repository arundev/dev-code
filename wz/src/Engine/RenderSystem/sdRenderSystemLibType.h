//*************************************************************************************************
// ����:	��Ⱦϵͳ������
//---------------------------------------------------------
// ����:	
// ����:		2012-12-18
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDERE_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_RENDERE_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_RENDERSYSTEM_EXPORT
	#define RENDERSYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_RENDERSYSTEM_IMPORT
		#define RENDERSYSTEM_ENTRY __declspec(dllimport)
	#else
		#define RENDERSYSTEM_ENTRY
	#endif
#endif

#endif