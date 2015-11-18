#include "sdRenderSystemPCH.h"
#include "sdRenderObject.h"

#include <sdMath.h>
#include <sdVector4.h>

using namespace Base::Math;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdRenderObject::sdRenderObject(sdRenderObject::ERenderObjectType eType, 
	NiMesh* pkMesh, NiMaterial* pkMaterial, NiPropertyList* pkRenderProps, 
	ShaderParamItem* pkShaderParams)
: m_eType(eType)
, m_pkMesh(pkMesh)
, m_pkMaterial(pkMaterial)
, m_pkProperties(pkRenderProps)
, m_pkShaderParams(pkShaderParams)
{
	// ����SortKey
	// @{
	// Material��VertexBuffer
	NIASSERT(pkMesh);
	IDirect3DVertexBuffer9*	pkVertexBuffer = NULL;
	NiDataStreamRef* pkVertexStreamRef = pkMesh->FindStreamRef(NiCommonSemantics::POSITION());
	if (pkVertexStreamRef)
	{
		NiDataStream* pkStream = pkVertexStreamRef->GetDataStream();
		NiDX9DataStream* spDX9Stream = NiVerifyStaticCast(NiDX9DataStream, pkStream);
		pkVertexBuffer = spDX9Stream->GetVertexBuffer();
	}
	m_kRenderKey.key0 = (((uint)m_pkMaterial & 0xffff) << 16) | ((uint)pkVertexBuffer & 0xffff);

	// BaseTexture��NormalTexture	
	NiPropertyState* pkPropertyState = pkMesh->GetPropertyState();
	NIASSERT(pkPropertyState);

	NiTexturingProperty* pkTexturingProp = pkPropertyState->GetTexturing();
	if (pkTexturingProp)
	{
		NiTexture* spBaseTexture = pkTexturingProp->GetBaseTexture();
		NiTexture* spNormalTexture = pkTexturingProp->GetNormalMap() ? 
			pkTexturingProp->GetNormalMap()->GetTexture() : NULL;

		m_kRenderKey.key0 = (((uint)spBaseTexture & 0xffff) << 16) | ((uint)spNormalTexture & 0xffff);
	}

	// DrawMode
	NiStencilProperty* pkStencilProp = pkPropertyState->GetStencil();
	if (pkStencilProp)
	{
		m_kRenderKey.key1 = (uint)pkStencilProp->GetDrawMode() + 1;	///< ???Ϊʲô��һ???
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::ApplyRenderSetting()
{
	ApplyRenderProps();
	ApplyMaterial();

	SetShaderParams();
	SetRenderStates();
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::RestoreRenderSetting()
{
	RestoreMaterial();
	RestoreRenderProps();
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::ApplyRenderProps()
{
	if (m_pkProperties)
	{
		ApplyTexturingProp();
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::RestoreRenderProps()
{
	if (m_pkProperties)
	{
		RestoreTexturingProp();
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::ApplyTexturingProp()
{
	// ����Meshԭ�е�NiTexturingProperty
	NiPropertyState* pkMeshPropertyState = m_pkMesh->GetPropertyState();
	if (!pkMeshPropertyState)
		return;

	NiTexturingProperty* pkMeshTexturingProp = pkMeshPropertyState->GetTexturing();
	if (!pkMeshTexturingProp)	
		return;

	// �����µ�NiTexturingProperty
	NiProperty* pkProperty = FindProperty(m_pkProperties, NiProperty::TEXTURING);
	if (!pkProperty)
		return;

	NiTexturingProperty* pkTexturingProp = (NiTexturingProperty*)pkProperty;
	if (!pkTexturingProp)	
		return;

	// ��������
	const NiTexturingProperty::NiMapArray& kMeshMaps = pkMeshTexturingProp->GetMaps();
	const NiTexturingProperty::NiMapArray& kMaps = pkTexturingProp->GetMaps();
	
	NiTexturingProperty::MapEnum eMapEnumTable[] = 
	{
		NiTexturingProperty::BASE_INDEX,
		NiTexturingProperty::DARK_INDEX,
		NiTexturingProperty::DETAIL_INDEX,
		NiTexturingProperty::GLOSS_INDEX,
		NiTexturingProperty::GLOW_INDEX,
		NiTexturingProperty::BUMP_INDEX,
		NiTexturingProperty::NORMAL_INDEX,
		NiTexturingProperty::PARALLAX_INDEX,
	};

	int iSize = sizeof(eMapEnumTable) / sizeof(eMapEnumTable[0]);
	for (int i = 0; i < iSize; ++i)
	{
		pkTexturingProp->SetMap(eMapEnumTable[i], kMeshMaps.GetAt(eMapEnumTable[i]));	///< ��������
		NiTexturingProperty::Map* pkMap = kMaps.GetAt(eMapEnumTable[i]);
		if (pkMap)
		{
			NiTexture* pkTexture = pkMap->GetTexture();
			
//			//if (pkTexture)
//			//	pkTexture->TickFrameID();
//
//			// �����������
//			if (!pkTexture)
//				continue;
//
//			if (pkTexture->GetRTTI() != &sdSourceTexture::ms_RTTI)
//				continue;
//
//			sdSourceTexture* pkSourceTexture = (sdSourceTexture*)pkTexture;
//			if (pkSourceTexture->GetTexStatus() == sdSourceTexture::E_TEX_IGNORED)	
//			{
//				ITextureManager* pkTextureManager = ITextureManager::GetTextureManager();
//				if (pkTextureManager)
//					pkTextureManager->LoadSourceTexture((sdSourceTexture*)pkSourceTexture);
//
//				uchar ucSkipLevel = 0;
//				sdTextureLodManager* pkTextureLodManager = ITextureLodManager::GetTextureLodManager();
//				if (pkTextureLodManager)
//					ucSkipLevel = pkTextureLodManager->GetSkipLevel();
//
//				pkSourceTexture->AsyncReloadResource(ucSkipLevel);
//			}
		}
	}

	// ����Mesh��ԭ��NiTexturingProperty���������µ�NiTexturingProperty��Mesh
	m_spOldTexturingProp = pkMeshTexturingProp;		
	pkMeshPropertyState->SetProperty(pkTexturingProp);
}	
//-------------------------------------------------------------------------------------------------
void sdRenderObject::RestoreTexturingProp()
{
	NiPropertyState* pkMeshPropertyState = m_pkMesh->GetPropertyState();
	if (!pkMeshPropertyState)
		return;

	pkMeshPropertyState->SetProperty(m_spOldTexturingProp);	///< �ָ�NiTexturingProperty
	m_spOldTexturingProp = 0;
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::ApplyMaterial()
{
	if (m_pkMaterial)
	{
		m_spOldMaterial = (NiMaterial*)m_pkMesh->GetActiveMaterial();	///< ���ݾɲ���
		m_pkMesh->ApplyAndSetActiveMaterial(m_pkMaterial);				///< �����²���
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::RestoreMaterial()
{
	if (m_pkMaterial)
	{
		m_pkMesh->ApplyAndSetActiveMaterial(m_spOldMaterial);
		m_pkMaterial = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::SetShaderParams()
{
	if (m_pkShaderParams)
	{
		IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
		for (uint i = 0; NULL != m_pkShaderParams[i].szName; ++i)
		{
			pkRenderDevice->SetGlobalShaderConstant(
				(NiFixedString&)(*m_pkShaderParams[i].szName),
				m_pkShaderParams[i].uiByteSize,
				m_pkShaderParams[i].pvData);
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderObject::SetRenderStates()
{
	//***********************
	// ���޸�
	// (���˾�����Ҫһ�鿪�������Ƶ���Mesh��RenderState������)
	//if (m_spMaterial)
	//{
	//	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	//	NiPropertyState* spPropertyState = m_spMesh->GetPropertyState();
	//	NiStencilProperty*	spStencilProp = spPropertyState->GetStencil();
	//	if (spStencilProp)
	//	{
	//		// �˴��ο�GB��NiStencilProperty
	//		static D3DCULL d3dCullTable[] = 
	//		{
	//			D3DCULL_CW,		// ��ӦGB��DRAW_CCW_OR_BOTH
	//			D3DCULL_CW,		// ��ӦGB��DRAW_CCW
	//			D3DCULL_CCW,	// ��ӦGB��DRAW_CW
	//			D3DCULL_NONE,	// ��ӦGB��DRAW_BOTH
	//		};
	//		NiStencilProperty::DrawMode eDrawMode = spStencilProp->GetDrawMode();
	//		pkRenderDevice->SetRenderState(D3DRS_CULLMODE, d3dCullTable[(uint)eDrawMode]);
	//	}
	//	else
	//	{
	//		pkRenderDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//	}
	//}
	//***********************
}
//-------------------------------------------------------------------------------------------------
NiProperty* sdRenderObject::FindProperty(NiPropertyList* pkPropList, int iPropType)
{
	NIASSERT(pkPropList);

	if (pkPropList->IsEmpty())	
		return NULL;

	NiTListIterator itr =pkPropList->GetHeadPos();
	while (itr)
	{
		NiProperty* pkProp = pkPropList->GetNext(itr);
		if (iPropType == pkProp->Type())
			return pkProp;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------
sdStaticMeshRenderObject::sdStaticMeshRenderObject(NiMesh* pkMesh, NiMaterial* pkMaterial, 
	NiPropertyList* pkRenderProps, ShaderParamItem* pkShaderParams)
: sdRenderObject(E_ROT_STATIC_MESH, pkMesh, pkMaterial, pkRenderProps, pkShaderParams)
{

}
//-------------------------------------------------------------------------------------------------
void sdStaticMeshRenderObject::Draw()
{
	ApplyRenderSetting();
	m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());
	RestoreRenderSetting();
}
//-------------------------------------------------------------------------------------------------
sdGhostMeshRenderObject::sdGhostMeshRenderObject(NiMesh* pkMesh, NiMaterial* pkMaterial, NiPropertyList* pkRenderProps,
	ShaderParamItem* pkShaderParams, uint uiGhostIndex)
: sdRenderObject(E_ROT_GHOST_MESH, pkMesh, pkMaterial, pkRenderProps, pkShaderParams)
, m_uiGhostIndex(uiGhostIndex)
{
	
}
//-------------------------------------------------------------------------------------------------
void sdGhostMeshRenderObject::Draw()
{
//	m_pkMesh->SetGhostIndex(m_uiGhostIndex);	///< ������Ҫ��Ⱦ�Ĳ�Ӱ���
//	m_pkMesh->SubmitGhostData(m_uiGhostIndex);	///< ���ò�Ӱ������Ϣ

	ApplyRenderSetting();
	m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());
	RestoreRenderSetting();

//	m_pkMesh->SetGhostIndex(-1);				///< �����Ӱ���
}
//-------------------------------------------------------------------------------------------------
sdLightVolumeRenderObject::sdLightVolumeRenderObject(sdLightSceneObject* pkLight, NiMaterial* pkMaterial, 
	NiPropertyList* pkRenderProps, ShaderParamItem* pkShaderParams)
: sdRenderObject(E_ROT_LIGHT_VOLUME, FindVolumeMeshForLight(pkLight), pkMaterial, pkRenderProps, pkShaderParams)
, m_pkLight(pkLight)
{

}
//-------------------------------------------------------------------------------------------------
NiMesh*	sdLightVolumeRenderObject::FindVolumeMeshForLight(sdLightSceneObject* pkLight)
{
	NIASSERT(pkLight);

	IRenderSystem* pkRenderSystem =	IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	switch (pkLight->GetLightType())
	{
	case sdLightSceneObject::E_LT_POINT:
		return pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_UNIT_CUBE);
		break;
	
	case sdLightSceneObject::E_LT_SPOT:
		return pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_UNIT_CONE);
		break;

	case sdLightSceneObject::E_LT_RECT_SPOT:
		return pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_UNIT_PYRAMID);
		break;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------
void sdLightVolumeRenderObject::ApplyLightSettings()
{
	NIASSERT(m_pkLight);
	NIASSERT(m_pkMesh);

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	switch (m_pkLight->GetLightType())
	{
	case sdLightSceneObject::E_LT_POINT:
		{
			// Translate light volume mesh to proper position
			m_pkMesh->SetWorldTranslate(m_pkLight->GetPosition());
			m_pkMesh->SetWorldRotate(m_pkLight->GetRotate());
			m_pkMesh->SetWorldScale(m_pkLight->GetAttenuRange() * 2.f);	///< ��Χ��뾶Ϊ0.5f

			// ����˥�����������ø���ɫ��
			float fAttenuRange = m_pkLight->GetAttenuRange();
			float fAttenuSoftness = max(0.001f, m_pkLight->GetAttenuSoftness());
			sdVector4 kAttenuParam(-1.f / (fAttenuSoftness * fAttenuRange), 1.f / fAttenuSoftness, 0.f, 0.f);
			pkRenderDevice->SetGlobalShaderConstant("g_vLocalLightAttenuParams", 
				sizeof(kAttenuParam), 
				&kAttenuParam);

			break;
		}
	case sdLightSceneObject::E_LT_SPOT:
	case sdLightSceneObject::E_LT_RECT_SPOT:
		{
			// Translate light volume mesh to proper position
			//	1.���ｫһ��Բ���岻��������Ϊһ��Բ׶��
			//	2.Gamebryo�Ĺ۲�����ϵ�У�X��ǰ��Y���ϡ�Z����
			NiMatrix3 kScaleRotate;
			NiPoint3 kCol;
			float fAttenuRange = m_pkLight->GetAttenuRange();
			float fSpotFovX = sdMath::Degree2Radian(m_pkLight->GetSpotFovX());
			float fSpotFovY = sdMath::Degree2Radian(m_pkLight->GetSpotFovY());
			const NiPoint3& kGBLightPosition = m_pkLight->GetPosition();
			const NiMatrix3& kLightRotate = m_pkLight->GetRotate();
			
			kLightRotate.GetCol(0, kCol);
			kScaleRotate.SetCol(0, kCol * fAttenuRange);
			kLightRotate.GetCol(1, kCol);
			kScaleRotate.SetCol(1, kCol * tan(fSpotFovX * 0.5f) * fAttenuRange);
			kLightRotate.GetCol(2, kCol);
			kScaleRotate.SetCol(2, kCol * tan(fSpotFovY * 0.5f) * fAttenuRange);
			
			m_pkMesh->SetWorldTranslate(kGBLightPosition);	
			m_pkMesh->SetWorldRotate(kScaleRotate);
			m_pkMesh->SetWorldScale(1.f);

			//
			// ����۲�����ϵͶ������ռ�ı任��
			// ����۲�ռ䵽��Դ����ռ�ı任�������ø���ɫ��
			//
			//	1.����۲�ռ�(D3D�۲�����ϵ,��������ϵ)��
			//		ԭ��λ������۲��
			//		X������
			//		Y���µ���
			//		Z���ڵ���(�ӹ۲�㵽Ŀ���)
			//
			//	2.����ռ�(��������ϵ,��������ϵ)
			//		X������
			//		Y���µ���
			//		Z���ڵ���(�ӹ۲�㵽Ŀ���)		
			//	
			//	3.��Դ�۲�ռ�(�۲�����ϵ,��������ϵ)��
			//		ԭ��λ��Ͷ����۲��
			//		X������
			//		Y���µ���
			//		Z���⵽��(��Ŀ��㵽�۲��)
			//
			//	4.��Դ�ü��ռ�(ͶӰ����ϵ,��������ϵ)��
			//		ԭ��λ�ڲü�����������
			//		X�����ң���Χ[-1,1]
			//		Y���µ��ϣ���Χ[-1,1]
			//		Z���⵽��, ��Χ[-1,1]
			//
			sdMatrix4 kViewToWorldMatrix;
			const sdMatrix4& kWorldToViewMatrix = pkRenderDevice->GetViewMatrix();	///< ��������ϵ-�۲�����ϵ
			sdMath::Invert(kViewToWorldMatrix, kWorldToViewMatrix);					///< �۲�����ϵ-��������ϵ
//			sdMatrix4 kViewToWorldMatrix = pkRenderDevice->GetInvViewMatrix();		///< �۲�����ϵ-��������ϵ
		
			NiPoint3 kGBLightDirection, kGBLightUp;
			kLightRotate.GetCol(0, &(kGBLightDirection.x));			///< ȡ����Դ���ر任��ǰ��
			kLightRotate.GetCol(2, &(kGBLightUp.x));				///< ȡ����Դ���ر任������
		
			sdMatrix4 kWorldToLightViewMatrix;
			sdVector3 kLightPosition(kGBLightPosition.x, kGBLightPosition.y, kGBLightPosition.z);
			sdVector3 kLightDirection(kGBLightDirection.x, kGBLightDirection.y, kGBLightDirection.z);
			sdVector3 kLightUp(kGBLightUp.x, kGBLightUp.y, kGBLightUp.z);
			sdMath::LookAtRHToRH(kWorldToLightViewMatrix,			///< ��������ϵ-��Դ�۲�����ϵ	
				kLightPosition,										///< X������
				kLightPosition + kLightDirection,					///< Y���µ���
				kLightUp);											///< Z���⵽��(��Ŀ��㵽�۲��)
		
			sdMatrix4 kLightViewToLightClipMatrix;
			float fAspect = tan(fSpotFovX * 0.5f) / tan(fSpotFovY * 0.5f);
			sdMath::PerspectiveFov(kLightViewToLightClipMatrix,		///< ��Դ�۲�����ϵ-��Դ�ü�����ϵ
				fSpotFovY, 
				fAspect, 
				0.01f,
				fAttenuRange);

			sdMatrix4 kViewToLightViewMatrix;
			sdMatrix4 kViewToLightClipMatrix;
			sdMath::Mult(kViewToLightViewMatrix, kViewToWorldMatrix, kWorldToLightViewMatrix);
			sdMath::Mult(kViewToLightClipMatrix, kViewToLightViewMatrix, kLightViewToLightClipMatrix);

			float fAttenuSoftness = max(0.001f, m_pkLight->GetAttenuSoftness());
			float fAttenuScale = 1.f / (fAttenuRange * fAttenuSoftness);
			float fAttenuOffset = 1.f / fAttenuSoftness;

			sdVector4 kViewAttenu;
			kViewAttenu.m_fX = kViewToLightViewMatrix._m02 * fAttenuScale + kViewToLightViewMatrix._m03 * fAttenuOffset;
			kViewAttenu.m_fY = kViewToLightViewMatrix._m12 * fAttenuScale + kViewToLightViewMatrix._m13 * fAttenuOffset;
			kViewAttenu.m_fZ = kViewToLightViewMatrix._m22 * fAttenuScale + kViewToLightViewMatrix._m23 * fAttenuOffset;
			kViewAttenu.m_fW = kViewToLightViewMatrix._m32 * fAttenuScale + kViewToLightViewMatrix._m33 * fAttenuOffset;

			kViewToLightClipMatrix.SetCol(2, kViewAttenu);		///< ����Z����

			pkRenderDevice->SetGlobalShaderConstant("g_mViewToLightClipSpace", 
				sizeof(kViewToLightClipMatrix), 
				&kViewToLightClipMatrix);

			// ����˥�����������ø���ɫ��
			sdVector4 kAttenuParam(-1.f / fAttenuSoftness, 1.f / fAttenuSoftness, 0.f, 0.f);
			pkRenderDevice->SetGlobalShaderConstant("g_vLocalLightAttenuParams", 
				sizeof(kAttenuParam), 
				&kAttenuParam);
		}
	}

	NiColor kAmbientColor = m_pkLight->GetAmbientColor() * m_pkLight->GetDimmer();
	float fAccum = kAmbientColor.r + kAmbientColor.g + kAmbientColor.b - 0.001f;	///< ???����???
	NiColorA kLocalLightShadow(kAmbientColor.r, kAmbientColor.g, kAmbientColor.b, fAccum);
	pkRenderDevice->SetGlobalShaderConstant("g_vLocalLightShadow", 
		sizeof(kLocalLightShadow), 
		&kLocalLightShadow);

	NiColor kDiffuseColor = m_pkLight->GetDiffuseColor() * m_pkLight->GetDimmer();
	pkRenderDevice->SetGlobalShaderConstant("g_vLocalLightColor", 
		sizeof(kDiffuseColor), 
		&kDiffuseColor);

	const sdMatrix4& kViewMatrix = pkRenderDevice->GetViewMatrix();
	const NiPoint3& kLightPosition = m_pkLight->GetPosition();
	sdVector4 kWorldPosition(kLightPosition.x, kLightPosition.y, kLightPosition.z, 1.f);
	sdVector4 kViewPosition;
	sdMath::Mult(kViewPosition, kWorldPosition, kViewMatrix);
	pkRenderDevice->SetGlobalShaderConstant("g_vLocalLightViewPos", 
		sizeof(sdVector4), 
		&kViewPosition);
}
//-------------------------------------------------------------------------------------------------
void sdLightVolumeRenderObject::RestoreLightSettings()
{
	m_pkMesh->SetWorldTranslate(NiPoint3::ZERO);
	m_pkMesh->SetWorldRotate(NiMatrix3::IDENTITY);
	m_pkMesh->SetWorldScale(1.f);
}
//-------------------------------------------------------------------------------------------------
void sdLightVolumeRenderObject::Draw()
{
	NIASSERT(m_pkLight);
	NIASSERT(m_pkMesh);

	ApplyRenderSetting();
	ApplyLightSettings();

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiMaterial* pkFlatMaterial = pkRenderSystem->GetDefaultMaterial(IRenderSystem::E_DM_STATIC_MESH_FLAT_SHADING);
	NIASSERT(pkFlatMaterial);

	NiMaterial* pkLightMaterial = const_cast<NiMaterial*>(m_pkMesh->GetActiveMaterial());
	NIASSERT(pkLightMaterial);


	// ������Ⱦ״̬(������������,����Դ���ǵĵط�ģ�嶼������Ϊ1��������Զ�Ķ�������Ϊ0)
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_GREATER);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_ALWAYS);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_INVERT);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		0x1 << 0x5);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		0x1 << 0x5);
	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	0x1 << 0x5);

	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	0x0);
	// @}


	// ����
	m_pkMesh->ApplyAndSetActiveMaterial(pkFlatMaterial);
	m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());


	// ������Ⱦ״̬(������������,����Ϊ1�ĵط�ģ�嶼����Ϊ0,��Դֵ������)
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_CCW);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_ZERO);

	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
	pkRenderDevice->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);
	pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);

	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	// @}


	// ����
	m_pkMesh->ApplyAndSetActiveMaterial(pkLightMaterial);
	m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());

	// ���Ƶƹ�������(����)
	bool bDebugShowLightVolume = true;
	if (bDebugShowLightVolume)
	{
		pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
		pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_WIREFRAME);
		pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
		pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_LESSEQUAL);
		pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		true);
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
		
		NiMaterial* pkWireframeMaterial = pkRenderSystem->GetDefaultMaterial(IRenderSystem::E_DM_STATIC_MESH_WIREFRAME_SHADING);
		NIASSERT(pkWireframeMaterial);

		NiMaterial* pkLightMaterial = const_cast<NiMaterial*>(m_pkMesh->GetActiveMaterial());
		NIASSERT(pkLightMaterial);

		m_pkMesh->ApplyAndSetActiveMaterial(pkWireframeMaterial);
		m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());
		m_pkMesh->ApplyAndSetActiveMaterial(pkLightMaterial);

		pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);
	}

	RestoreLightSettings();
	RestoreRenderSetting();
}
//-------------------------------------------------------------------------------------------------
sdProjectorVolumeRenderObject::sdProjectorVolumeRenderObject(sdProjectorSceneObject* pkProjector, NiMaterial* spMaterial, 
	NiPropertyList* spRenderProps, ShaderParamItem* pkShaderParams)
