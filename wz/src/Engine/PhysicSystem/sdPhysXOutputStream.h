//*************************************************************************************************
// ����:	PhysX���������������,���ڴ�ӡ������Ϣ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-17
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_OUTPUTSTREAM_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_OUTPUTSTREAM_H__
#include <NxUserOutputStream.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXOutputStream : public NxUserOutputStream, public NiMemObject
{
public:
	sdPhysXOutputStream();
	virtual ~sdPhysXOutputStream();

	// �麯������
	virtual void reportError(NxErrorCode code, const char * message, const char *file, int line);
	virtual NxAssertResponse reportAssertViolation(const char * message, const char *file, int line);
	virtual void print(const char * message);
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif