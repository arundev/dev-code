//*************************************************************************************************
// ����:	����Mesh���ƶ���
//---------------------------------------------------------
// ����:		
// ����:		2012-08-01
// ����޸�:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_QUAD_MESH_H__
#define _TERRAIN_QUAD_MESH_H__
#include <sdVector3.h>

class sdTerrain;
//-------------------------------------------------------------------------------------------------
// ����QuadTree�ڵ��Mesh����
//	1.ʹ�õ���Skirt,����IndexBuffer
//	2.����GPU��VertexBuffer,�Ż����ж���
//	  ��XBOX��,����24��vertex���ݵ�Post Vertex Cache.Cache���㷨��FIFO��(PC�ϵ�Ҳ��һ����)
//	3.ʹ��TriangleStrip������
//	4.ʹ��IndexStream
// (����Ԥ����MeshCache���Ż��ڴ�)
//-------------------------------------------------------------------------------------------------
class sdQuadMesh : public NiMesh
{
public:
	//	@param[in]	uiX,uiY		��HeightMap�е�ȡ��λ��
	//	@param[in]	uiSpacing	��HeightMap�е�ȡ�����
	//	@param[in]	uiSize		MeshTile�ߴ�,��ȡ����
	//	@param[in]	fSkirtDepth	MeshTile��Եȹ�ӳ���
	sdQuadMesh(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth, sdTerrain* pkTerrain = NULL);
	~sdQuadMesh();

	// ��̬��ʼ��
	// (������Ҫ��Ϊ�˳�ʼ�������㻺��)
	static void	StaticInitialize(uint uiTileSize);
	static void StaticDestroy();

	// ����Mesh,���ص�ǰMesh����С���߶�(�ڲ�ʹ��,���ڱ༭��)
	//	@param[out]	fMinHeight	Mesh��С�߶�
	//	@param[out]	fMaxHeight	Mesh���߶�
	//	@param[in]	fSkirtDepth	Mesh��Եȹ�Ӹ߶�
	void	UpdateGeometry(float& fMinHeight, float& fMaxHeight, float fSkirtDepth);

protected:
	// �Ӹ߶�ͼ����׼���߶�����(��ms_kVertexBuf)
	void CreateVertexData(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth);

	// ��������������(��ms_kVertexBuf)
	void CreateVertexStream();	

	// ������������/���㻺��ߴ�
	//	@param[in]	uiSize	MeshTile�ߴ�(����)
	static uint	CalcIndexNum(uint uiSize);
	static uint	CalcVertexNum(uint uiSize);

	// ��������������
	static void CreateIndexStream(uint uiSize);
	static void CreateIndexMap(uint uiSize);

protected:
	// �������ζ���
	sdTerrain* m_pkTerrain;

	// ��ǰTileMesh��HeightMap�е�ȡ��λ��λ���������HeightMap��ƫ��
	// ÿ��Mesh����ͬ,��Ϊm_uiTileSize��������������:0/64/128/192/256...
	uint	m_uiX;
	uint	m_uiY;

	// ��ǰTileMesh��HeightMap�е�ȡ�����
	// ��ͬLOD��Mesh��ͬ,һ����1/2/4/8/16/32
	uint	m_uiSpacing;

	// ����������
	NiDataStreamPtr			m_spVertexStream;

	// ��������
	NiFloatsExtraDataPtr	m_spExtraData;

protected:
	// ����Ķ�������
	static	NiDataStreamPtr	ms_spIndexStream;

	// ����
	static	std::vector<Base::Math::sdVector3us>	ms_kIndexMap;
	static	std::vector<short>						ms_kVertexBuf;
};
NiSmartPointer(sdQuadMesh);
//-------------------------------------------------------------------------------------------------
#endif