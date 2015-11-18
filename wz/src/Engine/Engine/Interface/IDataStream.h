//*************************************************************************************************
// 内容:	数据流接口
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-14
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE_SYSTEM_DATA_STREAM_INTERFACE_H__
#define _SD_ENGINE_FILE_SYSTEM_DATA_STREAM_INTERFACE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// 数据流接口
//-------------------------------------------------------------------------------------------------
class IDataStream : public Base::sdRefObject
{
public:
	virtual ~IDataStream(){};

	/** \brief 读取数据
	*
	*	@remark	读取数据
	*	@param[in,out]	pvBuff	读取缓存
	*	@param[in]		iCount	读取尺寸
	*	@return					成功读取的数据尺寸
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Read(void* pvBuff, size_t iCount) = 0;

	/** \brief 读取一行数据
	*
	*	@remark	读取一行数据
	*	@param[in,out]	pvBuff	读取缓存
	*	@param[in]		iCount	读取尺寸
	*	@param[in]		delim	行结束符
	*	@return	成功读取的数据尺寸
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim = "\n") = 0;

	// @breif 获取一行数据
	virtual std::string GetLine(bool bTrimAfter = true) = 0;

	// @breif 跳过指定长度数据
	// @param[in]	lCount
	virtual void Skip(long lCount) = 0;

	// @breif 跳过一行数据
	// @param[in]	delim
	// @return
	virtual size_t SkipLine(const std::string& delim = "\n") = 0;

	/** \brief 寻址到指定位置
	*
	*	@remark	寻址到指定位置
	*	@param[in] iOffset	寻址便宜
	*	@param[in] iOrigin	寻址起始位置
	*	@return	操作是否成功
	*	@see	IDataStream
	*	@note
	*/
	virtual bool Seek(int iOffset, int iOrigin) = 0;

	/** \brief 获取当前位置
	*
	*	@remark	获取当前位置
	*	@return	当前位置
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Tell() const = 0;

	/** \brief 判断是否到达数据流结尾
	*
	*	@remark	判断是否到达数据流结尾
	*	@return	是否到达数据流结尾
	*	@see	IDataStream
	*	@note
	*/
	virtual bool Eof() const = 0;

	/** \brief 获取数据流尺寸
	*
	*	@remark	获取数据流尺寸
	*	@return	数据流尺寸
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Size() const = 0;

	/** \brief 获取缓存
	*
	*	@remark	获取缓存
	*	@return	指向数据缓存的指针
	*	@see	IDataStream
	*	@note
	*/
	virtual const char* GetBuff() const = 0;

	/** \brief 关闭数据流
	*
	*	@remark	关闭数据流
	*	@see	IDataStream
	*	@note
	*/
	virtual void Close() = 0;
};
sdSmartPointer(IDataStream);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif