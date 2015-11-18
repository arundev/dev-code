//*************************************************************************************************
// 内容:	手动释放资源接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-21
// 最后修改:
//*************************************************************************************************
#pragma once
#include "MMacros.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
// 在.NET的对象中实际上有两个用于释放资源的函数：Dispose和Finalize。
//		Finalize的目的是用于释放非托管的资源，
//		Dispose是用于释放所有资源，包括托管的和非托管的。
public ref class MDisposable abstract : public System::IDisposable
{
public:
	MDisposable();
	!MDisposable();			///< finalizer
	virtual ~MDisposable();	///< destructor calls finalizer

protected:
	// 释放资源的真正执行函数
	virtual void Do_Dispose(bool bDisposing){};	
	
private:
	bool m_bDisposed;
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK