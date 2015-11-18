//*************************************************************************************************
// ����:	CheckSum��CRC32����(�ο���WZ::MicroClient::MfileSystem::Common)
//
// �ο����ϣ�
//	http://en.wikipedia.org/wiki/Error_detection_and_correction
//	http://en.wikipedia.org/wiki/Cyclic_redundancy_check
//
//---------------------------------------------------------
// ����:	
// ����:		2012-12-28
// ����޸�:	2012-12-31
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_SECURITY_CRYPTOGRAPHY_CRC32_H__
#define _SD_BASE_SECURITY_CRYPTOGRAPHY_CRC32_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_SECURITY
BASE_NAMESPACE_BEGIN_CRYPTOGRAPHY
//-------------------------------------------------------------------------------------------------
class sdCRC32
{
public:
	static DWORD Gen(char* pcData, DWORD dwLen);
	static DWORD Gen(char* pcData, DWORD dwLen, DWORD dwLastData);
	static DWORD Gen(const char* szFileName);
};
//-------------------------------------------------------------------------------------------------
// \brief ��У��(FreeBSD cksum\csum1.c)
//
//	@remark	��У��
//	@param[in] pucBuff	��������
//  @param[in] uiSize	�������ݳߴ�
//	@return				��У��	
//	@see
//	@note
//-------------------------------------------------------------------------------------------------
uint CheckSum(const char* pcBuff, uint uiSize);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_CRYPTOGRAPHY
BASE_NAMESPACE_END_SECURITY
BASE_NAMESPACE_END_BASE
#endif