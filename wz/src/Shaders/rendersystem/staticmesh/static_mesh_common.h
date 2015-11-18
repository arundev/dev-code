//*************************************************************************************************
// ����:	StaticMesh����ɫ�������еĹ��������뺯��
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:
//*************************************************************************************************
#ifndef _STATICMESH_COMMON_H__
#define _STATICMESH_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------------------
// LightMap
float4	a_vLightMapChannel			: ATTRIBUTE	= float4(1.f, 0.f, 0.f,	0.f);
float4 	a_vLightMapTransform		: ATTRIBUTE	= float4(0.f, 0.f, 1.f, 1.f);

// DetailNormalMap�����������
float3	a_vDetailNormalScale		: ATTRIBUTE	= float3(1.f, 1.f, 1.f);
float2	a_vDetailNormalUVTiling		: ATTRIBUTE = float2(5.f, 5.f);
float	a_fDetailNormalFadeDistance	: ATTRIBUTE = 30.f;

//
float4x4	a_mWorldProjectionMat	: ATTRIBUTE;

// ��Ҷ����
float4	a_vTransmit					: ATTRIBUTE = float4(1.f, 1.f, 1.f, 1.f);

// �Թ�ǿ��
float	a_fGlowPower				: ATTRIBUTE = 1.f;

//---------------------------------------------------------------------------------------
// �ӳ���ɫ�����������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdLightBufferSampler, 		sdTexLightBuffer, 	false);	
SD_POINT_CLAMP_SAMPLE(1, 	sdGeomBufferSampler, 		sdTexGeomBuffer, 	false);
SD_POINT_CLAMP_SAMPLE(2, 	sdMatBufferSampler, 		sdTexMatBuffer, 	false);	// Old MRT
SD_LINEAR_CLAMP_SAMPLE(3, 	sdCurveFogBufferSampler, 	sdTexFogCurveMap, 	false);	

SD_POINT_CLAMP_SAMPLE(2, 	sdMatBuffer0Sampler, 		sdTexMatBuffer0, 	false);	// MRT3
SD_POINT_CLAMP_SAMPLE(3, 	sdMatBuffer1Sampler, 		sdTexMatBuffer1, 	false);	// MRT3
SD_POINT_CLAMP_SAMPLE(4, 	sdMatBuffer2Sampler, 		sdTexMatBuffer2, 	false);	// MRT3

//---------------------------------------------------------------------------------------
// �����ɫ����Ⱦͨ��
//---------------------------------------------------------------------------------------
#define VERTEX_DIFFUSEMAP_CHANNEL			0x00000001	///< ������ͨ��
#define VERTEX_NORMALMAP_CHANNEL			0x00000002	///< ������ͼͨ��
#define VERTEX_GLOSSMAP_CHANNEL				0x00000004	///< �߹���ͼͨ��
#define VERTEX_LIGHTMAP_CHANNEL				0x00000008	///< ������ͼͨ��
#define VERTEX_GLOWMAP_CHANNEL				0x00000010	///< �Թ���ͼͨ��
#define VERTEX_DETAIL_NORMAL_CHANNEL		0x00000020	///< ϸ����ͼͨ��
#define VERTEX_ALPHATEST_FLAG				0x00000040	///< ͸������
#define VERTEX_COMPACTVERTEX_FLAG			0x00000080	///< ����ѹ��
#define VERTEX_LEAFANIM_FLAG				0x00000100	///< ��Ҷ���㶯��

#define	PIXEL_SPECULAR_LIGHTTING_CHANNEL	0x00000001	///< �߹�
#define	PIXEL_RIM_LIGHTTING_CHANNEL			0x00000002	///< ������
#define	PIXEL_GLOW_LIGHTTING_CHANNEL		0x00000004	///< �Թ�

//---------------------------------------------------------------------------------------
// ��Ҷ����
//---------------------------------------------------------------------------------------
float4 SmoothCurve(float4 x)
{
	return x * x * (3.f - 2.f * x);
}
//---------------------------------------------------------------------------------------
float4 TriangleWave(float4 x)
{
	return abs(frac(x + 0.5f) * 2.f - 1.f);
}
//---------------------------------------------------------------------------------------
float4 SmoothTriangleWave(float4 x)
{
	return SmoothCurve(TriangleWave(x));
}
//---------------------------------------------------------------------------------------
float3 CalcDetailWave(float3 vPos, float3 vNormal, float fSpeed)
{
	float fBranchPhase = 1.f;// dot(vPos, 1);
	float fVertexPhase = dot(vPos.xyz, fBranchPhase);
	float2 vWavesIn = g_fTime * fSpeed + float2(fVertexPhase, fVertexPhase);
	float4 vWaves = frac(vWavesIn.xxyy * float4(1.975f, 0.793f, 0.375f, 0.193f)) * 2.f - 1.f;
	vWaves = SmoothTriangleWave(vWaves);
	float2 vWavesSum = vWaves.xz + vWaves.yw;
	return vWavesSum.xxy * float3(vNormal.x * 0.2f * a_vTransmit.w / 20.f, vNormal.y * 0.2f * a_vTransmit.w / 20.f, 0.2f * a_vTransmit.w / 20.f);
}
//---------------------------------------------------------------------------------------
#endif