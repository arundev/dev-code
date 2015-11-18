//*************************************************************************************************
// ����:	����С�������
//---------------------------------------------------------
// ����:		
// ����:		2012-10-02
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_DOODADS_H__
#define _SD_ENGINE_SCENE_DOODADS_H__
#include <sdVector4.h>
namespace Scene
{
	// һ��Doodadʵ������
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
		// Doodad�ı任
		//	m_kPackData0,m_kPackData1,m_kPackData2					������ת���ž���
		//	m_kPosAndAlpha[0],m_kPosAndAlpha[1],m_kPosAndAlpha[2]	������������
		//	m_kPosAndAlpha[3]	��������Fading��Alphaֵ
		NiFloat16	m_kPackData0[4];
		NiFloat16	m_kPackData1[4];
		NiFloat16	m_kPackData2[4];
		Base::Math::sdVector4 m_kPosAndAlpha;	
	};

	typedef std::vector<sdDoodad*>	DoodadsVec;
	typedef std::vector<sdDoodad*>::iterator DoodadsVecItr;

	// һ��Doodads����,����һ��Doodads����ʵ��
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
		

		// Doodad�Ĵ���������
		sdDoodad*	CreateDoodad();
		void DestroyDoodad(sdDoodad* pkDoodad);
		void DestroyAllDoodads();	

		//
		void Update();


	protected:
		// Doodadʵ���б�,����ʹ��Pool�������ڴ���Ƭ
		DoodadsVec	m_kDoodadsVec;
		
		// Doodad��Mesh����
		NiMeshPtr	m_spMesh;

		// Doodad�����(���ڱ༭��)
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
		NiFixedString	m_szName;				// ��Դ·��	
		NiMeshPtr		m_spGeometryMesh;
		NiMaterialPtr	m_spGeometryMaterial;
		NiMaterialPtr	m_spShadingMaterial;

		// ��Դ������ʵ����Ϣ
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