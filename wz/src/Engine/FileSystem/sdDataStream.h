//*************************************************************************************************
// ����:	����������(�ο���WZ::Client::Global::sdDataStream)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-16
// ����޸�:
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

	///< �麯���̳�(�̳���IDataStream)
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
	size_t m_iSize;			///< ���ݳߴ�
	mutable char* m_pcData;	///< ������
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

	// �麯���̳�
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
	char* m_pcPos;	///< ��ǰ����λ��
	char* m_pcEnd;	///< ָ�����ݽ�β
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

	// �麯���̳�
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
	FILE* m_pfFile;		///< ��ǰ�ļ�����
	size_t m_iCurPos;	///< ��ǰ�ļ�����λ��
	size_t m_iStart;	///< 
};
sdSmartPointer(sdFileStream);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif