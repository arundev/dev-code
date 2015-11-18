//*************************************************************************************************
// ����:	������Դ����
//---------------------------------------------------------
// ����:		
// ����:		2012-09-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_LIGHT_H__
#define _SD_ENGINE_SCENE_LIGHT_H__
#include "sdEntity.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdLightSceneObject;
NiSmartPointer(sdLightSceneObject);
ENGINE_NAMESPACE_END_ENGINE

class sdLightModifier;
//-------------------------------------------------------------------------------------------------
// �ƹ����(�д�����޸����������ɶ�̬�ƹ�Ч��)
//-------------------------------------------------------------------------------------------------
class sdLight : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdLight)

public:
	// �ƹ�����
	enum ELightType
	{
		E_LT_UNKNOWN,
		E_LT_AMBIENT,	///< ������
		E_LT_POINT,		///< ���Դ
		E_LT_DIR,		///< �����
		E_LT_SPOT,		///< �۹��
		E_LT_RECTSPOT,	///< �۹��
		E_LT_DUALDIR,	///< ˫�����
		E_LT_BOX,		///< ��ʽ����

		NUM_LIGHTTYPES = E_LT_BOX,
	};

public:
	sdLight();
	virtual ~sdLight();

	// �麯���̳�(�̳���sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// ͨ�ù��ղ�������
	// @{
	void SetLightType(const uint& uiLightType);
	uint GetLightType() const;

	void SetAmbientColor(const NiColor& kColor);
	NiColor GetAmbientColor() const;

	void SetDiffuseColor(const NiColor& kColor);
	NiColor GetDiffuseColor() const;

	void SetSpecularColor(const NiColor& kColor);
	NiColor GetSpecularColor() const;

	void SetDimmer(const float& fDimmer);
	float GetDimmer() const;

	void SetAttenuRange(const float& fAttenuRange);
	float GetAttenuRange() const;

	void SetAttenuSoftness(const float& fAttenuSoftness);
	float GetAttenuSoftness() const;

	void SetSpotFovX(const float& fFovX);
	float GetSpotFovX() const;

	void SetSpotFovY(const float& fFovY);
	float GetSpotFovY() const;

	void SetSpotSoftness(const float& fSoftness);
	float GetSpotSoftness() const;
	// @}

protected:
	// �麯��(�̳���NiNode)
	virtual void UpdateWorldBound();
	virtual void UpdateDownwardPass(NiUpdateProcess& kUpdate);

//	// ������Ⱦ����
//	void UpdateLightSceneObject();

protected:
	// �ƹ�����
	ELightType	m_eLightType;

	// �ƹ����
	NiColor m_kAmbientColor;	///< ��������ɫ
	NiColor m_kDiffuseColor;	///< ��������ɫ
	NiColor m_kSpecularColor;	///< �߹���ɫ
	float m_fDimmer;			///< ��ǿ

	float m_fAttenuRange;		///< ˥����ȡֵ��Χ[0,infinity](���ڵ��Դ�;۹��,���ƾ���˥��)
	float m_fAttenuSoftness;	///< ˥��ǿ�ȣ�ȡֵ��Χ[0,1](���ڵ��Դ�;۹��,���ƾ���˥��)

	float m_fSpotFovX;			///< ȡֵ��Χ[0,180)(���ھ۹�ƣ���������˥��)
	float m_fSpotFovY;			///< ȡֵ��Χ[0,180)(���ھ۹�ƣ���������˥��)
	float m_fSpotSoftness;		///< ȡֵ��Χ[0,1](���ھ۹�ƣ���������˥��)

	// �ƹ���Ⱦ����
	Engine::sdLightSceneObjectPtr m_spLSOLight;

//	// �ӳٹ��յĻ���Mesh(���ھֲ���Դ)
//	NiMeshPtr	m_spDummyMesh;
};
NiSmartPointer(sdLight);
//-------------------------------------------------------------------------------------------------
#include "sdLight.inl"
#endif