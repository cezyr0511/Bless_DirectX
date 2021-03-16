#include "Shader_Base.hpp"

texture			g_BaseTexture;

float			g_fMove = 0.f;
float			g_fMove2 = 0.f;
float			g_fMove3 = 0.f;
int				g_iNum = 0;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	addressu = clamp;
	addressv = clamp;
};

struct VS_IN // ���ؽ� ���ۿ��� ������ ������ ����
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // ���ؽ� ���̴��� �����ϰ� �� ������ ��������
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
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
	
	if(g_iNum == 18)
		Out.vTexUV = In.vTexUV + float2(g_fMove2, 0.f);
	else if(g_iNum == 6)
		Out.vTexUV = In.vTexUV + float2(g_fMove3, 0.f);
	else
		Out.vTexUV = In.vTexUV + float2(g_fMove, 0.f);

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

	Out.vColor = tex2D(BaseSampler, In.vTexUV);		

	return Out;
}


technique Default_Device
{
	pass Default_Rendering //0
	{		
		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Default_Rendering //1
	{		
		cullmode = none;
		lighting = false;

		alphablendenable = true;	
		srcblend = srcalpha;
		destblend = one;

		zenable = true;
		zwriteenable = false;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}


};