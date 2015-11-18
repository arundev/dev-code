//*************************************************************************************************
// 内容:	地形纹理编辑模式
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_TERRAINSURFACEMODE_H__
#define _SD_GAMEEDITEX_TERRAINSURFACEMODE_H__
#include "sdEditMode.h"
#include "sdEditBrush.h"
#include "sdEditBrushShape.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX

class sdTerrainSurfaceMode : public sdEditMode
{
public:
	sdTerrainSurfaceMode();
	~sdTerrainSurfaceMode();

	// 虚函数继承
	virtual bool Initialize();
	virtual void Destroy();

	virtual void Enter();
	virtual	void Leave();
	virtual int Update();

	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam);

	// 属性设置与访问
	void	SetShowBrush(bool bShow);

	float	GetOutterRadius();
	void	SetOutterRadius(float fRadius);

	float	GetInnerRadius();
	void	SetInnerRadius(float fRadius);

	float	GetHardness();
	void	SetHardness(float fHardness);

	uint GetActiveLayerMap() const { return ((sdTerrainSurfaceLayerBrush*)(sdEditBrush*)m_pkEditBrush)->GetActiveLayerMap();}
	void SetActiveLayerMap(uint uiIndex) { ((sdTerrainSurfaceLayerBrush*)(sdEditBrush*)m_pkEditBrush)->SetActiveLayerMap(uiIndex);}

protected:
	// 鼠标状态
	bool	m_bLeftButtonDown;
	POINT	m_kMousePos;

	// 笔刷
	sdEditBrushPtr m_pkEditBrush;
};

#include "sdTerrainSurfaceMode.inl"
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif