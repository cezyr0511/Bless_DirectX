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

	_bool					bKeyDown[KEYMAX];     // 키가 눌렸는지 체크할 배열
	_bool					bKeyUp[KEYMAX];       // 키가 떼졌는지 체크할 배열

	_bool					bMouseDown[DIM_END];     // 키가 눌렸는지 체크할 배열
	_bool					bMouseUp[DIM_END];       // 키가 떼졌는지 체크할 배열

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
	_bool 	OnceKeyDown( _ubyte nKey  ,_bool* pData = NULL);// 키가 한번 눌렸는지 체크			-> 키를 누를때 메세지 큐에 저장
	_bool 	OnceKeyUp( _ubyte nKey  ,_bool* pData = NULL);  // 키가 한번 떼졌는지 체크			-> 키를 땔떼 메세지 큐에 저장

	_bool 	OnceMouseDown( _ubyte nKey ,_bool* pData = NULL);// 키가 한번 눌렸는지 체크			-> 키를 누를때 메세지 큐에 저장
	_bool 	OnceMouseUp( _ubyte nKey ,_bool* pData = NULL);  // 키가 한번 떼졌는지 체크			-> 키를 땔떼 메세지 큐에 저장

private:
	virtual void Free(void);

};

END
#endif // Key_Mgr_h__
