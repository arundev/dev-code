//*************************************************************************************************
// 内容:	编辑状态机
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITFSM_H__
#define _SD_GAMEEDITEX_EDITFSM_H__
#include <sdSingleton.h>
#include <sdFSM.h>

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 编辑状态机基类
//-------------------------------------------------------------------------------------------------
class sdEditFSM : public Base::sdTSingleton<sdEditFSM>, public Base::sdFSM
{
public:
	sdEditFSM(){};
	virtual ~sdEditFSM(){}; 

	// 虚函数继承(继承自Base::sdFSM)
	virtual int UpdateState(){ return __super::UpdateState();}

	// 初始化与销毁
	virtual bool Initialize(){ return true;}
	virtual void Destroy() {}

	// 输入消息(虚函数声明)
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseEnter(){}
	virtual void OnMouseLeave(){}
	virtual void OnMouseHover(){}
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonDBClick(WPARAM wParam, LPARAM lParam);
	virtual void OnRightButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnRightButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnRightButtonDBClick(WPARAM wParam, LPARAM lParam);
	virtual void OnMiddleButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnMiddleButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnMiddleButtonDBClick(WPARAM wParam, LPARAM lParam);
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif