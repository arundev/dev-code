//*************************************************************************************************
// ����:	Streaming
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_STREAMING_MACRO_H__
#define _SD_ENGINE_SCENE_STREAMING_MACRO_H__

//-----------------------------------------------------------------------------------
// GB��Streaming��ȱʡʵ��
//-----------------------------------------------------------------------------------
#define sdImplementDefaultStream(classname)					\
	void classname::LoadBinary(NiStream& kStream)			\
	{														\
		__super::LoadBinary(kStream);						\
	}														\
															\
	void classname::LinkObject(NiStream& kStream)			\
	{														\
		__super::LinkObject(kStream);						\
	}														\
															\
	bool classname::RegisterStreamables(NiStream& kStream)	\
	{														\
		return __super::RegisterStreamables(kStream);		\
	}														\
															\
	void classname::SaveBinary(NiStream& kStream)			\
	{														\
		__super::SaveBinary(kStream);						\
	}														\
															\
	bool classname::IsEqual(NiObject* pkObject)				\
	{														\
		return __super::IsEqual(pkObject);					\
	}														\
															\
	NiObject* classname::CreateObject(const char** pcArguments, uint uiArgCount)	\
	{														\
		classname* pkObject = NiNew classname;				\
		NIASSERT(pkObject);									\
		return pkObject;									\
	}														

//-----------------------------------------------------------------------------------
// GB��Streaming�Ŀ�ʵ��
//-----------------------------------------------------------------------------------
#define sdImplementAbstractStream(classname)				\
	void classname::LoadBinary(NiStream& kStream){}			\
	void classname::LinkObject(NiStream& kStream){}			\
	void classname::RegisterStreamables(NiStream& kStream){	return true;}	\
	void classname::SaveBinary(NiStream& kStream){}			\
	bool classname::IsEqual(NiObject* pkObject){ return __super::IsEqual(pkObject);	}	\

#endif