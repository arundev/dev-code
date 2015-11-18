//*************************************************************************************************
// ����:		��Դ
//---------------------------------------------------------
// ����:		
// ����:		2013-01-31
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_H__
#include <IResource.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
class sdResourceGroup;
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdResource : public IResource
{
	friend class sdResourceGroup;
public:
	sdResource(const std::string& kFileName, bool bReleaseOnGC, EResUsageType eUsageType);
	virtual ~sdResource();

	// �麯���̳�
	inline virtual void IgnoreResource();
	inline virtual bool Loaded();
	inline virtual bool Ignored();
	inline virtual bool Cached();
	inline virtual bool Freed();

	inline virtual void UpdatePrior(EResReqPrior ePrior, float fSqlDistance);

	inline virtual ResTicket GetTicket() const;
	inline virtual NiNode* GetRootNode();
	inline virtual NiActorManager* GetActorManager();

	// ���Է���
	inline const std::string& GetFileName() const;
	inline EResUsageType GetUsageType() const;
	inline EResReqPrior GetRequestPrior() const;

protected:
	inline void SetTicket(ResTicket kTicket);

protected:
	std::string		m_kFileName;	///< ��Դ�ļ���
	EResUsageType	m_eUsage;		///< ��Դʹ������
	EResReqPrior	m_ePrior;		///< ��Դ�������ȼ�	
	float			m_fSqlDistance;	///< ��Դ���ĵ�����ľ���(��ΪȨ��ʹ��)
	EResStatus		m_eStatus;		///< ��Դ����״̬
	ResTicket		m_kResTicket;	///< ��ԴID(������Դ������)
	bool			m_bReleaseOnGC;	///< �������ռ�ʱ�Ƿ��ͷ�
};
NiSmartPointer(sdResource);
//-------------------------------------------------------------------------------------------------
typedef std::list<sdResourcePtr> ResourceList;
typedef std::list<sdResourcePtr>::iterator ResourceListItr;
//-------------------------------------------------------------------------------------------------
#include "sdResource.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif