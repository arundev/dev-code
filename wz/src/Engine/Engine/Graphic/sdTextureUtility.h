//*************************************************************************************************
// 内容:	纹理工具,主要是辅助处理D3D纹理
//
// 参考资料:
//			http://en.wikipedia.org/wiki/Data_compression
//			http://en.wikipedia.org/wiki/Image_compression
//			http://en.wikipedia.org/wiki/Texture_compression
//			http://en.wikipedia.org/wiki/Block_Truncation_Coding
//			http://en.wikipedia.org/wiki/Color_Cell_Compression
//
//			http://en.wikipedia.org/wiki/S3_Texture_Compression
//			http://en.wikipedia.org/wiki/PVRTC
//			http://en.wikipedia.org/wiki/Ericsson_Texture_Compression
//			http://en.wikipedia.org/wiki/3Dc
//			http://en.wikipedia.org/wiki/Adaptive_Scalable_Texture_Compression
//
//			http://blog.csdn.net/lhc717/article/details/6802951	DXT纹理压缩
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:	2012-05-14
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_UTILITY_H__
#define _SD_ENGINE_TEXTURE_UTILITY_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-----------------------------------------------------------------------------
// DDS纹理工具,依赖于GB/D3D
//-----------------------------------------------------------------------------
class sdTextureUtility : public NiMemObject
{
public:
	// 用作输出的纹理格式
	enum eCompressFMT
	{
		DXT1 = 0,
		DXT2,
		DXT3,
		DXT4,
		DXT5,
	};

	// 用作输入输出的目标缓存
	struct stBuffer
	{
		uint	uiSize;
		void*	pBits;
	};

public:
	// 纹理压缩
	//	@param[in]	pSrcData			输入数据
	//	@param[in]	uiWidth,uiHeight	输入数据数据尺寸
	//	@param[in]	uiBytesPerPixel		输入数据每像素通道数/字节数(8:1 16:2 ...)
	//	@param[in]	eFormat				目标压缩格式,目前只支持DXT1~DXT5
	//	@param[out]	kDstBuffer			输出目标缓存
	static bool Compress(void* pSrcData, uint uiWidth, uint uiHeight, uint uiBytesPerPixel, eCompressFMT eFormat, stBuffer& kDstBuffer);
	
	// 纹理解压缩
	//	@param[in]	kSrcBuffer		输入缓存
	//	@param[in]	uiBytesPerPixel	输入数据每像素通道数
	//	@param[out]	kDstBuffer		输出目标缓存
	static bool Uncompress(stBuffer& kSrcBuffer, uint uiBytesPerPixel, stBuffer& kDstBuffer);

	// DXT5纹理转换为DXT1纹理
	static IDirect3DTexture9* ConvertDXT5ToDXT1(IDirect3DTexture9* d3dDXT5Tex);

	// 检查纹理格式
	static bool	IsDDSFormat(D3DFORMAT eFormat);
	static bool IsZStencilFormat(D3DFORMAT eFormat);

protected:
	// 输入格式转换为D3D格式
	static DWORD ConvertCompressFMT2D3DFMT(eCompressFMT eFormat);

	// 输入像素大小估计D3D格式
	static DWORD ConvertNumBytes2D3DFMT(uint uiBytesPerPixel);
};
//-----------------------------------------------------------------------------
// DDS文件保存辅助类
//-----------------------------------------------------------------------------
// 常量定义，参考ddraw.h文件
static const unsigned int FOURCC_DDS	= MAKEFOURCC('D', 'D', 'S', ' ');
static const unsigned int FOURCC_DXT1	= MAKEFOURCC('D', 'X', 'T', '1');
static const unsigned int FOURCC_DXT2	= MAKEFOURCC('D', 'X', 'T', '2');
static const unsigned int FOURCC_DXT3	= MAKEFOURCC('D', 'X', 'T', '3');
static const unsigned int FOURCC_DXT4	= MAKEFOURCC('D', 'X', 'T', '4');
static const unsigned int FOURCC_DXT5	= MAKEFOURCC('D', 'X', 'T', '5');
static const unsigned int FOURCC_RXGB	= MAKEFOURCC('R', 'X', 'G', 'B');
static const unsigned int FOURCC_ATI1	= MAKEFOURCC('A', 'T', 'I', '1');
static const unsigned int FOURCC_ATI2	= MAKEFOURCC('A', 'T', 'I', '2');
static const unsigned int FOURCC_A2XY	= MAKEFOURCC('A', '2', 'X', 'Y');
static const unsigned int FOURCC_DX10	= MAKEFOURCC('D', 'X', '1', '0');
static const unsigned int FOURCC_UVER	= MAKEFOURCC('U', 'V', 'E', 'R');

static const unsigned int DDSD_CAPS			= 0x00000001U;
static const unsigned int DDSD_PIXELFORMAT	= 0x00001000U;
static const unsigned int DDSD_WIDTH		= 0x00000004U;
static const unsigned int DDSD_HEIGHT		= 0x00000002U;
static const unsigned int DDSD_PITCH		= 0x00000008U;
static const unsigned int DDSD_MIPMAPCOUNT	= 0x00020000U;
static const unsigned int DDSD_LINEARSIZE	= 0x00080000U;
static const unsigned int DDSD_DEPTH		= 0x00800000U;

static const unsigned int DDSCAPS_COMPLEX	= 0x00000008U;
static const unsigned int DDSCAPS_TEXTURE	= 0x00001000U;
static const unsigned int DDSCAPS_MIPMAP	= 0x00400000U;

static const unsigned int DDSCAPS2_VOLUME	= 0x00200000U;
static const unsigned int DDSCAPS2_CUBEMAP	= 0x00000200U;

// DDPIXELFORMAT (Direct3D 9)
// Surface pixel format.
struct DDSPixelFormat
{
	unsigned int size;
	unsigned int flags;
	unsigned int fourcc;
	unsigned int bitCount;
	unsigned int rmask;
	unsigned int gmask;
	unsigned int bmask;
	unsigned int amask;
};

// 
struct DDSHeader10
{
	unsigned int dxgiFormat;
	unsigned int resourceDimension;
	unsigned int miscFlag;
	unsigned int arraySize;
	unsigned int reserved;
};

//
class DDSHeader
{
public:
	DDSHeader()
	{
		fourcc = 	FOURCC_DDS;
		size = 124;
		flags = (DDSD_CAPS | DDSD_PIXELFORMAT);
		height = 0;
		width = 0;
		pitch = 0;
		depth = 0;
		mipmapCount = 0;
		::ZeroMemory(reserved, sizeof(reserved));

		pf.size = 32;
		pf.flags = 0;
		pf.fourcc = 0;
		pf.bitCount = 0;
		pf.rmask = 0;
		pf.gmask = 0;
		pf.bmask = 0;
		pf.amask = 0;

		caps1 = DDSCAPS_TEXTURE;
		caps2 = 0;
		caps3 = 0;
		caps4 = 0;
		reserved2 = 0;
	};

	void SetWidth(unsigned int width)
	{
		this->flags |= DDSD_WIDTH;
		this->width = width;
	}

	void SetHeight(unsigned int height)
	{
		this->flags |= DDSD_HEIGHT;
		this->height = height;
	}

	void SetDepth(unsigned int depth)
	{
		this->flags |= DDSD_DEPTH;
		this->depth = depth;
	}

	void SetMipmapCount(unsigned int mipmapCount)
	{
		if (mipmapCount == 0 || mipmapCount == 1)
		{
			this->flags &= ~DDSD_MIPMAPCOUNT;
			this->mipmapCount = 1;

			if (this->caps2 == 0)
			{
				this->caps1 = DDSCAPS_TEXTURE;
			}
			else
			{
				this->caps1 = DDSCAPS_TEXTURE | DDSCAPS_COMPLEX;
			}
		}
		else
		{
			this->flags |= DDSD_MIPMAPCOUNT;
			this->mipmapCount = mipmapCount;
			this->caps1 |= DDSCAPS_COMPLEX | DDSCAPS_MIPMAP;
		}
	}

	void SetTexture2D()
	{

	}

	void SetTexture3D()
	{

	}

	void SetTextureCube()
	{

	}

	void SetLinearSize(unsigned int linearSize)
	{
		this->flags &= ~DDSD_PITCH;
		this->flags |= DDSD_LINEARSIZE;
		this->pitch = linearSize;
	}

	void SetPitch(unsigned int pitch)
	{
		this->flags &= ~DDSD_LINEARSIZE;
		this->flags |= DDSD_PITCH;
		this->pitch = pitch;
	}

	void SetFourCC(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3)
	{
		this->pf.flags = 0x00000004U;
		this->pf.fourcc = MAKEFOURCC(c0, c1, c2, c3);
		this->pf.bitCount = 0;
		this->pf.rmask = 0;
		this->pf.gmask = 0;
		this->pf.bmask = 0;
		this->pf.amask = 0;
	}

	void SetFormatCode(unsigned int code)
	{
		this->pf.flags = 0x00000004U;
		this->pf.fourcc = code;
		this->pf.bitCount = 0;
		this->pf.rmask = 0;
		this->pf.gmask = 0;
		this->pf.bmask = 0;
		this->pf.amask = 0;
	}

	void SetSwizzleCode(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3)
	{

	}

	void SetPixelFormat(unsigned int bitCount, unsigned int rmask, unsigned int gmask, unsigned int bmask, unsigned int amask)
	{
		this->pf.flags = 0x00000004U;
		if (amask != 0)
		{
			this->pf.flags |= 0x00000001U;
		}

		if (bitCount == 0)
		{
			unsigned int total = rmask | gmask | bmask | amask;
			while (total !=0)
			{
				bitCount++;
				total >>= 1;
			}
		}

		if (bitCount < 8)
		{
			bitCount = 8;
		}
		else if (bitCount < 16)
		{
			bitCount = 16;
		}
		else if (bitCount < 24)
		{
			bitCount = 24;
		}
		else
		{
			bitCount = 32;
		}

		this->pf.fourcc = 0;
		this->pf.bitCount = bitCount;
		this->pf.rmask = rmask;
		this->pf.gmask = gmask;
		this->pf.bmask = bmask;
		this->pf.amask = amask;
	}

	void SetDX10Format(unsigned int format)
	{
		this->pf.fourcc = FOURCC_DX10;
		this->header10.dxgiFormat = format;
	}

	void SetNormalFlag(bool b)
	{

	}

	void SetAlphaFlag(bool b)
	{

	}

	void SetUserVersion(unsigned int version)
	{

	}

public:
	unsigned int fourcc;
	unsigned int size;
	unsigned int flags;
	unsigned int height;
	unsigned int width;
	unsigned int pitch;
	unsigned int depth;
	unsigned int mipmapCount;
	unsigned int reserved[11];
	DDSPixelFormat pf;
	unsigned int caps1;
	unsigned int caps2;
	unsigned int caps3;
	unsigned int caps4;
	unsigned int reserved2;
	DDSHeader10 header10;
};
//-------------------------------------------------------------------------------------------------
// 计算D3D纹理每像素尺寸(考虑改进为查找表)
//-------------------------------------------------------------------------------------------------
DWORD SDBitsPerPixel(D3DFORMAT d3dFmt);
//-------------------------------------------------------------------------------------------------
// 计算D3D纹理大小
//-------------------------------------------------------------------------------------------------
DWORD SDCalcD3DTextureSize(IDirect3DTexture9* d3dTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif