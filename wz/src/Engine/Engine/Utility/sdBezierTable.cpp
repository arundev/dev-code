#include "sdEnginePCH.h"
#include "sdBezierTable.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//---------------------------------------------------------------------------------------------
void sdBezierTable::UpdateTable(int lockHandle)
{
	


	int beginIndex = m_kLockedIndiceTable[lockHandle];
	int endIndex = m_kLockedIndiceTable[lockHandle + 1];
	int prevIndex = (lockHandle - 1) >= 0 ? m_kLockedIndiceTable[lockHandle - 1] : beginIndex;
	int nextIndex = (lockHandle + 2) < (int)m_kLockedIndiceTable.size() ? m_kLockedIndiceTable[lockHandle + 2] : endIndex;

	float v0 = (float)m_kTable[beginIndex];
	float v3 = (float)m_kTable[endIndex];

	// Calculate two plot value between iBeginIndex and iEndIndex
	// 构建中间两个辅助点
	float v1, v2;
	float dx = (float)(endIndex - beginIndex);
	if (prevIndex == beginIndex)
	{
		if (nextIndex == endIndex)
		{
			v1 = v0 + (v3 - v0) / 3.f;
			v2 = v0 + (v3 - v0) * 2.f / 3.f;
		}
		else
		{
			float slope = (float)(m_kTable[nextIndex] - m_kTable[beginIndex]) / (nextIndex - beginIndex);
			v2 = m_kTable[endIndex] - dx * slope / 3.f;
			v1 = (v0 + v2) * 0.5f;
		}
	}
	else
	{
		if (nextIndex == endIndex)
		{
			float slope = (float)(m_kTable[endIndex] - m_kTable[prevIndex]) / (endIndex - prevIndex);
			v1 = m_kTable[beginIndex] + dx * slope / 3.f;
			v2 = (v1 + v3) * 0.5f;
		}
		else
		{
			float slope = (float)(m_kTable[endIndex] - m_kTable[prevIndex]) / (endIndex - prevIndex);
			v1 = m_kTable[beginIndex] + dx * slope / 3.f;

			slope = (float)(m_kTable[nextIndex] - m_kTable[beginIndex]) / (nextIndex - beginIndex);
			v2 = m_kTable[endIndex] - dx * slope / 3.f;
		}
	}

	// Bezier curve interpolate
	for (int i = beginIndex; i <= endIndex; ++i)
	{
		float t = float(i - beginIndex) / (endIndex - beginIndex);
		float h0 = (1 - t) * (1 - t) * (1 - t);
		float h1 = (1 - t) * (1 - t) * t * 3;
		float h2 = (1 - t) * t * t * 3;
		float h3 = t * t * t;

		float v = h0 * v0 + h1 * v1 + h2 * v2 + h3 * v3;

		m_kTable[i] = (uchar)(max(0.f, min(255.f, v)));
	}
}
//---------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL