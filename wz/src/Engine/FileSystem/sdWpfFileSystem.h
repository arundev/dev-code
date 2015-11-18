//*************************************************************************************************
// 内容:	Wpf文件系统
//---------------------------------------------------------
// 作者:		
// 创建:		2013-08-23
// 最后修改:
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

	// 虚函数继承
	virtual bool Initialize();
	virtual void Destroy();
	virtual void Update();

	virtual IDataStream* GetDataStream(const char* szFileName);

	// 数据缓存池(这一段多线程会不会有问题)
	uchar* OpenThreadDataBuff(DWORD dwThreadId, DWORD dwSize);
	void CloseThreadDataBuff(DWORD dwThreadId);

protected:
	// 根据文件名获取对应的Wpf文件接口,不存在则创建之
	//sdWpfFileNameMap::stWpfFileNameMapResult GetWpfFileNameMapResult(const char* szFileName);

	// 根据Wpf来获取对应的数据流缓存
	//sdDataStream* GetWpfFileStream(const sdWpfFileNameMap::stWpfFileNameMapResult& kResult);

	// 数据缓存池
	sdThreadDataBuff* GetThreadDataBuff(DWORD dwThreadId);
	void SetThreadDataBuff(DWORD dwThreadId, sdThreadDataBuff* pkThreadDataBuff);

protected:
//	sdWpfFileNameMap m_kWpfFileNameMap;	///< 文件路径到Wpf包的映射查询
	DWORD m_dwMainThreadId;				///< 主线程ID

	// 线程锁
	Base::Threading::sdCriticalSection m_kCriticalSection;

	// 线程数据缓存(用于临时存放IO数据)
	typedef std::vector<sdThreadDataBuffPtr> ThreadDataBuffVec;
	typedef std::vector<sdThreadDataBuffPtr>::iterator ThreadDataBuffVecItr;
	ThreadDataBuffVec m_kThreadDataBufVec;	///< 数据缓存池

	typedef std::map<uint, sdThreadDataBuff*> ThreadDataBufMap;
	typedef std::map<uint, sdThreadDataBuff*>::iterator ThreadDataBufMapItr;
	ThreadDataBufMap m_kThreadDataBufMap;	///< 线程的数据缓存池使用情况


	uint m_uiThreadDataBuffSize;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif