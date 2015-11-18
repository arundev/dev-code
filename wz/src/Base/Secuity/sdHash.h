//*************************************************************************************************
// 内容:	Hash计算
//
// 参考资料：
//		http://en.wikipedia.org/wiki/Hash_function		
//		http://en.wikipedia.org/wiki/Cryptographic_hash_function
//
//		http://blog.csdn.net/hqd_acm/article/details/5901955
//		http://blog.chinaunix.net/uid-20737871-id-1881210.html
//	
//---------------------------------------------------------
// 作者:	
// 创建:		2014-01-15
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_SECURITY_CRYPTOGRAPHY_HASH_H__
#define _SD_BASE_SECURITY_CRYPTOGRAPHY_HASH_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_SECURITY
BASE_NAMESPACE_BEGIN_CRYPTOGRAPHY
//-------------------------------------------------------------------------------------------------
// \brief 计算哈希值
//
//	@remark	计算哈希值
//	@param[in] pcStr	输入字符串
//	@return				哈希值	
//	@see
//	@note
//-------------------------------------------------------------------------------------------------
uint SDAPHash(const char* pcStr);
//-------------------------------------------------------------------------------------------------
// \brief 计算哈希值
//
//	@remark	计算哈希值
//	@param[in] pcStr	输入字符串
//	@return				哈希值	
//	@see
//	@note
//-------------------------------------------------------------------------------------------------
uint SDBKDRHash(const char* pcStr);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_CRYPTOGRAPHY
BASE_NAMESPACE_END_SECURITY
BASE_NAMESPACE_END_BASE
#endif