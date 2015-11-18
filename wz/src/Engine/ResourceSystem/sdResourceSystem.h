//*************************************************************************************************
// ����:		��Դϵͳ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:
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
// ��Դϵͳ,������Դ����,����,����
//
// \TODO ����sdResourceEvent����
//-------------------------------------------------------------------------------------------------
class sdResourceSystem : public IResourceSystem, public NiThreadProcedure
{
public:
	sdResourceSystem();
	virtual ~sdResourceSystem();

	// �麯���̳�(��IResourceSystem)
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


	// ���Է���
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
	// �麯���̳�(��NiThreadProcedure), ִ���첽��Դ��������
	virtual uint ThreadProcedure(NiThread* spThread);

	// �첽����NIF/KFM��Դ
	IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResource::ResTicket& kResTicket, sdResourceEvent* pkResEvent);

	// ע��/ȡ����Դ�����¼�����
	void RegisterEvent(sdResourceEvent* pkEvent);
	void UnregisterEvent(const void* pkObject);
	void ClearAllEvents();

	// �¼�����
	void RaiseEvent(sdResource* pkResource);

	// ������̼߳�������
	void RequestLoad(sdResource* pkResource, EResReqPrior ePrior = E_RES_PRIOR_NORMAL);

	// ������߳�ж������
	void RequestFree(sdResource* pkResource);

protected:
	// ��Դ��Ϣ
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

	// ��Դ�����б�������
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
	// ��Դ����������
	NiStream* m_spSyncStream;	///< ͬ������������,�������̼߳���
	NiStream* m_spAsyncStream;	///< �첽����������,���ڶ��̼߳���

	// ��Դ��
	sdNifResourcePoolPtr m_pkNifResourcePool;	///< NIF��Դ��
	sdKfResourcePoolPtr	 m_pkKfResourcePool;	///< KF��Դ��
	sdKfmResourcePoolPtr m_pkKfmResourcePool;	///< KFM��Դ��

	// ��Դ��
	typedef std::vector<sdResourceGroupPtr> ResourceGroupVec;
	typedef std::vector<sdResourceGroupPtr>::iterator ResourceGroupVecItr;
	ResourceGroupVec m_kResourceGroupVec;	///< ��Դ�����
	uint m_uiResourceGroupCount;			///< ��Դ������

	ACTORMANAGERCREATEFUNC m_pfnActorManagerCreateFunc;	///< KFM�ļ���������ָ��

	// ��Դ�߳�
	bool m_bActive;						///< �߳��Ƿ񼤻�
	bool m_bDisposing;
	NiThread* m_spProcessThread;		///< ��Դ�����߳�
	sdRequestInfoList m_kRequestList;	///< ��Դ�����б�(���߳�ʹ��)
	sdRequestInfoList m_kProcessList;	///< ��Դ�����б�,�������ڼ��ص���Դ(���̺߳���Դ�߳�ʹ��)
	sdRequestInfoList m_kFinishedList;	///< ��Դ��������б�,���������ɵ���Դ(���̺߳���Դ�߳�ʹ��)
	sdRequestInfoList m_kCachedList;	///< ��Դ�����б�,�����Ѿ�������ɵ��ǻ�û�ص�֪ͨ����Դ(���߳�ʹ��)
	Base::Threading::sdCriticalSection m_kProcessListLocker;	///< ��Դ�����б���
	Base::Threading::sdCriticalSection m_kFinishedListLocker;	///< ��Դ��������б���

	// ��Դ�����¼�
	NiTPointerMap<sdResource::ResTicket, sdResourceEventList*> m_kTicketEventMap;
	NiTPointerMap<const void*, sdResourceEventList*> m_kObjectEventMap;

	// ��Դ��ʱ
	Base::Diagnostics::sdProfileOutput m_kProfileOutput;
	double m_dMaxProcessTime;	///< ÿ��Tick�����ʱ��
	uint m_uiMaxRasingCount;	///< ÿ��Tick���������,���ڱ���ÿ�δ������֪ͨ����
};
#include "sdResourceSystem.inl"
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif