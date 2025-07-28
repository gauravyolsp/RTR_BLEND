// Win32 Headers
#include <windows.h>

// Global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");

	// Code
	// Window Class Intialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Registration of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindow(szAppName,
		TEXT("Gaurav Kumar"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x co-ordinate
		CW_USEDEFAULT, // y co-ordinate 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL); // USEFUL for SDK

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Paint Background of the Window
	UpdateWindow(hwnd);

	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

// Callback Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local Variables
	TCHAR szMsg[1024] = {0};

	// Code
	switch(iMsg)
	{
		case WM_CREATE:
			wsprintf(szMsg, "This is the first Msg");
			MessageBox(hwnd, szMsg, TEXT("WM_CREATE"), MB_OK);
		break;
		case WM_SIZE:
			wsprintf(szMsg, "Window size is changed");
			MessageBox(hwnd, szMsg, TEXT("WM_SIZE"), MB_OK);
		break;
		case WM_MOVE:
			wsprintf(szMsg, "Window is moved");
			MessageBox(hwnd, szMsg, TEXT("WM_MOVE"), MB_OK);
		break;
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					wsprintf(szMsg, "Ecape key is pressed");
					MessageBox(hwnd, szMsg, TEXT("VK_ESCAPE"), MB_OK);
				break;
				default :
				break;
			}
		break;
		case WM_CHAR:
			switch(wParam)
			{
				case 'F':
					wsprintf(szMsg, "F key pressed");
					MessageBox(hwnd, szMsg, TEXT("WM_CHAR"), MB_OK);
				break;
				case 'f':
					wsprintf(szMsg, "f key pressed");
					MessageBox(hwnd, szMsg, TEXT("WM_CHAR"), MB_OK);
				break;
				default:
				break;
			}
		break;
		case WM_LBUTTONDOWN:
			wsprintf(szMsg, "Left Mouse Button is clicked");
			MessageBox(hwnd, szMsg, TEXT("WM_LBUTTONDOWN"), MB_OK);
		break;
		case WM_CLOSE:
			wsprintf(szMsg, "Window is closing");
			MessageBox(hwnd, szMsg, TEXT("WM_CLOSE"), MB_OK);
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
