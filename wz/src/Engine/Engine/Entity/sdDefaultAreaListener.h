//*************************************************************************************************
// 内容:	场景区域默认监听处理器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_DEFAULT_AREA_LISTENER_H__
#define _SD_ENGINE_SCENE_DEFAULT_AREA_LISTENER_H__
#include "IAreaListener.h"

class sdDefaultAreaListener : public IAreaListener
{
public:
	sdDefaultAreaListener();
	virtual ~sdDefaultAreaListener();

	virtual void Update();

	// 虚函数继承
	virtual const NiPoint3& GetListenerPos() const;

	virtual void OnEnterArea(sdArea* pkArea);
	virtual void OnStayArea(sdArea* pkArea);
	virtual void OnLeaveArea(sdArea* pkArea);

};
#endif