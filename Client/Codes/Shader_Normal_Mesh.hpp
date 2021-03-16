#include "Shader_Base.hpp"

texture				g_DTexture;
texture				g_NTexture;
texture				g_STexture;
texture				g_CTexture;

vector				g_vCameraPos; // In.World

vector				g_vLightDir;
vector				g_vLightSpecular;
vector				g_vLightDiffuse;
vector				g_vLightAmbient;

float				g_fTime = 0.f;
float				g_fDyingTime = 0.f;
int					g_iPhase = 0;


sampler	DSampler = sampler_state
{
	texture = g_DTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler	NSampler = sampler_state
{
	texture = g_NTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler	SSampler = sampler_state
{
	texture = g_STexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler	CSampler = sampler_state
{
	texture = g_CTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN
{
	float4	m_vPosition : POSITION;
	float3  m_Normal : NORMAL;
	float2	m_TexUV : TEXCOORD0;
	float3  m_tangent : TANGENT;
	float3  m_Binormal : BINORMAL;
	
};

struct VS_OUT
{
	float4	m_vPosition : POSITION;
	float2	m_TexUV : TEXCOORD0;
	float3	m_LightDir : TEXCOORD1;
	float3  m_ViewDir : TEXCOORD2;
	float3  T : TEXCOORD3;
	float3  B : TEXCOORD4;
	float3  N : TEXCOORD5;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.m_vPosition = mul(vector(In.m_vPosition.xyz, 1.f), matWVP);
	Out.m_TexUV = In.m_TexUV;

	float4 worldPos = mul(In.m_vPosition, g_matWorld);

	Out.m_LightDir = g_vLightDir.xyz;

	float3 viewDir = normalize(worldPos.xyz - g_vCameraPos.xyz);
	Out.m_ViewDir = viewDir;

	float3 vNormal = In.m_Normal;

	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 vBinormal = float3(vNormal.y, -vNormal.x, 0.f);

	float3 worldBiNormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBiNormal);

	float3 vTangent = cross(In.m_Normal, vBinormal);

	float3 worldTangent = mul(In.m_tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	/*
	float3 vNormal = In.m_Normal;
	float3 vTangent = In.m_tangent;
	float3 vBinormal = cross(In.m_Normal, In.m_tangent);

	float3 worldNormal = mul(In.m_Normal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 worldTangent = mul(In.m_tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 worldBiNormal = mul(In.m_Binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBiNormal);*/

	return Out;

}

struct PS_IN
{	
	float2	m_TexUV		: TEXCOORD0;
	float3	m_LightDir	: TEXCOORD1;
	float3	m_ViewDir	: TEXCOORD2;
	float3  T			: TEXCOORD3;
	float3  B			: TEXCOORD4;
	float3  N			: TEXCOORD5;

};

struct PS_OUT
{
	float4	vColor : COLOR0;
};

PS_OUT PS_MASK_MAIN(PS_IN In)
{
	PS_OUT			Out;

	float3 tangentNormal = tex2D(NSampler, In.m_TexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 -1);

	float3x3 TBN = float3x3( normalize(In.T), normalize(In.B), normalize(In.N) );
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DSampler, In.m_TexUV);
	float3 lightDir = normalize(In.m_LightDir);
	float3 diffuse = saturate( dot( worldNormal, -lightDir ) );
	diffuse = g_vLightDiffuse.rgb * albedo.rgb * diffuse;

	float3 specular = 0;

	if(diffuse.x > 0.f)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(In.m_ViewDir);

		specular = saturate( dot( reflection, -viewDir ) );
		specular = pow(specular, 20.f);

		float4 specularIntensity = tex2D( SSampler, In.m_TexUV );
		specular *= specularIntensity.rgb * g_vLightSpecular.rgb;
	}

	float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo.xyz;

	float4 C = tex2D(CSampler, In.m_TexUV);

	C.r = C.g;

	if(g_iPhase == 1)
		C.g = 0.f;

	C *= g_fTime;

	Out.vColor = float4(ambient + C.xyz + diffuse + specular * 0.3f , 1.f);
	
	return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	float3 tangentNormal = tex2D(NSampler, In.m_TexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 -1);

	float3x3 TBN = float3x3( normalize(In.T), normalize(In.B), normalize(In.N) );
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DSampler, In.m_TexUV);
	float3 lightDir = normalize(In.m_LightDir);
	float3 diffuse = saturate( dot( worldNormal, -lightDir ) );
	diffuse = g_vLightDiffuse.rgb * albedo.rgb * diffuse;

	float3 specular = 0;

	if(diffuse.x > 0.f)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(In.m_ViewDir);

		specular = saturate( dot( reflection, -viewDir ) );
		specular = pow(specular, 20.f);

		float4 specularIntensity = tex2D( SSampler, In.m_TexUV );
		specular *= specularIntensity.rgb * g_vLightSpecular.rgb;
	}

	float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo.xyz;

	Out.vColor = float4(ambient + diffuse + (specular * 0.3f), 1.f);

	return Out;
}

PS_OUT GROUND_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	float3 tangentNormal = tex2D(NSampler, In.m_TexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 -1);

	float3x3 TBN = float3x3( normalize(In.T), normalize(In.B), normalize(In.N) );
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DSampler, In.m_TexUV);
	float3 lightDir = normalize(In.m_LightDir);
	float3 diffuse = saturate( dot( worldNormal, -lightDir ) );
	diffuse = g_vLightDiffuse.rgb * albedo.rgb * diffuse;

	float3 specular = 0;

	if(diffuse.x > 0.f)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(In.m_ViewDir);

		specular = saturate( dot( reflection, -viewDir ) );
		specular = pow(specular, 20.f);

		float4 specularIntensity = tex2D( SSampler, In.m_TexUV );
		specular *= specularIntensity.rgb * g_vLightSpecular.rgb;
	}

	float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo.xyz;

	Out.vColor = float4(ambient + diffuse + (specular * 0.3f), 1.f);

	return Out;
}

PS_OUT SK_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	float3 tangentNormal = tex2D(NSampler, In.m_TexUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 -1);

	float3x3 TBN = float3x3( normalize(In.T), normalize(In.B), normalize(In.N) );
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DSampler, In.m_TexUV);
	float3 lightDir = normalize(In.m_LightDir);
	float3 diffuse = saturate( dot( worldNormal, -lightDir ) );
	diffuse = g_vLightDiffuse.rgb * albedo.rgb * diffuse;

	float3 specular = 0;

	if(diffuse.x > 0.f)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(In.m_ViewDir);

		specular = saturate( dot( reflection, -viewDir ) );
		specular = pow(specular, 20.f);

		float4 specularIntensity = tex2D( SSampler, In.m_TexUV );
		specular *= specularIntensity.rgb * g_vLightSpecular.rgb;
	}

	float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo.xyz;

	float4 C = tex2D(CSampler, In.m_TexUV);

	C.r = C.g;

	if(g_iPhase == 1)
		C.g = 0.f;

	C *= g_fTime;

	Out.vColor = float4(ambient + C.xyz + diffuse + specular * 0.3f , 1.f);

	Out.vColor.a = 1.f - g_fDyingTime;
	
	return Out;
}


technique Default_Device
{
	pass Default_Rendering //0
	{
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Mask_Rendering //1
	{
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MASK_MAIN();
	}

	pass SK_Rendering //2
	{
		alphafunc = greater;
		alphablendenable = true;	
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 SK_PS_MAIN();
	}

	pass Default_Rendering //3
	{
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 GROUND_PS_MAIN();
	}

};