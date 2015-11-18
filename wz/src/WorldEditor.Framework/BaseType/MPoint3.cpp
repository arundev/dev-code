#include "WorldEditor.FrameworkPCH.h"
#include "MPoint3.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MPoint3::MPoint3() : m_fX(0.f), m_fY(0.f), m_fZ(0.f)
{

}
//-------------------------------------------------------------------------------------------------
MPoint3::MPoint3(float fX, float fY, float fZ) : m_fX(fX), m_fY(fY), m_fZ(fZ)
{

}
//-------------------------------------------------------------------------------------------------
MPoint3::MPoint3(const NiPoint3& kPoint3) : m_fX(kPoint3.x), m_fY(kPoint3.y), m_fZ(kPoint3.z)
{

}
//-------------------------------------------------------------------------------------------------
MPoint3::MPoint3(MPoint3^ rhs) : m_fX(rhs->X), m_fY(rhs->Y), m_fZ(rhs->Z)
{

}
//-------------------------------------------------------------------------------------------------
MPoint3::MPoint3(MPoint3% rhs) : m_fX(rhs.X), m_fY(rhs.Y), m_fZ(rhs.Z)
{

}
//-------------------------------------------------------------------------------------------------
void MPoint3::X::set(float fX)
{
	m_fX = fX;
}
//-------------------------------------------------------------------------------------------------
float MPoint3::X::get()
{
	return m_fX;
}
//-------------------------------------------------------------------------------------------------
void MPoint3::Y::set(float fY)
{
	m_fY = fY;
}
//-------------------------------------------------------------------------------------------------
float MPoint3::Y::get()
{
	return m_fY;
}
//-------------------------------------------------------------------------------------------------
void MPoint3::Z::set(float fZ)
{
	m_fZ = fZ;
}
//-------------------------------------------------------------------------------------------------
float MPoint3::Z::get()
{
	return m_fZ;
}
//-------------------------------------------------------------------------------------------------
bool MPoint3::Assign(Object^ obj)
{
	MPoint3^ pkValue = dynamic_cast<MPoint3^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fX = pkValue->m_fX;
	m_fY = pkValue->m_fY;
	m_fZ = pkValue->m_fZ;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MPoint3::Equals(Object^ obj)
{
	MPoint3^ pkValue = dynamic_cast<MPoint3^>(obj);
	if (pkValue == nullptr)
		return false;

	return (m_fX == pkValue->X) && (m_fY == pkValue->Y) && (m_fZ == pkValue->Z);
}
//-------------------------------------------------------------------------------------------------
System::String^ MPoint3::ToString()
{
	return System::String::Format("{0},{1},{2}", m_fX, m_fY, m_fZ);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK