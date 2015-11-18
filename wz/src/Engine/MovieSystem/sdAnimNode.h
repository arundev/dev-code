//*************************************************************************************************
// 内容:	动画节点属性
//---------------------------------------------------------
// 作者:		
// 创建:		2012-01-09
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIMNODE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIMNODE_H__

#include <IAnimNode.h>
#include <IAnimTrack.h>

#include "sdAnimProperty.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// Streaming
//-----------------------------------------------------------------------------
struct stAnimPropTable 
{
	stAnimPropTable*		baseTable;
	const sdAnimProperty**	propTable;
};

class sdAnimNode;
typedef void (sdAnimNode::*AnimValueCallback)(const sdAnimValue*);
typedef std::map<const sdAnimProperty*, sdAnimValue*> AnimValueMap;	///< 属性集合
typedef  std::map<const sdAnimProperty*, sdAnimValue*>::iterator AnimValueMapItr;
typedef  std::map<const sdAnimProperty*, sdAnimValue*>::const_iterator AnimValueMapConItr;
typedef std::map<const sdAnimProperty*, AnimValueCallback> AnimValueFuncMap;	///< 属性操作函数集合
typedef std::map<const sdAnimProperty*, AnimValueCallback>::iterator AnimValueFuncMapItr;

#define DECLARE_ANIM_PROP(classname)								\
	protected:														\
		static stAnimPropTable			_##classname##PropTable;	\
		static const sdAnimProperty*	_##classname##Props[];		\
		AnimValueMap					_##classname##ValueMap;		\
		AnimValueFuncMap				_##classname##FuncMap;		\
	public:															\
		virtual stAnimPropTable*		GetAnimPropTable() const;	\
		virtual const AnimValueMap&		GetAnimValueMap() const;	\
		virtual AnimValueMap&			GetAnimValueMap();			\
		virtual const AnimValueFuncMap&	GetAnimFuncMap() const;		\
		virtual AnimValueFuncMap&		GetAnimFuncMap();			\

