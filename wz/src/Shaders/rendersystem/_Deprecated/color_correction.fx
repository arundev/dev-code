//*************************************************************************************************
// 内容:	后处理颜色调整
//	
//	Color Correction and desturation
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-13
// 最后修改:
//*************************************************************************************************

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4 g_vSceneShadowsAndDesaturation 	: GLOBAL;	// RGB = SceneShadows, A = (1-SceneDesaturation)
float4 g_vSceneInverseHightLights 		: GLOBAL;	// RGB = 1 / SceneHightLights
float4 g_vSceneMidTones					: GLOBAL;	// RGB = SceneMidTones
float4 g_vSceneScaledLuminanceWeights	: GLOBAL;	// RGB = uminanceWeights * Desaturation

static float g_fConstSmallPow = 0.00000001f;

//---------------------------------------------------------------------------------------
// 
half3 ColorCorrection(half3 vInLDRColor)
{
	half3 vColor = pow(max(saturate(vInLDRColor - g_vSceneShadowsAndDesaturation.rgb) * 
			g_vSceneInverseHightLights.rgb, g_fConstSmallPow), g_vSceneMidTones.rgb);
		
	half3 vScaledLuminance = dot(vColor.rgb, g_vSceneScaledLuminanceWeights.rgb);
	half3 vMaterialResult = /*GammaOverlayColor + */ vColor * g_vSceneShadowsAndDesaturation.a + vScaledLuminance;
	
	// Gamma correction
	// -note that for some reason, HLSL generates better code when we add a color to
	// 	the vMaterialResult, so I changed the code so that we add 
	//	vGammaOverlayColor / vGmmaColorScale to the vMaterialResult as opposed to adding 
	// 	vGammaOverlayColor to the vLinearColor
	half3 vLinearColor = vMaterialResult.rgb /** vGmmaColorScale.rgb */;
	
	return vLinearColor;
}
//---------------------------------------------------------------------------------------
