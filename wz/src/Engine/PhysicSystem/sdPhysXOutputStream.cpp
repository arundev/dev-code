#include "sdPhysicsSystemPCH.h"
#include "sdPhysXOutputStream.h"

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
sdPhysXOutputStream::sdPhysXOutputStream()
{
	
}
//-------------------------------------------------------------------------------------------------
sdPhysXOutputStream::~sdPhysXOutputStream()
{

}
//-------------------------------------------------------------------------------------------------
void sdPhysXOutputStream::reportError(NxErrorCode code, const char * message, const char *file, int line)
{
	
}
//-------------------------------------------------------------------------------------------------
NxAssertResponse sdPhysXOutputStream::reportAssertViolation(const char * message, const char *file, int line)
{
	return NX_AR_CONTINUE;
}
//-------------------------------------------------------------------------------------------------
void sdPhysXOutputStream::print(const char * message)
{

}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM