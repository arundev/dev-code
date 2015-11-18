//*************************************************************************************************
// 内容:	基础数据流(参考自WZ::Client::Global::sdDataStream)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-16
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_DATA_STREAM__
#define _SD_ENGINE_FILESYSTEM_DATA_STREAM__

#include <IDataStream.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-----------------------------------------------------------------------------
// sdDataStream
//-----------------------------------------------------------------------------
class sdDataStream : public IDataStream
{
public:
	sdDataStream();
	sdDataStream(void* pvData, size_t size);
	virtual ~sdDataStream();

	///< 虚函数继承(继承自IDataStream)
	///< @{
	virtual size_t		Read(void* pvBuff, size_t iCount) { return 0;}
	virtual size_t		ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim = "\n") { return 0;}
	virtual std::string GetLine(bool bTrimAfter = true);
	virtual void		Skip(long lCount) {}
	virtual size_t		SkipLine(const std::string& delim = "\n") { return 0;}
	virtual bool		Seek(int iOffset, int iOrigin) { return true;}
	virtual size_t		Tell() const { return 0;}
	virtual bool		Eof() const { return true;}
	virtual size_t		Size() const { return m_iSize;}
	virtual const char* GetBuff() const{ return NULL;}
	virtual void		Close() {}
	///< @}

protected:
	size_t m_iSize;			///< 数据尺寸
	mutable char* m_pcData;	///< 数据区
};
sdSmartPointer(sdDataStream);
//-----------------------------------------------------------------------------
// sdMemoryStream
//-----------------------------------------------------------------------------
class sdMemoryStream : public sdDataStream
{
public:
	sdMemoryStream(void* pvMem, size_t size);
	virtual ~sdMemoryStream();

	// 虚函数继承
	virtual size_t		Read(void* pvBuff, size_t iCount);
	virtual size_t		ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim = "\n");
	virtual void		Skip(long lCount);
	virtual size_t		SkipLine(const std::string& delim = "\n");
	virtual bool		Seek(int iOffset, int iOrigin);
	virtual size_t		Tell() const;
	virtual bool		Eof() const;
	virtual void		Close();
	virtual const char* GetBuff() const;

protected:
	char* m_pcPos;	///< 当前操作位置
	char* m_pcEnd;	///< 指向数据结尾
};
sdSmartPointer(sdMemoryStream);
//-----------------------------------------------------------------------------
// sdFileStream
//-----------------------------------------------------------------------------
class sdFileStream : public sdDataStream
{
public:
	sdFileStream(FILE* pfFile);
	virtual ~sdFileStream();

	// 虚函数继承
	virtual size_t		Read(void* pvBuff, size_t iCount);
	virtual size_t		ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim = "\n");
	virtual void		Skip(long lCount);
	virtual size_t		SkipLine(const std::string& delim = "\n");
	virtual bool		Seek(int iOffset, int iOrigin);
	virtual size_t		Tell() const;
	virtual bool		Eof() const;
	virtual void		Close();
	virtual const char* GetBuff() const;

protected:
	FILE* m_pfFile;		///< 当前文件对象
	size_t m_iCurPos;	///< 当前文件操作位置
	size_t m_iStart;	///< 
};
sdSmartPointer(sdFileStream);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif