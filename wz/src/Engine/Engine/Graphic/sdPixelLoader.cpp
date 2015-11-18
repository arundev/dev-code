#include "sdEnginePCH.h"
#include "sdPixelLoader.h"
#include "sdPixelLoadTaskQueue.h"

#include "IDataStream.h"
#include "sdSourceTexture.h"

#define MAX_PER_LOAD_TASK	3
#define MAX_TRY_LOAD_COUNT	5

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdPixelLoader::sdPixelLoader(sdPixelLoadTaskQueue* pkTaskQueue, sdTextureCache* pkTextureCache)
{
	m_pkPixelLoadThread = NiNew sdPixelLoadThread(pkTaskQueue, pkTextureCache);
	NIASSERT(m_pkPixelLoadThread);

	m_pkPixelLoadThread->Initialize();
}
//-------------------------------------------------------------------------------------------------
sdPixelLoader::~sdPixelLoader()
{
	if (m_pkPixelLoadThread)
	{
		m_pkPixelLoadThread->Destroy();

		NiDelete m_pkPixelLoadThread;
		m_pkPixelLoadThread = 0;
	}
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::Start()
{
	m_pkPixelLoadThread->Start();
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::Stop()
{
	m_pkPixelLoadThread->Stop();
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::LoadWake()
{
	m_pkPixelLoadThread->LoadWake();
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::LoadSleep()
{
	m_pkPixelLoadThread->LoadSleep();
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::DecreaseWake()
{
	m_pkPixelLoadThread->DecreaseWake();
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoader::DecreaseSleep()
{
	m_pkPixelLoadThread->DecreaseSleep();
}
//-------------------------------------------------------------------------------------------------
sdPixelLoadThread::sdPixelLoadThread(sdPixelLoadTaskQueue* pkTaskQueue, sdTextureCache* pkTextureCache)
: m_bInitialized(false)
, m_bExit(false)
, m_hLoadThread(INVALID_HANDLE_VALUE)
, m_hLoadEvent(INVALID_HANDLE_VALUE)
, m_hDecreaseThread(INVALID_HANDLE_VALUE)
, m_hDecreaseEvent(INVALID_HANDLE_VALUE)
{
	NIASSERT(pkTaskQueue);
	m_pkTaskQueue = pkTaskQueue;

	NIASSERT(pkTextureCache);
	m_pkTextureCache = pkTextureCache;
}
//-------------------------------------------------------------------------------------------------
sdPixelLoadThread::~sdPixelLoadThread()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdPixelLoadThread::Initialize()
{
	if (m_bInitialized)
		return true;

	m_hLoadEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hLoadThread = ::CreateThread(NULL, 0, &LoadProc, (void*)this, CREATE_SUSPENDED, NULL);

	m_hDecreaseEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hDecreaseThread = ::CreateThread(NULL, 0, &DecreaseProc, (void*)this, CREATE_SUSPENDED, NULL);

	::SetThreadPriority(m_hLoadThread, THREAD_PRIORITY_ABOVE_NORMAL);
	::SetThreadPriority(m_hDecreaseThread, THREAD_PRIORITY_BELOW_NORMAL);

	// 参考了UE3的做法，不通过SetThreadAffinityMask,
	// 而是通过SetThreadIdealProcessor来常数设置加载线程的理想CPU
	::SetThreadIdealProcessor(m_hLoadThread, 1);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::Destroy()
{
	if (!m_bInitialized)
		return;

	m_bExit = true;

	if (m_hLoadThread != INVALID_HANDLE_VALUE)
	{
		::SetEvent(m_hLoadEvent);
		::WaitForSingleObject(m_hLoadThread, INFINITE);

		::CloseHandle(m_hLoadThread);
		::CloseHandle(m_hLoadEvent);

		m_hLoadThread = INVALID_HANDLE_VALUE;
		m_hLoadEvent = INVALID_HANDLE_VALUE;
	}

	if (m_hDecreaseThread != INVALID_HANDLE_VALUE)
	{
		::SetEvent(m_hDecreaseEvent);
		::WaitForSingleObject(m_hDecreaseThread, INFINITE);

		::CloseHandle(m_hDecreaseThread);
		::CloseHandle(m_hDecreaseEvent);

		m_hDecreaseThread = INVALID_HANDLE_VALUE;
		m_hDecreaseEvent = INVALID_HANDLE_VALUE;
	}

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::Start()
{
	::ResumeThread(m_hLoadThread);
	::ResumeThread(m_hDecreaseThread);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::Stop()
{
	::SuspendThread(m_hLoadThread);
	::SuspendThread(m_hDecreaseThread);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::LoadWake()
{
	::SetEvent(m_hLoadEvent);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::LoadSleep()
{
	::ResetEvent(m_hLoadEvent);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::DecreaseWake()
{
	::SetEvent(m_hDecreaseEvent);
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::DecreaseSleep()
{
	::ResetEvent(m_hDecreaseEvent);
}
//-------------------------------------------------------------------------------------------------
bool sdPixelLoadThread::CreateD3DTexture(sdSourceTexture* pkSourceTexture, FileSystem::IDataStream* pkDataStream)
{
	NIASSERT(pkSourceTexture);

	NiPixelData* pkPixelData = pkSourceTexture->GetSourcePixelData();
	NiPersistentSrcTextureRendererData* pkPersisData = pkSourceTexture->GetPersistentSourceRendererData();
	
	bool bResult = false;
	if (pkPersisData)
	{
		//bResult = pkSourceTexture->CopyFr
	}
	else if (pkPixelData)
	{
		//
	}
	else if (pkDataStream)
	{
		//NiDX9SourceTextureData* spSourceTextureData = (NiDX9SourceTextureData*)pkSourceTexture->GetRendererData();
		//if (!spSourceTextureData)
		//	return false;
		//
		//IDirect3DTexture9* d3dTexture = (IDirect3DTexture9*)pkSourceTexture->GetD3DTexture();
		//if (!spSourceTextureData->GetD3DTexture())
		//	return false;
		//
		//D3DFORMAT d3dFmt;
		//uint uiPreSize = SDCalcD3DTextureSize(d3dTexture);

		// 从内存数据获取纹理数据信息
		D3DXIMAGE_INFO d3dxImageInfo;
		HRESULT hr = D3DXGetImageInfoFromFileInMemory(
			(VOID*)pkDataStream->GetBuff(), 
			pkDataStream->Size(),
			&d3dxImageInfo);

		if (FAILED(hr))
		{
			pkSourceTexture->SetTexStatus(sdSourceTexture::E_TEX_ERROR);
			bResult = false;
		}

		// 从内存数据创建设备纹理
		

	}
	else
	{
		pkSourceTexture->SetTexStatus(sdSourceTexture::E_TEX_ERROR);
	}

	return bResult;
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::Load()
{
	// 从PixelLoadTaskQueue获取加载任务，加载任务应该根据频率等因素动态调整，但此处暂定为3个
	// 单线程IO必须保证任务获取顺序和完成顺序严格一致
	sdPixelLoadTaskQueue::PixelLoadTask akTaskBuf[MAX_PER_LOAD_TASK];
	while (!m_bExit)
	{
		::ZeroMemory(akTaskBuf, sizeof(sdPixelLoadTaskQueue::PixelLoadTask) * MAX_PER_LOAD_TASK);
		uint uiTaskNum = m_pkTaskQueue->GetRecentLoadedTask(akTaskBuf, MAX_PER_LOAD_TASK);

		if (uiTaskNum == 0)
		{
			::WaitForSingleObject(m_hLoadEvent, INFINITE);
			continue;
		}

		for (uint ui = 0; ui < uiTaskNum; ++ui)
		{
			sdPixelLoadTaskQueue::PixelLoadTask& kTask = akTaskBuf[ui];

			const NiFixedString& kFileName = kTask.m_pkTexture->GetPlatformSpecificFilename();
			if (kFileName != NULL)
			{
				uint uiFileSize = 0xffffffff;
				uchar* pucFileData = NULL;
				
				uint uiTryCount = 0;
				while (pucFileData == NULL && uiFileSize != 0)
				{
					++uiTryCount;

					// 读取文件
					NiFile* pkFile = NiFile::GetFile(kFileName, NiFile::READ_ONLY);
					if (!pkFile)
					{

					}

					uiFileSize = pkFile->GetFileSize();

				}
			}
		}

		m_pkTaskQueue->NotifyCompleteTask(akTaskBuf, uiTaskNum);
	}
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadThread::Decrease()
{
	
}
//-------------------------------------------------------------------------------------------------
DWORD sdPixelLoadThread::LoadProc(LPVOID lpData)
{
	sdPixelLoadThread* pkPixelLoadThread = (sdPixelLoadThread*)lpData;
	pkPixelLoadThread->Load();
	return 0;
}
//-------------------------------------------------------------------------------------------------
DWORD sdPixelLoadThread::DecreaseProc(LPVOID lpData)
{
	sdPixelLoadThread* pkPixelLoadThread = (sdPixelLoadThread*)lpData;
	pkPixelLoadThread->Decrease();
	return 0;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE