//*************************************************************************************************
// ����: ���ڶ���һЩ��ϵ�ṹ��غ궨��,���ļ���ȡ��Ogre/OgrePlatform.h)
//---------------------------------------------------------
// ����:	
// ����:		2012-08-04
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_PLATFORM_H__
#define _SD_BASE_PLATFORM_H__

BASE_NAMESPACE_BEGIN_BASE

// ������ڵ�����̬��Ͷ�̬��ĺ궨��
#if defined(SD_STATIC_LIB)
	#define _SDExport
	#define _SDPrivate
#else
	#define _SDExport	__declspec(dllimport)
	#define _SDPrivate
#endif

// ����Debug/Releaseģʽ
#ifdef _DEBUG
	#define SD_DEBUG_MODE 1
#else
	#define SD_DEBUG_MODE 0
#endif

// 
#define SD_STATIC			static

#define SD_INLINE			inline
#define SD_FORCE_INLINE		__forceinline

#define SD_STATIC_INLINE	static inline

BASE_NAMESPACE_END_BASE
#endif