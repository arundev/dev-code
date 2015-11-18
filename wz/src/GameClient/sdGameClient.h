//*************************************************************************************************
// ����:	�ͻ������
//---------------------------------------------------------
// ����:		
// ����:		2012-07-07
// ����޸�:
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

// �ͻ������
class sdGameClient : Base::sdTSingleton<sdGameClient>
{
public:
	sdGameClient();
	~sdGameClient();

	// ��ʼ��������
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

	// ʧȥ/��ý���
	void SetActive(bool bActive);

	// ������Ϣ�ص�����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ��ʾ�豸�ص�����
	static bool OnLostDevice(void* pData);
	static bool	OnResetDevice(bool bBefore, void* pData);

protected:
	bool	m_bInitialize;
	bool	m_bRunning;
	bool	m_bActive;
	
	HWND		m_hWnd;			///< ���ھ��	
	HINSTANCE	m_hInstance;	///< ģ����

	// ���ڳߴ�(�����˱߿�ͱ�����,��D3D��ʾ����Ҫ��)
	unsigned int	m_uiWindowWidth;	
	unsigned int	m_uiWindowHeight;

	// ʱ��
	Base::sdTimeMgr	m_kTimeMgr;

	// ��־ϵͳ
	Base::Diagnostics::sdLogger	m_kLogger;	

	// ����(�������ļ���ȡ)
	GameCore::sdGameConfig	m_kGameConfig;

	// ����
	GameCore::sdInputSystem	m_kInputSystem;

	// �������
	GameCore::sdCameraFSM	m_kCameraFSM;

	//(�����Ժ��Ǳ���װ����һ��EngineCore/EngineLogic����ȥ)
	//
	// Gamebryo��Ⱦ��
	NiDX9RendererPtr	m_spRenderer;

	// WZ��Ⱦϵͳ
	RenderSystem::sdRenderSystem_DX9	m_kRenderSystem;

	// WZ��Դϵͳ
	ResourceSystem::sdResourceSystem	m_kResourceSystem;

	// WZ����
	sdWorldManager m_kWorldManager;

	//*****************************************************
	sdMapPtr	m_pkMap;
	//*****************************************************
};

#endif
