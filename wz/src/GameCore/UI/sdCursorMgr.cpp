#include "sdGameCorePCH.h"
#include "sdCursorMgr.h"
#include <sdCursorUtil.h>

using namespace Base::Util;

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
SD_GAMECORE_NAMESPACE_BEGIN_UI
//-------------------------------------------------------------------------------------------------
sdCursorMgr::sdCursorMgr()
: m_iCurCursorId(E_CT_NORMAL)
{
	m_kCursorVec.resize(NUM_CURSORTYPES, 0);
	
	AddCursor(E_CT_NORMAL,				"cursor\\normal.cur");
	AddCursor(E_CT_ATTACK,				"cursor\\attack.cur");
//	AddCursor(E_CT_ATTACK_GRAY,			"cursor\\attack_gray.cur");
	AddCursor(E_CT_LOOT,				"cursor\\loot.cur");
	AddCursor(E_CT_LOOT_GRAY,			"cursor\\loot_gray.cur");
	AddCursor(E_CT_TALK,				"cursor\\talk.cur");
	AddCursor(E_CT_TALK_GRAY,			"cursor\\talk_gray.cur");
	AddCursor(E_CT_READ,				"cursor\\read.cur");

	AddCursor(E_CT_GATHER_HERB,			"cursor\\gather_herb.cur");
	AddCursor(E_CT_GATHER_HERB_GRAY,	"cursor\\gather_herb_gray.cur");
	AddCursor(E_CT_GATHER_MINE,			"cursor\\gather_mine.cur");
	AddCursor(E_CT_GATHER_MINE_GRAY,	"cursor\\gather_mine_gray.cur");

	AddCursor(E_CT_REPAIR,				"cursor\\repair.cur");

	AddCursor(E_CT_BUY,					"cursor\\buy.cur");
	AddCursor(E_CT_BUY_GRAY,			"cursor\\buy_gray.cur");
	AddCursor(E_CT_WATCH,				"cursor\\watch.cur");
	AddCursor(E_CT_BOX,					"cursor\\box.cur");

//	AddCursor(E_CT_LEFT_STRENTCH,		"cursor\\normal.cur");
	AddCursor(E_CT_RIGHT_STRENTCH,		"cursor\\right_strentch.cur");
	AddCursor(E_CT_UP_STRENTCH,			"cursor\\up_strentch.cur");
//	AddCursor(E_CT_DOWN_STRENTCH,		"cursor\\normal.cur");
//	AddCursor(E_CT_LEFTUP_STRENTCH,		"cursor\\normal.cur");
//	AddCursor(E_CT_LEFTDOWN_STRENTCH,	"cursor\\normal.cur");
	AddCursor(E_CT_RIGHTUP_STRENTCH,	"cursor\\rightup_strentch.cur");
//	AddCursor(E_CT_RIGHTDOWN_STRENTCH,	"cursor\\normal.cur");

//	AddCursor(E_CT_SUPERLINK,			"cursor\\normal.cur");

	AddCursor(E_CT_ITEM_TARGET_ITEM,	"cursor\\curItemToItem.cur");
	AddCursor(E_CT_ITEM_TARGET_OBJ,		"cursor\\curItemToTarget.cur");
	AddCursor(E_CT_ITEM_TARGET_POS,		"cursor\\curItemToPos.cur");

	AddCursor(E_CT_WUHUN_XISHOU,		"cursor\\wuhun_xishou.cur");
//	AddCursor(E_CT_WUHUN_XISHOU_GRAY,	"cursor\\wuhun_xishou_gray.cur");

//	AddCursor(E_CT_FENJIE,				"cursor\\ofenjie.cur");
//	AddCursor(E_CT_FENJIE_GRAY,			"cursor\\ofenjie_gray.cur");

//	AddCursor(E_CT_JIANDING,			"cursor\\ojianding.cur");
//	AddCursor(E_CT_JIANDING_GRAY,		"cursor\\ojianding_gray.cur");

//	AddCursor(E_CT_FINDPATH,			"cursor\\find_path.cur");
//	AddCursor(E_CT_FINDPATH_GRAY,		"cursor\\find_path_gray.cur");

//	AddCursor(E_CT_CHAIFEN,				"cursor\\chaifen.cur");
//	AddCursor(E_CT_CHAIFEN_GRAY,		"cursor\\chaifen_gray.cur");

//	AddCursor(E_CT_JIESUOBEIBAO,		"cursor\\jiesuobeibao.cur");
//	AddCursor(E_CT_JIESUOBEIBAO_GRAY,	"cursor\\jiesuobeibao_gray.cur");

//	AddCursor(E_CT_EMAIL,				"cursor\\email.cur");
//	AddCursor(E_CT_EMAIL_GRAY,			"cursor\\email_gray.cur");

//	AddCursor(E_TRANSLUCENT,			"cursor\\huanhua.cur");
//	AddCursor(E_TRANSLUCENT_GRAY,		"cursor\\huanhua_gray.cur");

	AddCursor(E_CT_TRANSLATE,			"cursor\\curTranslate.cur");
	AddCursor(E_CT_ROTATE,				"cursor\\curRotate.cur");
	AddCursor(E_CT_SCALE,				"cursor\\curScale.cur");
	AddCursor(E_CT_CREATE,				"cursor\\normal.cur");
	AddCursor(E_CT_GRAP_PRE,			"cursor\\OpenHandle.cur");
	AddCursor(E_CT_GRAP_POST,			"cursor\\ClosedHandle.cur");

	m_kCursorVec[E_CT_SUPERLINK] = sdCursorUtil::LoadCursor(NULL, IDC_HAND);

	SetCursor(m_iCurCursorId);
}
//-------------------------------------------------------------------------------------------------
sdCursorMgr::~sdCursorMgr()
{
	CursorVecItr itr = m_kCursorVec.begin();
	CursorVecItr itr_end = m_kCursorVec.end();
	for (; itr!=itr_end; ++itr)
	{
		sdCursorUtil::DestroyCursor(*itr);
	}
}
//-------------------------------------------------------------------------------------------------
void sdCursorMgr::AddCursor(uint uiID, const char* szCursorFile)
{
	if (uiID < m_kCursorVec.size())
	{
		//std::string strFullFileName = GetWorldResPath("dependence::client\\", szCursorFile);
		//m_kCursorVec[iId] = sdCursorUtil::LoadCursor(strFullFileName.c_str());
		//if (m_kCursorVec[iId] == NULL)
		//{
		//	m_kCursorVec[iId] = sdCursorUtil::LoadCursor(NULL, IDC_ARROW);
		//}
	}
}
//-------------------------------------------------------------------------------------------------
void sdCursorMgr::SetCursor(uint uiID)
{
	if (uiID < m_kCursorVec.size())
	{
		m_iCurCursorId = (int)uiID;
		sdCursorUtil::SetCursor(m_kCursorVec[uiID]);
	}
}
//-------------------------------------------------------------------------------------------------
int sdCursorMgr::GetCursor()
{
	return m_iCurCursorId;
}	
//-------------------------------------------------------------------------------------------------
void sdCursorMgr::SetVisiable(bool bVisiable)
{
	sdCursorUtil::SafeShowCursor(bVisiable);
}
//-------------------------------------------------------------------------------------------------
void sdCursorMgr::ForceUpdateCursor()
{
	sdCursorUtil::SetCursor(m_kCursorVec[m_iCurCursorId]);
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_UI
SD_GAMECORE_NAMESPACE_END_GAMECORE