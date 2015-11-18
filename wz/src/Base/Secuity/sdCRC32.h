//*************************************************************************************************
// 内容:	CheckSum和CRC32计算(参考自WZ::MicroClient::MfileSystem::Common)
//
// 参考资料：
//	http://en.wikipedia.org/wiki/Error_detection_and_correction
//	http://en.wikipedia.org/wiki/Cyclic_redundancy_check
//
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-28
// 最后修改:	2012-12-31
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
// \brief 检校和(FreeBSD cksum\csum1.c)
//
//	@remark	检校和
//	@param[in] pucBuff	输入数据
//  @param[in] uiSize	输入数据尺寸
//	@return				检校和	
//	@see
//	@note
//-------------------------------------------------------------------------------------------------
uint CheckSum(const char* pcBuff, uint uiSize);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_CRYPTOGRAPHY
BASE_NAMESPACE_END_SECURITY
BASE_NAMESPACE_END_BASE
#endif