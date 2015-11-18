//*************************************************************************************************
// ����:	������������������ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-11-23
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_ENTITY_AREA_LISTENER_H__
#define _SD_ENGINE_ENTITY_AREA_LISTENER_H__
#include "sdEntity.h"

class sdArea;
class IAreaListener
{
public:
	IAreaListener(){};
	virtual ~IAreaListener(){};

	virtual void OnEnterArea(sdArea* pkArea) = 0;
	virtual void OnStayArea(sdArea* pkArea) = 0;
	virtual void OnLeaveArea(sdArea* pkArea) = 0;

	virtual const NiPoint3& GetListenerPos() const = 0;
};
#endif