//*************************************************************************************************
// 内容:	实现WPF文件系统与NiFile操作转接
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-25
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE__
#define _SD_ENGINE_FILE__

#include <efd/FileCommon.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
class IDataStream;
sdSmartPointer(IDataStream);
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// 用于修改NiFile
//  1.优化文件读取:只读模式下在文件打开时，将整个文件读入内存
//-------------------------------------------------------------------------------------------------
class sdFile : public efd::FileCommon
{
	NiDeclareDerivedBinaryStream();

public:
	// 文件访问模式
	typedef enum
	{
		SD_OPEN_MODE_BASE = APPEND_ONLY_TEXT,
		
		SD_READ_ONLY,
		SD_WRITE_ONLY,
		SD_APPEND_ONLY,
		SD_READ_ONLY_TEXT,
		SD_WRITE_ONLY_TEXT,
		SD_APPEND_ONLY_TEXT,
	} OpenModeEx;

public:
	sdFile(const char* pcName, OpenMode eMode, uint uiBuffSize = 32768);
	virtual ~sdFile();

	// 文件读写函数
	uint SDFileRead(void* pvBuffer, uint uiBytes);
	uint SDFileWrite(const void* pvBuffer, uint uiBytes);

	// 虚函数继承(继承自NiFile)
	virtual void SetEndianSwap(bool bSwap);	///< 在NiImplementDerivedBinaryStream宏内自动实现
	virtual bool Flush();
	virtual bool Seek(int iNumBytes);
	virtual bool Seek(int iOffset, SeekOrigin eSeekFrom);
	virtual uint GetFileSize() const;
	virtual bool eof();
	virtual bool IsGood();
	virtual operator bool() const;

	// @brief 获取文件数据的起始指针(对于文件，会强制将所有数据读取到内存)
	virtual const char* GetBuff();
	
	// 初始化与销毁
	static void StaticInitialize();
	static void StaticDestroy();

protected:
	///< @brief 文件操作函数
	///< @{
	///< @brief 文件打开或创建函数
	///< @param[in] pcName
	///< @param[in] eMode
	///< @param[in] uiBuffSize
	///< @param[in] flushOnWrite
	///< @return
	static NiFile* FileCreateFunc(const char* pcName, 
		NiFile::OpenMode eMode, 
		uint uiBuffSize, 
		bool flushOnWrite);

	///< @brief 文件访问函数
	///< @param[in] pcName
	///< @param[in] eMode
	///< @return
	static bool FileAccessFunc(const char* pcName, NiFile::OpenMode eMode);

	///< @brief 目录创建函数
	///< @param[in] pcName
	///< @return
	static bool CreateDirectoryFunc(const char* pcName);

	///< @brief 文件存在判定函数
	///< @param[in] pcName
	///< @return
	static bool DirectoryExistsFunc(const char* pcName);
	///< @}

protected:
	FileSystem::IDataStreamPtr m_pkDataStream;	///< 文件数据流
	std::string m_strFileName;					///< 文件名(考虑改为NiFixedString/char)
	uint m_uiFileSize;
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif