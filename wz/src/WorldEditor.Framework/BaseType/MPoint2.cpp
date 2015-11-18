#include "WorldEditor.FrameworkPCH.h"
#include "MPoint2.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MPoint2::MPoint2() : m_fX(0.f), m_fY(0.f)
{

}
//-------------------------------------------------------------------------------------------------
MPoint2::MPoint2(float fX, float fY) : m_fX(fX), m_fY(fY)
{

}
//-------------------------------------------------------------------------------------------------
MPoint2::MPoint2(const NiPoint2& kPoint2) : m_fX(kPoint2.x), m_fY(kPoint2.y)
{

}
//-------------------------------------------------------------------------------------------------
MPoint2::MPoint2(MPoint2^ rhs) : m_fX(rhs->X), m_fY(rhs->Y)
{

}
//-------------------------------------------------------------------------------------------------
MPoint2::MPoint2(MPoint2% rhs) : m_fX(rhs.X), m_fY(rhs.Y)
{

}
//-------------------------------------------------------------------------------------------------
void MPoint2::X::set(float fX)
{
	m_fX = fX;
}
//-------------------------------------------------------------------------------------------------
float MPoint2::X::get()
{
	return m_fX;
}
//-------------------------------------------------------------------------------------------------
void MPoint2::Y::set(float fY)
{
	m_fY = fY;
}
//-------------------------------------------------------------------------------------------------
float MPoint2::Y::get()
{
	return m_fY;
}
//-------------------------------------------------------------------------------------------------
bool MPoint2::Assign(Object^ obj)
{
	MPoint2^ pkValue = dynamic_cast<MPoint2^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fX = pkValue->m_fX;
	m_fY = pkValue->m_fY;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MPoint2::Equals(Object^ obj)
{
	MPoint2^ pkValue = dynamic_cast<MPoint2^>(obj);
	if (pkValue == nullptr)
		return false;

	return (m_fX == pkValue->X) && (m_fY == pkValue->Y);
}
//-------------------------------------------------------------------------------------------------
System::String^ MPoint2::ToString()
{
	return System::String::Format("{0},{1}", m_fX, m_fY);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK