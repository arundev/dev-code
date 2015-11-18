//*************************************************************************************************
// 内容:	物件选择模式
//---------------------------------------------------------
// 作者:		
// 创建:		2014-03-19
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_ENTITY_SELECTION_MODE_H__
#define _SD_GAMEEDITEX_ENTITY_SELECTION_MODE_H__
#include "sdEditMode.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
class sdEntitySelectionMode : public sdEditMode
{
public:
	// 区域选择方式
	enum ESelectionRegion
	{
		E_SR_DEFAULT,	///< 点选
		E_SR_RECT,		///< 矩形框选择
		E_SR_CIRCLE,	///< 圆形选择
	};

public:


protected:
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif