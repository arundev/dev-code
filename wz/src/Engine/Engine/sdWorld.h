//*************************************************************************************************
// ����:	����
//---------------------------------------------------------
// ����:		
// ����:		2013-05-13
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_WORLD_H__
#define _SD_ENGINE_SCENE_WORLD_H__
#include <sdSingleton.h>

// �������������
#define MAX_WORLD_NAME_LEN 32

// ���ػص�����(������ʾ���ؽ���,�������ӻش�������Ϣ)
// @param[in]	iMax		������
// @param[in]	iCurrent	��ǰ����
// @param[in]	szMsg		��ǰ��Ϣ
typedef void (*LoadCallback)(int iMax, int iCurrent, const char* szMsg);

class sdMap;
//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
class sdWorld : public NiRefObject
{
public:
	sdWorld(const char* szName, const char* szPath);
	~sdWorld();

	const char* GetName() const { return m_szName;}
	void		SetName(const char* szName) { strncpy(m_szName, szName, MAX_WORLD_NAME_LEN);}

	const char* GetPath() const { return m_szPath;}
	void		SetPath(const char* szPath) { strncpy(m_szPath, szPath, MAX_WORLD_NAME_LEN);}

protected:
	char m_szName[MAX_WORLD_NAME_LEN];	///< ��������
	char m_szPath[MAX_WORLD_NAME_LEN];	///< ������Դ�ļ�������
};
NiSmartPointer(sdWorld);
//-----------------------------------------------------------------------------
// ���������,��Ҫ����·��ת��
//-----------------------------------------------------------------------------
class sdWorldManager : public Base::sdTSingleton<sdWorldManager>
{
public:
	sdWorldManager();
	~sdWorldManager();

	sdWorld*	GetCurrentWorld() { return m_pkWorld;}
	void		SetCurrentWorld(sdWorld* pkWorld) { m_pkWorld = pkWorld;}

	const char* GetGameRootPath() const { return m_szGameRootPath;}

	// ����
	// @{
	//bool	CreateScene(const char* szMapName);
	//void	DestroyScene();
	sdMap*	LoadScene(const char* szMapName, LoadCallback pfnCallback);
	bool	SaveScene(sdMap* pkMap, const char* szMapName, LoadCallback pfnCallback);
	// @}

	//
	// ת��·������Դ·��(��������ͬһ��Ŀ¼)
	//
	// Ŀǰ·�������¼���:
	//	1.����·��(·���к���":\\"��":/")					x:\
	//	2.���·��(��"\\",".\\","..\\","/","./","../"��ʼ)	\relativepath
	//	3.��Դ·��(·���к���":")							worldname:respath
	//	4.����·��											worldname:respath\relativepath
	//
	// ע�⣺
	//	1.����б��ͨ��
	//	2.���浽luaһ����'/'
	//	3.�ַ�����Сд
	//
	bool		Convert2ResPath(const char* szPath, char* szNewPath);

protected:
	bool	IsPhyPath(const char* szPath);
	bool	IsResPath(const char* szPath);
	bool	IsRelPath(const char* szPath);
	bool	GetWorldName(const char* szPath, char* szWorldName);
	bool	GetRelativePath(const char* szPath, char* szRelPath);

protected:
	sdWorldPtr	m_pkWorld;						///< ��ǰ����
	char		m_szGameRootPath[MAX_PATH_LEN];	///< ��Ϸ��Ŀ¼
};
//-----------------------------------------------------------------------------
#endif