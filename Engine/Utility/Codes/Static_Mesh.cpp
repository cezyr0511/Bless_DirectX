#include "Static_Mesh.h"

USING(Engine)

Engine::CStatic_Mesh::CStatic_Mesh(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CMesh(pGraphicDev)
{
	m_NTextures = NULL;
	m_STextures = NULL;
}

Engine::CStatic_Mesh::CStatic_Mesh(const CStatic_Mesh& rhs)
: Engine::CMesh(rhs)
, m_pAdjacency(rhs.m_pAdjacency)
, m_pSubSetBuffer(rhs.m_pSubSetBuffer)
, m_pSubSets(rhs.m_pSubSets)
, m_dwSubSetCnt(rhs.m_dwSubSetCnt)
, m_pMesh(rhs.m_pMesh)
, m_pOriMesh(rhs.m_pOriMesh)
, m_ppTextures(rhs.m_ppTextures)
{
	/*m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];*/

	for (_ulong i = 0; i < rhs.m_dwSubSetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}
	m_pAdjacency->AddRef();
	m_pSubSetBuffer->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();

	m_NTextures = NULL;
	m_STextures = NULL;

}

Engine::CStatic_Mesh::~CStatic_Mesh(void)
{

}

HRESULT Engine::CStatic_Mesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	_ulong	dwSubSetCnt = 0;

	if(FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjacency, &m_pSubSetBuffer, NULL, &dwSubSetCnt, &m_pOriMesh)))
		return E_FAIL;	

	// 메시를 구성하고 있는 정점의 fvf정보를 리턴한다.
	_ulong dwFVF = m_pOriMesh->GetFVF();

	if(dwFVF & D3DFVF_NORMAL)
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}
	else
	{
		// 내가 지정한 fvf정보로 메시를 복제하겠다.
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		// 노멀을 계산해준다.
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}

	// D3DXBuffer가 담고있는 정보의 가장 앞 주소를 리턴한다.
	m_pSubSets = (D3DXMATERIAL*)m_pSubSetBuffer->GetBufferPointer();

	m_dwSubSetCnt = dwSubSetCnt == 0 ? 1 : dwSubSetCnt;

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];

	if(0 != dwSubSetCnt)
	{
		for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
		{
			_tchar			szFileName[128] = L"";

			lstrcpy(szFullPath, pFilePath);

			MultiByteToWideChar(CP_ACP, 0, m_pSubSets[i].pTextureFilename, strlen(m_pSubSets[i].pTextureFilename)
				, szFileName, 128);

			lstrcat(szFullPath, szFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
				return E_FAIL;
		}
	}

	else
	{
		m_pSubSets[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Emissive = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pSubSets[0].MatD3D.Power = 1.0f;

		m_ppTextures[0] = NULL;				
	}

	return S_OK;
}

void Engine::CStatic_Mesh::Render_Mesh()
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		if(NULL != m_ppTextures[i])
			m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
	
}

void Engine::CStatic_Mesh::Render_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx /*= 0*/)
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		if(NULL != m_ppTextures[i])
			pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);

		pEffect->CommitChanges();

		pEffect->BeginPass(iPassIdx);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}	
}

void Engine::CStatic_Mesh::Render_Mesh(LPD3DXEFFECT pEffect, const char* pNormalName, const char* pSpecularName, _uint iPassIdx /*= 0*/)
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		if(NULL != m_ppTextures[i])
			pEffect->SetTexture("g_DTexture", m_ppTextures[i]);
		
		pEffect->SetTexture("g_NTexture", m_NTextures);

		pEffect->SetTexture("g_STexture", m_STextures);

		pEffect->CommitChanges();

		pEffect->BeginPass(iPassIdx);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}	
}

_bool Engine::CStatic_Mesh::Find_Alpha(const char* pFileName)
{
	_uint iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength + 1; ++i)
	{
		if(pFileName[i] == '.')
		{
			if(pFileName[i - 1] == 'A')
				return true;
		}
	}
	return false;
}

CStatic_Mesh* Engine::CStatic_Mesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStatic_Mesh*		pInstance = new CStatic_Mesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CStatic_Mesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* Engine::CStatic_Mesh::Clone(void)
{
	return new CStatic_Mesh(*this);
}

void Engine::CStatic_Mesh::Compute_MinMax(_vec3* pMin, _vec3* pMax)
{
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Element);

	_byte	byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			byOffset = Element[i].Offset;
			break;
		}
	}

	_byte*		pVertex = NULL;

	m_pMesh->LockVertexBuffer(0, (void**)&pVertex);

	_ulong dwFVF = m_pMesh->GetFVF();

	D3DXComputeBoundingBox((_vec3*)(pVertex + byOffset), m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFVF), pMin, pMax);

	m_pMesh->UnlockVertexBuffer();	
}

LPDIRECT3DTEXTURE9* Engine::CStatic_Mesh::Get_NTexture(void)
{
	return &m_NTextures;
}

LPDIRECT3DTEXTURE9* Engine::CStatic_Mesh::Get_STextures(void)
{
	return &m_STextures;
}

void Engine::CStatic_Mesh::Free(void)
{	
	Engine::CMesh::Free();

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		Engine::Safe_Release(m_ppTextures[i]);
	}

	Engine::Safe_Release(m_NTextures);
	Engine::Safe_Release(m_STextures);

	
	_ulong dwRefCnt = 0;
	dwRefCnt = Safe_Release(m_pOriMesh);
	dwRefCnt = Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubSetBuffer);

	if(false == m_isClone)
		Safe_Delete_Array(m_ppTextures);
	
}

