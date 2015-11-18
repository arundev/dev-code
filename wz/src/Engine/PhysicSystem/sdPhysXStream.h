//*************************************************************************************************
// ����:	PhysX������������,������������
//---------------------------------------------------------
// ����:		
// ����:		2012-08-17
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_STREAM_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_STREAM_H__
#include <NxStream.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
// �ļ�������
//-----------------------------------------------------------------------------
class sdPhysXStream : public NxStream, public NiMemObject
{
public:
	sdPhysXStream(const char* szFileName, bool bLoad);
	virtual ~sdPhysXStream();

	// �麯���̳�
	virtual	NxU8 readByte() const;
	virtual	NxU16 readWord() const;
	virtual	NxU32 readDword() const;
	virtual	NxF32 readFloat() const;
	virtual	NxF64 readDouble() const;
	virtual	void readBuffer(void* buffer, NxU32 size) const;

	virtual NxStream& storeByte(NxU8 b);
	virtual NxStream& storeWord(NxU16 w);
	virtual NxStream& storeDword(NxU32 d);
	virtual NxStream& storeFloat(NxF32 f);
	virtual NxStream& storeDouble(NxF64 f);
	virtual NxStream& storeBuffer(const void* buffer, NxU32 size);

protected:
	FILE* m_pfFile;	///< C�ļ�����
};
//-----------------------------------------------------------------------------
// д���ڴ�������
//-----------------------------------------------------------------------------
class sdPhysXMemoryWriteStream : public NxStream, public NiMemObject
{
public:
	sdPhysXMemoryWriteStream();
	virtual ~sdPhysXMemoryWriteStream();

	// �麯���̳�
	virtual	NxU8 readByte() const { NX_ASSERT(0); return 0;}
	virtual	NxU16 readWord() const{ NX_ASSERT(0); return 0;}
	virtual	NxU32 readDword() const{ NX_ASSERT(0); return 0;}
	virtual	NxF32 readFloat() const{ NX_ASSERT(0); return 0;}
	virtual	NxF64 readDouble() const{ NX_ASSERT(0); return 0;}
	virtual	void readBuffer(void* buffer, NxU32 size) const{ NX_ASSERT(0);}

	virtual NxStream& storeByte(NxU8 b);
	virtual NxStream& storeWord(NxU16 w);
	virtual NxStream& storeDword(NxU32 d);
	virtual NxStream& storeFloat(NxF32 f);
	virtual NxStream& storeDouble(NxF64 f);
	virtual NxStream& storeBuffer(const void* buffer, NxU32 size);

	// ��ȡ��ǰλ��
	NxU32 GetCurrentPosition() const { return m_nxCurSize;}

protected:
	NxU8*	m_pnxData;		///< �ڴ�������
	NxU32	m_nxMaxSize;	///< ���������ߴ�
	NxU32	m_nxCurSize;	///< ��������ǰָ��
};
//-----------------------------------------------------------------------------
// ��ȡ�ڴ�������
//-----------------------------------------------------------------------------
class sdPhysXMemoryReadStream : public NxStream, public NiMemObject
{
public:
	sdPhysXMemoryReadStream(const NxU8* data);
	virtual ~sdPhysXMemoryReadStream();

	// �麯���̳�
	virtual	NxU8 readByte() const;
	virtual	NxU16 readWord() const;
	virtual	NxU32 readDword() const;
	virtual	NxF32 readFloat() const;
	virtual	NxF64 readDouble() const;
	virtual	void readBuffer(void* buffer, NxU32 size) const;

	virtual NxStream& storeByte(NxU8 b) { NX_ASSERT(0); return (*this);}
	virtual NxStream& storeWord(NxU16 w){ NX_ASSERT(0); return (*this);}
	virtual NxStream& storeDword(NxU32 d){ NX_ASSERT(0); return (*this);}
	virtual NxStream& storeFloat(NxF32 f){ NX_ASSERT(0); return (*this);}
	virtual NxStream& storeDouble(NxF64 f){ NX_ASSERT(0); return (*this);}
	virtual NxStream& storeBuffer(const void* buffer, NxU32 size){ NX_ASSERT(0); return (*this);}

protected:
	mutable const NxU8*	m_pnxData;	///< Դ������
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif