//*************************************************************************************************
// 内容:	文件纹理
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SOURCE_TEXTURE_H__
#define _SD_ENGINE_SOURCE_TEXTURE_H__
#include "IDataStream.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
NiSmartPointer(sdSourceTexture);
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// sdSourceTexture
//
// 进行Managed类型贴图的动态缓存管理：
//	1.m_ucPriority
//	2.m_eResStatus
//	3.m_uiTimeStamp
//	4.m_uiFileSize/m_pucFileData
//		
// 需要注意的是:
//	1.这里没有对资源状态变化作原子操作(考虑原子操作或自旋锁)
//	2.如果先加入了异步加载队列,可能同步加载不会成功
//	3.缺少加载回调机制
//	4.纹理资源管理与Mesh/Skeleton不一样，
//		纹理数据是纯静态的，只需要有一份数据即可
//		Mesh/Skeleton会被修改，每份数据都需要一份拷贝(当然，可以使用写时拷贝)
//-------------------------------------------------------------------------------------------------
class sdSourceTexture : public NiSourceTexture
{
	NiDeclareRTTI;
	NiDeclareStream;
public:
	// 纹理状态
	typedef enum ETEXSTATUS
	{
		E_TEX_INVALID,		///< 初始状态
		E_TEX_PARAM_INITED,	///< 参数初始化完成
		E_TEX_PIX_PRE_LOAD,	///< 等待加载数据到内存
		E_TEX_PIX_LOADING,	///< 正在加载纹理数据到内存
		E_TEX_PIX_LOADED,	///< 成功加载纹理数据到内存
		E_TEX_CREATED,		///< 设备纹理创建成功
		E_TEX_IGNORED,		///< 取消状态(可能是创建设备纹理失败之类的)
		E_TEX_ERROR,		///< 纹理出错
	}ETexStatus;

	// Mipmap管理方式
	typedef enum EMIPMAPTYPE
	{
		E_MIPMAP_DEFAULT,			///< Mipmap使用缺省方式来管理
		E_MIPMAP_AUTO,				///< Mipmap由系统自动管理
		E_MIPMAP_MANUAL,			///< Mipmap手动管理
	}EMipmapType;

	// 纹理创建参数
	class CreateParams : public NiMemObject
	{
	public:
		FormatPrefs m_kPrefs;					///< 
		bool m_bAsyn;							///< 是否异步加载
		bool m_bLoadDirectToRendererHint;		///< 直接从磁盘加载纹理时,是否直接创建D3D纹理
		bool m_bSrcRendererDataIsPersistent;	///< 是否从PersistentData创建纹理对象
		uchar m_ucPriority;						///< 加载优先级
		EMIPMAPTYPE m_eMipmapType;				///< Mipmap管理方式
		uchar m_ucSkipMipmapLvl;				///< Mipmap跳过的等级(E_MIPMAP_MANUAL方式下有效)
		uchar m_ucMipmapLvl;					///< Mipmap等级(E_MIPMAP_MANUAL方式下有效)

	public:
		CreateParams() : m_bAsyn(true)
			,m_bLoadDirectToRendererHint(true)
			,m_bSrcRendererDataIsPersistent(false)
			,m_ucPriority(0)
			,m_eMipmapType(E_MIPMAP_AUTO)
			,m_ucSkipMipmapLvl(0)
			,m_ucMipmapLvl(0)
		{
	
		}
	};

public:
	/** \brief 创建SourceTexture对象(会被多线程调用)
	*
	*	@remark
	*	@param[in] kFilename
	*	@param[in] kParams	
	*	@return	
	*	@see	sdSourceTexture
	*	@note	代码参考自NiSourceTexture::Create()
	*			1.以不同的参数创建同一张纹理，返回的是最先创建的纹理
	*/
	static sdSourceTexturePtr Create(const NiFixedString& kFilename, CreateParams& kParams);

	/** \brief 重新加载数据(同步处理,涉及资源状态读取与更改,目前不支持加载中修改参数)
	*
	*	@remark
	*	@param[in]	ucMipmapSkipLvl	跳过的的mipmap等级
	*	@param[in]	ucMipmapLvl		请求的mipmap等级
	*	@return	
	*	@see	sdSourceTexture
	*	@note	函数内部禁止重入
	*/
	bool SyncReloadResource(uchar ucSkipMipmapLvl, uchar ucMipmapLvl);

	/** \brief 重新加载数据(异步处理,涉及资源状态读取与更改,目前不支持加载中修改参数)
	*
	*	@remark
	*	@param[in]	ucMipmapSkipLvl	跳过的的mipmap等级
	*	@param[in]	ucMipmapLvl		请求的mipmap等级
	*	@return	
	*	@see	sdSourceTexture
	*	@note	函数内部禁止重入
	*/
	bool AsyncReloadResource(uchar ucSkipMipmapLvl, uchar ucMipmapLvl);

	/** \brief 加载回调(加载线程调用,用于更新资源状态)
	*
	*	@remark
	*	@param[in]	eTexStatus	当前完成的资源状态
	*	@return					通知是否成功
	*	@see	sdSourceTexture
	*	@note	
	*/
	bool OnLoadingStatus(ETexStatus eTexStatus);

	//// 父类属性操作
	//void SetSourcePixelData(NiPixelData* pkPixelData){ m_spSrcPixelData = pkPixelData;}

	// 数据流(用于支持直接从裸数据流加载)
	void SetDataStream(FileSystem::IDataStream* pkDataStream){ m_spDataStream = pkDataStream;}
	FileSystem::IDataStream* GetDataStream() { return m_spDataStream;}

	// 属性操作
	EMipmapType GetMipmapType() const { return m_eMipmapType;}
	uchar GetMipmapLvl() const { return m_ucMipmapLvl;}
	uchar GetSkipMipmapLevel() const { return m_ucSkipMipmapLvl;}
	uchar GetPriority()const  { return m_ucPriority;}

	ETexStatus GetTexStatus() const {return m_eTexStatus;	}
	void SetTexStatus(ETexStatus eTexStatus){ m_eTexStatus = eTexStatus;}

	///< 尝试设置锁
	///< @{
	// 当前纹理是否位于加载队列
	//uint TrySetLoadingLock(){return AtomicSetFlagBit(E_TEX_IN_LOADING_MASK_SHIFT);}
	//uint TryResetLoadingLock(){	return AtomicResetFlagBit(E_TEX_IN_LOADING_MASK_SHIFT);	}
	///< @}

protected:
	// 标记
	//typedef enum ETEXSTATUSLOCK
	//{
	//	E_TEX_STATUS_UNLOCKED,
	//	E_TEX_STATUS_LOCKED,
	//}ETexStatusLock;

	enum
	{
		// 纹理位于加载队列的锁标记(用于原子化纹理对象与加载相关的操作)
		E_TEX_IN_LOADING_MASK		= 0x00010000,	

		E_TEX_IN_RELOADING_FUNC_MASK_SHIFT	= 0x1,	///< 每个纹理对象的AsyncReloadResource函数防止重入标记
		E_TEX_IN_LOADING_MASK_SHIFT			= 0x16,	///< 纹理位于加载队列的锁标记位位置
	};

protected:
	sdSourceTexture();
	virtual ~sdSourceTexture();

	//bool TryLockTexStatus()
	//{
	//	return (::InterlockedCompareExchange((volatile LONG*)(&m_uiTexStatusLock), E_TEX_STATUS_LOCKED,	E_TEX_STATUS_UNLOCKED) == E_TEX_STATUS_UNLOCKED);
	//}

	//void UnlockTexStatus()
	//{
	//	::InterlockedCompareExchange((volatile LONG*)(&m_uiTexStatusLock), E_TEX_STATUS_UNLOCKED, E_TEX_STATUS_LOCKED);
	//}

	// 将设置标记的指定位原子的设置为1/0
	uint AtomicSetFlagBit(uint uiBitPos)
	{
		return ::InterlockedBitTestAndSet((LONG volatile *)m_uiFlags, uiBitPos);
	}

	uint AtomicResetFlagBit(uint uiBitPos)
	{
		return ::InterlockedBitTestAndReset((LONG volatile *)m_uiFlags, uiBitPos);
	}

	uint TestFlagBit(uint uiBitPos)
	{
		return (m_uiFlags & (1 << uiBitPos));
	}

	// 当前
	uint TrySetAsyncReloadFuncLock(){return AtomicSetFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);}
	uint TryResetAsyncReloadFuncLock(){	return AtomicResetFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);	}
	uint TestAsyncReloadFuncLock(){ return TestFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);}

protected:
	///< 资源状态
	///<	1.如果使用无锁技术的话,必须很清楚状态的变迁
	///<	2.加上读写锁来保证原子操作和提高效率
	///< 这里使用的
	///< @{
	// 纹理状态(只能从低到高变迁，只有主线程在创建时和重载时、加载线程改动)
	ETexStatus m_eTexStatus;

	// 纹理加载优先级(只有在创建时赋值一次)
	uchar m_ucPriority;			

	// 纹理Mipmap的管理方式(只有在创建时赋值一次)
	EMipmapType m_eMipmapType;

	// 纹理Mipmap控制参数(动态调整，只有创建时、重载时、加载线程改动)
	uchar m_ucMipmapLvl;		///< 加载的Mipmap等级(0 means whole size, 1 means one level)
	uchar m_ucSkipMipmapLvl;	///< 加载Mipmap跳过的等级

	// 状态锁
	//volatile uint m_uiTexStatusLock;

	// 标记
	volatile uint m_uiFlags;
	///< @}

	// 纹理数据流,用于纹理异步加载
	Engine::FileSystem::IDataStreamPtr m_spDataStream;

	static EMIPMAPTYPE ms_eDefaultMipmapType;	///< Mipmap的管理方式
	static uchar ms_ucMinSkipMipmapLvl;			///< Mipmap的有效范围
	static uchar ms_ucMaxSkipMipmapLvl;			///<
	static uchar ms_ucMinLoadPriority;			///< 加载优先级范围
	static uchar ms_ucMaxLoadPriority;			///<

	static Base::Threading::sdCriticalSection m_kCreateLock;	///< 纹理创建锁(保证纹理创建的原子性)
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif