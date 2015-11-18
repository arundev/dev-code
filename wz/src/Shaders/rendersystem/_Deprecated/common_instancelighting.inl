//*************************************************************************************************
// 内容:	公共头文件(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-22
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

float3	g_vWidgetMaterial	: GLOBAL = float3(1.f, 1.f, 1.f);

//---------------------------------------------------------------------------------------
// 计算
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 竹子动画
//---------------------------------------------------------------------------------------
float3 BambooAnimation(float3 vWorldPos, float3 vVertexPos)
{
#define MAX_H_B		10.f
#define	MAX_BIAS_B	0.35f

	float fZ = (vVertexPos.z - vWorldPos.z) / MAX_H_B;
	float xx = sin(g_fTime * 0.4f);
	float yy = cos(g_fTime * 0.337f);
	float3 vBias = float3(xx, yy, 0) * fZ * MAX_BIAS_B;
	return vVertexPos + vBias;
}
//---------------------------------------------------------------------------------------
// 竹子光照
//---------------------------------------------------------------------------------------
void BambooLighting(out float4 vDA, out float4 vSpecular, float3 vWorldNormal, float3 vWorldPos)
{
	vDA = vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	
	float3 N = vWorldNormal;
	float3 V = normalize(g_vCameraPosition.xyz - vWorldPos);
	float3 L = 0.f;
	float NL = 0.f;
	float HL = 0.f;
	float NH = 0.f;
	float fLightIntensity = 0.f;
	float fLightSpecIntensity = 0.f;
	float3 vAmbientColor = 0.f;
	float3 vDiffuseColor = 0.f;
	float3 vSpecularColor = 0.f
	
	// dir0
	L = -normalize(g_vDirWorldDirection0);
	NL = dot(L, N);
	
	fLightIntensity = max(NL, 0.f);
	vAmbientColor = g_vDirAmbinet0.rgb * g_vMaterialAmbient.rgb + g_vAmbientAmbient0.rgb;
	vDiffuseColor = g_vDirDiffuse0.rgb * g_vMaterialDiffuse.rgb * fLightIntensity + g_vAmbientAmbient0.rgb;
	
	//HL = normalize(L + V);
	//NH = max(dot(N, HL), 0.f);
	//fLightSpecIntensity = pow(NH, 15.f);
	//vSpecularColor = g_vDirSpecular0.rgb * float3(0.8f, 0.8f, 0.8f) * fLightSpecIntensity + g_vAmbientAmbient0.rgb;
	
	vDA += (vAmbientColor + vDiffuseColor);
	//vSpecular += vSpecularColor;
	
	// dir1
	L = -normalize(g_vDirWorldDirection1);
	NL = dot(L, N);
	
	fLightIntensity = max(NL, 0.f);
	vAmbientColor = g_vDirAmbinet1.rgb * g_vMaterialAmbient.rgb;
	vDiffuseColor = g_vDirDiffuse1.rgb * g_vMaterialDiffuse.rgb * fLightIntensity;
	
	HL = normalize(L + V);
	NH = max(dot(N, HL), 0.f);
	fLightSpecIntensity = pow(NH, 30.f);
	vSpecularColor = g_vDirSpecular1.rgb * g_vMaterialSpecular.rgb * fLightSpecIntensity;
	
	vDA.rgb += (vAmbientColor + vDiffuseColor);
	vSpecular.rgb += vSpecularColor;
}
//---------------------------------------------------------------------------------------
// 草光照
//---------------------------------------------------------------------------------------
void GrassLighting(out float4 vDA, out float4 vSpecular, float3 vWorldNormal, float3 vWorldPos)
{
	vDA = vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	
	float3 N = vWorldNormal;
	float3 L = -normalize(float3(1.f, 1.f, -1.f));
	float NL = dot(L, N);
	
	float fLightIntensity = max(NL, 0.f);
	float3 vAmbient = (g_vDirAmbinet1.rgb * g_vMaterialAmbient.rgb + g_vAmbientAmbient0.rgb) * g_vWidgetMaterial;
	float3 vDiffuse = (g_vDirDiffuse1.rgb * g_vMaterialDiffuse.rgb * fLightIntensity + g_vAmbientAmbient0.rgb * 3.f) * g_vWidgetMaterial;
	
	vDA += (vAmbient + vDiffuse);
}
//---------------------------------------------------------------------------------------