//*************************************************************************************************
// 内容:	(没用到)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-08
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4	VS_Main_MaskOpaqueFog(float3 vPos : POSITION)	: POSITION
{
	return float4(vPos.xy, g_fFogEndHeight, 1.f);
}
//---------------------------------------------------------------------------------------
float4	VS_Main_ShadeOpaqueFog(float3 vPos : POSITION)	: POSITION
{
	return float4(vPos.xy, 1.f, 1.f);
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 	PS_Main_MaskOpaqueFog() : COLOR0
{
	return 0;
}
//---------------------------------------------------------------------------------------
float4 	PS_Main_ShadeOpaqueFog() : COLOR0
{
	return float4(g_vFogColor, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Environment_MaskOpaqueFog
<
	string	Description 		= "Environment_MaskOpaqueFog";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_0	VS_Main_MaskOpaqueFog();
		PixelShader		= compile	ps_2_0 	PS_Main_MaskOpaqueFog();
	}
}
//---------------------------------------------------------------------------------------
technique Environment_ShadeOpaqueFog
<
	string	Description 		= "Environment_ShadeOpaqueFog";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_0	VS_Main_ShadeOpaqueFog();
		PixelShader		= compile	ps_2_0 	PS_Main_ShadeOpaqueFog();
	}
}
//---------------------------------------------------------------------------------------