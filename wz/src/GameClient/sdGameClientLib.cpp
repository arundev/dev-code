#include "Stdafx.h"

// D3D¿â
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")

// Gamebryo¿â
#if (defined NIDEBUG)
#pragma comment(lib, "efd30VC90D.lib")
#pragma comment(lib, "NiAnimation30VC90D.lib")
#pragma comment(lib, "NiDX9Renderer30VC90D.lib")
#pragma comment(lib, "NiD3DXEffectShaderLib30VC90D.lib")
#pragma comment(lib, "NiFloodgate30VC90D.lib")
#pragma comment(lib, "NiMain30VC90D.lib")
#pragma comment(lib, "NiMesh30VC90D.lib")
#pragma comment(lib, "NiSystem30VC90D.lib")
#pragma comment(lib, "NiViewMath30VC90D.lib")
#pragma comment(lib, "NiPNGReader30VC90D.lib")
#endif

// ThirdParty¿â
#pragma comment(lib, "lua51d.lib")
#pragma comment(lib, "tolua51d.lib")

// Engine¿â
#pragma comment(lib, "Base.lib")
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "RenderSystem.lib")
#pragma comment(lib, "ResourceSystem.lib")
#pragma comment(lib, "GameCore.lib")
#pragma comment(lib, "GameEditEx.lib")