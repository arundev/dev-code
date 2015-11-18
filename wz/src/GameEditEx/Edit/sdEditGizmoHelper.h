//*************************************************************************************************
// ����:	�༭������ǽӿ�
//---------------------------------------------------------
// ����:		
// ����:		2014-03-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITGIZMOHELPER_H__
#define _SD_GAMEEDITEX_EDITGIZMOHELPER_H__

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// �༭������ǽӿ�,�����ǵļ��ظ��������
// (ͳһ������Ϊ�˷�ֹ�ڸ����༭״̬�ڲ��ظ������Ƕ���Ϊר�Ź�����)
//	1.֧����Ļ���½�����ϵ����ʾ
//-------------------------------------------------------------------------------------------------
class sdEditGizmoHelper : public NiRefObject
{
public:
	sdEditGizmoHelper();
	virtual ~sdEditGizmoHelper();

	virtual void Update();
	virtual void RenderGizmo();

	NiCamera* GetOrthCamera() { return m_spOrthCamera;}
	NiCamera* GetOrthScreenCamera() { return m_spOrthScreenCamera;}

	

protected:
	static NiNodePtr LoadGizmoNif(const char* szFileName);

	static void DisableZTest(NiAVObject* pkAVObject);
	static void EnableDrawBothMode(NiAVObject* pkAVObject);

protected:
	NiCameraPtr m_spOrthScreenCamera;	///< ������������ڻ�����Ļ����
	NiCameraPtr m_spOrthCamera;			///< �������
	NiAVObjectPtr m_spAxisObject;		///< ���½�������
};
NiSmartPointer(sdEditGizmoHelper);
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif