//*************************************************************************************************
// 内容:	资源加载回调
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_CALLBACK_H__
#define _SD_ENGINE_RESOURCE_CALLBACK_H__

namespace ResourceSystem
{
	// 资源加载回调接口
	class IResCallback : public NiMemObject
	{
	public:
		IResCallback();
		virtual IResCallback() = 0;

		// 加载完成回调
		// (在异步加载完成后被资源管理器调用)
		virtual void OnLoadingOK() = 0;

	protected:
		// 取消加载回调
		// (由加载申请者调用)
		//void CancelLoadingEvent();

	};
}
#endif