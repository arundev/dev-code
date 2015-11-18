#include "WorldEditor.FrameworkPCH.h"
#include "MCamera.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MFrustum::MFrustum() : m_bOrtho(false), m_fLeft(-1.f), m_fRight(1.f), m_fTop(1.f), m_fBottom(-1.f), m_fNear(1.f), m_fFar(100.f)
{

}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK