//*************************************************************************************************
// 内容:	场景逻辑对象,服务器对象,主要是一些由服务器控制的拾取对象,包括Trigger,Wrapper等
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_SCENE_ITEM_H__
#define _SD_GAMECORE_SCENE_ITEM_H__
#include "sdSceneServerObject.h"

class sdSceneItem : public sdSceneServerObject
{
	NiDeclareRTTI;

public:
	sdSceneItem();
	virtual ~sdSceneItem();
	

protected:

	// 资源
	//NiNodePtr	m_spNIFNode;
	//sdResourcePtr	m_spResLoadingPart;
};

#endif