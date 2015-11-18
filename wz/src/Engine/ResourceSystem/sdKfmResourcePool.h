//*************************************************************************************************
// ����:		KFM��Դ�����
//---------------------------------------------------------
// ����:		
// ����:		2013-02-01
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_POOL_H__
#include "sdResourcePool.h"
#include <IResource.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
class sdResourceSystem;
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdKfmResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// �麯���̳�
	virtual void RemoveAllInstance();	

	// ����Kfm�ļ�
	NiActorManagerPtr LoadKfmFile(const std::string& kFileName, EResUsageType eUsageType, bool bReleaseOnGC = true, bool bMainThreadLoad = false);

	// �ͷ�Kfm�ļ�
	void FreeKFMFile(const std::string& kFileName);

	// ���
	void RemoveGCInstance(const std::set<std::string>& kNoReleaseMap);
	
protected:
	sdKfmResourcePool();
	virtual ~sdKfmResourcePool();

	// ��Ӳ�̼�����Դ
	NiActorManagerPtr LoadFromDisk(const std::string& kFileName, bool bMainThreadLoad = false);

protected:
	class CacheKfmInfo : public NiMemObject
	{
	public:
		NiActorManagerPtr m_spActorMgr;
		int m_iCloneCount;		///< ����¡�Ĵ���
		bool m_bReleaseOnGC;

	public:
		CacheKfmInfo() : m_iCloneCount(0), m_bReleaseOnGC(true){}
	};

protected:
	typedef std::map<std::string, CacheKfmInfo> CacheKfmInfoMap;
	typedef std::map<std::string, CacheKfmInfo>::iterator CacheKfmInfoMapItr;
	CacheKfmInfoMap m_kCacheKfmInfoMap;
};
NiSmartPointer(sdKfmResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif