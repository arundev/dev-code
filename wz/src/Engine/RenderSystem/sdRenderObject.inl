//-------------------------------------------------------------------------------------------------
const sdRenderObject::ERenderObjectType sdRenderObject::GetType() const
{ 
	return m_eType;
}
//-------------------------------------------------------------------------------------------------
const sdRenderObject::RenderSortKey& sdRenderObject::GetSortKey() const 
{ 
	return m_kRenderKey;
}
//-------------------------------------------------------------------------------------------------
const NiMesh* sdRenderObject::GetMesh()	const 
{ 
	return m_pkMesh;
}
//-------------------------------------------------------------------------------------------------