#include "Input_Device.h"

USING(Engine)

IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
: m_pInputSDK(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(_byte) * KEYMAX);
	ZeroMemory(bKeyDown, sizeof(_bool) * KEYMAX);
	ZeroMemory(bKeyUp, sizeof(_bool) * KEYMAX);
	ZeroMemory(bMouseDown, sizeof(_bool) * DIM_END);
	ZeroMemory(bMouseUp, sizeof(_bool) * DIM_END);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput_Device::~CInput_Device()
{

}

HRESULT Engine::CInput_Device::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	// ��Ű���带 �����ϱ� ���� ������ü�� �����Ѵ�.
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;	

	// �� ��ǻ�Ϳ� ����Ǿ� �ִ� Ű���尴ü�� �����Ѵ�.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �����Ѵ�.
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ������ Ű������ ���� ������ �����Ѵ�.
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ Ű���带 Ȱ��ȭ��Ų��.
	m_pKeyBoard->Acquire();

	// �� ��ǻ�Ϳ� ����Ǿ� �ִ� ���콺 ��ü�� �����Ѵ�.
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �����Ѵ�.
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ������ ���콺�� ���� ������ �����Ѵ�.
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ ���콺�� Ȱ��ȭ��Ų��.
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput_Device::SetUp_InputState()
{
	HRESULT hr;
	hr = m_pKeyBoard->GetDeviceState(256, &m_byKeyState);
	HRESULT hr_Err = DIERR_INPUTLOST;
	HRESULT hr_Err2 = DIERR_NOTACQUIRED;

	if(hr == hr_Err || hr == hr_Err2)
	{
		m_pKeyBoard->Acquire();
	}

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	if(hr == hr_Err || hr == hr_Err2)
	{
		m_pMouse->Acquire();
	}
}

_bool Engine::CInput_Device::OnceKeyDown( _ubyte byKeyID ,_bool* pData /*= NULL*/ )
{
	_bool* pCompareData = NULL;
	if(pData == NULL)
	{
		pCompareData = &bKeyDown[byKeyID];
	}
	else
	{
		pCompareData = pData;
	}

	if( m_byKeyState[byKeyID] & 0x80 )
	{
		//  ������ ���� true
		if( *pCompareData == false )
		{
			*pCompareData = true;
			return TRUE;
		}
	}
	else
	{
		//  ���� ���� false
		if(*pCompareData == true)
		{
			*pCompareData = false;
		}
	}   

	return FALSE;
}

_bool Engine::CInput_Device::OnceKeyUp( _ubyte byKeyID ,_bool* pData /*= NULL*/ )
{
	_bool* pCompareData = NULL;

	if(pData == NULL)
	{
		pCompareData = &bKeyUp[byKeyID];
	}
	else
	{
		pCompareData = pData;
	}

	if(m_byKeyState[byKeyID] & 0x80 )
	{
		//  ������ ���� true
		if( *pCompareData == false )
		{
			*pCompareData = true;

		}
	}
	else
	{
		if(*pCompareData == true)//  ���� ���� false
		{
			*pCompareData = false;
			return TRUE;
		}		
	}   

	return FALSE;
}

_bool Engine::CInput_Device::OnceMouseDown( _ubyte byKeyID ,_bool* pData /*= NULL*/ )
{
	_bool* pCompareData = NULL;

	if(pData == NULL)
	{
		pCompareData = &bMouseDown[byKeyID];
	}
	else
	{
		pCompareData = pData;
	}

	if( m_MouseState.rgbButtons[byKeyID] & 0x80 )
	{
		//  ������ ���� true
		if( *pCompareData == false )
		{
			*pCompareData    =   true;
			return TRUE;
		}
	}
	else
	{
		//  ���� ���� false
		if(*pCompareData == true)
		{
			*pCompareData = false;
		}
	}   

	return FALSE;
}


_bool Engine::CInput_Device::OnceMouseUp( _ubyte byKeyID ,_bool* pData /*= NULL*/ )
{
	_bool* pCompareData = NULL;
	if(pData == NULL)
	{
		pCompareData = &bMouseDown[byKeyID];
	}
	else
	{
		pCompareData = pData;
	}

	if(m_MouseState.rgbButtons[byKeyID] & 0x80 )
	{
		//  ������ ���� true
		if( *pCompareData == false )
		{
			*pCompareData  = true;

		}
	}
	else
	{
		if(*pCompareData  == true)//  ���� ���� false
		{
			*pCompareData  = false;
			return TRUE;
		}		
	}   

	return FALSE;
}

void Engine::CInput_Device::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

