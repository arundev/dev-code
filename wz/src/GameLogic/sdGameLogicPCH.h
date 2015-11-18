#pragma once
#ifndef _SD_GAMELOGIC_STDAFX_H__
#define _SD_GAMELOGIC_STDAFX_H__

//
#include "sdGameLogic.h"

//
#ifdef GAMELOGIC_EXPORT
	#define GAMELOGIC_ENTRY __declspec(dllexport)
#else
	#ifdef GAMELOGIC_IMPORT
		#define GAMELOGIC_ENTRY __declspec(dllimport)
	#else
		#define GAMELOGIC_ENTRY
	#endif
#endif

#endif