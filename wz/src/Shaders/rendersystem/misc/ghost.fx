//*************************************************************************************************
// ����:	��ɫ��Ӱ
//			- �ⲿ������ʱ�̹�������,������ж�λ���
//			- ��Ӱ��͸���仯,���Կ���x^2��x^3
//---------------------------------------------------------
// ����:		
// ����:		2012-11-07
// ����޸�:	2013-05-02
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
//float4x3	g_mSkinnedBoneMatrix3[MAX_BONES] 	: SKINBONEMATRIX3;
//float4	g_mWorldMatrix[3]					: ATTRIBUTE;

// ��Ӱ��ɫ��͸����(���ⲿ������)
float4	a_vGhostColor	: ATTRIBUTE;
#define	a_fGhostAlpha	a_vGhostColor.a

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos	: POSITION0;
	float2	vUVSet0	: TEXCOORD0;
};
//---------------------------------------------------------------------------------------
struct VS_INPUT_SKINNED
{
	float3	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	float3	vNormal			: NORMAL0;
	float3	vTargent		: TANGENT0;
	float3	vBinormal		: BINORMAL0;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;
	float	fUVSetZ		: TEXCOORD0;	
	float2	vUVSet0		: TEXCOORD1;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SKINNED
{
	float4	vProjPos	: POSITION;
	float3	vNormal		: TEXCOORD0;
	float3	vTargent	: TEXCOORD1;
	float3	vBinormal	: TEXCOORD2;
	float2	vUVSet0		: TEXCOORD3;
//	float3	vUVSetDepth	: TEXCOORD4;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	//float4x4 mMatWorld = float4x4(g_mWorldMatrix, float4(0,0,0,1));
	float4 vWorldPos = mul(float4(kInput.vPos.xyz, 1.f), g_mWorld);
	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.fUVSetZ = vProjPos.w;
	kOutput.vUVSet0 = kInput.vUVSet0;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_SKINNED VS_Main_Skinned(VS_INPUT_SKINNED kInput)
{
	VS_OUTPUT_SKINNED kOutput = (VS_OUTPUT_SKINNED)0;
	
	// ������������
	float4x4 mSkinnedBoneTransform;
	float4 vWorldPos;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights, 
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	
	// ����ͶӰ����
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
	// 
	//kOutput.vUVSetDepth = float3(kInput.vUVSet0, dot(vWorldPos, g_mView._m02_m12_m22_m32));
	
	// �������編��
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTargent, mSkinnedBoneTransform,
		kOutput.vNormal, kOutput.vBinormal, kOutput.vTargent);
		
	//
	kOutput.vUVSet0 = kInput.vUVSet0;	
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{	
	return float4(tex2D(sdBaseSampler, kInput.vUVSet0).rgb, pow(saturate(a_fGhostAlpha), 2.f) * 0.25f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Skinned(VS_OUTPUT kInput) : COLOR0
{	
	//return float4(kInput.vNormal * a_fGhostAlpha); // * a_fGhostAlpha;
	return float4(tex2D(sdBaseSampler, kInput.vUVSet0).rgb, pow(saturate(a_fGhostAlpha), 2.f) * 0.25f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Misc_Ghost
<
	string Description = "Misc_Ghost";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------
technique Misc_Ghost_Skinned
<
	string Description = "Misc_Ghost_Skinned";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_Skinned ();
		PixelShader 	= compile ps_3_0 PS_Main_Skinned ();
	}
}
//---------------------------------------------------------------------------------------






















