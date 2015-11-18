//*************************************************************************************************
// 内容:	调试宏定义(参考自Gamebryo::NiSystem::NiDebug)
//---------------------------------------------------------
// 作者:	
// 创建:		2014-03-25
// 最后修改:	2014-03-25
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_DEBUG_H__
#define _SD_BASE_DIAGNOSTICS_DEBUG_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
// SDDEBUGBREAKPROC
#define SDDEBUGBREAKPROC()			__debugbreak()		

// Causes a break in debugger, function always returns false
inline bool sdDebugBreakProc()		{ SDDEBUGBREAKPROC(); return false;}

// NIASSERT
typedef bool (*sdAssertFailedProcFuncPtr)(
	const char* szExpression,
	const char* szFile,
	const char* szFunction,
	const int iLine);

class sdAssertFail
{
public:
	// This assertion is the generic platform-specific assert hander
	static bool DefaultAssertFail(
		const char* szExpression,
		const char* szFile,
		const char* szFunction,
		const int iLine);

	// This assertion routine will simply write out a debug string.
	// It will not trigger a breakpoint.This is useful for running 
	// automated tests.
	static bool SimpleAssertFail(
		const char* szExpression,
		const char* szFile,
		const char* szFunction,
		const int iLine);

public:
	static sdAssertFailedProcFuncPtr ms_pfnAssertFailedProc;
};

// SDASSERTINLINE results in a boolean type,
// to enbale nesting into statement calls, such as NiVerifyStaticCast
#ifdef SD_DEBUG
	#define SDASSERTINLINE(boolean_expresion)	\
		(boolean_expresion) ? true :			\
		sdAssertFail::ms_pfnAssertFailedProc(
		#boolean_expresion, __FILE__, __FUNCTION__, __LINE__)
#else
	// Defaults to false to avoud misuse of form: ClientFunc(NIASSERT(...))
	// Use NIVERIFY instead in those situations
	#define SDASSERTINLINE(value)		false
#endif

// SDASSERT
#define SDASSERT(boolean_expresion)		((void)SDASSERTINLINE(boolean_expresion))

// SDVERIFY
#ifdef SD_DEBUG
	#define SDVERIFY(condition)			SDASSERTINLINE(condition)
#else 
	#define SDVERIFY(condition)			(condition)
#endif

// SDVERIFYNOTEQUALS
#ifdef SD_DEBUG
	#define SDVERIFYNOTEQUALS(always_computed_value, debug_verified_value)	\
		SDASSERTINLINE((always_computed_value) == (debug_verified_value))
#else 
	#define SDVERIFYNOTEQUALS(always_computed_value, debug_verified_value)	\
		(always_computed_value)
#endif

// SDDEBUGBREAK
#ifdef SD_DEBUG
	#define SDDEBUGBREAK()	sdDebugBreakProc()
#else
	#define SDDEBUGBREAK()	false
#endif
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif