//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2014-04-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_MATRIX2_H__
#define _SD_BASE_MATH_MATRIX2_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdMatrix2 : public sdMemObject
{
public:
	union 
	{
		float m[2][2];			///< �������
		float _m[2];			///< �������
		struct  
		{
			float _m00, _m01;	///< ��������
			float _m10, _m11;	///< ��������
		};
		struct  
		{
			float _00, _01;		///< ��������
			float _10, _11;		///< ��������
		};
	};

public:
	static const sdMatrix2 ZERO;
	static const sdMatrix2 IDENTITY;

public:
	SD_INLINE sdMatrix2()
	{
	}

	SD_INLINE sdMatrix2(float m00, float m01, float m10, float m11)
		: _m00(m00), _m01(m01)
		, _m10(m10), _m11(m11)
	{
	}

	SD_INLINE sdMatrix2(const float* pfValue) 
	{
		::memcpy(m, pfValue, sizeof(m));
	}

	SD_INLINE sdMatrix2(const sdMatrix2& rkMatrix) 
	{
		::memcpy(m, &rkMatrix, sizeof(rkMatrix));
	}

	// ���������
	// @{

	// @}


};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif