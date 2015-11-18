//*************************************************************************************************
// 内容:	Windows SEH错误捕捉(参考自WZ::Global)
//---------------------------------------------------------
// 作者:	
// 创建:		2012-12-28
// 最后修改:	2012-12-31
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_SEH_FILTER_H__
#define _SD_BASE_DIAGNOSTICS_SEH_FILTER_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
// 1.Structured Exception Handling(SEH)
// 2.Vectored Exception Handling(VEH)
//-------------------------------------------------------------------------------------------------
inline int seh_filter(int code, char* pcBuff, int iBufSize)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		sprintf_s(pcBuff, iBufSize, "存储保护异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		sprintf_s(pcBuff, iBufSize, "存储类型未对齐异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_BREAKPOINT:
		sprintf_s(pcBuff, iBufSize, "中断异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_SINGLE_STEP:
		sprintf_s(pcBuff, iBufSize, "单步中断异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		sprintf_s(pcBuff, iBufSize, "数组越界异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_INEXACT_RESULT:
	case EXCEPTION_FLT_INVALID_OPERATION:
	case EXCEPTION_FLT_OVERFLOW:
	case EXCEPTION_FLT_STACK_CHECK:
	case EXCEPTION_FLT_UNDERFLOW:
		sprintf_s(pcBuff, iBufSize, "浮点数计算异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		sprintf_s(pcBuff, iBufSize, "除零异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_INT_OVERFLOW:
		sprintf_s(pcBuff, iBufSize, "数据溢出异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		sprintf_s(pcBuff, iBufSize, "页错误异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		sprintf_s(pcBuff, iBufSize, "非法指令异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_STACK_OVERFLOW:
		sprintf_s(pcBuff, iBufSize, "堆栈溢出异常, 错误代码: %x\n", code);
		break;
	case EXCEPTION_INVALID_HANDLE:
		sprintf_s(pcBuff, iBufSize, "无效句柄异常, 错误代码: %x\n", code);
		break;
	default:
		if (code & (1<<29))
			sprintf_s(pcBuff, iBufSize, "用户自定义软件异常, 错误代码: %x\n", code);
		else
			sprintf_s(pcBuff, iBufSize, "其他异常, 错误代码: %x\n", code);
		break;
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif