#include "Shader_Base.hpp"

float				g_fTime = 0.f;
float				texScaler = 1.0/128.0;
float				texOffset = 0.0;


texture				g_BaseTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

//float4 gaussFilter[7] =
//{
//	0.0, -3.0, 0.0, 1.0/64.0,  //0
//	0.0, -2.0, 0.0, 6.0/64.0,  //1
//	0.0, -1.0, 0.0, 15.0/64.0, //2
//	0.0, 0.0, 0.0,	20.0/64.0, //3
//	0.0, 1.0, 0.0,	15.0/64.0, //4
//	0.0, 2.0, 0.0,  6.0/64.0,  //5
//	0.0, 3.0, 0.0,  1.0/64.0   //6
//};

struct VS_IN // 버텍스 버퍼에서 가져온 정점의 정보
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // 버텍스 셰이더를 수행하고 난 이후의 정점정보
{
	vector	vPosition : POSITION;
	//vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	//vector	vWorldPos : TEXCOORD1;
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



VS_OUT VS_BLUR(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0.f;

	In.vPosition.xy = sign(In.vPosition.xy);

	Out.vPosition = float4( In.vPosition.xy, 0.0f, 1.0f);

	Out.vTexUV = (float2(Out.vPosition.x, -Out.vPosition.y) + 1.0f)/2.0f;

	return Out;
}


// 래스터라이즈를 수행한다. (VS_OUT 정점정보를 기반으로 픽셀을 만든다)

struct PS_IN
{	
	//vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

// 픽셀셰이더 : 픽셀의 정보를 바탕으로하여 픽셀의 최종적인 색을 결정한다.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(BaseSampler, In.vTexUV * 2.f);	

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In)
{
	PS_OUT			Out;

	float4			Color = 0.0f;

	float4 gaussFilter[7] =
	{
		0.0, -3.0, 0.0, 1.0/64.0,  //0
		0.0, -2.0, 0.0, 6.0/64.0,  //1
		0.0, -1.0, 0.0, 15.0/64.0, //2
		0.0, 0.0, 0.0,	20.0/64.0, //3
		0.0, 1.0, 0.0,	15.0/64.0, //4
		0.0, 2.0, 0.0,  6.0/64.0,  //5
		0.0, 3.0, 0.0,  1.0/64.0   //6
	};

	gaussFilter[0].w *= g_fTime;
	gaussFilter[1].w *= g_fTime;
	gaussFilter[2].w *= g_fTime;
	gaussFilter[3].w += g_fTime; 
	gaussFilter[4].w *= g_fTime;
	gaussFilter[5].w *= g_fTime;
	gaussFilter[6].w *= g_fTime;


	int i;
	for (i=0;i<7;i++)
	{
		Color += tex2D(BaseSampler,float2(In.vTexUV.x+gaussFilter[i].x * texScaler+texOffset,
			In.vTexUV.y+gaussFilter[i].y * texScaler+texOffset))
			*gaussFilter[i].w;
	}
	
	Out.vColor = Color * 1.0;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
/*
		LPDIRECT3DDEVICE9		pGraphicDev;
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// 렌더스테이츠를 설정한다.
		
		Cullmode = none;
		FillMode = wireframe;
	
		alphatestenable = true;
*/
		// 어떤버젼으로 번역해라.
		// 버텍스셰이더를 수행하기위한 진입점함수를 지정한다.	
		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	
	pass Default_Rendering
	{

		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_BLUR(); 
		PixelShader = compile ps_3_0 PS_MAIN2();
	}
	
};