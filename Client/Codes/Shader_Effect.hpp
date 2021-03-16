#include "Shader_Base.hpp"

texture				g_BaseTexture;

float				g_fTime = 0.f;
float				g_fMove = 0.f;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN // 버텍스 버퍼에서 가져온 정점의 정보
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // 버텍스 셰이더를 수행하고 난 이후의 정점정보
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

// 버텍스 셰이더 : 정점의 변환 + 정점의 수정
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	// 정점의 변환
	// mul:행렬과 행렬의 곱을 수행하고 결과를 리턴한다.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 투영행렬까지 곱했다.	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT MOVE_VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	// 정점의 변환
	// mul:행렬과 행렬의 곱을 수행하고 결과를 리턴한다.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 투영행렬까지 곱했다.	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV + float2(0, g_fMove);

	return Out;
}

struct PS_IN
{	
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

PS_OUT SK01_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		 vTextureCol = tex2D(BaseSampler, In.vTexUV);

	Out.vColor	  = vTextureCol;
	
	Out.vColor.a = 1.f - (vTextureCol.a * g_fTime);

	return Out;
}

PS_OUT SKILL_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		 vTextureCol = tex2D(BaseSampler, In.vTexUV);

	Out.vColor	  = vTextureCol;

	Out.vColor.rgb += g_fTime;

	Out.vColor.a = 1.f - (vTextureCol.a * g_fTime);

	return Out;
}

PS_OUT MAGIC_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		 vTextureCol = tex2D(BaseSampler, In.vTexUV);

	Out.vColor	  = vTextureCol;

	//Out.vColor.rgb += g_fTime;

	Out.vColor.a *= g_fTime;

	return Out;
}

PS_OUT DUMMY_PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		 vTextureCol = tex2D(BaseSampler, In.vTexUV);

	Out.vColor	  = vTextureCol;

	Out.vColor.a = 1.f - (vTextureCol.a * g_fTime);

	return Out;
}


technique Default_Device
{
	pass Default_Rendering //0
	{		
		lighting = false;
		zenable = true;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass SK01_Rendering //1
	{		
		lighting = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 SK01_PS_MAIN();
	}

	pass Particle_Rendering //2
	{		
		zenable = true;
		zwriteenable = false;

		lighting = false;
		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 SK01_PS_MAIN();
	}

	pass Hit_Rendering //3
	{		
		lighting = false;
		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Skill_Rendering //4
	{		
		lighting = false;

		zenable = true;
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 MOVE_VS_MAIN(); 
		PixelShader = compile ps_3_0 SKILL_PS_MAIN();
	}

	pass Skill_Rendering //5
	{		
		lighting = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 MAGIC_PS_MAIN();
	}

	pass Dummy_Rendering //6
	{		
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 DUMMY_PS_MAIN();
	}

};