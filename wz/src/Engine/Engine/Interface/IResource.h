//*************************************************************************************************
// ����:	��Դ
//---------------------------------------------------------
// ����:		
// ����:		2013-01-31
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_INTERFACE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_INTERFACE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// EResType, ��Դ����
//-------------------------------------------------------------------------------------------------
enum EResType
{
	E_RES_TYPE_NIF,
	E_RES_TYPE_KFM,
	NUM_RES,
};
//-------------------------------------------------------------------------------------------------
// EResReqPrior, ��Դ�������ȼ�
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
// EResStatus, ��Դ����״̬
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
// ��Դʹ������
//-------------------------------------------------------------------------------------------------
enum EResUsageType
{
	E_RES_USAGE_NIF_COMMON,		///< NIF��Դ
	E_RES_USAGE_NIF_BUILDING,
	E_RES_USAGE_NIF_AVATARPART,
	E_RES_USAGE_NIF_KFMROOT,

	E_RES_USAGE_KFM_COMMON,		///< KFM��Դ
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
	typedef uint ResTicket;									///< ��ԴID
	static const ResTicket INVALID_RES_TICKET = 0xffffffff;	///< �Ƿ���ԴID

public:
	virtual ~IResource() {};

	// ���������Դ
	virtual bool LoadResource(bool bMainThreadLoad = false) = 0;

	// ȡ��������Դ
	virtual void IgnoreResource() = 0;

	// ���󻺴���Դ(ֻ�ܱ����̵߳���)
	virtual void CacheResource() = 0;

	// ж����Դ
	virtual void FreeResource(bool bNotify = true) = 0;

	// ��Դ�Ƿ񱻼���
	virtual bool Loaded() = 0;

	// ��Դ�����Ƿ�ȡ��
	virtual bool Ignored() = 0;

	// ��Դ�Ƿ񱻻���
	virtual bool Cached() = 0;

	// ��Դ�Ƿ��ͷ�
	virtual bool Freed() = 0;

	// ������Դ���ȼ�
	virtual void UpdatePrior(EResReqPrior ePrior, float fSqlDistance) = 0;

	// ������Է���
	virtual ResTicket GetTicket() const = 0;

	// NIF���ڵ����Է���(����NIF��Դ)
	virtual NiNode* GetRootNode() = 0;

	// KFM���������Է���(����KFM��Դ)
	virtual NiActorManager* GetActorManager() = 0;

	// ��Դͳ��
	//virtual bool GetMemoryUsage(ISizer* pkSizer) = 0;
};
NiSmartPointer(IResource);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif