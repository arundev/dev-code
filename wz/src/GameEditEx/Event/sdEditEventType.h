//*************************************************************************************************
// 内容:	编辑事件
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITEVENT_H__
#define _SD_GAMEEDITEX_EDITEVENT_H__
#include <sdEventType.h>

namespace GameEditEx
{
	// 编辑事件类型
	enum eEditEventID
	{
		// 物件编辑事件
		E_EDITEVENT_SELECT = GameCore::NUM_EVENTS + 1,
		E_EDITEVENT_TRANSFORM,
		E_EDITEVENT_ROTATE,
		E_EDITEVENT_SCALE,

		// 地形编辑事件
		E_EDITEVENT_TERRAIN_DEFORM,
		E_EDITEVENT_TERRAIN_SURFACE,

		// 编辑事件
		NUM_EDITEVENTS,
	};

}
#endif