: sdRenderObject(E_ROT_LIGHT_VOLUME, FindVolumeMeshForProjector(pkProjector), spMaterial, spRenderProps, pkShaderParams)
, m_pkProjector(pkProjector)
{

}
//-------------------------------------------------------------------------------------------------
NiMesh*	sdProjectorVolumeRenderObject::FindVolumeMeshForProjector(sdProjectorSceneObject* pkProjector)
{
	NIASSERT(pkProjector);

	IRenderSystem* pkRenderSystem =	IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	return pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_UNIT_CUBE);
}
//-------------------------------------------------------------------------------------------------
void sdProjectorVolumeRenderObject::ApplyTexturingProp()
{
	// ����Meshԭ�е�NiTexturingProperty
	NiPropertyState* pkMeshPropertyState = m_pkMesh->GetPropertyState();
	if (!pkMeshPropertyState)
		return;

	NiTexturingProperty* pkMeshTexturingProp = pkMeshPropertyState->GetTexturing();
	if (!pkMeshTexturingProp)	
		return;

	// �����µ�NiTexturingProperty
	NiProperty* pkProperty = FindProperty(m_pkProperties, NiProperty::TEXTURING);
	if (!pkProperty)
		return;

	NiTexturingProperty* pkTexturingProp = (NiTexturingProperty*)pkProperty;
	if (!pkTexturingProp)	
		return;

	// ������ͼ
	if (m_pkProjector->GetReceiveLighting())
	{
		NiTexturingProperty::Map* pkBaseMap = pkTexturingProp->GetBaseMap();
		if (pkBaseMap)
			pkBaseMap->SetTexture(m_pkProjector->GetDiffuseMap());

		NiTexturingProperty::Map* pkNormalMap = pkTexturingProp->GetNormalMap();
		if (pkNormalMap)
			pkNormalMap->SetTexture(m_pkProjector->GetNormalMap());
	}
	else
	{
		NiTexturingProperty::Map* pkGlowMap = pkTexturingProp->GetGlowMap();
		if (pkGlowMap)
			pkGlowMap->SetTexture(m_pkProjector->GetDiffuseMap());
	}

	NiTexturingProperty::Map* pkDarkMap = pkTexturingProp->GetDarkMap();
	if (pkDarkMap)
		pkDarkMap->SetTexture(m_pkProjector->GetFilterMap());

	// ����Mesh��ԭ��NiTexturingProperty���������µ�NiTexturingProperty��Mesh
	m_spOldTexturingProp = pkMeshTexturingProp;		
	pkMeshPropertyState->SetProperty(pkTexturingProp);
}
//-------------------------------------------------------------------------------------------------
void sdProjectorVolumeRenderObject::RestoreTexturingProp()
{
	// �ָ�Mesh��NiTexturingProperty
	NiPropertyState* pkMeshPropertyState = m_pkMesh->GetPropertyState();
	if (!pkMeshPropertyState)
		return;

	pkMeshPropertyState->SetProperty(m_spOldTexturingProp);	
	m_spOldTexturingProp = 0;

	// ������Ⱦ�ĵ�NiTexturingProperty,���ָ�����
	NiProperty* pkProperty = FindProperty(m_pkProperties, NiProperty::TEXTURING);
	if (!pkProperty)
		return;

	NiTexturingProperty* pkTexturingProp = (NiTexturingProperty*)pkProperty;
	if (!pkTexturingProp)	
		return;

	NiTexturingProperty::Map* pkBaseMap = pkTexturingProp->GetBaseMap();
	if (pkBaseMap)
		pkBaseMap->SetTexture(NULL);

	NiTexturingProperty::Map* pkNormalMap = pkTexturingProp->GetNormalMap();
	if (pkNormalMap)
		pkNormalMap->SetTexture(NULL);

	NiTexturingProperty::Map* pkGlowMap = pkTexturingProp->GetGlowMap();
	if (pkGlowMap)
		pkGlowMap->SetTexture(NULL);

	NiTexturingProperty::Map* pkDarkMap = pkTexturingProp->GetDarkMap();
	if (pkDarkMap)
		pkDarkMap->SetTexture(NULL);
}
//-------------------------------------------------------------------------------------------------
void sdProjectorVolumeRenderObject::ApplyProjectorSettings()
{
	NIASSERT(m_pkProjector);
	NIASSERT(m_pkMesh);

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);	

	const NiPoint3& kGBPosition = m_pkProjector->GetPosition();
	const NiPoint3& kGBDirection = m_pkProjector->GetDirection();
	const NiPoint3& kGBUp = m_pkProjector->GetUpDirection();
	const NiPoint3& kGBSize = m_pkProjector->GetSize();
	const float fLength = m_pkProjector->GetLength();


	// ����Ͷ����ģ�͵ı任
	// @{
	// ģ�Ϳռ�����ž���(��ģ�Ϳռ�任�����ߴ�)			
	sdMatrix3 kScaleMatrix(
		kGBSize.x,	0.f,		0.f,
		0.f,		kGBSize.y,	0.f,
		0.f,		0.f,		kGBSize.z);

	// ͶӰ��������������(ǰ��,�Ϸ�,�ҷ�)
	sdVector3 kDirection(kGBDirection.x, kGBDirection.y, kGBDirection.z);
	kDirection.Normalise();

	sdVector3 kUp(kGBUp.x, kGBUp.y, kGBUp.z);								
	kUp.Normalise();

	sdVector3 kRight;			
	sdMath::Cross(kRight, kUp, kDirection);
	kRight.Normalise();

	sdMath::Cross(kUp, kDirection, kRight);		

	// ����ռ䵽ģ�Ϳռ����ת����(ģ�Ϳռ䣺����ΪX������ΪY������ΪZ)
	sdMatrix3 kWorldToModelRotationMatrix(
		kRight.m_fX,	kUp.m_fX,	kDirection.m_fX,
		kRight.m_fY,	kUp.m_fY,	kDirection.m_fY,
		kRight.m_fZ,	kUp.m_fZ,	kDirection.m_fZ);

	// ���յ�����ģ����ת���ž���
	sdMatrix3 kWorldToModelScaleRotationMatrix;
	sdMath::Mult(kWorldToModelScaleRotationMatrix, kWorldToModelRotationMatrix, kScaleMatrix);

	// ���ø�Mesh����
	NiMatrix3 kGBRotate;
	kGBRotate.SetCol(0, kWorldToModelScaleRotationMatrix._m00, kWorldToModelScaleRotationMatrix._m10, kWorldToModelScaleRotationMatrix._m20);
	kGBRotate.SetCol(1, kWorldToModelScaleRotationMatrix._m01, kWorldToModelScaleRotationMatrix._m11, kWorldToModelScaleRotationMatrix._m21);
	kGBRotate.SetCol(2, kWorldToModelScaleRotationMatrix._m02, kWorldToModelScaleRotationMatrix._m12, kWorldToModelScaleRotationMatrix._m22);

	m_pkMesh->SetWorldTranslate(kGBPosition);
	m_pkMesh->SetWorldRotate(kGBRotate);
	m_pkMesh->SetWorldScale(1.f);
	// @}


	//
	// ����۲�����ϵ��Ͷ������ռ�ı任��
	//
	//	1.����۲�ռ�(D3D�۲�����ϵ,��������ϵ)��
	//		ԭ��λ������۲��
	//		X������
	//		Y���µ���
	//		Z���ڵ���(�ӹ۲�㵽Ŀ���)
	//	
	//	2.Ͷ����۲�ռ�(�۲�����ϵ,��������ϵ)��
	//		ԭ��λ��Ͷ����۲��
	//		X������
	//		Y���µ���
	//		Z���⵽��(��Ŀ��㵽�۲��)
	//
	//	3.Ͷ����ü��ռ�(ͶӰ����ϵ,��������ϵ)��
	//		ԭ��λ�ڲü�����������
	//		X�����ң���Χ[-1,1]
	//		Y���µ��ϣ���Χ[-1,1]
	//		Z���⵽��, ��Χ[-1,1]
	//
	//	4.Ͷ��������ռ�(��������ϵ):
	//		ԭ��λ�����Ͻ�
	//		X�����ң���Χ[0,1]
	//		Y���ϵ��£���Χ[0,1]
	//		Z��ǰ���󣬷�Χ[0,1]
	//
	// @{
	const sdMatrix4& kWorldToViewMatrix = pkRenderDevice->GetViewMatrix();			///< ��������ϵ-�۲�����ϵ
	
	NiPoint3 kGBEyePosition = kGBPosition - kGBDirection * fLength * 0.5f;
	sdVector3 kEyePosition(kGBEyePosition.x, kGBEyePosition.y, kGBEyePosition.z);
	sdVector3 kViewEyePosition;
	sdMath::TransformPosition(kViewEyePosition,										///< ͶӰ��۲�����
		kEyePosition, 
		kWorldToViewMatrix);	

	NiPoint3 kGBTargetPosition = kGBPosition + kGBDirection * fLength * 0.5f;
	sdVector3 kTargetPosition(kGBTargetPosition.x, kGBTargetPosition.y, kGBTargetPosition.z);
	sdVector3 kViewTargetPosition;
	sdMath::TransformPosition(kViewTargetPosition,									///< ͶӰĿ���۲�����
		kTargetPosition, 
		kWorldToViewMatrix);	

	sdVector3 kViewUp;
	sdMath::TransformDirection(kViewUp, kUp, kWorldToViewMatrix);					///< �۲�ռ����Ϸ���

	sdMatrix4 kViewToProjectorViewMatrix;				
	sdMath::LookAtLHToRH(kViewToProjectorViewMatrix,	///< ����۲�����ϵ-Ͷ����۲�����ϵ
		kViewEyePosition,								///< 
		kViewTargetPosition,							///< 
		kViewUp);										///< 

	sdMatrix4 kProjectorViewToProjMatrix;				
	sdMath::Ortho(kProjectorViewToProjMatrix,	///< Ͷ����۲�����ϵ-Ͷ����ͶӰ����ϵ
		-kGBSize.x * 0.5f, kGBSize.x * 0.5f,	///< X��[-0.5x,0.5x]��[-1,1]
		-kGBSize.y * 0.5f, kGBSize.y * 0.5f,	///< Y��[-0.5y,0.5y]��[-1,1]
		-kGBSize.z * 0.1f, -kGBSize.z);			///< Z��[0,-z]��[-1,1]

	sdMatrix4 kClipToProjectorUVMatrix(			///< Ͷ����ͶӰ����ϵ-Ͷ������������ϵ
		0.5f,	0.f,	0.f,	0.f,			///< X��[-1,1]��[0,1]
		0.f,	-0.5f,	0.f,	0.f,			///< Y��[-1,1]��[0,1](��ת)
		0.f,	0.f,	-0.5f,	0.f,			///< Z��[-1,1]��[0,1](��ת)
		0.5f,	0.5f,	0.5f,	1.f);

	sdMatrix4 kTemp;
	sdMatrix4 kViewToProjectorUVMatrix;
	sdMath::Mult(kTemp, kViewToProjectorViewMatrix, kProjectorViewToProjMatrix);
	sdMath::Mult(kViewToProjectorUVMatrix, kTemp, kClipToProjectorUVMatrix);

	pkRenderDevice->SetGlobalShaderConstant("g_mViewToProjectorUV", 
		sizeof(sdMatrix4), 
		&kViewToProjectorUVMatrix);
	// @}


	sdVector3 kViewDirection;
	sdMath::TransformDirection(kViewDirection, kDirection, kWorldToViewMatrix);
	pkRenderDevice->SetGlobalShaderConstant("g_vProjectorDirection", 
		sizeof(sdVector3), 
		&kViewDirection);

	const NiPoint2& kGBUVScale = m_pkProjector->GetUVScale();
	pkRenderDevice->SetGlobalShaderConstant("g_vProjectorUVScale", 
		sizeof(NiPoint2), 
		&kGBUVScale);

	const NiPoint2& kGBUVOffset = m_pkProjector->GetUVOffset();
	pkRenderDevice->SetGlobalShaderConstant("g_vProjectorUVOffset", 
		sizeof(NiPoint2), 
		&kGBUVOffset);
	
	const NiPoint2& kGBUVSpeed = m_pkProjector->GetUVSpeed();
	pkRenderDevice->SetGlobalShaderConstant("g_vProjectorUVSpeed", 
		sizeof(NiPoint2), 
		&kGBUVSpeed);

	const NiColor& kDiffuseColor = m_pkProjector->GetDiffuseMaterial();
	const float fAlpha = m_pkProjector->GetAlpha();
	NiColorA kDiffuseColorA(kDiffuseColor.r, kDiffuseColor.g, kDiffuseColor.b, fAlpha);
	pkRenderDevice->SetGlobalShaderConstant("g_vDiffuseMaterial", 
		sizeof(NiPoint3), 
		&kDiffuseColorA);

	const NiColor& kSpeculareColor = m_pkProjector->GetSpecularMaterial();
	const float fShiness = m_pkProjector->GetShiness();
	NiColorA kSpeculareColorA(kSpeculareColor.r, kSpeculareColor.g, kSpeculareColor.b, fShiness);
	pkRenderDevice->SetGlobalShaderConstant("g_vSpecularMaterial", 
		sizeof(NiPoint3), 
		&kSpeculareColorA);

	float fAlphaTest = m_pkProjector->GetAlphaTest();
	pkRenderDevice->SetGlobalShaderConstant("g_fAlphaTestRef", 
		sizeof(float), 
		&fAlphaTest);

	//
	if (m_pkProjector->GetBlendType() == sdProjectorSceneObject::E_BT_NONE)
	{
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	else if (m_pkProjector->GetBlendType() == sdProjectorSceneObject::E_BT_ALPHABLEND) 
	{
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
		pkRenderDevice->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD);
		pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA);
		pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA);
	}
	else if (m_pkProjector->GetBlendType() == sdProjectorSceneObject::E_BT_ADD) 
	{
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
		pkRenderDevice->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD);
		pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA);
		pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);
	}
}
//-------------------------------------------------------------------------------------------------
void sdProjectorVolumeRenderObject::RestoreProjectorSettings()
{
	m_pkMesh->SetWorldTranslate(NiPoint3::ZERO);
	m_pkMesh->SetWorldRotate(NiMatrix3::IDENTITY);
	m_pkMesh->SetWorldScale(1.f);
}
//-------------------------------------------------------------------------------------------------
void sdProjectorVolumeRenderObject::Draw()
{
	NIASSERT(m_pkProjector);
	NIASSERT(m_pkMesh);

	ApplyRenderSetting();
	ApplyProjectorSettings();

	// ����
	m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());

	// ������������(����)
	bool bDebugShowProjectorVolume = true;
	if (bDebugShowProjectorVolume)
	{
		IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
		NIASSERT(pkRenderSystem);

		IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
		NIASSERT(pkRenderDevice);	

		pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
		pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_WIREFRAME);
		pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
		pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_LESSEQUAL);
		pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		true);
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);

		NiMaterial* pkWireframeMaterial = pkRenderSystem->GetDefaultMaterial(IRenderSystem::E_DM_STATIC_MESH_WIREFRAME_SHADING);
		NIASSERT(pkWireframeMaterial);

		NiMaterial* pkDecalMaterial = const_cast<NiMaterial*>(m_pkMesh->GetActiveMaterial());
		NIASSERT(pkDecalMaterial);

		m_pkMesh->ApplyAndSetActiveMaterial(pkWireframeMaterial);
		m_pkMesh->RenderImmediate(NiRenderer::GetRenderer());
		m_pkMesh->ApplyAndSetActiveMaterial(pkDecalMaterial);

		pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);
	}

	RestoreProjectorSettings();
	RestoreRenderSetting();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
