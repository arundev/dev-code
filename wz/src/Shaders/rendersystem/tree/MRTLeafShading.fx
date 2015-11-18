//*************************************************************************************************
// 内容:	Tree Leaf Shading着色器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-14
// 最后修改:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdLightSampler, 	sdLightBuf, 	false);	// 光照缓存
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, 	sdGeomBuf, 		false);	// 几何缓存
SD_POINT_CLAMP_SAMPLE(2, sdMatSampler, 		sdMatBuf, 		false);	// 材质缓存

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 	vPos		: POSITION0;
	float2	vUVSet0		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 	vProjPos		: POSITION0;
	float2	vUVSetScreenPos	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	kOutput.vUVSetScreenPos = kInput.vUVSet0 + g_vHalfPixelOffset;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR
{
	// 法线
	float4 vGeomData = tex2D(sdGeomSampler, kInput.vUVSetScreenPos);
	float3 vViewNormal = UnpackNormal(vGeomData.zw);
	
	// 本地光照
	float4 vLocalLight = tex2D(sdLightSampler, kInput.vUVSetScreenPos);
	
	// 全局光照
	float3 vDiffuseLight;
	AccumLightingOnlyDiffuse(vViewNormal, vLocalLight, vDiffuseLight);

	// 纹理
	float3 vDiffuseTex = tex2D(sdMatSampler, kInput.vUVSetScreenPos).rgb;
	
	return float4(vDiffuseLight * g_vTreeLeafMaterial * vDiffuseTex, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Tree_MRTLeafShading
<
	string	Description 		= "Tree_MRTLeafShading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main();
	}
}
//---------------------------------------------------------------------------------------