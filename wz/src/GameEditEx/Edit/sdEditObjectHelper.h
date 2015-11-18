//*************************************************************************************************
// 内容:	物件编辑对外接口
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITOBJECTHELPER_H__
#define _SD_GAMEEDITEX_EDITOBJECTHELPER_H__
#include "sdGameEditEx.h"

namespace GameEditEx
{
	// 地形编辑对外接口,相当于一个Facade,避免上层逻辑直接调用FSM
	class sdEditObjectnHelper : public NiRefObject
	{

	};
	NiSmartPointer(sdEditObjectnHelper);
}