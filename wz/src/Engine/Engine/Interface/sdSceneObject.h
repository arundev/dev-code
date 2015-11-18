//*************************************************************************************************
// ����:	RenderSystemʶ��ĵĳ�����Ⱦ����ӿ�, 
//			 1.SceneGraph�ڲü�����RenderPath�ύSceneObject,
//			 2.RenderPath����SceneObject�������ַ�����ӦRenderPass
//			 3.RenderPass����SceneObject������RenderObject
//---------------------------------------------------------
// ����:		
// ����:		2012-12-07
// ����޸�:	2014-04-24
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_OBJECT_H__
#define _SD_ENGINE_SCENE_OBJECT_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// ������Ⱦ����
//	0.sdSceneObject�ṩ����Ļ�������(Ŀǰ��Ҫ����Ⱦ����)
//	1.sdSceneObject���ṩ��ϵ�Ͳ㼶��ϵ��sdSceneObject֮���໥����
//	2.sdSceneObject�ṩ��Sceneʹ�ã���sdEntity�ڲ��ṩ����
//	3.sdSceneObject�ṩ��RenderSystemʹ��,��sdRenderObject�ṩ��Ⱦ����
//	4.�ڶ�sdMap���вü�֮�󣬵õ�SceneObject����ӵ�RenderSystem������sdRenderObject��
//	  ��RenderPath/RenderPass�����sdRenderObject����Ⱦ
//-------------------------------------------------------------------------------------------------
// At render system layer. a render system instance should hold a scene graph
// which is composed by various sorts of scene objects.
//
// The render path's responsiable to submit every scene objects to correct render pass
//
// At render pass layer, a render pass instance should hold a render list which
// is composed by various sorts of render objects.
//
// Render objects is composed by a scene object with some additional render data
// that will be used when rendering scene objects to render targets.
// (The gamebryo's extra data can not handle this situation clearly, beacuse one 
// scene object may corresponds to more than one render objects.)
//
// Render pass responsible to define object type(or use default render object type), 
// build render object instance from scene object instance, then draw these objects 
// correctly.
//-------------------------------------------------------------------------------------------------
class sdSceneObject : public NiRefObject
{
public:
	// ������������(Render System scene graph object types)
	enum ESceneObjectType
	{
		E_SOT_STATIC_MESH,	///< ��̬������
		E_SOT_SKINNED_MESH,	///< ��Ƥ������
		E_SOT_TERRAIN,		///< ����
		E_SOT_WATER,		///< ˮ
//		E_SOT_DECAL,		///< ����	
		E_SOT_PROJECTOR,	///< Ͷ����
		E_SOT_SKY,			///< 
		E_SOT_LIGHT,		///< �ƹ�
		E_SOT_CAMERA,		///<
		E_SOT_ENV,			///<
		
		E_NUM_SOT,
		E_SOT_UNKNOWN = -1,
	};

public:
	sdSceneObject(ESceneObjectType eType) : m_eType(eType) {};
	virtual ~sdSceneObject(){};

protected:
	ESceneObjectType m_eType;	///< ������������
};
NiSmartPointer(sdSceneObject);
//-------------------------------------------------------------------------------------------------
// �ƹⳡ������
//-------------------------------------------------------------------------------------------------
class sdLightSceneObject : public sdSceneObject
{
public:
	// �ƹ�����
	enum ELightType
	{
		E_LT_DEFAULT,
		E_LT_AMBIENT,	///< ������
		E_LT_POINT,		///< ���Դ
		E_LT_DIR,		///< �����
		E_LT_SPOT,		///< �۹��
		E_LT_RECT_SPOT,	///< �۹��
		E_LT_DUALDIR,	///< ˫�����
		E_LT_BOX,		///< ��ʽ����

		E_NUM_LT,
		E_LT_UNKNOWN = -1,
	};

public:
	sdLightSceneObject();

	// ����
	// @{
	void SetPosition(const NiPoint3& kPosition);
	const NiPoint3& GetPosition() const;

	void SetRotate(const NiMatrix3& kRotate);
	const NiMatrix3& GetRotate() const;

	void SetLightType(ELightType eType);
	ELightType GetLightType() const;

	void SetAmbientColor(const NiColor& kColor);
	NiColor GetAmbientColor() const;

	void SetDiffuseColor(const NiColor& kColor);
	NiColor GetDiffuseColor() const;

	void SetSpecularColor(const NiColor& kColor);
	NiColor GetSpecularColor() const;

	void SetDimmer(const float& fDimmer);
	float GetDimmer() const;

	void SetAttenuRange(float fAttenuRange);
	float GetAttenuRange() const;

	void SetAttenuSoftness(float fAttenuSoftness);
	float GetAttenuSoftness() const;

	void SetSpotFovX(float fFovX);
	float GetSpotFovX() const;

	void SetSpotFovY(float fFovY);
	float GetSpotFovY() const;

	void SetSpotSoftness(float fSoftness);
	float GetSpotSoftness() const;

	void SetFilterMap(NiTexture* spTexture);
	NiTexture* GetFilterMap() const;
	// @}

protected:
	ELightType	m_eLightType;	///< �ƹ�����

	NiPoint3 m_kPosition;		///< λ��
	NiMatrix3 m_kRotate;		///< ��ת

	NiColor m_kAmbientColor;	///< ��������ɫ
	NiColor m_kDiffuseColor;	///< ��������ɫ
	NiColor m_kSpecularColor;	///< �߹���ɫ
	float m_fDimmer;			///< ��ǿ

	float m_fAttenuRange;		///< ˥����ȡֵ��Χ[0,infinity](���ڵ��Դ�;۹��,���ƾ���˥��)
	float m_fAttenuSoftness;	///< ˥��ǿ�ȣ�ȡֵ��Χ[0,1](���ڵ��Դ�;۹��,���ƾ���˥��)

	float m_fSpotFovX;			///< ȡֵ��Χ[0,180)(���ھ۹�ƣ���������˥��)
	float m_fSpotFovY;			///< ȡֵ��Χ[0,180)(���ھ۹�ƣ���������˥��)
	float m_fSpotSoftness;		///< ȡֵ��Χ[0,1](���ھ۹�ƣ���������˥��)

	NiTexturePtr m_spFilterMap;	///< �ƹ������ͼ
	float m_fAlphaRef;			///< �ƹ������ͼ��Alpha�ο�ֵ
};
NiSmartPointer(sdLightSceneObject);
//-------------------------------------------------------------------------------------------------
// Ͷ�䳡������
// Projector
//	- Only project normal/diffuse/flatcolor to scene objects that marked as receive decals
//-------------------------------------------------------------------------------------------------
class sdProjectorSceneObject : public sdSceneObject
{
public:
	// Ͷ�䷽ʽ
	enum EProjectType
	{
		E_PT_ORTHOGONAL,	///< ����Ͷ��
		E_PT_PERSPECTIVE,	///< ͸��͸��
		E_PT_OMNI,			///< ��͸��

		NUM_PT,
	};

	// ��Ϸ�ʽ
	enum EBlendType
	{
		E_BT_NONE,
		E_BT_ALPHABLEND,
		E_BT_ADD,
		E_BT_REPLACE,

		NUM_BT,
	};

public:
	sdProjectorSceneObject();

	// ����
	// @{
	void SetBlendType(EBlendType eBlendType);
	EBlendType GetBlendType() const;

	void SetReceiveLighting(bool bRecLighting);
	bool GetReceiveLighting() const;

	void SetVisible(bool bVisible);
	bool GetVisible() const;

	void SetLayer(uint uiLayer);
	uint GetLaye() const;

	void SetPosition(const NiPoint3& kPosition);
	const NiPoint3& GetPosition() const;

	void SetDirection(const NiPoint3& kDirection);
	const NiPoint3& GetDirection() const;

	void SetUpDirection(const NiPoint3& kUp);
	const NiPoint3& GetUpDirection() const;

	void SetSize(const NiPoint3& kSize);
	const NiPoint3& GetSize() const;

	void SetLength(float fLength);
	float GetLength() const;

	void SetWidth(float fWidth);
	float GetWidth() const;

	void SetHeight(float fHeight);
	float GetHeight() const;

	void SetUVScale(const NiPoint2& kUVScale);
	const NiPoint2& GetUVScale() const;

	void SetUVOffset(const NiPoint2& kUVOffset);
	const NiPoint2& GetUVOffset() const;

	void SetUVSpeed(const NiPoint2& kUVSpeed);
	const NiPoint2& GetUVSpeed() const;

	void SetAlpha(float fAlpha);
	float GetAlpha() const;

	void SetAlphaTest(float fAlphaTest);
	float GetAlphaTest() const;

	 void SetDiffuseMaterial(const NiColor& kColor);
	 const NiColor& GetDiffuseMaterial() const;

	 void SetSpecularMaterial(const NiColor& kColor);
	 const NiColor& GetSpecularMaterial() const;

	 void SetShiness(float fShiness);
	 float GetShiness() const;

	 void SetDiffuseMap(NiTexture* spTexture);
	 NiTexture* GetDiffuseMap() const;

	 void SetNormalMap(NiTexture* spTexture);
	 NiTexture* GetNormalMap() const;

	 void SetFilterMap(NiTexture* spTexture);
	 NiTexture* GetFilterMap() const;

	 void SetMaterial(NiMaterial* spMaterial);
	 NiMaterial* GetMaterial() const;
	// @}

protected:
	EProjectType	m_eProjectType;	///< Ͷ������
	EBlendType		m_eBlendType;	///< �����������
	
	bool m_bReceiveLighting;		///< �Ƿ���ܹ���
	bool m_bVisible;				///< �Ƿ�ɼ�(???)

	// Decals will be covered by higher level decals which belong to same type
	uint m_uiLayer;					

	// ����������Ϣ
	NiPoint3	m_kPosition;		///< ����λ�ú���̬
	NiPoint3	m_kDirection;		///<
	NiPoint3	m_kUp;				///<
	NiPoint3	m_kSize;			///< �����ߴ�

	// ����������Ϣ
	NiPoint2	m_kUVScale;			///< 
	NiPoint2	m_kUVOffset;		///<
	NiPoint2	m_kUVSpeed;			///<

	float		m_fAlpha;
	float		m_fAlphaTest;

	// ����������Ϣ
	NiColor		m_kDiffuseMaterial;
	NiColor		m_kSpecularMaterial;
	float		m_fShiness;

	// Use 2D textures for othogonal or pespective project type decal.
	// Use cubemap textures for omni project type decal.
	// We can special one of them and leave another null, if we only want change 
	// scene surface's diffuse or normal
	NiTexturePtr	m_spDiffuseMap;
	NiTexturePtr	m_spNormalMap;
	NiTexturePtr	m_spFilterMap;

	// �����������
	NiMaterialPtr	m_spMaterial;
};
NiSmartPointer(sdProjectorSceneObject);
//-------------------------------------------------------------------------------------------------
#include "sdSceneObject.inl"
ENGINE_NAMESPACE_END_ENGINE
#endif