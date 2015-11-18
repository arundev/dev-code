#include "sdEnginePCH.h"
#include "sdTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdTexture* sdTexture::ms_pkHead = NULL;
sdTexture* sdTexture::ms_pkTail = NULL;
uint sdTexture::ms_uiLostNotifyIndex = 0;
uint sdTexture::ms_uiResetNotifyIndex = 0;
//-------------------------------------------------------------------------------------------------
void sdTexture::StaticInitialize()
{
	NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spRenderer);
	ms_uiLostNotifyIndex = spRenderer->AddLostDeviceNotificationFunc(sdTexture::DeviceLostCallBack, NULL);
	ms_uiResetNotifyIndex = spRenderer->AddResetNotificationFunc(sdTexture::DeviceResetCallBack, NULL);

	ms_pkHead = NULL;
	ms_pkTail = NULL;
}
//-------------------------------------------------------------------------------------------------
void sdTexture::StaticDestroy()
{
	NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spRenderer);
	spRenderer->RemoveLostDeviceNotificationFunc(ms_uiLostNotifyIndex);
	spRenderer->RemoveResetNotificationFunc(ms_uiResetNotifyIndex);

	ms_pkHead = NULL;
	ms_pkTail = NULL;
}
//-------------------------------------------------------------------------------------------------
bool sdTexture::DeviceLostCallBack(void*)
{
	// 为啥?
	::Sleep(50);	

	// 遍历列表,回调
	if (ms_pkHead)
	{
		sdTexture* pkTexture = ms_pkHead;
		do
		{
			pkTexture->OnDeviceLost();

			pkTexture = pkTexture->m_pkNext;
			if (pkTexture == ms_pkHead)
				break;
		}while(1);
	};

	return  true;
}
//-------------------------------------------------------------------------------------------------
bool sdTexture::DeviceResetCallBack(bool bBefore, void* pvData)
{
	// 遍历列表,回调
	if (ms_pkHead)
	{
		sdTexture* pkTexture = ms_pkHead;
		do
		{
			pkTexture->OnDeviceReset(bBefore);

			pkTexture = pkTexture->m_pkNext;
			if (pkTexture == ms_pkHead)
				break;
		}while(1);
	};

	return  true;
}
//-------------------------------------------------------------------------------------------------
sdTexture::sdTexture()
: m_pkPrev(NULL)
, m_pkNext(NULL)
{
	// 添加到列表
	AddTextureToList();
}

sdTexture::~sdTexture()
{
	// 从列表移除
	RemoveTextureFromList();
}
//-------------------------------------------------------------------------------------------------
void sdTexture::AddTextureToList()
{
	if (!ms_pkHead)
		ms_pkHead = this;

	if (ms_pkTail)
	{
		// 添加到链表结尾
		ms_pkTail->m_pkNext = this;
		m_pkPrev = ms_pkTail;
	}
	else
	{
		m_pkPrev = NULL;
	}

	ms_pkTail = this;
	m_pkNext = NULL;
}
//-------------------------------------------------------------------------------------------------
void sdTexture::RemoveTextureFromList()
{
	if (ms_pkHead == this)	ms_pkHead = m_pkNext;
	if (ms_pkTail == this)	ms_pkTail = m_pkPrev;
	if (m_pkPrev)	m_pkPrev->m_pkNext = m_pkNext;
	if (m_pkNext)	m_pkNext->m_pkPrev = m_pkPrev;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE