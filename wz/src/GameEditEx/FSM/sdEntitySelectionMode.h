//*************************************************************************************************
// ����:	���ѡ��ģʽ
//---------------------------------------------------------
// ����:		
// ����:		2014-03-19
// ����޸�:
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
	// ����ѡ��ʽ
	enum ESelectionRegion
	{
		E_SR_DEFAULT,	///< ��ѡ
		E_SR_RECT,		///< ���ο�ѡ��
		E_SR_CIRCLE,	///< Բ��ѡ��
	};

public:


protected:
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif