//*************************************************************************************************
// ����:	�ļ�ϵͳ�����ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2014-01-14
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE_SYSTEM_INTERFACE_H__
#define _SD_ENGINE_FILE_SYSTEM_INTERFACE_H__

#include <sdSingleton.h>
#include "IDataStream.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// �ļ�ϵͳ�ӿڣ��ļ�ϵͳ�����ṩ�������ܣ�
//	1.·������
//	2.�ļ���д
//-------------------------------------------------------------------------------------------------
class IFileSystem : public Base::sdTSingleton<IFileSystem>
{
public:
	virtual ~IFileSystem() {};

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;

	bool IsInitialized() { return m_bInitialized;}

	// @breif 
	// @param[in] 
	// @return	

	/** \brief ���ļ�����ȡ����������
	*
	*	@remark	ʹ����֮��ҪClose
	*	@param[in] szFileName	�ļ�·��
	*	@return					������
	*	@see	IFileSystem
	*	@note
	*/
	virtual IDataStream* GetDataStream(const char* szFileName) = 0;


	// ��ȡ�ļ�ϵͳ�ӿ�
	static IFileSystem* GetFileSystem(){ return (IFileSystem*)InstancePtr();}

protected:
	bool m_bInitialized;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif