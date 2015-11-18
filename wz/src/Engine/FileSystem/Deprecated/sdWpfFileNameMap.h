//*************************************************************************************************
// ����:	�����ļ�ȫ·��ӳ�䵽��Ӧ��wpf�ļ�
//---------------------------------------------------------
// ����:		
// ����:		2013-08-22
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_WPF_FILE_NAME_MAP_H__
#define _SD_ENGINE_FILESYSTEM_WPF_FILE_NAME_MAP_H__

#define MAX_WORLD_NAME_LEN		32
#define MAX_WPF_FILE_NAME_LEN	32
//#define MAX_PATH_LEN			MAX_PATH

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM

class sdWpfLoader;
//-------------------------------------------------------------------------------------------------
// �����ļ�·��ӳ�䵽�����Wpf�ļ�
//-------------------------------------------------------------------------------------------------
class sdWpfFileNameMap
{
public:
	// Wpf�ļ�����
	enum EWpfNameIndex
	{
		E_WPF_MISC,
		E_WPF_SCENE,
		E_WPF_MODEL,
		E_WPF_TEXTURE1,
		E_WPF_TEXTURE2,
		E_WPF_WORLDT,

		NUM_WPFS,
	};

	// ��ѯ����ֵ
	struct stWpfFileNameMapResult
	{
		sdWpfLoader*	pkWpfInterface;						///< Wpf�ļ��򿪽ӿ�
		char			szWorldName[MAX_WORLD_NAME_LEN];	///< ��������
		char			szWpfName[MAX_PATH_LEN];			///< Wpf�ļ�ȫ·��(�����ļ���)
		char			szRelPath[MAX_PATH_LEN];			///< Wpf�ڲ����·��
		EWpfNameIndex	eWpfNameIndex;						///< Wpf�ļ�����
	};

public:
	sdWpfFileNameMap();

	// �����ļ�·����ȡ�����ڵ�Wpf�ļ��ӿ�
	//	@param[in]		szFileName				�ļ�����ȫ·��(���ΪȫСд����б��)
	//	@param[in,out]	kWpfFileNameMapResult	���ҽ��
	bool GetWpfInterface(const char* szFileName, stWpfFileNameMapResult& kWpfFileNameMapResult);

	// 
	void SetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex, sdWpfLoader* pkWpfInterface);
	void RemoveWpfInterface(const char* szWorldName);
	bool ExistWpfNameMap(const char* szWorldName);

protected:
	// ������������Wpf���ͻ�ȡ��ӦWpf�ļ��ӿ�
	sdWpfLoader* GetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex);

protected:
	struct stWpfFileNameMapEntry
	{
		sdWpfLoader* apkWpfInterface[NUM_WPFS];	
		stWpfFileNameMapEntry() 
		{ 
			for (int i = 0; i < NUM_WPFS; ++i) 
				apkWpfInterface[i] = NULL;
		}
	};
	typedef stWpfFileNameMapEntry WpfFileNameMapEntry;
	typedef std::map<std::string, WpfFileNameMapEntry> WpfMap;
	typedef std::map<std::string, WpfFileNameMapEntry>::iterator WpfMapItr;
	WpfMap m_kWpfMap;						///< ����world������wpf�ļ�
	char m_szGameRootPath[MAX_PATH_LEN];	///< ��Ϸ��Ŀ¼
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif