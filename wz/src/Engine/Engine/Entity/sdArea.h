//*************************************************************************************************
// 内容:	场景区域
//	1.用于动态控制全局环境设设置,包括音效/环境系统/物理阻挡/区域触发器等
//	2.区域常见的实现方式有以下:
//    - 3D Box/Sphere/EditableMesh, 使用Octree之类的场景管理
//    - 2D BitMap
//	  - 3D MultiHeightMap
//	 这里使用第一种
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_AREA_H__
#define _SD_ENGINE_SCENE_AREA_H__
#include "sdEntity.h"

class IAreaListener;
//---------------------------------------------------------------------------------------
// 区域基类
//---------------------------------------------------------------------------------------
class sdArea : public sdEntity
{
public:
	sdArea();
	virtual ~sdArea();

	// 监听器处理
	static void				SetListener(IAreaListener* pkListener);
	static IAreaListener*	GetListener();

	// 虚函数继承
	//virtual void UpdateGame(){};

	// 属性设置与访问
	
protected:
	virtual	bool	IsInArea();

	void	OnEnterArea();
	void	OnStayArea();
	void	OnLeaveArea();

protected:
	//sdArea2DAudioFX			m_k2DAudioFX;
	//sdArea3DAmbientAudioFX	m_k3DAmbientAudioFX;
	//sdAreaReverbAudioFX		m_kReverbAudioFX;

	// 监听器
	static IAreaListener*	ms_pkListener;
};
//---------------------------------------------------------------------------------------
// 盒子型规则区域
//---------------------------------------------------------------------------------------
class sdBoxArea : public sdArea
{
public:
	sdBoxArea();
	virtual ~sdBoxArea();

	// 属性访问
	//float	GetDimX() const;
	//float	GetDimY() const;
	//float	GetDimZ() const;

	//void	SetDimX(float fVal);
	//void	SetDimY(float fVal);
	//void	SetDimZ(float fVal);

protected:
	float	m_fDimX;
	float	m_fDimY;
	float	m_fDimZ;
};
//---------------------------------------------------------------------------------------
// 球型规则区域
//---------------------------------------------------------------------------------------
class sdSphereArea : public sdArea
{
public:
	sdSphereArea();
	virtual ~sdSphereArea();

	// 属性访问
	//float	GetRadius() const;
	//void	SetRadius(float fVal);

protected:
	float	m_fRadius;
};
//---------------------------------------------------------------------------------------
// 不规则区域
//---------------------------------------------------------------------------------------
class sdMeshArea : public sdArea
{
public:
	sdMeshArea();
	virtual ~sdMeshArea();

	// 属性访问


protected:
	
};
//---------------------------------------------------------------------------------------
// 片型区域
//---------------------------------------------------------------------------------------
class sdSliceArea : public sdArea
{
public:
	sdSliceArea();
	virtual ~sdSliceArea();

	// 属性访问


protected:

};
//---------------------------------------------------------------------------------------
#include "sdArea.inl"
//---------------------------------------------------------------------------------------
#endif