//*************************************************************************************************
// ����:	���α�ˢͼ��(������ʾ)
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITBRUSHSHAPE_H__
#define _SD_GAMEEDITEX_EDITBRUSHSHAPE_H__

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-----------------------------------------------------------------------------
// ���Ʊ�ˢ����
//-----------------------------------------------------------------------------
class sdBrushShape : public NiNode
{
public:
	virtual ~sdBrushShape(){};

	// ���ݵ�ǰλ��,����ʰȡ���θ߶�,������ʾ
	virtual void UpdateShape(){};
};
NiSmartPointer(sdBrushShape);
//-----------------------------------------------------------------------------
// ˫Բ����ˢ
//-----------------------------------------------------------------------------
class sdDualCircleMesh : public sdBrushShape
{
public:
	sdDualCircleMesh();
	~sdDualCircleMesh();

	// �麯���̳�
	void UpdateShape();

	// ����
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

	// Բˢ��Բ�ܲ�������
	const static uint ms_uiCirclePointCount = 50;
};
NiSmartPointer(sdDualCircleMesh);
//-----------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif