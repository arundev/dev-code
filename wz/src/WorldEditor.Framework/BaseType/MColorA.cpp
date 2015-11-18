#include "WorldEditor.FrameworkPCH.h"
#include "MColorA.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MColorA::MColorA() : m_fR(0.f), m_fG(0.f), m_fB(0.f), m_fA(0.f)
{

}
//-------------------------------------------------------------------------------------------------
MColorA::MColorA(float fR, float fG, float fB, float fA) : m_fR(fR), m_fG(fG), m_fB(fB), m_fA(fA)
{

}
//-------------------------------------------------------------------------------------------------
MColorA::MColorA(MColorA^ rhs) : m_fR(rhs->R), m_fG(rhs->G), m_fB(rhs->B), m_fA(rhs->A)
{

}
//-------------------------------------------------------------------------------------------------
MColorA::MColorA(MColorA% rhs) : m_fR(rhs.R), m_fG(rhs.G), m_fB(rhs.B)
{

}
//-------------------------------------------------------------------------------------------------
void MColorA::R::set(float fR)
{
	m_fR = fR;
}
//-------------------------------------------------------------------------------------------------
float MColorA::R::get()
{
	return m_fR;
}
//-------------------------------------------------------------------------------------------------
void MColorA::G::set(float fG)
{
	m_fG = fG;
}
//-------------------------------------------------------------------------------------------------
float MColorA::G::get()
{
	return m_fG;
}
//-------------------------------------------------------------------------------------------------
void MColorA::B::set(float fB)
{
	m_fB = fB;
}
//-------------------------------------------------------------------------------------------------
float MColorA::B::get()
{
	return m_fB;
}
//-------------------------------------------------------------------------------------------------
void MColorA::A::set(float fA)
{
	m_fA = fA;
}
//-------------------------------------------------------------------------------------------------
float MColorA::A::get()
{
	return m_fA;
}
//-------------------------------------------------------------------------------------------------
bool MColorA::Assign(Object^ obj)
{
	MColorA^ pkValue = dynamic_cast<MColorA^>(obj);
	if (pkValue == nullptr)
		return false;

	m_fR = pkValue->m_fR;
	m_fG = pkValue->m_fG;
	m_fB = pkValue->m_fB;
	m_fA = pkValue->m_fA;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool MColorA::Equals(Object^ obj)
{
	MColorA^ pkColorA = dynamic_cast<MColorA^>(obj);
	if (pkColorA == nullptr)
		return false;

	return (m_fR == pkColorA->R) && (m_fG == pkColorA->G) && (m_fB == pkColorA->B) && (m_fA == pkColorA->A);
}
//-------------------------------------------------------------------------------------------------
System::String^ MColorA::ToString()
{
	return System::String::Format("{0},{1},{2},{3}", m_fR, m_fG, m_fB, m_fA);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK