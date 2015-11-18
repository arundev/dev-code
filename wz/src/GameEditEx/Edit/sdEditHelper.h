//*************************************************************************************************
// ����:	�༭������
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITHELPER_H__
#define _SD_GAMEEDITEX_EDITHELPER_H__

class sdMap;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// �༭������
//	1.ʵ��һЩ��״̬�޹صĸ����༭����
//	2.�Եײ㹦�ܽ��г���
class sdEditHelper : public NiRefObject
{
public:
	sdEditHelper();

	// �ײ������ʹ���
	sdMap*	GetMap() const { return m_pkMap;};

	// ��Ⱦ����
	// @{
	// ��ͼͼ�㿪��
	void SetEnableMapChannel(int iChannel, bool bEnable);
	// @}

	// ������������
	// @{
	
	// @}

	// ������Ч����
	// @{
	void SetEnableMLAA(bool bEnable);
	bool GetEnableMLAA();

	void SetEnableColorGrading(bool bEnable);
	bool GetEnableColorGrading();

	void SetEnableDof(bool bEnable);
	bool GetEnableDof();

	void SetEnableGodRay(bool bEnable);
	bool GetEnableGodRay();

	void SetEnableHDR(bool bEnable);
	bool GetEnableHDR();
	// @}

protected:
	sdMap*	m_pkMap;	//��ǰ�༭�ĵ�ͼ
};
NiSmartPointer(sdEditHelper);
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif