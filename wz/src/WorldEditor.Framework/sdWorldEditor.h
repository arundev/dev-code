#pragma once
#include <IRenderSystem.h>
#include <IResourceSystem.h>
#include <IFileSystem.h>
#include <sdMap.h>
#include <sdWorld.h>
#include <sdEventMgr.h>
#include <sdCameraFSM.h>
#include <sdTimeMgr.h>
#include <sdWorldEditFSM.h>

class sdWorldEditor : public Base::sdTSingleton<sdWorldEditor>
{
public:
	sdWorldEditor();
	~sdWorldEditor();

	// ��ʼ��������
	// @{
	bool	Initialize(HWND hWnd);
	void	Destroy();
	void	Update();

	bool	IsInitialized() { return m_bInitialized;}

	bool	SaveScene();
	bool	OpenScene();
	// @}

	// �������
	// @{

	// @}

	// ���α༭ģ��
	// @{
	//void SetTerrainBrushInnerRadius(float fRadius);
	//float GetTerrainBrushInnerRadius();

	//void SetTerrainBrushOutterRadius(float fRadius);
	//float GetTerrainBrushInnerRadius();

	// @}

	// ���Է���
	GameEditEx::sdWorldEditFSM* GetWorldEditFSM(){ return &m_kWorldEditFSM;}

	// ������Ϣ�ַ�������
	void	WndProc(int iMsg, HWND hWnd, WPARAM wParam, LPARAM lParam);

protected:
	// ��ʼ�����
	bool	m_bInitialized;

	// ������
	HWND	m_hWnd;	

	// ʱ��
	Base::sdTimeMgr	m_kTimeMgr;

	// �������
	GameCore::sdCameraFSM	m_kCameraFSM;

	// Gamebryo��Ⱦ��
	NiDX9RendererPtr	m_spRenderer;

	// ��Ⱦϵͳ
	Engine::RenderSystem::IRenderSystem*	m_pkRenderSystem;

	// ��Դϵͳ
	Engine::ResourceSystem::IResourceSystem* m_pkResourceSystem;

	// �ļ�ϵͳ
	Engine::FileSystem::IFileSystem* m_pkFileSystem;

	// ��Ϣ����
	GameCore::sdEventMgr m_kEventMgr;

	// �༭������
	GameEditEx::sdWorldEditFSM	m_kWorldEditFSM;

	// ����
	sdWorldManager m_kWorldManager;

	// ����
	sdMap*	m_pkMap;
};