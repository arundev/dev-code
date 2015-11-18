//*************************************************************************************************
// 内容:	公共头文件(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:	2013-05-02
//*************************************************************************************************

//---------------------------------------------------------------------------------------
// 灯光
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 环境光
//---------------------------------------------------------------------------------------
float4	g_vAmbientAmbient0	: AMBIENTLIGHT;

//---------------------------------------------------------------------------------------
// 直射光0
//---------------------------------------------------------------------------------------
float3	g_vDirWorldDirection0	: DIRECTION 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 0;
>;

float4	g_vDirAmbient0	: AMBINET 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 0;
>;

float4	g_vDirDiffuse0	: DIFFUSE 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 0;
>;

float3	g_vDirSpecular0	: SPECULAR 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 0;
>;

//---------------------------------------------------------------------------------------
// 直射光1
//---------------------------------------------------------------------------------------
float3	g_vDirWorldDirection1	: DIRECTION 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 1;
>;

float4	g_vDirAmbient1	: AMBINET 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 1;
>;

float4	g_vDirDiffuse1	: DIFFUSE 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 1;
>;

float3	g_vDirSpecular1	: SPECULAR 
<
	string	Object = "DirectionalLight";
	int ObjectIndex = 1;
>;

//---------------------------------------------------------------------------------------
// 材质
//---------------------------------------------------------------------------------------
float4 	g_vMaterialEmissive	: MATERIALEMISSIVE;
float4	g_vMaterialAmbient	: MATERIALAMBINET;
float4	g_vMaterialDiffuse	: MATERIALDIFFUSE;
float4	g_vMaterialSpecular	: MATERIALSPECULAR;

//---------------------------------------------------------------------------------------
// 光照计算
//---------------------------------------------------------------------------------------
void CaculateDirectionalLightContribution(
	float3 vInWorldPos, float3 vInWorldNrm, float3 vInWorldViewVec, float fInSpecPow, float fInShadow, 
	float4 vInMatAmbient, float4 vInMatDiffuse, float4 vInMatSpecular,
	float3 vInLightDir, float3 vInLightAmbient, float3 vInLightDiffuse, float3 vInLightSpecular, float fInLightAtten,
	out float3 vOutDiffuse, out float3 vOutSpecular)
{
	float3 N = vInWorldNrm;
	float3 L = normalize(-vInLightDir);
	float NL = max(dot(L, N), 0.f);
	float3 vDiffuse = vInMatDiffuse.rgb * lerp(vInLightAmbient, vInLightDiffuse, NL * fInLightAtten);
	
	float3 R = normalize(2.f * dot(N, L) * N - L);
	float3 V = normalize(vInWorldViewVec - vInWorldPos);
	float3 vSpecular = vInMatSpecular.rgb * pow(max(dot(R, V), 0.f), fInSpecPow) * vInLightSpecular;
	
	vOutDiffuse = vDiffuse;
	vOutSpecular = vSpecular;
}
//---------------------------------------------------------------------------------------