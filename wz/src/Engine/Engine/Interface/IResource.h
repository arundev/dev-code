//*************************************************************************************************
// 内容:	资源
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_INTERFACE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_INTERFACE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// EResType, 资源类型
//-------------------------------------------------------------------------------------------------
enum EResType
{
	E_RES_TYPE_NIF,
	E_RES_TYPE_KFM,
	NUM_RES,
};
//-------------------------------------------------------------------------------------------------
// EResReqPrior, 资源加载优先级
//-------------------------------------------------------------------------------------------------
enum EResReqPrior
{
	E_RES_PRIOR_ATONCE,
	E_RES_PRIOR_HIGHEST,
	E_RES_PRIOR_HIGH,
	E_RES_PRIOR_NORMAL,
	E_RES_PRIOR_LOW,
	E_RES_PRIOR_LOWEST,
};
//-------------------------------------------------------------------------------------------------
// EResStatus, 资源加载状态
//-------------------------------------------------------------------------------------------------
enum EResStatus
{
	E_RES_STATUS_WAITING,
	E_RES_STATUS_LOADING,
	E_RES_STATUS_LOADED,
	E_RES_STATUS_READY,
	E_RES_STATUS_IGNORED,
};
//-------------------------------------------------------------------------------------------------
// 资源使用类型
//-------------------------------------------------------------------------------------------------
enum EResUsageType
{
	E_RES_USAGE_NIF_COMMON,		///< NIF资源
	E_RES_USAGE_NIF_BUILDING,
	E_RES_USAGE_NIF_AVATARPART,
	E_RES_USAGE_NIF_KFMROOT,

	E_RES_USAGE_KFM_COMMON,		///< KFM资源
	E_RES_USAGE_KFM_SELF,
	E_RES_USAGE_KFM_PLAYER,
	E_RES_USAGE_KFM_NPC,
};
//-------------------------------------------------------------------------------------------------
// IResource
//-------------------------------------------------------------------------------------------------
class IResource : public NiRefObject
{
public:
	typedef uint ResTicket;									///< 资源ID
	static const ResTicket INVALID_RES_TICKET = 0xffffffff;	///< 非法资源ID

public:
	virtual ~IResource() {};

	// 请求加载资源
	virtual bool LoadResource(bool bMainThreadLoad = false) = 0;

	// 取消加载资源
	virtual void IgnoreResource() = 0;

	// 请求缓存资源(只能被主线程调用)
	virtual void CacheResource() = 0;

	// 卸载资源
	virtual void FreeResource(bool bNotify = true) = 0;

	// 资源是否被加载
	virtual bool Loaded() = 0;

	// 资源加载是否被取消
	virtual bool Ignored() = 0;

	// 资源是否被缓存
	virtual bool Cached() = 0;

	// 资源是否被释放
	virtual bool Freed() = 0;

	// 更新资源优先级
	virtual void UpdatePrior(EResReqPrior ePrior, float fSqlDistance) = 0;

	// 句柄属性访问
	virtual ResTicket GetTicket() const = 0;

	// NIF根节点属性访问(用于NIF资源)
	virtual NiNode* GetRootNode() = 0;

	// KFM管理器属性访问(用于KFM资源)
	virtual NiActorManager* GetActorManager() = 0;

	// 资源统计
	//virtual bool GetMemoryUsage(ISizer* pkSizer) = 0;
};
NiSmartPointer(IResource);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif