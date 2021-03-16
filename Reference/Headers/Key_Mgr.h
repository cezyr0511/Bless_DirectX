#ifndef Key_Mgr_h__
#define Key_Mgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKey_Mgr : public CBase
{
	DECLARE_SINGLETON(CKey_Mgr)

public:
	enum MOUSECLICK { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON , DIM_END};
	enum MOUSEMOVE  { DIM_X, DIM_Y, DIM_Z };
	const static _uint KEYMAX = 256;

private:
	LPDIRECTINPUT8			m_pInput;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

private:
	_byte					m_byKeyState[KEYMAX];
	DIMOUSESTATE			m_MouseState;

	_bool					bKeyDown[KEYMAX];     // Ű�� ���ȴ��� üũ�� �迭
	_bool					bKeyUp[KEYMAX];       // Ű�� �������� üũ�� �迭

	_bool					bMouseDown[DIM_END];     // Ű�� ���ȴ��� üũ�� �迭
	_bool					bMouseUp[DIM_END];       // Ű�� �������� üũ�� �迭

private:
	explicit CKey_Mgr(void);
	virtual ~CKey_Mgr(void);

public:
	_byte	GetDIKeyState(_ubyte KeyFlag);
	_byte	GetDIMouseState(MOUSECLICK KeyFlag);
	_long	GetDIMouseMove(MOUSEMOVE KeyFlag);

public:
	void	SetInputState(void);
	HRESULT	InitInputDevice(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT	InitKeyBoard(HWND hWnd);
	HRESULT	InitMouse(HWND hWnd);

public:
	_bool 	OnceKeyDown( _ubyte nKey  ,_bool* pData = NULL);// Ű�� �ѹ� ���ȴ��� üũ			-> Ű�� ������ �޼��� ť�� ����
	_bool 	OnceKeyUp( _ubyte nKey  ,_bool* pData = NULL);  // Ű�� �ѹ� �������� üũ			-> Ű�� ���� �޼��� ť�� ����

	_bool 	OnceMouseDown( _ubyte nKey ,_bool* pData = NULL);// Ű�� �ѹ� ���ȴ��� üũ			-> Ű�� ������ �޼��� ť�� ����
	_bool 	OnceMouseUp( _ubyte nKey ,_bool* pData = NULL);  // Ű�� �ѹ� �������� üũ			-> Ű�� ���� �޼��� ť�� ����

private:
	virtual void Free(void);

};

END
#endif // Key_Mgr_h__
