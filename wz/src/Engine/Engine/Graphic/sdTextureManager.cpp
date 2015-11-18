#include "sdEnginePCH.h"
#include "sdTextureManager.h"
#include "sdPixelLoadTaskQueue.h"
#include "sdSourceTexture.h"

using namespace Base::Threading;

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdTextureManager::sdTextureManager()
: m_bInitialized(false)
, m_bForceSynLoad(false)
, m_ucMinLoadPriority(0)
, m_ucMaxLoadPriority(MAX_TEXTURE_PRIORITY)
, m_pkPixelLoadTaskQueue(NULL)
{
}
//-------------------------------------------------------------------------------------------------
sdTextureManager::~sdTextureManager()
{

}
//-------------------------------------------------------------------------------------------------
bool sdTextureManager::Initialize()
{


	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTextureManager::Destroy()
{
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdTextureManager::Update()
{
	
}
//-------------------------------------------------------------------------------------------------
bool sdTextureManager::IsInitialized()
{
	return m_bInitialized;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureManager::LoadSourceTexture(sdSourceTexturePtr spTexture, uchar ucSkipMipmapLvl, bool bAsyn)
{
	NIASSERT(spTexture);

	// 检查优先级
	uchar ucPriority = spTexture->GetPriority();
	if (ucPriority < m_ucMinLoadPriority || ucPriority > m_ucMaxLoadPriority)
	{
		spTexture->OnLoadingStatus(sdSourceTexture::E_TEX_IGNORED);
		return false;
	}

	// 提交加载
	bool bResult = false;
	if (bAsyn && !m_bForceSynLoad)
	{
		const NiFixedString& kFileName = spTexture->GetPlatformSpecificFilename();
		if (kFileName.Exists())
			bResult = m_pkPixelLoadTaskQueue->AddLoadTask(spTexture, ucSkipMipmapLvl);
	}
	else
	{
		const NiFixedString& kFileName = spTexture->GetPlatformSpecificFilename();
		if (kFileName.Exists())
		{
			///////////////
		}
	}
	
	return bResult;
}
//-------------------------------------------------------------------------------------------------
sdSourceTexturePtr sdTextureManager::GetSourceTexture(const char* pcFileName, const sdSourceTexture::CreateParams& kCreateParams, bool& bCreate)
{
	NIASSERT(pcFileName);

	sdSourceTexturePtr spSourceTexture;
	
	{
		SDTLock<sdCriticalSection> kLockImpl(m_kLock);

		SourceTextureDictItr kIter = m_kSourceTextureDict.find(pcFileName);
		SourceTextureDictItr kIterEnd = m_kSourceTextureDict.end();
		if (kIter != kIterEnd)
		{
			spSourceTexture = kIter->second;
			NIASSERT(spSourceTexture);
 
			NIASSERT(kCreateParams.m_eMipmapType == spSourceTexture->GetMipmapType());
			NIASSERT(kCreateParams.m_ucSkipMipmapLvl == spSourceTexture->GetSkipMipmapLevel());
			NIASSERT(kCreateParams.m_ucMipmapLvl == spSourceTexture->GetMipmapLvl());
		}
		else
		{
			spSourceTexture = (sdSourceTexture*)sdSourceTexture::CreateObject();
			NIASSERT(spSourceTexture);

			bCreate = true;	///< 设置创建标记

			m_kSourceTextureDict[pcFileName] = spSourceTexture;	///< 加入字典
		}
	}
	
	return spSourceTexture;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureManager::RegisterSourceTexture(sdSourceTexturePtr spSourceTexture)
{
	NIASSERT(spSourceTexture);

	bool bResult = false;
	const NiFixedString& kFileName = spSourceTexture->GetPlatformSpecificFilename();

	{
		SDTLock<sdCriticalSection> kLockImpl(m_kLock);

		SourceTextureDictItr kIter = m_kSourceTextureDict.find(kFileName);
		SourceTextureDictItr kIterEnd = m_kSourceTextureDict.end();
		if (kIter == kIterEnd)
		{
			m_kSourceTextureDict[kFileName] = spSourceTexture;	///< 加入字典
			bResult = true;
		}
	}

	return bResult;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureManager::UnregisterSourceTexture(sdSourceTexturePtr spSourceTexture)
{
	NIASSERT(spSourceTexture);

	bool bResult = false;
	const NiFixedString& kFileName = spSourceTexture->GetPlatformSpecificFilename();

	{
		SDTLock<sdCriticalSection> kLockImpl(m_kLock);

		SourceTextureDictItr kIter = m_kSourceTextureDict.find(kFileName);
		SourceTextureDictItr kIterEnd = m_kSourceTextureDict.end();
		if (kIter != kIterEnd)
		{
			if (kIter->second == spSourceTexture)
			{
				m_kSourceTextureDict.erase(kIter);	///< 从字典移除
				bResult = true;
			}
		}
	}

	return bResult;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE