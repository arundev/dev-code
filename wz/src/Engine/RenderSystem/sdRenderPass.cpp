#include "sdRenderSystemPCH.h"
#include "sdRenderPass.h"
#include "sdRenderObject.h"
#include "sdRenderObjectAlloc.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
bool sdRenderPass::Comparator(sdRenderObject* lhs, sdRenderObject* rhs)
{
	NIASSERT(lhs);
	NIASSERT(rhs);

	if (lhs->GetType() != rhs->GetType())
		return (uint)lhs->GetType() < (uint)rhs->GetType();

	return lhs->GetSortKey() < rhs->GetSortKey();
}
//-------------------------------------------------------------------------------------------------
sdRenderPass::sdRenderPass(sdRenderObjectAlloc* pkAlloc)
: m_bInitialized(false)
, m_bActived(true)
, m_pkAlloc(pkAlloc)
{
	NIASSERT(pkAlloc && "Invalid render object alloc.");

	m_pkRenderParams = NiNew sdRenderParams;
	NIASSERT(m_pkRenderParams);
}
//-------------------------------------------------------------------------------------------------
sdRenderPass::~sdRenderPass()
{
	ClearRenderObjects();

	m_pkAlloc = NULL;

	if (m_pkRenderParams)
	{
		NiDelete m_pkRenderParams;
		m_pkRenderParams = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::SetRenderParams(const sdRenderParams* pkRenderParams)
{
	*m_pkRenderParams = *pkRenderParams;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::Begin()
{
	ClearRenderObjects();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::Draw()
{
	if (!m_bInitialized || !m_bActived)
		return;

	// 对渲染对象进行排序
	Sort();

	// 对渲染对象依次调用绘制函数
	sdRenderObject::ERenderObjectType eRenderObjectType = sdRenderObject::E_ROT_UNKNOWN;
	RenderObjectVecItr itr = m_kRenderObjectVec.begin();
	RenderObjectVecItr itr_end = m_kRenderObjectVec.end();
	for (; itr!=itr_end; ++itr)
	{
		(*itr)->Draw(/*eRenderObjectType*/);
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::End()
{
	ClearRenderObjects();
}
//-------------------------------------------------------------------------------------------------
sdRenderObject::ShaderParamItem* sdRenderPass::CloneShaderParams(sdRenderObject::ShaderParamItem* pkParams)
{
	typedef sdRenderObject::ShaderParamItem	ShaderParamItem;

	// 拷贝着色器参数数组,是一个以空元素结束的数组
	if (NULL == pkParams || NULL == pkParams[0].szName)
		return NULL;

	// 计算数组大小
	uint uiNum = 0;
	uint uiByteSize = 0;
	while(NULL != pkParams[uiNum].szName)
	{
		uiByteSize += pkParams[uiNum].uiByteSize;
		++uiNum;
	}

	// 在缓存池创建对象并拷贝之
	//  1.前一部分放置以空元素结束的ShaderParam数组
	//  2.后一部分放置ShaderParam指向的参数
	char* pcMemBuf = (char*)m_pkAlloc->Alloc((uiNum+1) * sizeof(ShaderParamItem) + uiByteSize);
	ShaderParamItem* pkDstParams = (ShaderParamItem*)pcMemBuf;
	char* pkDstData = pcMemBuf + (uiNum+1) * sizeof(ShaderParamItem);

	ShaderParamItem* pkCurSrcParam = pkParams;
	ShaderParamItem* pkCurDstParam = pkDstParams;
	char* pkCurDstData = pkDstData;
	for (uint i = 0; i < uiNum; ++i)
	{
		pkCurDstParam->szName = pkCurSrcParam->szName;
		pkCurDstParam->uiByteSize = pkCurSrcParam->uiByteSize;
		memcpy(pkCurDstData, pkCurSrcParam->pvData, pkCurSrcParam->uiByteSize);
		pkCurDstParam->pvData = (void*)pkCurDstData;

		++pkCurSrcParam;
		++pkCurDstParam;
		pkCurDstData += pkCurSrcParam->uiByteSize;
	}

	pkDstParams[uiNum].szName = NULL;
	pkDstParams[uiNum].uiByteSize = 0;
	pkDstParams[uiNum].pvData = NULL;

	return pkDstParams;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::InsertRenderObject(sdRenderObject* pkObject)
{
	NIASSERT(pkObject);
	m_kRenderObjectVec.push_back(pkObject);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::ClearRenderObjects()
{
	m_kRenderObjectVec.clear();
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::InsertStaticSceneObject(NiMesh* spMesh, NiMaterial* spMaterial,
	NiPropertyList* spRenderProps, sdRenderObject::ShaderParamItem* pkShaderParams)
{
	sdRenderObject::ShaderParamItem* pkCloneParams = CloneShaderParams(pkShaderParams);
	void* pkMemBuf = m_pkAlloc->Alloc(sizeof(sdStaticMeshRenderObject));
	new (pkMemBuf) sdStaticMeshRenderObject(spMesh, spMaterial, spRenderProps, pkCloneParams);
	InsertRenderObject((sdRenderObject*)pkMemBuf);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::InsertGhostMeshSceneObject(NiMesh *spMesh, NiMaterial *spMaterial, 
	NiPropertyList *spRenderProps,RenderSystem::sdRenderObject::ShaderParamItem *pkShaderParams, uint uiGhostIndex)
{
	sdRenderObject::ShaderParamItem* pkCloneParams = CloneShaderParams(pkShaderParams);
	void* pkMemBuf = m_pkAlloc->Alloc(sizeof(sdGhostMeshRenderObject));
	new (pkMemBuf) sdGhostMeshRenderObject(spMesh, spMaterial, spRenderProps, pkCloneParams, uiGhostIndex);
	InsertRenderObject((sdRenderObject*)pkMemBuf);	
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::InsertLightSceneObject(sdLightSceneObject* pkLight, NiMaterial* spMaterial,
	NiPropertyList* spRenderProps, sdRenderObject::ShaderParamItem* pkShaderParams)
{
	sdRenderObject::ShaderParamItem* pkCloneParams = CloneShaderParams(pkShaderParams);
	void* pkMemBuf = m_pkAlloc->Alloc(sizeof(sdLightVolumeRenderObject));
	new (pkMemBuf) sdLightVolumeRenderObject(pkLight, spMaterial, spRenderProps, pkCloneParams);
	InsertRenderObject((sdRenderObject*)pkMemBuf);
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::InsertProjectorSceneObject(sdProjectorSceneObject* pkProjector, NiMaterial* spMaterial,
	NiPropertyList* spRenderProps, sdRenderObject::ShaderParamItem* pkShaderParams)
{
	sdRenderObject::ShaderParamItem* pkCloneParams = CloneShaderParams(pkShaderParams);
	void* pkMemBuf = m_pkAlloc->Alloc(sizeof(sdProjectorVolumeRenderObject));
	new (pkMemBuf) sdProjectorVolumeRenderObject(pkProjector, spMaterial, spRenderProps, pkCloneParams);
	InsertRenderObject((sdRenderObject*)pkMemBuf);
}
//-------------------------------------------------------------------------------------------------
sdRenderPass::RenderObjectComparator sdRenderPass::GetComparator()
{
	return sdRenderPass::Comparator;
}
//-------------------------------------------------------------------------------------------------
void sdRenderPass::Sort()
{
	std::sort(m_kRenderObjectVec.begin(), m_kRenderObjectVec.end(), GetComparator());
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE