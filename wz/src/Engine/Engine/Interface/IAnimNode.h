//*************************************************************************************************
// ����:	�����ڵ�ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-12-28
// ����޸�:
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
// AnimNode����
//-----------------------------------------------------------------------------
enum EAnimNodeType
{
	E_ANIM_NODE_INVALID,
	E_ANIM_NODE_ROOT,		///< ���ڵ�
	E_ANIM_NODE_GROUP,		///< ��ڵ�
	E_ANIM_NODE_EFFECT,		///< ��Ч�ڵ�
	E_ANIM_NODE_CAMERA,		///< ������ڵ�
	E_ANIM_NODE_DIRECTOR,	///< ������
	E_ANIM_NODE_ACTOR,		///< ��ɫ�ڵ�
	E_ANIM_NODE_SUBTITLE,	///< ��Ļ�ڵ�
	E_ANIM_NODE_POSTPROCESS,///< ����ڵ�
	E_ANIM_NODE_DECAL,		///< �����ڵ�
	E_ANIM_NODE_ROLE,		///< 
	E_ANIM_NODE_SOUND,		///< ��Ч�ڵ�
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
	//// �����ڵ�״̬
	//enum EAnimNodeStatusFlag
	//{
	//	E_ANIM_NSF_NOINIT	= 0x00000000,
	//	E_ANIM_NSF_PLAYING	= 0x00000001,
	//	E_ANIM_NSF_PAUSE	= 0x00000010,
	//	E_ANIM_NSF_STOP		= 0x00000100,
	//};

public:
	virtual ~IAnimNode() {};

	// ��ȡ����
	virtual EAnimNodeType GetType() = 0;

	// ����/����/����
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;

	// ��ʼ��������
	virtual bool Initialize();
	virtual void Destroy();

	// ��ȡ�����ò���(��Ҫ���ڱ༭���ͽű�)
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// ����
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// �ڵ���
	virtual void AddChild(IAnimNode* pkNode) = 0;
	virtual void RemoveChild(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetChild() = 0;

	//virtual void SetSibling(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetSibling() = 0;

	//virtual void SetParent(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetParent() = 0;

	// ί��ʵ��(Ŀǰ�õ���GB��Node)
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

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// ����/����AnimNode
	virtual IAnimNode* CreateAnimNode(EAnimNodeType eAnimNodeType) = 0;
	virtual void DestroyAnimNode(IAnimNode* pkANimNode) = 0;

	// ��ȡ�����ӿ�
	static IFactory* GetFactory(){ return InstancePtr();}
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif