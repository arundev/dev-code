//*************************************************************************************************
// ����:	������ͼ���ع�����,��ȡ��Map�ļ��ع���
//---------------------------------------------------------
// ����:		
// ����:		2013-05-24
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_SCENE_LOAD_MANAGER_H__
#define _SD_ENGINE_SCENE_SCENE_LOAD_MANAGER_H__
#include "sdEntity.h"
#include <sdProfileOutput.h>

//--------------------------------------------------------------------------------------
enum ESceneLoadMode
{
	E_SCENE_LOAD_SYNC,	///< ͬ������
	E_SCENE_LOAD_ASYNC,	///< �첽����
};
//--------------------------------------------------------------------------------------
class sdSceneLoadMgr : public NiRefObject
{
public:
	sdSceneLoadMgr();
	~sdSceneLoadMgr();

	// ����
	void Update();

	// ����ع�����������Ƴ�����
	void AddEntity(sdEntity* pkEntity);
	void RemoveEntity(sdEntity* pkEntity);

	// ��������ģʽ
	ESceneLoadMode GetLoadMode() const { return m_eLoadMode;}
	void SetLoadMode(ESceneLoadMode eLoadMode) { m_eLoadMode = eLoadMode;}

	// ��С�����ƶ���
	float GetLoadRadius() const { return m_fMinLoadRadius;}
	void  SetLoadRadius(float fRadius) { m_fMinLoadRadius = fRadius;}

protected:
	
	// ͬ�����ض���
	void SyncLoad();

	// �첽���ض���
	void AsyncLoad();

protected:
	ESceneLoadMode m_eLoadMode;	///< ����ģʽ

	// Ŀǰʹ�ü򵥵������б�����Ǹ��ӵĿռ��ʷֽṹ���й���
	EntityVec m_kEntityVec;			///< �����б�
	EntityVec m_kProcessVec;		///< ��ǰ֡�������б�
	EntityVec m_kDelayUnloadingVec;	///< �ӳٵ���һ֡�����ж���б�

	// ��ʱ��(��ʱ����)
	Base::Diagnostics::sdProfileOutput m_kProfileOutput;

	NiPoint3 m_kLastLoadPos;	///< ��һ�ν��м���ʱ�����λ��
	float m_fMinLoadRadius;		///< ��Ҫ���¼��ص������С�ƶ�����
};
NiSmartPointer(sdSceneLoadMgr);
//--------------------------------------------------------------------------------------
#endif