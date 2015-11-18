//*************************************************************************************************
// 内容:	用于辅助执行编译检查(参考自WZ::Global::sdStaticAsseet)
//---------------------------------------------------------
// 作者:	
// 创建:		2012-07-24
// 最后修改:	2013-02-04
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_STATIC_ASSERT_H__
#define _SD_BASE_DIAGNOSTICS_STATIC_ASSERT_H__

#pragma warning(push)
#pragma warning(error:4200)

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
template <int iBoolValue>
struct sdStaticAssert
{
	int iTmp[iBoolValue];
};

template <int iValue, int iRef>
struct sdStaticAssertLess
{
	sdStaticAssert<iRef - iValue> kTmp;
};

template <int iValue, int iRef>
struct sdStaticAssertMore
{
	sdStaticAssert<iValue - iRef> kTmp;
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE

#pragma warning(pop)
#endif