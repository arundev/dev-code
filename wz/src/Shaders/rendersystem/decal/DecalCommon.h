//*************************************************************************************************
// ����:	������ɫ���Ĺ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-11-08
// ����޸�:	2014-04-23
//*************************************************************************************************
#ifndef _DecalCommon_H__
#define _DecalCommon_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// �۲�����ϵ��Ͷ��������Ͷ������ϵ�任����
float4x4	g_mViewToProjectorUV		: GLOBAL;

// Ͷ����UV����
float2		g_vProjectorUVScale			: GLOBAL = { 1.f, 1.f };
float2		g_vProjectorUVOffset		: GLOBAL = { 0.f, 0.f };
float2		g_vProjectorUVSpeed			: GLOBAL = { 1.f, 1.f };

// Ͷ�䷽��
float3		g_vProjectorDirection		: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf,	false);		// ��Ļ����뷨�߻���
SD_POINT_CLAMP_SAMPLE(1, sdLightSampler,	sdLightBuf,	false);		// ���ջ���


//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 	vProjPos				: POSITION0;
	float4	vUVSetScreenTex			: TEXCOORD0;
	float4	vUVSetFarClipViewPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	// ģ������ת����ͶӰ����
	float4 vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vProjPos = vProjPos;
	
	// ͶӰ�ռ��������
	float2 vTProjPos = vProjPos.xy / vProjPos.w;
	
	// ��ǰ���ӦԶ�ü����ϵĵ����������
	float4 vUVFarClipProjPos  = float4(vTProjPos.xy, 1.f, 1.f);
	kOutput.vUVSetFarClipViewPos = mul(vUVFarClipProjPos, g_mDepthToView) * vProjPos.w;

	// ͶӰ����ת������Ļ��������(����������ƫ��)
	float2 vUVSetScreenTex = GetTexCoordForPixel(vTProjPos);
	kOutput.vUVSetScreenTex  = float4(vUVSetScreenTex, 0.f, 1.f) * vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
#endif