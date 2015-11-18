//*************************************************************************************************
// 内容:	场景小物件对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-02
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_DOODADS_H__
#define _SD_ENGINE_SCENE_DOODADS_H__
#include <sdVector4.h>
namespace Scene
{
	// 一个Doodad实例对象
	//
	// \class sdDoodad	
	//		The self of sdDoodad is just a placeholder, it only defined placeholder's 
	//	location, rotation, scale. It represents a doodad when it is holder by sdDoodadGroup
	//	with a sdDoodadSort instance which defined the doodad's apperance and behavior. 
	//	It can be created only by sdDoodadGroup.
	//		
	class sdDoodad : public NiMemObject
	{
	public:

	protected:
		// Doodad的变换
		//	m_kPackData0,m_kPackData1,m_kPackData2					保存旋转缩放矩阵
		//	m_kPosAndAlpha[0],m_kPosAndAlpha[1],m_kPosAndAlpha[2]	保存世界坐标
		//	m_kPosAndAlpha[3]	保存用于Fading的Alpha值
		NiFloat16	m_kPackData0[4];
		NiFloat16	m_kPackData1[4];
		NiFloat16	m_kPackData2[4];
		Base::Math::sdVector4 m_kPosAndAlpha;	
	};

	typedef std::vector<sdDoodad*>	DoodadsVec;
	typedef std::vector<sdDoodad*>::iterator DoodadsVecItr;

	// 一个Doodads对象,包含一组Doodads对象实例
	//
	// \class sdDoodadsGroup
	//		It represent a group of doodads.
	//		It is the basic doodads rendering element.
	//		All children doodads have the same apperance and behivor which defined 
	//	by the sdDoodadSort, but distribute at different places with different 
	//	transform defined by the sdDoodad.
	//
	class sdDoodadsGroup : public NiRefObject
	{
	public:
		

		// Doodad的创建与销毁
		sdDoodad*	CreateDoodad();
		void DestroyDoodad(sdDoodad* pkDoodad);
		void DestroyAllDoodads();	

		//
		void Update();


	protected:
		// Doodad实例列表,考虑使用Pool来避免内存碎片
		DoodadsVec	m_kDoodadsVec;
		
		// Doodad的Mesh对象
		NiMeshPtr	m_spMesh;

		// Doodad缓存池(用于编辑器)
	};
	NiSmartPointer(sdDoodadsGroup);

	typedef std::vector<sdDoodadsGroup*> DoodadsGroupsVec;
	typedef std::vector<sdDoodadsGroup*>::iterator DoodadsGroupVecItr;

	//
	class sdDoodadsFarm : public NiRefObject
	{

	};
	NiSmartPointer(sdDoodadsFarm);

	//
	// \class sdDoodadsSort
	//	It is the definition a sort of doodad. 
	//	It defines this sort of doodad's apperance and behavior.
	//
	// \TODO 
	//	1.Geometry LOD support.
	//	2.Physics attributes for winds.
	//
	class sdDoodadsSort : public NiRefObject
	{

	protected:
		NiFixedString	m_szName;				// 资源路径	
		NiMeshPtr		m_spGeometryMesh;
		NiMaterialPtr	m_spGeometryMaterial;
		NiMaterialPtr	m_spShadingMaterial;

		// 资源的所有实例信息
		DoodadsGroupsVec m_kDoodadsGroupVec;
	};
	NiSmartPointer(sdDoodadsSort);

	//
	class sdDoodadsManager : public NiRefObject
	{

	};
	NiSmartPointer(sdDoodadsManager);
}
#endif