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

struct VS_IN // ���ؽ� ���ۿ��� ������ ������ ����
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // ���ؽ� ���̴��� �����ϰ� �� ������ ��������
{
	vector	vPosition : POSITION;
	//vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	//vector	vWorldPos : TEXCOORD1;
};

// ���ؽ� ���̴� : ������ ��ȯ + ������ ����
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	// ������ ��ȯ
	// mul:��İ� ����� ���� �����ϰ� ����� �����Ѵ�.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// ������ı��� ���ߴ�.	
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


// �����Ͷ���� �����Ѵ�. (VS_OUT ���������� ������� �ȼ��� �����)

struct PS_IN
{	
	//vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

// �ȼ����̴� : �ȼ��� ������ ���������Ͽ� �ȼ��� �������� ���� �����Ѵ�.
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

		// �������������� �����Ѵ�.
		
		Cullmode = none;
		FillMode = wireframe;
	
		alphatestenable = true;
*/
		// ��������� �����ض�.
		// ���ؽ����̴��� �����ϱ����� �������Լ��� �����Ѵ�.	
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