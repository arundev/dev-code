#include "sdGameEditExPCH.h"
#include "sdEditShape.h"

namespace GameEditEx
{
//-------------------------------------------------------------------------------------------------
sdCircleShape::sdCircleShape() : m_fRadius(10.f)
{

}
//-------------------------------------------------------------------------------------------------
sdCircleShape::~sdCircleShape()
{

}
//-------------------------------------------------------------------------------------------------
bool sdCircleShape::InShape(float fX, float fY)
{
	float fDistanceX = fX - m_kCenter.x;
	float fDistanceY = fY - m_kCenter.y;
	if (fDistanceX * fDistanceX + fDistanceY * fDistanceY <= m_fRadius * m_fRadius)
		return true;
	else 
		return false;
}
//-------------------------------------------------------------------------------------------------
float sdCircleShape::GetWeight(float fX, float fY)
{
	float fDistanceX = fX - m_kCenter.x;
	float fDistanceY = fY - m_kCenter.y;
	float fDistance = sqrt(fDistanceX * fDistanceX + fDistanceY * fDistanceY);
	if (fDistance <= m_fRadius)
	{
		return 1.f;
	}
	else
	{
		return 0.f;
	}
}
//-------------------------------------------------------------------------------------------------
sdDualCircleShape::sdDualCircleShape()
: m_fInnerRadius(5.f)
, m_fOuterRadius(10.f)
, m_fHardness(1.f)
{
	
}
//-------------------------------------------------------------------------------------------------
sdDualCircleShape::~sdDualCircleShape()
{

}
//-------------------------------------------------------------------------------------------------
void sdDualCircleShape::SetInnerRadius(float fInnerRadius)
{ 
	m_fInnerRadius = fInnerRadius;
	if (m_fOuterRadius < fInnerRadius)
		m_fOuterRadius = fInnerRadius;
}

void sdDualCircleShape::SetOuterRadius(float fOuterRadius)
{ 
	m_fOuterRadius = fOuterRadius;
	if (m_fInnerRadius > fOuterRadius)
		m_fInnerRadius = fOuterRadius;
}

void sdDualCircleShape::SetHardness(float fHardness)
{
	if (fHardness < -1.0f)	fHardness = -1.0f;
	if (fHardness > 1.0f)	fHardness = 1.0f;
	m_fHardness = fHardness;
}
//-------------------------------------------------------------------------------------------------
bool sdDualCircleShape::InShape(float fX, float fY)
{
	float fDistanceX = fX - m_kCenter.x;
	float fDistanceY = fY - m_kCenter.y;
	if (fDistanceX * fDistanceX + fDistanceY * fDistanceY <= m_fOuterRadius * m_fOuterRadius)
		return true;
	else 
		return false;
}
//-------------------------------------------------------------------------------------------------
float sdDualCircleShape::GetWeight(float fX, float fY)
{
	if (m_fInnerRadius > m_fOuterRadius)
		return 0.0f;

	float fDistanceX = fX - m_kCenter.x;
	float fDistanceY = fY - m_kCenter.y;
	float fDistance = sqrt(fDistanceX * fDistanceX + fDistanceY * fDistanceY);
	if (fDistance <= m_fInnerRadius)
	{
		return m_fHardness;
	}
	else if (fDistance < m_fOuterRadius)
	{
		return m_fHardness * (1.0f - (fDistance - m_fInnerRadius) / (m_fOuterRadius - m_fInnerRadius));
	}
	else
	{
		return 0.0f;
	}
}
//-------------------------------------------------------------------------------------------------
}