//*************************************************************************************************
// 内容:	地形高度编辑模式
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_TERRAINDEFORMMODE_H__
#define _SD_GAMEEDITEX_TERRAINDEFORMMODE_H__
#include "sdEditMode.h"
#include "sdEditBrush.h"
#include "sdEditBrushShape.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
class sdTerrainDeformMode : public sdEditMode
{
public:
	sdTerrainDeformMode();
	~sdTerrainDeformMode();

	// 虚函数继承
	virtual bool Initialize();
	virtual void Destroy();

	virtual void Enter();
	virtual	void Leave();
	virtual int	 Update();

	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam);

	// 属性访问与设置
	sdEditBrush::EBrushType	GetBrushType();
	void	SetActiveDeformBrush(sdEditBrush::EBrushType eType);

	void	SetShowBrush(bool bShow);

	float	GetOutterRadius();
	void	SetOutterRadius(float fRadius);

	float	GetInnerRadius();
	void	SetInnerRadius(float fRadius);

	float	GetHardness();
	void	SetHardness(float fHardness);

	float	GetHeight();
	void	SetHeight(float fHeight);

protected:
	// 鼠标状态
	bool	m_bLeftButtonDown;
	POINT	m_kMousePos;

	// 笔刷
	sdEditBrushPtr m_pkkEditBrushArray[sdEditBrush::NUM_BRUSHES];
	sdEditBrushPtr m_pkActiveBrush;
};
//-------------------------------------------------------------------------------------------------
#include "sdTerrainDeformMode.inl"
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif