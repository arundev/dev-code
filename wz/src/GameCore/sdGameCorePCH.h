//*************************************************************************************************
// 内容:	GameCore库的预编译头文件
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-19
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_PCH_H__
#define _SD_GAMECORE_PCH_H__

//
#define SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE	namespace GameCore{
#define SD_GAMECORE_NAMESPACE_END_GAMECORE		}

#define SD_GAMECORE_NAMESPACE_BEGIN_UI			namespace UI{
#define SD_GAMECORE_NAMESPACE_END_UI			}


#define USING_PHYSX
#define USING_GAMEBRYO
#include <sdEnginePCH.h>


#endif