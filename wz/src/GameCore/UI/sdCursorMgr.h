//*************************************************************************************************
// 内容:	硬件光标管理器
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-04
// 最后修改:	2013-01-04
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_CURSOR_MHR_H__
#define _SD_GAMECORE_CURSOR_MHR_H__
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
SD_GAMECORE_NAMESPACE_BEGIN_UI
//-------------------------------------------------------------------------------------------------
// 光标管理器
//	1.硬件光标：将光标图片设置给Windows系统，由GDI来绘制光标
//	2.软件光标：由D3D绘制光标
// 这里实现的是硬件光标
//-------------------------------------------------------------------------------------------------
class sdCursorMgr : public Base::sdTSingleton<sdCursorMgr>
{
public:
	// 光标类型
	enum ECursorType
	{
		E_CT_NORMAL = 0,			///< 普通
		E_CT_ATTACK,				///< 攻击
		E_CT_ATTACK_GRAY,			///< 攻击(远)
		E_CT_LOOT,					///< 拾取
		E_CT_LOOT_GRAY,				///< 拾取(远)
		E_CT_TALK,					///< NPC对话
		E_CT_TALK_GRAY,				///< NPC对话(远)
		E_CT_READ,					///< 阅读

		E_CT_GATHER_HERB,			///< 采集药草
		E_CT_GATHER_HERB_GRAY,		///< 采集药草(远)
		E_CT_GATHER_MINE,			///< 采矿
		E_CT_GATHER_MINE_GRAY,		///< 采矿(远)

		E_CT_REPAIR,				///< 修理

		E_CT_BUY,					///< 购买
		E_CT_BUY_GRAY,				///< 购买(远)
		E_CT_WATCH,					///< 精选
		E_CT_BOX,					///< 开启机关箱子

		E_CT_LEFT_STRENTCH,			///< 聊天框向左拉伸
		E_CT_RIGHT_STRENTCH,		///< 聊天框向右拉伸
		E_CT_UP_STRENTCH,			///< 聊天框向上拉伸
		E_CT_DOWN_STRENTCH,			///< 聊天框向下拉伸
		E_CT_LEFTUP_STRENTCH,		///< 聊天框向左上拉伸
		E_CT_LEFTDOWN_STRENTCH,		///< 聊天框向左下拉伸
		E_CT_RIGHTUP_STRENTCH,		///< 聊天框向右上拉伸
		E_CT_RIGHTDOWN_STRENTCH,	///< 聊天框向右下拉伸
		
		E_CT_SUPERLINK,				///< 超链接

		E_CT_ITEM_TARGET_ITEM,		///< 准备对物品使用物品
		E_CT_ITEM_TARGET_OBJ,		///< 准备对生物使用物品
		E_CT_ITEM_TARGET_POS,		///< 准备对场景坐标使用物品

		E_CT_WUHUN_XISHOU,			///< 武魂吸收状态
		E_CT_WUHUN_XISHOU_GRAY,		///< 武魂吸收状态(灰色状态)

		E_CT_FENJIE,				///< 分解
		E_CT_FENJIE_GRAY,			///< 分解(灰色状态)

		E_CT_JIANDING,				///< 鉴定
		E_CT_JIANDING_GRAY,			///< 鉴定(灰色状态)

		E_CT_FINDPATH,				///< 寻路
		E_CT_FINDPATH_GRAY,			///< 寻路(灰色状态)

		E_CT_CHAIFEN,				///< 拆分
		E_CT_CHAIFEN_GRAY,			///< 拆分(灰色状态)

		E_CT_JIESUOBEIBAO,			///< 解锁背包
		E_CT_JIESUOBEIBAO_GRAY,		///< 解锁背包(灰色状态)

		E_CT_EMAIL,					///< 邮箱
		E_CT_EMAIL_GRAY,				///< 邮箱(灰色状态)

		E_TRANSLUCENT,				///< 幻化
		E_TRANSLUCENT_GRAY,			///< 幻化(灰色状态)

		E_CT_TRANSLATE,				///< 编辑模式,平移
		E_CT_ROTATE,				///< 编辑模式,旋转
		E_CT_SCALE,					///< 编辑模式,缩放
		E_CT_CREATE,				///< 编辑模式,常见
		E_CT_GRAP_PRE,				///< 编辑模式,准备抓取
		E_CT_GRAP_POST,				///< 编辑模式,抓取成功
		
		NUM_CURSORTYPES,
	};

public:
	sdCursorMgr();
	virtual ~sdCursorMgr();

	void AddCursor(uint uiID, const char* szCursorFile);

	// 当前光标
	void SetCursor(uint uiID);
	int  GetCursor();

	void SetVisiable(bool bVisiable);

	// 强制更新
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