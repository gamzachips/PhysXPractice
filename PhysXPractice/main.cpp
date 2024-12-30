#include "pch.h"
#include "GameApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GameApp App(hInstance);  
	if (!App.Initialize())
		return -1;

	//AllocConsole();

	//// 2. 표준 입출력 스트림을 콘솔에 연결
	//FILE* fp;
	//freopen_s(&fp, "CONOUT$", "w", stdout);  // 표준 출력(stdout)
	//freopen_s(&fp, "CONOUT$", "w", stderr);  // 표준 에러(stderr)
	//freopen_s(&fp, "CONIN$", "r", stdin);   // 표준 입력(stdin)

	return App.Run();
}
