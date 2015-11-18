//*************************************************************************************************
// 内容:	地形着色(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-23
// 最后修改:	2013-05-02
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4 g_vRecipUVRepeats[3]	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,			sdGeomBuf, 			false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(1, sdLightSampler,		sdLightBuf, 		false);		// 屏幕局部光照缓存

SD_LINEAR_WRAP_SAMPLE(2, sdBaseDiffuseSampler, 	sdBaseDiffuseTex, 	false);		// 地形基本漫反射贴图
SD_LINEAR_WRAP_SAMPLE(3, sdBaseNormalSampler, 	sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_LINEAR_WRAP_SAMPLE(4, sdBlendSampler,		sdBlendTex,			false);		// 地形混合权重贴图

//SD_LINEAR_WRAP_SAMPLE( 5, sdDiffuseSampler00, sdDiffuseTex00, false)
//SD_LINEAR_WRAP_SAMPLE( 6, sdDiffuseSampler01, sdDiffuseTex01, false)
//SD_LINEAR_WRAP_SAMPLE( 7, sdDiffuseSampler02, sdDiffuseTex02, false)
//SD_LINEAR_WRAP_SAMPLE( 8, sdDiffuseSampler03, sdDiffuseTex03, false)
//SD_LINEAR_WRAP_SAMPLE( 9, sdDiffuseSampler04, sdDiffuseTex04, false)
//SD_LINEAR_WRAP_SAMPLE(10, sdDiffuseSampler05, sdDiffuseTex05, false)
//SD_LINEAR_WRAP_SAMPLE(11, sdDiffuseSampler06, sdDiffuseTex06, false)
//SD_LINEAR_WRAP_SAMPLE(12, sdDiffuseSampler07, sdDiffuseTex07, false)
//SD_LINEAR_WRAP_SAMPLE(13, sdDiffuseSampler08, sdDiffuseTex08, false)
//SD_LINEAR_WRAP_SAMPLE(14, sdDiffuseSampler09, sdDiffuseTex09, false)
//SD_LINEAR_WRAP_SAMPLE(15, sdDiffuseSampler10, sdDiffuseTex10, false)
//SD_LINEAR_WRAP_SAMPLE(16, sdDiffuseSampler11, sdDiffuseTex11, false)
SD_SAMPLE_EX( 5, sdDiffuseSampler00, sdDiffuseTex00, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX( 6, sdDiffuseSampler01, sdDiffuseTex01, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX( 7, sdDiffuseSampler02, sdDiffuseTex02, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX( 8, sdDiffuseSampler03, sdDiffuseTex03, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX( 9, sdDiffuseSampler04, sdDiffuseTex04, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(10, sdDiffuseSampler05, sdDiffuseTex05, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(11, sdDiffuseSampler06, sdDiffuseTex06, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(12, sdDiffuseSampler07, sdDiffuseTex07, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(13, sdDiffuseSampler08, sdDiffuseTex08, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(14, sdDiffuseSampler09, sdDiffuseTex09, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(15, sdDiffuseSampler10, sdDiffuseTex10, WRAP, LINEAR, ANISOTROPIC, LINEAR)
SD_SAMPLE_EX(16, sdDiffuseSampler11, sdDiffuseTex11, WRAP, LINEAR, ANISOTROPIC, LINEAR)

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点
	float2	vUVSet0			: TEXCOORD0;	// 屏幕矩形纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// 当前点对应远裁剪面上的点的世界坐标
	float3	vUVFarClipViewPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的观察坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 当前点对应远裁剪面上的点的世界坐标
	float4 vUVFarClipProjPos  = float4(kInput.vPos.xy, 1.f, 1.f);
	float4 vUVFarClipWorldPos = mul(vUVFarClipProjPos, g_mDepthToWorld);
	kOutput.vUVFarClipWorldPos = vUVFarClipWorldPos.xyz;
	
	// 当前点对应远裁剪面上的点的观察坐标
	// (替换掉w分量是为了避免计算误差累积?)
	kOutput.vUVFarClipViewPos  = mul(float4(vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
// 平面着色XY
float4 PS_Main_Planar_XY(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize.xy;
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.z - 0.001f);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	
	// 计算总权重
	float fTotalWeight = dot(float3(dot(vBlendWeight00, 1.f), dot(vBlendWeight01, 1.f), dot(vBlendWeight02, 1.f)), 1.f);
	
	// 采样BlendMap
	float4 vBaseDiffuseDarkColor = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.5f));

#ifdef _SD_EDITOR
	vBaseDiffuseDarkColor =  max(vBaseDiffuseDarkColor, float4(g_vVertexColorMask, g_fLightMapMask));
#endif
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(vWorldPos.x, -vWorldPos.y);
	
	// 采样NormalMap
	float4 vDiffuseGloss = tex2D(sdDiffuseSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	vDiffuseGloss /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	
	vDiffuseGloss *= float4(vBaseDiffuseDarkColor.rgb, 1.f);
	// @}
	
	
	// 计算最终颜色
	// @{
	// 采样LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// 计算观察方向与法线
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);	
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// 合成光照
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBaseDiffuseDarkColor.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// 合成最终颜色
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
					
	vColor *= vPlanarWeight.z;
	// @}
	
	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 平面着色YZ
float4 PS_Main_Planar_YZ(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize.xy;
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.x - 0.001f);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	
	// 采样BlendMap
	float4 vBaseDiffuseDarkColor = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.5f));

#ifdef _SD_EDITOR
	vBaseDiffuseDarkColor =  max(vBaseDiffuseDarkColor, float4(g_vVertexColorMask, g_fLightMapMask));
#endif
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(vWorldPos.y * sign(vBaseNormalTex.x - 0.5f), -vWorldPos.z);
	
	// 采样NormalMap
	float4 vDiffuseGloss = tex2D(sdDiffuseSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	
	vDiffuseGloss *= float4(vBaseDiffuseDarkColor.rgb, 1.f);
	// @}
	
	
	// 计算最终颜色
	// @{
	// 采样LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// 计算观察方向与法线
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// 合成光照
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBaseDiffuseDarkColor.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// 合成最终颜色
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
					
	vColor *= vPlanarWeight.x;
	// @}
	
	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 平面着色XZ
float4 PS_Main_Planar_XZ(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize.xy;
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.y - 0.001f);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	
	// 采样BlendMap
	float4 vBaseDiffuseDarkColor = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.5f));

#ifdef _SD_EDITOR
	vBaseDiffuseDarkColor =  max(vBaseDiffuseDarkColor, float4(g_vVertexColorMask, g_fLightMapMask));
#endif
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(-vWorldPos.x * sign(vBaseNormalTex.y - 0.5f), -vWorldPos.z);
	
	// 采样NormalMap
	float4 vDiffuseGloss = tex2D(sdDiffuseSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	
	vDiffuseGloss *= float4(vBaseDiffuseDarkColor.rgb, 1.f);
	// @}
	
	
	// 计算最终颜色
	// @{
	// 采样LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// 计算观察方向与法线
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// 合成光照
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBaseDiffuseDarkColor.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// 合成最终颜色
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
					
	vColor *= vPlanarWeight.y;
	// @}
	
	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_XY
<
	string Description = "Terrain_AtlasShading_Planar_XY";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_XY ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_YZ
<
	string Description = "Terrain_AtlasShading_Planar_YZ";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_YZ ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_XZ
<
	string Description = "Terrain_AtlasShading_Planar_XZ";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_XZ ();
	}
}
//---------------------------------------------------------------------------------------