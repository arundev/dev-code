//*************************************************************************************************
// ����:	ˮ��ɫ���Ĺ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-10-04
// ����޸�:	
//*************************************************************************************************
#ifndef _WATER_COMMON_H__
#define _WATER_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// World Space sun light direction
float3	g_vSunLightDir		: GLOBAL;



//float	g_fMaxRefractAlpha	: GLOBAL;

float4	g_vFresnelParams		: GLOBAL;
#define	g_fNormalFadingDistance	g_vFresnelParams.x
#define	g_fWaterFresnel			g_vFresnelParams.y
#define g_fWaterNormalScale		g_vFresnelParams.z
#define g_fSpecularScatter		g_vFresnelParams.w	// �߹���ɢ��


float	g_fRefelectRadius					: GLOBAL;

// ˮ�淨����ͼ����
float4	g_vNormalMapParams	: GLOBAL;
#define	g_fNormalMapScale	g_vNormalMapParams.x
#define	g_fSpecularGradient	g_vNormalMapParams.y
#define	g_fWaterSpeed		g_vNormalMapParams.z
#define	g_fAlphaCorrection	g_vNormalMapParams.w

float3	g_vFoamParams		: GLOBAL;

// 
float2	g_vSunScaleShiness		: GLOBAL;
#define	g_fSpecularIntensity	g_vSunScaleShiness.x
#define g_fSpecularPower		g_vSunScaleShiness.y


//
float	g_fRefractionDistortionStrength		: GLOBAL;

float2	g_vWaterHeightMapSizeAndBaseHeight	: GLOBAL;
#define	g_fWaterHeightMapSize	g_vWaterHeightMapSizeAndBaseHeight.x
#define	g_fWaterBaseHeight		g_vWaterHeightMapSizeAndBaseHeight.y

// ˮ����
float	g_fFogIntensity	: GLOBAL;

// ˮ����ɫ
float3	g_vWaterColor	: GLOBAL;


//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
#endif