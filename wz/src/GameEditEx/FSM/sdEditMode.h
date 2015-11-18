//*************************************************************************************************
// ����:	�༭ģʽ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITMODE_H__
#define _SD_GAMEEDITEX_EDITMODE_H__
#include <sdFSM.h>

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// �༭ģʽ����
//-------------------------------------------------------------------------------------------------
class sdEditMode : public Base::sdState
{
public:
	// �༭ģʽ
	enum eEditMode
	{
		// ���
		E_EDITMODE_VIEW = 0,		///< �۲�ģʽ
		E_EDITMODE_SELECT,			///< ѡ��ģʽ
		E_EDITMODE_TRANSLATE,		///< ѡ��-ƽ��ģʽ
		E_EDITMODE_ROTATE,			///< ѡ��-��תģʽ
		E_EDITMODE_SCALE,			///< ѡ��-����ģʽ
		E_EDITMODE_CREATE,			///< ����ģʽ
		E_EDITMODE_REMOVE,			///< ɾ��ģʽ

		// ����
		E_EDITMODE_TERRAIN_DEFORM,	///< �ر�༭
		E_EDITMODE_TERRAIN_SURFACE,	///< �ر�����༭

		NUM_EDITMODES = E_EDITMODE_TERRAIN_SURFACE,
	};

public:
	sdEditMode(){};
	virtual ~sdEditMode(){};

	// ��ʼ��������
	virtual bool Initialize(){ return true;}
	virtual void Destroy(){}

	// �麯���̳�
	virtual void Enter() {}
	virtual	void Leave() {}
	virtual int Update();

	// ������Ϣ(�麯������)
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam){};
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam){};
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnLeftButtonDBClick(WPARAM wParam, LPARAM lParam){};
	virtual void OnRightButtonDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnRightButtonUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnRightButtonDBClick(WPARAM wParam, LPARAM lParam){};
	virtual void OnMiddleButtonDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnMiddleButtonUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnMiddleButtonDBClick(WPARAM wParam, LPARAM lParam){};
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif