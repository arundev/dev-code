//*************************************************************************************************
// ����:	���ü�������(�ο���Gamebryo::NiRefObject��AirEngine::MemoryObject)
//---------------------------------------------------------
// ����:		
// ����:		2014-01-14
// ����޸�:	2014-01-14
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_REFERENCE_OBJECT_H__
#define _SD_BASE_REFERENCE_OBJECT_H__

#include "sdAtomic.h"
#include "sdMemObject.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
// ���ü�������
//------------------------------------------------------------------------------------------------- 
class sdRefObject : public sdMemObject
{
public:
	sdRefObject(): m_uiRefCount(1)
	{

	}

	virtual ~sdRefObject()
	{

	}

	/** \brief �������ü���
	*
	*	@remark	�������ü���
	*	@see	sdRefObject
	*	@note
	*/
	void IncRefCount()
	{
		Threading::SDAtomicInc32(&m_uiRefCount);
	}

	/** \brief �������ü���
	*
	*	@remark	�������ü���
	*	@see	sdRefObject
	*	@note
	*/
	void DecRefCount()
	{
		if (Threading::SDAtomicDec32(&m_uiRefCount) == 0)
			DeleteThis();
	}

	/** \brief ��ȡ���ü���
	*
	*	@remark	��ȡ���ü���
	*	@see	sdRefObject
	*	@return	��ǰ���ü���
	*	@note
	*/
	unsigned int GetRefCount() const
	{
		return m_uiRefCount;
	}

protected:
	virtual void DeleteThis()
	{
		delete this;
	}

private:
	uint m_uiRefCount;	///< ���ü���
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE
#endif