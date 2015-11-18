#include "WorldEditor.FrameworkPCH.h"
#include "MEntity.h"

#include <sdEntity.h>

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MEntity::MEntity()
: MGameObject(/*NiNew sdEntity*/)
{

}
//-----------------------------------------------------------------------------
MEntity::MEntity(sdEntity* pkEntity)
: MGameObject(pkEntity)
{

}
//-----------------------------------------------------------------------------
MEntity::MEntity(MNativePtWrap^ pkNativePointer)
: MGameObject(pkNativePointer)
{

}
//-----------------------------------------------------------------------------
void MEntity::Update(float fTime)
{

}
//-----------------------------------------------------------------------------
void MEntity::Render()
{

}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK