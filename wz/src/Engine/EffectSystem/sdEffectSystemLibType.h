//*************************************************************************************************
// ����:	EffectSystem������
//---------------------------------------------------------
// ����:	
// ����:		2013-01-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_EFFECT_SYSTEM_LIBTYPE_H__
#define _SD_ENGINE_EFFECT_SYSTEM_LIBTYPE_H__

#ifdef SD_ENGINE_EFFECTSYSTEM_EXPORT
	#define EFFECTSYSTEM_ENTRY __declspec(dllexport)
#else
	#ifdef SD_ENGINE_EFFECTSYSTEM_IMPORT
		#define EFFECTSYSTEM_ENTRY __declspec(dllimport)
	#else
		#define EFFECTSYSTEM_ENTRY
	#endif
#endif

#endif