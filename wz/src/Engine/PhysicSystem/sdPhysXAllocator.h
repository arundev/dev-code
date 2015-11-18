//*************************************************************************************************
// ����:	PhysX���ڴ����������,���ڶ��ڴ����
//---------------------------------------------------------
// ����:		
// ����:		2012-08-17
// ����޸�:
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

	// �麯������
	virtual void* mallocDEBUG(size_t size, const char* fileName, int line);
	virtual void* mallocDEBUG(size_t size, const char* fileName, int line, const char* className, NxMemoryType type);
	virtual void* malloc(size_t size);
	virtual void* malloc(size_t size, NxMemoryType type);
	virtual void* realloc(void* memory, size_t size);
	virtual void free(void* memory);
	virtual void checkDEBUG(void) {};

protected:
	size_t m_uiMaxAlloc;	///< High watermark,�����������ڴ�
	size_t m_uiAllocated;	///< �ܷ������
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif