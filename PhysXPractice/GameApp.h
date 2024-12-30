#pragma once

#include <windows.h>

#define MAX_LOADSTRING 100

class Game;
class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	static HWND m_hWnd;	
	static GameApp* m_pInstance;

public:
	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;                
	WCHAR m_szTitle[MAX_LOADSTRING];      
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_wcex;
	float m_previousTime;
	float m_currentTime;
	int  m_nCmdShow;
	//GameTimer m_Timer;
	UINT m_ClientWidth;
	UINT m_ClientHeight;


public:
	bool Initialize();
	bool Run();
	void Update();
	void Render();
	static Game* GetGame() { return mGame; }
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static Game* mGame;

};

