//*************************************************************************************************
// ����:		������
//---------------------------------------------------------
// ����:		
// ����:		2013-02-02
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_STREAM_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_STREAM_H__
#include <NiSearchPath.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// sdResourceStream
//-------------------------------------------------------------------------------------------------
class sdResourceStream : public NiStream
{
public:
	sdResourceStream();
};
//-------------------------------------------------------------------------------------------------
// sdResourceStream
//-------------------------------------------------------------------------------------------------
class sdResourceSearchPath : public NiSearchPath
{
public:
	sdResourceSearchPath();

	// �麯���̳�
	virtual bool GetNextSearchPath(char* pcPath, unsigned int uiStringLen);

protected:
	uint m_uiBaseNextPath;	///< ��������m_uiNextPath
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif
