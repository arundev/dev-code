//*************************************************************************************************
// 内容:	Water Surf, 海岸线
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-07
// 最后修改:	
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
#define	TEX_U_SCALE			0.2f
#define	SURF_CIRCLE_TIME	8.f

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdWaterSurfTex
<
	string	NTM 		= "shader";	
	int 	NTMIndex	= 0;
>;

sampler sdWaterSurfSampler = sampler_state
{
	Texture		= (sdWaterSurfTex);
	ADDRESSU	= WRAP;	
	ADDRESSV	= CLAMP;
	MAGFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	SRGB_ENABLE;
};	


//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos	: POSITION0;
	float3	vUVSet0	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;
	float2	vUVSet0		: TEXCOORD0;	
	float2	vUVSet1		: TEXCOORD1;
	float2	vUVSet2		: TEXCOORD2;
	float3	vUVSetAlpha	: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
float GetValue(float fTP)
{
	float f = 0.5f;
	float f1 = fTP / f;
	float f2 = (1.f - fTP) / (1.f - f);
	float ft = min(f1, f2);
	
	return 1.f - pow(ft, 3.f);
}
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;

	// 计算投影坐标
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mViewProj);
	
	// 
	float fVertexGroup = (sin(kInput.vUVSet0.z * 0.001f) * 1.3f + 1.3f) / 2.f;
	float fTP0 = fmod(fmod(g_fTime * 1.2f, SURF_CIRCLE_TIME) / SURF_CIRCLE_TIME + fVertexGroup, 1.f);
	float fTP1 = fmod(fTP0 + 0.35f + fVertexGroup, 1.f);
	float fTP2 = fmod(fTP0 + 0.65f + fVertexGroup, 1.f);	
	float fVertexCtg = fmod(kInput.vUVSet0.z, 2.f); // 0 sea 1 land
	
	float v0 = GetValue(fTP0);
	kOutput.vUVSet0.x = kInput.vUVSet0.x * TEX_U_SCALE;
	kOutput.vUVSet0.y = fVertexCtg * (6.f - 5.1f * v0);
	
	float v1 = GetValue(fTP1);
	kOutput.vUVSet1.x = kOutput.vUVSet0.x + 0.4f;
	kOutput.vUVSet1.y = fVertexCtg * (6.f - 5.1f * v1);
	
	float v2 = GetValue(fTP2);
	kOutput.vUVSet2.x = kOutput.vUVSet1.x + 0.75f;
	kOutput.vUVSet2.y = fVertexCtg * (6.f - 5.1f * v2);
	
	kOutput.vUVSetAlpha = pow(float3(v0, v1, v2), 2.8f);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor0 = tex2D(sdWaterSurfSampler, kInput.vUVSet0);
	vColor0.w *= kInput.vUVSetAlpha;
	
	float4 vColor1 = tex2D(sdWaterSurfSampler, kInput.vUVSet1);
	vColor1.w *= kInput.vUVSetAlpha;
	
	float4 vColor2 = tex2D(sdWaterSurfSampler, kInput.vUVSet2);
	vColor2.w *= kInput.vUVSetAlpha;
	
	vColor1 = vColor1 * vColor1.w + vColor0 * (1.f - vColor1.w);
	vColor2 = vColor2 * vColor2.w + vColor1 * (1.f - vColor2.w);
	vColor2.w = clamp(vColor2.w, 0, 0.7f);
			
	return vColor2;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Water_WaterSurf
<
	string Description = "Water_WaterSurf";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = true;
		//ZWriteEnable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = true;
		//BlendOp = Add;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------