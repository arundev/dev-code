//*************************************************************************************************
// ����:	����ģʽ
//---------------------------------------------------------
// ����:		
// ����:		2012-06-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_SINGLETON_H__
#define _SD_BASE_SINGLETON_H__

BASE_NAMESPACE_BEGIN_BASE

// ����ģʽ,�ο���Ogre
template <class T> class sdTSingleton
{
public:
	sdTSingleton()
	{
		assert(InstancePtr() == 0);

#if defined (_MSC_VER) && _MSC_VER < 1200
		int nOffset = (int)(T*)1 - (int)(TSingleton<T>*)(T*)1;
		InstancePtr() = (T*)((int)this + nOffset);
#else
		InstancePtr() = static_cast<T*>(this);
#endif
	}

	virtual ~sdTSingleton()
	{
		InstancePtr() = 0;
	}

	static T& Instance()
	{
		return *InstancePtr();
	}

	static T*& InstancePtr()
	{
		static T* instance = 0;
		return instance;
	}

private:
	// ���ÿ������캯���͸�ֵ����
	sdTSingleton(const sdTSingleton&);
	void operator=(const sdTSingleton&);
};
BASE_NAMESPACE_END_BASE
#endif