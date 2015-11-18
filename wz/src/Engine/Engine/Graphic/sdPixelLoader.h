//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PIXEL_LOADER_H__
#define _SD_ENGINE_PIXEL_LOADER_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
class IDataStream;
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
class sdTextureCache;
class sdPixelLoadTaskQueue;
class sdPixelLoadThread;
NiSmartPointer(sdPixelLoadThread);
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// sdPixelLoader,���߳�������������ṩ����ӿڣ�����ʵ����sdPixelLoadThread��
//---------------------------------------------------------------------------------------
class sdPixelLoader : public NiRefObject
{
public:
	sdPixelLoader(sdPixelLoadTaskQueue* pkTaskQueue, sdTextureCache* pkTextureCache);
	~sdPixelLoader();

	void Start();
	void Stop();

	void LoadWake();
	void LoadSleep();

	void DecreaseWake();
	void DecreaseSleep();

protected:
	sdPixelLoadThreadPtr m_pkPixelLoadThread;	///< �������ʵ��
};
//---------------------------------------------------------------------------------------
// sdPixelLoadThread�����̼߳��غ������������Ķ��߳�������ش���(�ڲ�ʵ�֣����ṩ����ӿ�)
//---------------------------------------------------------------------------------------
class sdPixelLoadThread : public NiRefObject
{
public:
	sdPixelLoadThread(sdPixelLoadTaskQueue* pkTaskQueue, sdTextureCache* pkTextureCache);
	~sdPixelLoadThread();

	// ��ʼ��������
	virtual bool Initialize();
	virtual void Destroy();
	virtual bool IsInitialized(){ return m_bInitialized;}

	//
	void Start();
	void Stop();

	void LoadWake();
	void LoadSleep();

	void DecreaseWake();
	void DecreaseSleep();

protected:
	// ���ڴ����ݴ���D3D����
	bool CreateD3DTexture(sdSourceTexture* pkSourceTexture, FileSystem::IDataStream* pkDataStream);

	// ���߳�ִ�к���
	void Load();
	void Decrease();

	// ���߳���ں���
	static DWORD WINAPI LoadProc(LPVOID lpData);
	static DWORD WINAPI DecreaseProc(LPVOID lpData);

protected:
	bool m_bInitialized;		///< �Ƿ񱻳�ʼ��
	bool m_bExit;				///< �˳����

	HANDLE m_hLoadThread;		///< �����߳�
	HANDLE m_hLoadEvent;		///< ��������¼�

	HANDLE m_hDecreaseThread;	///<
	HANDLE m_hDecreaseEvent;	///<

	sdPixelLoadTaskQueue* m_pkTaskQueue;	///< ָ���ϲ��ָ��
	sdTextureCache* m_pkTextureCache;		///< ָ���ϲ��ָ��
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif