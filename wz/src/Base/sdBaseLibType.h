//*************************************************************************************************
// ����:	����������(�ο���Gamebryo)
//---------------------------------------------------------
// ����:	
// ����:		2014-01-14
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_LIBRARY_TYPE_H__
#define _SD_BASE_LIBRARY_TYPE_H__

#ifdef SD_BASE_EXPORT
	#define SD_BASE_ENTRY __declspec(dllexport)		///< DLL library prohect use this
#else
	#ifdef SD_BASE_IMPORT
		#define ENGINE_ENTRY __declspec(dllimport)	///< Client of dll use this
	#else
		#define ENGINE_ENTRY						///< Static library project use this
	#endif
#endif

#endif