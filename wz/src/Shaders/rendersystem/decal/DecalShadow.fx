//*************************************************************************************************
// 内容:	贴花阴影(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-08
// 最后修改:	
//*************************************************************************************************
#include "DecalCommon.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4x4 	g_mWorldToLightProj	: GLOBAL;
float2		g_vShadowMapSize	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(1, sdShadowSampler,	sdShadowTex,	false);	

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SHADOW
{
	float4 	vProjPos				: POSITION0;
	float4	vUVSetProjPos			: TEXCOORD0;
	float4	vUVSetFarClipWorldPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT_SHADOW VS_Main_DecalShadow(float3 vPos : POSITION0)
{
	VS_OUTPUT_SHADOW kOutput = (VS_OUTPUT_SHADOW)0;
	
	float4 vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vProjPos = vProjPos;
	
	float2 vTProjPos = vProjPos.xy / vProjPos.w;
	kOutput.vUVSetFarClipWorldPos = mul(float4(vTProjPos, 1, 1), g_mDepthToWorld) * vProjPos.w;

	float2 vVProjPos = GetTexCoordForPixel(vTProjPos);
	kOutput.vUVSetProjPos  = float4(vVProjPos, 0, 1) * vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
void PCFShadowMap(float4 vWorldPos, sampler2D sdShadowSampler, float2 vShadowMapSize, 
	float4x4 mWorldToLightProj, float fShadowBias, out float fShadowOut)
{
	float4 vLightProjPos = mul(vWorldPos, mWorldToLightProj);
	float2 vShadowTex = 0.5f * vLightProjPos.xy / vLightProjPos.w + float2(0.5f, 0.5f);
	vShadowTex.y = 1.f - vShadowTex.y;
	
	float2 vShadowMapSizeInverse = 1.f / vShadowMapSize;
	float fLightSpaceDepth = vLightProjPos.z / vLightProjPos.w;	
	//fLightSpaceDepth = saturate(fLightSpaceDepth);
	
	//float2 vBorderTest = saturate(vShadowTex) - vShadowTex;
	//if (any(vBorderTest))
	//{
	//	fShadowOut = 1.f;
	//}
	//else
	{
		fLightSpaceDepth -= fShadowBias;
		
		fShadowOut = 0.f;
		float4 vOnes = float4(1.f, 1.f, 1.f, 1.f);
		float4 vKernels = float4(1.f, 1.f, 1.f, 1.f);
		float akKernels[4] = {1.f, 1.f, 1.f, 1.f};
		
		akKernels[0] = 1.f - frac(vShadowTex.y * g_vShadowMapSize.y);
		akKernels[3] = frac(vShadowTex.y * g_vShadowMapSize.y);
	
		vKernels.x = 1.f - frac(vShadowTex.x * g_vShadowMapSize.x);
		vKernels.w = frac(vShadowTex.x * g_vShadowMapSize.x);
		
		float fTotalPercent = 0.f;
		
		// This loop is mannually unrolled here to avoid long shader compilation times.
		//for (int i = 0; i < 4; ++i)
		// i == 0
		{
			float4 vShadowMapDepth = 0.f;
			float2 vPos = vShadowTex;
			vShadowMapDepth.x = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.y = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.z = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.w = tex2D(sdShadowSampler, vPos).x;
		
		#ifdef IS_EDIT_MODE
			float4 vShading = (fLightSpaceDepth >= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels * vShadowMapDepth) * akKernels[0];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[0];
		#else
			float4 vShading = (fLightSpaceDepth <= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels) * akKernels[0];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[0];	
		#endif
		}
		// i == 1
		{
			float4 vShadowMapDepth = 0.f;
			float2 vPos = vShadowTex;
			vPos.y += vShadowMapSizeInverse.y;
			vShadowMapDepth.x = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.y = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.z = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.w = tex2D(sdShadowSampler, vPos).x;
		
		#ifdef IS_EDIT_MODE
			float4 vShading = (fLightSpaceDepth >= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels * vShadowMapDepth) * akKernels[1];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[1];
		#else
			float4 vShading = (fLightSpaceDepth <= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels) * akKernels[1];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[1];	
		#endif
		}
		// i == 2
		{
			float4 vShadowMapDepth = 0.f;
			float2 vPos = vShadowTex;
			vPos.y += 2.f * vShadowMapSizeInverse.y;
			vShadowMapDepth.x = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.y = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.z = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.w = tex2D(sdShadowSampler, vPos).x;
		
		#ifdef IS_EDIT_MODE
			float4 vShading = (fLightSpaceDepth >= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels * vShadowMapDepth) * akKernels[2];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[2];
		#else
			float4 vShading = (fLightSpaceDepth <= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels) * akKernels[2];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[2];	
		#endif
		}
		// i == 3
		{
			float4 vShadowMapDepth = 0.f;
			float2 vPos = vShadowTex;
			vPos.y += 3.f * vShadowMapSizeInverse.y;
			vShadowMapDepth.x = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.y = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.z = tex2D(sdShadowSampler, vPos).x;
			vPos.x += vShadowMapSizeInverse.x;
			vShadowMapDepth.w = tex2D(sdShadowSampler, vPos).x;
		
		#ifdef IS_EDIT_MODE
			float4 vShading = (fLightSpaceDepth >= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels * vShadowMapDepth) * akKernels[3];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[3];
		#else
			float4 vShading = (fLightSpaceDepth <= vShadowMapDepth);
			fShadowOut += dot(vShading, vKernels) * akKernels[3];
			fTotalPercent += dot(vOnes, vKernels) * akKernels[3];	
		#endif
		}
	
		fShadowOut = fShadowOut / fTotalPercent;
	}
}
//---------------------------------------------------------------------------------------
float4 PS_Main_DecalShadow(VS_OUTPUT_SHADOW kInput) : COLOR
{
	// 深度
	float4 vGeomData 	= tex2D(sdGeomSampler, kInput.vUVSetProjPos.xy/kInput.vUVSetProjPos.w);
	float  fDepth		= UnpackDepth(vGeomData.xy);

	// 当前点世界坐标
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVSetFarClipWorldPos.xyz / kInput.vUVSetFarClipWorldPos.w, fDepth);
	
	//
	float fShade = 0.f;
	PCFShadowMap(float4(vWorldPos, 1.f), sdShadowSampler, g_vShadowMapSize, g_mWorldToLightProj, 0.00001f, fShade);
		
#ifndef IS_EDIT_MODE
	return float4(0, 0, 0, fShade);
#else
	return float4(0, 0, 0, 1.f - fShade);
#endif	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Decal_DecalShadow
<
	string Description = "Decal_DecalShadow";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_DecalShadow ();
		PixelShader 	= compile ps_3_0 PS_Main_DecalShadow ();
		
		//AlphaBlendEnable = true;
		//AlphaTestEnable = false;
		//BlendOp = Add;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------