//*************************************************************************************************
// 内容:	PhysX的数据流重载,用于数据流化
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-17
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_STREAM_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_STREAM_H__
#include <NxStream.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
// 文件数据流
//-----------------------------------------------------------------------------
class sdPhysXStream : public NxStream, public NiMemObject
{
public:
	sdPhysXStream(const char* szFileName, bool bLoad);
	virtual ~sdPhysXStream();

	// 虚函数继承
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
	FILE* m_pfFile;	///< C文件对象
};
//-----------------------------------------------------------------------------
// 写入内存数据流
//-----------------------------------------------------------------------------
class sdPhysXMemoryWriteStream : public NxStream, public NiMemObject
{
public:
	sdPhysXMemoryWriteStream();
	virtual ~sdPhysXMemoryWriteStream();

	// 虚函数继承
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

	// 获取当前位置
	NxU32 GetCurrentPosition() const { return m_nxCurSize;}

protected:
	NxU8*	m_pnxData;		///< 内存数据流
	NxU32	m_nxMaxSize;	///< 数据流最大尺寸
	NxU32	m_nxCurSize;	///< 数据流当前指针
};
//-----------------------------------------------------------------------------
// 读取内存数据流
//-----------------------------------------------------------------------------
class sdPhysXMemoryReadStream : public NxStream, public NiMemObject
{
public:
	sdPhysXMemoryReadStream(const NxU8* data);
	virtual ~sdPhysXMemoryReadStream();

	// 虚函数继承
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
	mutable const NxU8*	m_pnxData;	///< 源数据流
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif