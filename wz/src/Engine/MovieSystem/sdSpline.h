//*************************************************************************************************
// 内容:	样条曲线(来自CryEngine/Common/ISpline.h)
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Interpolation
//		https://en.wikipedia.org/wiki/Spline_interpolation
//		https://en.wikipedia.org/wiki/Spline_(mathematics)
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//		https://en.wikipedia.org/wiki/Kochanek–Bartels spline
//		https://en.wikipedia.org/wiki/Bézier curve
//		https://en.wikipedia.org/wiki/B-spline
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-07
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_SPLINE_H__
#define _MOVIE_SYSTEM_ANIM_SPLINE_H__

#include <IAnimSpline.h>

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// Help Function,清零操作
//-----------------------------------------------------------------------------
template <typename T> inline void Zero(T& kValue){memset(&kValue, 0, sizeof(kValue));}
template <> inline void Zero(float& kValue){ kValue = 0.f;}
//-----------------------------------------------------------------------------
// ISplineKey,样条控制点
//-----------------------------------------------------------------------------
template <typename T>
class TSplineKey : public NiMemObject
{
public:
	typedef T value_type;

	float		m_fTime;		///< Key time
	int			m_iFlags;		///< Key flags
	value_type	m_kValue;		///< Key value
	value_type	m_kInTangent;	///< Incoming tangent
	value_type	m_kOutTangent;	///< Outgoing tangent

public:
	TSplineKey() : m_fTime(0.f), m_iFlags(0) {}

	// compare keys.
	bool operator<(const TSplineKey<T> &key) const { return m_fTime < key.m_fTime; }
	bool operator==(const TSplineKey<T> &key) const { return m_fTime == key.m_fTime; }
	bool operator>(const TSplineKey<T> &key) const { return m_fTime > key.m_fTime; }
	bool operator<=(const TSplineKey<T> &key) const { return m_fTime <= key.m_fTime; }
	bool operator>=(const TSplineKey<T> &key) const { return m_fTime >= key.m_fTime; }
	bool operator!=(const TSplineKey<T> &key) const { return m_fTime != key.m_fTime; }
};
//-----------------------------------------------------------------------------
// TTCBSplineKey
//-----------------------------------------------------------------------------
template <typename T>
class TTCBSplineKey : public TSplineKey<T>
{
public:
	float m_fTens;		///< Key tension value.
	float m_fCont;		///< Key continuity value.
	float m_fBias;		///< Key bias value.
	float m_fEaseTo;	///< Key ease to value.
	float m_fEaseFrom;	///< Key ease from value.

public:
	TTCBSplineKey() : m_fTens(0.f), m_fCont(0.f), m_fBias(0.f), m_fEaseFrom(0.f), m_fEaseTo(0.f){}
};
//-----------------------------------------------------------------------------
// TBezierSplineKey
//-----------------------------------------------------------------------------
template <typename T>
class TBezierSplineKey : public TSplineKey<T>
{

};
//-----------------------------------------------------------------------------
// TBasisFunction, 样条基函数
//-----------------------------------------------------------------------------
template <int N>
class TBasisFunction : public NiMemObject
{
public:
	const float& operator[](int i) const { return m_f[i]; }

protected:
	float m_f[N];
};
//-----------------------------------------------------------------------------
// Cubic HermitBasis.
//-----------------------------------------------------------------------------
class sdHermitBasis : public TBasisFunction<4>
{
public:
	sdHermitBasis(float t) 
	{
		const float t2 = t*t;	///< t2 = t^2
		const float t3 = t2*t;	///< t3 = t^3

		const float t3_2 = t3 + t3;		///< t3_2 = t^3 * 2
		const float t2_3 = 3 * t2;		///< t2_3 = t^2 * 3
		const float t3_t2 = t3 - t2;	///< t3_t2 = t^3 - t^2

		m_f[0] = t3_2 - t2_3 + 1;	///< 2 * t^3 - 3 * t^2 + 1
		m_f[1] = -t3_2 + t2_3;		///< -2 * t^3 + 3 * t^2
		m_f[2] = t3_t2 - t2 + t;	///< t^3 - t^2 - t^2 + t
		m_f[3] = t3_t2;				///< t3 - t2
	}
};
//-----------------------------------------------------------------------------
// Cubic BezierBasis.
//-----------------------------------------------------------------------------
class sdBezierBasis : public TBasisFunction<4>
{
public:
	sdBezierBasis(const float t) 
	{
		const float t2 = t * t;
		const float t3 = t2 * t;

		m_f[0] = -t3 + 3 * t2 - 3 * t + 1;	///< (1 - t) * (1 - t) * (1 - t)
		m_f[1] = 3 * t3 - 6 * t2 + 3 * t;	///< (1 - t) * (1 - t) * t
		m_f[2] = -3 * t3 + 3 * t2;			///< (1 - t) * t * t
		m_f[3] = t3;						///< t * t * t
	}
};

//-----------------------------------------------------------------------------
// TSpline, General Spline 
//-----------------------------------------------------------------------------
template <typename KeyType, typename BasisType>
class TSpline : public NiRefObject
{
public:
	typedef	KeyType							key_type;
	typedef typename KeyType::value_type	value_type;
	typedef	BasisType						basis_type;

public:
	TSpline(): m_uiFlags(0), m_ORT(0), m_curr(0), m_fRangeStart(0.f), m_fRangeEnd(0.f){};
	virtual	~TSpline(){};

	// 属性访问与编辑
	// @{
	inline void AddFlag(int iFlag);
	inline void RemoveFlag(int iFlag);
	inline int  GetFlag(int iFlag);

	inline void SetORT( int ort ) { m_ORT = ort; };
	inline int  GetORT() const { return m_ORT; };
	inline bool IsORT(int ort) const { return (m_ORT == ort); };

	inline void SetRange(float fStart,float fEnd);
	inline float GetRangeStart() const;
	inline float GetRangeEnd() const;
	// @}

	// Keys access methods.
	// @{
	inline void ReserveKeys(int n);		///< Reserve memory for more keys.
	inline void ClearKeys();
	inline void ResizeKeys(int num);	///< Set new key count.
	inline bool EmptyKeys() const;		///< Check if curve empty (no keys).
	inline int	NumKeys() const;		///< Return number of keys in curve.

	inline key_type& GetKey(int n);				///< Return n key.
	inline float& GetKeyTime(int n);			///< Shortcut to key n time.
	inline value_type& GetKeyValue(int n);		///< Shortcut to key n value.
	inline value_type& GetKeyInTangent(int n);	///< Shortcut to key n incoming tangent.
	inline value_type& GetKeyOutTangent(int n);	///< Shortcut to key n outgoing tangent.
	inline int& GetKeyFlags(int n);				///< Shortcut to key n flags.

	inline key_type& GetKey(int n) const;				///< Return n key.
	inline float& GetKeyTime(int n) const;				///< Shortcut to key n time.
	inline value_type& GetKeyValue(int n) const;		///< Shortcut to key n value.
	inline value_type& GetKeyInTangent(int n) const;	///< Shortcut to key n incoming tangent.
	inline value_type& GetKeyOutTangent(int n) const;	///< Shortcut to key n outgoing tangent.
	inline int& GetKeyFlags(int n) const;				///< Shortcut to key n flags.

	inline int GetInTangentType(int n) const;
	inline int GetOutTangentType(int n) const;

	inline void EraseKey(int n);
	inline void SortKeys();
	inline void PushBackKey(const key_type &kKey);
	inline int	InsertKey(const key_type &kKey);
	inline int	InsertKey(float fTime, value_type const& kValue);
	inline void FindKeysInRange(float fStartTime, float fEndTime, int& iFirstFoundKey, int& iNumFoundKeys) const;
	inline void RemoveKeysInRange(float fStartTime, float fEndTime);

	inline void SetModified(bool bOn, bool bSort = false);
	// @}

	// 更新
	inline void Update();
	inline bool IsUpdated() const;

	// Interpolate the value along the spline.
	inline bool Interpolate(float fTime, value_type& kValue);
	
	// This two functions must be overridden in derived spline classes.
	// @{
	// Pre-compute spline tangents.
	virtual void Preompute() = 0;

	// Interpolate value between two keys.
	virtual	void InterpolateKeys(int iKeyFrom, int iKeyTo, float u, value_type& kValue) = 0;
	// @}

protected:
	// Return key before or equal to this time.
	inline int SeekKey(float fTime);

	inline void AdjustTime(float &fTime);

protected:
	// Spline flag flags.
	enum EFlagType
	{
		E_FLAG_MODIFIED		=	0x0001,	///< Track modified.
		E_FLAG_MUST_SORT	=	0x0002,	///< Track modified and must be sorted.
	};

	// Out of range types.
	enum EORTType
	{
		E_ORT_CONSTANT			=	0x0001,	///< Constant track.
		E_ORT_CYCLE				=	0x0002,	///< Cycle track
		E_ORT_LOOP				=	0x0003,	///< Loop track.
		E_ORT_OSCILLATE			=	0x0004,	///< Oscillate track.
		E_ORT_LINEAR			=	0x0005,	///< Linear track.
		E_ORT_RELATIVE_REPEAT	=	0x0007,	///< Relative repeat track.
	};

protected:
	std::vector<key_type> m_kKeyVec;	///< List of keys.
	uint8 m_uiFlags;
	uint8 m_ORT;			///< Out-Of-Range type.
	int16 m_curr;			///< Current key in track.
	float m_fRangeStart;
	float m_fRangeEnd;
};

//-----------------------------------------------------------------------------
// Extended version of Hermit Spline.
// Provides more control on key tangents.
//-----------------------------------------------------------------------------
//template <class T,class Key = ISplineKey<T> >
//class sdHermitSplineEx : public TSpline<Key, sdHermitBasis>
//{
//public:
//	typedef TSpline<Key, sdHermitBasis> super_type;
//	using_type(super_type, key_type);
//	using_type(super_type, value_type);
//
//	virtual void comp_deriv()
//	{
//		this->SetModified( false );
//		if (this->num_keys() > 1)
//		{
//			int last = this->num_keys()-1;
//			key_type &k0 = this->key(0);
//			key_type &k1 = this->key(last);
//			Zero(k0.m_kInTangent);
//			Zero(k0.m_kOutTangent);
//			Zero(k1.m_kInTangent);
//			Zero(k1.m_kOutTangent);
//
//			Zero(k0.m_kInTangent);
//			k0.m_kOutTangent = (0.5f)*(this->value(1) - this->value(0));
//			k1.m_kInTangent = (0.5f)*(this->value(last) - this->value(last-1));
//			Zero(k1.m_kOutTangent);
//
//			for (int i = 1; i < (this->num_keys()-1); ++i)
//			{
//				key_type &key = this->key(i);
//				key.m_kInTangent = 0.5f*(this->value(i+1) - this->value(i-1));
//				key.m_kOutTangent = key.m_kInTangent;
//				switch (this->GetInTangentType(i))
//				{
//				case SPLINE_KEY_TANGENT_STEP:
//					key.m_kInTangent = value_type();
//					break;
//				case SPLINE_KEY_TANGENT_ZERO:
//					key.m_kInTangent = value_type();
//					break;
//				case SPLINE_KEY_TANGENT_LINEAR:
//					key.m_kInTangent = this->value(i) - this->value(i-1);
//					break;
//				}
//				switch (this->GetOutTangentType(i))
//				{
//				case SPLINE_KEY_TANGENT_STEP:
//					key.m_kOutTangent = value_type();
//					break;
//				case SPLINE_KEY_TANGENT_ZERO:
//					key.m_kOutTangent = value_type();
//					break;
//				case SPLINE_KEY_TANGENT_LINEAR:
//					key.m_kOutTangent = this->value(i+1) - this->value(i);
//					break;
//				}
//			}
//		}
//	}
//
//protected:
//	virtual	void interp_keys( int from, int to, float u, T& val )
//	{
//		if (this->GetInTangentType(to) == SPLINE_KEY_TANGENT_STEP || this->GetOutTangentType(from) == SPLINE_KEY_TANGENT_STEP)
//		{
//			val = this->value(from);
//			return;
//		}
//		typename TSpline<Key, sdHermitBasis >::basis_type basis( u );
//		val = (basis[0] * this->value(from)) + (basis[1] * this->value(to)) + (basis[2] * this->dd(from)) + (basis[3] * this->ds(to));
//	}
//};
//-----------------------------------------------------------------------------
// Bezier Spline.
//-----------------------------------------------------------------------------
template <class T, class Key = TSplineKey<T> >
class TBezierSpline : public TSpline<Key, sdBezierBasis>
{
public:
	typedef TSpline<Key, sdBezierBasis>		super_type;
	typedef typename super_type::key_type	key_type;
	typedef typename super_type::value_type value_type;

public:
	// 虚函数继承
	virtual void Preompute()
	{
		SetModified(true);

		int iNum = NumKeys();
		if (iNum > 1)
		{
			const float fOneThird = 1 / 3.0f;
			const int iLast = iNum - 1;

			// 初始化两侧切线
			if (GetInTangentType(0) != E_SPLINE_KEY_TANGENT_CUSTOM)
				Zero(GetKey(0).m_kInTangent);
			if (GetOutTangentType(0) != E_SPLINE_KEY_TANGENT_CUSTOM)
				GetKey(0).m_kOutTangent = fOneThird * (GetKeyValue(1) - GetKeyValue(0));

			if (GetInTangentType(iLast) != E_SPLINE_KEY_TANGENT_CUSTOM)
				GetKey(iLast).m_kInTangent = fOneThird * (GetKeyValue(iLast) - GetKeyValue(iLast-1));
			if (GetOutTangentType(iLast) != E_SPLINE_KEY_TANGENT_CUSTOM)
				Zero(GetKey(iLast).m_kOutTangent);

			for (int i = 1; i < iLast; ++i)
			{
				key_type& kKey = GetKey(i);
				T ds0 = kKey.m_kInTangent;
				T dd0 = kKey.m_kOutTangent;

				const float fDeltaTime = GetKeyTime(i+1) - GetKeyTime(i-1);
				if(fDeltaTime <= 0)
				{
					Zero(kKey.m_kInTangent);
					Zero(kKey.m_kOutTangent);
				}
				else
				{
					const float k = (GetKeyTime(i) - GetKeyTime(i-1)) / fDeltaTime;
					const value_type fDeltaValue = GetKeyValue(i+1) - GetKeyValue(i-1);
					kKey.m_kInTangent = fOneThird * fDeltaValue * k;
					kKey.m_kOutTangent = fOneThird * fDeltaValue * (1-k);
				}

				switch (this->GetInTangentType(i))
				{
				case E_SPLINE_KEY_TANGENT_STEP:
					Zero(kKey.m_kInTangent);
					break;
				case E_SPLINE_KEY_TANGENT_ZERO:
					Zero(kKey.m_kInTangent);
					break;
				case E_SPLINE_KEY_TANGENT_LINEAR:
					kKey.m_kInTangent = fOneThird * (GetKeyValue(i) - GetKeyValue(i-1));
					break;
				case E_SPLINE_KEY_TANGENT_CUSTOM:
					kKey.m_kInTangent = ds0;
					break;
				}

				switch (this->GetOutTangentType(i))
				{
				case E_SPLINE_KEY_TANGENT_STEP:
					Zero(kKey.m_kOutTangent);
					break;
				case E_SPLINE_KEY_TANGENT_ZERO:
					Zero(kKey.m_kOutTangent);
					break;
				case E_SPLINE_KEY_TANGENT_LINEAR:
					kKey.m_kOutTangent = fOneThird * (GetKeyValue(i+1) - GetKeyValue(i));
					break;
				case E_SPLINE_KEY_TANGENT_CUSTOM:
					kKey.m_kOutTangent = dd0;
					break;
				}
			}
		}
	}

protected:
	// 虚函数继承
	virtual	void InterpolateKeys(int iKeyFrom, int iKeyTo, float u, value_type& kValue);
};
//-----------------------------------------------------------------------------
#include "sdSpline.h"
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif