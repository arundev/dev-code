//*************************************************************************************************
// 内容:		资源
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
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

	// 虚函数继承
	inline virtual void IgnoreResource();
	inline virtual bool Loaded();
	inline virtual bool Ignored();
	inline virtual bool Cached();
	inline virtual bool Freed();

	inline virtual void UpdatePrior(EResReqPrior ePrior, float fSqlDistance);

	inline virtual ResTicket GetTicket() const;
	inline virtual NiNode* GetRootNode();
	inline virtual NiActorManager* GetActorManager();

	// 属性访问
	inline const std::string& GetFileName() const;
	inline EResUsageType GetUsageType() const;
	inline EResReqPrior GetRequestPrior() const;

protected:
	inline void SetTicket(ResTicket kTicket);

protected:
	std::string		m_kFileName;	///< 资源文件名
	EResUsageType	m_eUsage;		///< 资源使用类型
	EResReqPrior	m_ePrior;		///< 资源加载优先级	
	float			m_fSqlDistance;	///< 资源中心到相机的距离(作为权重使用)
	EResStatus		m_eStatus;		///< 资源加载状态
	ResTicket		m_kResTicket;	///< 资源ID(用于资源管理器)
	bool			m_bReleaseOnGC;	///< 在垃圾收集时是否被释放
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