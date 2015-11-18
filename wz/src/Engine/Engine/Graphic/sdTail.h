//*************************************************************************************************
// ����:	������β(ʵ��1)
//---------------------------------------------------------
// ����:		
// ����:		2012-12-26
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_GRAPHIC_TAIL_H__
#define _SD_ENGINE_GRAPHIC_TAIL_H__

ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM

class sdTail : public NiNode
{
	NiDeclareRTTI;

public:
	sdTail();
	virtual ~sdTail();

	// �����������
	//  @param fMaxTailLength	������β����
	//	@param fSegmentLength	����Mesh�ֶγ���	
	bool Create(float fMaxTailLength, float fSegmentLength);

	// ����/����/��ֹ
	void Start();
	void Stop();
	void UpdateFrame(float fAccumTime, float fFrameTime);

	// ���������ҽӵ�
	void SetDummy(NiNode* spDummy0, NiNode* spDummy1);
	
	// ���õ�������
	void SetTexture(NiTexture* spTexture0, NiTexture* spTexture1);

protected:
	// ��������Mesh
	NiMeshPtr CreateMesh(float fMaxTailLength, float fSegmentLength);

	// 

protected:
	// �������
	NiMeshPtr m_spGeometry0;
	NiMeshPtr m_spGeometry1;

	// �ҽӵ�
	NiNode* m_spDummy0;
	NiNode* m_spDummy1;

	// �������
	float	m_fMaxTailLength;	///< ���ⳤ��
	float	m_fSegmentLength;	///< ����Mesh�ֶγ���

	// ��ǰ����״̬
	bool	m_bPlaying;
	float	m_fFadeOuting;
	float	m_fFadeTime;
	float	m_fLastTime;

private:
	// ����������
	struct stTailVertex
	{
		NiPoint3 pos;
		NiColorA color;
		NiPoint2 uv;
	};
};
NiSmartPointer(sdTail);

ENGINE_NAMESPACE_END_RENDERSYSTEM
#endif