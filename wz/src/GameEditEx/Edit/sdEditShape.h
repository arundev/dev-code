//*************************************************************************************************
// ����:	��ˢ��״,����һ��ʸ����״������һ��դ��ͼ,
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITSHAPE_H__
#define _SD_GAMEEDITEX_EDITSHAPE_H__

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-----------------------------------------------------------------------------
// ����һ��༭����
//-----------------------------------------------------------------------------
class sdEditShape : public NiRefObject
{
public:
	sdEditShape(){};
	virtual ~sdEditShape(){};
};
NiSmartPointer(sdEditShape);
//-----------------------------------------------------------------------------
// ���ﵥλ��,��������
//-----------------------------------------------------------------------------
class sdCircleShape : public sdEditShape
{
public:
	sdCircleShape();
	virtual ~sdCircleShape();

	// ��ȡָ��λ��Ȩ��
	bool	InShape(float fX, float fY);
	float	GetWeight(float fX, float fY);

	// 
	void SetCenter(const NiPoint2& kPos) { m_kCenter = kPos;}
	void SetCenter(float fX, float fY) { m_kCenter.x = fX; m_kCenter.y = fY;}
	const NiPoint2& GetCenter() const { return m_kCenter;}

	void SetRadius(float fRadius){ m_fRadius = fRadius;}
	float GetRadius() const { return m_fRadius;}
protected:
	NiPoint2	m_kCenter;
	float		m_fRadius;
};
NiSmartPointer(sdCircleShape);
//-----------------------------------------------------------------------------
class sdDualCircleShape : public sdEditShape
{
public:
	sdDualCircleShape();
	virtual ~sdDualCircleShape();

	// ��ȡָ��λ��Ȩ��
	bool	InShape(float fX, float fY);
	float	GetWeight(float fX, float fY);	///< [-1.f, 1.f]

	// 
	void SetCenter(const NiPoint2& kPos) { m_kCenter = kPos;}
	void SetCenter(float fX, float fY) { m_kCenter.x = fX; m_kCenter.y = fY;}
	const NiPoint2& GetCenter() const { return m_kCenter;}

	void SetInnerRadius(float fInnerRadius);
	void SetOuterRadius(float fOuterRadius);
	float GetInnerRadius() const { return m_fInnerRadius;}
	float GetOuterRadius() const { return m_fOuterRadius;}

	void  SetHardness(float fHardness);
	float GetHardness() const { return m_fHardness;}
protected:
	NiPoint2	m_kCenter;
	float	m_fInnerRadius;
	float	m_fOuterRadius;
	float	m_fHardness;
};
NiSmartPointer(sdDualCircleShape);
//-----------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif