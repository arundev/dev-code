#include "WorldEditor.FrameworkPCH.h"
#include "MPoint4.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MPoint4::MPoint4() : m_fX(0.f), m_fY(0.f), m_fZ(0.f), m_fW(0.f)
{

}
//-------------------------------------------------------------------------------------------------
MPoint4::MPoint4(float fX, float fY, float fZ, float fW) : m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW)
{

}
//-------------------------------------------------------------------------------------------------
MPoint4::MPoint4(MPoint4^ rhs) : m_fX(rhs->X), m_fY(rhs->Y), m_fZ(rhs->Z), m_fW(rhs->W)
{

}
//-------------------------------------------------------------------------------------------------
MPoint4::MPoint4(MPoint4% rhs) : m_fX(rhs.X), m_fY(rhs.Y), m_fZ(rhs.Z), m_fW(rhs.W)
{

}
//-------------------------------------------------------------------------------------------------
void MPoint4::X::set(float fX)
{
	m_fX = fX;
}
//-------------------------------------------------------------------------------------------------
float MPoint4::X::get()
{
	return m_fX;
}
//-------------------------------------------------------------------------------------------------
void MPoint4::Y::set(float fY)
{
	m_fY = fY;
}
//-------------------------------------------------------------------------------------------------
float MPoint4::Y::get()
{
	return m_fY;
}
//-------------------------------------------------------------------------------------------------
void MPoint4::Z::set(float fZ)
{
	m_fZ = fZ;
}
//-------------------------------------------------------------------------------------------------
float MPoint4::Z::get()
{
	return m_fZ;
}
//-------------------------------------------------------------------------------------------------
void MPoint4::W::set(float fW)
{
	m_fW = fW;
}
//-------------------------------------------------------------------------------------------------
float MPoint4::W::get()
{
	return m_fW;
}
//-------------------------------------------------------------------------------------------------
bool MPoint4::Assign(Object^ obj)
{
	MPoint4^ pkValue = dynamic_cast<MPoint4^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fX = pkValue->m_fX;
	m_fY = pkValue->m_fY;
	m_fZ = pkValue->m_fZ;
	m_fW = pkValue->m_fW;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MPoint4::Equals(Object^ obj)
{
	MPoint4^ pkValue = dynamic_cast<MPoint4^>(obj);
	if (pkValue == nullptr)
		return false;

	return (m_fX == pkValue->X) && (m_fY == pkValue->Y) && (m_fZ == pkValue->Z) && (m_fW == pkValue->W);
}
//-------------------------------------------------------------------------------------------------
System::String^ MPoint4::ToString()
{
	return System::String::Format("{0},{1},{2},{3}", m_fX, m_fY, m_fZ, m_fW);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK