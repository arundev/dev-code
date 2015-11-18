#include "sdEnginePCH.h"
#include "sdQuadMesh.h"
#include "sdTerrain.h"

// GPU���������ߴ�(Post T&L Cache, ���洦����Ķ���)
#define GPU_POST_VERTEX_CACHE_SIZE 24

using namespace Base::Math;
//-------------------------------------------------------------------------------------------------
NiDataStreamPtr sdQuadMesh::ms_spIndexStream = NULL;
std::vector<Base::Math::sdVector3us>	sdQuadMesh::ms_kIndexMap;
std::vector<short>	sdQuadMesh::ms_kVertexBuf;
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::StaticInitialize(uint uiTileSize)
{
	// ��ʼ������������(������Ⱦ)
	CreateIndexStream(uiTileSize);

	// ��ʼ���������ұ�(���ڿ��ٹ�������������)
	CreateIndexMap(uiTileSize);

	// �������ݻ���
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
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache��С(������)��24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache����С(������)��12
	uint uiTileSize = uiSize;						///< Tile�ߴ�(����)��64
	uint uiTileAllocSize = uiTileSize + 2 + 1;		///< Tile����Skirt��Ķ�����(����)��67

	// ����Cache������(����/����),6
	uint uiColumeNum = (uiTileAllocSize - 1) / (uiCacheColume - 1);

	// ����ÿCache��������
	uint uiColumeIndexCount = uiCacheColume * (uiTileAllocSize - 1) * 2 + 
		(uiTileAllocSize - 1 - 1) * 2;

	// �������ʣ����������(�������ż���)
	uint uiLastColumeIndexCount = 0;
	uint uiLastColume = (uiTileAllocSize - 1) - (uiCacheColume - 1) * uiColumeNum;
	if (uiLastColume)
	{
		uiLastColumeIndexCount = (uiLastColumeIndexCount + 1) * (uiTileAllocSize - 1) * 2 + 
			(uiTileAllocSize - 1 -1) * 2;
	}

	// ������֮�������������������
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
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache��С(������)��24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache����С(������)��12
	uint uiTileSize = uiSize;						///< Tile�ߴ�(����)��64
	uint uiTileAllocSize = uiTileSize + 2 + 1;		///< Tile����Skirt��Ķ�����(����)��67

	// ����Cache������(����/����)��6
	uint uiColumeNum = (uiTileAllocSize - 1) / (uiCacheColume - 1);

	// ����Cache��֮��Ĺ�������(ǡ��Ϊ������ʱ-1,���򲻱�)��5
	if ((uiTileAllocSize - 1) == (uiCacheColume - 1) * uiColumeNum)
		--uiColumeNum;

	// Cache���ڽӴ��ж������ݱ��ظ��洢��67 * (67 + 5)
	uint uiVertexNum = uiTileAllocSize * (uiTileAllocSize + uiColumeNum);

	return uiVertexNum;
}
//-------------------------------------------------------------------------------------------------
void sdQuadMesh::CreateIndexStream(uint uiSize)
{
	//
	// ����GPU���㻺���С��Tile��С��������
	//	1.ÿuiCacheColume������Ϊһ��,��֮�乲��һ�ж���
	//	2.���δ���ÿ��,��֮����������������ν�,�ظ��洢һ�ж�������������������
	//	3.ÿ�����д���,��֮����������������ν�
	//
	// ��һ����ʼ��������
	// (����CacheΪ24, uiSizeΪ64)
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
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache��С(����),24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache����С(����),12
	uint uiTileSize = uiSize;						///< Tile�ߴ�(����), 64
	uint uiTileAllocSize =	uiTileSize + 2 + 1;		///< Tile����Skirt��Ķ�����(����), 67
	uint uiVertex = 0;
	for (uint uiVertexXStart = 0; uiVertexXStart < uiTileAllocSize-1; uiVertexXStart += uiCacheColume-1)
	{
		//
		// ÿ����ʼ��ֱ���:
		// 0-11-22-33-34(�ܼ�35��)
		// 0-11-22-33-44-55-66(�ܼ�67��,��ʼ��Ϊ���һ�еĲ�����)
		//
		// ÿ��12�ж���,��֮�乲��һ�ж���,�����ֱ���:
		// [0,11]	[11,22]	[22,33]	[33,34]
		// [0,11]	[11,22]	[22,33]	[33,44]	[44,55]	[55,66]
		//
		// �������X�����λ�úͿ��(������������������Tile����)
		uint uiVertexXEnd = min(uiVertexXStart + uiCacheColume, uiTileAllocSize);	
		uint uiVertexXDelta = uiVertexXEnd - uiVertexXStart;

		// ��ǰ����ʼ����
		uint uiVertexColumeStart = uiVertex;

		// ����0-1�й��ɵ�������
		// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
		for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
		{
			kIndiceVec.push_back(uiVertex++);	// 0��
			kIndiceVec.push_back(uiVertex++);	// 1��
		}

		// �ظ�ѹ���1�����һ��(23),�õ��˻�������(22,23,23)
		// �ظ�ѹ���1�е�һ��(1),�õ��˻�������(23,23,1)
		kIndiceVec.push_back(uiVertex - 1);
		kIndiceVec.push_back(uiVertexColumeStart + 1);

		// ����1-2�й��ɵ�������
		// 1 24 3 25 5 26 7 27 9 28 11 29 13 30 15 31 17 32 19 33 21 34 23 35
		for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
		{
			kIndiceVec.push_back(uiVertexColumeStart + (x - uiVertexXStart) * 2 + 1);	// 1��
			kIndiceVec.push_back(uiVertex++);											// 2��
		}

		// ���δ���2-66�й���64��������
		for (uint y = 2; y < uiTileAllocSize - 1; ++y)
		{
			// �ظ�ѹ����һ�����һ����,�õ��˻�������
			// �ظ�ѹ����һ�е�һ����,�õ��˻�������
			kIndiceVec.push_back(uiVertex - 1);
			kIndiceVec.push_back(uiVertexColumeStart + y * uiVertexXDelta);

			// ѹ��һ��������
			for (uint x = uiVertexXStart; x < uiVertexXEnd; ++x)
			{
				kIndiceVec.push_back(uiVertex - uiVertexXDelta);	// ��ʼ��
				kIndiceVec.push_back(uiVertex++);					// ������
			}
		}

		// ������һ���Ľӷ�
		//
		// �ظ�ѹ����һ�����һ����,�õ��˻�������
		// ѹ����һ����һ����,�õ��˻�������
		kIndiceVec.push_back(uiVertex - 1);
		kIndiceVec.push_back(uiVertex);
	}
	// @}

	// ����������
	// @{
	// ȥ�����һ�����������������������
	int iIndexSize = kIndiceVec.size() - 2;

	// 
	NiDataStreamFactory* spDataStreamFactory = NiDataStream::GetFactory();
	NiDataStreamFactory::NiDataStreamFactoryCallback spOldCallback = spDataStreamFactory->GetCallback();
	spDataStreamFactory->SetCallback(NULL);

	// ��������ʼ������������
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
	// IndexMap������Indexһһ��Ӧ�Ķ������������Vertex����,���ڸ���������������ռ�λ��
	//
	// NiPoint��xyֵ:	Tile����
	// NiPoint��zֵ:	0�����ȹ�ӵ�, 1����ȹ�ӵ�
	//
	uint uiCacheSize = GPU_POST_VERTEX_CACHE_SIZE;	///< Cache��С(����)��24
	uint uiCacheColume = uiCacheSize / 2;			///< Cache����С(����)��12
	uint uiTileSize = uiSize;						///< Tile�ߴ�(��λ)(Power(2,n)), 64
	uint uiTileAllocSize =	uiTileSize + 2 + 1;		///< Tile����Skirt��Ķ�����(����), 67
	int  iStartEdge = -1;							///< ��ʼ��Ե,[-1,65],-1
	int	 iEndEdge = (int)uiTileSize + 1;			///< ��ֹ��Ե,[-1,65],65
	for (int iVertexXStart = -1; iVertexXStart < iEndEdge; iVertexXStart += (int)uiCacheColume-1)
	{
		// �������X�����λ��(�����������������򵽴�Tile����λ��)
		// [-1,65]�ֽ�Ϊ: [-1,10] [10,21] [21,32] [32,43] [43,54] [54,65]
		int iVertexXEnd = min(iVertexXStart + (int)uiCacheColume - 1, iEndEdge);	

		// ��ʼ��[-1,0]��
		for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
		{
			uint uiX = min((uint)max(x, 0), uiTileSize);			///< ��ȹ�ӵ�ǯλ,[-1,65]��[0,64]
			uint uiY = 0;											///< ��[-1,0], ��ȡ��0��
			uint uiZ = (x == iStartEdge || x == iEndEdge) ? 1 : 0;	///< ��λ��Tile��Ե�ĵ�
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, 1));		///< ��[-1],ȹ�����
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, uiZ));		///< ��[0],���˿���λ��ȹ�����
		}

		// �м���[1-64]��
		for (int y = 1; y <= (int)uiTileSize; ++y)
		{
			for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
			{
				uint uiX = min((uint)max(x, 0), uiTileSize);			///< ��ȹ�ӵ�ǯλ,[-1,65]��[0,64]
				uint uiY = (uint)y;										///< ��[1-64]
				uint uiZ = (x == iStartEdge || x == iEndEdge) ? 1 : 0;	///< ��λ��Tile��Ե�ĵ�
				ms_kIndexMap.push_back(sdVector3us(uiX, uiY, uiZ));
			}
		}

		// ������[65]��
		for (int x = iVertexXStart; x <= iVertexXEnd; ++x)
		{
			uint uiX = min((uint)max(x, 0), uiTileSize);			///< ��ȹ�ӵ�ǯλ,[-1,65]��[0,64]
			uint uiY = 64;											///< ��(65),ȹ�����,ȡ��64��
			ms_kIndexMap.push_back(sdVector3us(uiX, uiY, 1));		///< ��(65),ȹ�����
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

	// ��ȡ��������
	CreateVertexData(uiX, uiY, uiSpacing, uiSize, fSkirtDepth);

	// ��������������
	CreateVertexStream();
	
	// ����Mesh
	// @{
	SetName("TerrainQuadMesh");
	SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRISTRIPS);
	
	// ����������������������
	AddStreamRef(ms_spIndexStream, NiCommonSemantics::INDEX(), 0);
	AddStreamRef(m_spVertexStream, NiCommonSemantics::POSITION(), 0);

	// ��������
	// a_vTerrainQuadParam
	// [origin_x][origin_y][scale][morph_factor]
	float vExtraData[4] = { 0 };
	vExtraData[0] = (float)m_uiX;
	vExtraData[1] = (float)m_uiY;
	vExtraData[2] = (float)m_uiSpacing;
	vExtraData[3] = 1.0f;

	m_spExtraData = NiNew NiFloatsExtraData(4, vExtraData);
	AddExtraData("a_vTerrainQuadParam", m_spExtraData);

	// ��Χ��(����Ҫ����)


	//
	UpdateProperties();
	UpdateEffects();
	Update(0.0f);

	// Ԥȡ
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

	// �������Mesh
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
		// ���Ҷ�Ӧ����λ�ö�Ӧ��Tile������
		const sdVector3us& kMapping = ms_kIndexMap[uiVertex];

		// ���ݲ������������λ��
		uint uiX = kMapping.m_kX * m_uiSpacing;
		uint uiY = kMapping.m_kY * m_uiSpacing;

		// ���Ҹ߶�(������ץȡ�Ķ���߶�,û�о�����ֵ)
		float fHeight = pkHeightMap->GetHeight((int)(m_uiX + uiX), (int)(m_uiY + uiY));

		// ���¸߶ȷ�Χ
		fMinHeight = min(fMinHeight, fHeight);
		fMaxHeight = max(fMaxHeight, fHeight);

		// ��ȥȹ�Ӹ߶�
		fHeight -= kMapping.m_kZ * fSkirtDepth;

		// ѹ�����㵽������,�Լ���DMA����
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

	// ��ʼ����������
	// @{
	uint uiVertexNum = CalcVertexNum(uiSize);
	::ZeroMemory(&(ms_kVertexBuf[0]), uiVertexNum * 2);

	uchar* pucVertexData = (uchar*)&(ms_kVertexBuf[0]);
	for (uint uiVertex = 0; uiVertex < uiVertexNum; ++uiVertex)
	{
		// ���Ҷ�Ӧ����λ�ö�Ӧ��Tile������
		const sdVector3us& kMapping = ms_kIndexMap[uiVertex];
		
		// ���ݲ������������λ��
		uint uiX = kMapping.m_kX * m_uiSpacing;
		uint uiY = kMapping.m_kY * m_uiSpacing;

		// ���Ҹ߶�(������ץȡ�Ķ���߶�,û�о�����ֵ)
		float fHeight = pkHeightMap->GetHeight((int)(m_uiX + uiX), (int)(m_uiY + uiY));

		// ��ȥȹ�Ӹ߶�
		fHeight -= kMapping.m_kZ * fSkirtDepth;

		// ѹ�����㵽������,�Լ���DMA����
		// **************************************************************************
		// ����һ:
		//
		//	ԭ��:
		//		1.XY������洢����һ��int16
		//		2.Z��0.05f�ľ���ѹ�����洢����int16�ķ�Χ��
		//
		//  ʵ��:
		//		short* psVertexData = &(ms_kVertexBuf[0]);
		//		*psVertexData = (short)(ucX * 129 + ucY);				++psVertexData;
		//		*psVertexData = (short)floor(fHeight * 20.0f + 0.5f);	++psVertexData;
		//
		//	����:
		//		1.������Ϊʲô�ǳ���129,������128(�����Ǵ�������,���Բ���256)
		//		2.�߶�ֵ����20��������,�Ӷ���С�ֱ�����0.05
		// **************************************************************************
		// ������:
		//
		//	ԭ��:
		//		1.X�洢��uint8
		//		2.Y�洢��uint8
		//		2.Z��0.05f�ľ���ѹ��,��������uint16����ֵ��Χ��,�ֱ𽫸�8λ�͵�8λ�洢������uint8
		//
		//  ʵ��:
		//		uchar* pucVertexData = (uchar*)&(ms_kVertexBuf[0]);
		//		ushort usCompactedZ = (ushort)(floor(fHeight * 20.f) + 32768);
		//		*psVertexData = ucX;					++psVertexData;	
		//		*psVertexData = ucY;					++psVertexData;
		//		*psVertexData = usCompactedZ >> 8;		++psVertexData;
		//		*psVertexData = usCompactedZ & 0xff;	++psVertexData;
		// **************************************************************************
		// ������:
		//
		//	ԭ��:
		//		1.X�洢��uint8
		//		2.Y�洢��uint8
		//		2.Z��MinZ��MaxZ֮��ѹ����uint16����ֵ��Χ��([0,65536]),�ֱ𽫸�8λ�͵�8λ�洢������uint8
		//
		//  ʵ��:
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
	// ����һ��F_INT16_2,ֻ����������,Debug�汾�ᱻ������.
	// ��������F_INT8_4,
	//
	// ȡ��������������
	NiDataStreamFactory* spDataStreamFactory = NiDataStream::GetFactory();
	NiDataStreamFactory::NiDataStreamFactoryCallback spOldCallback = spDataStreamFactory->GetCallback();
	spDataStreamFactory->SetCallback(NULL); 

	// �������Ȩ��
	NiUInt8 uiAccessMask = NiDataStream::ACCESS_GPU_READ;
	if (m_pkTerrain->GetEnableEditHeight())
		uiAccessMask |= NiDataStream::ACCESS_CPU_WRITE_MUTABLE;
	else
		uiAccessMask |= NiDataStream::ACCESS_CPU_WRITE_STATIC;

	// ��������������
	m_spVertexStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT8_4,	///<NiDataStreamElement::F_INT16_2
		ms_kVertexBuf.size() / 2,
		uiAccessMask,
		NiDataStream::USAGE_VERTEX,
		&(ms_kVertexBuf[0]));
	NIASSERT(m_spVertexStream);

	// �����������ص�����
	spDataStreamFactory->SetCallback(spOldCallback);
}
//-------------------------------------------------------------------------------------------------