#include "sdBasePCH.h"
#include "sdHash.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_SECURITY
BASE_NAMESPACE_BEGIN_CRYPTOGRAPHY
//-------------------------------------------------------------------------------------------------
uint SDAPHash(const char* pcStr)
{
	uint uiHash = 0;

	for (int i = 0; *pcStr; ++i)
	{
		if ((i & 1) == 0)
			uiHash ^= ((uiHash << 7) ^ (*pcStr++) ^ (uiHash >> 3));
		else
			uiHash ^= (~((uiHash << 11) ^ (*pcStr++) ^ (uiHash >> 5)));

	}

	return (uiHash & 0x7fffffff);
}
//-------------------------------------------------------------------------------------------------
uint BKDRHash(const char* pcStr)
{
	uint uiSeed = 131;	///< 31 131 1313 13131 131313 etc.
	uint uiHash = 0;

	while (*pcStr)
		uiHash = uiHash * uiSeed + (*pcStr++);

	return (uiHash & 0x7fffffff);
}
//-------------------------------------------------------------------------------------------------
//__int64 SDStringHash(const char* pcStr)
//{
//	uint uiHash1 = SDBKDRHash(pcStr);
//	uint uiHash2 = SDAPHash(pcStr);
//	
//	__int64 uiHash = uiHash1;
//	uiHash = uiHash << 32 | uiHash2;
//
//	return uiHash;
//
//}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_CRYPTOGRAPHY
BASE_NAMESPACE_END_SECURITY
BASE_NAMESPACE_END_BASE