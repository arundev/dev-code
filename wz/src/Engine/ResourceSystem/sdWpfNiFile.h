//*************************************************************************************************
// 内容:	NiFile的WpfFileSystem实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-03
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_WPF_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_WPF_H__

namespace ResourceSystem
{
	//class sdWpfNiFile : public NiFile
	//{
	//	NiDeclareDerivedBinaryStream();
	//public:
	//	sdWpfNiFile(const char* pcName, OpenMode eMode, uint uiBufferSize = 32768);
	//	virtual ~sdWpfNiFile();
	//	
	//	// 虚函数继承
	//	// @{
	//	virtual operator bool() const { return !m_szFileName.empty();}

	//	// Wpf文件寻址
	//	virtual bool	Seek(int iNumBytes);
	//	virtual bool	Seek(int iOffset, int iWhence);

	//	// 文件信息
	//	virtual uint		GetFileSize() const;
	//	virtual const char*	GetBuffer();
	//	virtual bool		eof();
	//	virtual bool		IsGood();
	//	// @}

	//	// 静态初始化与反初始化
	//	static void Initialize();
	//	static void Destroy();

	//protected:
	//	// NiFile文件操作函数,用于指定给NiFile接口
	//	static NiFile* FileCreateFunc(const char *pcName, OpenMode eMode, uint uiBufferSize);
	//	static bool FileAccessFunc(const char *pcName, OpenMode eMode);
	//	static bool CreateDirFunc(const char* pcName);
	//	static bool DirectoryExist(const char* pcName);

	//	// D3DXEffectLoader操作函数
	//	static void D3DXEffectLoadFiles(NiTObjectArray<NiString>& pkFileList,
	//		const char* pcDirectory, const char* pcExt, bool bRecurseDirectories);

	//	// Wpf文件读写
	//	uint	MyFileRead(void* pvBuffer, ulong uiBytes);
	//	uint	MyFileWrite(const void* pvBuffer, ulong uiBytes);

	//protected:
	//	std::string	m_szFileName;
	//	uint		m_uiFileSize;
	//	//sdDataStreamBase
	//};
}
#endif