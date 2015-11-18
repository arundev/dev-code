//*************************************************************************************************
// ����:	Ӳ����������
//---------------------------------------------------------
// ����:		
// ����:		2013-01-04
// ����޸�:	2013-01-04
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_CURSOR_MHR_H__
#define _SD_GAMECORE_CURSOR_MHR_H__
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
SD_GAMECORE_NAMESPACE_BEGIN_UI
//-------------------------------------------------------------------------------------------------
// ��������
//	1.Ӳ����꣺�����ͼƬ���ø�Windowsϵͳ����GDI�����ƹ��
//	2.�����꣺��D3D���ƹ��
// ����ʵ�ֵ���Ӳ�����
//-------------------------------------------------------------------------------------------------
class sdCursorMgr : public Base::sdTSingleton<sdCursorMgr>
{
public:
	// �������
	enum ECursorType
	{
		E_CT_NORMAL = 0,			///< ��ͨ
		E_CT_ATTACK,				///< ����
		E_CT_ATTACK_GRAY,			///< ����(Զ)
		E_CT_LOOT,					///< ʰȡ
		E_CT_LOOT_GRAY,				///< ʰȡ(Զ)
		E_CT_TALK,					///< NPC�Ի�
		E_CT_TALK_GRAY,				///< NPC�Ի�(Զ)
		E_CT_READ,					///< �Ķ�

		E_CT_GATHER_HERB,			///< �ɼ�ҩ��
		E_CT_GATHER_HERB_GRAY,		///< �ɼ�ҩ��(Զ)
		E_CT_GATHER_MINE,			///< �ɿ�
		E_CT_GATHER_MINE_GRAY,		///< �ɿ�(Զ)

		E_CT_REPAIR,				///< ����

		E_CT_BUY,					///< ����
		E_CT_BUY_GRAY,				///< ����(Զ)
		E_CT_WATCH,					///< ��ѡ
		E_CT_BOX,					///< ������������

		E_CT_LEFT_STRENTCH,			///< �������������
		E_CT_RIGHT_STRENTCH,		///< �������������
		E_CT_UP_STRENTCH,			///< �������������
		E_CT_DOWN_STRENTCH,			///< �������������
		E_CT_LEFTUP_STRENTCH,		///< ���������������
		E_CT_LEFTDOWN_STRENTCH,		///< ���������������
		E_CT_RIGHTUP_STRENTCH,		///< ���������������
		E_CT_RIGHTDOWN_STRENTCH,	///< ���������������
		
		E_CT_SUPERLINK,				///< ������

		E_CT_ITEM_TARGET_ITEM,		///< ׼������Ʒʹ����Ʒ
		E_CT_ITEM_TARGET_OBJ,		///< ׼��������ʹ����Ʒ
		E_CT_ITEM_TARGET_POS,		///< ׼���Գ�������ʹ����Ʒ

		E_CT_WUHUN_XISHOU,			///< �������״̬
		E_CT_WUHUN_XISHOU_GRAY,		///< �������״̬(��ɫ״̬)

		E_CT_FENJIE,				///< �ֽ�
		E_CT_FENJIE_GRAY,			///< �ֽ�(��ɫ״̬)

		E_CT_JIANDING,				///< ����
		E_CT_JIANDING_GRAY,			///< ����(��ɫ״̬)

		E_CT_FINDPATH,				///< Ѱ·
		E_CT_FINDPATH_GRAY,			///< Ѱ·(��ɫ״̬)

		E_CT_CHAIFEN,				///< ���
		E_CT_CHAIFEN_GRAY,			///< ���(��ɫ״̬)

		E_CT_JIESUOBEIBAO,			///< ��������
		E_CT_JIESUOBEIBAO_GRAY,		///< ��������(��ɫ״̬)

		E_CT_EMAIL,					///< ����
		E_CT_EMAIL_GRAY,				///< ����(��ɫ״̬)

		E_TRANSLUCENT,				///< �û�
		E_TRANSLUCENT_GRAY,			///< �û�(��ɫ״̬)

		E_CT_TRANSLATE,				///< �༭ģʽ,ƽ��
		E_CT_ROTATE,				///< �༭ģʽ,��ת
		E_CT_SCALE,					///< �༭ģʽ,����
		E_CT_CREATE,				///< �༭ģʽ,����
		E_CT_GRAP_PRE,				///< �༭ģʽ,׼��ץȡ
		E_CT_GRAP_POST,				///< �༭ģʽ,ץȡ�ɹ�
		
		NUM_CURSORTYPES,
	};

public:
	sdCursorMgr();
	virtual ~sdCursorMgr();

	void AddCursor(uint uiID, const char* szCursorFile);

	// ��ǰ���
	void SetCursor(uint uiID);
	int  GetCursor();

	void SetVisiable(bool bVisiable);

	// ǿ�Ƹ���
	void ForceUpdateCursor();

protected:
	typedef std::vector<HCURSOR> CursorVec;
	typedef std::vector<HCURSOR>::iterator CursorVecItr;
	CursorVec	m_kCursorVec;
	int			m_iCurCursorId;
};
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_UI
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif