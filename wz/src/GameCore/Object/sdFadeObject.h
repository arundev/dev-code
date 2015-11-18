//*************************************************************************************************
// 内容:	场景逻辑对象,渐变对象
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_FADE_OBJECT_H__
#define _SD_GAMECORE_FADE_OBJECT_H__
#include "sdActor.h"

class sdFadeObject : public sdActor
{



protected:
	///< 隐藏掩码, 表示隐藏原因
	uint	m_uiHideMask;	

	///< 半透明参数
	bool	m_bBlendStatus;
};

#endif