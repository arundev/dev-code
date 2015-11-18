//*************************************************************************************************
// ����:	Ray,��ȡ��Ogre/OgreRay.h
//---------------------------------------------------------
// ����:		
// ����:		2012-08-30
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_RAY_H__
#define _SD_BASE_MATH_RAY_H__
#include "sdMath.h"
#include "sdVector3.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdRay : public sdMemObject
{
protected:
	sdVector3	m_kOrigin;
	sdVector3	m_kDirection;

public:
	sdRay(): m_kOrigin(sdVector3::ZERO), m_kDirection(sdVector3::UNIT_Z) {}
	sdRay(const sdVector3& kOrigin, const sdVector3& kDir): m_kOrigin(kOrigin), m_kDirection(kDir) {}


	// ���Է���������
	// @{
	void SetOrigin(const sdVector3& kOrigin) {m_kOrigin = kOrigin;} 
	const sdVector3& GetOrigin() const {return m_kOrigin;} 

	void SetDirection(const sdVector3& kDir) {m_kDirection = kDir;} 
	const sdVector3& GetDirection() const {return m_kDirection;} 
	// @}


	// ��
	// @{
	

	// @}
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif