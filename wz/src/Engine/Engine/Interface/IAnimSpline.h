//*************************************************************************************************
// 内容:	样条(来自CryEngine)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-22
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIM_SPLINE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIM_SPLINE_H__

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// ESplineType
//-----------------------------------------------------------------------------
enum ESplineType
{
	E_SPLINE_STEP,
	E_SPLINE_LINEAR,
	E_SPLINE_CATMULLROM,
	E_SPLINE_HERMIT,
	E_SPLINE_TCB,
	E_SPLINE_BEZIER,
	E_SPLINE_B,
};
//-----------------------------------------------------------------------------
// These flags are mostly applicable for hermit based splines.
//-----------------------------------------------------------------------------
enum ESplineKeyTangentType
{
	E_SPLINE_KEY_TANGENT_DEFAULT = 0,
	E_SPLINE_KEY_TANGENT_CUSTOM  = 1,
	E_SPLINE_KEY_TANGENT_ZERO    = 2,
	E_SPLINE_KEY_TANGENT_STEP    = 3,
	E_SPLINE_KEY_TANGENT_LINEAR  = 4
};

#define SPLINE_KEY_TANGENT_IN_SHIFT		(0)
#define SPLINE_KEY_TANGENT_IN_MASK		(0x07)										///< 0000111
#define SPLINE_KEY_TANGENT_OUT_SHIFT	(3)
#define SPLINE_KEY_TANGENT_OUT_MASK		(0x07<<(SPLINE_KEY_TANGENT_OUT_SHIFT))		///< 0111000
#define SPLINE_KEY_TANGENT_UNIFY_SHIFT	(6)
#define SPLINE_KEY_TANGENT_UNIFY_MASK	(0x01<<(SPLINE_KEY_TANGENT_UNIFY_SHIFT))	///< 1000000

#define SPLINE_KEY_TANGENT_ALL_MASK (SPLINE_KEY_TANGENT_IN_MASK|SPLINE_KEY_TANGENT_OUT_MASK|SPLINE_KEY_TANGENT_UNIFY_MASK)
#define SPLINE_KEY_TANGENT_UNIFIED ((E_SPLINE_KEY_TANGENT_CUSTOM << SPLINE_KEY_TANGENT_IN_SHIFT) \
	|(E_SPLINE_KEY_TANGENT_CUSTOM << SPLINE_KEY_TANGENT_OUT_SHIFT)								 \
	|(0x01 << SPLINE_KEY_TANGENT_UNIFY_SHIFT))
#define SPLINE_KEY_TANGENT_BROKEN ((E_SPLINE_KEY_TANGENT_CUSTOM << SPLINE_KEY_TANGENT_IN_SHIFT) \
	|(E_SPLINE_KEY_TANGENT_CUSTOM << SPLINE_KEY_TANGENT_OUT_SHIFT)								\
	|(0x00 << SPLINE_KEY_TANGENT_UNIFY_SHIFT))

//-----------------------------------------------------------------------------
// ESplineKeyFlags
//-----------------------------------------------------------------------------
enum ESplineKeyFlags
{
	ESPLINE_KEY_UI_SELECTED_SHIFT = 16,
	ESPLINE_KEY_UI_SELECTED_MAX_DIMENSION_COUNT = 4,   // should be power of 2 (see ESPLINE_KEY_UI_SELECTED_MASK)
	ESPLINE_KEY_UI_SELECTED_MASK = ((1<<ESPLINE_KEY_UI_SELECTED_MAX_DIMENSION_COUNT)-1) << ESPLINE_KEY_UI_SELECTED_SHIFT
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif