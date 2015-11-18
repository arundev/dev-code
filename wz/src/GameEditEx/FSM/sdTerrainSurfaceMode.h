//*************************************************************************************************
// ����:	��������༭ģʽ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
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

	// �麯���̳�
	virtual bool Initialize();
	virtual void Destroy();

	virtual void Enter();
	virtual	void Leave();
	virtual int Update();

	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam);

	// �������������
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
	// ���״̬
	bool	m_bLeftButtonDown;
	POINT	m_kMousePos;

	// ��ˢ
	sdEditBrushPtr m_pkEditBrush;
};

#include "sdTerrainSurfaceMode.inl"
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif