
#include <Windows.h>
#include <gdiplus.h>
//#include <objidl.h>

#pragma comment (lib,"Gdiplus.lib")

void OnPain(HDC hdc, int iStartX, int iStartY, int iEndX, int iEndY)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(iStartX, iStartY, iEndX), 20.0f);
	graphics.DrawLine(&pen, iStartX, iStartY, iEndX, iEndY);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc = GetDC(hWnd);
	PAINTSTRUCT  ps;

	int iMouseX = 0;
	int iMouseY = 0;

	static int iStartX;
	static int iStartY;

	static int iEndX;
	static int iEndY;

	static bool bDrawing = false;

	int iButtons = static_cast<int>(wParam);

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_MOUSEMOVE:
	{
		// get the position of the mouse
		iMouseX = static_cast<int>(LOWORD(lParam));
		iMouseY = static_cast<int>(HIWORD(lParam));

		if (bDrawing)
		{
			iEndX = iMouseX;
			iEndY = iMouseY;


			//OnPain(hdc, iStartX, iStartY, iEndX, iEndY);
			InvalidateRect(hWnd, NULL, FALSE);

			if (iButtons & MK_CONTROL)
			{
				//OnPain(hdc, iStartX, iStartY, iEndX, iEndY);
				//InvalidateRect(hWnd, NULL, TRUE);
			}
			
		}

		if (iButtons & MK_CONTROL)
		{
			//iStartX = static_cast<int>(LOWORD(lParam));
			//iStartY = static_cast<int>(HIWORD(lParam));
		}
	}
	break;

	case WM_LBUTTONDOWN:

		iStartX = static_cast<int>(LOWORD(lParam));
		iStartY = static_cast<int>(HIWORD(lParam));

		bDrawing = true;

	break;

	case WM_LBUTTONUP:
		//InvalidateRect(hWnd, NULL, TRUE);

		OnPain(hdc, iStartX, iStartY, iEndX, iEndY);
		InvalidateRect(hWnd, NULL, FALSE);

		bDrawing = false;
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}