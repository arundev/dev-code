#include "sdPhysicsSystemPCH.h"
#include "sdPhysXAllocator.h"

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
sdPhysXAllocator::sdPhysXAllocator() : m_uiMaxAlloc(0), m_uiAllocated(0)
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXAllocator::~sdPhysXAllocator()
{

}
//-------------------------------------------------------------------------------------------------
void* sdPhysXAllocator::mallocDEBUG(size_t size, const char* fileName, int line)
{
	return this->malloc(size);
}
//-------------------------------------------------------------------------------------------------
void* sdPhysXAllocator::mallocDEBUG(size_t size, const char* fileName, int line, const char* className, NxMemoryType type)
{
	return this->malloc(size);
}
//-------------------------------------------------------------------------------------------------
void* sdPhysXAllocator::malloc(size_t size)
{
	//m_uiAllocated += size;
	//if (m_uiMaxAlloc < size)
	//{
	//	m_uiMaxAlloc = size;
	//	LOG_TEXT("")
	//}
	return ::malloc(size);
}
//-------------------------------------------------------------------------------------------------
void* sdPhysXAllocator::malloc(size_t size, NxMemoryType type)
{
	return ::malloc(size);
}
//-------------------------------------------------------------------------------------------------
void* sdPhysXAllocator::realloc(void* memory, size_t size)
{
	return ::realloc(memory, size);
}
//-------------------------------------------------------------------------------------------------
void sdPhysXAllocator::free(void* memory)
{
	return ::free(memory);
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM