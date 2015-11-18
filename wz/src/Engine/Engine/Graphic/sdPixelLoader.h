//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
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
// sdPixelLoader,多线程纹理加载器，提供对外接口，具体实现在sdPixelLoadThread中
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
	sdPixelLoadThreadPtr m_pkPixelLoadThread;	///< 具体加载实现
};
//---------------------------------------------------------------------------------------
// sdPixelLoadThread，多线程加载函数，负责具体的多线程纹理加载处理(内部实现，不提供对外接口)
//---------------------------------------------------------------------------------------
class sdPixelLoadThread : public NiRefObject
{
public:
	sdPixelLoadThread(sdPixelLoadTaskQueue* pkTaskQueue, sdTextureCache* pkTextureCache);
	~sdPixelLoadThread();

	// 初始化与销毁
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
	// 从内存数据创建D3D纹理
	bool CreateD3DTexture(sdSourceTexture* pkSourceTexture, FileSystem::IDataStream* pkDataStream);

	// 多线程执行函数
	void Load();
	void Decrease();

	// 多线程入口函数
	static DWORD WINAPI LoadProc(LPVOID lpData);
	static DWORD WINAPI DecreaseProc(LPVOID lpData);

protected:
	bool m_bInitialized;		///< 是否被初始化
	bool m_bExit;				///< 退出标记

	HANDLE m_hLoadThread;		///< 加载线程
	HANDLE m_hLoadEvent;		///< 加载完成事件

	HANDLE m_hDecreaseThread;	///<
	HANDLE m_hDecreaseEvent;	///<

	sdPixelLoadTaskQueue* m_pkTaskQueue;	///< 指向上层的指针
	sdTextureCache* m_pkTextureCache;		///< 指向上层的指针
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif