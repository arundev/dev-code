//*************************************************************************************************
// 内容:	地形系统基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-19
// 最后修改:
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

		// 初始化与销毁
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;

		bool IsInitialized() { return m_bInitialized;}

		// 属性访问
		void SetVisiable(bool bVisiable) { m_bVisiable = bVisiable;}
		bool GetVisiable() { return m_bVisiable; }

		// 地形裁剪(世界坐标)
		//(wz封装了一层TerrainTileEntity,貌似没啥必要,这里直接返回Mesh)
		virtual void Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec) = 0;

		// 地形渲染属性
		virtual const sdTerrainParams& GetTerrainParams() const = 0;

		// 获取地形系统接口
		static sdTerrainSystem* GetTerrainSystem(){ return InstancePtr();}

	protected:
		bool m_bInitialized;
		bool m_bVisiable;
	};
}*/
#endif