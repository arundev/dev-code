#include "sdFileSystemPCH.h"
#include "sdWpfFileSystem.h"
#include "sdDataStream.h"

#define DEFAULT_RESOURCE_THREAD_NUM 3

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
sdWpfFileSystem::sdWpfFileSystem()
{
	m_dwMainThreadId = ::GetCurrentThreadId();	///< 获取主线程ID
}
//-------------------------------------------------------------------------------------------------
sdWpfFileSystem::~sdWpfFileSystem()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------
bool sdWpfFileSystem::Initialize()
{
	if (m_bInitialized)
		return true;


	// 初始化线程数据缓存池
	m_uiThreadDataBuffSize = 0;
	m_kThreadDataBufVec.resize(DEFAULT_RESOURCE_THREAD_NUM);

	ThreadDataBuffVecItr kItr = m_kThreadDataBufVec.begin();
	ThreadDataBuffVecItr kItrBegin = m_kThreadDataBufVec.begin();
	ThreadDataBuffVecItr kItrEnd = m_kThreadDataBufVec.end();
	for (; kItr != kItrEnd; ++kItr)
	{
		sdThreadDataBuffPtr pkThreadDataBuff = new sdThreadDataBuff;
		(*kItr) = pkThreadDataBuff;
		
		if (kItr == kItrBegin)
			pkThreadDataBuff->ResizeBuff(sdThreadDataBuff::E_MAIN_THREAD_DATA_BUFF_SIZE);
		else
			pkThreadDataBuff->ResizeBuff(sdThreadDataBuff::E_RES_THREAD_DATA_BUFF_SIZE);
	}


	return m_bInitialized = true;
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileSystem::Destroy()
{
	m_bInitialized = false;

	// 销毁线程数据缓存池
	ThreadDataBuffVecItr kItr = m_kThreadDataBufVec.begin();
	ThreadDataBuffVecItr kItrEnd = m_kThreadDataBufVec.end();
	for (; kItr != kItrEnd; ++kItr)
		(*kItr) = 0;
	m_kThreadDataBufVec.clear();

	m_kThreadDataBufMap.clear();

	m_uiThreadDataBuffSize = 0;
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileSystem::Update()
{

}
//-------------------------------------------------------------------------------------------------
IDataStream* sdWpfFileSystem::GetDataStream(const char* szFileName)
{
	// 优先尝试读取目录文件,其次才去wpf包查找
	sdDataStream* pkDataStream = NULL;
	FILE* pkFile = fopen(szFileName, "rb");
	if (pkFile)
	{
		pkDataStream = new sdFileStream(pkFile);
	}
	else
	{
		
	}

	return pkDataStream;
}
//-------------------------------------------------------------------------------------------------
//sdWpfFileNameMap::stWpfFileNameMapResult sdWpfFileSystem::GetWpfFileNameMapResult(const char* szFileName)
//{
//	m_kCriticalSection.Lock();
//
//	sdWpfFileNameMap::stWpfFileNameMapResult kResult;
//	//if (m_kWpfFileNameMap.GetWpfInterface(szFileName, kResult) && !kResult.pkWpfInterface && kResult.szWpfName[0] != '\0')
//	//{
//	//	sdWpfLoader* pkWpfInterface = new sdWpfLoader;	
//	//	if (!pkWpfInterface->WpfOpen(kResult.szWpfName))
//	//	{
//	//		delete pkWpfInterface;
//	//		pkWpfInterface = NULL;
//
//	//		m_kCriticalSection.Unlock();
//	//		return kResult;
//	//	}
//
//	//	kResult.pkWpfInterface = pkWpfInterface;
//	//	m_kWpfFileNameMap.SetWpfInterface(kResult.szWorldName, kResult.eWpfNameIndex, kResult.pkWpfInterface);
//	//}
//
//	m_kCriticalSection.Unlock();
//	return kResult;
//}
//-------------------------------------------------------------------------------------------------
uchar* sdWpfFileSystem::OpenThreadDataBuff(DWORD dwThreadId, DWORD dwSize)
{
	sdThreadDataBuff* pkThreadDataBuff = GetThreadDataBuff(dwThreadId);
	if (!pkThreadDataBuff)
	{
		if (m_uiThreadDataBuffSize < m_kThreadDataBufVec.size())
		{
			pkThreadDataBuff = m_kThreadDataBufVec[m_dwMainThreadId];
		}
		else
		{
			pkThreadDataBuff = new sdThreadDataBuff;
			pkThreadDataBuff->ResizeBuff(sdThreadDataBuff::E_OTHER_THREAD_DATA_BUFF_SIZE);
			
			m_kThreadDataBufVec.push_back(pkThreadDataBuff);
		}

		SetThreadDataBuff(dwThreadId, pkThreadDataBuff);
		m_dwMainThreadId++;
	}

	return pkThreadDataBuff->OpenBuff(dwSize);
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileSystem::CloseThreadDataBuff(DWORD dwThreadId)
{
	sdThreadDataBuff* pkThreadDataBuff = GetThreadDataBuff(dwThreadId);
	if (pkThreadDataBuff)
		pkThreadDataBuff->CloseBuff();
}
//-------------------------------------------------------------------------------------------------
sdThreadDataBuff* sdWpfFileSystem::GetThreadDataBuff(DWORD dwThreadId)
{
	ThreadDataBufMapItr kItr = m_kThreadDataBufMap.find(dwThreadId);
	ThreadDataBufMapItr kItrEnd = m_kThreadDataBufMap.end();
	if (kItr != kItrEnd)
		return kItr->second;
	return NULL;
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileSystem::SetThreadDataBuff(DWORD dwThreadId, sdThreadDataBuff* pkThreadDataBuff)
{
	m_kThreadDataBufMap[dwThreadId] = pkThreadDataBuff;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE