#include "WorldEditor.FrameworkPCH.h"
#include "MQuaternion.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MQuaternion::MQuaternion() 
: m_fX(NiQuaternion::IDENTITY.m_fX)
, m_fY(NiQuaternion::IDENTITY.m_fY)
, m_fZ(NiQuaternion::IDENTITY.m_fZ)
, m_fW(NiQuaternion::IDENTITY.m_fW)
{

}
//-------------------------------------------------------------------------------------------------
MQuaternion::MQuaternion(float fX, float fY, float fZ, float fW) : m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW)
{

}
//-------------------------------------------------------------------------------------------------
MQuaternion::MQuaternion(MQuaternion^ rhs) : m_fX(rhs->X), m_fY(rhs->Y), m_fZ(rhs->Z), m_fW(rhs->W)
{

}
//-------------------------------------------------------------------------------------------------
MQuaternion::MQuaternion(MQuaternion% rhs) : m_fX(rhs.X), m_fY(rhs.Y), m_fZ(rhs.Z), m_fW(rhs.W)
{

}
//-------------------------------------------------------------------------------------------------
void MQuaternion::X::set(float fX)
{
	m_fX = fX;
}
//-------------------------------------------------------------------------------------------------
float MQuaternion::X::get()
{
	return m_fX;
}
//-------------------------------------------------------------------------------------------------
void MQuaternion::Y::set(float fY)
{
	m_fY = fY;
}
//-------------------------------------------------------------------------------------------------
float MQuaternion::Y::get()
{
	return m_fY;
}
//-------------------------------------------------------------------------------------------------
void MQuaternion::Z::set(float fZ)
{
	m_fZ = fZ;
}
//-------------------------------------------------------------------------------------------------
float MQuaternion::Z::get()
{
	return m_fZ;
}
//-------------------------------------------------------------------------------------------------
void MQuaternion::W::set(float fW)
{
	m_fW = fW;
}
//-------------------------------------------------------------------------------------------------
float MQuaternion::W::get()
{
	return m_fW;
}
//-------------------------------------------------------------------------------------------------
bool MQuaternion::Assign(Object^ obj)
{
	MQuaternion^ pkValue = dynamic_cast<MQuaternion^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fX = pkValue->m_fX;
	m_fY = pkValue->m_fY;
	m_fZ = pkValue->m_fZ;
	m_fW = pkValue->m_fW;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MQuaternion::Equals(Object^ obj)
{
	MQuaternion^ pkValue = dynamic_cast<MQuaternion^>(obj);
	if (pkValue == nullptr)
		return false;

	return (m_fX == pkValue->X) && (m_fY == pkValue->Y) && (m_fZ == pkValue->Z) && (m_fW == pkValue->W);
}
//-------------------------------------------------------------------------------------------------
System::String^ MQuaternion::ToString()
{
	return System::String::Format("{0},{1},{2},{3}", m_fX, m_fY, m_fZ, m_fW);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK