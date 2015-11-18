//*************************************************************************************************
// 内容:	地形笔刷, wz原设计为:
//				Brush(画刷)			包装器
//				Shape(形状)			用于提供一个矩形区域内掩模和权重
//				Canvas(画布)		用于根据有效掩模位置的权重操作对象
//				BrushShape(形状)	刷子显示对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITBRUSH_H__
#define _SD_GAMEEDITEX_EDITBRUSH_H__
#include "sdEditShape.h"
#include "sdEditCanvas.h"
#include "sdEditBrushShape.h"

//
BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
class sdRay;
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-----------------------------------------------------------------------------
// 编辑笔刷基类
//-----------------------------------------------------------------------------
class sdEditBrush : public NiRefObject
{
public:
	// 编辑笔刷
	enum EBrushType
	{
		E_BRUSH_INVALID = 0,
		E_BRUSH_PULL,		// 地表拉伸
		E_BRUSH_SMOOTH,		// 地表平滑
		E_BRUSH_NOISE,		// 地表噪声
		E_BRUSH_AVERAGE,	// 地表平均
		E_BRUSH_FLAT,		// 地表踏平
		E_BRUSH_LAYER,		// 地表纹理混合
		NUM_BRUSHES,	
	};	

public:
	sdEditBrush(){};
	virtual ~sdEditBrush() {};

	void SetEditCanvas(sdEditCanvas* pkEditCanvas) { m_pkEditCanvas = pkEditCanvas;}
	sdEditCanvas* GetEditCanvas() const {return m_pkEditCanvas;}

	void SetEditShape(sdEditShape* pkEditShape) { m_pkEditShape = pkEditShape;}
	sdEditShape* GetEditShape() const {return m_pkEditShape;}

	void SetBrushShape(sdBrushShape* pkBrushShape) { m_pkBrushShape = pkBrushShape;}
	sdBrushShape* GetBrushShape() const { return m_pkBrushShape;}

	virtual EBrushType GetType() = 0;
	virtual void Apply(const Base::Math::sdRay& kRay) = 0;

protected:
	sdBrushShapePtr	m_pkBrushShape;
	sdEditShapePtr	m_pkEditShape;
	sdEditCanvasPtr m_pkEditCanvas;
};
NiSmartPointer(sdEditBrush);
//-----------------------------------------------------------------------------
// 地表高度拉伸笔刷
//-----------------------------------------------------------------------------
class sdTerrainDeformPoolBrush : public sdEditBrush
{
public:
	sdTerrainDeformPoolBrush() : m_fHeight(1.f){}

	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_PULL;}
	virtual void Apply(const Base::Math::sdRay& kRay);

	// 属性访问
	float GetHeight() { return m_fHeight;}
	void SetHeight(float fHeight) { m_fHeight = fHeight;}	

protected:
	float	m_fHeight;	// 用于确定拉伸强度,可以为负
};
NiSmartPointer(sdTerrainDeformPoolBrush);
//-----------------------------------------------------------------------------
// 地表高度平滑笔刷
//-----------------------------------------------------------------------------
class sdTerrainDeformSmoothBrush : public sdEditBrush
{
public:
	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_SMOOTH;}
	virtual void Apply(const Base::Math::sdRay& kRay);
};
NiSmartPointer(sdTerrainDeformSmoothBrush);
//-----------------------------------------------------------------------------
// 地表高度噪声笔刷
//-----------------------------------------------------------------------------
class sdTerrainDeformNoiseBrush : public sdEditBrush
{
public:
	sdTerrainDeformNoiseBrush() : m_fHeight(10.f){}

	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_NOISE;}
	virtual void Apply(const Base::Math::sdRay& kRay);

	// 属性访问
	float GetHeight() { return m_fHeight;}
	void SetHeight(float fHeight) { m_fHeight = fHeight;}	

protected:
	float	m_fHeight;	// 用于确定噪声强度,可以为负
};
NiSmartPointer(sdTerrainDeformNoiseBrush);
//-----------------------------------------------------------------------------
// 地表高度踏平笔刷
//-----------------------------------------------------------------------------
class sdTerrainDeformFlatBrush : public sdEditBrush
{
public:
	sdTerrainDeformFlatBrush() : m_fHeight(0.f){}

	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_FLAT;}
	virtual void Apply(const Base::Math::sdRay& kRay);

	// 属性访问
	float GetHeight() { return m_fHeight;}
	void SetHeight(float fHeight) { m_fHeight = fHeight;}	

protected:
	float	m_fHeight;	// 用于确定踏平的高度
};
NiSmartPointer(sdTerrainDeformFlatBrush);
//-----------------------------------------------------------------------------
// 地表高度平均笔刷
//-----------------------------------------------------------------------------
class sdTerrainDeformAverageBrush : public sdEditBrush
{
public:
	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_AVERAGE;}
	virtual void Apply(const Base::Math::sdRay& kRay);
};
NiSmartPointer(sdTerrainDeformAverageBrush);
//-----------------------------------------------------------------------------
// 地表纹理编辑笔刷
//-----------------------------------------------------------------------------
class sdTerrainSurfaceLayerBrush : public sdEditBrush
{
public:
	sdTerrainSurfaceLayerBrush() : m_uiActiveLayerMap(0){}

	// 虚函数继承
	virtual EBrushType GetType() { return E_BRUSH_LAYER;}
	virtual void Apply(const Base::Math::sdRay& kRay);

	// 属性设置与访问
	uint GetActiveLayerMap() const { return m_uiActiveLayerMap;}
	void SetActiveLayerMap(uint uiIndex) { m_uiActiveLayerMap = uiIndex;}

protected:
	// 当前激活图层
	uint	m_uiActiveLayerMap;	
};
NiSmartPointer(sdTerrainSurfaceLayerBrush);
//-----------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif