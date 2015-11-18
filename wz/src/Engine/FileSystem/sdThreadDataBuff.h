//*************************************************************************************************
// 内容:	极其简单的线程数据缓存(参考自WZ::Client""Global::sdThreadDataBuff)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-08-02
// 最后修改:	2014-01-14
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_THREAD_DATA_BUFF__
#define _SD_ENGINE_FILESYSTEM_THREAD_DATA_BUFF__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// 线程数据缓存(每线程一个,用于作为文件读取缓存)
//-------------------------------------------------------------------------------------------------
class sdThreadDataBuff : public Base::sdRefObject
{
public:
	enum
	{
		E_MAIN_THREAD_DATA_BUFF_SIZE = 25 * 1024 * 1024,	///< 主线程25M
		E_RES_THREAD_DATA_BUFF_SIZE = 10 * 1024 * 1024,		///< 资源线程10M
		E_OTHER_THREAD_DATA_BUFF_SIZE = 1 * 1024 * 1024,	///< 其他线程1M
	};

public:
	sdThreadDataBuff();
	~sdThreadDataBuff();

	uchar* OpenBuff(uint uiSize);
	void CloseBuff();
	void ResizeBuff(uint uiSize);

protected:
	uchar* m_pucDataBuff;	///< 数据缓存
	uint m_uiSize;			///< 数据缓存尺寸
};
sdSmartPointer(sdThreadDataBuff);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif