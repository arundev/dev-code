//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-02-20
// ����޸�:	2014-01-07
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_LOD_MANAGER_H__
#define _SD_ENGINE_TEXTURE_LOD_MANAGER_H__
#include "ITextureManager.h"

#define TEX_LOD_LEVEL 4

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
class sdTextureLodManager : public ITextureLodManager
{
public:
	sdTextureLodManager();
	virtual ~sdTextureLodManager();

	// �麯���̳�
	virtual bool Initialize();
	virtual void Destroy();
	virtual bool IsInitialized();

	virtual void Update();

	virtual uchar GetBaseSkipLevel();
	virtual void SetBaseSkipLevel(char iLevel);

	virtual uchar GetSkipLevel();

	virtual void SetEnableDynamicLevel(bool bEnable);
	
protected:
	bool m_bInitialized;	///< �Ƿ񱻳�ʼ��
	bool m_bEnable;			///< �Ƿ�����̬LOD
	char m_cBaseSkipLevel;
	char m_cDynamicLevel;
	uint m_auiLevelArray[TEX_LOD_LEVEL];
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif