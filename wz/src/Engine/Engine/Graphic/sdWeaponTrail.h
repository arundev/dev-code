//*************************************************************************************************
// 内容:	刀光拖尾(实现2)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-01-16
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_GRAPHIC_WEAPONTRAIL_H__
#define _SD_ENGINE_GRAPHIC_WEAPONTRAIL_H__

ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// sdWeaponTrail
//	1.控制点部分适用环形缓存,以减少memcpy
//	2.插值部分每次只对新加控制点进行插值以减少计算
//-----------------------------------------------------------------------------
class sdWeaponTrail : public NiNode
{
	NiDeclareRTTI;
public:
	sdWeaponTrail();
	~sdWeaponTrail();

	// 播放/更新/终止
	void Start();
	void Stop();
	void UpdateFrame(float fAccumTime, float fFrameTime);

	// Dummy点设置
	inline void SetDummyPosition(const NiPoint3& kPos);
	inline void SetDummyRotation(const NiMatrix3& kRot);
	

protected:

	// B-Spline插值
	void bspline();

	//
	// De Boor's algorithm, 一种B样条插值算法
	// 参考:
	//	https://zh.wikipedia.org/wiki/德布尔算法
	//	https://en.wikipedia.org/wiki/De Boor's algorithm
	//
	// @param	
	void deBoor();

protected:
	float m_fAccumFrameTime;	///< 上一次更新到现在的累积时间(用于限制防止更新速率过高)

	bool m_bStarted;	///< 当前是否已经开始刀光
	bool m_bStoping;	///< 当前是否正在结束刀光(用于刀光收尾)

	NiPoint3  m_kDummyPosition;		///< Dummy目标位置
	NiPoint3  m_kDummyZDirection;	///< Dummy目标Z轴方向;

	float m_fBias;			///< 刀光绑定位置偏移
	float m_fWidth;			///< 刀光宽度
	float m_fSegmentLength;	///< 刀光插值间距(单位:m)
	float m_fMaxLength;		///< 刀光最大长度(单位:m)
	int m_iMaxSampleNum;	///< 刀光最大采样点数量

	float m_fAlpha;		///< 刀光透明度
	NiColor m_kColor;	///< 刀光颜色

	NiPoint2 m_kUVScale;	///< UV缩放
	NiPoint2 m_kUBBias;		///< UV偏移

	bool m_bShimmer;	///< Shimmer效果
	float m_fShimmerIntensity;

	typedef std::vector<float> LengthVec;
	typedef std::vector<float>::iterator LengthVecItr;
	typedef std::vector<NiPoint3> Pt3Vec;
	typedef std::vector<NiPoint3>::iterator Pt3VecItr;
	Pt3Vec		m_kCtrlPtVec0;			///< 控制点缓存
	Pt3Vec		m_kCtrlPtVec1;			///< 控制点缓存
	LengthVec	m_kCtrlSegmentLength;	///< 控制点之间距离数组
	int			m_iCtrlPtNum;			///< 当前控制点数量
	int			m_iStartCtrlPtIndex;	///< 当前控制点起始位置(回绕式更新)

	// Mesh对象与绘制参数
	NiMeshPtr m_spMesh;
	NiFloatsExtraDataPtr m_spParams;
};
NiSmartPointer(sdWeaponTrail);
//-----------------------------------------------------------------------------
#include "sdWeaponTrail.h"
ENGINE_NAMESPACE_END_RENDERSYSTEM
#endif
