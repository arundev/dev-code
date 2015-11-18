//*************************************************************************************************
// 内容:	纹理基类,
//		该基类及其子类直接封装D3D纹理,但是以GB纹理的形式提供给外面使用,
//		以便在实现一些GB内部不支持的功能的同时不用修改GB引擎	
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_H__
#define _SD_ENGINE_TEXTURE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// 主要是封装一些纹理共有功能(有待进一步完善接口)
//	1.设备重设处理
//-------------------------------------------------------------------------------------------------
class sdTexture : public NiRefObject
{
public:
	sdTexture();
	virtual ~sdTexture();

	// 静态初始化
	static void	StaticInitialize();
	static void StaticDestroy();

protected:
	// 挂接到纹理链表
	void AddTextureToList();
	void RemoveTextureFromList();

	// 设备回调处理
	virtual bool OnDeviceLost() = 0;
	virtual bool OnDeviceReset(bool bBefore) = 0;

	// 设备回调函数(注册给GB)
	static bool DeviceLostCallBack(void* pvData);
	static bool DeviceResetCallBack(bool bBeforeReset, void* pvData);

protected:
	// GB设备回调索引
	static uint	ms_uiLostNotifyIndex;
	static uint	ms_uiResetNotifyIndex;

	// 所有Texture对象双向链表(参考GB/NiTexture.h)
	//	1.设备丢失与重设时处理所有对象
	sdTexture*	m_pkPrev;
	sdTexture*	m_pkNext;
	static sdTexture*	ms_pkHead;
	static sdTexture*	ms_pkTail;
};
NiSmartPointer(sdTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif