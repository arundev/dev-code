//*************************************************************************************************
// ����:	�ر�߶�ͼ
//
// �ο���ַ:
//		https://en.wikipedia.org/wiki/Natural_neighbor_interpolation
//		https://en.wikipedia.org/wiki/Bilinear_interpolation
//		https://en.wikipedia.org/wiki/Bicubic_interpolation
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// ����:		
// ����:		2012-08-01
// ����޸�:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_HEIGHT_MAP_H__
#define _TERRAIN_HEIGHT_MAP_H__

#include "sdCubicSampler.h"

class sdQuadMesh;

//
// �ر�߶�ͼ,
//	1.������������,��С������pow(2,n)����λ
//	2.GBĬ����������ϵ,����ΪX,��ǰΪY,����ΪZ
//	3.Ĭ�ϸ߶�ͼ��(0,0,0)�㿪ʼ��XY����������
//	4.����ֻ������uiSpacingΪ1�����
//
// ע�ͣ�
//	1.WZ��Ĭ�ϳߴ���pow(2,n) + 1 + 2�� 
//		pow(2,n)	�ǵؿ��С
//		1			�Ƕ���Ķ���
//		2			�����߱�Ե�������������ȹ��
//	2.WZ��Ĭ��������[-1, pow(2,n)]
//
// ע�⣺
//	1.Ŀǰ��֧��m_uiSpacing��1�����
//
// \TODO ����Ԥ���˵ĵͷֱ��ʸ߶�ͼ,���ڴ���LOD
//
class sdHeightMap : public NiRefObject
{
	friend sdQuadMesh;
public:
	sdHeightMap(uint uiSize);
	sdHeightMap(uint uiSize, float fMinHeight, float fMaxHeight);
	~sdHeightMap();

	// ���Է���
	uint	GetSpacing() const { return m_uiSpacing;}
	uint	GetSize() const { return m_uiSize;};
	float	GetMinHeight() const { return m_fMinHeight;}
	float	GetMaxHeight() const { return m_fMaxHeight;}
	float	GetAlignHeight() const { return m_fAlignHeight;}

	// �߶�(�߶�ͼ�������꣬��Ч��Χ[0, m_uiSize],ԭʼ�߶�)
	//	0.ʹ��int������uint��Ϊ�˱��ڴ����������
	//	1.Խ���߽�Ķ�ȡ���ȡ��Ӧ�߽��
	//	2.Խ���߽��д�벻����
	void	SetRawHeight(int iX, int iY, float fHeight);
	float	GetRawHeight(int iX, int iY);

	// �߶�(����������������㴦,��ֵ�߶�)
	float	GetHeight(int iX, int iY);

	// �߶�(������������λ��,��ֵ�߶�)
	//	1.����ͼ����Ⱦ�ķ�ʽ������������,���������ڲ����в�ֵ
	//	2.˫���Բ�ֵ(2x2)
	float	GetHeight(float fX, float fY);
	float	GetHeight_Bilinear(float fX, float fY);

	// ����(����������������㴦,ƽ������)
	void	GetNormal(int iX, int iY, float& fNX, float& fNY, float& fNZ);

	// �߶�(����������������㴦,��ֵ�¶�, ���˾�����Normal���㲻һ��)
	void	GetGradients(int iX, int iY, float& fGX, float& fGY);

	// ɨ�辫ϸ������ĵ㣬��ȡ�μ������뾫ϸ��������ָ�����괦���ֵ(���һ��)
	// (Ϊ�����Ч��,����û�м���������)
	//	@param[in]	iStartX,iStartY	������Ը߶�ͼ����ʼ����λ��
	//	@param[in]	iX,iY			��ϸ���������������[0, m_uiSize * m_uiSpacing / uiSpacing]
	//	@param[in]	uiSpacing		��ϸ������������С,Ӧ������power(2,n),��1/2/4/8/16/32
	float	GetMorphDelta(int iStartX, int iStartY, int iX, int iY, uint uiSpacing);
	//float	GetMorphDelta(int iStartX, int iStartY, int iWidth, int iHeight, uint uiSpacing);

protected:
	//NiPoint3	m_kOrigin;			// �߶�ͼ��ʼλ��
	//float		m_fScale;			// �߶�ͼ��������
	//float		m_fHorizontalScale;	// ˮƽ����
	//float		m_fVerticalScale;	// ��ֱ����,���߱�(��ʾʱ�߶ȿ��ű���)
	//float		m_fBaseHeight;		// �߶�ͼ��׼�߶�

	// �߶�ͼ����(���Ǿ�ȷ��ʽ,���Կ���ѹ��Ϊunsigned short)
	float	*m_pfHeight;

	// �߶�ͼ������(Cry�д˲���,һ����1/2/4/8/16)
	uint	m_uiSpacing;
	float	m_fSpacingDiv;	///< �����൹��

	// �߶�ͼ�߳���С
	uint	m_uiSize;		// pow(2,n)
	uint	m_uiAllocSize;	// pow(2,n) + 1

	// �������С�߶�,ѹ������ʱʹ��
	float	m_fMinHeight;
	float	m_fMaxHeight;

	// �߶ȶ��뵥λֵ,������߶ȵ���С�ı�߶�
	// (��ʱ��Ⱦ��������Ҫѹ���������ݶ�ʹ�ö��㸡����)
	float	m_fAlignHeight;

	// Cubic��������������iSpacing��Ϊ1ʱ������λλ�õĸ߶�
	static sdCubicSampler ms_kCubicSampler;
};
NiSmartPointer(sdHeightMap);
#endif