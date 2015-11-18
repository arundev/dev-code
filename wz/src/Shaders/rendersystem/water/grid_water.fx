//*************************************************************************************************
// 内容:	Grid Water(已废弃)
//
//		Author 	: 	Xieweibo(XieWeibo@snda.com)
//		Date	:	2011.4.19
//		Grid Water Shader
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-02
// 最后修改:	
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
#define	GRID_SIZE			128.0f
#define	GRID_PIXEL_SIZE		(1.f / 128.f)
#define	REMOVE_TILE_SCALE	0.66666f

#define GRID1_SIZE			2744.f	// size in meters(i.e. in spatial domain) of the first grid
#define GRID2_SIZE			196.f	// size in meters(i.e. in spatial domain) of the second grid
#define GRID3_SIZE			14.f	// size in meters(i.e. in spatial domain) of the third grid
#define GRID4_SIZE			2.f	

// Four corner point
float4	g_vCornerPoints[4]	: GLOBAL;

// HeightMap高度缩放系数
float	g_fZScale			: GLOBAL;

// ChoppyMap缩放系数
//float3	g_vChoppyScale		: GOBAL

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
// vertex texture fetch for grid water vertex shader
SD_LINEAR_WRAP_SAMPLE(0, sdHeightSampler,	sdHeightTex,	false);	
//SD_LINEAR_WRAP_SAMPLE(1, sdChoppySampler,	sdChoppyTex,	false);	

SD_LINEAR_WRAP_SAMPLE(1, sdWaterNormalSampler,		sdWaterNormalTex,	false);	// 水面法线纹理
SD_POINT_CLAMP_SAMPLE(2, sdRefractSampler,			sdRefractTex,		false);	// 水面折射纹理			

// 水面反射立方体环境纹理
texture sdReflectTex<string	NTM = "shader";	int NTMIndex = 3;>;																
samplerCUBE sdReflectCubeSampler = sampler_state							
{																
	Texture		= (sdReflectTex);										
	ADDRESSU	= WRAP;										
	ADDRESSV	= WRAP;	
	ADDRESSW	= WRAP;										
	MAGFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	MIPFILTER	= NONE;	// ANISOTROPIC
	MAXANISOTROPY = 4;
	//MipmapLodBias = (LODBias);
	SRGBTexture = true;
};

SD_LINEAR_CLAMP_SAMPLE(4, sdFresnelSampler,			sdFresnelTex,		false);	// 菲涅尔系数纹理

//---------------------------------------------------------------------------------------
// mannual bilinear filter for texture
// 手动双线性过滤
//---------------------------------------------------------------------------------------
float4 tex2D_bilinear_filter(sampler2D tex, float4 uv)
{
	float2 f = frac(uv.xy * GRID_SIZE);
	
	float4 t1 = tex2Dlod(tex, uv);
	float4 t2 = tex2Dlod(tex, uv + float4(GRID_PIXEL_SIZE, 0, 0, 0));	
	float4 t3 = tex2Dlod(tex, uv + float4(0, GRID_PIXEL_SIZE, 0, 0));	
	float4 t4 = tex2Dlod(tex, uv + float4(GRID_PIXEL_SIZE, GRID_PIXEL_SIZE, 0, 0));	
	
	float4 tA = lerp(t1, t2, f.x);
	float4 tB = lerp(t3, t4, f.x);	

	return lerp(tA, tB, f.y);
}
//---------------------------------------------------------------------------------------
// Schlick's Fresnel, which is actually water's BRDF
//---------------------------------------------------------------------------------------
float Fresnel_Schlick(float3 V, float3 N, float f)
{
	float e_n = max(dot(V, N), 0.f);
	return f + (1.f - 1) * pow(1.f - e_n, 5);
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos	: POSITION0;
};
//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float4	vUVSet0				: TEXCOORD0;	// xy height map uv,zw uniforma uv
	float4	vUVSetWorldPos		: TEXCOORD1;	// .w save refrac scale
	float4	vUVSetViewPos		: TEXCOORD2;
	float4	vUVSetProjPos		: TEXCOORD3;
};
//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 世界坐标
	float4 vWorldPosition = lerp(lerp(g_vCornerPoints[0], g_vCornerPoints[1], kInput.vPos.x),
		lerp(g_vCornerPoints[2], g_vCornerPoints[3], kInput.vPos.x), kInput.vPos.y);
	kOutput.vUVSetWorldPos = vWorldPosition / vWorldPosition.w;
	
	// 采样高度图，修正世界坐标高度
	float2 vHeightUV = vWorldPosition.xy / g_fWaterHeightMapSize;
	float4 vDisplacement = tex2Dlod(sdHeightSampler, float4(vHeightUV, 0, 0));
	kOutput.vUVSetWorldPos.z = g_fWaterBaseHeight + vDisplacement.r * g_fZScale;
	
	//
	//vWorldPosition.xyz += tex2D(tex2Dlod(sdChoppySampler, float4(vHeightUV, 0, 0)).xyz * g_vChoppyScale;
	
	// 观察坐标
	float4 vViewPosition = mul(float4(kInput.vPos, 1.0f), g_mWorldView);
	kOutput.vUVSetViewPos = vViewPosition;
	
	// 投影坐标
	float4 vProjPosition = mul(float4(vWorldPosition.xyz, 1), g_mViewProj);
	kOutput.vProjPos = vProjPosition;
	kOutput.vUVSetProjPos = vProjPosition;

	// 纹理坐标
	float2 vHeightUVSet = vWorldPosition.xy / g_fWaterHeightMapSize;
	kOutput.vUVSet0 = float4(vHeightUVSet, vProjPosition.xy);
	
	//
	kOutput.vUVSetWorldPos.w = g_fRefractionDistortionStrength / max(0.002f,vProjPosition.z / vProjPosition.w);
	
	// 当前点对应远裁剪面世界坐标
	//float4 vFarWorldPos = mul(float4(vProjPos.xy / vProjPos.w, 1, 1), g_mDepthToWorld) * vProjPos.w;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样法线贴图, 并混合
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, kInput.vUVSet0.xy).xyz;
	float3 vDetailNormal = tex2D(sdWaterNormalSampler, kInput.vUVSet0.xy * g_fWaterFresnel).xyz;
	float3 vBlendNormal = (vBaseNormal + vDetailNormal) * 0.5f;
	
	// 观察方向
	float3 V = normalize(g_vViewPos - kInput.vUVSetWorldPos.xyz);	
	
	// 计算法线合成系数，并合成
	float fAdditionalReflection = V.x * V.x + V.y * V.y;
	fAdditionalReflection /= g_fNormalFadingDistance;
	float3 N = lerp(vBlendNormal, vBaseNormal, saturate(fAdditionalReflection));

	//
	// @{
	V = normalize(V);
	float3 L = -g_vSunLightDir;
	float3 R = normalize(reflect(V, vDetailNormal));
	//float3 R2 = normalize(reflect(V, vBaseNormal));
	// @}
	
	
	// 折射
	// @{
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
		
	// 计算折射纹理坐标
	// Calc refraction UV which bease on device Z
	//float fRefracScale = kInput.vUVSetWorldPos.w;
	float2 vRefracScale = g_fRefractionDistortionStrength / max(0.002, kInput.vUVSetProjPos.z);
	float2 vRefractionUV = vScreenUVSet - vRefracScale * N.xy;
	
	// 采样折射纹理
	// Get refraction texture color
	float4 vRefractColor = tex2D(sdRefractSampler, vRefractionUV);	
	// @}
	
	
	// 反射
	// @{
	// 计算反射纹理坐标
	// Calc reflection
	//float2 vRefelectionUV = vScreenUVSet + vRefracScale * N.xy;

	// 采样反射纹理
	// Get refelect color
	float3 vReflectColor = texCUBE(sdReflectCubeSampler, -R.xzy);
	// @}

	
	// 计算菲涅尔系数
	//float fCosAngleZ = vEyeDir.z;//dot(float3(0,0,1), vEyeDir); //to assume plane
	float fFresnelFactor = dot(V, N);
	float fFresnel = tex1D(sdFresnelSampler, fFresnelFactor).r;
	//fFresnel += fAdditionalReflection;
	//fFresnel = saturate(fFresnel);
	fFresnel -= g_fWaterFresnel;
	fFresnel = saturate(fFresnel);
	
	// 采样场景深度
	//float4 vGeoTex = tex2D(sdGeomSampler, vScreenUVSet);
	//float fDepth = UnpackDepth(vGeoTex.xy);
	//float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarWorldPos.xyz / kInput.vUVSetFarWorldPos.w, fDepth);
	//
	//float4 vRefractionGeoTex = tex2D(sdGeomSampler, vRefractionUV);
	//float fRefractionDepth = UnpackDepth(vRefractionGeoTex.xy);
	//float4 vRefractFarWorldPos = mul(float4((vRefractionUV - 0.5f) * float2(2.f, -2.f), 1.f, 1.f, g_mDepthToWorld);
	//vRefractFarWorldPos.xyz /= vRefractFarWorldPos.w;
	//float3 vRefractWorldPos = lerp(g_vCameraPostion, vRefractFarWorldPos, fRefractionDepth);
	
	// 计算太阳光照，并合成到反射颜色
	// Caculate lighting which just take sunlight into consideration
	float3 vSunLight = g_fSpecularIntensity * g_vMainLightColor * pow(dot(L, R), g_fSpecularPower);
	vReflectColor += vSunLight;
	
	// 计算最终颜色
	float4 vOutColor = 0;
	vOutColor.rgb = lerp(vRefractColor.rgb, vReflectColor.rgb, fFresnel);
	
	// 计算雾，并合成到最终颜色
	// Add height fog
	//float4 vFogColorAndFactor = CalcHeightFogCurveFactor(float4(kInput.vUVSetWorldPos.xyz, kInput.vUVSetViewPos.z), false)
	//vOutColor.rgb = vFogColorAndFactor.rgb + vOutColor.rgb * vFogColorAndFactor.w;
	
	return vOutColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Water_GridWater_VTF
<
	string Description = "Water_GridWater_VTF";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------