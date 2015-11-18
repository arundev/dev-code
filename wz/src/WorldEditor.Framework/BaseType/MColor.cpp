#include "WorldEditor.FrameworkPCH.h"
#include "MColor.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MColor::MColor() : m_fR(0.f), m_fG(0.f), m_fB(0.f)
{

}
//-------------------------------------------------------------------------------------------------
MColor::MColor(float fR, float fG, float fB) : m_fR(fR), m_fG(fG), m_fB(fB)
{

}
//-------------------------------------------------------------------------------------------------
MColor::MColor(MColor^ rhs) : m_fR(rhs->R), m_fG(rhs->G), m_fB(rhs->B)
{

}
//-------------------------------------------------------------------------------------------------
MColor::MColor(MColor% rhs) : m_fR(rhs.R), m_fG(rhs.G), m_fB(rhs.B)
{

}
//-------------------------------------------------------------------------------------------------
void MColor::R::set(float fR)
{
	m_fR = fR;
}
//-------------------------------------------------------------------------------------------------
float MColor::R::get()
{
	return m_fR;
}
//-------------------------------------------------------------------------------------------------
void MColor::G::set(float fG)
{
	m_fG = fG;
}
//-------------------------------------------------------------------------------------------------
float MColor::G::get()
{
	return m_fG;
}
//-------------------------------------------------------------------------------------------------
void MColor::B::set(float fB)
{
	m_fB = fB;
}
//-------------------------------------------------------------------------------------------------
float MColor::B::get()
{
	return m_fB;
}
//-------------------------------------------------------------------------------------------------
bool MColor::Assign(Object^ obj)
{
	MColor^ pkValue = dynamic_cast<MColor^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fR = pkValue->m_fR;
	m_fG = pkValue->m_fG;
	m_fB = pkValue->m_fB;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MColor::Equals(Object^ obj)
{
	MColor^ pkColor = dynamic_cast<MColor^>(obj);
	if (pkColor == nullptr)
		return false;

	return (m_fR == pkColor->R) && (m_fG == pkColor->G) && (m_fB == pkColor->B);
}
//-------------------------------------------------------------------------------------------------
System::String^ MColor::ToString()
{
	return System::String::Format("{0},{1},{2}", m_fR, m_fG, m_fB);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK