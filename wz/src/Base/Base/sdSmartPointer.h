//*************************************************************************************************
// 内容:	智能指针对象(参考自Gamebryo::NiSmartPointer)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-14
// 最后修改:	2014-01-14
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_SMART_POINTER_H__
#define _SD_BASE_SMART_POINTER_H__

#include "sdMemObject.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
// 智能指针对象
//-------------------------------------------------------------------------------------------------
template <class T>
class sdPointer : public sdMemObject
{
public:
	sdPointer(T* pkObject = (T*)0)
	{
		m_pkObject = pkObject;
		if (m_pkObject)
			m_pkObject->IncRefCount();
	}

	sdPointer(const sdPointer& kObject)
	{
		m_pkObject = kObject.m_pkObject;
		if (m_pkObject)
			m_pkObject->IncRefCount();
	}

	~sdPointer()
	{
		if (m_pkObject)
			m_pkObject->DecRefCount();
	}

	operator T*() const
	{
		return m_pkObject;
	}

	T& operator*() const
	{
		return *m_pkObject;
	}

	T* operator->() const
	{
		return m_pkObject;
	}

	sdPointer& operator=(const sdPointer& kObject)
	{
		if (m_pkObject != kObject.m_pkObject)
		{
			if (m_pkObject)
				m_pkObject->DecRefCount();

			m_pkObject = kObject.m_pkObject;

			if (m_pkObject)
				m_pkObject->IncRefCount();
		}
		return *this;
	}

	sdPointer& operator=(T* pkObject)
	{
		if (m_pkObject != pkObject)
		{
			if (m_pkObject)
				m_pkObject->DecRefCount();

			m_pkObject = pkObject;

			if (m_pkObject)
				m_pkObject->IncRefCount();
		}
		return *this;
	}

	bool operator==(T* pkObject) const
	{
		return m_pkObject == pkObject;
	}

	bool operator!=(T* pkObject) const
	{
		return m_pkObject !+ pkObject;
	}

	bool operator==(const sdPointer& kObject) const
	{
		return m_pkObject == kObject.m_pkObject;
	}

	bool operator!=(const sdPointer& kObject) const
	{
		return m_pkObject != kObject.m_pkObject;
	}

protected:
	T* m_pkObject;	///< 对象
};
//-------------------------------------------------------------------------------------------------
#define sdSmartPointer(classname)	\
	class classname;				\
	typedef Base::sdPointer<classname> classname##Ptr
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE
#endif