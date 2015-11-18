#include "sdEnginePCH.h"
////#include "sdWpfLoader.h"
////#include <algorithm>
////#include <stdio.h>
////#include  <io.h>
////#include <fcntl.h>
////#include <sys/stat.h>
////#include <ShlObj.h>
////#include <vector>
////#include "Compr.h"
////#include "..\sdCriticalSection.h"
//
//#pragma warning(disable:4996)
//
//DWORD sdWpfLoader::m_dwCryptTable[MAX_WPF_HASH_BUFFF_SIZE];
//
////#include <sdsafe.h>
//
//char szLogBuffer[1024];
//
//static int iFileCount = 0;
//
//class __auto_inter_wpf_buffer
//{
//public:
//	__auto_inter_wpf_buffer()
//	{
//		sdWpfLoader::InitBuffer();
//	}
//} __auto_wpf_buffer;
//
//sdWpfLoader::sdWpfLoader(void)
//{
//	InitializeCriticalSection(&m_lock);
//
//	m_lCount = 0;
//	m_lCurrent = 0
//
//	m_dwExpendLen = 10 * 1024 *1024;
//	m_bSaveWhenExpended = true;
//	m_bSaving = false;
//	m_pBlankBlockList = NULL;
//	for (int i = 0 ; i < EFT_NUM; i ++)
//	{
//		m_iFileHandle[i] = -1;
//	}
//
//	m_eWrLastError = EWR_UNKNOWN;
//	m_bModifyed = false;
//	m_iWpfStartPos = 0;
//	m_dwOpenType = EWOT_READONLY;
//	m_pFcb1List = NULL;
//	m_pFcb2List = NULL;
//	m_pFcbList = NULL;
//	m_pWpfRoot = NULL;
//	m_pDirList = NULL;
//	m_CreateDirHeader.pList = NULL;
//	m_CreateDirHeader.pNext = NULL;
//	m_CreateFcbHeader.pfcb = NULL;
//	m_CreateFcbHeader.pNext = NULL;
//
//	m_bCompress = false;
//
//	//InitBuffer();
//
//	sprintf_s(szLogBuffer, 1024, "sdWpfLoader_%i.log", iFileCount);
//}
//
//sdWpfLoader::~sdWpfLoader(void)
//{
//	WpfClose();
//	DeleteCriticalSection(&m_lock);
//}
//
//void sdWpfLoader::SetIsCompress(bool bCompress)
//{
//	m_bCompress = bCompress;
//}
//
//bool sdWpfLoader::WpfOpen(const char * strPathName,__int64 iOffset,DWORD dwOpenType)
//{
//	//LOG_TEXT("[sdWpfOpen] - %s Offset [%i] Type [%i], strPathName, iOffset, dwOpenType");
//
//	if(strPathName == NULL)
//	{
//		m_eWrLastError = EWR_PARA_ERROR;
//		return false;
//	}
//
//	if(m_bSaving)
//	{
//		m_eWrLastError = EWR_FILE_SAVING;
//		return false;
//	}
//
//	//先关闭当前打开的wpf文件
//	WpfClose();
//
//	//m_strWpfDir = strPathName;
//	//replace(m_strWpfDir.begin(),m_strWpfDir.end(),'/','\\');
//	//size_t iPos = m_strWpfDir.find_last_of("\\");
//	//if (iPos != string::npos)
//	//{
//	//	m_strWpfFileName = m_strWpfDir.substr(iPos + 1,m_strWpfDir.length() - iPos - 1);
//	//	m_strWpfDir = m_strWpfDir.substr(0,iPos);
//	//}
//	m_strWpfFileName = strPathName;
//
//	//打开或创建文件
//	errno_t err = 0;
//	if (dwOpenType & EWOT_RDWR)
//	{
//		err = 	_sopen_s(&(m_iFileHandle[EFT_MAIN]), strPathName, _O_BINARY | _O_RDWR , _SH_DENYWR,  _S_IREAD | _S_IWRITE );
//		dwOpenType |= EWOT_LISTDIR;
//	}
//	else if ((dwOpenType & EWOT_READONLY) || (dwOpenType & EWOT_LISTDIR))
//	{
//		err = 	_sopen_s(&(m_iFileHandle[EFT_MAIN]), strPathName, _O_BINARY | _O_RDONLY , _SH_DENYWR,  _S_IREAD);
//	}
//	else
//	{
//		m_eWrLastError = EWR_PARA_ERROR;
//		return false;
//	}
//
//	if(err != 0)
//	{
//		m_eWrLastError = EWR_FILE_OPEN_FAIL;
//		return false;
//	}
//
//	for (int i = EFT_THREAD ; i < EFT_NUM; i ++)
//	{
//		err = 	_sopen_s(&(m_iFileHandle[i]), strPathName, _O_BINARY | _O_RDONLY , _SH_DENYNO,  _S_IREAD);
//
//		if(err != 0)
//		{
//			m_eWrLastError = EWR_FILE_OPEN_FAIL;
//			return false;
//		}
//	}
//
//	m_iWpfStartPos = iOffset;
//
//	//读入文件头
//	_lseeki64(m_iFileHandle[EFT_MAIN],m_iWpfStartPos,SEEK_SET);
//	_read(m_iFileHandle[EFT_MAIN],&m_WpfHeader,sizeof(S_WpfHeader));
//
//	//如果上次文件头没有正确保存,读取必份的文件
//	if (m_WpfHeader.bySavingHeader)
//	{
//		//读入文件头备份
//		_lseeki64(m_iFileHandle[EFT_MAIN],m_iWpfStartPos + sizeof(S_WpfHeader),SEEK_SET);
//		_read(m_iFileHandle[EFT_MAIN],&m_WpfHeader_Bak,sizeof(S_WpfHeader));
//	}
//
//	if(m_WpfHeader.dwMagic != WPF_MAGIC)
//	{
//		m_eWrLastError = EWR_NOT_WPF_FILE;
//		WpfClose();
//		return false;
//	}
//
//	m_WpfHeader.bySavingHeader = FALSE;//理论上不可能出现文件头及备份文件头m_WpfHeader.bySavingHeader同时为TRUE,还是清一下标记
//	m_dwOpenType = dwOpenType;
//
//	//如果以写方式打开要求读入空闲块描述信息
//	if ((m_dwOpenType & EWOT_RDWR) && m_WpfHeader.dwBlankBlockSize > 0)
//	{
//		PBlankBlock pBlock = new BlankBlock[m_WpfHeader.dwBlankBlockSize];
//		_lseeki64(m_iFileHandle[EFT_MAIN],m_iWpfStartPos + m_WpfHeader.iBlankBlockPos,SEEK_SET);
//		_read(m_iFileHandle[EFT_MAIN],pBlock,m_WpfHeader.dwBlankBlockSize * sizeof(BlankBlock));
//
//		PBlankBlock p = pBlock;
//		PBlankBlockNode pList = NULL,pList2 = NULL;
//
//		pList = new BlankBlockNode();
//		pList->blankBlock.dwStart = p->dwStart;
//		pList->blankBlock.dwSize = p->dwSize;
//		pList->pParent = NULL;
//		pList->pNext = NULL;
//		m_pBlankBlockList = pList;
//		pList2 = pList;
//		p ++;
//
//		for (DWORD i = 1; i < m_WpfHeader.dwBlankBlockSize; i++,p++)
//		{
//			pList = new BlankBlockNode();
//			pList->blankBlock.dwStart = p->dwStart;
//			pList->blankBlock.dwSize = p->dwSize;
//			pList->pParent = pList2;
//			pList2->pNext = pList;
//			pList2 = pList;
//		}
//
//		pList2->pNext = NULL;
//
//		SAFE_DELETE_ARRAY(pBlock);
//	}
//
//	//载入文件分配表
//	return WpfLoadFat();	
//}
//
//void sdWpfLoader::WpfClear()
//{
//	PBlankBlockNode pBlankBlockList = m_pBlankBlockList;
//	while(m_pBlankBlockList)
//	{
//		pBlankBlockList = m_pBlankBlockList->pNext;
//		SAFE_DELETE(m_pBlankBlockList);
//		m_pBlankBlockList = pBlankBlockList;
//	}
//
//	SAFE_DELETE_ARRAY(m_pFcb1List);
//	SAFE_DELETE_ARRAY(m_pFcb2List);
//	SAFE_DELETE_ARRAY(m_pFcbList);
//	SAFE_DELETE_ARRAY(m_pDirList);
//	//清除新建目录及文件时申请的内存
//	NEWDIR *p = m_CreateDirHeader.pNext,*p2 = NULL;
//	while(p)
//	{
//		p2 = p->pNext;
//		SAFE_DELETE(p->pList);
//		SAFE_DELETE(p);
//		p = p2;
//	}
//
//	m_pWpfRoot = NULL;
//	//文件FCB
//	NEWFCB *q = m_CreateFcbHeader.pNext,*q2 = NULL;
//	while(q)
//	{
//		q2 = q->pNext;
//		SAFE_DELETE(q->pfcb->pfcb1);
//		SAFE_DELETE(q->pfcb->pfcb2);
//		SAFE_DELETE(q->pfcb);
//		SAFE_DELETE(q);
//		q = q2;
//	}
//
//	m_MhashFcb1.clear();
//	m_CreateDirHeader.pList = NULL;
//	m_CreateDirHeader.pNext = NULL;
//	m_CreateFcbHeader.pfcb = NULL;
//	m_CreateFcbHeader.pNext = NULL;
//
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.begin();
//	while (itr != m_mapLoadingFileData.end())
//	{
//		for (int i = 0; i < EFT_NUM; i++)
//		{
//			itr->second.m_pOpenFileFcb1[i] = NULL;
//			itr->second.m_iOpenFilePointPos[i] = NULL;
//		}
//		++itr;
//	}
//}
//
//void sdWpfLoader::WpfClose()
//{
//	if(m_bSaving)
//	{
//		m_eWrLastError = EWR_FILE_SAVING;
//		return;
//	}
//
//	WpfClear();
//
//	//关闭文件
//	for (int i = 0 ; i < EFT_NUM; i ++)
//	{
//		if(m_iFileHandle[i] != -1)
//		{
//			_close(m_iFileHandle[i]);
//			m_iFileHandle[i] = -1;
//		}
//	}
//
//	m_iWpfStartPos = 0;
//	m_bModifyed = false;
//}
//
//E_WPF_RTN sdWpfLoader::GetLastError()
//{
//	return m_eWrLastError;
//}
//
//PWpfFileList sdWpfLoader::GetRoot()
//{
//	return m_pWpfRoot;
//}
//
//WpfHeader* sdWpfLoader::GetHeader()
//{
//	if (!IsOpenWpfFile())
//	{
//		m_eWrLastError = EWR_FILE_NOT_OPEN;
//		return NULL;
//	}
//
//	return &m_WpfHeader;
//}
//
//PWpfFileList sdWpfLoader::FindDir(const char * strPathName)
//{
//	if(strPathName == NULL || (m_dwOpenType & EWOT_LISTDIR))
//	{
//		m_eWrLastError = EWR_PARA_ERROR;
//		return NULL;
//	}
//
//	if(!IsOpenWpfFile())
//	{
//		m_eWrLastError = EWR_FILE_NOT_OPEN;
//		return NULL;
//	}
//
//	string strDir = strPathName;
//	replace(strDir.begin(),strDir.end(),'/','\\');
//	if(strDir.empty())
//	{
//		return m_pWpfRoot;
//	}
//
//	return FindDirFromList(strDir.c_str(),m_pWpfRoot);
//}
//
//PWpfFileList sdWpfLoader::FindDirFromList(const string &strDir,PWpfFileList pList)
//{
//	if(!pList || strDir.empty() || (m_dwOpenType & EWOT_LISTDIR) == 0)
//		return NULL;
//
//	size_t iPos = strDir.find_first_of("\\");
//
//	MList::iterator itr = pList->MDirs.find( iPos == string::npos?strDir:strDir.substr(0,iPos) );
//	if(itr != pList->MDirs.end())
//	{
//		if(iPos != string::npos)
//			return FindDirFromList(strDir.substr(iPos + 1,strDir.length() - iPos),itr->second);
//		else
//			return itr->second;
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
//std::string  sdWpfLoader::GetDirFullPath(PWpfFileList pList)
//{
//	string strPath = "";
//	string strTemp = "";
//
//	while(pList && pList->pfcb && pList->pfcb->pParent && pList->pfcb->pfcb2)//root 不包含在全路径中
//	{
//		strTemp = pList->pfcb->pfcb2->strName;
//		if (strPath.empty())
//			strPath = strTemp;
//		else
//			strPath = strTemp + "\\" + strPath;
//
//		pList = pList->pfcb->pParent;
//	}
//
//	return strPath.c_str();
//}
//
//PFCB sdWpfLoader::FindFile_PFCB(const char * strPathName)
//{
//	if(!IsOpenWpfFile() || (m_dwOpenType & EWOT_READONLY))
//	{
//		m_eWrLastError = EWR_FILE_NOT_OPEN;
//		return NULL;
//	}
//
//	string strPath = strPathName;
//	//replace(strPath.begin(),strPath.end(),'/','\\');
//	size_t iPos = strPath.find_last_of("\\");
//
//	PWpfFileList pList = NULL;
//	string strFileName = strPathName;
//
//	if(iPos != string::npos)
//	{
//		string strDir = strPath.substr(0,iPos);
//		strFileName = strPath.substr(iPos + 1,strPath.length() - iPos);
//		pList = FindDirFromList(strDir,m_pWpfRoot);
//	}
//	else
//	{
//		pList = m_pWpfRoot;
//	}
//
//	if(pList)
//	{
//		MSFCB::iterator itr = pList->MFiles.find(strFileName);
//		if(itr != pList->MFiles.end())
//		{
//			return itr->second;
//		}
//	}
//
//	return NULL;
//}
//
//long sdWpfLoader::FileOpen(const char * strPathName,E_FPTYPE fpType)
//{
//	sdCriticalSection2(m_lock);
//
//	//LOG_TEXT("[FileOpen] - %s" Type [%i]", strPathName, fpType);
//
//	if (!strPathName || !IsOpenWpfFile())
//	{
//		// LOG_ERROR("[FileOpen] - Error1", strPathName);
//		return -1;
//	}
//
//	if(m_bSaving)
//	{
//		// LOG_ERROR("[FileOpen] - Error2", strPathName);
//		m_eWrLastError = EWR_FILE_SAVING;
//		return NULL;
//	}
//
//	LoadingFileData fileData;
//
//	if ((m_dwOpenType & EWOT_RDWR) || (m_dwOpenType & EWOT_LISTDIR))
//	{
//		PFCB pfcb = FindFile(strPathName);
//		if (pfcb)
//		{
//			fileData.m_pOpenFileFcb1[fpType] = pfcb->pfcb1;
//			fileData.m_pOpenFileFcb2[fpType] = pfcb->pfcb2;
//		}
//		else
//		{
//			fileData.m_pOpenFileFcb1[fpType] = NULL;
//			fileData.m_pOpenFileFcb2[fpType] = NULL;
//		}
//	}
//	else if (m_dwOpenType & EWOT_READONLY)
//	{
//		MIFCB1::iterator itr = m_MhashFcb1.find(HashFileName(strPathName));
//		if (itr != m_MhashFcb1.end())
//		{
//			fileData.m_pOpenFileFcb1[fpType] = itr->second;
//			fileData.m_pOpenFileFcb2[fpType] = NULL;
//		}
//		else
//		{
//			fileData.m_pOpenFileFcb1[fpType] = NULL;
//			fileData.m_pOpenFileFcb2[fpType] = NULL;
//		}	
//	}
//	else
//	{
//		fileData.m_pOpenFileFcb1[fpType] = NULL;
//		fileData.m_pOpenFileFcb2[fpType] = NULL;	
//	}
//
//	if (NULL == fileData.m_pOpenFileFcb1[fpType])
//	{
//		return -1;
//	}
//
//	_lseeki64(m_iFileHandle[fpType],m_iWpfStartPos + m_WpfHeader.wHeaderSize + pfcb->pfcb1->dwStart * m_WpfHeader.wBytesPerBlock,SEEK_SET);	
//	m_iOpenFilePointPos[fpType] = 0;
//
//	m_mapLoadingFileData[m_lCount] = fileData;
//
//	m_lCurrent = m_lCount;
//
//	return m_lCount++;
//}
//
//bool sdWpfLoader::FileClose(long lCurrFile, E_FPTYPE fpType)
//{
//	sdCriticalSection2(m_lock);
//
//	//LOG_TEXT("[FileClose] - %i" Type [%i]", lCurrFile, fpType);
//
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr != m_mapLoadingFileData.end())
//	{
//		itr->second.m_pOpenFileFcb1[fpType] = NULL;
//		itr->second.m_iOpenFilePointPos[fpType] = 0;
//		m_mapLoadingFileData.erase(itr);
//	}
//
//	if (m_lCurrent == lCurrFile)
//		m_lCurrent = -1;
//
//	return true;
//}
//
//bool sdWpfLoader::FileSeek(long lCurrFile, DWORD dwOffset,int iOrigin,E_FPTYPE fpType)
//{
//	sdCriticalSection2(m_lock);
//
//	//LOG_TEXT("[FileSeek] [%i] [%i]-[%i] Type [%i]", lCurrFile, dwOffset, iOrigin, fpType);
//
//	if(m_bSaving)
//	{
//		// LOG_ERROR("[FileSeek] [%i] - Error1", lCurrFile);
//		m_eWrLastError = EWR_FILE_SAVING;
//		return false;
//	}
//
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		// LOG_ERROR("[FileSeek] [%i] - Error2", lCurrFile);
//		return false;
//	}
//
//	m_lCurrent = lCurrFile;
//
//	if (NULL == itr->second.m_pOpenFileFcb1[fpType])
//	{
//		// LOG_ERROR("[FileSeek] [%i] - Error3", lCurrFile);
//		return false;	
//	}
//
//	switch(iOrigin)
//	{
//	case SEEK_SET:
//		if(dwOffset >= itr->second.m_pOpenFileFcb1[fpType]->dwSize)
//		{
//			// LOG_ERROR("[FileSeek] [%i] - Error4", lCurrFile);
//			return false;
//		}
//
//		_lseeki64(m_iFileHandle[fpType],m_iWpfStartPos + m_WpfHeader.wHeaderSize + itr->second.m_pOpenFileFcb1[fpType]->dwStart * m_WpfHeader.wBytesPerBlock + dwOffset,SEEK_SET);	
//		break;
//	case SEEK_CUR:
//		if(dwOffset + itr->second.m_iOpenFilePointPos[fpType] >= itr->second.m_pOpenFileFcb1[fpType]->dwSize)
//		{
//			// LOG_ERROR("[FileSeek] [%i] - Error5", lCurrFile);
//			return false;
//		}
//
//		itr->second.m_iOpenFilePointPos[fpType] += dwOffset;
//
//		_lseeki64(m_iFileHandle[fpType],m_iWpfStartPos + m_WpfHeader.wHeaderSize + itr->second.m_pOpenFileFcb1[fpType]->dwStart * m_WpfHeader.wBytesPerBlock + itr->second.m_iOpenFilePointPos[fpType],SEEK_SET);	
//		break;
//	case SEEK_END:
//		if(dwOffset >= itr->second.m_pOpenFileFcb1[fpType]->dwSize)
//		{
//			// LOG_ERROR("[FileSeek] [%i] - Error6", lCurrFile);
//			return false;
//		}
//
//		itr->second.m_iOpenFilePointPos[fpType] = itr->second.m_pOpenFileFcb1[fpType]->dwSize - dwOffset;
//
//		_lseeki64(m_iFileHandle[fpType],m_iWpfStartPos + m_WpfHeader.wHeaderSize + itr->second.m_pOpenFileFcb1[fpType]->dwStart * m_WpfHeader.wBytesPerBlock + itr->second.m_iOpenFilePointPos[fpType],SEEK_SET);	
//		break;
//
//	default:
//		break;
//	}
//
//	return true;
//}
//
//DWORD sdWpfLoader::FileRead(long lCurrFile, void* pBbuff,DWORD dwSize,E_FPTYPE fpType)
//{
//	sdCriticalSection2(m_lock);
//
//	//LOG_TEXT("[FileRead] [%i] Size [%i] Type [%i]", lCurrFile, dwSize, fpType);
//
//	if(m_bSaving)
//	{
//		// LOG_ERROR("[FileRead] [%i] - Error1", lCurrFile);
//		m_eWrLastError = EWR_FILE_SAVING;
//		return 0;
//	}
//	
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		// LOG_ERROR("[FileRead] [%i]-[%i] - Error2", lCurrFile, m_lCurrent);
//		return false;
//	}
//
//	if (m_lCurrent != lCurrFile)
//	{
//		// LOG_ERROR("[FileRead] Changing [%i]-[%i] - Error2", lCurrFile, m_lCurrent);
//		m_lCurrent == lCurrFile
//		_lseeki64(m_iFileHandle[fpType],m_iWpfStartPos + m_WpfHeader.wHeaderSize + itr->second.m_pOpenFileFcb1[fpType]->dwStart * m_WpfHeader.wBytesPerBlock + itr->second.m_iOpenFilePointPos[fpType],SEEK_SET);	
//	}
//
//	if(NULL == m_pOpenFileFcb1[fpType] || dwSize <= 0 || m_iOpenFilePointPos[fpType] >= m_pOpenFileFcb1[fpType]->dwSize)
//	{
//		// LOG_ERROR("[FileRead] - Error3");
//		return 0;
//	}
//
//	if(itr->second.m_iOpenFilePointPos[fpType] + dwSize >= itr->second.m_pOpenFileFcb1[fpType]->dwSize)
//	{
//		dwSize = itr->second.m_pOpenFileFcb1[fpType]->dwSize - itr->second.m_iOpenFilePointPos[fpType];
//	}
//
//	if (m_bCompress)
//	{
//		DWORD dwUncompressSize = 0;
//		BYTE* pByteData = AcquireFileUnCOmpressData(lCurrFile, itr->second, fpType, dwUncompressSize);
//		memcpy(pBbuff, pByteData, dwUncompressSize);
//		DWORD dwRtn = dwUncompressSize;
//		return dwRtn;
//	}
//	else
//	{
//		DWORD dwRtn = _read(m_iFileHandle[fpType],pBbuff,dwSize);
//		if (dwRtn > 0)
//		{
//			itr->second.m_iOpenFilePointPos[fpType] += dwRtn;
//		}
//		return dwRtn;
//	}
//}
//
//BYTE* sdWpfLoader::AcquireFileUnCOmpressData(long lCurrFile, const LoadingFileData& LoadingFileDataObject, E_FPTYPE fpType, DWORD& dwUnCompressSize)
//{
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//}
//
//DWORD sdWpfLoader::FileLength(long lCurrFile, E_FPTYPE fpType)
//{
//	sdCriticalSection2(m_lock);
//
//	//LOG_TEXT("[FileLength]");
//
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		// LOG_ERROR("[FileLength]");
//		return 0;
//	}
//
//	if(NULL == itr->second.m_pOpenFileFcb1[fpType])
//	{
//		// LOG_ERROR("[FileLength]");
//		return 0;
//	}
//
//	if (m_bCompress)
//	{
//		DWORD dwUncompressSize = 0;
//		BYTE* pByteData = AcquireFileUnCOmpressData(lCurrFile, itr->second, fpType, dwUncompressSize);
//		memcpy(pBbuff, pByteData, dwUncompressSize);
//		return dwUncompressSize;
//	}
//
//	return itr->second.m_pOpenFileFcb1[fpType]->dwSize;
//}
//
//__int64 sdWpfLoader::GetFileOffset(long lCurrFile, E_FPTYPE fpType)
//{
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		return -1;
//	}
//
//	if(NULL == m_pOpenFileFcb1[fpType])
//	{
//		return -1;
//	}
//
//	return m_iWpfStartPos + m_WpfHeader.wHeaderSize + itr->second.m_pOpenFileFcb1[fpType]->dwStart * m_WpfHeader.wBytesPerBlock;
//}
//
//bool sdWpfLoader::WpfLoadFat()
//{
//	if(!IsOpenWpfFile())
//	{
//		m_eWrLastError = EWR_FILE_NOT_OPEN;
//		return false;
//	}
//
//	if(m_WpfHeader.dwDirCount < 1)
//	{
//		m_eWrLastError = EWR_NOT_WPF_FILE;
//		return false;
//	}
//
//	_lseeki64(m_iFileHandle[EFT_MAIN],m_iWpfStartPos + m_WpfHeader.iFatPos,SEEK_SET);
//
//	DWORD dwCount = m_WpfHeader.dwDirCount + m_WpfHeader.dwFileCount;
//
//	m_pFcb1List = new FCB1[dwCount];
//	_read(m_iFileHandle[EFT_MAIN],m_pFcb1List,dwCount * FCB1_SIZE);
//
//	// 要求构造目录结构
//	if (m_dwOpenType & EWOT_LISTDIR)
//	{
//		m_pFcb2List = new FCB2[dwCount];
//		_read(m_iFileHandle[EFT_MAIN],m_pFcb2List,dwCount * FCB2_SIZE);
//
//		m_pFcbList = new FCB[dwCount];
//		memset(m_pFcbList,0,dwCount * sizeof(FCB));
//
//		m_pDirList = new WpfFileList[m_WpfHeader.dwDirCount];
//
//		//递归构造目录
//		vector<PWpfFileList> VDirs;
//		m_pWpfRoot = &(m_pDirList[0]);
//		m_pWpfRoot->pfcb = &(m_pFcbList[0]);
//		m_pWpfRoot->pfcb->pfcb1 = &(m_pFcb1List[0]);
//		m_pWpfRoot->pfcb->pfcb2 = &(m_pFcb2List[0]);
//		VDirs.push_back(m_pWpfRoot);
//
//		DWORD dwDirNo = 1,dwPos = 0;
//		PWpfFileList pFront = NULL;
//		while(!VDirs.empty())
//		{
//			pFront = VDirs.front();
//
//			dwPos = pFront->pfcb->pfcb1->dwStart;
//			for (DWORD i = 0; i < pFront->pfcb->pfcb1->dwSize; i++,dwPos++)
//			{
//				PFCB2 fcb2 = &(m_pFcb2List[dwPos]);
//				if(fcb2->dwAttribute & EFA_DIR)
//				{
//					PWpfFileList sublist = &(m_pDirList[dwDirNo]);
//					dwDirNo ++;
//					PFCB newfcb = &(m_pFcbList[dwPos]);
//					newfcb->pfcb1 = &(m_pFcb1List[dwPos]);
//					newfcb->pfcb2 = &(m_pFcb2List[dwPos]);
//					newfcb->pParent = pFront;
//					sublist->pfcb = newfcb;
//
//					VDirs.push_back(sublist);
//					pFront->MDirs[fcb2->strName] = sublist;
//
//				}
//				else if (fcb2->dwAttribute & EFA_FILE)
//				{
//					PFCB newfcb = &(m_pFcbList[dwPos]);
//					newfcb->pfcb1 = &(m_pFcb1List[dwPos]);
//					newfcb->pfcb2 = &(m_pFcb2List[dwPos]);
//					newfcb->pParent = pFront;
//
//					pFront->MFiles[fcb2->strName] = newfcb;
//				}
//			}
//
//			VDirs.erase(VDirs.begin());
//		}
//	}
//
//	if (m_dwOpenType & EWOT_READONLY)
//	{
//		// hash map
//		PFCB1 pfcb1 = m_pFcb1List;
//		for (DWORD i = 0; i < dwCount; ++i, ++pfcb1)
//		{
//			m_MhashFcb1[pfcb1->iHashKey] = pfcb1;
//		}
//	}
//
//	return true;
//}
//
//bool sdWpfLoader::IsOpenWpfFile()
//{
//	return (m_iFileHandle[EFT_MAIN] != -1);
//}
//
//bool sdWpfLoader::InitBuffer()
//{
//	DWORD seed = 0x00100001;
//	DWORD index1 = 0;
//	DWORD index2 = 0;
//	DWORD temp1 = 0,temp2 = 0;
//	int i = 0;
//
//	memset(m_dwCryptTable,0,sizeof(m_dwCryptTable));
//	for(index1 = 0; index1 < 0x100; index1++)
//	{
//		for (index2 = index1,i = 0; i < 5; i++,index2 += 0x100)
//		{
//			seed = (seed * 125 + 3)  % 0x2AAAAB;
//			temp1 = (seed & 0xFFFF) << 0x10;
//			seed = (seed * 125 + 3)  % 0x2AAAAB;
//			temp1 = (seed & 0xFFFF);
//			m_dwCryptTable[index2] = (temp1 | temp2);			
//		}
//	}
//
//	return true;
//}
//
//inline DWORD sdWpfLoader::HashString(const char * pStr,DWORD type)
//{
//	DWORD seed1 = 0x7FED7FED;
//	DWORD seed2 = 0xEEEEEEEE;
//	DWORD ch;			/* One key character */
//
//	while(*pStr != 0)
//	{
//		ch = toupper(*pStr++);
//		seed1 = m_dwCryptTable[(type << 8) + ch] ^ (seed1 + seed2);
//		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
//	}
//
//	return seed1;	
//}
//
//inline __int64 sdWpfLoader::HashFileName(const char * strFileName)
//{
//	DWORD o1 = HashString(strFileName,1);
//	DWORD o2 = HashString(strFileName,2);
//
//	return ( (((__int64)(o1)) << 32) + o2);
//}
//
//bool sdWpfLoader::IsFileExist(const char* strPathName)
//{
//	return m_MhashFcb1.find(HashFileName(strPathName)) != m_MhashFcb1.end();
//}
//
//size_t sdWpfLoader::FileTell(long lCurrFile, E_FPTYPE fpType)
//{
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		// LOG_ERROR
//		assert(0);
//		return 0;
//	}
//	return itr->second.m_iOpenFilePointPos[fpType];
//}
//
//bool sdWpfLoader::FileEof(long lCurrFile, E_FPTYPE fpType)
//{
//	LoadingFileDataMap::iterator itr = m_mapLoadingFileData.find(lCurrFile);
//	if (itr == m_mapLoadingFileData.end())
//	{
//		// LOG_ERROR
//		assert(0);
//		return 0;
//	}
//	return itr->second.m_iOpenFilePointPos[fpType] >= itr->second.m_pOpenFileFcb1[fpType]->dwSize;
//}
//
//sdWpfLoader* CreateWpfInterface()
//{
//	return new sdWpfLoader();
//}
//
//void DestroyWpfInterface(sdWpfLoader*& pkWpfInterface)
//{
//	if (pkWpfInterface)
//	{
//		delete pkWpfInterface;
//		pkWpfInterface = NULL;
//	}
//}