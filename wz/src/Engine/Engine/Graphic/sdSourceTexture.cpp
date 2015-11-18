#include "sdEnginePCH.h"
#include "sdSourceTexture.h"

#include "ITextureManager.h"

using namespace Base::Threading;

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
NiImplementRTTI(sdSourceTexture, NiSourceTexture);

sdSourceTexture::EMIPMAPTYPE sdSourceTexture::ms_eDefaultMipmapType = sdSourceTexture::E_MIPMAP_AUTO;
uchar sdSourceTexture::ms_ucMinSkipMipmapLvl = 0;
uchar sdSourceTexture::ms_ucMaxSkipMipmapLvl = 8;
uchar sdSourceTexture::ms_ucMinLoadPriority = 5;
uchar sdSourceTexture::ms_ucMaxLoadPriority = 0;
//-------------------------------------------------------------------------------------------------
sdSourceTexturePtr sdSourceTexture::Create(const NiFixedString& kFilename, sdSourceTexture::CreateParams& kParams)
{
	ITextureManager* pkTextureManager = ITextureManager::GetTextureManager();
	if (!pkTextureManager)
		return NULL;

	ITextureLodManager* pkTextureLodManager = ITextureLodManager::GetTextureLodManager();
	if (!pkTextureLodManager)
		return NULL;
	
	// 规整路径
	char acStandardFilename[NI_MAX_PATH];
	NiStrcpy(acStandardFilename, NI_MAX_PATH, kFilename);
	NiPath::Standardize(acStandardFilename);

	// 创建SourceTexture对象
	bool bCreated = false;
	sdSourceTexturePtr spSourceTexture = pkTextureManager->GetSourceTexture(acStandardFilename, kParams, bCreated);
	if (spSourceTexture == 0)
		return NULL;

	// 对于新创建的纹理进行初始化(同一个纹理只会有一个线程的bCreated返回true)
	if (bCreated)
	{
		spSourceTexture->m_kFilename = acStandardFilename;
		spSourceTexture->m_kPlatformFilename = acStandardFilename;
		spSourceTexture->m_kFormatPrefs = kParams.m_kPrefs;
		spSourceTexture->m_bLoadDirectToRendererHint = kParams.m_bLoadDirectToRendererHint;
		spSourceTexture->m_bSrcRendererDataIsPersistent = kParams.m_bSrcRendererDataIsPersistent;
		spSourceTexture->m_ucPriority = kParams.m_ucPriority;
		spSourceTexture->m_eMipmapType = kParams.m_eMipmapType;
		//spSourceTexture->m_ucSkipMipmapLvl = kParams.m_ucSkipMipmapLvl;
		//spSourceTexture->m_ucMipmapLvl = kParams.m_ucMipmapLvl;

		if (spSourceTexture->m_ucPriority < ms_ucMinLoadPriority)	
			spSourceTexture->m_ucPriority = ms_ucMinLoadPriority;

		if (spSourceTexture->m_ucPriority > ms_ucMaxLoadPriority)	
			spSourceTexture->m_ucPriority = ms_ucMaxLoadPriority;

		uchar ucSkipMipmapLevel = 0;
		uchar ucMipmapLvl = 0;
		EMipmapType eMipmapType = spSourceTexture->GetMipmapType();
		if (eMipmapType == E_MIPMAP_AUTO || eMipmapType == E_MIPMAP_DEFAULT)
		{
			ucSkipMipmapLevel = pkTextureLodManager->GetSkipLevel();
			ucMipmapLvl = 0;
		}
		else
		{
			ucSkipMipmapLevel = kParams.m_ucSkipMipmapLvl;
			ucMipmapLvl = kParams.m_ucMipmapLvl;
		}

		if (kParams.m_bAsyn)
		{
			spSourceTexture->AsyncReloadResource(ucSkipMipmapLevel, ucMipmapLvl);
		}
		else
		{
			spSourceTexture->SyncReloadResource(ucSkipMipmapLevel, ucMipmapLvl);
		}

		if (spSourceTexture->GetTexStatus() < E_TEX_PARAM_INITED)
			spSourceTexture->SetTexStatus(E_TEX_PARAM_INITED);
	}
	else
	{
		if (kParams.m_bAsyn)
		{
			while (spSourceTexture->GetTexStatus() < E_TEX_PARAM_INITED)
				continue;	///< 异步加载时只需要循环等待纹理初始化完成
		}
		else
		{
			while (spSourceTexture->GetTexStatus() < E_TEX_CREATED)
				continue;	///< 同步加载时需要循环等待纹理创建完成(有待改进)
		}
	}

	return spSourceTexture;
}
//-------------------------------------------------------------------------------------------------
sdSourceTexture::sdSourceTexture()
: m_eTexStatus(E_TEX_INVALID)
, m_ucPriority(0)
, m_eMipmapType(E_MIPMAP_DEFAULT)
, m_ucMipmapLvl(0)
, m_uiFlags(0)
{

}
//-------------------------------------------------------------------------------------------------
sdSourceTexture::~sdSourceTexture()
{

}
//-------------------------------------------------------------------------------------------------
bool sdSourceTexture::SyncReloadResource(uchar ucSkipMipmapLvl, uchar ucMipmapLvl)
{
	if (ucSkipMipmapLvl < ms_ucMinSkipMipmapLvl)
		ucSkipMipmapLvl = ms_ucMinSkipMipmapLvl;

	if (ucSkipMipmapLvl > ms_ucMaxSkipMipmapLvl)
		ucSkipMipmapLvl = ms_ucMaxSkipMipmapLvl;

	NIASSERT(!m_pkRendererData);	///< 暂时不支持Reload

	//if (CreateRendererData())
	//{
	//	SetTexStatus(E_TEX_CREATED);
	//}
	//else
	//{
	//
	//}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdSourceTexture::AsyncReloadResource(uchar ucSkipMipmapLvl, uchar /*ucMipmapLvl*/)
{
	// 尝试获取当前函数使用调用权限锁
	uint uiLock = TrySetAsyncReloadFuncLock();
	if (uiLock == 1)
		return false;

	// 检查参数与状态
	if (ucSkipMipmapLvl < ms_ucMinSkipMipmapLvl)
		ucSkipMipmapLvl = ms_ucMinSkipMipmapLvl;

	if (ucSkipMipmapLvl > ms_ucMaxSkipMipmapLvl)
		ucSkipMipmapLvl = ms_ucMaxSkipMipmapLvl;

	ITextureManager* pkTextureManager = ITextureManager::GetTextureManager();
	if (!pkTextureManager)
	{
		TryResetAsyncReloadFuncLock();	///< 释放当前函数使用调用权限锁
		return false;
	}

	// 纹理状态相同，不需要重新加载
	uchar ucCurSkipMipmapLvl = GetSkipMipmapLevel();
	uchar ucCurMipmapLvl = GetMipmapLvl();
	if ((ucSkipMipmapLvl == ucCurSkipMipmapLvl))
	{
		if (GetTexStatus() > E_TEX_PARAM_INITED)
		{
			TryResetAsyncReloadFuncLock();	///< 释放当前函数使用调用权限锁
			return true;
		}
	}

	//if (ucSkipMipmapLvl > ucCurSkipMipmapLvl)
	//{
	//	if (GetTexStatus() == E_TEX_CREATED && m_spDataStream)
	//	{
	//		//return pkTextureManager->DecreaseMipmap(this, ucSkipMipmapLvl);	///< 数据存在,则直接降级纹理
	//	}
	//}

	// 请求升级纹理
	bool bResult = false;
	if (ucSkipMipmapLvl < ucCurSkipMipmapLvl || GetTexStatus() == E_TEX_PARAM_INITED || GetTexStatus() == E_TEX_IGNORED)
	{
		bResult = pkTextureManager->LoadSourceTexture(this, ucSkipMipmapLvl, true);	
	}
	
	TryResetAsyncReloadFuncLock();	///< 释放当前函数使用调用权限锁
	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdSourceTexture::OnLoadingStatus(sdSourceTexture::ETexStatus eTexStatus)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
NiImplementCreateObject(sdSourceTexture);
//-------------------------------------------------------------------------------------------------
void sdSourceTexture::LoadBinary(NiStream& kStream)
{
	NiSourceTexture::LoadBinary(kStream);
}
//-------------------------------------------------------------------------------------------------
void sdSourceTexture::LinkObject(NiStream& kStream)
{
	NiSourceTexture::LinkObject(kStream);
}
//-------------------------------------------------------------------------------------------------
bool sdSourceTexture::RegisterStreamables(NiStream& kStream)
{
	if (!NiSourceTexture::RegisterStreamables(kStream))
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdSourceTexture::SaveBinary(NiStream& kStream)
{
	NiSourceTexture::SaveBinary(kStream);
}
//-------------------------------------------------------------------------------------------------
bool sdSourceTexture::IsEqual(NiObject* pkObject)
{
	if (!NiSourceTexture::IsEqual(pkObject))
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE