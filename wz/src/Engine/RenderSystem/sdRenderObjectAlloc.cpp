#include "sdRenderSystemPCH.h"
#include "sdRenderObjectAlloc.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdRenderObjectAlloc::sdRenderObjectAlloc(uint uiCapicaty, uint uiIncreaseStep)
: m_uiCapacity(uiCapicaty)
, m_uiSize(0)
, m_uiNumObject(0)
, m_uiIncreaseStep(uiIncreaseStep)
, m_pkIncreaseChild(NULL)
{
	m_pcBuffer = new char[uiCapicaty];
	NIASSERT(m_pcBuffer);
}
//-------------------------------------------------------------------------------------------------
sdRenderObjectAlloc::~sdRenderObjectAlloc()
{
	delete m_pkIncreaseChild;
	m_pkIncreaseChild = NULL;

	delete [] m_pcBuffer;
	m_pcBuffer = NULL;

	m_uiCapacity = 0;
	m_uiSize = 0;
	m_uiNumObject = 0;
	m_uiIncreaseStep = 0;
}
//-------------------------------------------------------------------------------------------------
void* sdRenderObjectAlloc::Alloc(uint uiSize)
{
	if (m_uiSize + uiSize > m_uiCapacity)
	{
		if (NULL == m_pkIncreaseChild)
		{
			uint uiCapacity = m_uiIncreaseStep;
			if (uiCapacity < uiSize)
			{
				// 所要求分配对象过大,超出内存增加步长,则增大到所需求的大小
				NIASSERT(0 && "Too big render object size or too small increase step was setting.");
				uiCapacity = uiSize;
			}

			m_pkIncreaseChild = NiNew sdRenderObjectAlloc(uiCapacity, m_uiIncreaseStep);
		}

		return m_pkIncreaseChild->Alloc(uiSize);
	}

	void* pResult = &m_pcBuffer[m_uiSize];
	m_uiSize += uiSize;
	++m_uiNumObject;

	return pResult;
}
//-------------------------------------------------------------------------------------------------
void sdRenderObjectAlloc::DeallocAll()
{
	delete m_pkIncreaseChild;
	m_pkIncreaseChild = NULL;

	m_uiSize = 0;
	m_uiNumObject = 0;
}
//-------------------------------------------------------------------------------------------------
uint sdRenderObjectAlloc::Capacity() const
{
	uint uiCapacity = m_uiCapacity;
	if (m_pkIncreaseChild)
		uiCapacity += m_pkIncreaseChild->Capacity();
	return uiCapacity;
}
//-------------------------------------------------------------------------------------------------
uint sdRenderObjectAlloc::Size() const
{
	uint uiSize = m_uiSize;
	if (m_pkIncreaseChild)
		uiSize += m_pkIncreaseChild->Size();
	return uiSize;
}
//-------------------------------------------------------------------------------------------------
uint sdRenderObjectAlloc::NumObjects() const
{
	uint uiNumObject = m_uiNumObject;
	if (m_pkIncreaseChild)
		uiNumObject += m_pkIncreaseChild->NumObjects();
	return uiNumObject;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE