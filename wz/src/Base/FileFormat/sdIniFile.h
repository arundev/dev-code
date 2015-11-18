//*************************************************************************************************
// ����:	ini�ļ������뱣��
//---------------------------------------------------------
// ����:		
// ����:		2012-09-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_IO_INI_FILE_H__
#define _SD_BASE_IO_INI_FILE_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_IO

// ��װWin32����ini�ļ��Ķ�ȡ�뱣��
// (Win32��ֱ��ͬ����ȡ�ͱ���Ini�ļ���,��������ֻ�Ǽ򵥵�API��װ)
class sdIniFile
{
public:
	virtual ~sdIniFile(){};

	// ���ļ�
	void	Open(const std::string& szFullPath);

	// ��ȡ�뱣��
	char*	ReadConfigString(const char* szSection, const char* szKey, const char* szDefault = "");
	int		ReadConfigInt(const char* szSection, const char* szKey, int iDefault = 0);
	bool	ReadConfigBoolean(const char* szSection, const char* szKey, bool bDefault = false);
	float	ReadConfigFloat(const char* szSection, const char* szKey, float fDefault = 0);
	
	void	WriteConfigString(const char* szSection, const char* szKey, const char* szValue);
	void	WriteConfigBoolean(const char* szSection, const char* szKey, bool bValue);
	void	WriteConfigInt(const char* szSection, const char* szKey, int iValue);
	void	WriteConfigFloat(const char* szSection, const char* szKey, float fValue);	

protected:
	std::string m_szFullPath;
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_IO
#endif