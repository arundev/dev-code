#include "sdEnginePCH.h"
#include "sdThreadDataBuff.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
sdThreadDataBuff::sdThreadDataBuff()
: m_pucDataBuff(NULL)
, m_uiSize(0)
{

}
//-------------------------------------------------------------------------------------------------
sdThreadDataBuff::~sdThreadDataBuff()
{
	if (m_pucDataBuff)
	{
		delete m_pucDataBuff;
		m_pucDataBuff = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
uchar* sdThreadDataBuff::OpenBuff(uint uiSize)
{
	if (uiSize >= m_uiSize)
		ResizeBuff(uiSize);
	return m_pucDataBuff;
}
//-------------------------------------------------------------------------------------------------
void sdThreadDataBuff::CloseBuff()
{
	void* pvBuff = InterlockedExchangePointer(&m_pucDataBuff, NULL);	///< ????
	if (pvBuff)
		delete pvBuff;
	m_uiSize = 0;
}
//-------------------------------------------------------------------------------------------------
void sdThreadDataBuff::ResizeBuff(uint uiSize)
{
	if (m_pucDataBuff)
	{
		delete m_pucDataBuff;
		m_pucDataBuff = NULL;
	}

	m_uiSize = uiSize + 1;
	m_pucDataBuff = new uchar[m_uiSize];
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE