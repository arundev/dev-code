//*************************************************************************************************
// ����:	��������
//	1.���ڶ�̬����ȫ�ֻ���������,������Ч/����ϵͳ/�����赲/���򴥷�����
//	2.���򳣼���ʵ�ַ�ʽ������:
//    - 3D Box/Sphere/EditableMesh, ʹ��Octree֮��ĳ�������
//    - 2D BitMap
//	  - 3D MultiHeightMap
//	 ����ʹ�õ�һ��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-23
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_AREA_H__
#define _SD_ENGINE_SCENE_AREA_H__
#include "sdEntity.h"

class IAreaListener;
//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
class sdArea : public sdEntity
{
public:
	sdArea();
	virtual ~sdArea();

	// ����������
	static void				SetListener(IAreaListener* pkListener);
	static IAreaListener*	GetListener();

	// �麯���̳�
	//virtual void UpdateGame(){};

	// �������������
	
protected:
	virtual	bool	IsInArea();

	void	OnEnterArea();
	void	OnStayArea();
	void	OnLeaveArea();

protected:
	//sdArea2DAudioFX			m_k2DAudioFX;
	//sdArea3DAmbientAudioFX	m_k3DAmbientAudioFX;
	//sdAreaReverbAudioFX		m_kReverbAudioFX;

	// ������
	static IAreaListener*	ms_pkListener;
};
//---------------------------------------------------------------------------------------
// �����͹�������
//---------------------------------------------------------------------------------------
class sdBoxArea : public sdArea
{
public:
	sdBoxArea();
	virtual ~sdBoxArea();

	// ���Է���
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
// ���͹�������
//---------------------------------------------------------------------------------------
class sdSphereArea : public sdArea
{
public:
	sdSphereArea();
	virtual ~sdSphereArea();

	// ���Է���
	//float	GetRadius() const;
	//void	SetRadius(float fVal);

protected:
	float	m_fRadius;
};
//---------------------------------------------------------------------------------------
// ����������
//---------------------------------------------------------------------------------------
class sdMeshArea : public sdArea
{
public:
	sdMeshArea();
	virtual ~sdMeshArea();

	// ���Է���


protected:
	
};
//---------------------------------------------------------------------------------------
// Ƭ������
//---------------------------------------------------------------------------------------
class sdSliceArea : public sdArea
{
public:
	sdSliceArea();
	virtual ~sdSliceArea();

	// ���Է���


protected:

};
//---------------------------------------------------------------------------------------
#include "sdArea.inl"
//---------------------------------------------------------------------------------------
#endif