//*************************************************************************************************
// 内容:	内建的Streaming与RTTI基类
//			 - 基于类属性getter/setter静态函数注册
//			 - 实现批量流化功能
//			 - 实现RTTI
//---------------------------------------------------------
// 资料：
//	1.关于对象属性方案(这里采用d)
//	  a.作为成员变量，每个变量通过各自的函数访问
//	  b.作为成员变量，每个变量通过统一的函数接口，通过字符串来索引访问
//	  c.作为一个字符串集合(哈希表), 每个变量通过统一的函数接口，通过字符串来索引访问
//	  d.作为一个通用类型集合, 每个变量通过统一的函数接口，通过字符串来索引访问
//	2.关于对象流化(这里采用b)
//	  a.在基类使用SaveStream/LoadStream虚函数接口，派生类实现这些接口(常用于文本和二进制保存)
//	  b.在基类实现属性表,对每种类型实现写入，减少派生类的工作量
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-10
// 最后修改:	2014-05-04
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_GAME_OBJECT_H__
#define _SD_ENGINE_SCENE_GAME_OBJECT_H__
#include "sdRTTI.h"
#include "sdStreamMacros.h"
#include "sdHashCode.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class ISizer;
ENGINE_NAMESPACE_END_ENGINE

// 最大游戏对象名长度
#define MAX_GAME_OBJECT_NAME_LEN 64

//-------------------------------------------------------------------------------------------------
// 属性操作
//-------------------------------------------------------------------------------------------------
// 关于集合类型属性set函数规范的说明:
// uint (sdGameObject::*PSET_PROP_XX_C)(const XX&, uint);
//	@return	返回值为集合中最后的最大元素数量
//	@param0	第一个参数为传入值的引用
//	@param1	第二个参数为欲设置到的索引
// 内部实现规范:
//	- INVALID_ELEMENT_INDEX < index	: 不做任何设置,直接返回集合最大元素数量
//	- INVALID_ELEMENT_INDEX >= index && CURRENT_ELEMENT_INDEX <= index : 直接将值添加到集合末尾,返回集合最大元素数量
//	- 0 <= index && index <= CURRENT_ELEMENT_INDEX : 替换已存在元素,返回集合最大元素数量
//-------------------------------------------------------------------------------------------------
class sdGameObject;

// 属性操作函数指针
typedef	void 			(sdGameObject::*PropFunc)(void);							///< 基础型

typedef	bool 			(sdGameObject::*PGET_PROP_BOOL)(void) const;				///< 考虑返回引用型
typedef	char 			(sdGameObject::*PGET_PROP_CHAR)(void) const;	
typedef	uchar 			(sdGameObject::*PGET_PROP_UCHAR)(void) const;	
typedef	int 			(sdGameObject::*PGET_PROP_INT)(void) const;	
typedef	uint 			(sdGameObject::*PGET_PROP_UINT)(void) const;
typedef	float 			(sdGameObject::*PGET_PROP_FLOAT)(void) const;
typedef	NiPoint2		(sdGameObject::*PGET_PROP_NIPT2)(void) const;			
typedef	NiPoint3		(sdGameObject::*PGET_PROP_NIPT3)(void) const;
typedef	NiColor			(sdGameObject::*PGET_PROP_NICOLOR)(void) const;
typedef	NiFixedString	(sdGameObject::*PGET_PROP_NISTRING)(void) const;
typedef	sdGameObject*	(sdGameObject::*PGET_PROP_SDOBJECT)(void) const;
typedef	uint			(sdGameObject::*PGET_PROP_NIPT3_C)(NiPoint3&, uint) const;	///< 集合,返回值为集合元素个数
typedef	uint			(sdGameObject::*PGET_PROP_NISTRING_C)(NiFixedString&, uint) const;
typedef	uint			(sdGameObject::*PGET_PROP_SDOBJECT_C)(sdGameObject*&, uint) const;

typedef	void 			(sdGameObject::*PSET_PROP_BOOL)(const bool&);
typedef	void 			(sdGameObject::*PSET_PROP_CHAR)(const char&);
typedef	void 			(sdGameObject::*PSET_PROP_UCHAR)(const uchar&);
typedef	void 			(sdGameObject::*PSET_PROP_INT)(const int&);
typedef	void 			(sdGameObject::*PSET_PROP_UINT)(const uint&);
typedef	void 			(sdGameObject::*PSET_PROP_FLOAT)(const float&);
typedef	void			(sdGameObject::*PSET_PROP_NIPT2)(const NiPoint2&);			
typedef	void			(sdGameObject::*PSET_PROP_NIPT3)(const NiPoint3&);
typedef	void			(sdGameObject::*PSET_PROP_NICOLOR)(const NiColor&);
typedef	void			(sdGameObject::*PSET_PROP_NISTRING)(const NiFixedString&);
typedef	void			(sdGameObject::*PSET_PROP_SDOBJECT)(const sdGameObject*&);
typedef	uint			(sdGameObject::*PSET_PROP_NIPT3_C)(const NiPoint3&, uint);	///< 集合,返回值为集合元素个数
typedef	uint			(sdGameObject::*PSET_PROP_NISTRING_C)(const NiFixedString&, uint);
typedef	uint			(sdGameObject::*PSET_PROP_SDOBJECT_C)(const sdGameObject*&, uint);

// 属性类型
#define PROP_UNKNOWN		0
#define PROP_BOOL			1
#define PROP_CHAR			2
#define PROP_UCHAR			3
#define PROP_INT			4
#define PROP_UINT			5
#define PROP_FLOAT			6
#define PROP_NIPT2			7
#define PROP_NIPT3			8
#define PROP_NICOLOR		9
#define PROP_NISTRING		10
#define PROP_SDOBJECT		11
#define PROP_NIPT3_C		12	///< 集合
#define PROP_NISTRING_C		13	///< 集合
#define PROP_SDOBJECT_C		14	///< 集合
#define PROP_TOTAL			15	///< 总数

#define	INVALID_ELEMENT_INDEX	16384	///< 2 << 14

// 属性操作函数返回值
union PropResult
{
	bool			bResult;
	char			cResult;
	uchar			ucResult;
	int				iResult;
	uint			uiResult;
	float			fResult;
	sdGameObject*	pkObjectResult;
};

// 属性设置函数
union PropSetterFunc
{
	PropFunc		pfn;
	void			(sdGameObject::*pfn_b)(const bool&);
	void			(sdGameObject::*pfn_c)(const char&);	
	void			(sdGameObject::*pfn_uc)(const uchar&);
	void			(sdGameObject::*pfn_i)(const int&);
	void			(sdGameObject::*pfn_ui)(const uint&);	
	void			(sdGameObject::*pfn_f)(const float&);			
	void			(sdGameObject::*pfn_nipt2)(const NiPoint2&);
	void			(sdGameObject::*pfn_nipt3)(const NiPoint3&);
	void			(sdGameObject::*pfn_nicolor)(const NiColor&);
	void			(sdGameObject::*pfn_nistring)(const NiFixedString&);
	void			(sdGameObject::*pfn_sdobject)(const sdGameObject*&);	
	void			(sdGameObject::*pfn_nipt3c)(const NiPoint3&, uint);	
	void			(sdGameObject::*pfn_nistringc)(const NiFixedString&, uint);	
	void			(sdGameObject::*pfn_sdobjectc)(const sdGameObject*&, uint);			
};

// 属性获取函数
union PropGetterFunc
{
	PropFunc		pfn;
	bool			(sdGameObject::*pfn_b)(void) const;
	char			(sdGameObject::*pfn_c)(void) const;	
	uchar			(sdGameObject::*pfn_uc)(void) const;
	int				(sdGameObject::*pfn_i)(void) const;
	uint			(sdGameObject::*pfn_ui)(void) const;	
	float			(sdGameObject::*pfn_f)(void) const;	
	NiPoint2		(sdGameObject::*pfn_nipt2)(void) const;	
	NiPoint3		(sdGameObject::*pfn_nipt3)(void) const;	
	NiColor			(sdGameObject::*pfn_nicolor)(void) const;
	NiFixedString	(sdGameObject::*pfn_nistring)(void) const;
	sdGameObject*	(sdGameObject::*pfn_sdobject)(void) const;
	uint			(sdGameObject::*pfn_nipt3c)(NiPoint3&, uint) const;	
	uint			(sdGameObject::*pfn_nistringc)(NiFixedString&, uint) const;	
	uint			(sdGameObject::*pfn_sdobjectc)(sdGameObject*&, uint) const;			
};

// 属性操作入口的属性
#define	PROP_ATTR_NONE		0
#define	PROP_ATTR_READ		1
#define	PROP_ATTR_WRITE		2
#define	PROP_ATTR_STREAM	4
#define	PROP_ATTR_UNUSED0	8
#define	PROP_ATTR_UNUSED1	16
#define	PROP_ATTR_UNUSED2	32
#define	PROP_ATTR_UNUSED3	64
#define	PROP_ATTR_FILEPATH	128

#define	PROP_ATTR_DEFAULT	PROP_ATTR_READ | PROP_ATTR_WRITE | PROP_ATTR_STREAM

// 属性操作入口
struct PropFuncEntry
{
	PropFunc	getter;		///< 属性操作入口的读取函数
	PropFunc	setter;		///< 属性操作入口的写入函数
	std::string	func_name;	///< 属性操作入口的函数名
	uchar		type;		///< 属性操作入口的类型
	uchar		flag;		///< 属性操作入口的属性
};

struct PropFuncMap
{
	const PropFuncMap*			basePropFuncMap;
	std::vector<PropFuncEntry>*	propFuncEntries;
};

//-------------------------------------------------------------------------------------------------
// 用于构造类属性getter/setter方法树的宏,将所有对象的setter/getter函数通过通用数组存储起来方便后续成批调用
//-------------------------------------------------------------------------------------------------
#define	DECLARE_PROPFUNC_MAP()										\
	private:														\
		static std::vector<PropFuncEntry>	ms_akPropFuncEntries;	\
	public:															\
		static const PropFuncMap ms_kPropFuncMap;					\
		virtual const PropFuncMap* GetPropFuncMap() const;

#define	IMPL_PROPFUNC_MAP(theClass, baseClass)						\
	const PropFuncMap* theClass::GetPropFuncMap() const	{ return &theClass::ms_kPropFuncMap;};						\
	const PropFuncMap theClass::ms_kPropFuncMap	= { &baseClass::ms_kPropFuncMap, &theClass::ms_akPropFuncEntries};	\
	std::vector<PropFuncEntry> theClass::ms_akPropFuncEntries;
																	
//-------------------------------------------------------------------------------------------------
// 用于声明和定义属性及其getter/setter的宏
//-------------------------------------------------------------------------------------------------	
///< 基础型属性	
#define	DECLARE_BASE_PROP(baseType, theProp)	\
public:											\
	baseType Get##theProp() const;				\
	void Set##theProp(const baseType& param);	\
protected:										\
	baseType m_k##theProp;
	
#define IMPLE_BASE_PROP(baseType, theClass, theProp)							\
	baseType theClass::Get##theProp() const{ return m_k##theProp;}				\
	void theClass::Set##theProp(const baseType& param){ m_k##theProp = param;}
	
///< 结构型属性(未完成)
#define	DECLARE_CLASS_PROP(theProp)					\
public:												\
	sdGameObject* Get##theProp() const;				\
	void Set##theProp(const sdGameObject*& param);	\
protected:										

///< 集合型属性(未完成)
#define	DECLARE_NIPT2COLLE_PROP(theProp)					\
public:														\
	uint Get##theProp(NiPoint2& value, uint index) const;	\
	void Set##theProp(const NiPoint2& param);				\
protected:													

#define	DECLARE_NIPT3COLLE_PROP(theProp)					\
public:														\
	uint Get##theProp(NiPoint3& value, uint index) const;	\
	void Set##theProp(const NiPoint3& param);				\
protected:													

#define	DECLARE_NISTRINGCOLLE_PROP(theProp)						\
public:															\
	uint Get##theProp(NiFixedString& value, uint index) const;	\
	void Set##theProp(const NiFixedString& param);				\
protected:

#define	DECLARE_SDOBJECTCOLLE_PROP(theProp)						\
public:															\
	uint Get##theProp(sdGameObject& value, uint index) const;	\
	void Set##theProp(const sdGameObject& param);				\
protected:														

// 用于注册属性getter/setter的宏	
#define BEGIN_PROPFUNC_MAP(theClass)				\
	static struct _##theClass##_REGISTER_PROPERTY	\
	{ 												\
		_##theClass##_REGISTER_PROPERTY()			\
		{

#define END_PROPFUNC_MAP(theClass)		\
		}								\
	}_##theClass##RegisterObject;		\
	
extern void RegisterPropFunction(std::vector<PropFuncEntry>& entries, const NiFixedString& funcName,\
		uchar type, uchar flag, PropFunc getter, PropFunc setter);
	
extern void UnregisterPropFunction(std::vector<PropFuncEntry>& entries, const NiFixedString& funcName);

#define REGISTER_PROPFUNC(theType, theFlag, theClass, theProp)		\
	RegisterPropFunction(*(theClass::ms_kPropFuncMap.propFuncEntries), #theProp, theType, theFlag, 	\
			(PropFunc)(static_cast<PGET_##theType>(&theClass::Get##theProp)),							\
			(PropFunc)(static_cast<PSET_##theType>(&theClass::Set##theProp)));
			
#define UNREGISTER_PROPFUNC(theClass, theProp)						\
	UnregisterPropFunction(*(theClass::ms_kPropFuncMap.propFuncEntries), #theProp);
	
// 实现Object自动流化基础功能
#define DEFINE_GAMEOBJECT_PROP(type, simple, whole)			\
	type Get##simple() const { return Get##whole();}			\
	void Set##simple(const type& val){ Set##whole(val);}
	
//-------------------------------------------------------------------------------------------------
// 属性宏
//-------------------------------------------------------------------------------------------------
#define	SET_GET(type, name)	\
	protected:				\
		type	m_k##name;	\
	public:					\
		void Set##name(const type& val) { m_k##name = val;}	\
		type Get##name() const { return m_k##name;}
		
#define	PUBLIC_SET_GET(type, name)	\
	public:							\
		type	m_k##name;			\
		void Set##name(const type& val) { m_k##name = val;}	\
		type Get##name() const { return m_k##name;}		
	
#define	SET__NOIMPL_GET(type, name)	\
	public:							\
		type	m_k##name;			\
		void Set##name(const type& val)	\
		type Get##name() const { return m_k##name;}		
	
#define	SET_NOIMPL_GET_NOIMPL(type, name)	\
	protected:								\
		type	m_k##name;					\
	public:									\
		void Set##name(const type& val);	\
		type Get##name() const;
		
#define	SET_NOIMPL_GET_NOIMPL_NOVAR(type, name)	\
	public:										\
		void Set##name(const type& val);		\
		type Get##name() const;
	
//-------------------------------------------------------------------------------------------------
// 游戏对象： 目前暂时继承自NiNode, 后期整理内存结构时去掉
//---------------------------------------------------------
// 游戏对象内部封装渲染对象，同时向逻辑层提供操作接口
//	1.游戏对象只需要单向将数据更新到渲染对象，目前采用的拉模型，即渲染器主动从游戏对象拉数据(区别于推)
//-------------------------------------------------------------------------------------------------
class sdGameObject : public NiNode
{
	// 只能暂时共存
	NiDeclareRTTI;
	NiDeclareStream;

	// Stream
	DECLARE_PROPFUNC_MAP()

public:
	sdGameObject();
	virtual ~sdGameObject();
	

	// RootRTTI相关变量/函数
	// @{
	static TypeClass ms_sdGameObjectRTTI;
	virtual TypeClass*	 GetTypeClass() const;
	static sdGameObject* CreateGameObject();
	static sdGameObject* FactoryCreateObject(const NiFixedString& szClassName);
	// @}
	

	// 属性
	//	1.Name(NM)
	//	2.Hash(CD)
	//	3.Type(TP)
	//	4.Tag(TG)(未实现)
	//	5.Layer(LY)(未实现)
	//	6.Enable(EN)(未实现)
	// @{
	//DEFINE_GAMEOBJECT_PROP(NiFixedString, NM, NM)
	inline NiFixedString	GetNM() const;
	inline void				SetNM(const NiFixedString&);

	//DEFINE_GAMEOBJECT_PROP(NiFixedString, CD, CD)
	inline NiFixedString	GetCD() const;
	inline void				SetCD(const NiFixedString&);

	//DEFINE_GAMEOBJECT_PROP(NiFixedString, TP, TP)
	inline NiFixedString	GetTP() const;
	inline void				SetTP(const NiFixedString&);	
	// @}
	

	inline NiFixedString	GetObjectName() const;
	inline void				SetObjectName(const NiFixedString&);

	inline NiFixedString	GetClassType() const;
	inline void				SetClassType(const NiFixedString&);		
	
	inline uint				GetHashCode() const;
	inline void				SetHashCode(const uint&);
		
	uint GetTypeSize(uchar);
	

	// 流化相关接口
	// @{
	// 通过属性名称设置与获取变量值
	template<typename T> 
	bool SetPropValueByName(const char* name, const T& value)
	{
		std::vector<PropFuncEntry> kVecEntries;
		GetPropertyEntries(kVecEntries);

		std::vector<PropFuncEntry>::iterator itr = kVecEntries.begin();
		std::vector<PropFuncEntry>::iterator itr_end = kVecEntries.end();
		for (; itr!= itr_end; ++itr)
		{
			if (itr->func_name == name)
			{
				typedef void (sdGameObject::*set_func)(const T&);
				set_func pfn = (set_func)(itr->setter);
				(this->*pfn)(value);

				return true;
			}
		}

		return false;
	}

	template<typename T> 
	bool GetPropValueByName(const char* name, T& value)
	{
		std::vector<PropFuncEntry> kVecEntries;
		GetPropertyEntries(kVecEntries);

		std::vector<PropFuncEntry>::iterator itr = kVecEntries.begin();
		std::vector<PropFuncEntry>::iterator itr_end = kVecEntries.end();
		for (; itr!= itr_end; ++itr)
		{
			if (itr->func_name == name)
			{
				typedef T (sdGameObject::*get_func)() const;
				get_func pfn = (get_func)(itr->getter);
				value = (this->*pfn)();

				return true;
			}
		}

		return false;
	}

	template<typename T> 
	uint GetPropValueByName(const char* name, const T& value, uint index)
	{
		std::vector<PropFuncEntry> kVecEntries;
		GetPropertyEntries(kVecEntries);

		uint uiElementNum = 0;
		std::vector<PropFuncEntry>::iterator itr = kVecEntries.begin();
		std::vector<PropFuncEntry>::iterator itr_end = kVecEntries.end();
		for (; itr!= itr_end; ++itr)
		{
			if (itr->func_name == name)
			{
				typedef void (sdGameObject::*get_func)(const T&, uint) const;
				get_func pfn = (get_func)(itr->getter);
				uiElementNum = (this->*pfn)(value, index);
				break;
			}
		}

		return uiElementNum;
	}

	uint GetPropValueByName(const char* name, uint index, void* outData, uint dataSize, uint& retDataSize);
	bool SetPropValueByName(const char* name, uint index, void* inData, uint dataSize);

	uchar GetPropTypeByName(const char* name);	///< 取的属性类型
	uchar GetPropAttrByName(const char* name);	///< 取的属性特性
	
	uint GetPropertyEntries(std::vector<PropFuncEntry>&);

	sdGameObject* CloneWithoutCreate();	///< 浅拷贝
	// @}

	// 虚函数
	virtual bool OnCreate() { return true;}
	
	// 提供内存分配接口(注意这里没有new/delete[]数组支持)
	//static void* operator new(size_t);
	//static void operator delete(void* , size_t);

	// 对象流化接口
	//virtual bool SaveLuaStream();
	//virtual bool LoadLuaStream();
	//virtual bool SavBinaryStream();
	//virtual bool LoadBinaryStream();

	// 内存统计
	virtual void GetMemoryUsage(Engine::ISizer& kSizer){}

protected:
//	char m_szName[MAX_GAME_OBJECT_NAME_LEN];	///< 游戏对象名称
	uint m_uiHashCode;							///< 游戏对象全局唯一哈希值
	
	static uint	ms_akTypeSizeTable[PROP_TOTAL];	///< 对象尺寸表
	static sdHashCode ms_kHashCodeGen;			///< 哈希生成器表
};
//-------------------------------------------------------------------------------------------------
#include "sdGameObject.inl"
#endif