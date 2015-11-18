#include "sdFileSystemPCH.h"
#include "sdDataStream.h"
#include "sdWpfFileSystem.h"

#include <sdStrUtil.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-----------------------------------------------------------------------------------------
sdDataStream::sdDataStream()
: m_pcData(NULL)
, m_iSize(0)
{
	
}
//-----------------------------------------------------------------------------------------
sdDataStream::sdDataStream(void* pvData, size_t size)
: m_pcData((char*)pvData)
, m_iSize(size)
{

}
//-----------------------------------------------------------------------------------------
sdDataStream::~sdDataStream()
{

}
//-----------------------------------------------------------------------------------------
std::string sdDataStream::GetLine(bool bTrimAfter)
{
	std::stringstream kRetStream;

	char szTmp[128];
	size_t iMaxSize = 127;
	size_t iSize = iMaxSize;
	while (iSize == 127)	///< keep looping while not hitting delimiter
	{
		iSize = ReadLine(szTmp, iMaxSize);
		kRetStream << szTmp;
	}

	std::string kRetString(kRetStream.str());
	if (bTrimAfter)
		kRetString = Base::Util::sdStrUtil::Trim(kRetString);

	return kRetString;
}
//-----------------------------------------------------------------------------------------
sdMemoryStream::sdMemoryStream(void* pvMem, size_t size)
: sdDataStream(pvMem, size)
, m_pcPos((char*)pvMem)
, m_pcEnd((char*)pvMem + size)
{

}
//-----------------------------------------------------------------------------------------
sdMemoryStream::~sdMemoryStream()
{
	if (m_pcData)
		Close();
}
//-----------------------------------------------------------------------------------------
size_t sdMemoryStream::Read(void* pvBuff, size_t iCount)
{
	size_t size = iCount;

	// Read over end of memory
	if (m_pcPos + size > m_pcEnd)	
		size = m_pcEnd - m_pcPos;

	if (size == 0)
		return 0;
	
	memcpy(pvBuff, m_pcPos, size);
	m_pcPos += size;

	return size;
}
//-----------------------------------------------------------------------------------------
size_t sdMemoryStream::ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim /* = "" */)
{
	// Deal with both Unix & Windows LFs
	bool trimCR = false;
	if (delim.find_first_of('\n') != std::string::npos)
		trimCR = true;

	size_t size = strcspn((const char*)m_pcPos, delim.c_str());
	if (size > iMaxCount)
		size = iMaxCount;

	// Make sure pos can never go past the end of the data
	if (m_pcPos + size > m_pcEnd)
		size = m_pcEnd - m_pcPos;

	if (size > 0)
		memcpy(pvBuff, m_pcPos, size);

	// Reposition pointer
	m_pcPos += size + 1;

	// Trim off trailing CR if this was a CR/LF entry
	if (trimCR && ((char*)pvBuff)[size-1] == '\r')
		--size;		///< terminate 1 character early

	// Terminate
	((char*)pvBuff)[size] = '\0';

	return size;
}
//-----------------------------------------------------------------------------------------
void sdMemoryStream::Skip(long lCount)
{
	size_t newPos = (size_t)((m_pcPos - m_pcData) + lCount);
	assert(m_pcData + newPos <= m_pcEnd);
	m_pcPos = m_pcData + newPos;
}
//-----------------------------------------------------------------------------------------
size_t sdMemoryStream::SkipLine(const std::string& delim /* = "" */)
{
	size_t pos = strcspn((const char*)m_pcPos, delim.c_str());

	// Make sure pos can never go past the end of the data
	if (m_pcPos + pos > m_pcEnd)	
		pos = m_pcEnd - m_pcPos;

	m_pcPos += pos + 1;

	return pos;
}
//-----------------------------------------------------------------------------------------
bool sdMemoryStream::Seek(int iOffset, int iOrigin)
{
	assert(iOrigin >= 0 && iOrigin <= 2);

	if (iOrigin == SEEK_CUR)
		m_pcPos += iOffset;
	else if (iOrigin == SEEK_SET)
		m_pcPos = m_pcData + iOffset;
	else if (iOrigin == SEEK_END)
		m_pcPos = m_pcData + m_iSize + iOffset;

	assert(m_pcPos >= m_pcData && m_pcPos <= m_pcData + m_iSize);

	return true;
}
//-----------------------------------------------------------------------------------------
size_t sdMemoryStream::Tell() const
{
	return m_pcPos - m_pcData;
}
//-----------------------------------------------------------------------------------------
bool sdMemoryStream::Eof() const
{
	return m_pcPos >= m_pcEnd;
}
//-----------------------------------------------------------------------------------------
void sdMemoryStream::Close()
{
	//g_pkWpfFileSystem->CLoseThreadDataBuffer(::GetCurrentThreadId());
	//m_pcData = NULL;
}
//-----------------------------------------------------------------------------------------
const char* sdMemoryStream::GetBuff() const
{
	return (const char*)m_pcData;
}
//-----------------------------------------------------------------------------------------
sdFileStream::sdFileStream(FILE* pfFile)
: sdDataStream()
, m_pfFile(pfFile)
, m_iStart(0)
, m_iCurPos(0)
{
	// Determine size
	fseek(pfFile, 0, SEEK_END);
	m_iSize = ftell(pfFile);
	fseek(pfFile, 0, SEEK_SET);
}
//-----------------------------------------------------------------------------------------
sdFileStream::~sdFileStream()
{
	if (m_pcData != NULL)
		Close();
}
//-----------------------------------------------------------------------------------------
size_t sdFileStream::Read(void* pvBuff, size_t iCount)
{
	size_t size = iCount;

	// Read over end of memory
	if (m_iCurPos + size > m_iSize)	
		size = m_iSize - m_iCurPos;

	if (size == 0)
		return 0;

	size = fread(pvBuff, 1, size, m_pfFile);
	m_iCurPos += size;

	return size; 
}
//-----------------------------------------------------------------------------------------
size_t sdFileStream::ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim /* = "" */)
{
	char acTempArea[128];

	return 0;
/*
	// Deal with both Unix & Windows LFs
	bool trimCR = false;
	if (delim.find_first_of('\n') != std::string::npos)
		trimCR = true;

	size_t size = strcspn((const char*)m_pcPos, delim.c_str());
	if (size > iMaxCount)
		size = iMaxCount;

	// Make sure pos can never go past the end of the data
	if (m_pcPos + size > m_pcEnd)
		size = m_pcEnd - m_pcPos;

	if (size > 0)
		memcpy(pvBuff, m_pcPos, size);

	// Reposition pointer
	m_pcPos += size + 1;

	// Trim off trailing CR if this was a CR/LF entry
	if (trimCR && ((char*)pvBuff)[size-1] == '\r')
		--size;		///< terminate 1 character early

	// Terminate
	((char*)pvBuff)[size] = '\0';

	return size;*/
}
//-----------------------------------------------------------------------------------------
void sdFileStream::Skip(long lCount)
{
	m_iCurPos += lCount;
	fseek(m_pfFile, lCount, SEEK_CUR);
}
//-----------------------------------------------------------------------------------------
size_t sdFileStream::SkipLine(const std::string& delim /* = "" */)
{
	// Re-use ReadLine(), but don't copy data
	char* pcNullBuf = NULL;
	return ReadLine(pcNullBuf, 1024, delim);
}
//-----------------------------------------------------------------------------------------
bool sdFileStream::Seek(int iOffset, int iOrigin)
{
	fseek(m_pfFile, iOffset, iOrigin);
	m_iCurPos = ftell(m_pfFile);

	return true;
}
//-----------------------------------------------------------------------------------------
size_t sdFileStream::Tell() const
{
	return ftell(m_pfFile) - m_iStart;
}
//-----------------------------------------------------------------------------------------
bool sdFileStream::Eof() const
{
	if (feof(m_pfFile) != 0)
		return true;
	return m_iCurPos >= m_iSize;
}
//-----------------------------------------------------------------------------------------
void sdFileStream::Close()
{
	if (m_pfFile)
	{
		fclose(m_pfFile);
		m_pfFile = NULL;
	}

	m_iCurPos = 0;
	m_iStart = 0;

	m_iSize = 0;
	m_pcData = NULL;

	IFileSystem* pkFileSystem = IFileSystem::GetFileSystem();
	sdWpfFileSystem* pkWpfFileSystem = (sdWpfFileSystem*)pkFileSystem;

	pkWpfFileSystem->CloseThreadDataBuff(::GetCurrentThreadId());
}
//-----------------------------------------------------------------------------------------
const char* sdFileStream::GetBuff() const
{
	if (!m_pcData)
	{
		IFileSystem* pkFileSystem = IFileSystem::GetFileSystem();
		sdWpfFileSystem* pkWpfFileSystem = (sdWpfFileSystem*)pkFileSystem;

		m_pcData = (char*)pkWpfFileSystem->OpenThreadDataBuff(::GetCurrentThreadId(), m_iSize+1);
		if (!m_pcData)
			return NULL;

		int iCur = ftell(m_pfFile);
		fread(m_pcData, 1, m_iSize, m_pfFile);
		m_pcData[m_iSize] = 0;
		fseek(m_pfFile, iCur, SEEK_SET);
	}

	return m_pcData;
}
//-----------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE