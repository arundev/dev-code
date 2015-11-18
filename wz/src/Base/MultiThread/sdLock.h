//*************************************************************************************************
// 内容:		锁
//---------------------------------------------------------
// 作者:		
// 创建:		2014--1-13
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_LOCK_H__
#define _SD_BASE_THREADING_LOCK_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-------------------------------------------------------------------------------------------------
template <class T> 
class SDTLock
{
public:
	SDTLock(T& kLockImpl) : m_kLockImpl(kLockImpl) { m_kLockImpl.Lock();}
	~SDTLock(){m_kLockImpl.Unlock();}

protected:
	T m_kLockImpl;
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif