//*************************************************************************************************
// ����:		KF��Դ�����
//---------------------------------------------------------
// ����:		
// ����:		2013-02-01
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_KF_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_KF_RESOURCE_POOL_H__
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
class sdKfResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// �麯���̳�
	virtual void RemoveAllInstance();	

	// ����Kf�ļ�
	NiSequenceDataPtr LoadSequenceData(const std::string& kFileName, NiKFMTool::Sequence* pkKFMSequence, NiStream* spStream);

	// �ͷ�Kf�ļ�
	void FreeSequenceData(NiActorManager* spActorMgr);
	
protected:
	sdKfResourcePool();
	virtual ~sdKfResourcePool();

	// ����
	NiSequenceDataPtr GetSequenceDataFromArray(NiKFMTool::Sequence* spKFMSequence, std::vector<NiSequenceDataPtr>& kSeqDataVec);

	// ��Ӳ�̼�����Դ
	bool LoadFromDisk(const std::string& kFileName, NiStream* spStream, std::vector<NiSequenceDataPtr>& kSeqDataVec);

	// ����
	void FreeSequenceData(const std::string& kFileName, NiKFMTool::Sequence* spKFMSequence);

protected:
	class CacheSequenceInfo : public NiMemObject
	{
	public:
		std::vector<NiSequenceDataPtr> m_kSequenceDataVec;
		int m_iCloneCount;	///< ����¡�Ĵ���

	public:
		CacheSequenceInfo() : m_iCloneCount(0){}
	};

protected:
	typedef std::map<std::string, CacheSequenceInfo> CacheSequenceMap;
	typedef std::map<std::string, CacheSequenceInfo>::iterator CacheSequenceMapItr;
	CacheSequenceMap m_kCacheSequenceMap;
};
NiSmartPointer(sdKfResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif