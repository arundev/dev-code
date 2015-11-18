#include "sdEnginePCH.h"
#include "sdQuadMesh.h"
#include "sdTerrain.h"

// GPU顶点后处理缓存尺寸(Post T&L Cache, 保存处理过的顶点)
#define GPU_POST_VERTEX_CACHE_SIZE 24

using namespace Base::Math;
//-------------------------------------------------------------------------------------------------
NiDataStreamPtr sdQuadMesh::ms_spIndexStream = NULL;
std::vector<Base::Math::sdVector3us>	sdQuadMesh::ms_kIndexMap;
std::vector<short>	sdQuadMesh::ms_kVertexBuf;
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::StaticInitialize(uint uiTileSize)
{
	// 初始化索引数据流(用于渲染)
	CreateIndexStream(uiTileSize);

	// 初始化索引查找表(用于快速构建顶点数据流)
	CreateIndexMap(uiTileSize);

	// 顶点数据缓存
	ms_kVertexBuf.resize(CalcVertexNum(uiTileSize) * 2, 0);
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::StaticDestroy()
{
	ms_spIndexStream = 0;
}
//-------------------------------------------------------------------------------------------------
uint sdQuadMesh::CalcIndexNum(uint uiSize)
{
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache大小(顶点数)，24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache栏大小(顶点数)，12
	uint uiTileSize = uiSize;						///< Tile尺寸(格数)，64
	uint uiTileAllocSize = uiTileSize + 2 + 1;		///< Tile计算Skirt后的顶点宽度(点数)，67

	// 计算Cache整栏数(格数/格数),6
	uint uiColumeNum = (uiTileAllocSize - 1) / (uiCacheColume - 1);

	// 计算每Cache栏索引数
	uint uiColumeIndexCount = uiCacheColume * (uiTileAllocSize - 1) * 2 + 
		(uiTileAllocSize - 1 - 1) * 2;

	// 计算最后剩余栏索引数(非整栏才计算)
	uint uiLastColumeIndexCount = 0;
	uint uiLastColume = (uiTileAllocSize - 1) - (uiCacheColume - 1) * uiColumeNum;
	if (uiLastColume)
	{
		uiLastColumeIndexCount = (uiLastColumeIndexCount + 1) * (uiTileAllocSize - 1) * 2 + 
			(uiTileAllocSize - 1 -1) * 2;
	}

	// 计算栏之间零面积三角形索引数
	uint uiColumeZeroTriIndexCount = uiColumeNum * 2;
	if (!uiLastColume)
	{
		uiColumeZeroTriIndexCount -= 2;
	}

	//
	uint uiIndexCount = uiColumeIndexCount * uiColumeNum + 
		uiColumeZeroTriIndexCount + 
		uiLastColumeIndexCount;

	return uiIndexCount;
}
//-------------------------------------------------------------------------------------------------
uint sdQuadMesh::CalcVertexNum(uint uiSize)
{
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache大小(顶点数)，24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache栏大小(顶点数)，12
	uint uiTileSize = uiSize;						///< Tile尺寸(格数)，64
	uint uiTileAllocSize = uiTileSize + 2 + 1;		///< Tile计算Skirt后的顶点宽度(点数)，67

	// 计算Cache整栏数(格数/格数)，6
	uint uiColumeNum = (uiTileAllocSize - 1) / (uiCacheColume - 1);

	// 计算Cache栏之间的共享列数(恰好为整数栏时-1,否则不变)，5
	if ((uiTileAllocSize - 1) == (uiCacheColume - 1) * uiColumeNum)
		--uiColumeNum;

	// Cache栏邻接处列顶点数据被重复存储，67 * (67 + 5)
	uint uiVertexNum = uiTileAllocSize * (uiTileAllocSize + uiColumeNum);

	return uiVertexNum;
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::CreateIndexStream(uint uiSize)
{
	//
	// 根据GPU顶点缓存大小和Tile大小构建索引
	//	1.每uiCacheColume个顶点为一栏,栏之间共享一列顶点
	//	2.依次处理每栏,栏之间用零面积三角形衔接,重复存储一列顶点来减少索引复杂性
	//	3.每栏分行处理,行之间用零面积三角形衔接
	//
	// 第一栏起始部分如下
	// (假设Cache为24, uiSize为64)
	//	83	84	
	//	71	72	73	74	75	76	77	78	79	80	81	82
	//	60	61	62	63	64	65	66	67	68	69	70	71
	//	48	49	50	51	52	53	54	55	56	57	58	59
	//	36	37	38	39	40	41	42	43	44	45	46	47
	//	24	25	26	27	28	29	30	31	32	33	34	35
	//	1	3	5	7	9	11	13	15	17	19	21	23
	//	0	2	4	6	8	10	12	14	16	18	20	22
	//
	// @{
	std::vector<unsigned short>	kIndiceVec;
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache大小(点数),24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache栏大小(点数),12
	uint uiTileSize = uiSize;						///< Tile尺寸(格数), 64
	uint uiTileAllocSize =	uiTileSize + 2 + 1;		///< Tile计算Skirt后的顶点宽度(点数), 67
	uint uiVertex = 0;
	for (uint uiVertexXStart = 0; uiVertexXStart < uiTileAllocSize-1; uiVertexXStart += uiCacheColume-1)
	{
		//
		// 每栏起始点分别是:
		// 0-11-22-33-34(总计35列)
		// 0-11-22-33-44-55-66(总计67列,起始列为最后一列的不存在)
		//
		// 每栏12列顶点,栏之间共享一列顶点,各栏分别是:
		// [0,11]	[11,22]	[22,33]	[33,34]
		// [0,11]	[11,22]	[22,33]	[33,44]	[44,55]	[55,66]
		//
		// 计算该栏X轴结束位置和宽度(可能是栏正常结束或Tile结束)
		uint uiVertexXEnd = min(uiVertexXStart + uiCacheColume, uiTileAllocSize);	
		uint uiVertexXDelta = uiVertexXEnd - uiVertexXStart;

		// 当前栏起始索引
		uint uiVertexColumeStart = uiVertex;

		// 处理0-1行构成的三角形
		// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
		for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
		{
			kIndiceVec.push_back(uiVertex++);	// 0行
			kIndiceVec.push_back(uiVertex++);	// 1行
		}

		// 重复压入第1行最后一个(23),得到退化三角形(22,23,23)
		// 重复压入第1行第一个(1),得到退化三角形(23,23,1)
		kIndiceVec.push_back(uiVertex - 1);
		kIndiceVec.push_back(uiVertexColumeStart + 1);

		// 处理1-2行构成的三角形
		// 1 24 3 25 5 26 7 27 9 28 11 29 13 30 15 31 17 32 19 33 21 34 23 35
		for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
		{
			kIndiceVec.push_back(uiVertexColumeStart + (x - uiVertexXStart) * 2 + 1);	// 1行
			kIndiceVec.push_back(uiVertex++);											// 2行
		}

		// 依次处理2-66行构成64行三角形
		for (uint y = 2; y < uiTileAllocSize - 1; ++y)
		{
			// 重复压入上一行最后一个点,得到退化三角形
			// 重复压入上一行第一个点,得到退化三角形
			kIndiceVec.push_back(uiVertex - 1);
			kIndiceVec.push_back(uiVertexColumeStart + y * uiVertexXDelta);

			// 压入一行三角形
			for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
			{
				kIndiceVec.push_back(uiVertex - uiVertexXDelta);	// 起始行
				kIndiceVec.push_back(uiVertex++);					// 结束行
			}
		}

		// 处理到下一栏的接缝
		//
		// 重复压入上一行最后一个点,得到退化三角形
		// 压入下一栏第一个点,得到退化三角形
		kIndiceVec.push_back(uiVertex - 1);
		kIndiceVec.push_back(uiVertex);
	}
	// @}

	// 建立数据流
	// @{
	// 去掉最后一栏结束的两个零面积三角形
	int iIndexSize = kIndiceVec.size() - 2;

	// 
	NiDataStreamFactory* spDataStreamFactory = NiDataStream::GetFactory();
	NiDataStreamFactory::NiDataStreamFactoryCallback spOldCallback = spDataStreamFactory->GetCallback();
	spDataStreamFactory->SetCallback(NULL);

	// 创建并初始化索引数据流
	ms_spIndexStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		iIndexSize,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX,
		&(kIndiceVec[0]));

	//
	spDataStreamFactory->SetCallback(spOldCallback);
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::CreateIndexMap(uint uiSize)
{
	using namespace Base::Math;

	ms_kIndexMap.clear();
	ms_kIndexMap.reserve(CalcVertexNum(uiSize));

	//
	// IndexMap保存于Index一一对应的顶点索引家乡的Vertex坐标,用于根据数字索引反查空间位置
	//
	// NiPoint的xy值:	Tile坐标
	// NiPoint的z值:	0代表非裙子点, 1代表裙子点
	//
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache大小(点数)，24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache栏大小(点数)，12
	uint uiTileSize = uiSize;						///< Tile尺寸(单位)(Power(2,n)), 64
	uint uiTileAllocSize =	uiTileSize + 2 + 1;		///< Tile计算Skirt后的顶点宽度(点数), 67
	int  iStartEdge = -1;							///< 起始边缘,[-1,65],-1
	int	 iEndEdge = (int)uiTileSize + 1;			///< 终止边缘,[-1,65],65
	for (int iVertexXStart = -1; iVertexXStart < iEndEdge; iVertexXStart += (int)uiCacheColume-1)
	{
		// 计算该栏X轴结束位置(可能是栏正常结束或到达Tile结束位置)
		// [-1,65]分解为: [-1,10] [10,21] [21,32] [32,43] [43,54] [54,65]
		int iVertexXEnd = min(iVertexXStart + (int)uiCacheColume - 1, iEndEdge);	

		// 起始行[-1,0]行
		for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
		{
			uint uiX = min((uint)max(x, 0), uiTileSize);			///< 列裙子点钳位,[-1,65]到[0,64]
			uint uiY = 0;											///< 行[-1,0], 均取第0行
			uint uiZ = (x == iStartEdge || x == iEndEdge) ? 1 : 0;	///< 列位于Tile边缘的点
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, 1));		///< 行[-1],裙子外边
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, uiZ));		///< 行[0],两端可能位于裙子外边
		}

		// 中间行[1-64]行
		for (int y = 1; y <= (int)uiTileSize; ++y)
		{
			for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
			{
				uint uiX = min((uint)max(x, 0), uiTileSize);			///< 列裙子点钳位,[-1,65]到[0,64]
				uint uiY = (uint)y;										///< 行[1-64]
				uint uiZ = (x == iStartEdge || x == iEndEdge) ? 1 : 0;	///< 列位于Tile边缘的点
				ms_kIndexMap.push_back(sdVector3us(uiX, uiY, uiZ));
			}
		}

		// 结束行[65]行
		for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
		{
			uint uiX = min((uint)max(x, 0), uiTileSize);			///< 列裙子点钳位,[-1,65]到[0,64]
			uint uiY = 64;											///< 行(65),裙子外边,取第64行
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, 1));		///< 行(65),裙子外边
		}
	}
}
//-------------------------------------------------------------------------------------------------
sdQuadMesh::sdQuadMesh(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth, sdTerrain* pkTerrain)
: m_pkTerrain(pkTerrain)
, m_uiX(uiX)
, m_uiY(uiY)
, m_uiSpacing(uiSpacing)
{
	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 提取顶点数据
	CreateVertexData(uiX, uiY, uiSpacing, uiSize, fSkirtDepth);

	// 创建顶点数据流
	CreateVertexStream();
	
	// 创建Mesh
	// @{
	SetName("TerrainQuadMesh");
	SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRISTRIPS);
	
	// 顶点数据流与索引数据流
	AddStreamRef(ms_spIndexStream, NiCommonSemantics::INDEX(), 0);
	AddStreamRef(m_spVertexStream, NiCommonSemantics::POSITION(), 0);

	// 附属数据
	// a_vTerrainQuadParam
	// [origin_x][origin_y][scale][morph_factor]
	float vExtraData[4] = { 0 };
	vExtraData[0] = (float)m_uiX;
	vExtraData[1] = (float)m_uiY;
	vExtraData[2] = (float)m_uiSpacing;
	vExtraData[3] = 1.0f;

	m_spExtraData = NiNew NiFloatsExtraData(4, vExtraData);
	AddExtraData("a_vTerrainQuadParam", m_spExtraData);

	// 包围盒(不需要计算)


	//
	UpdateProperties();
	UpdateEffects();
	Update(0.0f);

	// 预取
	spRenderer->PrecacheMesh(this);
	// @}
}
//-------------------------------------------------------------------------------------------------
sdQuadMesh::~sdQuadMesh()
{

}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::UpdateGeometry(float& fMinHeight, float& fMaxHeight, float fSkirtDepth)
{
	sdHeightMap* pkHeightMap = m_pkTerrain->GetHeightMap();
	NIASSERT(pkHeightMap);

	// 重新填充Mesh
	// @{
	NiDataStreamElementLock kVertexStreamLock(this,
		NiCommonSemantics::POSITION(), 0,
		NiDataStreamElement::F_UINT8_4,	///<NiDataStreamElement::F_INT16_2
		NiDataStream::LOCK_WRITE);

	NiTStridedRandomAccessIterator<uchar> kVertexStreamItr = kVertexStreamLock.begin<uchar>();

	uint uiSize = m_pkTerrain->GetTileSize();
	uint uiVertexNum = CalcVertexNum(uiSize);
	uchar* pucVertexData = (uchar*)&(kVertexStreamItr[0]);
	for (uint uiVertex = 0; uiVertex < uiVertexNum; ++uiVertex)
	{
		// 查找对应索引位置对应的Tile点坐标
		const sdVector3us& kMapping = ms_kIndexMap[uiVertex];

		// 根据采样间距计算相对位置
		uint uiX = kMapping.m_kX * m_uiSpacing;
		uint uiY = kMapping.m_kY * m_uiSpacing;

		// 查找高度(这里是抓取的顶点高度,没有经过插值)
		float fHeight = pkHeightMap->GetHeight((int)(m_uiX + uiX), (int)(m_uiY + uiY));

		// 更新高度范围
		fMinHeight = min(fMinHeight, fHeight);
		fMaxHeight = max(fMaxHeight, fHeight);

		// 减去裙子高度
		fHeight -= kMapping.m_kZ * fSkirtDepth;

		// 压缩顶点到数据流,以减少DMA传输
		uchar ucX = (uchar)kMapping.m_kX;
		uchar ucY = (uchar)kMapping.m_kY;

		float fHMMinHeight = pkHeightMap->GetMinHeight();
		float fHMMaxHeight = pkHeightMap->GetMaxHeight();
		ushort usCompactZ = static_cast<ushort>((NiClamp(fHeight, fHMMinHeight, fHMMaxHeight) - fHMMinHeight) / (fHMMaxHeight - fHMMinHeight) * 65535.f);

		*pucVertexData = ucX;
		++pucVertexData;

		*pucVertexData = ucY;
		++pucVertexData;

		*pucVertexData = usCompactZ >> 8;
		++pucVertexData;

		*pucVertexData = usCompactZ & 0xff;
		++pucVertexData;
	}

	kVertexStreamLock.Unlock();
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::CreateVertexData(uint uiX, uint uiY, uint uiSpacing, uint uiSize, float fSkirtDepth)
{
	sdHeightMap* pkHeightMap = m_pkTerrain->GetHeightMap();
	NIASSERT(pkHeightMap);

	// 初始化顶点数组
	// @{
	uint uiVertexNum = CalcVertexNum(uiSize);
	::ZeroMemory(&(ms_kVertexBuf[0]), uiVertexNum * 2);

	uchar* pucVertexData = (uchar*)&(ms_kVertexBuf[0]);
	for (uint uiVertex = 0; uiVertex < uiVertexNum; ++uiVertex)
	{
		// 查找对应索引位置对应的Tile点坐标
		const sdVector3us& kMapping = ms_kIndexMap[uiVertex];
		
		// 根据采样间距计算相对位置
		uint uiX = kMapping.m_kX * m_uiSpacing;
		uint uiY = kMapping.m_kY * m_uiSpacing;

		// 查找高度(这里是抓取的顶点高度,没有经过插值)
		float fHeight = pkHeightMap->GetHeight((int)(m_uiX + uiX), (int)(m_uiY + uiY));

		// 减去裙子高度
		fHeight -= kMapping.m_kZ * fSkirtDepth;

		// 压缩顶点到数据流,以减少DMA传输
		// **************************************************************************
		// 方案一:
		//
		//	原理:
		//		1.XY打包并存储到到一个int16
		//		2.Z以0.05f的精度压缩并存储到到int16的范围内
		//
		//  实现:
		//		short* psVertexData = &(ms_kVertexBuf[0]);
		//		*psVertexData = (short)(ucX * 129 + ucY);				++psVertexData;
		//		*psVertexData = (short)floor(fHeight * 20.0f + 0.5f);	++psVertexData;
		//
		//	疑问:
		//		1.不明白为什么是乘以129,而不是128(这里是带符号数,所以不是256)
		//		2.高度值乘以20四舍五入,从而最小分辨率是0.05
		// **************************************************************************
		// 方案二:
		//
		//	原理:
		//		1.X存储到uint8
		//		2.Y存储到uint8
		//		2.Z以0.05f的精度压缩,并调整到uint16的数值范围内,分别将高8位和低8位存储到两个uint8
		//
		//  实现:
		//		uchar* pucVertexData = (uchar*)&(ms_kVertexBuf[0]);
		//		ushort usCompactedZ = (ushort)(floor(fHeight * 20.f) + 32768);
		//		*psVertexData = ucX;					++psVertexData;	
		//		*psVertexData = ucY;					++psVertexData;
		//		*psVertexData = usCompactedZ >> 8;		++psVertexData;
		//		*psVertexData = usCompactedZ & 0xff;	++psVertexData;
		// **************************************************************************
		// 方案三:
		//
		//	原理:
		//		1.X存储到uint8
		//		2.Y存储到uint8
		//		2.Z在MinZ和MaxZ之间压缩到uint16的数值范围内([0,65536]),分别将高8位和低8位存储到两个uint8
		//
		//  实现:
		//		uchar* pucVertexData = (uchar*)&(ms_kVertexBuf[0]);
		//		ushort usCompactedZ = (ushort)((fHeight - fMinHeight) / (fMaxHeight - fMinHeight) * 65535.f));
		//		*psVertexData = ucX;					++psVertexData;	
		//		*psVertexData = ucY;					++psVertexData;
		//		*psVertexData = usCompactedZ >> 8;		++psVertexData;
		//		*psVertexData = usCompactedZ & 0xff;	++psVertexData;
		// **************************************************************************
		uchar ucX = (uchar)kMapping.m_kX;
		uchar ucY = (uchar)kMapping.m_kY;
		
		float fMinHeight = pkHeightMap->GetMinHeight();
		float fMaxHeight = pkHeightMap->GetMaxHeight();
		ushort usCompactZ = static_cast<ushort>((NiClamp(fHeight, fMinHeight, fMaxHeight) - fMinHeight) / (fMaxHeight - fMinHeight) * 65535.f);

		*pucVertexData = ucX;
		++pucVertexData;

		*pucVertexData = ucY;
		++pucVertexData;

		*pucVertexData = usCompactZ >> 8;
		++pucVertexData;

		*pucVertexData = usCompactZ & 0xff;
		++pucVertexData;
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::CreateVertexStream()
{
	// 
	// 方案一：F_INT16_2,只有两个分量,Debug版本会被报警告.
	// 方案二：F_INT8_4,
	//
	// 取消数据流调函数
	NiDataStreamFactory* spDataStreamFactory = NiDataStream::GetFactory();
	NiDataStreamFactory::NiDataStreamFactoryCallback spOldCallback = spDataStreamFactory->GetCallback();
	spDataStreamFactory->SetCallback(NULL); 

	// 顶点访问权限
	NiUInt8 uiAccessMask = NiDataStream::ACCESS_GPU_READ;
	if (m_pkTerrain->GetEnableEditHeight())
		uiAccessMask |= NiDataStream::ACCESS_CPU_WRITE_MUTABLE;
	else
		uiAccessMask |= NiDataStream::ACCESS_CPU_WRITE_STATIC;

	// 构建顶点数据流
	m_spVertexStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT8_4,	///<NiDataStreamElement::F_INT16_2
		ms_kVertexBuf.size() / 2,
		uiAccessMask,
		NiDataStream::USAGE_VERTEX,
		&(ms_kVertexBuf[0]));
	NIASSERT(m_spVertexStream);

	// 重置数据流回调函数
	spDataStreamFactory->SetCallback(spOldCallback);
}
//-------------------------------------------------------------------------------------------------