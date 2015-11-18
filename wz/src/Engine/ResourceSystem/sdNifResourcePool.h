//*************************************************************************************************
// ����:		NIF��Դ�����
//---------------------------------------------------------
// ����:		
// ����:		2013-01-31
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_POOL_H__
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
class sdNifResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// �麯���̳�
	virtual void RemoveAllInstance();	

	// ����Nif�ļ�
	NiNodePtr LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, bool bMainThreadLoad = false);
	NiNodePtr LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream);

	// �ͷ�Nif�ļ�
	void FreeNifFile(const std::string& kFileName);
	void FreeKFMRoot(NiActorManager* spActorMgr);

	// ������з�KFMRoot��NIF Cache
	void RemoveGCInstance();
	
protected:
	sdNifResourcePool();
	virtual ~sdNifResourcePool();

	// ��Ӳ�̼�����Դ
	NiNodePtr LoadFromDisk(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream);

protected:
	class CacheNifInfo : public NiMemObject
	{
	public:
		NiNodePtr m_spRootNode;
		int m_iCloneCount;	///< ����¡�Ĵ���
		bool m_bKFMRoot;	///< �Ƿ���KFM���ڵ�

	public:
		CacheNifInfo() : m_iCloneCount(0), m_bKFMRoot(false){}
	};

protected:
	typedef std::map<std::string, CacheNifInfo> CacheNifMap;
	typedef std::map<std::string, CacheNifInfo>::iterator CacheNifMapItr;
	CacheNifMap m_kCacheNifMap;

};
NiSmartPointer(sdNifResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif