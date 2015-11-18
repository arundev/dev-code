//*************************************************************************************************
// 内容:	编辑模式
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITMODE_H__
#define _SD_GAMEEDITEX_EDITMODE_H__
#include <sdFSM.h>

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 编辑模式基类
//-------------------------------------------------------------------------------------------------
class sdEditMode : public Base::sdState
{
public:
	// 编辑模式
	enum eEditMode
	{
		// 物件
		E_EDITMODE_VIEW = 0,		///< 观察模式
		E_EDITMODE_SELECT,			///< 选择模式
		E_EDITMODE_TRANSLATE,		///< 选择-平移模式
		E_EDITMODE_ROTATE,			///< 选择-旋转模式
		E_EDITMODE_SCALE,			///< 选择-缩放模式
		E_EDITMODE_CREATE,			///< 创建模式
		E_EDITMODE_REMOVE,			///< 删除模式

		// 地形
		E_EDITMODE_TERRAIN_DEFORM,	///< 地表编辑
		E_EDITMODE_TERRAIN_SURFACE,	///< 地表纹理编辑

		NUM_EDITMODES = E_EDITMODE_TERRAIN_SURFACE,
	};

public:
	sdEditMode(){};
	virtual ~sdEditMode(){};

	// 初始化与销毁
	virtual bool Initialize(){ return true;}
	virtual void Destroy(){}

	// 虚函数继承
	virtual void Enter() {}
	virtual	void Leave() {}
	virtual int Update();

	// 输入消息(虚函数声明)
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