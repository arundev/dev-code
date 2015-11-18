//*************************************************************************************************
// ����:	Color Pick ��ȷʰȡ
//---------------------------------------------------------
// ����:		
// ����:		2012-11-22
// ����޸�:	
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
//float4x4 g_mWorldViewProj
//<
//	string VarType = "Predefined";
//	string DefinedMapping = "WORLDVIEWPROJECTION";
//>;
//
//float4x4 g_mViewProj
//<
//	string VarType = "Predefined";
//	string DefinedMapping = "WORLDVIEWPROJECTION";
//>;
//
//// Ӳ����������
//#if defined (DIRECT3D)
//	#define BONEMATRIX_TYPE	float4x3
//#else
//	#define BONEMATRIX_TYPE	float3x4
//#endif
//
//static const int MAX_BONES = 32;
//
//BONEMATRIX_TYPE	g_mSkinnedBoneMatrix3[MAX_BONES] : SKINBONEMATRIX3;

float4	g_vObjectColor	: GLOBAL = float4(0.f, 0.f, 0.f, 0.f);

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT_SKINNED
{
	float3	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 VS_Main(float3 vPos : POSITION0) : POSITION0
{
	return mul(float4(vPos.xyz, 1.f), g_mWorldViewProj);
}
//---------------------------------------------------------------------------------------
float4 VS_Main_Skinned(VS_INPUT_SKINNED kInput) : POSITION0
{
	float4x4 mSkinnedBoneTransform;
	float4 vWorldPos;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights, 
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);	
	return mul(vWorldPos, g_mViewProj);;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main() : COLOR0
{	
	return g_vObjectColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PrecisionPick_StaticMesh_ColorWrite
<
	string Description = "PrecisionPick_StaticMesh_ColorWrite";
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
technique PrecisionPick_SkinnedMesh_ColorWrite
<
	string Description = "PrecisionPick_SkinnedMesh_ColorWrite";
	int BonesPerPartition = MAX_BONES;
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Skinned ();
		PixelShader 	= compile ps_2_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------