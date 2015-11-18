//*************************************************************************************************
// 内容:	常用宏定义
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
// This macro converts a mamaged String* object to a const char* pointer.
#define MStringToCharPointer(pkObject)	\
	(const char*)System::Runtime::InteropServices::Marshal	\
	::StringToHGlobalAnsi(pkObject).ToPointer()

// This macro frees a pointer that was allocated with MStringToCharPointer.
#define MFreeCharPointer(pkObject)	\
	System::Runtime::InteropServices::Marshal	\
	::FreeHGlobal(System::IntPtr((void*)pkObject))

//-----------------------------------------------------------------------------
// This macro defines a managed assertion function.
#define MAssert	System::Diagnostics::Debug::Assert

//-----------------------------------------------------------------------------
// This macro increments the reference count for an NiRefObject
#define MInitRefObject(refobject)	\
	MAssert(refobject != NULL, "Error: Attemping to initialize null NiRefObject!");	\
	refobject->IncRefCount();

// This macro decrements the reference count for an NiRefObject
#define MDisposeRefObject(refobject)	\
	if (refobject)								\
	{											\
		refobject->DecRefCount();				\
		refobject = NULL;						\
	}
//-----------------------------------------------------------------------------	
// RAW指针
//-----------------------------------------------------------------------------
public ref class MNativePtWrap
{
public:
	MNativePtWrap(void* pvNative) : m_pvNative(pvNative){}
	void* GetPointer() { return m_pvNative;}

protected:
	void* m_pvNative;	///< void指针
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK