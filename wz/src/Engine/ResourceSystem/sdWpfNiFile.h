//*************************************************************************************************
// ����:	NiFile��WpfFileSystemʵ��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-03
// ����޸�:
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
	//	// �麯���̳�
	//	// @{
	//	virtual operator bool() const { return !m_szFileName.empty();}

	//	// Wpf�ļ�Ѱַ
	//	virtual bool	Seek(int iNumBytes);
	//	virtual bool	Seek(int iOffset, int iWhence);

	//	// �ļ���Ϣ
	//	virtual uint		GetFileSize() const;
	//	virtual const char*	GetBuffer();
	//	virtual bool		eof();
	//	virtual bool		IsGood();
	//	// @}

	//	// ��̬��ʼ���뷴��ʼ��
	//	static void Initialize();
	//	static void Destroy();

	//protected:
	//	// NiFile�ļ���������,����ָ����NiFile�ӿ�
	//	static NiFile* FileCreateFunc(const char *pcName, OpenMode eMode, uint uiBufferSize);
	//	static bool FileAccessFunc(const char *pcName, OpenMode eMode);
	//	static bool CreateDirFunc(const char* pcName);
	//	static bool DirectoryExist(const char* pcName);

	//	// D3DXEffectLoader��������
	//	static void D3DXEffectLoadFiles(NiTObjectArray<NiString>& pkFileList,
	//		const char* pcDirectory, const char* pcExt, bool bRecurseDirectories);

	//	// Wpf�ļ���д
	//	uint	MyFileRead(void* pvBuffer, ulong uiBytes);
	//	uint	MyFileWrite(const void* pvBuffer, ulong uiBytes);

	//protected:
	//	std::string	m_szFileName;
	//	uint		m_uiFileSize;
	//	//sdDataStreamBase
	//};
}
#endif