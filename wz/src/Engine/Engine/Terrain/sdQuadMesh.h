//*************************************************************************************************
// 内容:	地形Mesh绘制对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-01
// 最后修改:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_QUAD_MESH_H__
#define _TERRAIN_QUAD_MESH_H__
#include <sdVector3.h>

class sdTerrain;
//-------------------------------------------------------------------------------------------------
// 地形QuadTree节点的Mesh对象
//	1.使用地形Skirt,共享IndexBuffer
//	2.根据GPU的VertexBuffer,优化排列顶点
//	  在XBOX中,保存24个vertex数据的Post Vertex Cache.Cache的算法是FIFO的(PC上的也是一样的)
//	3.使用TriangleStrip来绘制
//	4.使用IndexStream
// (考虑预分配MeshCache以优化内存)
//-------------------------------------------------------------------------------------------------
class sdQuadMesh : public NiMesh
{
public:
	//	@param[in]	uiX,uiY		在HeightMap中的取样位置
	//	@param[in]	uiSpacing	在HeightMap中的取样间隔
	//	@param[in]	uiSize		MeshTile尺寸,即取样数
	//	@param[in]	fSkirtDepth	MeshTile边缘裙子长度
	sdQuadMesh(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth, sdTerrain* pkTerrain = NULL);
	~sdQuadMesh();

	// 静态初始化
	// (这里主要是为了初始化共享顶点缓存)
	static void	StaticInitialize(uint uiTileSize);
	static void StaticDestroy();

	// 更新Mesh,返回当前Mesh的最小最大高度(内部使用,用于编辑器)
	//	@param[out]	fMinHeight	Mesh最小高度
	//	@param[out]	fMaxHeight	Mesh最大高度
	//	@param[in]	fSkirtDepth	Mesh边缘裙子高度
	void	UpdateGeometry(float& fMinHeight, float& fMaxHeight, float fSkirtDepth);

protected:
	// 从高度图创建准备高度数据(到ms_kVertexBuf)
	void CreateVertexData(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth);

	// 创建顶点数据流(从ms_kVertexBuf)
	void CreateVertexStream();	

	// 计算索引缓存/顶点缓存尺寸
	//	@param[in]	uiSize	MeshTile尺寸(格数)
	static uint	CalcIndexNum(uint uiSize);
	static uint	CalcVertexNum(uint uiSize);

	// 创建索引数据流
	static void CreateIndexStream(uint uiSize);
	static void CreateIndexMap(uint uiSize);

protected:
	// 所属地形对象
	sdTerrain* m_pkTerrain;

	// 当前TileMesh在HeightMap中的取样位置位置相对整个HeightMap的偏移
	// 每个Mesh都不同,且为m_uiTileSize的整数倍，例如:0/64/128/192/256...
	uint	m_uiX;
	uint	m_uiY;

	// 当前TileMesh在HeightMap中的取样间隔
	// 不同LOD的Mesh不同,一般是1/2/4/8/16/32
	uint	m_uiSpacing;

	// 顶点数据流
	NiDataStreamPtr			m_spVertexStream;

	// 额外数据
	NiFloatsExtraDataPtr	m_spExtraData;

protected:
	// 共享的顶点索引
	static	NiDataStreamPtr	ms_spIndexStream;

	// 缓存
	static	std::vector<Base::Math::sdVector3us>	ms_kIndexMap;
	static	std::vector<short>						ms_kVertexBuf;
};
NiSmartPointer(sdQuadMesh);
//-------------------------------------------------------------------------------------------------
#endif