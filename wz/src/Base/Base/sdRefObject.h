//*************************************************************************************************
// 内容:	引用计数对象(参考自Gamebryo::NiRefObject和AirEngine::MemoryObject)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-14
// 最后修改:	2014-01-14
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_REFERENCE_OBJECT_H__
#define _SD_BASE_REFERENCE_OBJECT_H__

#include "sdAtomic.h"
#include "sdMemObject.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
// 引用计数对象
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

	/** \brief 增加引用计数
	*
	*	@remark	增加引用计数
	*	@see	sdRefObject
	*	@note
	*/
	void IncRefCount()
	{
		Threading::SDAtomicInc32(&m_uiRefCount);
	}

	/** \brief 减少引用计数
	*
	*	@remark	增加引用计数
	*	@see	sdRefObject
	*	@note
	*/
	void DecRefCount()
	{
		if (Threading::SDAtomicDec32(&m_uiRefCount) == 0)
			DeleteThis();
	}

	/** \brief 获取引用计数
	*
	*	@remark	获取引用计数
	*	@see	sdRefObject
	*	@return	当前引用计数
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
	uint m_uiRefCount;	///< 引用计数
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE
#endif