//*************************************************************************************************
// ����:	��ͼͼ�����ɹ���
//---------------------------------------------------------
// ����:		
// ����:		2012-08-14
// ����޸�:	2013-05-15
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SURFACE_ATLAS_H__
#define _SD_ENGINE_SURFACE_ATLAS_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// ���ڽ����������Tileƴ�Ӳ������һ�Ŵ��Surface����,Ҫ��
//	1.�������������ָ����ʽ
//	2.���е������Ѿ�ָ��λ�úͳߴ�(�ⲿ����)
//	2.���������ָ���ߴ�,ָ����ʽ
//-------------------------------------------------------------------------------------------------
class sdSurfaceAtlas : public NiRefObject
{
public:
	sdSurfaceAtlas(uint uiWidth, uint uiHeight, D3DFORMAT eFormat);
	~sdSurfaceAtlas();

	// Tile����������
	uint	AllocTile(uint uiLeft, uint uiTop, uint uiWidth, uint uiHeight);
	bool	DeallocTile(uint uiIndex);
	void	DeallocAllTiles();

	// ��/���TileSurface��Tile
	// The specified surface must locates at the system memory pool, and has the same format with atlas.
	bool	BindSourceSurfaceToTile(uint uiIndex, IDirect3DSurface9* pSurface, uint uiLeft = 0, uint uiTop = 0);
	void	UnbindSourceSurfaceFromTile(uint uiIndex);

	// ��/���TargetSurface
	// The specified surface must locates at the default pool,and has the same resolution and format with atlas.
	bool	BindTargetSurface(IDirect3DSurface9* pSurface);
	void	UnbindTargetSurface(); 

	// ����Tile��TargetSurface
	bool	UpdateTargetSurface();

	// 
	uint	GetWidth() const { return m_uiWidth;}
	uint	GetHeight() const { return m_uiHeight;}

protected:
	// ����һ��Tile��������Դ
	struct stAtlasTileSurface
	{
		IDirect3DSurface9*	d3dSurface;	///< D3DԴ����
		uint	uiLeft;
		uint	uiTop;

		stAtlasTileSurface(): d3dSurface(NULL), uiLeft(0), uiTop(0) {}
		stAtlasTileSurface(IDirect3DSurface9* surface, uint left, uint top): d3dSurface(surface), uiLeft(left), uiTop(top) {}

		void Reset()
		{
			d3dSurface = NULL;
			uiLeft = 0;
			uiTop = 0;
		}

		bool operator==(const stAtlasTileSurface& rhs) const
		{
			return (d3dSurface == rhs.d3dSurface) && (uiLeft == rhs.uiLeft) && (uiTop == rhs.uiTop);
		}
	};

	// ������ǰ�����ϵ�һ��Tile
	struct stAtlasTile : public NiMemObject
	{
		// ��ǰTile��Ŀ������
		uint uiLeft;
		uint uiTop;
		uint uiWidth;
		uint uiHeight;

		// ��ǰTile��ԴSurface
		stAtlasTileSurface kSource;

		// ���Surface�Ѿ����޸Ĺ�
		bool bDirty;

		stAtlasTile(uint left, uint top, uint w, uint h): uiLeft(left), uiTop(top), uiWidth(w), uiHeight(h), bDirty(false){}
	};
	typedef std::vector<stAtlasTile> AtlasTileVec;
	typedef std::vector<stAtlasTile>::iterator AtlasTileVecItr;


protected:
	uint	m_uiWidth;
	uint	m_uiHeight;

	// ��ǰ�����Ŀ��Surface��ʽ
	D3DFORMAT	m_eFormat;	
	bool		m_bIsDDSFormat;	

	// ����Tile�б�
	AtlasTileVec m_kAtlasTileVec;

	// ���Surface
	IDirect3DSurface9*	m_d3dTargetSurface;
};
NiSmartPointer(sdSurfaceAtlas);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif