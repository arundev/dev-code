//*************************************************************************************************
// 内容:	D3D遮挡查询(废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-18
// 最后修改:	2013-02-20
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_QUERY_OCCLUSION_DX9_H__
#define _SD_ENGINE_RENDER_SYSTEM_QUERY_OCCLUSION_DX9_H__
#include "sdQueryOcclusion.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// sdOcclusionQuery
//-------------------------------------------------------------------------------------------------
class sdOcclusionQuery : public NiRefObject
{
public:
	std::vector<void*>	m_akApiQueryVec;
	int m_iResult;

public:
	sdOcclusionQuery() : m_iResult(-1){}
};
//-------------------------------------------------------------------------------------------------
// sdQueryOcclusion_DX9
//-------------------------------------------------------------------------------------------------
class sdQueryOcclusion_DX9 : public sdQueryOcclusion
{
public:
	sdQueryOcclusion_DX9();
	~sdQueryOcclusion_DX9();

	// 虚函数继承
	virtual void BeginQuery(uint uiOcclusionId) = 0;
	virtual void EndQuery() = 0;						
	virtual void Update() = 0;		
	virtual int	 GetResult(uint uiOcclusionId) = 0;	
	virtual void Reset() = 0;

protected:
	typedef std::pair<uint, sdOcclusionQuery> OcclusionQueryPair;
	typedef std::map<uint, sdOcclusionQuery> OcclusionQueryMap;
	typedef std::map<uint, sdOcclusionQuery>::iterator OcclusionQueryMapItr;
	typedef std::map<uint, sdOcclusionQuery>::const_iterator OcclusionQueryMapConItr;
	OcclusionQueryMap m_kOcclusionQueryMap;
	void* m_pvCurApiQuery;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif