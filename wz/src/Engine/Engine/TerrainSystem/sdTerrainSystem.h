//*************************************************************************************************
// ����:	����ϵͳ�����ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-12-19
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TERRAIN_SYSTEM_H__
#define _SD_ENGINE_TERRAIN_SYSTEM_H__
#include <sdSingleton.h>
/*
namespace TerrainSystem
{
	class sdTerrainSystem : public Base::sdTSingleton<sdTerrainSystem>
	{
	public:
		sdTerrainSystem() : m_bInitialized(false), m_bVisiable(true) {}
		virtual ~sdTerrainSystem() = 0;

		// ��ʼ��������
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;

		bool IsInitialized() { return m_bInitialized;}

		// ���Է���
		void SetVisiable(bool bVisiable) { m_bVisiable = bVisiable;}
		bool GetVisiable() { return m_bVisiable; }

		// ���βü�(��������)
		//(wz��װ��һ��TerrainTileEntity,ò��ûɶ��Ҫ,����ֱ�ӷ���Mesh)
		virtual void Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec) = 0;

		// ������Ⱦ����
		virtual const sdTerrainParams& GetTerrainParams() const = 0;

		// ��ȡ����ϵͳ�ӿ�
		static sdTerrainSystem* GetTerrainSystem(){ return InstancePtr();}

	protected:
		bool m_bInitialized;
		bool m_bVisiable;
	};
}*/
#endif