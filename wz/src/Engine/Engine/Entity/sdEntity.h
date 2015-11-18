//*************************************************************************************************
// ����:	��������ڵ������,���ڹ�������ͼ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-31
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_ENTITY_H__
#define _SD_ENGINE_SCENE_ENTITY_H__
#include "sdGameObject.h"
#include "IRenderPath.h"
#include <sdMath.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
class sdRenderPath;
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE

class sdMap;
class sdLight;
//--------------------------------------------------------------------------------------
enum EEntityType
{
	E_ET_ENTITY = 0,
	E_ET_BUILDING,		///< ����
	E_ET_LIGHT,			///< ��Դ
	E_ET_DECAL,			///< ����
};
//--------------------------------------------------------------------------------------
enum ELoadingState
{
	E_LS_WAIT = 0,
	E_LS_LOADING,
	E_LS_VALID,
};
//--------------------------------------------------------------------------------------
enum ECullLevel
{
	E_CULLLEVEL_NEVER	= 0,
	E_CULLLEVEL_HIGH	= 1,
	E_CULLLEVEL_MIDDLE	= 2,
	E_CULLLEVEL_LOW		= 3,
};
//--------------------------------------------------------------------------------------
enum EEntityTexMask
{
	E_ETL_DIFFUSEMAP		= 0x00000001,
	E_ETL_GLOSSMAP			= 0x00000002,
	E_ETL_NORMALMAP			= 0x00000004,
	E_ETL_LIGHTMAP			= 0x00000008,
	E_ETL_DETAILNORMALMAP	= 0x00000010,
	E_ETL_ENVIRONMENTMAP	= 0x00000020,
};
//-------------------------------------------------------------------------------------------------
// һ����������,֧������Ч�����ƣ�
//
//	����
//	����
//	����
//	Ѱ·
//	����
//	����
//
//	�ƹ�
//	Ͷ����Ӱ
//	������Ӱ
//	��������
//	ʹ�ù�̽ͷ(Use Light Probes)	Ϊ���������û���̽ͷ������
//	ê�㸲��(Anchor Override)		����ʹ�ù�̽ͷϵͳʱȷ����ֵλ��
//	����Ч��
//	��ӰЧ��
//-------------------------------------------------------------------------------------------------
class sdEntity : public sdGameObject
{
	///< ֻ����ʱ����
	NiDeclareRTTI;
	NiDeclareStream;
	
	///< �¶�̬ϵͳ
	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdEntity)

