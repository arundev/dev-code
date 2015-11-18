//*************************************************************************************************
// 内容:	地形笔刷图形(用于显示)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITBRUSHSHAPE_H__
#define _SD_GAMEEDITEX_EDITBRUSHSHAPE_H__

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-----------------------------------------------------------------------------
// 绘制笔刷基类
//-----------------------------------------------------------------------------
class sdBrushShape : public NiNode
{
public:
	virtual ~sdBrushShape(){};

	// 根据当前位置,重新拾取地形高度,更新显示
	virtual void UpdateShape(){};
};
NiSmartPointer(sdBrushShape);
//-----------------------------------------------------------------------------
// 双圆环笔刷
//-----------------------------------------------------------------------------
class sdDualCircleMesh : public sdBrushShape
{
public:
	sdDualCircleMesh();
	~sdDualCircleMesh();

	// 虚函数继承
	void UpdateShape();

	// 属性
	void SetInnerCircleVisible(bool bVisible = true) { m_spInnerCircle->SetAppCulled(!bVisible);}
	void SetOuterCircleVisible(bool bVisible = true) { m_spOuterCircle->SetAppCulled(!bVisible);}
	void SetInnerRadius(float fInnerRadius) { m_fInnerRadius = fInnerRadius;}
	void SetOuterRadius(float fOuterRadius) { m_fOuterRadius = fOuterRadius;}
	float GetInnerRadius() const { return m_fInnerRadius;}
	float GetOuterRadius() const { return m_fOuterRadius;}

protected:
	void CreateInnerCircle();
	void CreateOutterCirlce();
	void UpdateInnerCircle();
	void UpdateOutterCirlce();	

protected:
	float	m_fInnerRadius;
	float	m_fOuterRadius;
	NiMeshPtr	m_spInnerCircle;
	NiMeshPtr	m_spOuterCircle;

	// 圆刷子圆周采样点数
	const static uint ms_uiCirclePointCount = 50;
};
NiSmartPointer(sdDualCircleMesh);
//-----------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif