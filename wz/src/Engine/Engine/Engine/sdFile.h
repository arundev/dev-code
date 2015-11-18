//*************************************************************************************************
// ����:	ʵ��WPF�ļ�ϵͳ��NiFile����ת��
//---------------------------------------------------------
// ����:		
// ����:		2013-07-25
// ����޸�:
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
// �����޸�NiFile
//  1.�Ż��ļ���ȡ:ֻ��ģʽ�����ļ���ʱ���������ļ������ڴ�
//-------------------------------------------------------------------------------------------------
class sdFile : public efd::FileCommon
{
	NiDeclareDerivedBinaryStream();

public:
	// �ļ�����ģʽ
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

	// �ļ���д����
	uint SDFileRead(void* pvBuffer, uint uiBytes);
	uint SDFileWrite(const void* pvBuffer, uint uiBytes);

	// �麯���̳�(�̳���NiFile)
	virtual void SetEndianSwap(bool bSwap);	///< ��NiImplementDerivedBinaryStream�����Զ�ʵ��
	virtual bool Flush();
	virtual bool Seek(int iNumBytes);
	virtual bool Seek(int iOffset, SeekOrigin eSeekFrom);
	virtual uint GetFileSize() const;
	virtual bool eof();
	virtual bool IsGood();
	virtual operator bool() const;

	// @brief ��ȡ�ļ����ݵ���ʼָ��(�����ļ�����ǿ�ƽ��������ݶ�ȡ���ڴ�)
	virtual const char* GetBuff();
	
	// ��ʼ��������
	static void StaticInitialize();
	static void StaticDestroy();

protected:
	///< @brief �ļ���������
	///< @{
	///< @brief �ļ��򿪻򴴽�����
	///< @param[in] pcName
	///< @param[in] eMode
	///< @param[in] uiBuffSize
	///< @param[in] flushOnWrite
	///< @return
	static NiFile* FileCreateFunc(const char* pcName, 
		NiFile::OpenMode eMode, 
		uint uiBuffSize, 
		bool flushOnWrite);

	///< @brief �ļ����ʺ���
	///< @param[in] pcName
	///< @param[in] eMode
	///< @return
	static bool FileAccessFunc(const char* pcName, NiFile::OpenMode eMode);

	///< @brief Ŀ¼��������
	///< @param[in] pcName
	///< @return
	static bool CreateDirectoryFunc(const char* pcName);

	///< @brief �ļ������ж�����
	///< @param[in] pcName
	///< @return
	static bool DirectoryExistsFunc(const char* pcName);
	///< @}

protected:
	FileSystem::IDataStreamPtr m_pkDataStream;	///< �ļ�������
	std::string m_strFileName;					///< �ļ���(���Ǹ�ΪNiFixedString/char)
	uint m_uiFileSize;
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif