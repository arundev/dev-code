//*************************************************************************************************
// ����:	��ľ��ɫ���Ĺ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-11-14
// ����޸�:	
//*************************************************************************************************
#ifndef _TREE_COMMON_H__
#define _TREE_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// �������±����Ⱦͨ��
#define	LEAFANIM_FLAG		0x04
#define	NOISEFADING_FLAG	0x20

//
float	g_fTreeSwayRadius			: GLOBAL = 0.3f;
float	g_fTreeSwaySpeed			: GLOBAL = 0.11f;
float	g_fTreeLightPassing			: GLOBAL = 0.f;
float	g_fTreeLeafShading			: GLOBAL = 0.f;
float	g_fTreeWholeLightDistance 	: GLOBAL = 0.f;
float3	g_vTreeLeafMaterial			: GLOBAL = float3(1.f, 1.f, 1.f);

//
float	a_fTreeLeafScale	: ATTRIBUTE = 1.f;

//
static float3 g_akCornerOffset[4] = {
				float3( 0.5f,  0.5f, 0),
				float3(-0.5f,  0.5f, 0),
				float3( 0.5f, -0.5f, 0),
				float3(-0.5f, -0.5f, 0)
				};

static float3 g_akCornerNormal[4] = {
				float3( 4.f,  4.f, -1.f),
				float3(-4.f,  4.f, -1.f),
				float3( 4.f, -4.f, -1.f),
				float3(-4.f, -4.f, -1.f)
				};
				
//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_WRAP_SAMPLE(0, sdNoiseSampler, sdNoiseTex, false);	// ��������

#endif