//*************************************************************************************************
// 内容:	Water
//
//		Author 	: 	Xieweibo(XieWeibo@snda.com)
//		Date	:	2011.4.19
//		Grid Water Shader
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:	
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,				sdGeomBuf,			false);	// 场景深度缓存
SD_LINEAR_CLAMP_SAMPLE(1, sdSceneSampler,			sdSceneBuf,			false);	// 场景颜色缓存
SD_LINEAR_WRAP_SAMPLE(2, sdWaterNormalSampler,		sdWaterNormalTex,	false);	// 水面法线纹理
SD_LINEAR_WRAP_SAMPLE(3, sdRippleNormalBufSampler,	sdRippleNormalBuf,	false);	// 水面扰动法线纹理
SD_LINEAR_CLAMP_SAMPLE(4, sdRefelectSampler,		sdRefelectTex,		false);	// 水面反射纹理
SD_POINT_CLAMP_SAMPLE(5, sdRefractSampler,			sdRefractTex,		false);	// 水面折射纹理

SD_LINEAR_CLAMP_SAMPLE(6, sdFresnelSampler,			sdFresnelTex,		false);	// 菲涅尔系数纹理

//SD_LINEAR_WRAP_SAMPLE(4, sdFoamSampler,				sdFoamTex,			false);	// 浅水水边泡沫
//SD_LINEAR_CLAMP_SAMPLE(4, sdCurveFogSampler,		sdCurveFogTex,		false);	// 雾

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
//
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSet0				: TEXCOORD0;	// xy height map uv,zw uniforma uv
	float4	vUVSetWorldPos		: TEXCOORD1;	// .w save refrac scale
	float4	vUVSetViewPos		: TEXCOORD2;
	float4	vUVSetProjPos		: TEXCOORD3;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_NOREFLECT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSet0				: TEXCOORD0;	// xy height map uv,zw uniforma uv
	float4	vUVSetWorldPos		: TEXCOORD1;	// .w save refrac scale
	float4	vUVSetViewPos		: TEXCOORD2;
	float4	vUVSetProjPos		: TEXCOORD3;
	float4	vUVSetFarWorldPos	: TEXCOORD4;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 
VS_OUTPUT VS_Main_Water(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 世界坐标
	float4 vWorldPosition = mul(float4(kInput.vPos, 1.0f), g_mWorld);
	kOutput.vUVSetWorldPos = vWorldPosition;
	
	// 观察坐标
	float4 vViewPosition = mul(float4(kInput.vPos, 1.0f), g_mWorldView);
	kOutput.vUVSetViewPos = vViewPosition;
	
	// 投影坐标
	float4 vProjPosition = mul(float4(vWorldPosition.xyz, 1), g_mViewProj);
	kOutput.vProjPos = vProjPosition;
	kOutput.vUVSetProjPos = vProjPosition;

	// 纹理坐标
	float2 vHeightUVSet = vWorldPosition.xy / g_fWaterHeightMapSize;
	kOutput.vUVSet0 = vHeightUVSet;
	
	//
	kOutput.vUVSetWorldPos.w = g_fRefractionDistortionStrength / max(0.002f,vProjPosition.z / vProjPosition.w);
	
	// 当前点对应远裁剪面世界坐标
	//float4 vFarWorldPos = mul(float4(vProjPos.xy / vProjPos.w, 1, 1), g_mDepthToWorld) * vProjPos.w;

	return kOutput;
}
//---------------------------------------------------------------------------------------
// 不带反射
VS_OUTPUT_NOREFLECT VS_Main_NoReflectNormalBlendWater(VS_INPUT kInput)
{
	VS_OUTPUT_NOREFLECT kOutput;
	
	// 世界坐标
	float4 vWorldPosition = mul(float4(kInput.vPos, 1.0f), g_mWorld);
	kOutput.vUVSetWorldPos = vWorldPosition;
	
	// 观察坐标
	float4 vViewPosition = mul(float4(kInput.vPos, 1.0f), g_mWorldView);
	kOutput.vUVSetViewPos = vViewPosition;
	
	// 投影坐标
	float4 vProjPosition = mul(float4(vWorldPosition.xyz, 1), g_mViewProj);
	kOutput.vProjPos = vProjPosition;
	kOutput.vUVSetProjPos = vProjPosition;

	// 纹理坐标
	float2 vHeightUVSet = vWorldPosition.xy / g_fWaterHeightMapSize;
	kOutput.vUVSet0 = vHeightUVSet;
	
	//
	kOutput.vUVSetWorldPos.w = g_fRefractionDistortionStrength / max(0.002f,vProjPosition.z / vProjPosition.w);
	
	// 当前点对应远裁剪面世界坐标
	float4 vFarWorldPos = mul(float4(vProjPosition.xy / vProjPosition.w, 1, 1), g_mDepthToWorld) * vProjPosition.w;
	kOutput.vUVSetFarWorldPos = vFarWorldPos;

	return kOutput;
} 

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalBlendWater(VS_OUTPUT kInput) : COLOR0
{
	// 法线
	// @{
	// 根据水移动速率计算采样位置偏移(这里没考虑调节水移动方向)
	float fCycle = 1.f / g_fWaterSpeed;
	float fTranslation = fmod(g_fTime, fCycle) * g_fWaterSpeed;
	float2 vTranslation = float2(fTranslation, -fTranslation);

	// 采样法线纹理
	float2 vUVSet0 = kInput.vUVSet0 * 0.2f;
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, vUVSet0 + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 2.f + vTranslation * 4.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 4.f + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 8.f + vTranslation).xyz;
	
	// 恢复法线
	vBaseNormal = vBaseNormal * 0.5f - 1.f;
	
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// 采样角色扰动波纹缓存
	float3 vRipplePerturbation = tex2D(sdRippleNormalBufSampler, vScreenUVSet).xyz;
	
	// 计算用于高光计算的法线
	float3 vNewBaseNormal = vBaseNormal;
	vNewBaseNormal.xy *= g_fSpecularScatter;
	vNewBaseNormal = normalize(float3(vNewBaseNormal.xy + vRipplePerturbation.xy, vNewBaseNormal.z));
	
	// 计算用于普通计算的法线
	vBaseNormal.xy *= g_fNormalMapScale;
	float3 N = normalize(float3(vBaseNormal.xy + vRipplePerturbation.xy, vBaseNormal.z));
	// @}
	
	
	//
	// @{
	// 观察方向
	float3 V = normalize(g_vViewPos - kInput.vUVSetWorldPos.xyz);	
	
	// 光照方向
	float3 L = -g_vSunLightDir;
	L.z *= g_fSpecularGradient;
	L = normalize(L);
	
	// 高光反射方向
	float3 R = 2.f * dot(V, vNewBaseNormal) * vNewBaseNormal - V;
	
	// 环境反射方向
	//float3 R2 = 2.f * dot(V, vBaseNormal) * vBaseNormal - V;
	float3 R2 = 2.f * dot(V, N) * N - V;
	// @}
	
	
	// 反射
	// @{
	// 计算视点反射点世界坐标与投影坐标
	//float3 vBaseRefrlVec = normalize(reflect(V, /*BaseNormal*/N));
	float3 vRefelWorldPos = normalize(reflect(V, float3(0, 0, 1))) * (-25.f) + kInput.vUVSetWorldPos.xyz;
	float4 vRefelProjPos = mul(float4(vRefelWorldPos, 1.f), g_mViewProj);
	vRefelProjPos.xy /= vRefelProjPos.w;
	
	// 计算视点反射点屏幕纹理坐标,采样反射纹理(场景纹理)
	float2 vSceneRefelUV = GetTexCoordForPixel(vRefelProjPos.xy) - 0.05f * N.xy;
	float3 vSceneRefelColor = tex2D(sdSceneSampler, vSceneRefelUV).rgb;
	
	// 计算场景反射纹理与天空反射纹理过渡系数
	vRefelProjPos.xy = 1.f - pow(vRefelProjPos.xy, 4.f);
	vRefelProjPos.xy = vRefelProjPos.yx;
	vRefelProjPos.x = saturate(vRefelProjPos.x);
	float fRefelRatio = (vRefelProjPos.y >= 0) ? vRefelProjPos.x * vRefelProjPos.y : 0;
	float fRefelZ = 0.75f - R2.z * R2.z;
	float fRefelRatio1 = (fRefelZ >= 0) ? fRefelZ * 1.3333f : 0;
	fRefelRatio *= fRefelRatio1;

	// 计算视点反射点屏幕纹理坐标,采样天空纹理
	float2 vSkyRefelUV = vScreenUVSet - 0.5f * N.xy;
	float3 vSkyRefelColor = tex2D(sdRefelectSampler, float2(vSkyRefelUV.x, 1.f - vSkyRefelUV.y)).rgb;
	
	// 合成最终反射
	float3 vReflectColor = lerp(vSkyRefelColor, vSceneRefelColor, fRefelRatio);
	// @}
	
	
	// 折射
	// @{
	// 计算折射纹理坐标
	float2 vRefracScale = g_fRefractionDistortionStrength / max(0.002, kInput.vUVSetProjPos.z);
	float2 vRefractionUV = vScreenUVSet - vRefracScale * N.xy;
	
	// 采样折射纹理
	float4 vRefractColor = tex2D(sdRefractSampler, vRefractionUV);	
	
	// Calc distortion mask to fix pixel which should not be distorted
	float fDistortionMask = vRefractColor.a > 0;
	vRefractionUV = vScreenUVSet - vRefracScale * N.xy * fDistortionMask;
	
	// 采样折射纹理
	vRefractColor = tex2D(sdRefractSampler, vRefractionUV);
	vRefractColor.a = pow(vRefractColor.a, g_fAlphaCorrection);
	
	// 计算水面平滑度
	float fWaterSmooth = vRefractColor.a;
	// @}
	
	// Calc reflection
	//float2 vRefelUV = vScreenUVSet;
	//vRefelUV.y = 1.f - vRefelUV.y;
	//float2 vReflectionUV = vRefelUV + vRefracScale * N.xy;
	
	// 计算菲涅尔系数
	float fFresnelFactor = dot(V, N);
	float fFresnel = tex1D(sdFresnelSampler, fFresnelFactor).r;
	fFresnel -= g_fWaterFresnel;
	fFresnel = saturate(fFresnel);
	
	// 采样场景深度
	//float4 vGeoTex = tex2D(sdGeomSampler, vScreenUVSet);
	//float fDepth = UnpackDepth(vGeoTex.xy);
	//float4 vRefractionGeoTex = tex2D(sdGeomSampler, vRefractionUV);
	//float fRefractionDepth = UnpackDepth(vRefractionGeoTex.xy);
	//float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarWorldPos.xyz / kInput.vUVSetFarWorldPos.w, fDepth);
	
	// 计算太阳光照，并合成到反射颜色
	// Caculate lighting which just take sunlight into consideration
	float3 vSunLight = g_fSpecularIntensity * g_vMainLightColor * saturate(L.z) * 
		pow(saturate(dot(L, R) /**0.5f + 0.5f*/), g_fSpecularPower);
	vReflectColor += vSunLight;
	
	// 计算最终颜色
	float4 vOutColor = 0;
	vOutColor.rgb = lerp(vRefractColor.rgb, vReflectColor.rgb, fFresnel);
	vOutColor.rgb = lerp(vRefractColor.rgb, vOutColor.rgb, fWaterSmooth);
	
	// 计算雾，并合成到最终颜色
	// Add height fog
	//float4 vFogColorAndFactor = CalcHeightFogCurveFactor(float4(kInput.vUVSetWorldPos.xyz, kInput.vUVSetViewPos.z), false)
	//vOutColor.rgb = vFogColorAndFactor.rgb + vOutColor.rgb * vFogColorAndFactor.w;
	
	return vOutColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NoReflectNormalBlendWater(VS_OUTPUT_NOREFLECT kInput) : COLOR0
{
	// 法线
	// Get normal from generated normal map, and sample 2 tiems for remove tiling
	// @{
	// 根据水移动速率计算采样位置偏移(这里没考虑调节水移动方向)
	float fCycle = 1.f / g_fWaterSpeed;
	float fTranslation = fmod(g_fTime, fCycle) * g_fWaterSpeed;
	float2 vTranslation = float2(fTranslation, -fTranslation);

	// 采样法线纹理
	float2 vUVSet0 = kInput.vUVSet0 * 0.2f;
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, vUVSet0 + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 2.f + vTranslation * 4.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 4.f + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 8.f + vTranslation).xyz;
	
	// 恢复法线
	vBaseNormal = vBaseNormal * 0.5f - 1.f;
	
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// 采样角色扰动波纹缓存
	float3 vRipplePerturbation = tex2D(sdRippleNormalBufSampler, vScreenUVSet).xyz;
	
	// 计算用于高光计算的法线
	float3 vNewBaseNormal = vBaseNormal;
	vNewBaseNormal.xy *= g_fSpecularScatter;
	vNewBaseNormal = normalize(float3(vNewBaseNormal.xy + vRipplePerturbation.xy, vNewBaseNormal.z));
	
	// 计算用于普通计算的法线
	vBaseNormal.xy *= g_fNormalMapScale;
	float3 N = normalize(float3(vBaseNormal.xy + vRipplePerturbation.xy, vBaseNormal.z));
	// @}
	
	
	//
	// @{
	// 观察方向
	float3 V = normalize(g_vViewPos - kInput.vUVSetWorldPos.xyz);	
	
	// 光照方向
	float3 L = -g_vSunLightDir;
	L.z *= g_fSpecularGradient;
	L = normalize(L);
	
	// 高光反射方向
	float3 R = 2.f * dot(V, vNewBaseNormal) * vNewBaseNormal - V;
	
	// 环境反射方向
	//float3 R2 = 2.f * dot(V, vBaseNormal) * vBaseNormal - V;
	float3 R2 = 2.f * dot(V, N) * N - V;
	// @}
	
	
	// 反射
	// @{
	// 计算视点反射点世界坐标与投影坐标
	//float3 vBaseRefrlVec = normalize(reflect(V, /*BaseNormal*/N));
	float3 vRefelWorldPos = normalize(reflect(V, float3(0, 0, 1))) * (-25.f) + kInput.vUVSetWorldPos.xyz;
	float4 vRefelProjPos = mul(float4(vRefelWorldPos, 1.f), g_mViewProj);
	vRefelProjPos.xy /= vRefelProjPos.w;
	
	// 计算视点反射点屏幕纹理坐标,采样反射纹理(场景纹理)
	//float2 vSceneRefelUV = GetTexCoordForPixel(vRefelProjPos.xy) - 0.05f * N.xy;
	//float3 vSceneRefelColor = tex2D(sdSceneSampler, vSceneRefelUV);
	
	// 计算场景反射纹理与天空反射纹理过渡系数
	//vRefelProjPos.xy = 1.f - pow(vRefelProjPos.xy, 4.f);
	//vRefelProjPos.xy = vRefelProjPos.yx;
	//vRefelProjPos.x = saturate(vRefelProjPos.x);
	//float fRefelRatio = (vRefelProjPos.y >= 0) ? vRefelProjPos.x * vRefelProjPos.y : 0;
	//float fRefelZ = 0.75f - R2.z * R2.z;
	//float fRefelRatio1 = (fRefelZ >= 0) ? fRefelZ * 1.3333f : 0;
	//fRefelRatio *= fRefelRatio1;

	// 计算视点反射点屏幕纹理坐标,采样天空纹理
	float2 vSkyRefelUV = vScreenUVSet - 0.5f * N.xy;
	float3 vSkyRefelColor = tex2D(sdRefelectSampler, float2(vSkyRefelUV.x, 1.f - vSkyRefelUV.y)).rgb;
	
	// 合成最终反射
	//float3 vReflectColor = lerp(vSkyRefelColor, vSceneRefelColor, fRefelRatio);
	float3 vReflectColor = vSkyRefelColor;
	// @}
	
	
	// 折射
	// @{
	// 计算折射纹理坐标
	//float2 vRefracScale = g_fRefractionDistortionStrength / max(0.002, kInput.vUVSetProjPos.z);
	//float2 vRefractionUV = vScreenUVSet - vRefracScale * N.xy;
	
	// 采样折射纹理
	//float4 vRefractColor = tex2D(sdRefractSampler, vRefractionUV);	
	
	// Calc distortion mask to fix pixel which should not be distorted
	//float fDistortionMask = vRefractColor.a > 0;
	//vRefractionUV = vScreenUVSet - vRefracScale * N.xy * fDistortionMask;
	
	// 采样折射纹理
	//vRefractColor = tex2D(sdRefractSampler, vRefractionUV);
	//vRefractColor.a = pow(vRefractColor.a, g_fAlphaCorrection);
	
	// 计算水面平滑度
	//float fWaterSmooth = vRefractColor.a;
	// @}
	
	// Calc reflection
	//float2 vRefelUV = vScreenUVSet;
	//vRefelUV.y = 1.f - vRefelUV.y;
	//float2 vReflectionUV = vRefelUV + vRefracScale * N.xy;
	
	// 计算菲涅尔系数
	float fFresnelFactor = dot(V, N);
	float fFresnel = tex1D(sdFresnelSampler, fFresnelFactor).r;
	fFresnel -= g_fWaterFresnel;
	fFresnel = saturate(fFresnel);
	
	// 采样场景深度
	float4 vGeoTex = tex2D(sdGeomSampler, vScreenUVSet);
	float fDepth = UnpackDepth(vGeoTex.xy);

	// 计算像素世界坐标
	float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarWorldPos.xyz / kInput.vUVSetFarWorldPos.w, fDepth);
	
	// 计算水面平滑度
	float fDeltDepth = kInput.vUVSetWorldPos.z - vPixelWorldPos.z;
	float fWaterSmooth = smoothstep(0.f, 2.f, fDeltDepth);	//saturate(fDeltDepth)
	fWaterSmooth = sqrt(fWaterSmooth);
	
	// 计算太阳光照，并合成到反射颜色
	// Caculate lighting which just take sunlight into consideration
	float3 vSunLight = g_fSpecularIntensity * g_vMainLightColor * saturate(L.z) * 
		pow(saturate(dot(L, R) /**0.5f + 0.5f*/), g_fSpecularPower);
	vReflectColor += vSunLight;
	
	// 计算最终颜色
	float4 vOutColor = 0;
	vOutColor.rgb = vReflectColor;
	vOutColor.a = max(0.4f, fFresnel + vSunLight) * fWaterSmooth;
	
	// 计算雾，并合成到最终颜色
	// Add height fog
	//float4 vFogColorAndFactor = CalcHeightFogCurveFactor(float4(kInput.vUVSetWorldPos.xyz, kInput.vUVSetViewPos.z), false)
	//vOutColor.rgb = vFogColorAndFactor.rgb + vOutColor.rgb * vFogColorAndFactor.w;
	
	return vOutColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_FFTWater(VS_OUTPUT kInput) : COLOR0
{
	// 采样法线贴图
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, kInput.vUVSet0).xyz;
	float3 vDetailNormal = tex2D(sdWaterNormalSampler, kInput.vUVSet0 * g_fWaterFresnel).xyz;
	
	// 观察方向
	float3 V = normalize(g_vViewPos - kInput.vUVSetWorldPos.xyz);	
	
	// 计算法线合成系数，并合成
	float fAdditionalReflection = V.x * V.x + V.y * V.y;
	fAdditionalReflection /= g_fNormalFadingDistance;
	float3 N = lerp(vDetailNormal, vBaseNormal, saturate(fAdditionalReflection));
	
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// 采样角色扰动波纹缓存，并合成到法线
	float3 vRipplePerturbation = tex2D(sdRippleNormalBufSampler, vScreenUVSet).xyz;
	N = normalize(vRipplePerturbation + N);
	
	//
	// @{
	V = normalize(V);
	float3 L = -g_vSunLightDir;
	float3 R = normalize(reflect(V, vDetailNormal));
	// @}
	
	
	// 反射
	// @{
	// 计算视点反射点世界坐标与投影坐标
	//float3 vBaseRefrlVec = normalize(reflect(V, /*BaseNormal*/N));
	float3 vRefelWorldPos = normalize(reflect(V, float3(0, 0, 1))) * (-25.f) + kInput.vUVSetWorldPos.xyz;
	float4 vRefelProjPos = mul(float4(vRefelWorldPos, 1.f), g_mViewProj);
	vRefelProjPos.xy /= vRefelProjPos.w;
	
	// 计算视点反射点屏幕纹理坐标,采样反射纹理(场景纹理)
	float2 vSceneRefelUV = GetTexCoordForPixel(vRefelProjPos.xy) - 0.05f * N.xy;
	float3 vSceneRefelColor = tex2D(sdSceneSampler, vSceneRefelUV).rgb;
	
	// 计算场景反射纹理与天空反射纹理过渡系数
	vRefelProjPos.xy = 1.f - pow(vRefelProjPos.xy, 4.f);
	vRefelProjPos.xy = vRefelProjPos.yx;
	vRefelProjPos.x = saturate(vRefelProjPos.x);
	float fRefelRatio = (vRefelProjPos.y >= 0) ? vRefelProjPos.x * vRefelProjPos.y : 0;
	float fRefelZ = 0.75f - R.z * R.z;
	float fRefelRatio1 = (fRefelZ >= 0) ? fRefelZ * 1.3333f : 0;
	fRefelRatio *= fRefelRatio1;

	// 计算视点反射点屏幕纹理坐标,采样天空纹理
	float2 vSkyRefelUV = vScreenUVSet - 0.5f * N.xy;
	float3 vSkyRefelColor = tex2D(sdRefelectSampler, float2(vSkyRefelUV.x, 1.f - vSkyRefelUV.y));
	
	// 合成最终反射
	float3 vReflectColor = lerp(vSkyRefelColor, vSceneRefelColor, fRefelRatio);
	// @}
	
	
	// 折射
	// @{
	// 计算折射纹理坐标
	float2 vRefracScale = g_fRefractionDistortionStrength / max(0.002, kInput.vUVSetProjPos.z);
	float2 vRefractionUV = vScreenUVSet - vRefracScale * N.xy;
	
	// 采样折射纹理
	float4 vRefractColor = tex2D(sdRefractSampler, vRefractionUV);	
	
	// Calc distortion mask to fix pixel which should not be distorted
	float fDistortionMask = vRefractColor.a > 0;
	vRefractionUV = vScreenUVSet - vRefracScale * N.xy * fDistortionMask;
	
	// 采样折射纹理
	vRefractColor = tex2D(sdRefractSampler, vRefractionUV);
	vRefractColor.a = pow(vRefractColor.a, g_fAlphaCorrection);
	
	// 计算水面平滑度
	float fWaterSmooth = vRefractColor.a;
	// @}
	
	// Calc reflection
	//float2 vRefelUV = vScreenUVSet;
	//vRefelUV.y = 1.f - vRefelUV.y;
	//float2 vReflectionUV = vRefelUV + vRefracScale * N.xy;
	
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
	vOutColor.rgb = lerp(vRefractColor.rgb, vOutColor.rgb, fWaterSmooth);
	
	// 计算雾，并合成到最终颜色
	// Add height fog
	//float4 vFogColorAndFactor = CalcHeightFogCurveFactor(float4(kInput.vUVSetWorldPos.xyz, kInput.vUVSetViewPos.z), false)
	//vOutColor.rgb = vFogColorAndFactor.rgb + vOutColor.rgb * vFogColorAndFactor.w;
	
	return vOutColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// NormalMap的水面
technique Water_NormalBlendWater
<
	string Description = "Water_NormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_Water ();
		PixelShader 	= compile ps_3_0 PS_Main_NormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------
// NormalMap的水面(无反射)
technique Water_NoReflectNormalBlendWater
<
	string Description = "Water_NoReflectNormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_NoReflectNormalBlendWater ();
		PixelShader 	= compile ps_3_0 PS_Main_NoReflectNormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------
// FFTMap的水面
technique Water_FFTWater
<
	string Description = "Water_FFTWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_Water ();
		PixelShader 	= compile ps_3_0 PS_Main_FFTWater ();
	}
}
//---------------------------------------------------------------------------------------