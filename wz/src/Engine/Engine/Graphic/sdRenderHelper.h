//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDERSYSTEM_UTIL_H__
#define _SD_ENGINE_RENDERSYSTEM_UTIL_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdRenderHelper : public NiRefObject
{
public:
	static void DisableZWrite(NiAVObject* pkAVObject);
	static void DisableZTest(NiAVObject* pkAVObject);
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif