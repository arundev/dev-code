//*************************************************************************************************
// 内容:	遮挡查询(废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-18
// 最后修改:	2013-02-18
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_QUERY_OCCLUSION_H__
#define _SD_ENGINE_RENDER_SYSTEM_QUERY_OCCLUSION_H__
#include <sdSingleton.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// IQueryOcclusion
//-------------------------------------------------------------------------------------------------
class sdQueryOcclusion : public Base::sdTSingleton<sdQueryOcclusion>
{
public:
	virtual ~sdQueryOcclusion() {}

	// Begin issuing an occlusion query.
	virtual void BeginQuery(uint uiOcclusionId) = 0;

	// End issuing an occlusion query.
	virtual void EndQuery() = 0;						

	// Retrieve query result from GPU driver.
	virtual void Update() = 0;		

	// return -1 means the corresponding query has't been send.
	virtual int GetResult(uint uiOcclusionId) = 0;	

	// Reset all states.
	virtual void Reset() = 0;

	// 获取查询接口
	static sdQueryOcclusion* GetQueryOcclusion(){ return InstancePtr();}
};
//-------------------------------------------------------------------------------------------------
// sdOcclusionQueryBuilder
//
// \class	Occlusion query helper class
//			Start issuing an occlusion query when it is created at stack.
//			Finish issuling the occlusion query when it is detroyed at stack
//
// \remark	It should never be created on heap
//-------------------------------------------------------------------------------------------------
class sdOcclusionQueryBuilder : public NiMemObject
{
public: 
	sdOcclusionQueryBuilder(uint uiOcclusionId)
	{
		sdQueryOcclusion::GetQueryOcclusion()->BeginQuery(uiOcclusionId);
	}

	~sdOcclusionQueryBuilder()
	{
		sdQueryOcclusion::GetQueryOcclusion()->EndQuery();
	}
};
//-------------------------------------------------------------------------------------------------
#define QUERY_OCCLUSION_BEGIN(uiOcclusionId)	sdQueryOcclusion::GetQueryOcclusion()->BeginQuery(uiOcclusionId);
#define QUERY_OCCLUSION_END						sdQueryOcclusion::GetQueryOcclusion()->EndQuery();
#define QUERY_OCCLUSION_SECTION(uiOcclusionId)	sdOcclusionQueryBuilder(uiOcclusionId)
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif