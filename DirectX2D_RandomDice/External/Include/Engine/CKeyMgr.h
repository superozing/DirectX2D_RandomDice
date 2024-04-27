#pragma once
#include <utility>

// Key Value
enum KEY
{
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

	NUM0, NUM1, NUM2, NUM3, NUM4,
	NUM5, NUM6, NUM7, NUM8, NUM9, 

	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	TAB, LSHIFT, LALT, LCTRL, SPACE,
	ESC, ENTER, BACK, DEL,
	LEFT, RIGHT, UP, DOWN,

	LBTN, RBTN,

	KEY_END,
};

enum KEY_STATE
{
	NONE,
	TAP,
	PRESSED,
	RELEASED,
};

struct FKeyData
{
	KEY			eKey;
	KEY_STATE	eState;
	bool		bPressed;
};

struct FFontOutput
{
	float	AccTime;
	float	MaxTime;
	wstring OutputWStr;
};

class CKeyMgr
	: public CSingleton<CKeyMgr>
{
	SINGLE(CKeyMgr);
private:
	vector<FKeyData>	m_vecKeyData;

	Vec2				m_vMousePos;
	Vec2				m_vMousePrevPos;

	Vec2				m_vMouseWorldPos;

	Vec2				m_vMouseDrag;

#ifndef _RELEASE_GAME
	list<FFontOutput>	m_mouseClick;

public:
	void render();
#endif

public:
	KEY_STATE GetKeyState(KEY _Key) { return m_vecKeyData[_Key].eState; }
	Vec2 GetMousePos() const { return m_vMousePos; }
	Vec2 GetMouseDrag() const { return m_vMouseDrag; }

	Vec2 GetMouseWorldPos() const { return m_vMouseWorldPos; }

public:
	void init();
	void tick();
};

