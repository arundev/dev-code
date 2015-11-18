//*************************************************************************************************
// 内容:	地形Normal/Depth绘制到G-Buffer(V2,ruantianlong modifiy)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-09-11
// 最后修改:	2013-09-12
//*************************************************************************************************
#include "terrain_common_v2.h"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_CUBESAMPLE(0, sdPlanarTableSampler, 	sdPlanarTableTex);				//
SD_POINT_CLAMP_SAMPLE(1, sdDepthSampler, 			sdDepthBuf, 		false);		// 地形屏幕深度缓存
SD_LINEAR_WRAP_SAMPLE(2, sdBaseNormalSampler, 		sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_POINT_CLAMP_SAMPLE(3, sdTileSampler,				sdTileTex,			false);		// 地形TileMap
SD_LINEAR_WRAP_SAMPLE(4, sdBlendSampler,			sdBlendTex,			false);		// 地形BlendMap
SD_POINT_CLAMP_SAMPLE(5, sdAtlasTableSampler,		sdAtlasTableTex,	false);		// 地形法线贴图查询表
SD_LINEAR_WRAP_SAMPLE(6, sdNormalAtlasSampler,		sdNormalAtlasTex,	false);		// 地形法线贴图图集

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
//	float3	vUVFarClipViewPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的观察坐标
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
	
	// 当前点对应远裁剪面上的点的世界坐标
	float4 vUVFarClipProjPos  = float4(kInput.vPos.xy, 1.f, 1.f);
	float4 vUVFarClipWorldPos = mul(vUVFarClipProjPos, g_mDepthToWorld);
	kOutput.vUVFarClipWorldPos = vUVFarClipWorldPos.xyz;
	
	// 当前点对应远裁剪面上的点的观察坐标
	//	1.需要g_mView的平移变换,所以要恢复float4
	// 	2.替换掉w分量是避免计算误差累积
	//kOutput.vUVFarClipViewPos  = mul(float4(vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
// 渲染近处,渲染基础法线和法线贴图(新版，主要是改进了Mipmap计算)
float4 PS_Main_Near_BaseNormalAndNormalMap_New(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float2 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet).xy;
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));	
	// @}
	
	// TileMap
	// @{
	// 根据UV采样TileMap(这里从[0,1]恢复到[0,255]的图层索引区间)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;	
	// @}
	
	// BlendMap
	// @{
	// 计算新的UV(将采样点从BlendTex的中心点向背离Tile中心方向偏移)
	//	1.计算当前Fragment相对TexTile中心点的偏移(单位:TexTile)
	//	2.将当前Fragment相对TexTile中心点的偏移转化为对BlendMapPixel中心偏移(单位:BlendMapPixel)
	//	3.从UVSet减去对BlendMapPixel中心偏移(注意UVSet本身没有处理半像素偏移)
	//
	// WZ原式:
	//	float2 tileCenterOffset = frac(vUVSet * (2048.0 / 4.0)) - 0.5;
	//	vUVSet -= tileCenterOffset * (1.0 / 2048.0);
	//
	// 本地实现:
	//	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5;
	//	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	//
	// 修订版(仅仅修正对BlendMap像素中心偏移)
	//	float2 vUVSet2 = vUVSet + g_vRecipBlendMapSize * 0.5f;
	//
	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	
	// 根据UV采样BlendMap
	float3 vBlendTex = tex2D(sdBlendSampler, vUVSet2).rgb;
	
	// 归一化权重
	float fTotalWeight = dot(vBlendTex, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	// @}
	
	// 计算切线空间
	// @{
	// 采样立方体纹理
	//	1.GB坐标系是X向右Y向前Z向上
	//	2.D3D坐标系是X向右Y向上Z向前
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	
	// 计算新的地形UV(??)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	
	// 计算当前点的切线空间
	float3 vWorldBinormal 	= cross(float3(vPlanarVec.xy, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	// 法线混合(NormalAtlasTable, NormalAtlas)
	// @{
//	// 计算当前像素到观察点矢量
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
//	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
//	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
//	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
//	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// 计算图集UV
//	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fNormalAtlasLevelScale + g_fNormalAtlasLevelOffset);
//	
//	// 贴图混合
//	float3 vNormal = SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3).xyz * vBlendTex.b +
//		SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3).xyz * vBlendTex.g +
//		SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3).xyz * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);										///< 每张NormalMap的原始尺寸是512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
	
	float2 avUVSet1[3];		///< 三张贴图的UV坐标
	float3 vMipmapLevel;	///< 三张贴图的Mipmap等级
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1[0] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1[1] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1[2] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	// 计算纹理页的在纹理图集中的位置
	int2 iTexRowCol[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;				///< 计算每边的原始纹理数(每张DiffuseMap的原始尺寸是512)
	for (int i = 0; i < 3; ++i)
	{
		iTexRowCol[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);			///< 计算纹理页所在行
		iTexRowCol[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));	///< 计算纹理页所在列
	}

	// 将纹理页的采样坐标转换到纹理图集上的采样坐标
	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);							///< 取出mipmap的等级(0,1,2,3...)
	float3 vMipmapScale = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);	///< 计算mipmap相对原始纹理的缩放比例(1,1/2,1/4,1/8...)
	avUVSet2[0].xy = ComputeUV(vMipmapScale[0], avUVSet1[0], iTexRowCol[0]);
	avUVSet2[1].xy = ComputeUV(vMipmapScale[1], avUVSet1[1], iTexRowCol[1]);
	avUVSet2[2].xy = ComputeUV(vMipmapScale[2], avUVSet1[2], iTexRowCol[2]);
	
	float3 vMipmapScale2 = vMipmapScale * 0.5f;						///< 计算下一级mipmap相对原始纹理的缩放比例
	avUVSet2[0].zw = ComputeUV(vMipmapScale2[0], avUVSet1[0], iTexRowCol[0]);
	avUVSet2[1].zw = ComputeUV(vMipmapScale2[1], avUVSet1[1], iTexRowCol[1]);
	avUVSet2[2].zw = ComputeUV(vMipmapScale2[2], avUVSet1[2], iTexRowCol[2]);

	// 贴图混合
	float3 vWeight = frac(vMipmapLevel);							///< 取出mipmap的浮点值部分作为两层mipmap之间的插值权重
	float3 vNormal = (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
	// 变换DetailNormal(从[0,1]到[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// 缩放DetailNormal
	vNormal.xy *= g_fTerrainNormalScale;
	
	// 与远处法线区域做过渡(从视点到分割线进行线性过渡,过渡效果很差,几乎等于没有,待进一步优化)
	float fNormalSmooth = saturate(5.f - 5.f * length(vWorldPos - g_vViewPos) / g_fTerrainFarStart);
	vNormal.xy *= fNormalSmooth;
	
	// 归一化
	vNormal = normalize(vNormal);
	//vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal从切线空间转换到世界坐标(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// 变换Normal到观察空间
	// 	1.需要乘以逆转置矩阵,
	//	2.ViewMatrix旋转部分是正交矩阵,平移部分不是,我们只需要旋转变换
	//	3.g_mView旋转部分(3x3)是正交矩阵，逆转置矩阵是它自己
	float3 vViewNormal = mul(float4(vNormal, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_V2
<
	string Description = "Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_New";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Near_BaseNormalAndNormalMap_New ();
	}
}
//---------------------------------------------------------------------------------------