//---------------------------------------------------------------------------------------------
int sdHermiteShape::AddPoint(const NiPoint3& kPos) 
{ 
	m_kCtrlPointVec.push_back(kPos);
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::SetPoint(int iIndex, const NiPoint3& kPos) 
{ 
	if (iIndex >= 0 && iIndex < (int)m_kCtrlPointVec.size())
		m_kCtrlPointVec[iIndex] = kPos;
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::SetPoints(const std::vector<NiPoint3>& kPoint3Vec) 
{ 
	m_kCtrlPointVec = kPoint3Vec;
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::RemovePoint(int iIndex)
{
	if (iIndex >= 0 && iIndex < (int)m_kCtrlPointVec.size())
		m_kCtrlPointVec.erase(m_kCtrlPointVec.begin() + iIndex);
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::RemoveAllPoints() 
{ 
	m_kCtrlPointVec.clear();
	m_iMaxInterNum = -1;
}
//---------------------------------------------------------------------------------------------
NiPoint3& sdHermiteShape::GetPoint(int iIndex)
{
	return m_kCtrlPointVec[iIndex];
}
//---------------------------------------------------------------------------------------------
std::vector<NiPoint3>& sdHermiteShape::GetPoints()
{
	return m_kCtrlPointVec;
}
//---------------------------------------------------------------------------------------------
const std::vector<NiPoint3>& sdHermiteShape::GetPoints() const
{
	return m_kCtrlPointVec;
}
//---------------------------------------------------------------------------------------------
std::vector<NiPoint3>& sdHermiteShape::GetInterPoints()
{
	return m_kInterPointVec;
}
//---------------------------------------------------------------------------------------------
const std::vector<NiPoint3>& sdHermiteShape::GetInterPoints() const
{
	return m_kInterPointVec;
}
//---------------------------------------------------------------------------------------------
std::vector<NiPoint3>& sdHermiteShape::GetExtendPoints()
{
	return m_kExtendPointVec;
}
//---------------------------------------------------------------------------------------------
const std::vector<NiPoint3>& sdHermiteShape::GetExtendPoints() const
{
	return m_kExtendPointVec;
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::SetExtendWidth(float fWidth)
{
	m_fExtendWidth = fWidth;
}
//---------------------------------------------------------------------------------------------
float sdHermiteShape::GetExtendWidth()
{
	return m_fExtendWidth;
}
//---------------------------------------------------------------------------------------------
void sdHermiteShape::SetMaxInterNum(int iNum)
{
	m_iMaxInterNum = iNum;
}
//---------------------------------------------------------------------------------------------
int sdHermiteShape::GetMaxInterNum()
{
	return m_iMaxInterNum;
}
//---------------------------------------------------------------------------------------------