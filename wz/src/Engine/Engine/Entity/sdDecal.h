//*************************************************************************************************
// ����:	������������
//---------------------------------------------------------
// ����:		
// ����:		2012-12-06
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_DECAL_H__
#define _SD_ENGINE_SCENE_DECAL_H__
#include "sdEntity.h"

//-------------------------------------------------------------------------------------------------
// ��������(�д�����޸����������ɶ�̬����Ч��,����ƽ�ơ���ת�����š�������)
//-------------------------------------------------------------------------------------------------
class sdDecal : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdDecal)

public:
	sdDecal();
	virtual ~sdDecal();

	// �麯���̳�(�̳���sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// ͨ��������������
	// @{
	void SetWidth(const float& fValue);
	float GetWidth() const;

	void SetHeight(const float& fValue);
	float GetHeight() const;

	void SetLength(const float& fValue);
	float GetLength() const;

	void SetDiffuseMaterial(const NiColor& kValue);
	NiColor GetDiffuseMaterial() const;

	void SetSpecularMaterial(const NiColor& kValue);
	NiColor GetSpecularMaterial() const;

	void SetUVScale(const NiPoint2& kValue);
	NiPoint2 GetUVScale() const;

	void SetUVOffset(const NiPoint2& kValue);
	NiPoint2 GetUVOffset() const;

	void SetUVSpeed(const NiPoint2& kValue);
	NiPoint2 GetUVSpeed() const;

	void sdDecal::SetDiffuseMap(const NiFixedString& kValue);
	NiFixedString GetDiffuseMap() const;

	void sdDecal::SetNormalMap(const NiFixedString& kValue);
	NiFixedString GetNormalMap() const;

	void sdDecal::SetFilterMap(const NiFixedString& kValue);
	NiFixedString GetFilterMap() const;

	void sdDecal::SetRecvLighting(const bool& bValue);
	bool GetRecvLighting() const;

	void sdDecal::SetAlpha(const float& fValue);
	float GetAlpha() const;

	void sdDecal::SetBlendType(const int& iValue);
	int GetBlendType() const;
	// @}

protected:
	// �麯��(�̳���NiNode)
	virtual void UpdateWorldBound();
	virtual void UpdateDownwardPass(NiUpdateProcess& kUpdate);

protected:
	// ��������
	float m_fWidth;					///< �����ߴ�
	float m_fHeight;				///<
	float m_fLength;				///<

	NiColor m_kDiffuseMaterial;		///< �������������
	NiColor m_kSpecularMaterial;	///< �����߹����

	NiPoint2 m_kUVScale;			///< ������������
	NiPoint2 m_kUVOffset;			///<
	NiPoint2 m_kUVSpeed;			///< 

	bool m_bDiffuseMapDirty;		///< �����������ͼ�Ƿ��޸Ĺ�
	NiFixedString m_kDiffuseMap;	///< ��������ͼ·��
	NiTexturePtr m_spDiffuseMap;	///< ��������ͼ

	bool m_bNormalMapDirty;			///< ��Ƿ�����ͼ�Ƿ��޸Ĺ�
	NiFixedString m_kNormalMap;		///< ������ͼ·��
	NiTexturePtr m_spNormalMap;		///< ������ͼ

	bool m_bFilterMapDirty;			///< ��ǹ�����ͼ�Ƿ��޸Ĺ�
	NiFixedString m_kFilterMap;		///< ������ͼ·��
	NiTexturePtr m_spFilterMap;		///< ������ͼ

	bool m_bReceiveLighting;		///< �Ƿ��ܹ�
	float m_fAlpha;					///< ͸����
	int m_iBlendType;				///< �������

	// ������Ⱦ����
	Engine::sdProjectorSceneObjectPtr m_spPSOProjector;
};
NiSmartPointer(sdDecal);
//-------------------------------------------------------------------------------------------------
#include "sdDecal.inl"
#endif