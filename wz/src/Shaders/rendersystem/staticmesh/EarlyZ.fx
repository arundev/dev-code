//*************************************************************************************************
// 内容:	StaticMesh的EarlyZPass着色器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-10
// 最后修改:	2013-05-28
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT_ALPHATEST
{
	float3	vPos	: POSITION;
	float2	vUVSet0	: TEXCOORD0;	
};


//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_ALPHATEST
{
	float4	vProjPos	: POSITION;
	float2	vUVSet0		: TEXCOORD0;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4	VS_StaticMesh_Solid(float3 vPos : POSITION)	: POSITION
{
	return mul(float4(vPos, 1.0f), g_mWorldViewProj);
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_ALPHATEST	VS_StaticMesh_AlphaTest(VS_INPUT_ALPHATEST kInput)
{
	VS_OUTPUT_ALPHATEST kOutput = (VS_OUTPUT_ALPHATEST)0;
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);
	kOutput.vUVSet0 = kInput.vUVSet0;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 	PS_StaticMesh_Solid() : COLOR0
{
	return 0;
}
//---------------------------------------------------------------------------------------
float4 	PS_StaticMesh_DiffuseMap_AlphaTest(VS_OUTPUT_ALPHATEST kInput) : COLOR0
{
	//clip(tex2D(sdBaseSampler, kInput.vUVSet0).b - g_fAlphaTestRef);
	clip(tex2D(sdBaseSampler, kInput.vUVSet0).a - g_fAlphaTestRef);
	return 0;
}
//---------------------------------------------------------------------------------------
float4 	PS_StaticMesh_NormalMap_AlphaTest(VS_OUTPUT_ALPHATEST kInput) : COLOR0
{
	//clip(tex2D(sdNormalSampler, kInput.vUVSet0).b - g_fAlphaTestRef);
	clip(tex2D(sdNormalSampler, kInput.vUVSet0).a - g_fAlphaTestRef);
	return 0;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique StaticMesh_Solid_EarlyZ
<
	string	Description 		= "StaticMesh_Solid_EarlyZ";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a 	VS_StaticMesh_Solid();
		PixelShader		= compile	ps_2_a 	PS_StaticMesh_Solid();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_DiffuseMap_EarlyZ
<
	string	Description 		= "StaticMesh_DiffuseMap_EarlyZ";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a 	VS_StaticMesh_AlphaTest();
		PixelShader		= compile	ps_2_a 	PS_StaticMesh_DiffuseMap_AlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_NormalMap_EarlyZ
<
	string	Description 		= "StaticMesh_NormalMap_EarlyZ";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a 	VS_StaticMesh_AlphaTest();
		PixelShader		= compile	ps_2_a 	PS_StaticMesh_NormalMap_AlphaTest();
	}
}
//---------------------------------------------------------------------------------------
