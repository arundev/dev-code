//*************************************************************************************************
// 内容:	PhysX的内存分配器重载,用于堆内存管理
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-17
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_ALLOCATOR_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_ALLOCATOR_H__
#include <NxUserAllocator.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXAllocator : public NxUserAllocator, public NiMemObject
{
public:
	sdPhysXAllocator();
	virtual ~sdPhysXAllocator();

	// 虚函数重载
	virtual void* mallocDEBUG(size_t size, const char* fileName, int line);
	virtual void* mallocDEBUG(size_t size, const char* fileName, int line, const char* className, NxMemoryType type);
	virtual void* malloc(size_t size);
	virtual void* malloc(size_t size, NxMemoryType type);
	virtual void* realloc(void* memory, size_t size);
	virtual void free(void* memory);
	virtual void checkDEBUG(void) {};

protected:
	size_t m_uiMaxAlloc;	///< High watermark,单次最大分配内存
	size_t m_uiAllocated;	///< 总分配计数
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif