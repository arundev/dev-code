//*************************************************************************************************
// 内容:		原子操作(参考自SGDP::sdu)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-10
// 最后修改:	2014-01-10
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_ATOMIC_H__
#define _SD_BASE_THREADING_ATOMIC_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-------------------------------------------------------------------------------------------------
// 原子操作
//-------------------------------------------------------------------------------------------------
// @brief 在内存中原子读取一个32位值
// @param[in] piMem	指向需要被读取值的指针
// @return			读取的值
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT32 SDAtomicRead32(volatile INT32 * piMem)
{
	return *piMem;
}
//-------------------------------------------------------------------------------------------------
// @brief 在内存中原子读取一个64位值
// @param[in] piMem	指向需要被读取值的指针
// @return			读取的值
//-------------------------------------------------------------------------------------------------
SD_FORCE_INLINE INT64 SDAtomicRead64(volatile INT64 * piMem)
{
	return *piMem;
}
//-------------------------------------------------------------------------------------------------
// @brief 在内存中原子设置一个32位值
// @param[in] piMem		指向需要设置值的指针
// @param[in] dwValue	需要设置的值
// @return				读取的值
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
// @brief 32位原子加法，将*piMem值添加一个iValue值
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem被加之前的原始值
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
// @brief 64位原子加法，将*piMem值添加一个iValue值
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem被加之前的原始值
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
// @brief 32位原子减法，将*piMem值减去一个iValue值
// @param[in,out]	piMem
// @param[in]		iValue	
// @return					*piMem被减之前的原始值
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
// @brief 64位原子减法，将*piMem值减去一个iValue值
// @param[in,out]	piMem
// @param[in]		iValue	
// @return			*piMem被减之前的原始值
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
// @brief 32位原子自加算法
// @param[in,out]	piMem
// @return			*piMem自加之后值
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
// @brief 64位原子自加算法
// @param[in,out]	piMem
// @return			*piMem自加之后值
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
// @brief 32位原子自减算法
// @param[in,out]	piMem
// @return			*piMem自减之后的值
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
// @brief	64位原子减一
// @param[in, out]	pqwMem	指向被减数的指针
// @return					减去之后的值
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
// @brief 交换算法，交换*piMem和iValue的值
// @param[in,out]	piMem	指向被比较的值的指针
// @param[in]		iValue	交换的值
// @return					*piMem被交换值之前的原始值
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
// @brief 交换算法，交换*piMem和iValue的值
// @param[in,out]	piMem	指向被比较的值的指针
// @param[in]		iValue	交换的值
// @return			*piMem被交换值之前的原始值
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
// @brief 32位原子的CAS(Compare and Swap)算法，
//		将*piMem的值与iCmp作比较，如果一致，则将iValue的值赋给*piMem
// @param[in,out]	piMem	指向被比较的值的指针
// @param[in]		iValue	若*piMem与iCmp一致，被赋值的值
// @param[in]		iCmp	比较的值
// @return					*piMem被赋值之前的原始值
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
// @brief 64位原子的CAS(Compare and Swap)算法，
//		将*piMem的值与iCmp作比较，如果一致，则将iValue的值赋给*piMem
// @param[in,out]	piMem	指向被比较的值的指针
// @param[in]		iValue	若*piMem与iCmp一致，被赋值的值
// @param[in]		iCmp	比较的值
// @return					*piMem被赋值之前的原始值
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
// @brief 原子的指针的交换算法
// @param[in,out]	ppvPtr	指向被比较的值的指针
// @param[in,out]	pvWith	交换的值
// @return					*ppvPtr被交换值之前的原始值
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
// @brief 原子的指针的CAS(Compare and Swap)算法，
//		将ppvMem指向的指针与pvCmp指针作比较，如果一直，则将ppvMem指向的指针与pvValue指针交换
// @param[in,out]	ppvMem
// @param[in out]	pvValue	
// @param[in]		pvCmp	
// @return					*ppvPtr被交换之前的原始值
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