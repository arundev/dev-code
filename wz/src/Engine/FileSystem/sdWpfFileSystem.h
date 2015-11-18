//*************************************************************************************************
// ����:	Wpf�ļ�ϵͳ
//---------------------------------------------------------
// ����:		
// ����:		2013-08-23
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_WPF_FILE_SYSTEM_H__
#define _SD_ENGINE_FILESYSTEM_WPF_FILE_SYSTEM_H__

#include <sdSingleton.h>
#include <IFileSystem.h>

//#include "sdWpfFileNameMap.h"
#include "sdThreadDataBuff.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
class sdWpfFileSystem : public IFileSystem
{
public:
	sdWpfFileSystem();
	~sdWpfFileSystem();

	// �麯���̳�
	virtual bool Initialize();
	virtual void Destroy();
	virtual void Update();

	virtual IDataStream* GetDataStream(const char* szFileName);

	// ���ݻ����(��һ�ζ��̻߳᲻��������)
	uchar* OpenThreadDataBuff(DWORD dwThreadId, DWORD dwSize);
	void CloseThreadDataBuff(DWORD dwThreadId);

protected:
	// �����ļ�����ȡ��Ӧ��Wpf�ļ��ӿ�,�������򴴽�֮
	//sdWpfFileNameMap::stWpfFileNameMapResult GetWpfFileNameMapResult(const char* szFileName);

	// ����Wpf����ȡ��Ӧ������������
	//sdDataStream* GetWpfFileStream(const sdWpfFileNameMap::stWpfFileNameMapResult& kResult);

	// ���ݻ����
	sdThreadDataBuff* GetThreadDataBuff(DWORD dwThreadId);
	void SetThreadDataBuff(DWORD dwThreadId, sdThreadDataBuff* pkThreadDataBuff);

protected:
//	sdWpfFileNameMap m_kWpfFileNameMap;	///< �ļ�·����Wpf����ӳ���ѯ
	DWORD m_dwMainThreadId;				///< ���߳�ID

	// �߳���
	Base::Threading::sdCriticalSection m_kCriticalSection;

	// �߳����ݻ���(������ʱ���IO����)
	typedef std::vector<sdThreadDataBuffPtr> ThreadDataBuffVec;
	typedef std::vector<sdThreadDataBuffPtr>::iterator ThreadDataBuffVecItr;
	ThreadDataBuffVec m_kThreadDataBufVec;	///< ���ݻ����

	typedef std::map<uint, sdThreadDataBuff*> ThreadDataBufMap;
	typedef std::map<uint, sdThreadDataBuff*>::iterator ThreadDataBufMapItr;
	ThreadDataBufMap m_kThreadDataBufMap;	///< �̵߳����ݻ����ʹ�����


	uint m_uiThreadDataBuffSize;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif