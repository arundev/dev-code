//*************************************************************************************************
// ����:	����򵥵��߳����ݻ���(�ο���WZ::Client""Global::sdThreadDataBuff)
//---------------------------------------------------------
// ����:		
// ����:		2013-08-02
// ����޸�:	2014-01-14
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_THREAD_DATA_BUFF__
#define _SD_ENGINE_FILESYSTEM_THREAD_DATA_BUFF__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// �߳����ݻ���(ÿ�߳�һ��,������Ϊ�ļ���ȡ����)
//-------------------------------------------------------------------------------------------------
class sdThreadDataBuff : public Base::sdRefObject
{
public:
	enum
	{
		E_MAIN_THREAD_DATA_BUFF_SIZE = 25 * 1024 * 1024,	///< ���߳�25M
		E_RES_THREAD_DATA_BUFF_SIZE = 10 * 1024 * 1024,		///< ��Դ�߳�10M
		E_OTHER_THREAD_DATA_BUFF_SIZE = 1 * 1024 * 1024,	///< �����߳�1M
	};

public:
	sdThreadDataBuff();
	~sdThreadDataBuff();

	uchar* OpenBuff(uint uiSize);
	void CloseBuff();
	void ResizeBuff(uint uiSize);

protected:
	uchar* m_pucDataBuff;	///< ���ݻ���
	uint m_uiSize;			///< ���ݻ���ߴ�
};
sdSmartPointer(sdThreadDataBuff);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif