//*************************************************************************************************
// 内容:		资源系统
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEME_RESOURCE_SYSTEM_H__
#define _SD_ENGINE_RESOURCE_SYSTEME_RESOURCE_SYSTEM_H__
#include <IResourceSystem.h>

#include "sdNifResourcePool.h"
#include "sdKfResourcePool.h"
#include "sdKfmResourcePool.h"
#include "sdResource.h"
#include "sdResourceEvent.h"
#include "sdResourceGroup.h"

#include <sdProfileOutput.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// 资源系统,负责资源加载,管理,销毁
//
// \TODO 考虑sdResourceEvent复用
//-------------------------------------------------------------------------------------------------
class sdResourceSystem : public IResourceSystem, public NiThreadProcedure
{
public:
	sdResourceSystem();
	virtual ~sdResourceSystem();

	// 虚函数继承(自IResourceSystem)
	virtual bool Initialize();
	virtual void Destroy();
	virtual void Update();

	virtual NiSourceTexturePtr AsyncLoadTexture(const std::string& szFileName);

	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, IResourceEventListener* pkObject);
	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, IResourceEventListener* pkObject);

	virtual void AsyncFreeResource(IResource::ResTicket kResTicket);

	//inline IResourcePtr LoadResource(const sdResourceParam& kResParam, void (* pfnCallback)(IResource*));
	//inline IResourcePtr LoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, void (* pfnCallback)(IResource*));

	//template <typename T> IResourcePtr LoadResource(const sdResourceParam& kResParam, T* pkObject, void (* pfnCallback)(IResource*));
	//template <typename T> IResourcePtr LoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, T* pkObject, void (* pfnCallback)(IResource*));

	virtual NiNodePtr			SyncLoadNif(const char* szFileName);
	NiSourceTexturePtr			SyncLoadTexture(const char* szFileName, bool bMipmap = true);

	virtual NiSourceTexturePtr	LoadTexture(const std::string& szFileName, bool bMipmap = true);
	virtual NiTexturePtr		LoadTextureByD3D(const std::string& szFileName);
	virtual NiSourceCubeMapPtr	LoadCubeTexture(const std::string& szFileName);
	virtual NiPixelDataPtr		LoadTextureToMemory(const std::string& szFileName);

	virtual void GarbageCollect();

	virtual bool Convert2ResPath(const char* szPath, char* szNewPath);


	// 属性访问
	inline NiStream* GetSyncStream();
	inline NiStream* GetAsyncStream();

	inline sdNifResourcePool* GetNifResourcePool();
	inline sdKfResourcePool* GetKfResourcePool();
	inline sdKfmResourcePool* GetKfmResourcePool();

	typedef NiActorManager* (*ACTORMANAGERCREATEFUNC)(const char* pcKFMFileName, 
		bool bCumulativeAnimation /* = false */,
		bool bLoadFileFromDisk /* = true */,
		NiStream* spStream /* = Stream */,
		NiPoseBinding* spPoseBinding /* = NULL */);
	inline ACTORMANAGERCREATEFUNC GetActorManagerCreateFunc();

protected:
	// 虚函数继承(自NiThreadProcedure), 执行异步资源加载任务
	virtual uint ThreadProcedure(NiThread* spThread);

	// 异步加载NIF/KFM资源
	IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResource::ResTicket& kResTicket, sdResourceEvent* pkResEvent);

	// 注册/取消资源加载事件队列
	void RegisterEvent(sdResourceEvent* pkEvent);
	void UnregisterEvent(const void* pkObject);
	void ClearAllEvents();

	// 事件触发
	void RaiseEvent(sdResource* pkResource);

	// 发起多线程加载请求
	void RequestLoad(sdResource* pkResource, EResReqPrior ePrior = E_RES_PRIOR_NORMAL);

	// 发起多线程卸载请求
	void RequestFree(sdResource* pkResource);

protected:
	// 资源信息
	class sdRequestInfo : public NiMemObject
	{
	public:
		enum EReqType
		{
			E_REQ_LOAD,
			E_REQ_FREE,
		};

	public:
		sdResourcePtr m_spResource;
		EReqType m_eRequest;

	public:
		sdRequestInfo() : m_eRequest(E_REQ_LOAD){}
	};
	typedef std::list<sdRequestInfo> sdRequestInfoList;
	typedef std::list<sdRequestInfo>::iterator sdRequestInfoListItr;

	// 资源请求列表排序器
	struct stRequestProcessListOrderFunctor
	{
		bool operator()(const sdRequestInfo& lhs, const sdRequestInfo& rhs)
		{	
			EResReqPrior eLPrior = lhs.m_spResource->GetRequestPrior();
			EResReqPrior eRPrior = rhs.m_spResource->GetRequestPrior();
			return eLPrior < eRPrior;
		}
	};

protected:
	// 资源加载数据流
	NiStream* m_spSyncStream;	///< 同步加载数据流,用于主线程加载
	NiStream* m_spAsyncStream;	///< 异步加载数据流,用于多线程加载

	// 资源池
	sdNifResourcePoolPtr m_pkNifResourcePool;	///< NIF资源池
	sdKfResourcePoolPtr	 m_pkKfResourcePool;	///< KF资源池
	sdKfmResourcePoolPtr m_pkKfmResourcePool;	///< KFM资源池

	// 资源组
	typedef std::vector<sdResourceGroupPtr> ResourceGroupVec;
	typedef std::vector<sdResourceGroupPtr>::iterator ResourceGroupVecItr;
	ResourceGroupVec m_kResourceGroupVec;	///< 资源组队列
	uint m_uiResourceGroupCount;			///< 资源组数量

	ACTORMANAGERCREATEFUNC m_pfnActorManagerCreateFunc;	///< KFM文件创建函数指针

	// 资源线程
	bool m_bActive;						///< 线程是否激活
	bool m_bDisposing;
	NiThread* m_spProcessThread;		///< 资源加载线程
	sdRequestInfoList m_kRequestList;	///< 资源请求列表(主线程使用)
	sdRequestInfoList m_kProcessList;	///< 资源处理列表,保存正在加载的资源(主线程和资源线程使用)
	sdRequestInfoList m_kFinishedList;	///< 资源处理完成列表,保存加载完成的资源(主线程和资源线程使用)
	sdRequestInfoList m_kCachedList;	///< 资源缓冲列表,保存已经加载完成但是还没回调通知的资源(主线程使用)
	Base::Threading::sdCriticalSection m_kProcessListLocker;	///< 资源处理列表锁
	Base::Threading::sdCriticalSection m_kFinishedListLocker;	///< 资源处理完成列表锁

	// 资源加载事件
	NiTPointerMap<sdResource::ResTicket, sdResourceEventList*> m_kTicketEventMap;
	NiTPointerMap<const void*, sdResourceEventList*> m_kObjectEventMap;

	// 资源计时
	Base::Diagnostics::sdProfileOutput m_kProfileOutput;
	double m_dMaxProcessTime;	///< 每个Tick最大处理时间
	uint m_uiMaxRasingCount;	///< 每个Tick最大处理数量,用于避免每次处理过多通知请求
};
#include "sdResourceSystem.inl"
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif