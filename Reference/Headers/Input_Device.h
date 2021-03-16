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
	// 내가 입력장치의 기본 설정을 셋팅한다.
	LPDIRECTINPUT8				m_pInputSDK;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;
private:
	_byte					m_byKeyState[KEYMAX];
	DIMOUSESTATE			m_MouseState;

	_bool					bKeyDown[KEYMAX];     // 키가 눌렸는지 체크할 배열
	_bool					bKeyUp[KEYMAX];       // 키가 떼졌는지 체크할 배열

	_bool					bMouseDown[DIM_END];     // 키가 눌렸는지 체크할 배열
	_bool					bMouseUp[DIM_END];       // 키가 떼졌는지 체크할 배열

public:
	_bool 	OnceKeyDown( _ubyte byKeyID  ,_bool* pData = NULL);// 키가 한번 눌렸는지 체크			-> 키를 누를때 메세지 큐에 저장
	_bool 	OnceKeyUp( _ubyte byKeyID  ,_bool* pData = NULL);  // 키가 한번 떼졌는지 체크			-> 키를 땔떼 메세지 큐에 저장

	_bool 	OnceMouseDown( _ubyte byKeyID ,_bool* pData = NULL);// 키가 한번 눌렸는지 체크			-> 키를 누를때 메세지 큐에 저장
	_bool 	OnceMouseUp( _ubyte byKeyID ,_bool* pData = NULL);  // 키가 한번 떼졌는지 체크			-> 키를 땔떼 메세지 큐에 저장
public:
	virtual void Free(void);

};

END

#endif // Input_Device_h__
