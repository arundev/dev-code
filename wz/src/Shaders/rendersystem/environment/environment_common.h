//*************************************************************************************************
// 内容:	环境着色器的公共部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-23
// 最后修改:	
//*************************************************************************************************
#ifndef _ENVIRONMENT_COMMON_H__
#define _ENVIRONMENT_COMMON_H__

//
#include "../common.h"

//*****************************************************************************
// 相关变量
//*****************************************************************************
// 天空雾参数
//	g_vSkyFogColorDensity.rgb	天空雾颜色
//	g_vSkyFogColorDensity.a		天空雾强度
//	g_vSkyFogTopBottom.x		-1.f / max((Top - Bottom), 0.001f)
//	g_vSkyFogTopBottom.y		Top / Bottom
float4		g_vSkyFogColorDensity		: GLOBAL;
float2		g_vSkyFogTopBottom			: GLOBAL;
#define		g_vSkyFogColor				g_vFogColorDensity.rgb
#define		g_fSkyFogDensity			g_vFogColorDensity.a


//-----------------------------------------------------------------------------
// 计算天空雾
//-----------------------------------------------------------------------------
float CalcSkyFogDensity(float3 vViewPos)
{
	return saturate(vViewPos.z * g_vSkyFogTopBottom.x + g_vSkyFogTopBottom.y) * g_fSkyFogDensity;
}
//-----------------------------------------------------------------------------
#endif