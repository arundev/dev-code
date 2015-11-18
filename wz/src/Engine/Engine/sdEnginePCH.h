//*************************************************************************************************
// ����:	Engine���Ԥ����ͷ�ļ�
//---------------------------------------------------------
// ����:	
// ����:		2012-12-18
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PCH_H__
#define _SD_ENGINE_PCH_H__

//*********************************************************
// PhysXͷ�ļ�
//*********************************************************
#ifdef USING_PHYSX
	#undef min	///< ��ֹPhysX��windows.h���������ͻ
	#undef max
	#pragma warning(push) 
	#pragma warning(disable:4819)
	#include <NxUserOutputStream.h>
	#include <NxPhysicsSDK.h>
	#include <NxCooking.h>
	#include <NxController.h>
	#include <NxControllerManager.h>
	#include <PhysXLoader.h>
	#pragma warning(pop) 
#endif

//*********************************************************
// GBͷ�ļ�
//*********************************************************
#ifdef USING_GAMEBRYO
	#include <NiSystem.h>
	#include <NiMain.h>
	#include <NiTexture.h>
	#include <NiSourceTexture.h>
	#include <NiMesh.h>
	#include <NiAnimation.h>
	#include <NiDX9Renderer.h>
	#include <NiDX9TextureData.h>
	#include <NiDX9Direct3DTexture.h>
	#include <NiDX9Direct3DTextureData.h>
	#include <NiDX9SourceTextureData.h>
	#include <NiViewMath.h>
	#include <NiPNGReader.h>
	#include <NiD3DShaderFactory.h>
	#include <NiD3DShaderProgramFactory.h>
	#include <NiD3DXEffectShaderLibrary.h>
	#include <NiD3DXEffectFactory.h>
#endif

//*********************************************************
// windows ͷ�ļ�
//*********************************************************
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//*********************************************************
// Cͷ�ļ�
//*********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//*********************************************************
// C++ͷ�ļ�
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
// �������ļ�
//*********************************************************
#include <sdBasePCH.h>

#define ENGINE_NAMESPACE_BEGIN_ENGINE			namespace Engine{	
#define ENGINE_NAMESPACE_END_ENGINE				}

#define ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM	namespace ResourceSystem{	
#define ENGINE_NAMESPACE_END_RESOURCESYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM		namespace RenderSystem{	
#define ENGINE_NAMESPACE_END_RENDERSYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM		namespace EffectSystem{	
#define ENGINE_NAMESPACE_END_EFFECTSYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM		namespace MovieSystem{	
#define ENGINE_NAMESPACE_END_MOVIESYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_TERRAINSYSTEM	namespace TerrainSystem{	
#define ENGINE_NAMESPACE_END_TERRAINSYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM		namespace PhysicSystem{	
#define ENGINE_NAMESPACE_END_PHYSICSYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_SOUNDSYSTEM		namespace SoundSystem{	
#define ENGINE_NAMESPACE_END_SOUNDSYSTEM		}

#define ENGINE_NAMESPACE_BEGIN_FILESYSTEM		namespace FileSystem{	
#define ENGINE_NAMESPACE_END_FILESYSTEM			}

#define ENGINE_NAMESPACE_BEGIN_UTIL				namespace Util{	
#define ENGINE_NAMESPACE_END_UTIL				}

//*********************************************************
// ��������
//********************************************************* 
// ���·������
#define MAX_PATH_LEN	1024

#endif