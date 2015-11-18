//*************************************************************************************************
// ����:	�༭�¼�
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITEVENT_H__
#define _SD_GAMEEDITEX_EDITEVENT_H__
#include <sdEventType.h>

namespace GameEditEx
{
	// �༭�¼�����
	enum eEditEventID
	{
		// ����༭�¼�
		E_EDITEVENT_SELECT = GameCore::NUM_EVENTS + 1,
		E_EDITEVENT_TRANSFORM,
		E_EDITEVENT_ROTATE,
		E_EDITEVENT_SCALE,

		// ���α༭�¼�
		E_EDITEVENT_TERRAIN_DEFORM,
		E_EDITEVENT_TERRAIN_SURFACE,

		// �༭�¼�
		NUM_EDITEVENTS,
	};

}
#endif