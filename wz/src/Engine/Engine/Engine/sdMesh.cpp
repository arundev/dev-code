#include "sdEnginePCH.h"
#include "sdMesh.h"
/*
NiImplementRTTI(sdMesh, NiMesh);
NiImplementCreateClone(sdMesh);
NiImplementCreateObject(sdMesh);
//-------------------------------------------------------------------------------------------------
void NiMesh::CopyMembers(NiMesh* pkDest, NiCloningProcess& kCloning)
{
	pkDest->SetPrimitiveType(m_eMeshPrimitiveType);
	pkDest->m_uiSubmeshCount = m_uiSubmeshCount;
	//clone stream refs before copy members.
	pkDest->CloneStreamRefs(this, &kCloning);
	NiRenderObject::CopyMembers(pkDest, kCloning);
	pkDest->SetInstanced(m_bInstancingEnabled);
	pkDest->SetModelBound(m_kBound);


	// Clone the modifiers. Don't add them yet - do that in ProcessClone
	// when all clones are known to have been generated.
	for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
	{
		NiMeshModifier* pkModifier = GetModifierAt(ui);

		// The clone will be stored in the clone map for later retrieval
		pkModifier->CreateClone(kCloning);
	}
}
//---------------------------------------------------------------------------
void NiMesh::ProcessClone(NiCloningProcess& kCloning)
{
	NiRenderObject::ProcessClone(kCloning);

	NiObject* pkClone = NULL;
	bool bCloned = kCloning.m_pkCloneMap->GetAt(this, pkClone);
	NIASSERT(bCloned && pkClone->IsKindOf(GetRTTI()));
	if (bCloned && pkClone->IsKindOf(GetRTTI()))
	{
		NiMesh* pkMesh = (NiMesh*) pkClone;

		for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
		{
			NiMeshModifier* pkModifier = GetModifierAt(ui);
			pkModifier->ProcessClone(kCloning);

			NIVERIFY(kCloning.m_pkCloneMap->GetAt(pkModifier, pkClone));
			NiMeshModifier* pNewModifierPtr = (NiMeshModifier*)pkClone;
			pkMesh->AddModifier(pNewModifierPtr, true);
		}
	}

	UpdateCachedPrimitiveCount();
}
//---------------------------------------------------------------------------
void NiMesh::CloneStreamRefs(NiMesh* pkOriginal, NiCloningProcess* pkCloning)
{
	bool bLocalCloningProcess = false;
	if (pkCloning == NULL)
	{
		pkCloning = NiNew NiCloningProcess;
		bLocalCloningProcess = true;
	}

	// Make the internal array the size of the original's array.
	// This will keep unnecessary allocations to a minimum.
	m_kDataStreams.Realloc(pkOriginal->m_kDataStreams.GetSize());

	NiUInt32 ui = 0;
	for (; ui < pkOriginal->m_kDataStreams.GetSize(); ui++)
	{
		// Use the copy constructor to copy all relevant data
		NiDataStreamRef* pkRef = NiNew NiDataStreamRef(*
			pkOriginal->m_kDataStreams.GetAt(ui));
		m_kDataStreams.Add(pkRef);

		// Now override the datastreams with their cloned counterparts
		NiDataStream* pkDS = pkRef->GetDataStream();

		if (pkDS)
		{
			pkDS = (NiDataStream*)pkDS->CreateClone(*pkCloning);
			pkRef->SetDataStream(pkDS);
		}
	}

	// Clone output data streams as well
	const NiUInt32 uiOutputStreamMaterialCount = 
		pkOriginal->m_kOutputDataStreamsByMaterial.GetSize();
	m_kOutputDataStreamsByMaterial.Realloc(uiOutputStreamMaterialCount);
	for (ui = 0; ui < uiOutputStreamMaterialCount; ui++)
	{
		OutputStream* pkOrigOutputStream = 
			pkOriginal->m_kOutputDataStreamsByMaterial.GetAt(ui);
		NIASSERT(pkOrigOutputStream);

		OutputStream* pkNewOutputStream = OutputStream::CloneOutputStream(
			pkOrigOutputStream, *pkCloning);
		m_kOutputDataStreamsByMaterial.Add(pkNewOutputStream);
	}

	if (bLocalCloningProcess)
	{
		NiDelete pkCloning;
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// streaming
//---------------------------------------------------------------------------
void NiMesh::LoadBinary(NiStream& kStream)
{
	NiRenderObject::LoadBinary(kStream);

	// Load mesh primitive type
	NiStreamLoadEnum(kStream, m_eMeshPrimitiveType);

	// Load the submesh count
	NiStreamLoadBinary(kStream, m_uiSubmeshCount);

	// Load instancing parameters
	NiBool kBool;
	NiStreamLoadBinary(kStream, kBool);
	m_bInstancingEnabled = NIBOOL_IS_TRUE(kBool);

	// Load mesh bounds
	m_kBound.LoadBinary(kStream);

	// Load DataStreamRefs
	StreamRefsLoadBinary(kStream);

	// Load Modifiers
	kStream.ReadMultipleLinkIDs();
}
//---------------------------------------------------------------------------
void NiMesh::LinkObject(NiStream& kStream)
{
	NiRenderObject::LinkObject(kStream);

	// Link modifiers
	NiUInt32 uiModifierCount = kStream.GetNumberOfLinkIDs();
	for (NiUInt32 i = 0; i < uiModifierCount; i++)
	{
		NiMeshModifier* pkModifier = 
			(NiMeshModifier*)kStream.GetObjectFromLinkID();
		if (pkModifier)
			AddModifier(pkModifier, false);
	}
}
//---------------------------------------------------------------------------
void NiMesh::PostLinkObject(NiStream& kStream)
{
	NiRenderObject::PostLinkObject(kStream);

	if (m_bInstancingEnabled)
	{
		// Ensure the provided instancing data will work on the current
		// platform.
		NiInstancingUtilities::ValidateInstancingData(this);
	}

	// Prepares an NiMeshModifier on this NiMesh
	for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
	{
		NiMeshModifier* pkModifier = GetModifierAt(ui);
		AttachModifier(pkModifier);
	}

	UpdateCachedPrimitiveCount();

	if (ms_bPreload)
	{
		NiRenderer* pkRenderer = NiRenderer::GetRenderer();
		if (pkRenderer)
			pkRenderer->PrecacheMesh(this, ms_bDestroyAppData);
	}
}
//---------------------------------------------------------------------------
bool NiMesh::RegisterStreamables(NiStream& kStream)
{
	if (!NiRenderObject::RegisterStreamables(kStream))
		return false;

	if (!StreamRefsRegisterStreamables(kStream))
		return false;

	// Register modifiers
	for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
	{
		NiMeshModifier* pkModifier = GetModifierAt(ui);
		pkModifier->RegisterStreamables(kStream);
	}

	return true;
}
//---------------------------------------------------------------------------
void NiMesh::SaveBinary(NiStream& kStream)
{
	NiRenderObject::SaveBinary(kStream);

	// Save primitive type
	NiStreamSaveEnum(kStream, m_eMeshPrimitiveType);

	// Save submesh count
	NiStreamSaveBinary(kStream, m_uiSubmeshCount);

	// Save instancing parameters
	NiBool kBool = m_bInstancingEnabled;
	NiStreamSaveBinary(kStream, kBool);

	// Save bounds
	m_kBound.SaveBinary(kStream);

	// Save stream refs
	StreamRefsSaveBinary(kStream);

	// Save Modifiers
	NiStreamSaveBinary(kStream, (NiUInt32)GetModifierCount());
	for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
	{
		NiMeshModifier* pkModifier = GetModifierAt(ui);
		kStream.SaveLinkID(pkModifier);
	}
}
//---------------------------------------------------------------------------
void NiMesh::StreamRefsLoadBinary(NiStream& kStream)
{
	// Preallocate the datastreams
	NiUInt32 uiStreamRefCount = 0;
	NiStreamLoadBinary(kStream, uiStreamRefCount);
	m_kDataStreams.Realloc(uiStreamRefCount);

	// call LoadBinary on each stream ref
	for (NiUInt32 uiStreamRef = 0; uiStreamRef < uiStreamRefCount;
		uiStreamRef++)
	{
		NiDataStreamRef* pkStreamRef = NiNew NiDataStreamRef();
		pkStreamRef->LoadBinary(kStream);
		m_kDataStreams.Add(pkStreamRef);
	}

	// Output data streams are not streamed in or out
}
//---------------------------------------------------------------------------
bool NiMesh::StreamRefsRegisterStreamables(NiStream& kStream)
{
	// Register element usage references and data stream for each stream
	NiUInt32 uiStreamCount = m_kDataStreams.GetSize();
	for (NiUInt32 ui = 0; ui < uiStreamCount; ui++)
	{
		m_kDataStreams.GetAt(ui)->RegisterStreamables(kStream);
	}

	// Output data streams are not streamed in or out
	return true;
}
//---------------------------------------------------------------------------
void NiMesh::StreamRefsSaveBinary(NiStream& kStream)
{
	// Not all NiDataStream objects should be saved. Compute a count of how
	// many will be saved.
	NiUInt32 uiStreamRefCount = 0;
	for (NiUInt32 ui = 0; ui < m_kDataStreams.GetSize(); ++ui)
	{
		NiDataStreamRef* pkStreamRef = m_kDataStreams.GetAt(ui);
		NiDataStream* pkDataStream = pkStreamRef->GetDataStream();
		if (pkDataStream && pkDataStream->GetStreamable())
		{
			uiStreamRefCount++;
		}
	}
	NiStreamSaveBinary(kStream, uiStreamRefCount);

	for (NiUInt32 uiStreamRef = 0; uiStreamRef < m_kDataStreams.GetSize();
		uiStreamRef++)
	{
		NiDataStreamRef* pkStreamRef = m_kDataStreams.GetAt(uiStreamRef);

		NiDataStream* pkDataStream = pkStreamRef->GetDataStream();
		if (!pkDataStream || !pkDataStream->GetStreamable())
		{
			continue;
		}

		pkStreamRef->SaveBinary(kStream);
	}

	// Output data streams are not streamed in or out
}
//---------------------------------------------------------------------------
bool NiMesh::StreamRefsIsEqual(const NiMesh* pkOther) const
{
	if (pkOther->GetStreamRefCount() != GetStreamRefCount())
		return false;

	NiUInt32 ui = 0;
	for (; ui < m_kDataStreams.GetSize(); ++ui)
	{
		NiDataStreamRef* pkStreamRef = m_kDataStreams.GetAt(ui);
		NiDataStreamRef* pkOtherStreamRef = pkOther->m_kDataStreams.GetAt(ui);

		if (!pkStreamRef->IsEqual(pkOtherStreamRef))
			return false;
	}

	const NiUInt32 uiOutputStreamMaterialCount = 
		m_kOutputDataStreamsByMaterial.GetSize();

	if (uiOutputStreamMaterialCount != 
		pkOther->m_kOutputDataStreamsByMaterial.GetSize())
	{
		return false;
	}

	for (ui = 0; ui < uiOutputStreamMaterialCount; ui++)
	{
		const OutputStream* pkOutputStream = 
			m_kOutputDataStreamsByMaterial.GetAt(ui);
		const OutputStream* pkOtherOutputStream = 
			pkOther->m_kOutputDataStreamsByMaterial.GetAt(ui);

		if ((pkOutputStream == NULL) != (pkOtherOutputStream == NULL))
			return false;
		if (pkOutputStream && !pkOutputStream->IsEqual(pkOtherOutputStream))
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool NiMesh::IsInstancable(const NiMesh* pkTestMesh) const
{
	// Compare primitive type
	if (m_eMeshPrimitiveType != pkTestMesh->m_eMeshPrimitiveType)
		return false;

	// Compare submesh count
	if (m_uiSubmeshCount != pkTestMesh->m_uiSubmeshCount)
		return false;

	// Stream Output meshes can't be instanced
	if (m_bInputDataIsFromStreamOut || pkTestMesh->m_bInputDataIsFromStreamOut)
		return false;

	// Make sure we are not GPU skinned
	NiSkinningMeshModifier* pkSkinningModifier = (NiSkinningMeshModifier*)
		GetModifierByType(&NiSkinningMeshModifier::ms_RTTI);
	if (pkSkinningModifier)
	{
		if (!pkSkinningModifier->GetSoftwareSkinned())
			return false;
	}

	// Compare the data stream count
	if (pkTestMesh->GetStreamRefCount() != GetStreamRefCount())
		return false;

	// Compare the data streams
	for (NiUInt32 uiStreamRef = 0; uiStreamRef < m_kDataStreams.GetSize(); 
		uiStreamRef++)
	{
		NiDataStreamRef* pkStreamRef = m_kDataStreams.GetAt(uiStreamRef);

		bool bFound = false;
		for (NiUInt32 ui = 0; ui < m_kDataStreams.GetSize(); ui++)
		{
			NiDataStreamRef* pkOtherStreamRef = 
				pkTestMesh->m_kDataStreams.GetAt(ui);

			if (!pkStreamRef->IsEqual(pkOtherStreamRef))
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			return false;
	}

	// Compare output streams
	for (NiUInt32 uiOutputStreamMaterials = 0; 
		uiOutputStreamMaterials < m_kOutputDataStreamsByMaterial.GetSize(); 
		uiOutputStreamMaterials++)
	{
		OutputStream* pkOutputStream = m_kOutputDataStreamsByMaterial.GetAt(
			uiOutputStreamMaterials);

		bool bFound = false;
		for (NiUInt32 ui = 0; 
			ui < pkTestMesh->m_kOutputDataStreamsByMaterial.GetSize(); 
			ui++)
		{
			OutputStream* pkOtherOutputStream = 
				pkTestMesh->m_kOutputDataStreamsByMaterial.GetAt(ui);

			if (!pkOutputStream->IsEqual(pkOtherOutputStream))
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			return false;
	}

	// Compare material count
	NiUInt32 uiNumMaterials = m_kMaterials.GetSize();

	if (uiNumMaterials != pkTestMesh->m_kMaterials.GetSize())
		return false;

	// Compare materials
	for (NiUInt32 i = 0; i < uiNumMaterials; i++)
	{
		const NiMaterial* pkThisMaterial = m_kMaterials.GetAt(i).GetMaterial();
		const NiMaterial* pkThatMaterial = 
			pkTestMesh->m_kMaterials.GetAt(i).GetMaterial();

		if (pkThisMaterial != pkThatMaterial)
		{
			if (pkThisMaterial->GetName() != pkThatMaterial->GetName())
				return false;

			if (m_kMaterials.GetAt(i).GetMaterialExtraData() != 
				pkTestMesh->m_kMaterials.GetAt(i).GetMaterialExtraData())
			{
				return false;
			}
		}
	}

	// Compare material flags
	if (m_uiActiveMaterial != pkTestMesh->m_uiActiveMaterial ||
		m_bMaterialNeedsUpdateDefault != 
		pkTestMesh->m_bMaterialNeedsUpdateDefault)
	{
		return false;
	}

	// Compare properties
	for (NiUInt32 ui = 0; ui < NiProperty::MAX_TYPES; ui++)
	{
		NiProperty* pkThisProperties[NiProperty::MAX_TYPES];
		m_spPropertyState->GetProperties(pkThisProperties);

		NiProperty* pkTestProperties[NiProperty::MAX_TYPES];
		pkTestMesh->GetPropertyState()->GetProperties(pkTestProperties);

		if (!pkThisProperties[ui] && !pkTestProperties[ui])
			continue;

		if ((!pkThisProperties[ui] && pkTestProperties[ui]) ||
			(pkThisProperties[ui] && !pkTestProperties[ui]) ||
			(!pkThisProperties[ui]->IsEqual(pkTestProperties[ui])))
		{
			return false;
		}
	}

	// Compare effects
	for (NiUInt32 ui = 0; ui < NiDynamicEffect::MAX_TYPES; ui++)
	{
		NiDynamicEffectState* pkTestEffects = pkTestMesh->GetEffectState();
		if (!m_spEffectState && !pkTestEffects)
			continue;

		if ((!m_spEffectState && pkTestEffects) ||
			(m_spEffectState && !pkTestEffects) ||
			(!m_spEffectState->Equal(pkTestEffects)))
		{
			return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool NiMesh::IsEqual(NiObject* pkObject)
{
	if (!NiRenderObject::IsEqual(pkObject))
		return false;

	NiMesh* pkMesh = (NiMesh*) pkObject;

	// Compare primitive type
	if (m_eMeshPrimitiveType != pkMesh->m_eMeshPrimitiveType)
		return false;

	// Compare submesh count
	if (m_uiSubmeshCount != pkMesh->m_uiSubmeshCount)
		return false;

	// Compare instancing parameters
	if (m_bInstancingEnabled != pkMesh->m_bInstancingEnabled)
		return false;

	// Compare IsStreamOutput setting
	if (m_bInputDataIsFromStreamOut != pkMesh->m_bInputDataIsFromStreamOut)
		return false;

	if (m_pkBaseInstanceStream)
	{
		if (!pkMesh->m_pkBaseInstanceStream)
		{
			return false;
		}
		if (!m_pkBaseInstanceStream->IsEqual(pkMesh->m_pkBaseInstanceStream))
		{
			return false;
		}
	}
	else
	{
		if (pkMesh->m_pkBaseInstanceStream)
		{
			return false;
		}
	}

	if (m_pkVisibleInstanceStream)
	{
		if (!pkMesh->m_pkVisibleInstanceStream)
		{
			return false;
		}
		if (!m_pkVisibleInstanceStream->
			IsEqual(pkMesh->m_pkVisibleInstanceStream))
		{
			return false;
		}
	}
	else
	{
		if (pkMesh->m_pkVisibleInstanceStream)
		{
			return false;
		}
	}

	// Compare mesh data
	if (!StreamRefsIsEqual(pkMesh))
		return false;

	// Compare bounds
	if (m_kBound != pkMesh->m_kBound)
		return false;

	// Compare modifiers
	if (GetModifierCount() != pkMesh->GetModifierCount())
		return false;

	for (NiUInt32 ui = 0; ui < GetModifierCount(); ui++)
	{
		NiMeshModifier* pkModifier = GetModifierAt(ui);
		NiMeshModifier* pkOtherModifier = pkMesh->GetModifierAt(ui);
		if ((pkModifier && !pkOtherModifier) ||
			(!pkModifier && pkOtherModifier) ||
			(pkModifier && pkOtherModifier &&
			!pkModifier->IsEqual(pkOtherModifier)))
		{
			return false;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void NiMesh::GetViewerStrings(NiViewerStringsArray* pkStrings)
{
	NiRenderObject::GetViewerStrings(pkStrings);
	pkStrings->Add(NiGetViewerString(NiMesh::ms_RTTI.GetName()));
	pkStrings->Add(NiGetViewerString(
		"Primitive Type", 
		GetPrimitiveTypeString()));
}
//-------------------------------------------------------------------------------------------------
*/