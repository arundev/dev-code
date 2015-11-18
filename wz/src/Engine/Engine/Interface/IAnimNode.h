//*************************************************************************************************
// 内容:	动画节点接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_IANIMNODE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_IANIMNODE_H__

#include <IFactory.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
class sdLuaReadUtil;
class sdLuaWriteUtil;
ENGINE_NAMESPACE_END_UTIL
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// AnimNode类型
//-----------------------------------------------------------------------------
enum EAnimNodeType
{
	E_ANIM_NODE_INVALID,
	E_ANIM_NODE_ROOT,		///< 根节点
	E_ANIM_NODE_GROUP,		///< 组节点
	E_ANIM_NODE_EFFECT,		///< 特效节点
	E_ANIM_NODE_CAMERA,		///< 摄相机节点
	E_ANIM_NODE_DIRECTOR,	///< 导演组
	E_ANIM_NODE_ACTOR,		///< 角色节点
	E_ANIM_NODE_SUBTITLE,	///< 字幕节点
	E_ANIM_NODE_POSTPROCESS,///< 后处理节点
	E_ANIM_NODE_DECAL,		///< 贴花节点
	E_ANIM_NODE_ROLE,		///< 
	E_ANIM_NODE_SOUND,		///< 音效节点
	E_ANIM_NODE_SCENEENTITY,///< 
	E_ANIM_NODE_REVERB,
	NUM_ANIM_NODES,
};
//-----------------------------------------------------------------------------
// IAnimNode
//-----------------------------------------------------------------------------
class IAnimNode : public NiRefObject
{
public:
	//// 动画节点状态
	//enum EAnimNodeStatusFlag
	//{
	//	E_ANIM_NSF_NOINIT	= 0x00000000,
	//	E_ANIM_NSF_PLAYING	= 0x00000001,
	//	E_ANIM_NSF_PAUSE	= 0x00000010,
	//	E_ANIM_NSF_STOP		= 0x00000100,
	//};

public:
	virtual ~IAnimNode() {};

	// 获取类型
	virtual EAnimNodeType GetType() = 0;

	// 加载/保存/清理
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;

	// 初始化与销毁
	virtual bool Initialize();
	virtual void Destroy();

	// 获取与设置参数(主要用于编辑器和脚本)
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// 更新
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// 节点树
	virtual void AddChild(IAnimNode* pkNode) = 0;
	virtual void RemoveChild(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetChild() = 0;

	//virtual void SetSibling(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetSibling() = 0;

	//virtual void SetParent(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetParent() = 0;

	// 委托实体(目前用的是GB的Node)
	virtual NiAVObject* GetEntity() = 0;

	//virtual void SetPosition(const NiPoint3& kPosition) = 0;
	//virtual void GetPosition(NiPoint3& kPosition) = 0;
};
NiSmartPointer(IAnimNode);

//-----------------------------------------------------------------------------
// IAnimNodeFactory
//-----------------------------------------------------------------------------
class IAnimNodeFactory : public Base::Misc::IFactory
{
public:
	virtual ~IAnimNodeFactory() {};

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// 创建/销毁AnimNode
	virtual IAnimNode* CreateAnimNode(EAnimNodeType eAnimNodeType) = 0;
	virtual void DestroyAnimNode(IAnimNode* pkANimNode) = 0;

	// 获取工厂接口
	static IFactory* GetFactory(){ return InstancePtr();}
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif