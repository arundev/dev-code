//*************************************************************************************************
// 内容:	深度渲染(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-22
// 最后修改:	
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
//float4x3	g_mSkinnedBoneMatrix3[MAX_BONES] : SKINBONEMATRIX3;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdColorTex
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdColorSampler = sampler_state
{
	Texture = (sdColorTex);
	AddressU = WRAP;
	AddressV = WRAP;	
	AddressW = WRAP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGBTexture = true;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT_SKINNED
{
	float3	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION0;
	float	fUVSetZ		: TEXCOORD0;	
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_UVSET
{
	float4	vProjPos	: POSITION0;
	float	fUVSetZ		: TEXCOORD0;
	float2	vUVSet		: TEXCOORD1;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = mul(float4(vPos.xyz, 1.f), g_mWorldViewProj);
	kOutput.fUVSetZ = kOutput.vProjPos.w;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_UVSET VS_Main_AlphaTest(float3 vPos : POSITION0, float2	vUVSet : TEXCOORD0)
{
	VS_OUTPUT_UVSET kOutput = (VS_OUTPUT_UVSET)0;
	kOutput.vProjPos = mul(float4(vPos.xyz, 1.f), g_mWorldViewProj);
	kOutput.fUVSetZ = kOutput.vProjPos.w;
	kOutput.vUVSet = vUVSet;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Skinned(VS_INPUT_SKINNED kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	float4x4 mSkinnedBoneTransform;
	float4 vWorldPos;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights, 
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);	
		
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.fUVSetZ = kOutput.vProjPos.w;
		
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{	
	return kInput.fUVSetZ;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_AlphaTest(VS_OUTPUT_UVSET kInput) : COLOR0
{	
	return float4(kInput.fUVSetZ, kInput.fUVSetZ, kInput.fUVSetZ, tex2D(sdColorSampler, kInput.vUVSet).w);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ActorBlur(VS_OUTPUT kInput) : COLOR0
{	
	return float4(0.f, 0.f, 1.f, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Height(VS_OUTPUT kInput) : COLOR0
{	
	return EncodeFloat2RGBA(kInput.fUVSetZ);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique ZStep_StaticMesh
<
	string Description = "ZStep_StaticMesh";
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
technique ZStep_StaticMesh_AlphaTest
<
	string Description = "ZStep_StaticMesh_AlphaTest";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_AlphaTest ();
		PixelShader 	= compile ps_2_0 PS_Main_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique ZStep_SkinnedMesh
<
	string Description = "ZStep_SkinnedMesh";
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
technique StaticMesh_Blur
<
	string Description = "StaticMesh_Blur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_ActorBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique SkinnedMesh_Blur
<
	string Description = "SkinnedMesh_Blur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Skinned ();
		PixelShader 	= compile ps_2_0 PS_Main_ActorBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_HeightMap_Power
<
	string Description = "StaticMesh_HeightMap_Power";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Height ();
	}
}
//---------------------------------------------------------------------------------------