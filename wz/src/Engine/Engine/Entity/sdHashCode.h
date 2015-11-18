//*************************************************************************************************
// 内容:	
//
//	@brief 此系统目前作为sdGameObject唯一ID生成器,此ID要求既能高效进行比较,又能有较好的可读性,
//	采用的实验性方法如下: 字段大小为4byte, 等于一个unsigned int长度, 以方便进行高效的整数比较
//	操作,而又要有一定的可读性,方便流化到文件中,所以将4byte分别表示为字符,这样32bit的hash code
//	既能当unsigned int解释,也能作为4个字符的字符串使用,为了减少碰撞率,又要不影响简单字符转义,
//	需要一些比较特别的字符串
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_HASHCODE_H__
#define _SD_ENGINE_SCENE_HASHCODE_H__

// 非法HashCode
#define INVALID_HASHCODE 0

// 这里考虑改进:
//	1.使用惰性删除
//	2.使用位数组表示,参考<编程珠玑>
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
