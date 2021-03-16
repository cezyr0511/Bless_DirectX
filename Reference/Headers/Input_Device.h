#ifndef Input_Device_h__
#define Input_Device_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:		
	enum MOUSEKEYSTATE {DIM_LB, DIM_RB, DIM_HB, DIM_END};
	enum MOUSEMOVESTATE {DIMS_X, DIMS_Y, DIMS_Z, DIMS_END};
	const static _uint KEYMAX = 256;
private:
	explicit CInput_Device(void);
	virtual ~CInput_Device(void);
public:
	_byte Get_DIKeyState(_ubyte byKeyID) {
		return m_byKeyState[byKeyID];}
	_byte Get_DIMouseState(MOUSEKEYSTATE byMouseID) {
		return m_MouseState.rgbButtons[byMouseID];}
	_long Get_DIMouseMove(MOUSEMOVESTATE byMouseMoveState) {
		return *(((_long*)(&m_MouseState)) + byMouseMoveState);}
public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	void SetUp_InputState(void);
private:
	// ���� �Է���ġ�� �⺻ ������ �����Ѵ�.
	LPDIRECTINPUT8				m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;
private:
	_byte					m_byKeyState[KEYMAX];
	DIMOUSESTATE			m_MouseState;

	_bool					bKeyDown[KEYMAX];     // Ű�� ���ȴ��� üũ�� �迭
	_bool					bKeyUp[KEYMAX];       // Ű�� �������� üũ�� �迭

	_bool					bMouseDown[DIM_END];     // Ű�� ���ȴ��� üũ�� �迭
	_bool					bMouseUp[DIM_END];       // Ű�� �������� üũ�� �迭

public:
	_bool 	OnceKeyDown( _ubyte byKeyID  ,_bool* pData = NULL);// Ű�� �ѹ� ���ȴ��� üũ			-> Ű�� ������ �޼��� ť�� ����
	_bool 	OnceKeyUp( _ubyte byKeyID  ,_bool* pData = NULL);  // Ű�� �ѹ� �������� üũ			-> Ű�� ���� �޼��� ť�� ����

	_bool 	OnceMouseDown( _ubyte byKeyID ,_bool* pData = NULL);// Ű�� �ѹ� ���ȴ��� üũ			-> Ű�� ������ �޼��� ť�� ����
	_bool 	OnceMouseUp( _ubyte byKeyID ,_bool* pData = NULL);  // Ű�� �ѹ� �������� üũ			-> Ű�� ���� �޼��� ť�� ����
public:
	virtual void Free(void);

};

END

#endif // Input_Device_h__
