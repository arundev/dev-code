//*************************************************************************************************
// 内容:	文件系统基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-14
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE_SYSTEM_INTERFACE_H__
#define _SD_ENGINE_FILE_SYSTEM_INTERFACE_H__

#include <sdSingleton.h>
#include "IDataStream.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// 文件系统接口，文件系统对外提供两个功能：
//	1.路径解析
//	2.文件读写
//-------------------------------------------------------------------------------------------------
class IFileSystem : public Base::sdTSingleton<IFileSystem>
{
public:
	virtual ~IFileSystem() {};

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;

	bool IsInitialized() { return m_bInitialized;}

	// @breif 
	// @param[in] 
	// @return	

	/** \brief 从文件名获取数据流对象，
	*
	*	@remark	使用完之后要Close
	*	@param[in] szFileName	文件路径
	*	@return					数据流
	*	@see	IFileSystem
	*	@note
	*/
	virtual IDataStream* GetDataStream(const char* szFileName) = 0;


	// 获取文件系统接口
	static IFileSystem* GetFileSystem(){ return (IFileSystem*)InstancePtr();}

protected:
	bool m_bInitialized;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif