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

	//// 2. ǥ�� ����� ��Ʈ���� �ֿܼ� ����
	//FILE* fp;
	//freopen_s(&fp, "CONOUT$", "w", stdout);  // ǥ�� ���(stdout)
	//freopen_s(&fp, "CONOUT$", "w", stderr);  // ǥ�� ����(stderr)
	//freopen_s(&fp, "CONIN$", "r", stdin);   // ǥ�� �Է�(stdin)

	return App.Run();
}
