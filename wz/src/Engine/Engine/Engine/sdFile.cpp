#include "sdEnginePCH.h"
#include "sdFile.h"

#include "IDataStream.h"
#include "IFileSystem.h"

#include <sdFileUtil.h>

using namespace Base::Util;
using namespace Engine::FileSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE

NiImplementDerivedBinaryStream(sdFile, SDFileRead, SDFileWrite);
//-------------------------------------------------------------------------------------------------
NiFile* sdFile::FileCreateFunc(const char* pcName, NiFile::OpenMode eMode, uint uiBuffSize, bool flushOnWrite)
{
	char szFileName[MAX_PATH];
	strcpy_s(szFileName, pcName);
	sdFileUtil::NormalizePath(szFileName);

	if (eMode > SD_OPEN_MODE_BASE)
		return NiNew sdFile(szFileName, eMode, uiBuffSize);
	else
		return NiFile::DefaultFileCreateFunc(szFileName, eMode, uiBuffSize, flushOnWrite);
}
//-------------------------------------------------------------------------------------------------
bool sdFile::FileAccessFunc(const char* pcName, NiFile::OpenMode eMode)
{
	if (eMode > SD_OPEN_MODE_BASE)
		return false;
	else
		return NiFile::DefaultFileAccessFunc(pcName, eMode);
}
//-------------------------------------------------------------------------------------------------
bool sdFile::CreateDirectoryFunc(const char* pcName)
{
	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdFile::DirectoryExistsFunc(const char* pcName)
{
	DWORD dwAttrib = ::GetFileAttributes(pcName);
	if (dwAttrib == -1)
		return false;

	if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
	//return g_pkWpfFileSystem->IsFileExist(pcName);
}
//-------------------------------------------------------------------------------------------------
//void D3DXEffectLoadFiles(NiTObjectArray<NiString>& kFileList, const char* pcDirectory, const char* pcExt, bool bRecurseDirectories)
//{
//	typedef std::vector<std::string> FileVec;
//	typedef std::vector<std::string>::iterator FileVecItr;
//	FileVec kFileVec;
//	g_pkWpfFileSystem->GetFileList(kFileVec, pcDirectory, pcExt, bRecurseDirectories);
//	
//	FileVecItr itr = kFileVec.begin();
//	FileVecItr itr = kFileVec.end();
//	for (; itr != itr_end; ++itr)
//	{
//		kFileList.Add(itr->c_str());
//	}
//}
//-------------------------------------------------------------------------------------------------
void sdFile::StaticInitialize()
{
	NiFile::SetFileCreateFunc(FileCreateFunc);
	NiFile::SetFileAccessFunc(FileAccessFunc);
	NiFile::SetDirectoryExistsFunc(DirectoryExistsFunc);
	//NiD3DXEffectLoader::SetNiD3DXEffectLoaderGetFiles(D3DXEffectLoadFiles);
}
//-------------------------------------------------------------------------------------------------
void sdFile::StaticDestroy()
{
	NiFile::SetFileCreateFunc(NULL);
	NiFile::SetFileAccessFunc(NULL);
	NiFile::SetDirectoryExistsFunc(NULL);
	//NiD3DXEffectLoader::SetNiD3DXEffectLoaderGetFiles(NULL);
}
//-------------------------------------------------------------------------------------------------
sdFile::sdFile(const char* pcName, OpenMode eMode, uint uiBuffSize)
: m_uiFileSize(0)
{
	if (pcName)
	{
		SetEndianSwap(false);

		m_strFileName = pcName;
		m_bGood = false;

		m_pkDataStream = IFileSystem::GetFileSystem()->GetDataStream(pcName);
		if (m_pkDataStream)
		{
			m_bGood = true;
			m_uiFileSize = m_pkDataStream->Size(); 
		}
	}
}
//-------------------------------------------------------------------------------------------------
sdFile::~sdFile()
{
	if (m_pkDataStream)
	{
		m_pkDataStream->Close();
		m_pkDataStream = 0;
	}
}
//-------------------------------------------------------------------------------------------------
uint sdFile::SDFileRead(void* pvBuffer, uint uiBytes)
{
	if (uiBytes == 0 || !m_bGood || !m_pkDataStream)
		return 0;

	return m_pkDataStream->Read(pvBuffer, uiBytes);
}
//-------------------------------------------------------------------------------------------------
uint sdFile::SDFileWrite(const void* pvBuffer, uint uiBytes)
{
	//return __super::FileWrite(pvBuffer, ulBytes);
	return 0;
}
//-------------------------------------------------------------------------------------------------
bool sdFile::Flush()
{
	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdFile::Seek(int iNumBytes) 
{
	if (!m_bGood || !m_pkDataStream)
		return false;

	return m_pkDataStream->Seek(iNumBytes, ms_iSeekCur);
}
//-------------------------------------------------------------------------------------------------
bool sdFile::Seek(int iOffset, SeekOrigin eSeekFrom)
{
	if (!m_bGood || !m_pkDataStream)
		return false;

	return m_pkDataStream->Seek(iOffset, eSeekFrom);
}
//-------------------------------------------------------------------------------------------------
uint sdFile::GetFileSize() const
{
	if (!m_bGood || !m_pkDataStream)
		return false;

	return m_pkDataStream->Size();
}
//-------------------------------------------------------------------------------------------------
bool sdFile::eof()
{
	if (!m_bGood || !m_pkDataStream)
		return false;

	return m_pkDataStream->Eof();
}
//-------------------------------------------------------------------------------------------------
bool sdFile::IsGood()
{
	return m_bGood;
}
//-------------------------------------------------------------------------------------------------
sdFile::operator bool() const
{
	return m_bGood;
}
//-------------------------------------------------------------------------------------------------
const char* sdFile::GetBuff()
{
	if (!m_bGood || !m_pkDataStream)
		return NULL;

	return m_pkDataStream->GetBuff();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE