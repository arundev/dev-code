//*************************************************************************************************
// 内容:	ini文件加载与保存
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_IO_INI_FILE_H__
#define _SD_BASE_IO_INI_FILE_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_IO

// 包装Win32关于ini文件的读取与保存
// (Win32是直接同步读取和保存Ini文件的,所以这里只是简单地API封装)
class sdIniFile
{
public:
	virtual ~sdIniFile(){};

	// 打开文件
	void	Open(const std::string& szFullPath);

	// 读取与保存
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