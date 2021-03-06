//*************************************************************************************************
// 内容:	工厂接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-15
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MISC_IFACTORY_H__
#define _SD_BASE_MISC_IFACTORY_H__

#include "sdSingleton.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MISC
//-------------------------------------------------------------------------------------------------
// 注意: 
//	析构函数基本不会被调用,销毁应该写在Destroy里面
//-------------------------------------------------------------------------------------------------
class IFactory : public Base::sdTSingleton<IFactory>
{
public:
	virtual ~IFactory() = 0;

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MISC
BASE_NAMESPACE_END_BASE
#endif