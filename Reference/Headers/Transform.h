#ifndef Transform_h__
#define Transform_h__

#include "Engine_Include.h"
#include "Component.h"

// 이친구를 포함하고 있는 객체의 월드행렬을 장치에 셋팅한다.

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z};
	enum INFOMATION {INFO_SCALE, INFO_ANGLE, INFO_POSITION, INFO_END};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);
public:
	_matrix* Get_WorldMatrix(void) { return &m_matWorld; }
	_vec3*	 Get_vPos(void)			{ return &m_vInfomation[INFO_POSITION]; }
	_vec3*	 Get_vLook(void);
	_vec3*	 Get_vRight(void);
	_vec3*	 Get_vLeft(void);
	_vec3*	 Get_vBack(void);
public:
	void Set_Infomation(INFOMATION eInfo, const _float& fX, const _float& fY, const _float& fZ );
	void Set_Infomation(INFOMATION eInfo, const _vec3* pVector);
	void Set_ParentMatrix(_matrix* pParentMatrix);
	void Set_WorldMatrix(_matrix* pWorldMatrix, _bool isBill);
	void SetUp_OnGraphicDev(void);
	void SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
public:
	HRESULT Ready_Component(void);
	void Update_Component(void);
public:
	void Go_Straight(const _float& fSpeedTimeDelta);
	void Go_Straight(_vec3 vDir, const _float& fSpeedTimeDelta);
	void Back_Straight(const _float& fSpeedTimeDelta);
	void Rotation(ANGLE eType, const _float& fAngleTimeDelta);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
private:	
	_vec3				m_vInfomation[INFO_END];
	_vec3				m_vLeft;
	_vec3				m_vBack;
	_matrix				m_matWorld;
	_matrix*			m_pParentMatrix;

private:
	_bool				m_isBill;
public:
	virtual CComponent* Clone(void);
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

END

#endif // Transform_h__