public:
	sdEntity();
	virtual ~sdEntity();

	// @breif �麯��,��sdEntity����ȡ��Ҫ����Ⱦϵͳ�����sdSceneObject����
	// @param[in] kCamera		�ü����
	// @param[in] kMeshVec		��Ⱦ����
	virtual void Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec);

	// @breif �麯��,��sdEntity����ȡ��Ҫ����Ⱦϵͳ�����SceneObject������뵽RenderPath
	// @param[in] kCamera		�ü����
	// @param[in] kRenderPath	��Ⱦ·��
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// �麯��,����
	virtual void UpdateGame();

	// �麯���̳�,��д����Update����
	// �ٶ����ڵ�bound�����,����������⸸�ڵ㱻��ε���BoundMerge,����CPUѹ��
	virtual void Update(float fTime, bool bUpdateControllers = true);
	virtual void UpdateSelected(float fTime);

	// ���Է���
	//	1.Translate(TR)
	//	2.Rotation(RT)
	//	3.Scale(SC)
	//	4.ViewDistance(VD)
	// @{
	DEFINE_GAMEOBJECT_PROP(NiPoint3, TR, TranslateProp)
	SD_INLINE NiPoint3 GetTranslateProp() const;
	SD_INLINE void SetTranslateProp(const NiPoint3&);
	
	DEFINE_GAMEOBJECT_PROP(NiPoint3, RT, RotateProp)
	SD_INLINE NiPoint3 GetRotateProp() const;
	SD_INLINE void SetRotateProp(const NiPoint3&);
	
	DEFINE_GAMEOBJECT_PROP(float, SC, ScaleProp)
	SD_INLINE float	GetScaleProp() const;
	SD_INLINE void SetScaleProp(const float&);

	DEFINE_GAMEOBJECT_PROP(float, VD, ViewDistance)
	SD_INLINE float	GetViewDistance() const;
	SD_INLINE void SetViewDistance(const float&);
	// @}

	// ��ͼ
	SD_INLINE void SetMap(sdMap* pkMap);
	SD_INLINE sdMap* GetMap();

	// ����
	SD_INLINE void SetType(EEntityType eType);
	SD_INLINE EEntityType GetType();

	// ����״��
	SD_INLINE void SetLoadingStatus(ELoadingState eStatus);
	SD_INLINE ELoadingState GetLoadingStatus();
	
	// ����
	SD_INLINE void SetShowEdge(bool bShow);
	SD_INLINE bool GetShowEdge() const;

	SD_INLINE void SetEdgeColor(const NiColor& kColor);
	SD_INLINE const NiColor& GetEdgeColor() const;

	// �ƹ�
	SD_INLINE void AddAffectLight(sdLight* pkLight);
	SD_INLINE void ClearAffectLight();
	SD_INLINE uint GetAffecttLightNum();
	SD_INLINE std::vector<sdLight*> GetAffectLights();

	// ��Ⱦ֡����
	// @{
	SD_INLINE uint GetRenderedFrameId();
	SD_INLINE void TickRenderedFrameId();

	SD_INLINE static uint GetSystemRenderedFrameId();
	SD_INLINE static void TickSystemRenderedFrameId();

	SD_INLINE static uint GetSystemVisiableFrameId();
	SD_INLINE static void TickSystemVisiableFrameId();
	// @}

protected:

	// ������
	virtual void ProcessEdge(NiAVObject* pkAVObject);
	virtual void ProcessEdgeRecursion(NiAVObject* pkAVObject, bool bEdge);


protected:
	sdMap*		m_pkMap;			///< ������ͼ
	EEntityType	m_eType;			///< ��������
	ELoadingState m_eLoadingStatus;	///< ��Դ����״��
	
	//NiPoint3	m_kBoundCenter;
	//float		m_fBoundRadius;

	float		m_fViewDistance;	///< ���Ӽ�����

	//uint		m_uiLightMask;		///< ����ͨ������
	//uint		m_uiPickMask;		///< ����ʰȡ����
	//uint		m_uiTypeMask;		///< ������������

	bool		m_bVisible;
	bool		m_bLocked;
	bool		m_bDynamic;			///<
	bool		m_bRenderable;

	// ȫ�ֹ���
	bool m_bLightProbe;		///< �Ƿ��ȫ�ֹ���
							///< ȫ�ֹ���Ч��

	// ��ӰЧ��
	bool m_bCastShadow;		///< �Ƿ�Ͷ����Ӱ
	bool m_bReceiveShadow;	///< �Ƿ������Ӱ

	// ����Ч��
	bool m_bEdge;
	NiColor m_kEdgeColor;

	// ��ӰЧ��
	bool m_bEnableGhost;
	bool m_bRenderGhost;

	// �ֲ��ƹ��б�
	std::vector<sdLight*> m_kAffectLight;	

	uint		m_uiRenderedFrameId;		///< ��Ⱦ֡����		
	static uint	ms_uiSystemVisiableFrameId;	///< ��ǰϵͳ����֡����
	static uint ms_uiSystemRenderedFrameId;	///< ��ǰϵͳ��Ⱦ֡����
};
NiSmartPointer(sdEntity);
//-------------------------------------------------------------------------------------------------
typedef std::vector<sdEntityPtr> EntityVec;
typedef std::vector<sdEntityPtr>::iterator EntityVecItr;
typedef std::vector<sdEntityPtr>::reverse_iterator EntityVecRItr;
typedef std::vector<sdEntityPtr>::const_iterator EntityVecConItr;
//-------------------------------------------------------------------------------------------------
#include "sdEntity.inl"
#endif