#include "sdRenderSystemPCH.h"
#include "sdQueryOcclusion_DX9.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdQueryOcclusion_DX9::sdQueryOcclusion_DX9()
: m_pvCurApiQuery(NULL)
{

}
//-------------------------------------------------------------------------------------------------
sdQueryOcclusion_DX9::~sdQueryOcclusion_DX9()
{

}
//-------------------------------------------------------------------------------------------------
void sdQueryOcclusion_DX9::BeginQuery(uint uiOcclusionId)
{
	// Nested BeginQuery/EndQuery pairs are not allowed.
	NIASSERT(m_pvCurApiQuery == NULL);

	// Find corresponging sdQueryOcclusion, or create a new one if it is not existed.
	OcclusionQueryMapItr itr = m_kOcclusionQueryMap.find(uiOcclusionId);
	if (itr == m_kOcclusionQueryMap.end())
	{
		std::pair<OcclusionQueryMapItr, bool> kResult = 
			m_kOcclusionQueryMap.insert(OcclusionQueryPair(uiOcclusionId, sdOcclusionQuery()));
		NIASSERT(kResult.second);

		itr = kResult.first;		
	}

	// DXAPI calling
	sdOcclusionQuery& kOcclusionQuery = itr->second;
	{
		// Create an d3d occlusion query, which means adding an occlusion testing
		// begin marker to the command buffer queue.
		IDirect3DDevice9* d3ddevice = (IDirect3DDevice9*)NiDX9Renderer::GetRenderer()->GetD3DDevice();
		
		IDirect3DQuery9* d3dQuery;
		HRESULT hr = d3ddevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &d3dQuery);
		NIASSERT(S_OK == hr);

		hr = d3dQuery->Issue(D3DISSUE_BEGIN);
		NIASSERT(S_OK == hr);

		m_pvCurApiQuery = d3dQuery;
	}
	kOcclusionQuery.m_akApiQueryVec.push_back(m_pvCurApiQuery);
}
//-------------------------------------------------------------------------------------------------
void sdQueryOcclusion_DX9::EndQuery()
{
	// Nested BeginQuery/EndQuery pairs are not allowed.
	NIASSERT(m_pvCurApiQuery == NULL);

	// DXAPI calling
	{
		// Add an end marker to the command buffer queue.
		((IDirect3DQuery9*)m_pvCurApiQuery)->Issue(D3DISSUE_END);
	}
}
//-------------------------------------------------------------------------------------------------
void sdQueryOcclusion_DX9::Update()
{
	OcclusionQueryMapItr itr = m_kOcclusionQueryMap.begin();
	OcclusionQueryMapItr itr_end = m_kOcclusionQueryMap.end();
	for (; itr != itr_end; ++itr)
	{
		uint uiOcclusionId = itr->first;
		sdOcclusionQuery& kQuery = itr->second;

		kQuery.m_iResult = 0;
		for (int i = 0; i < (int)m_kOcclusionQueryMap.size(); ++i)
		{
			// DXAPI calling
			DWORD dwNumPixels = 0;
			{
				IDirect3DQuery9* d3dQuery = (IDirect3DQuery9*)kQuery.m_akApiQueryVec[i];
				NIASSERT(d3dQuery);

				// \TODO
				// 1.We may need test passing zero value instead of D#DGETDATA_FLUSH.
				// 2.We may need test calling GetData() as the sequence as queries be created.
				while (S_FALSE == d3dQuery->GetData(&dwNumPixels, sizeof(dwNumPixels), D3DGETDATA_FLUSH));
				//while (S_FALSE == d3dQuery->GetData(&dwNumPixels, sizeof(dwNumPixels), 0));
			}

			kQuery.m_iResult += dwNumPixels;
		}
	}
}
//-------------------------------------------------------------------------------------------------
int sdQueryOcclusion_DX9::GetResult(uint uiOcclusionId)
{
	OcclusionQueryMapItr itr = m_kOcclusionQueryMap.find(uiOcclusionId);
	if (itr == m_kOcclusionQueryMap.end())
		return -1;

	const sdOcclusionQuery& kQuery = itr->second;
	NIASSERT(-1 != kQuery.m_iResult);
	return kQuery.m_iResult;
}
//-------------------------------------------------------------------------------------------------
void sdQueryOcclusion_DX9::Reset()
{
	m_kOcclusionQueryMap.clear();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE