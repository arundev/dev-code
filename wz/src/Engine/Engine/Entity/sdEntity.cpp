#include "sdEnginePCH.h"
#include "sdEntity.h"

using namespace Engine::RenderSystem;

NiImplementRTTI(sdEntity, sdGameObject);
sdImplementDefaultStream(sdEntity);

IMPL_PROPFUNC_MAP(sdEntity, sdGameObject)
IMPL_DYNAMIC(sdEntity, sdGameObject)

uint sdEntity::ms_uiSystemRenderedFrameId = 0;
uint sdEntity::ms_uiSystemVisiableFrameId = 0;
//-------------------------------------------------------------------------------------------------
sdEntity::sdEntity()
: m_uiRenderedFrameId(0)
, m_eType(E_ET_ENTITY)
, m_eLoadingStatus(E_LS_VALID)
, m_fViewDistance(128.f)
, m_bDynamic(false)
, m_bEdge(false)
{
	BEGIN_PROPFUNC_MAP(sdEntity)
		REGISTER_PROPFUNC(PROP_NIPT3, PROP_ATTR_DEFAULT, sdEntity, TR)	///< Translate
		REGISTER_PROPFUNC(PROP_NIPT3, PROP_ATTR_DEFAULT, sdEntity, RT)	///< Rotate
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdEntity, SC)	///< Scale
//		REGISTER_PROPFUNC(PROP_BOOL,  PROP_ATTR_DEFAULT, sdEntity, LK)	///< Locked
//		REGISTER_PROPFUNC(PROP_BOOL,  PROP_ATTR_DEFAULT, sdEntity, EN)	///< Enabled
//		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdEntity, BR)	///< BoundRadius
//		REGISTER_PROPFUNC(PROP_NIPT3, PROP_ATTR_DEFAULT, sdEntity, BC)	///< BoundCenter
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdEntity, VD)	///< ViewDistance
//		REGISTER_PROPFUNC(PROP_INT,	  PROP_ATTR_DEFAULT, sdEntity, CL)	///< CullLevel
	END_PROPFUNC_MAP(sdEntity)

	SetName("Entity");

	SetShowEdge(true);
	SetEdgeColor(NiColor::WHITE);

	//SetEntityMask();
}
//-------------------------------------------------------------------------------------------------
sdEntity::~sdEntity()
{

}
//-------------------------------------------------------------------------------------------------
void sdEntity::Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec)
{
	if(!IsVisualObject() || GetAppCulled())
		return;

	static std::stack<NiAVObject*> kObjectStack;	///< 节点堆栈,以避免反复申请内存
	while (!kObjectStack.empty())					///< 清除已有内容
	{
		kObjectStack.pop();
	}

	kObjectStack.push(this);
	while (!kObjectStack.empty())
	{
		NiAVObject* spAVObject = kObjectStack.top();
		kObjectStack.pop();

		// 裁剪

		// 分类处理
		if (NiIsKindOf(NiNode, spAVObject))
		{
			if (NiIsExactKindOf(NiBillboardNode, spAVObject))
			{
				
			}
			else if (NiIsExactKindOf(NiSwitchNode, spAVObject))
			{

			}
			else
			{
				NiNode* spNode = (NiNode*)spAVObject;
				NIASSERT(spNode);

				for (uint uiIndex = 0; uiIndex < spNode->GetChildCount(); ++uiIndex)
				{
					kObjectStack.push(spNode->GetAt(uiIndex));
				}
			}
		}
		else if (NiIsExactKindOf(NiMesh, spAVObject))
		{
			NiMesh* spMesh = (NiMesh*)spAVObject;
			NIASSERT(spMesh);

			kMeshVec.push_back(spMesh);
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdEntity::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	///< Do nothing
	///< Should be overrided in subderived classes

	TickSystemRenderedFrameId();
}
//-------------------------------------------------------------------------------------------------
void sdEntity::UpdateGame()
{

}
//-------------------------------------------------------------------------------------------------
void sdEntity::Update(float fTime, bool bUpdateControllers)
{
	NiNode* pkParent = m_pkParent;
	m_pkParent = NULL;
	NiAVObject::Update(fTime, bUpdateControllers);
	m_pkParent = pkParent;
}
//-------------------------------------------------------------------------------------------------
void sdEntity::UpdateSelected(float fTime)
{
	NiNode* pkParent = m_pkParent;
	m_pkParent = NULL;
	NiAVObject::UpdateSelected(fTime);
	m_pkParent = pkParent;
}
//-------------------------------------------------------------------------------------------------
void sdEntity::SetShowEdge(bool bShow)
{
	if (m_bEdge != bShow)
	{
		m_bEdge = bShow;

		ProcessEdge(this);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEntity::ProcessEdge(NiAVObject* pkAVObject)
{
	ProcessEdgeRecursion(pkAVObject, m_bEdge);
}
//-------------------------------------------------------------------------------------------------
void sdEntity::ProcessEdgeRecursion(NiAVObject* pkAVObject, bool bEdge)
{
	if(!pkAVObject)
		return;

	if (pkAVObject->IsNode())
	{
		NiNode* pkNode = (NiNode*)pkAVObject;
		for (uint ui = 0; ui < pkNode->GetChildCount(); ++ui)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
				ProcessEdgeRecursion(pkChild, bEdge);
		}
	}
	else
	{

	}
}
//-------------------------------------------------------------------------------------------------