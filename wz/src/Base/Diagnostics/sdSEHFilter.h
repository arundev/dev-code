//*************************************************************************************************
// ����:	Windows SEH����׽(�ο���WZ::Global)
//---------------------------------------------------------
// ����:	
// ����:		2012-12-28
// ����޸�:	2012-12-31
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
		sprintf_s(pcBuff, iBufSize, "�洢�����쳣, �������: %x\n", code);
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		sprintf_s(pcBuff, iBufSize, "�洢����δ�����쳣, �������: %x\n", code);
		break;
	case EXCEPTION_BREAKPOINT:
		sprintf_s(pcBuff, iBufSize, "�ж��쳣, �������: %x\n", code);
		break;
	case EXCEPTION_SINGLE_STEP:
		sprintf_s(pcBuff, iBufSize, "�����ж��쳣, �������: %x\n", code);
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		sprintf_s(pcBuff, iBufSize, "����Խ���쳣, �������: %x\n", code);
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_INEXACT_RESULT:
	case EXCEPTION_FLT_INVALID_OPERATION:
	case EXCEPTION_FLT_OVERFLOW:
	case EXCEPTION_FLT_STACK_CHECK:
	case EXCEPTION_FLT_UNDERFLOW:
		sprintf_s(pcBuff, iBufSize, "�����������쳣, �������: %x\n", code);
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		sprintf_s(pcBuff, iBufSize, "�����쳣, �������: %x\n", code);
		break;
	case EXCEPTION_INT_OVERFLOW:
		sprintf_s(pcBuff, iBufSize, "��������쳣, �������: %x\n", code);
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		sprintf_s(pcBuff, iBufSize, "ҳ�����쳣, �������: %x\n", code);
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		sprintf_s(pcBuff, iBufSize, "�Ƿ�ָ���쳣, �������: %x\n", code);
		break;
	case EXCEPTION_STACK_OVERFLOW:
		sprintf_s(pcBuff, iBufSize, "��ջ����쳣, �������: %x\n", code);
		break;
	case EXCEPTION_INVALID_HANDLE:
		sprintf_s(pcBuff, iBufSize, "��Ч����쳣, �������: %x\n", code);
		break;
	default:
		if (code & (1<<29))
			sprintf_s(pcBuff, iBufSize, "�û��Զ�������쳣, �������: %x\n", code);
		else
			sprintf_s(pcBuff, iBufSize, "�����쳣, �������: %x\n", code);
		break;
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif