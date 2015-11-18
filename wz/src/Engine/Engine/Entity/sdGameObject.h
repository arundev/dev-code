//*************************************************************************************************
// ����:	�ڽ���Streaming��RTTI����
//			 - ����������getter/setter��̬����ע��
//			 - ʵ��������������
//			 - ʵ��RTTI
//---------------------------------------------------------
// ���ϣ�
//	1.���ڶ������Է���(�������d)
//	  a.��Ϊ��Ա������ÿ������ͨ�����Եĺ�������
//	  b.��Ϊ��Ա������ÿ������ͨ��ͳһ�ĺ����ӿڣ�ͨ���ַ�������������
//	  c.��Ϊһ���ַ�������(��ϣ��), ÿ������ͨ��ͳһ�ĺ����ӿڣ�ͨ���ַ�������������
//	  d.��Ϊһ��ͨ�����ͼ���, ÿ������ͨ��ͳһ�ĺ����ӿڣ�ͨ���ַ�������������
//	2.���ڶ�������(�������b)
//	  a.�ڻ���ʹ��SaveStream/LoadStream�麯���ӿڣ�������ʵ����Щ�ӿ�(�������ı��Ͷ����Ʊ���)
//	  b.�ڻ���ʵ�����Ա�,��ÿ������ʵ��д�룬����������Ĺ�����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:	2014-05-04
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

// �����Ϸ����������
#define MAX_GAME_OBJECT_NAME_LEN 64

//-------------------------------------------------------------------------------------------------
// ���Բ���
//-------------------------------------------------------------------------------------------------
// ���ڼ�����������set�����淶��˵��:
// uint (sdGameObject::*PSET_PROP_XX_C)(const XX&, uint);
//	@return	����ֵΪ�������������Ԫ������
//	@param0	��һ������Ϊ����ֵ������
//	@param1	�ڶ�������Ϊ�����õ�������
// �ڲ�ʵ�ֹ淶:
//	- INVALID_ELEMENT_INDEX < index	: �����κ�����,ֱ�ӷ��ؼ������Ԫ������
//	- INVALID_ELEMENT_INDEX >= index && CURRENT_ELEMENT_INDEX <= index : ֱ�ӽ�ֵ��ӵ�����ĩβ,���ؼ������Ԫ������
//	- 0 <= index && index <= CURRENT_ELEMENT_INDEX : �滻�Ѵ���Ԫ��,���ؼ������Ԫ������
//-------------------------------------------------------------------------------------------------
class sdGameObject;

// ���Բ�������ָ��
typedef	void 			(sdGameObject::*PropFunc)(void);							///< ������

typedef	bool 			(sdGameObject::*PGET_PROP_BOOL)(void) const;				///< ���Ƿ���������
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
typedef	uint			(sdGameObject::*PGET_PROP_NIPT3_C)(NiPoint3&, uint) const;	///< ����,����ֵΪ����Ԫ�ظ���
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
typedef	uint			(sdGameObject::*PSET_PROP_NIPT3_C)(const NiPoint3&, uint);	///< ����,����ֵΪ����Ԫ�ظ���
typedef	uint			(sdGameObject::*PSET_PROP_NISTRING_C)(const NiFixedString&, uint);
typedef	uint			(sdGameObject::*PSET_PROP_SDOBJECT_C)(const sdGameObject*&, uint);

// ��������
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
#define PROP_NIPT3_C		12	///< ����
#define PROP_NISTRING_C		13	///< ����
#define PROP_SDOBJECT_C		14	///< ����
#define PROP_TOTAL			15	///< ����

#define	INVALID_ELEMENT_INDEX	16384	///< 2 << 14

// ���Բ�����������ֵ
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

// �������ú���
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

// ���Ի�ȡ����
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

// ���Բ�����ڵ�����
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

// ���Բ������
struct PropFuncEntry
{
	PropFunc	getter;		///< ���Բ�����ڵĶ�ȡ����
	PropFunc	setter;		///< ���Բ�����ڵ�д�뺯��
	std::string	func_name;	///< ���Բ�����ڵĺ�����
	uchar		type;		///< ���Բ�����ڵ�����
	uchar		flag;		///< ���Բ�����ڵ�����
};

struct PropFuncMap
{
	const PropFuncMap*			basePropFuncMap;
	std::vector<PropFuncEntry>*	propFuncEntries;
};

//-------------------------------------------------------------------------------------------------
// ���ڹ���������getter/setter�������ĺ�,�����ж����setter/getter����ͨ��ͨ������洢�������������������
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
// ���������Ͷ������Լ���getter/setter�ĺ�
//-------------------------------------------------------------------------------------------------	
///< ����������	
#define	DECLARE_BASE_PROP(baseType, theProp)	\
public:											\
	baseType Get##theProp() const;				\
	void Set##theProp(const baseType& param);	\
protected:										\
	baseType m_k##theProp;
	
#define IMPLE_BASE_PROP(baseType, theClass, theProp)							\
	baseType theClass::Get##theProp() const{ return m_k##theProp;}				\
	void theClass::Set##theProp(const baseType& param){ m_k##theProp = param;}
	
///< �ṹ������(δ���)
#define	DECLARE_CLASS_PROP(theProp)					\
public:												\
	sdGameObject* Get##theProp() const;				\
	void Set##theProp(const sdGameObject*& param);	\
protected:										

///< ����������(δ���)
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

// ����ע������getter/setter�ĺ�	
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
	
// ʵ��Object�Զ�������������
#define DEFINE_GAMEOBJECT_PROP(type, simple, whole)			\
	type Get##simple() const { return Get##whole();}			\
	void Set##simple(const type& val){ Set##whole(val);}
	
//-------------------------------------------------------------------------------------------------
// ���Ժ�
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
// ��Ϸ���� Ŀǰ��ʱ�̳���NiNode, ���������ڴ�ṹʱȥ��
//---------------------------------------------------------
// ��Ϸ�����ڲ���װ��Ⱦ����ͬʱ���߼����ṩ�����ӿ�
//	1.��Ϸ����ֻ��Ҫ�������ݸ��µ���Ⱦ����Ŀǰ���õ���ģ�ͣ�����Ⱦ����������Ϸ����������(��������)
//-------------------------------------------------------------------------------------------------
class sdGameObject : public NiNode
{
	// ֻ����ʱ����
	NiDeclareRTTI;
	NiDeclareStream;

	// Stream
	DECLARE_PROPFUNC_MAP()

public:
	sdGameObject();
	virtual ~sdGameObject();
	

	// RootRTTI��ر���/����
	// @{
	static TypeClass ms_sdGameObjectRTTI;
	virtual TypeClass*	 GetTypeClass() const;
	static sdGameObject* CreateGameObject();
	static sdGameObject* FactoryCreateObject(const NiFixedString& szClassName);
	// @}
	

	// ����
	//	1.Name(NM)
	//	2.Hash(CD)
	//	3.Type(TP)
	//	4.Tag(TG)(δʵ��)
	//	5.Layer(LY)(δʵ��)
	//	6.Enable(EN)(δʵ��)
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
	

	// ������ؽӿ�
	// @{
	// ͨ�����������������ȡ����ֵ
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

	uchar GetPropTypeByName(const char* name);	///< ȡ����������
	uchar GetPropAttrByName(const char* name);	///< ȡ����������
	
	uint GetPropertyEntries(std::vector<PropFuncEntry>&);

	sdGameObject* CloneWithoutCreate();	///< ǳ����
	// @}

	// �麯��
	virtual bool OnCreate() { return true;}
	
	// �ṩ�ڴ����ӿ�(ע������û��new/delete[]����֧��)
	//static void* operator new(size_t);
	//static void operator delete(void* , size_t);

	// ���������ӿ�
	//virtual bool SaveLuaStream();
	//virtual bool LoadLuaStream();
	//virtual bool SavBinaryStream();
	//virtual bool LoadBinaryStream();

	// �ڴ�ͳ��
	virtual void GetMemoryUsage(Engine::ISizer& kSizer){}

protected:
//	char m_szName[MAX_GAME_OBJECT_NAME_LEN];	///< ��Ϸ��������
	uint m_uiHashCode;							///< ��Ϸ����ȫ��Ψһ��ϣֵ
	
	static uint	ms_akTypeSizeTable[PROP_TOTAL];	///< ����ߴ��
	static sdHashCode ms_kHashCodeGen;			///< ��ϣ��������
};
//-------------------------------------------------------------------------------------------------
#include "sdGameObject.inl"
#endif