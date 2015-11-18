//*************************************************************************************************
// ����:	������β(ʵ��2)
//---------------------------------------------------------
// ����:		
// ����:		2012-01-16
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_GRAPHIC_WEAPONTRAIL_H__
#define _SD_ENGINE_GRAPHIC_WEAPONTRAIL_H__

ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// sdWeaponTrail
//	1.���Ƶ㲿�����û��λ���,�Լ���memcpy
//	2.��ֵ����ÿ��ֻ���¼ӿ��Ƶ���в�ֵ�Լ��ټ���
//-----------------------------------------------------------------------------
class sdWeaponTrail : public NiNode
{
	NiDeclareRTTI;
public:
	sdWeaponTrail();
	~sdWeaponTrail();

	// ����/����/��ֹ
	void Start();
	void Stop();
	void UpdateFrame(float fAccumTime, float fFrameTime);

	// Dummy������
	inline void SetDummyPosition(const NiPoint3& kPos);
	inline void SetDummyRotation(const NiMatrix3& kRot);
	

protected:

	// B-Spline��ֵ
	void bspline();

	//
	// De Boor's algorithm, һ��B������ֵ�㷨
	// �ο�:
	//	https://zh.wikipedia.org/wiki/�²����㷨
	//	https://en.wikipedia.org/wiki/De Boor's algorithm
	//
	// @param	
	void deBoor();

protected:
	float m_fAccumFrameTime;	///< ��һ�θ��µ����ڵ��ۻ�ʱ��(�������Ʒ�ֹ�������ʹ���)

	bool m_bStarted;	///< ��ǰ�Ƿ��Ѿ���ʼ����
	bool m_bStoping;	///< ��ǰ�Ƿ����ڽ�������(���ڵ�����β)

	NiPoint3  m_kDummyPosition;		///< DummyĿ��λ��
	NiPoint3  m_kDummyZDirection;	///< DummyĿ��Z�᷽��;

	float m_fBias;			///< �����λ��ƫ��
	float m_fWidth;			///< ������
	float m_fSegmentLength;	///< �����ֵ���(��λ:m)
	float m_fMaxLength;		///< ������󳤶�(��λ:m)
	int m_iMaxSampleNum;	///< ����������������

	float m_fAlpha;		///< ����͸����
	NiColor m_kColor;	///< ������ɫ

	NiPoint2 m_kUVScale;	///< UV����
	NiPoint2 m_kUBBias;		///< UVƫ��

	bool m_bShimmer;	///< ShimmerЧ��
	float m_fShimmerIntensity;

	typedef std::vector<float> LengthVec;
	typedef std::vector<float>::iterator LengthVecItr;
	typedef std::vector<NiPoint3> Pt3Vec;
	typedef std::vector<NiPoint3>::iterator Pt3VecItr;
	Pt3Vec		m_kCtrlPtVec0;			///< ���Ƶ㻺��
	Pt3Vec		m_kCtrlPtVec1;			///< ���Ƶ㻺��
	LengthVec	m_kCtrlSegmentLength;	///< ���Ƶ�֮���������
	int			m_iCtrlPtNum;			///< ��ǰ���Ƶ�����
	int			m_iStartCtrlPtIndex;	///< ��ǰ���Ƶ���ʼλ��(����ʽ����)

	// Mesh��������Ʋ���
	NiMeshPtr m_spMesh;
	NiFloatsExtraDataPtr m_spParams;
};
NiSmartPointer(sdWeaponTrail);
//-----------------------------------------------------------------------------
#include "sdWeaponTrail.h"
ENGINE_NAMESPACE_END_RENDERSYSTEM
#endif
