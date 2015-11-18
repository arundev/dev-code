//*************************************************************************************************
// 内容:	刀光拖尾(实现1)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-26
// 最后修改:
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

	// 创建刀光对象
	//  @param fMaxTailLength	刀光拖尾长度
	//	@param fSegmentLength	刀光Mesh分段长度	
	bool Create(float fMaxTailLength, float fSegmentLength);

	// 播放/更新/终止
	void Start();
	void Stop();
	void UpdateFrame(float fAccumTime, float fFrameTime);

	// 设置两个挂接点
	void SetDummy(NiNode* spDummy0, NiNode* spDummy1);
	
	// 设置刀光纹理
	void SetTexture(NiTexture* spTexture0, NiTexture* spTexture1);

protected:
	// 创建刀光Mesh
	NiMeshPtr CreateMesh(float fMaxTailLength, float fSegmentLength);

	// 

protected:
	// 刀光对象
	NiMeshPtr m_spGeometry0;
	NiMeshPtr m_spGeometry1;

	// 挂接点
	NiNode* m_spDummy0;
	NiNode* m_spDummy1;

	// 刀光参数
	float	m_fMaxTailLength;	///< 刀光长度
	float	m_fSegmentLength;	///< 刀光Mesh分段长度

	// 当前播放状态
	bool	m_bPlaying;
	float	m_fFadeOuting;
	float	m_fFadeTime;
	float	m_fLastTime;

private:
	// 顶点数据流
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