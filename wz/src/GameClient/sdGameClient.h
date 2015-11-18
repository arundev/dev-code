//*************************************************************************************************
// 内容:	客户端入口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-07
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _GAME_CLIENT_H__
#define _GAME_CLIENT_H__

// Base
#include <sdSingleton.h>
#include <sdTimeMgr.h>
#include <sdLogger.h>

// Engine
#include <sdResourceSystem.h>
#include <sdRenderSystem_DX9.h>
#include <sdMap.h>
#include <sdWorld.h>

// EngineCore
#include <sdGameConfig.h>
#include <sdInputSystem.h>
#include <sdCameraFSM.h>

// 客户端入口
class sdGameClient : Base::sdTSingleton<sdGameClient>
{
public:
	sdGameClient();
	~sdGameClient();

	// 初始化与销毁
	bool Initialize(HINSTANCE hInstance);
	bool Run();
	void Destroy();

	inline bool IsInitialized(){ return m_bInitialize;};
	inline bool	IsRunning() { return m_bRunning;};

	inline HWND GetWindowHandle(){ return m_hWnd;};

protected:
	bool CreateGameWindow();
	bool CreateGameRenderer();

	void UpdateFrame();
	void RenderFrame();

	// 失去/获得焦点
	void SetActive(bool bActive);

	// 窗口消息回调函数
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 显示设备回调函数
	static bool OnLostDevice(void* pData);
	static bool	OnResetDevice(bool bBefore, void* pData);

protected:
	bool	m_bInitialize;
	bool	m_bRunning;
	bool	m_bActive;
	
	HWND		m_hWnd;			///< 窗口句柄	
	HINSTANCE	m_hInstance;	///< 模块句柄

	// 窗口尺寸(加上了边框和标题栏,比D3D显示区域要大)
	unsigned int	m_uiWindowWidth;	
	unsigned int	m_uiWindowHeight;

	// 时间
	Base::sdTimeMgr	m_kTimeMgr;

	// 日志系统
	Base::Diagnostics::sdLogger	m_kLogger;	

	// 配置(从配置文件读取)
	GameCore::sdGameConfig	m_kGameConfig;

	// 输入
	GameCore::sdInputSystem	m_kInputSystem;

	// 相机控制
	GameCore::sdCameraFSM	m_kCameraFSM;

	//(可能以后是被封装到下一层EngineCore/EngineLogic里面去)
	//
	// Gamebryo渲染器
	NiDX9RendererPtr	m_spRenderer;

	// WZ渲染系统
	RenderSystem::sdRenderSystem_DX9	m_kRenderSystem;

	// WZ资源系统
	ResourceSystem::sdResourceSystem	m_kResourceSystem;

	// WZ世界
	sdWorldManager m_kWorldManager;

	//*****************************************************
	sdMapPtr	m_pkMap;
	//*****************************************************
};

#endif
