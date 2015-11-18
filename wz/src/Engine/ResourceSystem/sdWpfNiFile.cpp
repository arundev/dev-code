#include "Stdafx.h"
#include "sdWpfNiFile.h"

#include <sdFileUtil.h>

using namespace std;
using namespace Base;

namespace ResourceSystem
{
////-------------------------------------------------------------------------------------------------
//NiFile* sdWpfNiFile::FileCreateFunc(const char *pcName, OpenMode eMode, uint uiBufferSize)
//{
//	string strFileName = sdFileUtil::NormalizePath(pcName);
//	if (eMode == NiFile::READ_ONLY)
//		return NiNew sdWpfNiFile(strFileName.c_str(), eMode, uiBufferSize);
//	else
//		return NiNew NiFile(strFileName.c_str(), eMode, uiBufferSize);
//}
////-------------------------------------------------------------------------------------------------
//bool sdWpfNiFile::FileAccessFunc(const char *pcName, OpenMode eMode)
//{
//	//return g_pkWpfFileSystem->IsFileExist(pcName);
//	return true;
//}
////-------------------------------------------------------------------------------------------------
//bool sdWpfNiFile::DirectoryExist(const char* pcName)
//{
//	DWORD dwAttribute = ::GetFileAttributes(pcName);
//	if (dwAttribute == -1) return false;
//	if ((dwAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0)	return true;
//
//	//return g_pkWpfFileSystem->IsFileExist(pcName);
//	return true;
//}
////-------------------------------------------------------------------------------------------------
//void sdWpfNiFile::D3DXEffectLoadFiles(NiTObjectArray<NiString>& pkFileList,
//	const char* pcDirectory, const char* pcExt, bool bRecurseDirectories)
//{
//	vector<string> akFiles;
//	//g_pkWpfFileSystem->GetFileList(akFiles, pcDirectory, pcExt, bRecurseDirectories);
//
//	vector<string>::iterator itr = akFiles.begin();
//	vector<string>::iterator itr_end = akFiles.end();
//	for (; itr!=itr_end; ++itr)
//	{
//		pkFileList.Add(itr->c_str());
//	}
//}
////-------------------------------------------------------------------------------------------------
//bool sdWpfNiFile::CreateDirFunc(const char* pcName)
//{
//	return false;
//}
////-------------------------------------------------------------------------------------------------
//
////-------------------------------------------------------------------------------------------------
//void sdWpfNiFile::Initialize()
//{
//	NiFile::SetFileCreateFunc(&sdWpfNiFile::FileCreateFunc);
//	NiFile::SetFileAccessFunc(&sdWpfNiFile::FileAccessFunc);
//	NiFile::SetDirectoryExistsFunc(&sdWpfNiFile::DirectoryExist);
//	//NiD3DXEffectLoader::SetNiD3DXEffectLoaderGetFiles(&sdWpfNiFile::D3DXEffectLoadFiles);
//}
////-------------------------------------------------------------------------------------------------
//void sdWpfNiFile::Destroy()
//{
//	NiFile::SetFileCreateFunc(NULL);
//	NiFile::SetFileAccessFunc(NULL);
//	NiFile::SetDirectoryExistsFunc(NULL);
//	//NiD3DXEffectLoader::SetNiD3DXEffectLoaderGetFiles(NULL);
//}
////-------------------------------------------------------------------------------------------------
//sdWpfNiFile::sdWpfNiFile(const char* pcName, OpenMode eMode, uint uiBufferSize)
//{
//	
//}
////-------------------------------------------------------------------------------------------------
}