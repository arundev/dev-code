//*************************************************************************************************
// ����:	��Ϸ��ʾ���ù���
//---------------------------------------------------------
// ����:		
// ����:		2012-07-09
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_VIDEO_CONFIG_H__
#define _SD_GAMECORE_VIDEO_CONFIG_H__

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// ��ȡ��Ϸ����ini,��ȡ��ǰ��������Ϣ
class sdVideoConfig : public NiMemObject
{
public:
	sdVideoConfig();
	~sdVideoConfig();

	// ����ǰ���ó�ʼ��Ϊ��Ӧ��������
	//void InitLow();
	//void InitMid();
	//void InitHeight();
	//void InitBest();
	
public:
	unsigned int	m_uiWidth;
	unsigned int	m_uiHeight;
	bool			m_bVSync;
	bool			m_bFullScreen;

	// 
	unsigned int	m_uiMipmapSkipLevel;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif