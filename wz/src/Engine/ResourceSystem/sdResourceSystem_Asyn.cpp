#include "sdResourceSystemPCH.h"
#include "sdResourceSystem.h"
#include "sdNifResource.h"
#include "sdKfmResource.h"

#include <NiFilename.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
NiSourceTexturePtr sdResourceSystem::AsyncLoadTexture(const std::string& szFileName)
{
	// 检查文件名
	if (szFileName.empty())
		return 0;

	// 检查路径合法性


	// 加载纹理
	NiSourceTexture::FormatPrefs kFormatPrefs;
	//NiSourceTexturePtr spTexture = NiSourceTexture::Create(kFileName, kFormatPrefs);
	//NIASSERT(spTexture);

	return 0;
}
//-------------------------------------------------------------------------------------------------
IResourcePtr sdResourceSystem::AsyncLoadResource(const sdResourceParam& kResParam, IResourceEventListener* pkObject)
{
	sdResourceEventPtr pkResourceEvent = NiNew sdResourceEvent(pkObject);
	NIASSERT(pkResourceEvent);

	sdResource::ResTicket kResTicket = 0;

	return AsyncLoadResource(kResParam, kResTicket, pkResourceEvent);
}
//-------------------------------------------------------------------------------------------------
IResourcePtr sdResourceSystem::AsyncLoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, IResourceEventListener* pkObject)
{
	sdResourceEventPtr pkResourceEvent = NiNew sdResourceEvent(pkObject);
	NIASSERT(pkResourceEvent);

	return AsyncLoadResource(kResParam, kResTicket, pkResourceEvent);
}
//-------------------------------------------------------------------------------------------------
IResourcePtr sdResourceSystem::AsyncLoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, sdResourceEvent* pkResEvent)
{
	// 检查文件名
	const std::string& kFileName = kResParam.GetFileName();
	if (kFileName.empty())
		return 0;

	// 检查扩展名
	NiFilename kFilename(kFileName.c_str());
	NiString kFileExt = kFilename.GetExt();
	kFileExt.ToUpper();

	if (kResParam.GetResType() == E_RES_TYPE_NIF && !kFileExt.Equals(".NIF"))
		return 0;

	if (kResParam.GetResType() == E_RES_TYPE_KFM && !kFileExt.Equals(".KFM"))
		return 0;

	// 相对路径扩展为绝对路径
	if (!kResParam.IsAbsPath())
	{
		// TODO
	}

	// 查找对应资源组
	sdResourceGroup* pkResourceGroup = m_kResourceGroupVec[kResParam.GetResType()];
	if (!pkResourceGroup)
		return 0;

	// 创建资源
	sdResourcePtr pkResource = 0;
	if (kResParam.GetResType() == E_RES_TYPE_NIF)
		pkResource = NiNew sdNifResource(kFileName, kResParam.IsReleaseOnGC(), kResParam.GetUsageType());
	else if (kResParam.GetResType() == E_RES_TYPE_KFM)
		pkResource = NiNew sdKfmResource(kFileName, kResParam.IsReleaseOnGC(), kResParam.GetUsageType(), kResParam.GetChangedFileName(), kResParam.GetModelRootName());
	
	if (!pkResource)
		return 0;

	// 加入对应资源组
	kResTicket = pkResourceGroup->AddResource(pkResource, false);
	if (kResTicket == sdResource::INVALID_RES_TICKET)
	{
		pkResource = 0;
		return 0;
	}

	// 事件回调函数指针加入队列
	if (pkResEvent)
	{
		pkResEvent->SetTicket(kResTicket);
		RegisterEvent(pkResEvent);
	}

	// 请求加载
	RequestLoad(pkResource, kResParam.GetResPrior());

	return (sdResource*)pkResource;
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::AsyncFreeResource(IResource::ResTicket kResTicket)
{
	if (kResTicket == IResource::INVALID_RES_TICKET)
		return;

	uint uiResGroupType = kResTicket >> 28;
	if (uiResGroupType >= m_uiResourceGroupCount)
		return;

	sdResourceGroup* pkResourceGroup = m_kResourceGroupVec[uiResGroupType];
	if (!pkResourceGroup)
		return;

	RequestFree(pkResourceGroup->RemoveResource(kResTicket));
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::RequestLoad(sdResource* pkResource, EResReqPrior ePrior /* = E_RES_PRIOR_NORMAL */)
{
	if (!pkResource)	
		return;

	sdRequestInfo kResReqInfo;
	kResReqInfo.m_spResource = pkResource;
	kResReqInfo.m_eRequest = sdRequestInfo::E_REQ_LOAD;

	kResReqInfo.m_spResource->UpdatePrior(ePrior, 0.f);

	m_kRequestList.push_back(kResReqInfo);
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::RequestFree(sdResource* pkResource)
{
	if (!pkResource)	
		return;

	pkResource->IgnoreResource();

	sdRequestInfo kResReqInfo;
	kResReqInfo.m_spResource = pkResource;
	kResReqInfo.m_eRequest = sdRequestInfo::E_REQ_FREE;

	kResReqInfo.m_spResource->UpdatePrior(E_RES_PRIOR_ATONCE, 0.f);

	m_kRequestList.push_back(kResReqInfo);
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE