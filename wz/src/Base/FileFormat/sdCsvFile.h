//*************************************************************************************************
// 内容:	csv文件加载与保存
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_IO_CSV_FILE_H__
#define _SD_BASE_IO_CSV_FILE_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_IO

class sdCsvFile
{
public:
	virtual ~sdCsvFile(){};

	// 打开文件
	void	Open(const std::string& szFullPath){};
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_IO
#endif