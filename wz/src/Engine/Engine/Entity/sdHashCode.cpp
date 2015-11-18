#include "sdEnginePCH.h"
#include "sdHashCode.h"

#define CODE_DICT_SIZE	61
#define TOTAL_CODE_NUM	14776336

using namespace std;

//-----------------------------------------------------------------------------------------
char g_akCodeDict[] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z'
};
//-----------------------------------------------------------------------------------------
sdHashCode::sdHashCode() : m_iA(0), m_iB(0), m_iC(0), m_iD(0)
{
	srand(uint(time(0)));
}
//-----------------------------------------------------------------------------------------
sdHashCode::~sdHashCode()
{

}
//-----------------------------------------------------------------------------------------
uint sdHashCode::GetCode()
{
	uint uiNewCode = GetCode(false);
	int iLoopCount = 0;
	while(iLoopCount < TOTAL_CODE_NUM && IsCodeExist(uiNewCode))
	{
		uiNewCode = GetCode(true);
		++iLoopCount;
	}

	if (iLoopCount == TOTAL_CODE_NUM)
		return 0;
	else
		return uiNewCode;
}
//-----------------------------------------------------------------------------------------
bool sdHashCode::AddCode(uint uiCode)
{
	if (!IsCodeExist(uiCode))
	{
		m_uiHashCodes.push_back(uiCode);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------
void sdHashCode::RemoveCode(uint uiCode)
{
	vector<uint>::iterator itr = find(m_uiHashCodes.begin(), m_uiHashCodes.end(), uiCode);
	if (itr != m_uiHashCodes.end())
	{
		m_uiHashCodes.erase(itr);
	}
}
//-----------------------------------------------------------------------------------------
bool sdHashCode::IsCodeExist(uint uiCode)
{
	vector<uint>::iterator itr = find(m_uiHashCodes.begin(), m_uiHashCodes.end(), uiCode);
	return (itr != m_uiHashCodes.end());
}
//-----------------------------------------------------------------------------------------
void sdHashCode::Clear()
{
	m_uiHashCodes.clear();
}
//-----------------------------------------------------------------------------------------
uint sdHashCode::GetCode(bool bCollide)
{
	uint uiCode = 0;
	char* pucCode = (char*)(&uiCode);
	if (bCollide)
	{
		if (m_iA + 1 < CODE_DICT_SIZE)		++m_iA;
		else if (m_iB + 1 < CODE_DICT_SIZE)	++m_iB;
		else if (m_iC + 1 < CODE_DICT_SIZE)	++m_iC;
		else if (m_iD + 1 < CODE_DICT_SIZE)	++m_iD;
		else	m_iA = m_iB = m_iC = m_iD = 0;
	}
	else
	{
		m_iA = (uint)(((float)rand() / (float)RAND_MAX) * CODE_DICT_SIZE + 0.5f);
		m_iB = (uint)(((float)rand() / (float)RAND_MAX) * CODE_DICT_SIZE + 0.5f);
		m_iC = (uint)(((float)rand() / (float)RAND_MAX) * CODE_DICT_SIZE + 0.5f);
		m_iD = (uint)(((float)rand() / (float)RAND_MAX) * CODE_DICT_SIZE + 0.5f);
	}

	(*(pucCode + 0)) = g_akCodeDict[m_iA];
	(*(pucCode + 1)) = g_akCodeDict[m_iB];
	(*(pucCode + 2)) = g_akCodeDict[m_iC];
	(*(pucCode + 3)) = g_akCodeDict[m_iD];

	return uiCode;
}
//-----------------------------------------------------------------------------------------