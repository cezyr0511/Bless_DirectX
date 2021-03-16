#include "Dynamic_Mesh.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"

USING(Engine)

Engine::CDynamic_Mesh::CDynamic_Mesh(LPDIRECT3DDEVICE9 pGraphicDev)
: CMesh(pGraphicDev)
, m_pLoader(NULL)
, m_pRootFrame(NULL)
, m_pAnimationCtrl(NULL)
{

}

Engine::CDynamic_Mesh::CDynamic_Mesh(const CDynamic_Mesh& rhs)	
: CMesh(rhs)
, m_pLoader(rhs.m_pLoader)
, m_pRootFrame(rhs.m_pRootFrame)
, m_MeshContainerList(rhs.m_MeshContainerList)
, m_pAnimationCtrl(CAnimationCtrl::Create(*rhs.m_pAnimationCtrl))
, m_matParent(rhs.m_matParent)
{
	m_pLoader->AddRef();
}

Engine::CDynamic_Mesh::~CDynamic_Mesh(void)
{

}

_matrix* Engine::CDynamic_Mesh::Get_FrameMatrix(const char* pFrameName) const
{
	D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

	if(NULL == pFindFrame)
		return NULL;

	return &pFindFrame->CombinedTransformationMatrix;
}

HRESULT Engine::CDynamic_Mesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	if(NULL == m_pLoader)
		return E_FAIL;		

	m_pAnimationCtrl = CAnimationCtrl::Create(m_pGraphicDev);
	if(NULL == m_pAnimationCtrl)
		return E_FAIL;	
	
	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, m_pLoader, NULL, &m_pRootFrame, m_pAnimationCtrl->Get_AnimationCtrl())))
		return E_FAIL;


	D3DXMatrixRotationY(&m_matParent, D3DXToRadian(-90.0f));

	Update_CombinedMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &m_matParent);

	SetUp_MatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}


// 뼈들이 가지고 있는 CombinedTransformationMatrix에 값을 채우자.
void Engine::CDynamic_Mesh::Update_CombinedMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pMatrix)
{
	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pMatrix;

	if(NULL != pFrame->pFrameSibling)
		Update_CombinedMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pMatrix);

	if(NULL != pFrame->pFrameFirstChild)
		Update_CombinedMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);		
}

void Engine::CDynamic_Mesh::SetUp_MatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if(NULL != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBone; ++i)
		{
			const char* pFrameName = pMeshContainer->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED* pFrameFind = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pFrameFind->CombinedTransformationMatrix;		
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}	

	if(NULL != pFrame->pFrameSibling)
		SetUp_MatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if(NULL != pFrame->pFrameFirstChild)
		SetUp_MatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

void Engine::CDynamic_Mesh::Render_Mesh(void)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();	

	for (; iter != iter_end; ++iter)
	{
		for (_ulong i = 0; i < (*iter)->dwNumBone; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffsetMatrices[i] * *(*iter)->ppCombinedTransformationMatrices[i];

		void*		pSrcVertices = NULL, *pDestVertices = NULL;

		(*iter)->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices, NULL, pSrcVertices, pDestVertices); // 현재 메시컨테이너를 그려져야할 행렬만큼 변환해준다.

		(*iter)->pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < (*iter)->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, (*iter)->ppTextures[i]);
			(*iter)->MeshData.pMesh->DrawSubset(i);
		}		
	}	
}

void Engine::CDynamic_Mesh::Render_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx /*= 0*/)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();	

	for (; iter != iter_end; ++iter)
	{
		for (_ulong i = 0; i < (*iter)->dwNumBone; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffsetMatrices[i] * *(*iter)->ppCombinedTransformationMatrices[i];

		void*		pSrcVertices = NULL, *pDestVertices = NULL;

		(*iter)->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices, NULL, pSrcVertices, pDestVertices); // 현재 메시컨테이너를 그려져야할 행렬만큼 변환해준다.

		(*iter)->pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < (*iter)->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", (*iter)->ppTextures[i]);

			pEffect->CommitChanges();

			pEffect->BeginPass(iPassIdx);

			(*iter)->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}		
	}	
}


void Engine::CDynamic_Mesh::Render_NorMal_Mesh(LPD3DXEFFECT pEffect, _uint iPassIdx /*= 0*/)
{
	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();
	MESHCONTAINERLIST::iterator	iter_end = m_MeshContainerList.end();	

	for (; iter != iter_end; ++iter)
	{
		for (_ulong i = 0; i < (*iter)->dwNumBone; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffsetMatrices[i] * *(*iter)->ppCombinedTransformationMatrices[i];

		void*		pSrcVertices = NULL, *pDestVertices = NULL;

		(*iter)->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices, NULL, pSrcVertices, pDestVertices); // 현재 메시컨테이너를 그려져야할 행렬만큼 변환해준다.

		(*iter)->pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();


		for (_ulong i = 0; i < (*iter)->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_DTexture", (*iter)->ppTextures[i]);
			
			if(NULL != (*iter)->pNTextures[i])
				pEffect->SetTexture("g_NTexture", (*iter)->pNTextures[i]);

			if(NULL != (*iter)->pSTextures[i])
				pEffect->SetTexture("g_STexture", (*iter)->pSTextures[i]);

			if(NULL != (*iter)->pCTextures[i])
				pEffect->SetTexture("g_CTexture", (*iter)->pCTextures[i]);

			pEffect->CommitChanges();

			pEffect->BeginPass(iPassIdx);

			(*iter)->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}		
	}	
}

void Engine::CDynamic_Mesh::Set_AnimationSet(const _uint& iIndex)
{
	m_pAnimationCtrl->Set_AnimationSet(iIndex);
}

void Engine::CDynamic_Mesh::Play_AnimationSet(const _float& fTimeDelta)
{
	m_pAnimationCtrl->Play_AnimationSet(fTimeDelta);

	Update_CombinedMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &m_matParent);	
}

CComponent* Engine::CDynamic_Mesh::Clone(void)
{
	return new CDynamic_Mesh(*this);
}

CDynamic_Mesh* Engine::CDynamic_Mesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamic_Mesh*		pInstance = new CDynamic_Mesh(pGraphicDev);

	if(FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		MSG_BOX("CDynamic_Mesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CDynamic_Mesh::Compute_MinMax(_vec3* pMin, _vec3* pMax, const _uint& iContainerIdx)
{
	D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	MESHCONTAINERLIST::iterator	iter = m_MeshContainerList.begin();

	for (_uint i = 0; i < iContainerIdx; ++i)
		++iter;

	(*iter)->MeshData.pMesh->GetDeclaration(Element);

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

	(*iter)->MeshData.pMesh->LockVertexBuffer(0, (void**)&pVertex);

	_ulong dwFVF = (*iter)->MeshData.pMesh->GetFVF();

	D3DXComputeBoundingBox((_vec3*)(pVertex + byOffset), (*iter)->MeshData.pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFVF), pMin, pMax);

	(*iter)->MeshData.pMesh->UnlockVertexBuffer();	
}

BOOL Engine::CDynamic_Mesh::Check_AnimationSet(const _float& fTime)
{
	return m_pAnimationCtrl->Check_AnimationSet(fTime);
}

_int Engine::CDynamic_Mesh::Get_AniIndex(void)
{
	return	m_pAnimationCtrl->Get_AniIndex();
}

_double Engine::CDynamic_Mesh::Get_AniPosition(void)
{
	return m_pAnimationCtrl->Get_AniPosition();
}

void Engine::CDynamic_Mesh::Free(void)
{
	Engine::CMesh::Free();

	if(false == m_isClone)// 원형객체의 경우에만.
	{
		m_pLoader->DestroyFrame(m_pRootFrame);

	}

	Engine::Safe_Release(m_pAnimationCtrl);

	m_MeshContainerList.clear();
	
	Engine::Safe_Release(m_pLoader);	
}