#define IMPLEMENT_ANIM_PROP(classname, basename)					\
	stAnimPropTable classname::_##classname##PropTable =			\
	{ &basename::_##basename##PropTable, _##classname##Props};		\
	stAnimPropTable*		GetAnimPropTable() const				\
	{ return &_##classname##PropTable;}								\
	const AnimValueMap&		GetAnimValueMap() const					\
	{ return _##classname##ValueMap;}								\
	AnimValueMap&			GetAnimValueMap()						\
	{ return _##classname##ValueMap;}								\
	const AnimValueFuncMap&	GetAnimFuncMap() const					\
	{ return _##classname##FuncMap;}								\
	AnimValueFuncMap&		GetAnimFuncMap()						\
	{ return _##classname##FuncMap;}								\
	const sdAnimProperty* classname::_##classname##Props[] =		\
	{	

#define ANIM_PROP(prop)	&prop##Property;

#define END_ANIM_PROP()			\
		NULL,					\
	};

#define REGISTER_ANIM_PROP(name, type)					\
	public:												\
		static const sdAnimProperty	name##Property;		\
		void Set##name##Property(type value)			\
		{ SetValue(&name##Property, value);}			\
		type Get##name##Property() const				\
		{type value; GetValue(&name##Property, value); return value;}

#define INIT_ANIM_PROP(classname, name, inherits, value)	\
	const sdAnimProperty classname::name##Property(#name, inherits, new sdAnimValue(value))

//-----------------------------------------------------------------------------
class sdAnimNode : public IAnimNode
{
	DECLARE_ANIM_PROP(sdAnimNode)

	REGISTER_ANIM_PROP(NodeName, std::string);
	REGISTER_ANIM_PROP(Position, NiPoint3);
	REGISTER_ANIM_PROP(Rotation, NiQuaternion);
	REGISTER_ANIM_PROP(Scale, float);
	REGISTER_ANIM_PROP(Visible, bool);
	REGISTER_ANIM_PROP(TimeDelay, float);
	REGISTER_ANIM_PROP(TimeScale, float);
	REGISTER_ANIM_PROP(TimeLength, float);
	REGISTER_ANIM_PROP(Flags, int);

public:
	sdAnimNode();
	virtual ~sdAnimNode();

	// 虚函数继承
	// @{
	virtual EAnimNodeType GetType();

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	virtual void Clear();
	virtual void UpdateFrame(float fAccumTime, float fFrameTime);

	virtual void AddChild(IAnimNode* pkNode) = 0;
	virtual void RemoveChild(IAnimNode* pkNode) = 0;
	virtual IAnimNode* GetChild() = 0;
	virtual IAnimNode* GetSibling() = 0;
	virtual IAnimNode* GetParent() = 0;

	virtual NiAVObject* GetEntity() = 0;

	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;
	// @}

	// 属性值操作
	// @{
	const sdAnimProperty* FindProperty(const std::string& strName) const;

	bool HasProperty(const std::string& strName) const;
	bool HasProperty(const sdAnimProperty* pkProp) const;

	template<class T> 
	inline void SetValue(const sdAnimProperty* pkProp, const T& kValue)
	{
		sdAnimValue* pkAnimValue = NULL;
		if (HasProperty(pkProp))
		{
			AnimValueMapItr itr = GetAnimValueMap().find(pkProp);
			AnimValueMapItr itr_end = GetAnimValueMap().end();
			if (itr != itr_end)
			{
				//if (!itr->second->GetIsTrackValue())
				//{
				//	// 手动设置具有最高优先级
				//	// 如果旧值是手动设置,并且现在是由Track自动设置的话,跳过
				//}
				//else
				{
					pkAnimValue = itr->second;
					pkAnimValue->Set(kValue);
					pkAnimValue->SetIsTrackValue(m_bIsUpdating);
				}
			}
			else
			{
				pkAnimValue = NiNew sdAnimValue(kValue);
				pkAnimValue->SetIsTrackValue(m_bIsUpdating);
				GetAnimValueMap()[pkProp] = pkAnimValue;
			}
		}

		if (pkAnimValue)
		{
			AnimValueFuncMapItr itr = GetAnimFuncMap().find(pkProp);
			AnimValueFuncMapItr itr_end = GetAnimFuncMap().end();
			if (itr != itr_end && itr->second != NULL)
			{
				AnimValueCallback pfFunc = itr->second;
				(this->*pfFunc)(pkAnimValue);
			}
		}
	};

	template<class T>
	inline void GetValue(const sdAnimProperty* pkProp, T& kValue) const
	{
		AnimValueMapConItr itr = GetAnimValueMap().find(pkProp);
		AnimValueMapConItr itr_end = GetAnimValueMap().end();
		if (itr != itr_end)
		{
			itr->second->Get(kValue);
		}
		else
		{
			if (pkProp->GetHerits() && m_pkParentNode)
				((sdAnimNode*)m_pkParentNode)->GetValue(pkProp, kValue);

			else
				pkProp->GetDefaultValue()->Get(kValue);
		}
	}

	inline void ClearValue(const sdAnimProperty* pkProp);
	inline void ClearAllValues();
	// @}

	// 字符串与enum类型之间的转换
	static const char* GetStringFromType(EAnimNodeType eType);
	static EAnimNodeType GetTypeFromString(const char* szName);

protected:
	// 节点树
	IAnimNode*	m_pkSiblingNode;	///< 兄弟节点
	IAnimNode*	m_pkChildNode;		///< 孩子节点
	IAnimNode*	m_pkParentNode;		///< 父节点

	// 节点控制器
	IAnimAffectorControl* m_pkAnimAffectorControl;

	// 节点世界姿态
	NiPoint3	 m_kWorldTranslate;
	NiQuaternion m_kWorldQuaternion;
	float		 m_fWorldScale;

	// 当前是否处于TrackUpdating状态(内部状态)
	bool m_bIsUpdating;
};
NiSmartPointer(sdAnimNode);
//-----------------------------------------------------------------------------
#include "sdAnimNode.inl"
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif