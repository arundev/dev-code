//*************************************************************************************************
// 内容:	世界编辑状态机
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_WORLDEDITFSM_H__
#define _SD_GAMEEDITEX_WORLDEDITFSM_H__
#include <sdEditFSM.h>
#include <sdEventArg.h>

#include "sdEditHelper.h"
#include "sdEditGizmoHelper.h"
#include "sdEditTerrainHelper.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 世界编辑状态机
//-------------------------------------------------------------------------------------------------
class sdWorldEditFSM : public sdEditFSM
{
public:
	sdWorldEditFSM();
	virtual ~sdWorldEditFSM();

	// 虚函数继承(继承自Base::sdFSM)
	virtual int UpdateState();

	// 虚函数继承(继承自sdEditFSM)
	virtual bool Initialize();
	virtual void Destroy();

	// 属性访问
	sdEditHelper* GetEditHelper() const { return m_pkEditHelper;}
	sdEditGizmoHelper* GetEditGizmoHelper() const { return m_pkEditGizmoHelper;}
	sdEditTerrainHelper* GetEditTerrainHelper() const { return m_pkEditTerrainHelper;}

protected:
	// 事件处理,用于与Event绑定
	virtual bool OnEditModeSelect(const GameCore::stEventArg& kArgs);
	virtual bool OnEditModeTranslate(const GameCore::stEventArg& kArgs);
	virtual bool OnEditModeRotate(const GameCore::stEventArg& kArgs);
	virtual bool OnEditModeScale(const GameCore::stEventArg& kArgs);
	virtual bool OnEditModeTerrainDeform(const GameCore::stEventArg& kArgs);
	virtual bool OnEditModeTerrainSurface(const GameCore::stEventArg& kArgs);

protected:
	sdEditHelperPtr			m_pkEditHelper;
	sdEditGizmoHelperPtr	m_pkEditGizmoHelper;
	sdEditTerrainHelperPtr	m_pkEditTerrainHelper;
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif