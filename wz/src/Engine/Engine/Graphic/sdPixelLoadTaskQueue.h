//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PIXEL_LOADER_TASK_QUEUE_H__
#define _SD_ENGINE_PIXEL_LOADER_TASK_QUEUE_H__

#include <sdCriticalSection.h>
#include "sdFixedStringExt.h"

#define MAX_TEXTURE_PRIORITY 5	///< (0~4)

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
NiSmartPointer(sdSourceTexture);
class sdTextureCache;
class sdPixelLoader;
NiSmartPointer(sdPixelLoader);
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// sdPixelLoadTaskQueue,纹理任务队列
//
// 注意：
//	1.不支持取消加载
//	2.任务加入加载队列之后不支持参数修改
//---------------------------------------------------------------------------------------
class sdPixelLoadTaskQueue : public NiRefObject
{
public:
	// 纹理任务
	class PixelLoadTask : public NiMemObject
	{
	public:
		char				m_cMipmap;		///< 纹理的mipmap等级
		sdSourceTexturePtr	m_pkTexture;	///< 纹理对象
	};
	typedef std::list<PixelLoadTask> PixelLoadTaskList;
	typedef std::list<PixelLoadTask>::iterator PixelLoadTaskListItr;

	//
	class PixelLoadTaskIndicator : public NiMemObject
	{
	public:
		uint	m_uiPreloadCount;
		uint	m_uiLoadingCount;
	};

public:
	sdPixelLoadTaskQueue(sdTextureCache* pkTextureCache);
	~sdPixelLoadTaskQueue();

	/** \brief 添加纹理加载任务(多线程调用)
	*
	*	@remark
	*	@param[in] spTexture		待处理的纹理对象
	*	@param[in] ucSkipMipmapLvl	目标Mipmap等级
	*	@return						添加是否成功
	*	@see	sdSourceTexture
	*	@note	
	*/
	bool AddLoadTask(sdSourceTexturePtr pkTexture, uchar ucSkipMipmapLvl);

	// @breif	添加纹理降级任务(多线程调用)
	// @param[in] pkTexture			待处理的纹理对象
	// @param[in] ucSkipMipmapLvl	目标Mipmap等级
	// @return						添加是否成功
	bool AddDecreaseTask(sdSourceTexture* pkTexture, uchar ucSkipMipmapLvl);

	// @brief	获取当前的纹理任务(加载线程调用)
	// @param[out]	pkTaskes
	// @param[in]	uiTaskWanted
	// @return		
	uint GetRecentPreloadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted);
	uint GetRecentLoadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted);

	// @brief 通知纹理任务完成(加载线程调用)
	// @param[in]	pkTaskes
	// @param[in]	uiTaskNum
	// @return
	void NotifyCompleteTask(PixelLoadTask* pkTaskes, uint uiTaskNum);

protected:
	// 从内存纹理创建D3D设备纹理
	bool CreateD3DTexture(sdSourceTexture* pkSourceTexture);

protected:
	// 待加载队列(纹理数据还在磁盘的的纹理对象)
	PixelLoadTaskList m_kPreloadListVec[MAX_TEXTURE_PRIORITY + 1];			
	PixelLoadTaskIndicator m_kLoadIndicatorList[MAX_TEXTURE_PRIORITY + 1];

	// 待创建设备对象队列(纹理数据已经被加载到内存但是还需要创建设备纹理)
	PixelLoadTaskList m_kLoadedList;

	// 所有任务数据库(保证加载任务的唯一性)
	typedef Engine::TFixedStringMap<sdSourceTexture*> SourceTextureMap;
	typedef Engine::TFixedStringMap<sdSourceTexture*>::iterator SourceTextureMapItr;
	SourceTextureMap m_kTaskDB;

	// 纹理加载器
	sdPixelLoaderPtr m_pkPixelLoader;

	// 线程锁(保证对本类访问的唯一性)
	Base::Threading::sdCriticalSection m_kLock;
};

//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif