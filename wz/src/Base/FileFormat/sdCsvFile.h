//*************************************************************************************************
// ����:	csv�ļ������뱣��
//---------------------------------------------------------
// ����:		
// ����:		2012-09-05
// ����޸�:
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

	// ���ļ�
	void	Open(const std::string& szFullPath){};
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_IO
#endif