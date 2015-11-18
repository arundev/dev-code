//*************************************************************************************************
// 内容:	地形Normal/Depth绘制到G-Buffer
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
SD_POINT_CLAMP_SAMPLE(0, sdDepthSampler,		sdDepthBuf, 		false);		// 地形屏幕深度缓存
SD_LINEAR_WRAP_SAMPLE(1, sdBaseNormalSampler, 	sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_LINEAR_WRAP_SAMPLE(2, sdBlendSampler,		sdBlendTex,			false);		// 地形混合权重贴图

SD_LINEAR_WRAP_SAMPLE( 3, sdNormalSampler00, sdNormalTex00, false)
SD_LINEAR_WRAP_SAMPLE( 4, sdNormalSampler01, sdNormalTex01, false)
SD_LINEAR_WRAP_SAMPLE( 5, sdNormalSampler02, sdNormalTex02, false)
SD_LINEAR_WRAP_SAMPLE( 6, sdNormalSampler03, sdNormalTex03, false)
SD_LINEAR_WRAP_SAMPLE( 7, sdNormalSampler04, sdNormalTex04, false)
SD_LINEAR_WRAP_SAMPLE( 8, sdNormalSampler05, sdNormalTex05, false)
SD_LINEAR_WRAP_SAMPLE( 9, sdNormalSampler06, sdNormalTex06, false)
SD_LINEAR_WRAP_SAMPLE(10, sdNormalSampler07, sdNormalTex07, false)
SD_LINEAR_WRAP_SAMPLE(11, sdNormalSampler08, sdNormalTex08, false)
SD_LINEAR_WRAP_SAMPLE(12, sdNormalSampler09, sdNormalTex09, false)
SD_LINEAR_WRAP_SAMPLE(13, sdNormalSampler10, sdNormalTex10, false)
SD_LINEAR_WRAP_SAMPLE(14, sdNormalSampler11, sdNormalTex11, false)

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
	float3	vUVNearClipWorldPos	: TEXCOORD1;	// 当前点对应近裁剪面上的点的世界坐标
	float3	vUVFarClipWorldPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的世界坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 当前点对应近远裁剪面上的点的世界坐标
	kOutput.vUVNearClipWorldPos = mul(float4(kInput.vPos.xy, 0.f, 1.f), g_mDepthToWorld).xyz;
	kOutput.vUVFarClipWorldPos = mul(float4(kInput.vPos.xy, 1.f, 1.f), g_mDepthToWorld).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 渲染远处,只渲染基础法线贴图
float4 PS_Main_Far_BaseNormal(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 裁剪掉指定近平面以内的像素
	clip(length(vWorldPos - kInput.vUVNearClipWorldPos) - g_fTerrainFarStart);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// 变换Normal到观察空间
	// 	1.需要乘以逆转置矩阵,
	//	2.ViewMatrix旋转部分是正交矩阵,平移部分不是,我们只需要旋转变换
	//	3.g_mView旋转部分(3x3)是正交矩阵，逆转置矩阵是它自己
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	
	// 输出打包的法线和深度
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// 渲染近处,渲染基础法线和法线贴图, XY平面
float4 PS_Main_Near_BaseNormalAndNormalMap_XY(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.z - 0.001f);
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// 计算当前点的切线空间
	float3 vWorldBinormal 	= cross(float3(1.f, 0.f, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(vWorldPos.x, -vWorldPos.y);
	
	// 采样NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	vNormal = vNormal * 2.f - 1.f;
	
	// 归一化
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal从切线空间转换到世界坐标(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// 变换Normal到观察空间
	// 	1.需要乘以逆转置矩阵,
	//	2.ViewMatrix旋转部分是正交矩阵,平移部分不是,我们只需要旋转变换
	//	3.g_mView旋转部分(3x3)是正交矩阵，逆转置矩阵是它自己
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal) * vPlanarWeight.z);
}

//---------------------------------------------------------------------------------------
// 渲染近处,渲染基础法线和法线贴图, YZ平面
float4 PS_Main_Near_BaseNormalAndNormalMap_YZ(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.x - 0.001f);
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// 计算当前点的切线空间
	float3 vWorldBinormal 	= cross(float3(0.f, sign(vWorldNormal.x), 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(vWorldPos.y * sign(vWorldNormal.x), -vWorldPos.z);
	
	// 采样NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	// 变换DetailNormal(从[0,1]到[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// 归一化
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal从切线空间转换到世界坐标(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal.xyz + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// 变换Normal到观察空间
	// 	1.需要乘以逆转置矩阵,
	//	2.ViewMatrix旋转部分是正交矩阵,平移部分不是,我们只需要旋转变换
	//	3.g_mView旋转部分(3x3)是正交矩阵，逆转置矩阵是它自己
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth * step(vPlanarWeight.z, 0.001f), PackNormal(vViewNormal) * vPlanarWeight.x);
}

//---------------------------------------------------------------------------------------
// 渲染近处,渲染基础法线和法线贴图, XZ平面
float4 PS_Main_Near_BaseNormalAndNormalMap_XZ(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.y - 0.001f);
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// 计算当前点的切线空间
	float3 vWorldBinormal 	= cross(float3(-sign(vWorldNormal.y), 0.f, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(-vWorldPos.x * sign(vWorldNormal.y), -vWorldPos.z);
	
	// 采样NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	// 变换DetailNormal(从[0,1]到[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// 归一化
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal从切线空间转换到世界坐标(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal.xyz + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// 变换Normal到观察空间
	// 	1.需要乘以逆转置矩阵,
	//	2.ViewMatrix旋转部分是正交矩阵,平移部分不是,我们只需要旋转变换
	//	3.g_mView旋转部分(3x3)是正交矩阵，逆转置矩阵是它自己
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	float2 vOutputDepth = step(vPlanarWeight.xz, 0.001f);
	return float4(vPackedDepth * vOutputDepth.x * vOutputDepth.y, PackNormal(vViewNormal) * vPlanarWeight.y);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Far_BaseNormal_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Far_BaseNormal_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Far_BaseNormal ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XY_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XY_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_XY ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_YZ_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_YZ_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_YZ ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XZ_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XZ_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_XZ ();
	}
}
//---------------------------------------------------------------------------------------