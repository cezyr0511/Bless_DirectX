#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
: m_pGraphicDev(pGraphicDev)
, m_pFilePath(pFilePath)
, m_dwRefCnt(0)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

HRESULT Engine::CHierarchyLoader::Ready_HierarchyLoader(void)
{
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*			pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	if(NULL != Name)
		Allocate_Name(&pNewFrame->Name, Name);

	pNewFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name
														   , CONST D3DXMESHDATA *pMeshData
														   , CONST D3DXMATERIAL *pMaterials
														   , CONST D3DXEFFECTINSTANCE *pEffectInstances
														   , DWORD NumMaterials
														   , CONST DWORD *pAdjacency
														   , LPD3DXSKININFO pSkinInfo
														   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*			pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if(NULL != Name)
		Allocate_Name(&pNewMeshContainer->Name, Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH			pMesh = pMeshData->pMesh;
	pMesh->AddRef();

	_ulong dwMeshFVF = pMesh->GetFVF();

	// ?????? ?????? ?????´?.
	_ulong dwNumFaces = pMesh->GetNumFaces();
		
	pNewMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];

	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	if(!(dwMeshFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwMeshFVF | D3DFVF_NORMAL, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);

		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);

		Engine::Safe_Release(pMesh);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwMeshFVF, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);

		Engine::Safe_Release(pMesh);
	}

	pNewMeshContainer->NumMaterials = NumMaterials == 0 ? 1 : NumMaterials;

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	pNewMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	pNewMeshContainer->pNTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	pNewMeshContainer->pSTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	pNewMeshContainer->pCTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];

	if(0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			_tchar			szFileName[128] = L"";
			_tchar			szFullPath[128] = L"";

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, 128);

			lstrcpy(szFullPath, m_pFilePath);
			lstrcat(szFullPath, szFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTextures[i])))
				return E_FAIL;
			
			_wstring wstrFullPath1 = szFullPath;
			
			int a; 
			a = wstrFullPath1.find(L"_D_");
			wstrFullPath1.replace(a+1, 1, L"N");

			const wchar_t* cstr1 = wstrFullPath1.c_str();
			
			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, cstr1, &pNewMeshContainer->pNTextures[i])))
			{
				pNewMeshContainer->pNTextures[i] = NULL;
			}
			
			_wstring wstrFullPath2 = szFullPath;

			int b; 
			b = wstrFullPath2.find(L"_D_");
			wstrFullPath2.replace(b+1, 1, L"S");

			const wchar_t* cstr2 = wstrFullPath2.c_str();

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, cstr2, &pNewMeshContainer->pSTextures[i])))
			{
				pNewMeshContainer->pSTextures[i] = NULL;
			}

			_wstring wstrFullPath3 = szFullPath;

			int c; 
			c = wstrFullPath3.find(L"_D_");
			wstrFullPath3.replace(c+1, 1, L"C");

			const wchar_t* cstr3 = wstrFullPath3.c_str();

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, cstr3, &pNewMeshContainer->pCTextures[i])))
			{
				pNewMeshContainer->pCTextures[i] = NULL;
			}
		}
	}
	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 1.0f;

		pNewMeshContainer->ppTextures[0] = NULL;
		pNewMeshContainer->pNTextures[0] = NULL;
		pNewMeshContainer->pSTextures[0] = NULL;
		pNewMeshContainer->pCTextures[0] = NULL;
	}

	if(NULL != pSkinInfo)
	{
		pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(), pNewMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pNewMeshContainer->pOriMesh);

		pNewMeshContainer->pSkinInfo = pSkinInfo;
		pNewMeshContainer->pSkinInfo->AddRef();

		pNewMeshContainer->dwNumBone = pSkinInfo->GetNumBones();

		pNewMeshContainer->ppCombinedTransformationMatrices = new _matrix*[pNewMeshContainer->dwNumBone];
		pNewMeshContainer->pOffsetMatrices = new _matrix[pNewMeshContainer->dwNumBone];
		pNewMeshContainer->pRenderingMatrices = new _matrix[pNewMeshContainer->dwNumBone];

		for (_ulong i = 0; i < pNewMeshContainer->dwNumBone; ++i)		
			pNewMeshContainer->pOffsetMatrices[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);		
	}
	

	*ppNewMeshContainer = pNewMeshContainer;		
	
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(NULL == pFrameToFree)
		return S_OK;

	Engine::Safe_Delete_Array(pFrameToFree->Name);

	if(NULL != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(NULL != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if(NULL != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Engine::Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*			pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		Engine::Safe_Release(pMeshContainer->ppTextures[i]);
		Engine::Safe_Release(pMeshContainer->pSTextures[i]);
		Engine::Safe_Release(pMeshContainer->pNTextures[i]);
		Engine::Safe_Release(pMeshContainer->pCTextures[i]);
	}
	
	Engine::Safe_Delete_Array(pMeshContainer->ppTextures);
	Engine::Safe_Delete_Array(pMeshContainer->pSTextures);
	Engine::Safe_Delete_Array(pMeshContainer->pNTextures);
	Engine::Safe_Delete_Array(pMeshContainer->pCTextures);

	Engine::Safe_Release(pMeshContainer->pOriMesh);

	Engine::Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Engine::Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);
	Engine::Safe_Delete_Array(pMeshContainer->pRenderingMatrices);

	Engine::Safe_Delete_Array(pMeshContainer->Name);
	Engine::Safe_Delete_Array(pMeshContainer->pMaterials);
	Engine::Safe_Release(pMeshContainer->MeshData.pMesh);
	Engine::Safe_Delete_Array(pMeshContainer->pAdjacency);
	_ulong dwRefCnt = Engine::Safe_Release(pMeshContainer->pSkinInfo);


	Engine::Safe_Delete(pMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppSrcName, const char* pDestName)
{
	_uint		iLength = strlen(pDestName);

	*ppSrcName = new char[iLength + 1];

	strcpy(*ppSrcName, pDestName);		
}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CHierarchyLoader*		pInstance = new CHierarchyLoader(pGraphicDev, pFilePath);

	if(FAILED(pInstance->Ready_HierarchyLoader()))
	{
		MSG_BOX("CHierarchyLoader Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong Engine::CHierarchyLoader::Release(void)
{
	if(0 == m_dwRefCnt)
	{
		Engine::Safe_Release(m_pGraphicDev);		

		delete this;

		return 0;
	}

	return m_dwRefCnt--;	
}

