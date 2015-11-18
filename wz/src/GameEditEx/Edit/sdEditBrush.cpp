#include "sdGameEditExPCH.h"
#include "sdEditBrush.h"

#include <sdMap.h>
#include <sdTerrain.h>
#include <IRenderSystem.h>

//
#include <sdRay.h>
#include <sdMatrix3.h>
#include <sdTimeMgr.h>

//
using namespace Base;
using namespace Base::Math;
using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformPoolBrush::Apply(const sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		// 
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)GetEditShape();
		NIASSERT(pkDualCirlceShape);		
		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);
		
		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(�߶�ͼ����)
		float fScale = pkTerrain->GetScale();
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fHStartX = (fStartX - kOrigin.x) / fScale;
		float fHStartY = (fStartY - kOrigin.y) / fScale;
		float fHEndX = (fEndX - kOrigin.x) / fScale;
		float fHEndY = (fEndY - kOrigin.y) / fScale;

		// ǯλ����Χ������(�߶�ͼ����)
		uint uiSize = pkTerrain->GetTerrainSize();

		if (fHStartX > (float)uiSize)	return;
		if (fHStartY > (float)uiSize)	return;
		if (fHEndX < 0)	return;
		if (fHEndY < 0) return;

		if (fHStartX < 0)	fHStartX = 0.0f;
		if (fHStartY < 0)	fHStartY = 0.0f;
		if (fHEndX > (float)uiSize)	fHEndX = (float)uiSize;
		if (fHEndY > (float)uiSize)	fHEndY = (float)uiSize;

		uint uiHStartX = (uint)fHStartX;
		uint uiHStartY = (uint)fHStartY;
		uint uiHEndX = (uint)(fHEndX);
		uint uiHEndY = (uint)(fHEndY);

		// �����߶�����,����
		//	1.�任����������ϵ
		//	2.��ȡȨֵ
		//	3.�任
		static const float fPullSpeed = 10.f;
		float fFrameTime = sdTimeMgr::Instance().GetFrameTime();
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					float fOldHeight = pkTerrain->GetRawHeight(uiHX, uiHY);
					float fWeight = pkDualCirlceShape->GetWeight(fX, fY);
					float fBias = fFrameTime * fPullSpeed * fWeight * m_fHeight;
					pkTerrain->SetRawHeight(uiHX, uiHY, fOldHeight + fBias);
				}
			}
		}

		// ����Mesh
		pkTerrain->UpdateGeometry(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformSmoothBrush::Apply(const Base::Math::sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	// ���Ƹ����ٶ�
	//static float fLastFrameTime = 0.f;
	//static const float fSmoothSpeed = 1.f;
	//float fFrameTime = sdTimeMgr::Instance().GetCurFrameTime();
	//if (fFrameTime - fLastFrameTime < 1.0f / fSmoothSpeed)
	//	return;
	//fLastFrameTime = fFrameTime;

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		//
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)(sdEditShape*)m_pkEditShape;
		NIASSERT(pkDualCirlceShape);
		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);

		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(�߶�ͼ����)
		float fScale = pkTerrain->GetScale();
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fHStartX = (fStartX - kOrigin.x) / fScale;
		float fHStartY = (fStartY - kOrigin.y) / fScale;
		float fHEndX = (fEndX - kOrigin.x) / fScale;
		float fHEndY = (fEndY - kOrigin.y) / fScale;

		// ǯλ����Χ������
		uint uiSize = pkTerrain->GetTerrainSize();

		if (fHStartX > (float)uiSize)	return;
		if (fHStartY > (float)uiSize)	return;
		if (fHEndX < 0)	return;
		if (fHEndY < 0) return;

		if (fHStartX < 0)	fHStartX = 0.0f;
		if (fHStartY < 0)	fHStartY = 0.0f;
		if (fHEndX > (float)uiSize)	fHEndX = (float)uiSize;
		if (fHEndY > (float)uiSize)	fHEndY = (float)uiSize;

		uint uiHStartX = (uint)fHStartX;
		uint uiHStartY = (uint)fHStartY;
		uint uiHEndX = (uint)(fHEndX);
		uint uiHEndY = (uint)(fHEndY);

		// �����߶�����,����
		//	1.���ƫ��,�任����������ϵ
		//	2.��ȡȨֵ
		//	3.�任
		// @{
		// 
		sdMatrix3 kFilter33(0.1f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.1f);

		// ��������()
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					// ����ƽ��߶�
					float fAverageHeight = 0.0f;
					for (int iDeltaY = 0; iDeltaY < 3; ++iDeltaY)
					{
						for (int iDeltaX = 0; iDeltaX < 3; ++iDeltaX)
						{
							int iHX = (int)uiHX + iDeltaX - 1;
							int iHY = (int)uiHY + iDeltaY - 1;
							
							if (iHX < 0)	iHX = 0;
							if (iHY < 0)	iHY = 0;
							if (iHX > (int)uiSize)	iHX = (int)uiSize;
							if (iHY > (int)uiSize)	iHY = (int)uiSize;

							fAverageHeight += pkTerrain->GetRawHeight((uint)iHX, (uint)iHY) * 
								kFilter33.m[iDeltaX][iDeltaY];
						}
					}

					// ��ֵ
					float fFactor = pkDualCirlceShape->GetWeight(fX, fY);
					float fOldHeight = pkTerrain->GetRawHeight(uiHX, uiHY);
					pkTerrain->SetRawHeight(uiHX, uiHY, sdMath::Lerp(fOldHeight, fAverageHeight, fFactor));
				}
			}
		}
		// @}

		// ����Mesh
		pkTerrain->UpdateGeometry(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformNoiseBrush::Apply(const Base::Math::sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		//
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)(sdEditShape*)m_pkEditShape;
		NIASSERT(pkDualCirlceShape);
		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);

		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(�߶�ͼ����)
		float fScale = pkTerrain->GetScale();
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fHStartX = (fStartX - kOrigin.x) / fScale;
		float fHStartY = (fStartY - kOrigin.y) / fScale;
		float fHEndX = (fEndX - kOrigin.x) / fScale;
		float fHEndY = (fEndY - kOrigin.y) / fScale;

		// ǯλ����Χ������
		uint uiSize = pkTerrain->GetTerrainSize();

		if (fHStartX > (float)uiSize)	return;
		if (fHStartY > (float)uiSize)	return;
		if (fHEndX < 0)	return;
		if (fHEndY < 0) return;

		if (fHStartX < 0)	fHStartX = 0.0f;
		if (fHStartY < 0)	fHStartY = 0.0f;
		if (fHEndX > (float)uiSize)	fHEndX = (float)uiSize;
		if (fHEndY > (float)uiSize)	fHEndY = (float)uiSize;

		uint uiHStartX = (uint)fHStartX;
		uint uiHStartY = (uint)fHStartY;
		uint uiHEndX = (uint)(fHEndX);
		uint uiHEndY = (uint)(fHEndY);

		// �����߶�����,����
		//	1.���ƫ��,�任����������ϵ
		//	2.��ȡȨֵ
		//	3.�任
		static const float fNoiseSpeed = 1.f;
		float fFrameTime = sdTimeMgr::Instance().GetFrameTime();
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					float fOldHeight = pkTerrain->GetRawHeight(uiHX, uiHY);
					float fWeight = pkDualCirlceShape->GetWeight(fX, fY);
					float fBias = fFrameTime * fWeight * 
						(8.f - sdMath::Frand() * 16.f) * fNoiseSpeed * m_fHeight;

					pkTerrain->SetRawHeight(uiHX, uiHY, fOldHeight + fBias);
				}
			}
		}

		// ����Mesh
		pkTerrain->UpdateGeometry(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformFlatBrush::Apply(const Base::Math::sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		//
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)(sdEditShape*)m_pkEditShape;
		NIASSERT(pkDualCirlceShape);
		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);

		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(�߶�ͼ����)
		float fScale = pkTerrain->GetScale();
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fHStartX = (fStartX - kOrigin.x) / fScale;
		float fHStartY = (fStartY - kOrigin.y) / fScale;
		float fHEndX = (fEndX - kOrigin.x) / fScale;
		float fHEndY = (fEndY - kOrigin.y) / fScale;

		// ǯλ����Χ������
		uint uiSize = pkTerrain->GetTerrainSize();

		if (fHStartX > (float)uiSize)	return;
		if (fHStartY > (float)uiSize)	return;
		if (fHEndX < 0)	return;
		if (fHEndY < 0) return;

		if (fHStartX < 0)	fHStartX = 0.0f;
		if (fHStartY < 0)	fHStartY = 0.0f;
		if (fHEndX > (float)uiSize)	fHEndX = (float)uiSize;
		if (fHEndY > (float)uiSize)	fHEndY = (float)uiSize;

		uint uiHStartX = (uint)fHStartX;
		uint uiHStartY = (uint)fHStartY;
		uint uiHEndX = (uint)(fHEndX);
		uint uiHEndY = (uint)(fHEndY);

		// �����߶�����,����
		//	1.���ƫ��,�任����������ϵ
		//	2.��ȡȨֵ
		//	3.�任
		static const float fNoiseSpeed = 1.f;
		float fFrameTime = sdTimeMgr::Instance().GetFrameTime();
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					pkTerrain->SetRawHeight(uiHX, uiHY, m_fHeight);
				}
			}
		}

		// ����Mesh
		pkTerrain->UpdateGeometry(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformAverageBrush::Apply(const Base::Math::sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		//
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)(sdEditShape*)m_pkEditShape;
		NIASSERT(pkDualCirlceShape);
		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);

		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(�߶�ͼ����)
		float fScale = pkTerrain->GetScale();
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fHStartX = (fStartX - kOrigin.x) / fScale;
		float fHStartY = (fStartY - kOrigin.y) / fScale;
		float fHEndX = (fEndX - kOrigin.x) / fScale;
		float fHEndY = (fEndY - kOrigin.y) / fScale;

		// ǯλ����Χ������
		uint uiSize = pkTerrain->GetTerrainSize();

		if (fHStartX > (float)uiSize)	return;
		if (fHStartY > (float)uiSize)	return;
		if (fHEndX < 0)	return;
		if (fHEndY < 0) return;

		if (fHStartX < 0)	fHStartX = 0.0f;
		if (fHStartY < 0)	fHStartY = 0.0f;
		if (fHEndX > (float)uiSize)	fHEndX = (float)uiSize;
		if (fHEndY > (float)uiSize)	fHEndY = (float)uiSize;

		uint uiHStartX = (uint)fHStartX;
		uint uiHStartY = (uint)fHStartY;
		uint uiHEndX = (uint)(fHEndX);
		uint uiHEndY = (uint)(fHEndY);

		// �����߶�����,�����Ȩƽ��ֵ
		float fNumerator = 0.f;
		float fDenominator = 0.f;
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					float fOldHeight = pkTerrain->GetRawHeight(uiHX, uiHY);
					float fWeight = pkDualCirlceShape->GetWeight(fX, fY);
					fNumerator += fOldHeight * fWeight;
					fDenominator += fWeight;
				}
			}
		}

		// �����߶�����, ����
		float fAverage = fNumerator / fDenominator;
		for (uint uiHY = uiHStartY; uiHY < uiHEndY; ++uiHY)
		{
			float fY = (uiHY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiHX = uiHStartX; uiHX < uiHEndX; ++uiHX)
			{
				float fX = (uiHX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					float fOldHeight = pkTerrain->GetRawHeight(uiHX, uiHY);
					float fWeight = pkDualCirlceShape->GetWeight(fX, fY);
					float fBias = sdMath::Lerp(fOldHeight, fAverage, fWeight);
					pkTerrain->SetRawHeight(uiHX, uiHY, fBias);
				}
			}
		}

		// ����Mesh
		pkTerrain->UpdateGeometry(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainSurfaceLayerBrush::Apply(const sdRay& kRay)
{
	NIASSERT(m_pkBrushShape);
	NIASSERT(m_pkEditShape);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// ����ʰȡ���ε�
	sdVector3 kIntersect;
	if (pkTerrain->Pick(kRay, kIntersect))	
	{
		//
		sdDualCircleShape* pkDualCirlceShape = (sdDualCircleShape*)(sdEditShape*)m_pkEditShape;
		NIASSERT(pkDualCirlceShape);

		pkDualCirlceShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);

		// ���㷶Χ(��������)
		float fOuterRadius = pkDualCirlceShape->GetOuterRadius();
		float fStartX	= kIntersect.m_fX - fOuterRadius;
		float fStartY	= kIntersect.m_fY - fOuterRadius;
		float fEndX	= kIntersect.m_fX + fOuterRadius;
		float fEndY = kIntersect.m_fY + fOuterRadius;

		// ���㷶Χ(BlendMap����)
		float fScale = pkTerrain->GetScale() * pkTerrain->GetBlendMapScale();
		float fScaleDiv = 1.f / fScale;
		const NiPoint3& kOrigin = pkTerrain->GetOrigin();
		float fBStartX = (fStartX - kOrigin.x) * fScaleDiv;
		float fBStartY = (fStartY - kOrigin.y) * fScaleDiv;
		float fBEndX = (fEndX - kOrigin.x) * fScaleDiv;
		float fBEndY = (fEndY - kOrigin.y) * fScaleDiv;

		// ǯλ����Χ������(BlendMap����)
		uint uiSize = pkTerrain->GetBlendMapSize();

		if (fBStartX > (float)uiSize)	return;
		if (fBStartY > (float)uiSize)	return;
		if (fBEndX < 0.f)	return;
		if (fBEndY < 0.f)	return;

		if (fBStartX < 0)	fBStartX = 0.0f;
		if (fBStartY < 0)	fBStartY = 0.0f;
		if (fBEndX > (float)uiSize)	fBEndX = (float)uiSize;
		if (fBEndY > (float)uiSize)	fBEndY = (float)uiSize;

		uint uiBStartX = (uint)sdMath::Ceil(fBStartX);	///< ����ȡ������С�༭��Χ
		uint uiBStartY = (uint)sdMath::Ceil(fBStartY);
		uint uiBEndX = (uint)sdMath::Floor(fBEndX);		///< ����ȡ��
		uint uiBEndY = (uint)sdMath::Floor(fBEndY);

		// ����BlendMap����,����
		//	1.�任����������ϵ
		//	2.��ȡȨֵ
		//	3.�任
		static const float fPaintSpeed = 500.0f;
		float fFrameTime = sdTimeMgr::Instance().GetFrameTime();
		for (uint uiBY = uiBStartY; uiBY < uiBEndY; ++uiBY)
		{
			float fY = (uiBY + 0.5f) * fScale + kOrigin.y; 
			for (uint uiBX = uiBStartX; uiBX < uiBEndX; ++uiBX)
			{
				float fX = (uiBX + 0.5f) * fScale + kOrigin.x;
				if (pkDualCirlceShape->InShape(fX, fY))
				{
					// ����ʱ��仯
					float fWeight = pkDualCirlceShape->GetWeight(fX, fY);
					float fBias = fFrameTime * fWeight * fPaintSpeed;
					uint uiOldBlend = pkTerrain->GetBlendMapData(uiBX, uiBY, m_uiActiveLayerMap);
					pkTerrain->SetBlendMapData(uiBX, uiBY, m_uiActiveLayerMap, (uchar)NiClamp(uiOldBlend + fBias, 0.0, 255.0f));
					
					// ����
					//pkTerrain->SetBlendMapData(uiBX, uiBY, m_uiActiveLayerMap, 255u);
				}

				// WZ����

			}
		}

		// ��������
		pkTerrain->UpdateBlendMap(kIntersect.m_fX, kIntersect.m_fY, fOuterRadius);

		// ���±�ˢ
		m_pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		m_pkBrushShape->Update(0.0f);
		m_pkBrushShape->UpdateShape();
	}
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX