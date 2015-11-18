//*************************************************************************************************
// ����:	�������,
//		�û��༰������ֱ�ӷ�װD3D����,������GB�������ʽ�ṩ������ʹ��,
//		�Ա���ʵ��һЩGB�ڲ���֧�ֵĹ��ܵ�ͬʱ�����޸�GB����	
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_H__
#define _SD_ENGINE_TEXTURE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// ��Ҫ�Ƿ�װһЩ�����й���(�д���һ�����ƽӿ�)
//	1.�豸���账��
//-------------------------------------------------------------------------------------------------
class sdTexture : public NiRefObject
{
public:
	sdTexture();
	virtual ~sdTexture();

	// ��̬��ʼ��
	static void	StaticInitialize();
	static void StaticDestroy();

protected:
	// �ҽӵ���������
	void AddTextureToList();
	void RemoveTextureFromList();

	// �豸�ص�����
	virtual bool OnDeviceLost() = 0;
	virtual bool OnDeviceReset(bool bBefore) = 0;

	// �豸�ص�����(ע���GB)
	static bool DeviceLostCallBack(void* pvData);
	static bool DeviceResetCallBack(bool bBeforeReset, void* pvData);

protected:
	// GB�豸�ص�����
	static uint	ms_uiLostNotifyIndex;
	static uint	ms_uiResetNotifyIndex;

	// ����Texture����˫������(�ο�GB/NiTexture.h)
	//	1.�豸��ʧ������ʱ�������ж���
	sdTexture*	m_pkPrev;
	sdTexture*	m_pkNext;
	static sdTexture*	ms_pkHead;
	static sdTexture*	ms_pkTail;
};
NiSmartPointer(sdTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif