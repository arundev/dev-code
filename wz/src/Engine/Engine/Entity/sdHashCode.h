//*************************************************************************************************
// ����:	
//
//	@brief ��ϵͳĿǰ��ΪsdGameObjectΨһID������,��IDҪ����ܸ�Ч���бȽ�,�����нϺõĿɶ���,
//	���õ�ʵ���Է�������: �ֶδ�СΪ4byte, ����һ��unsigned int����, �Է�����и�Ч�������Ƚ�
//	����,����Ҫ��һ���Ŀɶ���,�����������ļ���,���Խ�4byte�ֱ��ʾΪ�ַ�,����32bit��hash code
//	���ܵ�unsigned int����,Ҳ����Ϊ4���ַ����ַ���ʹ��,Ϊ�˼�����ײ��,��Ҫ��Ӱ����ַ�ת��,
//	��ҪһЩ�Ƚ��ر���ַ���
//
//---------------------------------------------------------
// ����:		
// ����:		2012-11-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_HASHCODE_H__
#define _SD_ENGINE_SCENE_HASHCODE_H__

// �Ƿ�HashCode
#define INVALID_HASHCODE 0

// ���￼�ǸĽ�:
//	1.ʹ�ö���ɾ��
//	2.ʹ��λ�����ʾ,�ο�<�������>
class sdHashCode : public NiMemObject
{
public:
	sdHashCode();
	~sdHashCode();

	uint	GetCode();
	bool	AddCode(uint uiCode);
	void	RemoveCode(uint uiCode);
	bool	IsCodeExist(uint uiCode);

	void	Clear();
protected:
	uint	GetCode(bool bCollide);

protected:
	int	 m_iA, m_iB, m_iC, m_iD;
	std::vector<uint> m_uiHashCodes;
};

#endif
