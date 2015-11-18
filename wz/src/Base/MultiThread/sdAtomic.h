//*************************************************************************************************
// ����:		ԭ�Ӳ���(�ο���SGDP::sdu)
//---------------------------------------------------------
// ����:		
// ����:		2014-01-10
// ����޸�:	2014-01-10
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_ATOMIC_H__
#define _SD_BASE_THREADING_ATOMIC_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-------------------------------------------------------------------------------------------------
// ԭ�Ӳ���
//-------------------------------------------------------------------------------------------------
// @brief ���ڴ���ԭ�Ӷ�ȡһ��32λֵ
// @param[in] piMem	ָ����Ҫ����ȡֵ��ָ��
// @return			��ȡ��ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicRead32(volatile INT32 * piMem)
{
	return *piMem;
}
//-------------------------------------------------------------------------------------------------
// @brief ���ڴ���ԭ�Ӷ�ȡһ��64λֵ
// @param[in] piMem	ָ����Ҫ����ȡֵ��ָ��
// @return			��ȡ��ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicRead64(volatile INT64 * piMem)
{
	return *piMem;
}
//-------------------------------------------------------------------------------------------------
// @brief ���ڴ���ԭ������һ��32λֵ
// @param[in] piMem		ָ����Ҫ����ֵ��ָ��
// @param[in] dwValue	��Ҫ���õ�ֵ
// @return				��ȡ��ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE void SDAtomicSet32(volatile INT32 * piMem, INT32 iValue)
{
#if (defined(WIN32) || defined(WIN64))
	::InterlockedExchange((volatile LONG *)piMem, iValue);
#else
	*piMem = dwValue
#endif
}
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE void SDAtomicSet64(volatile INT64* piMem, INT64 iValue)
{
#if defined(WIN64)
	::InterlockedExchange64(piMem, iValue);
#else
	*piMem = iValue;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 32λԭ�Ӽӷ�����*piMemֵ���һ��iValueֵ
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem����֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicAdd32(volatile INT32 * piMem, INT32 iValue)
{
//#if (defined(WIN32) || defined(WIN64))
	return (INT32)::InterlockedExchangeAdd((volatile LONG *)piMem, (LONG)iValue);
//#else
//	//asm volatile("lock; xadd1 %0,%1"
//	//	: "=r" (iValue), "=m" (*piMem)
//	//	: "0" (iValue), "m" (*piMem)
//	//	: "memory", "cc");
//	//return iValue;
//#endif
}
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE UINT32 SDAtomicAdd32(volatile UINT32 * puiMem, UINT32 uiValue)
{
	return (UINT32)::InterlockedExchangeAdd((volatile LONG *)puiMem, (LONG)uiValue);
}
//-------------------------------------------------------------------------------------------------
// @brief 64λԭ�Ӽӷ�����*piMemֵ���һ��iValueֵ
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem����֮ǰ��ԭʼֵ
//
//	asm volatile("lock; xaddq %0,%1"
//	: "=r" (iValue), "=m" (*piMem)
//	: "0" (iValue), "m" (*piMem)
//	: "memory", "cc");
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicAdd64(volatile INT64 * piMem, INT64 iValue)
{
#if defined(WIN64)
	return ::InterlockedExchangeAdd64(piMem, iValue);
#else
	return *piMem;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 32λԭ�Ӽ�������*piMemֵ��ȥһ��iValueֵ
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem����֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicSub32(volatile INT32 * piMem, INT32 iValue)
{
//#if (defined(WIN32) || defined(WIN64))
	return (INT32)::InterlockedExchangeAdd((volatile LONG *)piMem, (LONG)-iValue);
//#else
//	//asm volatile("lock; subl %1,%0"
//	//	: /* no output */
//	//	: "m" (*piMem), "r" (iValue)
//	//	: "memory", "cc");
//	//return *pdwMem + 1;
//#endif
}
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE UINT32 SDAtomicSub32(volatile UINT32 * puiMem, UINT32 uiValue)
{
	return (UINT32)::InterlockedExchangeAdd((volatile LONG *)puiMem, -((LONG)uiValue));
}
//-------------------------------------------------------------------------------------------------
// @brief 64λԭ�Ӽ�������*piMemֵ��ȥһ��iValueֵ
// @param[in,out]	piMem
// @param[in]		iValue	
// @return			*piMem����֮ǰ��ԭʼֵ
//
//	asm volatile("lock; subq %0,%1"
//	: /* no output */
//	: "m" (*piMem), "r" (iValue)
//	: "memory", "cc");
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicSub64(volatile INT64 * piMem, INT64 iValue)
{
#if defined(WIN64)
	return ::InterlockedExchangeAdd64(piMem, -iValue);
#else
	return *piMem;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 32λԭ���Լ��㷨
// @param[in,out]	piMem
// @return			*piMem�Լ�֮��ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicInc32(volatile INT32 * piMem)
{
#if (defined(WIN32) || defined(WIN64))
	return (INT32)::InterlockedIncrement((volatile LONG *)piMem);
#else
	return SDAtomicAdd32(piMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE UINT32 SDAtomicInc32(volatile UINT32 * puiMem)
{
#if (defined(WIN32) || defined(WIN64))
	return (UINT32)::InterlockedIncrement((volatile LONG *)puiMem);
#else
	return SDAtomicAdd32(puiMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 64λԭ���Լ��㷨
// @param[in,out]	piMem
// @return			*piMem�Լ�֮��ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicInc64(volatile INT64 * piMem)
{
#if defined(WIN64)
	return ::InterlockedIncrement64(piMem);
#else
	return SDAtomicAdd64(piMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 32λԭ���Լ��㷨
// @param[in,out]	piMem
// @return			*piMem�Լ�֮���ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicDec32(volatile INT32 * piMem)
{
#if (defined(WIN32) || defined(WIN64))
	return (INT32)::InterlockedDecrement((volatile LONG *)piMem);
#else
	return SDAutomicSub32(piMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicDec32(volatile UINT32 * puiMem)
{
#if (defined(WIN32) || defined(WIN64))
	return (UINT32)::InterlockedDecrement((volatile LONG *)puiMem);
#else
	return SDAutomicSub32(puiMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief	64λԭ�Ӽ�һ
// @param[in, out]	pqwMem	ָ�򱻼�����ָ��
// @return					��ȥ֮���ֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicDec64(volatile INT64 * piMem)
{
#if defined(WIN64)
	return ::InterlockedDecrement64(piMem);
#else
	return SDAtomicSub64(piMem, 1);
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief �����㷨������*piMem��iValue��ֵ
// @param[in,out]	piMem	ָ�򱻱Ƚϵ�ֵ��ָ��
// @param[in]		iValue	������ֵ
// @return					*piMem������ֵ֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicXchg32(volatile INT32 * piMem, INT32 iValue)
{
#if (defined(WIN32) || defined(WIN64))
	return ::InterlockedExchange((volatile LONG*)piMem, iValue);
#else
	UINT32 uiPrev = iValue;
	asm volatile("xchgl %0, %1"
		: "=r" (uiPrev), "+m" (*piMem)
		: "0" (prev));
	return uiPrev;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief �����㷨������*piMem��iValue��ֵ
// @param[in,out]	piMem	ָ�򱻱Ƚϵ�ֵ��ָ��
// @param[in]		iValue	������ֵ
// @return			*piMem������ֵ֮ǰ��ԭʼֵ
//
// 	UINT64 uiPrev = iValue;
//	asm volatile("xchg %0, %1"
//	: "=r" (uiPrev), "+m" (*piMem)
//	: "0" (prev));
//	return uiPrev;
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicXchg64(volatile INT64 * piMem, INT64 iValue)
{
#if defined(WIN64)
	return ::InterlockedExchange64(piMem, iValue);
#else
	return *piMem;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 32λԭ�ӵ�CAS(Compare and Swap)�㷨��
//		��*piMem��ֵ��iCmp���Ƚϣ����һ�£���iValue��ֵ����*piMem
// @param[in,out]	piMem	ָ�򱻱Ƚϵ�ֵ��ָ��
// @param[in]		iValue	��*piMem��iCmpһ�£�����ֵ��ֵ
// @param[in]		iCmp	�Ƚϵ�ֵ
// @return					*piMem����ֵ֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicCas32(volatile INT32 * piMem, INT32 iValue, INT32 iCmp)
{
#if (defined(WIN32) || defined(WIN64))
	return ::InterlockedCompareExchange((volatile LONG *)piMem, iValue, iCmp);
#else
	//UINT32 uiPrev;
	//asm volatile("lock; cmpxchgl %1,%2"
	//	: "=a" (uiPrev)
	//	: "r" (iValue), "m" (*piMem), "0" (iCmp)
	//	: "memory", "cc");
	//return uiPrev;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief 64λԭ�ӵ�CAS(Compare and Swap)�㷨��
//		��*piMem��ֵ��iCmp���Ƚϣ����һ�£���iValue��ֵ����*piMem
// @param[in,out]	piMem	ָ�򱻱Ƚϵ�ֵ��ָ��
// @param[in]		iValue	��*piMem��iCmpһ�£�����ֵ��ֵ
// @param[in]		iCmp	�Ƚϵ�ֵ
// @return					*piMem����ֵ֮ǰ��ԭʼֵ
//
// 	UINT64 uiPrev;
//	asm volatile("lock; cmpxchgq %1,%2"
//		: "=a" (uiPrev)
//		: "r" (iValue), "m" (*piMem), "0" (iCmp)
//		: "memory", "cc");
//	return uiPrev;
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicCas64(volatile INT64* piMem, INT64 iValue, INT64 iCmp)
{
#if defined(WIN64)
	return ::InterlockedCompareExchange64(piMem, iValue, iCmp);
#else
	return *piMem;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief ԭ�ӵ�ָ��Ľ����㷨
// @param[in,out]	ppvPtr	ָ�򱻱Ƚϵ�ֵ��ָ��
// @param[in,out]	pvWith	������ֵ
// @return					*ppvPtr������ֵ֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE VOID* SDAtomicXchgPtr(VOID * volatile * ppvPtr, VOID * pvWith)
{
#if (defined(WIN32) || defined(WIN64))
	return InterlockedExchangePointer((VOID**)ppvPtr, pvWith);
#else
	//void* pvPrev;
	//asm volatile("xchgl %2, %1"
	//	: "=a" (pvPrev), "+m" (*ppvPtr)
	//	: "0" (pvWith));
	//return pvPrev;
	//
	//void* pvPrev;
	//asm volatile("xchgq %2, %1"
	//	: "=a" (pvPrev), "+m" (*ppvPtr)
	//	: "0" (pvWith));
	//return pvPrev;
#endif
}
//-------------------------------------------------------------------------------------------------
// @brief ԭ�ӵ�ָ���CAS(Compare and Swap)�㷨��
//		��ppvMemָ���ָ����pvCmpָ�����Ƚϣ����һֱ����ppvMemָ���ָ����pvValueָ�뽻��
// @param[in,out]	ppvMem
// @param[in out]	pvValue	
// @param[in]		pvCmp	
// @return					*ppvPtr������֮ǰ��ԭʼֵ
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE VOID* SDAtomicCasPtr(VOID * volatile * ppvMem, VOID * pvValue, VOID * pvCmp)
{
#if (defined(WIN32) || defined(WIN64))
	return ::InterlockedCompareExchangePointer((VOID * volatile *)ppvMem, pvValue, pvCmp);
#else
	//VOID* pvPrev;
	//asm volatile("lock; cmpxchgl %2,%1"
	//	: "=a" (pvPrev) "=m" (*ppvMem)
	//	: "r" (pvValue), "m" (*ppvMem), "0" (pvCmp));
	//return uiPrev;
	//
	//VOID* pvPrev;
	//asm volatile("lock; cmpxchgq %2,%1"
	//	: "=a" (pvPrev) "=m" (*ppvMem)
	//	: "r" (pvValue), "m" (*ppvMem), "0" (pvCmp));
	//return uiPrev;
#endif
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif