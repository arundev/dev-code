//*************************************************************************************************
// ����:	
//		Hermite��ֵ
//
// ����:
//		0.����ʹ�õ���Cubic Hermite Spline
//		1.[0,1]������ڲ�
//		 ��֪f(t), ����
//				f(0)  = p0,		f(1)  = p1, 
//				f`(0) = m0,		f`(1) = m1,
//		 ��,
//				f(t) = (2 * t * t * t - 3 * t * t + 1) * p0 + 
//					   (t * t * t - 2 * t * t + t) * m0 +
//					   (-2 * t * t * t + 3 * t * t) * p1 +
//					   (t * t * t - t * t) * m1
//
// �ο���ַ:
//		https://en.wikipedia.org/wiki/Hermite_form
//		https://en.wikipedia.org/wiki/Hermite_interpolation
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// ����:		
// ����:		2012-12-27
// ����޸�:	2012-12-27
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_HERMITE_SHAPE_H__
#define _SD_ENGINE_UTIL_HERMITE_SHAPE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL

class sdHermiteShape : public NiRefObject
{
public:
	sdHermiteShape();
	~sdHermiteShape();

	// ���Ƶ�����༭
	inline int AddPoint(const NiPoint3& kPos);
	inline void	SetPoint(int iIndex, const NiPoint3& kPos);
	inline void	SetPoints(const std::vector<NiPoint3>& kPoint3Vec);
	inline void	RemovePoint(int iIndex);
	inline void	RemoveAllPoints();
	inline NiPoint3& GetPoint(int iIndex);
	inline std::vector<NiPoint3>& GetPoints();
	inline const std::vector<NiPoint3>& GetPoints() const;

	// ��ֵ���������
	inline std::vector<NiPoint3>& GetInterPoints();
	inline const std::vector<NiPoint3>& GetInterPoints() const;

	// ��չ���������
	inline std::vector<NiPoint3>& GetExtendPoints();
	inline const std::vector<NiPoint3>& GetExtendPoints() const;

	// ��ֵ����չ�뾶
	inline void SetExtendWidth(float fWidth);
	inline float GetExtendWidth();

	// �����Ƶ�֮������ֵ������
	inline void SetMaxInterNum(int iNum);
	inline int GetMaxInterNum();

	// �ӿ��Ƶ����ɲ�ֵ��
	bool Interpolation();

	// ��չ��ֵ��
	bool ExtentInterPoint();
	
	// ��ȡʹ�õ��ڴ�ĳߴ�
	//virtual uint GetMemSize();

protected:
	// (vector��ʹ���±����ʱ����Խ��,����ʹ��ʱ��ָ��������)
	typedef std::vector<NiPoint3> Point3Vec;
	typedef std::vector<NiPoint3>::iterator Point3VecItr;
	Point3Vec	m_kCtrlPointVec;	///< ԭʼ���Ƶ�
	Point3Vec	m_kInterPointVec;	///< ��ֵ������ĵ�
	Point3Vec	m_kExtendPointVec;	///< ������չ��Ķ���

	float m_fExtendWidth;	///< ��ֵ����չ���
	int	m_iMaxInterNum;		///< �����Ƶ�֮������ֵ������,Ϊ-1������
};
NiSmartPointer(sdHermiteShape);

#include "sdHermiteShape.inl"

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif