#include<windows.h>
#include"MyDialog.h"



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);


int speed[10], distance[20], time[30];
HINSTANCE hInstance;
HWND hwnd1;
int iCmdShow;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My DialogBox");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);


	hwnd = CreateWindow(szAppName, TEXT("My Dialog Box"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);

}





LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	

	switch (iMsg) {

	case WM_CREATE:
		//		hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		break;
	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_SPACE:
			DialogBox(hInstance, MAKEINTRESOURCE(SPEEDF), hwnd, MyDlgProc);
			break;
		case VK_DOWN:
			
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			break;


		}
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}

BOOL CALLBACK MyDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int speed1, distance1, time1, speed2, distance2, time2;
	HMODULE hlib = NULL;
	typedef int(*pfnSpeed)(int, int);
	typedef int(*pfnDistance)(int, int);
	typedef int(*pfnTime)(int, int);
	pfnSpeed pfn1 = NULL;
	pfnDistance pfn2 = NULL;
	pfnTime pfn3 = NULL;
	TCHAR STR[255];

	switch (iMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hDlg, ID_ETSPEED));
		SetFocus(GetDlgItem(hDlg, ID_ETDISTANCE));
		SetFocus(GetDlgItem(hDlg, ID_ETTIME));
		return(TRUE);
	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			exit(0);
			break;
		}
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		case IDCALCULATE:
			speed2 = GetDlgItemInt(hDlg, ID_ETSPEED, speed, TRUE);
			distance2 = GetDlgItemInt(hDlg, ID_ETDISTANCE, distance, TRUE);
			time2 = GetDlgItemInt(hDlg, ID_ETTIME, time, TRUE);
			hlib = LoadLibrary(TEXT("Physics_sdt.dll"));
			if (hlib == NULL)
			{
				MessageBox(hDlg, TEXT("Dll not linked"), TEXT("MESSAGES"), MB_OK | MB_ICONERROR | MB_TOPMOST);
				DestroyWindow(hDlg);
			}
			pfn1 = (pfnSpeed)GetProcAddress(hlib, "Speed_Phy");

			pfn2 = (pfnDistance)GetProcAddress(hlib, "Distance_Phy");

			pfn3 = (pfnTime)GetProcAddress(hlib, "Time_Phy");
			speed1 = pfn1(distance2, time2);
			distance1 = pfn2(speed2, time2);
			time1 = pfn3(distance2, speed2);
			wsprintf(STR, "%d", speed1);
			MessageBox(hDlg, STR, TEXT("speed is"), MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
			wsprintf(STR, "%d", distance1);
			MessageBox(hDlg, STR, TEXT("distance is"), MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
			wsprintf(STR, "%d", time1);
			MessageBox(hDlg, STR, TEXT("time is"), MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
			FreeLibrary(hlib);
			EndDialog(hDlg, 0);
			ShowWindow(hwnd1, iCmdShow);
			UpdateWindow(hwnd1);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		return(TRUE);
	}
	return(FALSE);
}



