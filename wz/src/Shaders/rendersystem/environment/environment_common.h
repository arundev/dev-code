//*************************************************************************************************
// ����:	������ɫ���Ĺ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-10-23
// ����޸�:	
//*************************************************************************************************
#ifndef _ENVIRONMENT_COMMON_H__
#define _ENVIRONMENT_COMMON_H__

//
#include "../common.h"

//*****************************************************************************
// ��ر���
//*****************************************************************************
// ��������
//	g_vSkyFogColorDensity.rgb	�������ɫ
//	g_vSkyFogColorDensity.a		�����ǿ��
//	g_vSkyFogTopBottom.x		-1.f / max((Top - Bottom), 0.001f)
//	g_vSkyFogTopBottom.y		Top / Bottom
float4		g_vSkyFogColorDensity		: GLOBAL;
float2		g_vSkyFogTopBottom			: GLOBAL;
#define		g_vSkyFogColor				g_vFogColorDensity.rgb
#define		g_fSkyFogDensity			g_vFogColorDensity.a


//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
float CalcSkyFogDensity(float3 vViewPos)
{
	return saturate(vViewPos.z * g_vSkyFogTopBottom.x + g_vSkyFogTopBottom.y) * g_fSkyFogDensity;
}
//-----------------------------------------------------------------------------
#endif