//*************************************************************************************************
// ����:	
//		Bezier��ֵ
//
// ����:
//		0.����ʹ�õ���Cubic Bezier Spline
//		1.[0,1]������ڲ�
//		 ��֪f(t), ����
//				f(0)  = p0,		f(1)  = p3, 
//		 ��,
//				f(t) = (1 - t) * (1 - t) * (1 - t) * p0 + 
//					   (1 - t) * (1 - t) * t * 3 * p1 +
//					   (1 - t) * t * t * 3 * p2 +
//					   t * t * t * p3;
//
// �ο���ַ:
//		https://en.wikipedia.org/wiki/B��zier curve
//		https://en.wikipedia.org/wiki/De Casteljau's algorithm	����û��ʹ�ø��㷨
//---------------------------------------------------------
// ����:		
// ����:		2012-01-17
// ����޸�:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_BEZIER_SHAPE_H__
#define _SD_ENGINE_UTIL_BEZIER_SHAPE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL

class sdBezierTable : public NiRefObject
{
public:

protected:
	
	// �ӿ��Ƶ����ɲ�ֵ��
	void UpdateTable(int iLockHandle);

protected:
	std::vector<uchar> m_kTable;
	std::vector<uchar> m_kLockedIndiceTable;
};
NiSmartPointer(sdBezierTable);

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif