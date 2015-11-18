//*************************************************************************************************
// ����:	�������󹤳�
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_FACTORY_H__
#define _SD_GAMECORE_FACTORY_H__
#include "sdDynamicPool.h"
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

class sdSceneItem;

// ���󹤳�
//	1.���̰߳�ȫ
//	2.֧���ڴ�ص��������Initialize/Destory�ӿ�
class sdFactory : public Base::sdTSingleton<sdFactory>
{
public:
	sdFactory(){};
	virtual ~sdFactory(){};

	virtual void Initialize();
	virtual void Destroy();

	///< ���������黹
	///< Fetch/Release, Alloc/Dealloc, New/Delete
	///< @{
	//sdSceneItem*	AllocSceneItem();
	//void			DeallocSceneItem(sdSceneItem* pkSceneItem);
	/// @}

	///< ���Է���(�����ڴ������ܷ����͵���)
	///< @{
	//inline const TDynamicPool<sdSceneItem>& GetSceneItemPool() const;
	///< @}

protected:
	//TDynamicPool<sdSceneItem>	m_kSceneItemPool;
};

#include "sdFactory.inl"
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif