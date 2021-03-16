USING(Engine)

//=For.Management By.Ck 17.02.21 =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CComponent* Engine::Get_Component(const _tchar* pLayerTag, Engine::CGameObject::COMTYPE eType, const _tchar* pComponentTag, const _uint& iObjectIdx/* = 0*/)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, eType, pComponentTag, iObjectIdx);
}
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Initialize_Management(const _uint& iIndex, const _tchar* pRendererTag)
{
	return CManagement::GetInstance()->Initialize_Management(iIndex, pRendererTag);
}
_int Engine::Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Management(fTimeDelta);
}
void Engine::Render_Management(void)
{
	CManagement::GetInstance()->Render_Management();
}
HRESULT Engine::SetUp_CurrentScene(CScene* pCurrentScene)
{
	return CManagement::GetInstance()->SetUp_CurrentScene(pCurrentScene);
}

//=For.Component_Mgr By.Ck 17.02.27 =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Reserve_ComponentMgr(const _uint& iSize)
{
	return CComponent_Mgr::GetInstance()->Reserve_ComponentMgr(iSize);
}
HRESULT Engine::Add_Component(const _uint& iIndex, const _tchar* pComponentTag, CComponent* pComponent)
{
	return CComponent_Mgr::GetInstance()->Add_Component(iIndex, pComponentTag, pComponent);
}
CComponent* Engine::Clone_Component(const _uint& iIndex, const _tchar* pComponentTag)
{
	return CComponent_Mgr::GetInstance()->Clone_Component(iIndex, pComponentTag);
}
void Engine::Release_Component(const _uint& iIndex)
{
	CComponent_Mgr::GetInstance()->Release_Component(iIndex);
}

//=For.Navi_Mgr By.Ck 17.03.16 =======================================================================================================================================================================================================================================================================================
//-Getter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-Setter------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-General------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT	Engine::Initialize_Navi(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwSize)
{
	return CNavi_Mgr::GetInstance()->Initialize_Navi(pGraphicDev, dwSize);
}
HRESULT	Engine::AddCell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	return CNavi_Mgr::GetInstance()->AddCell(pPointA, pPointB, pPointC);
}
void Engine::Render_NaviMesh(void)
{
	CNavi_Mgr::GetInstance()->Render_NaviMesh();
}
void Engine::LinkCell(void)
{
	CNavi_Mgr::GetInstance()->LinkCell();
}
_ulong Engine::MoveOnNaviMesh(_vec3* pPos, const _vec3* pDir, const _ulong& dwIndex)
{
	return CNavi_Mgr::GetInstance()->MoveOnNaviMesh(pPos, pDir, dwIndex);
}

//=Release_Utility====================================================================================================================================================================================================================================================================================================================
void Engine::Release_Utility(void)
{
	if(0 != CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("CManagement Release Failed");

	if(0 != CComponent_Mgr::GetInstance()->DestroyInstance())
		MSG_BOX("CComponent_Mgr Release Failed");

	if(0 != CNavi_Mgr::GetInstance()->DestroyInstance())
		MSG_BOX("CNavi_Mgr Release Failed");
}