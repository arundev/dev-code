//*************************************************************************************************
// 内容:	整个Engine.Physics系统的公共头文件
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-18
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PCH_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PCH_H__

//*********************************************************
// GB头文件
//*********************************************************
#include <NiSystem.h>
#include <NiMain.h>
#include <NiMesh.h>
#include <NiAnimation.h>
#include <NiDX9Renderer.h>
#include <NiDX9Direct3DTextureData.h>
#include <NiViewMath.h>
#include <NiPNGReader.h>
#include <NiD3DShaderFactory.h>
#include <NiD3DShaderProgramFactory.h>
#include <NiD3DXEffectShaderLibrary.h>
#include <NiD3DXEffectFactory.h>

//*********************************************************
// windows 头文件
//*********************************************************
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//*********************************************************
// C头文件
//*********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//*********************************************************
// C++头文件
//*********************************************************
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <set>
#include <map>
//#include <hashmap>
#include <algorithm>

//*********************************************************
// 依赖库文件
//*********************************************************
#include <sdBasePCH.h>
#include <sdEnginePCH.h>

#endif