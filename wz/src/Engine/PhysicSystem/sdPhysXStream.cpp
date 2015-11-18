#include "sdPhysicsSystemPCH.h"
#include "sdPhysXStream.h"

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
sdPhysXStream::sdPhysXStream(const char* szFileName, bool bLoad)
{
	NX_ASSERT(szFileName);

	fopen_s(&m_pfFile, szFileName, bLoad ? "rb": "wb");
}
//-------------------------------------------------------------------------------------------------
sdPhysXStream::~sdPhysXStream()
{
	if (m_pfFile)
		fclose(m_pfFile);
}
//-------------------------------------------------------------------------------------------------
NxU8 sdPhysXStream::readByte() const
{
	NxU8 b;
	size_t r = fread(&b, sizeof(NxU8), 1, m_pfFile);
	NX_ASSERT(r);
	return b;
}
//-------------------------------------------------------------------------------------------------
NxU16 sdPhysXStream::readWord() const
{
	NxU16 w;
	size_t r = fread(&w, sizeof(NxU16), 1, m_pfFile);
	NX_ASSERT(r);
	return w;
}
//-------------------------------------------------------------------------------------------------
NxU32 sdPhysXStream::readDword() const
{
	NxU32 d;
	size_t r = fread(&d, sizeof(NxU32), 1, m_pfFile);
	NX_ASSERT(r);
	return d;
}
//-------------------------------------------------------------------------------------------------
NxF32 sdPhysXStream::readFloat() const
{
	NxF32 f;
	size_t r = fread(&f, sizeof(NxF32), 1, m_pfFile);
	NX_ASSERT(r);
	return f;
}
//-------------------------------------------------------------------------------------------------
NxF64 sdPhysXStream::readDouble() const
{
	NxF64 f;
	size_t r = fread(&f, sizeof(NxF64), 1, m_pfFile);
	NX_ASSERT(r);
	return f;
}
//-------------------------------------------------------------------------------------------------
void sdPhysXStream::readBuffer(void* buffer, NxU32 size) const
{
	size_t r = fread(&buffer, size, 1, m_pfFile);
	NX_ASSERT(r);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeByte(NxU8 b)
{
	size_t r = fread(&b, sizeof(NxU8), 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeWord(NxU16 w)
{
	size_t r = fread(&w, sizeof(NxU16), 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeDword(NxU32 d)
{
	size_t r = fread(&d, sizeof(NxU32), 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeFloat(NxF32 f)
{
	size_t r = fread(&f, sizeof(NxF32), 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeDouble(NxF64 f)
{
	size_t r = fread(&f, sizeof(NxF64), 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXStream::storeBuffer(const void* buffer, NxU32 size)
{
	size_t r = fread(&buffer, size, 1, m_pfFile);
	NX_ASSERT(r);
	return (*this);
}
//-------------------------------------------------------------------------------------------------
sdPhysXMemoryWriteStream::sdPhysXMemoryWriteStream() : m_pnxData(0), m_nxMaxSize(0), m_nxCurSize(0)
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXMemoryWriteStream::~sdPhysXMemoryWriteStream()
{
	NX_DELETE_ARRAY(m_pnxData);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeFloat(NxF32 f)
{
	storeBuffer(&f, sizeof(NxF32));
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return (*this);
}
//-------------------------------------------------------------------------------------------------
NxStream& sdPhysXMemoryWriteStream::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 nxExpectedSize = m_nxCurSize + size;
	if (nxExpectedSize > m_nxMaxSize)
	{
		m_nxMaxSize = nxExpectedSize + 4096;	///< 尺寸线性增长
		
		NxU8* pnxNewData = new NxU8[m_nxMaxSize];
		NX_ASSERT(pnxNewData);

		if (pnxNewData)
		{
			memcpy(pnxNewData, m_pnxData, m_nxCurSize);
			delete[] m_pnxData;
		}

		m_pnxData = pnxNewData;
	}

	memcpy(m_pnxData + m_nxCurSize, buffer, size);
	m_nxCurSize += size;

	return (*this);
}
//-------------------------------------------------------------------------------------------------
sdPhysXMemoryReadStream::sdPhysXMemoryReadStream(const NxU8* data) : m_pnxData(data)
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXMemoryReadStream::~sdPhysXMemoryReadStream()
{
	///< We don't own the data, so no need todelete
}
//-------------------------------------------------------------------------------------------------
NxU8 sdPhysXMemoryReadStream::readByte() const
{
	NxU8 b;
	memcpy(&b, m_pnxData, sizeof(NxU8));
	m_pnxData += sizeof(NxU8);
	return b;
}
//-------------------------------------------------------------------------------------------------
NxU16 sdPhysXMemoryReadStream::readWord() const
{
	NxU16 w;
	memcpy(&w, m_pnxData, sizeof(NxU16));
	m_pnxData += sizeof(NxU16);
	return w;
}
//-------------------------------------------------------------------------------------------------
NxU32 sdPhysXMemoryReadStream::readDword() const
{
	NxU32 d;
	memcpy(&d, m_pnxData, sizeof(NxU32));
	m_pnxData += sizeof(NxU32);
	return d;
}
//-------------------------------------------------------------------------------------------------
NxF32 sdPhysXMemoryReadStream::readFloat() const
{
	NxF32 f;
	memcpy(&f, m_pnxData, sizeof(NxF32));
	m_pnxData += sizeof(NxF32);
	return f;
}
//-------------------------------------------------------------------------------------------------
NxF64 sdPhysXMemoryReadStream::readDouble() const
{
	NxF64 f;
	memcpy(&f, m_pnxData, sizeof(NxF64));
	m_pnxData += sizeof(NxF64);
	return f;
}
//-------------------------------------------------------------------------------------------------
void sdPhysXMemoryReadStream::readBuffer(void* buffer, NxU32 size) const
{
	memcpy(buffer, m_pnxData, size);
	m_pnxData += size;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM