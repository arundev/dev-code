//*************************************************************************************************
// ����:	����Mesh���ƶ���
//---------------------------------------------------------
// ����:		
// ����:		2012-08-01
// ����޸�:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_QUAD_NODE_H__
#define _TERRAIN_QUAD_NODE_H__
#include <sdVector2.h>
#include <sdVector4.h>
#include <sdAxisAlignedBox.h>
#include "sdQuadMesh.h"

class sdTerrain;
class sdQuadNode;
NiSmartPointer(sdQuadNode);
//-------------------------------------------------------------------------------------------------
// ����QuadTree�ڵ�,ÿ���ڵ�ֻ��һ��Mesh,��ʱľ��֧�ֽڵ���LOD.
//
//	\TODO ����Ԥ����ڵ����Ż��ڴ�
//	\TODO ���ǵ��Ժ�ʵʩ���޵�ͼ��Ҫ����Ĳ���,������ò�Ҫ������ص��ྲ̬��Ա����
//	\TODO �����Ժ�ʵ��ÿ���ڵ��ڶ༶LOD
//	\TODO ����ʵ�ָ�ƽ����LOD��ĿǰLODû�й��ˣ�ֱ�Ӳ���
//-------------------------------------------------------------------------------------------------
class sdQuadNode : public NiRefObject
{
public:
	// �Ĳ����ڵ�ö��
	enum EQuadNodeIndex
	{
		E_LT_CHILD = 0,
		E_RT_CHILD,
		E_LB_CHILD,
		E_RB_CHILD,
		NUM_CHILDREN,
	};

public:
	sdQuadNode(sdTerrain* pkTerrain = NULL);
	~sdQuadNode();

	// �Ӹ߶�ͼ�����Ĳ���,��ָ����Χ����ϸ��,ֱ���ߴ����QuadSize
	bool	Build(Base::Math::sdVector4ui& kRect, sdQuadNode* pkParent);

	// ����
	void	Destroy();

	// �����Ĳ���ָ������ļ�����(һ���Ǹ߶�ͼ���޸���)
	void	UpdateGeometry(float fCenterX, float fCenterY, float fRadius);

	// ��ָ������ü��Ĳ���
	void	Cull(const NiCamera& kCamera, NiFrustumPlanes& kFrustumPlanes,std::vector<NiMesh*>& kMeshVec);

	// ���Է���
	bool	IsLeaf() const { return (m_uiLevel == 0);}
	uint	GetLevel() const { return m_uiLevel;}
	uint	GetSpacing() const { return m_uiSpacing;}
	float	GetError() const { return m_fError;}
	float	GetAccumError() const { return m_fAccumError;}

	const Base::Math::sdAxisAlignedBox& GetBound() const { return m_kAABB;}

protected:
	// ���㵱ǰ�ڵ�����һ�ڵ�����߶�ͼ֮������������ֵ
	float	CalcError();

	// ���㵱ǰ�ڵ�߶�ͼ�İ�Χ��
	void	CalcBound(Base::Math::sdAxisAlignedBox& kAABB);

protected:
	// �������ζ���
	sdTerrain* m_pkTerrain;

	// ���ڵ��뺢�ӽڵ�(ע����⻥������)
	sdQuadNodePtr	m_pkQuadNodeChild[NUM_CHILDREN];
	sdQuadNode*		m_pkQuadNodePar;
	
	// ��ǰ�ڵ��Mesh
	sdQuadMeshPtr	m_pkQuadMesh;

	uint	m_uiLevel;		// ��ǰ�ڵ����(Ҷ�ڵ�Ϊ0,���ϵ���,���ڵ����)
	uint	m_uiSpacing;	// ��ǰ�ڵ�դ��ߴ�(Ҷ�ڵ�Ϊ1,���ϼ��μ�������)
	float	m_fError;		// ��ǰ�ڵ�����һ�ڵ�������,��Ϊ����Tileȹ�Ӹ߶�
	float	m_fAccumError;	// ��ǰ�ڵ��ۻ�������(Ҷ�ڵ�Ϊ0,��Ҷ�ڵ������ۻ�)

	// ��ǰ�ڵ���ʼ��λ��
	uint	m_uiOriginX;	
	uint	m_uiOriginY;		

	// ��ǰ�ڵ��Χ��(�ýڵ����������Ĳ�����Ӧ�߶�ͼ�İ�Χ��)
	Base::Math::sdAxisAlignedBox	m_kAABB;	
};
//-------------------------------------------------------------------------------------------------
#endif