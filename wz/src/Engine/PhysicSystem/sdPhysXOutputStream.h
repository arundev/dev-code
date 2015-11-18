//*************************************************************************************************
// 内容:	PhysX的输出数据流重载,用于打印调试信息
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-17
// 最后修改:
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

	// 虚函数重载
	virtual void reportError(NxErrorCode code, const char * message, const char *file, int line);
	virtual NxAssertResponse reportAssertViolation(const char * message, const char *file, int line);
	virtual void print(const char * message);
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif