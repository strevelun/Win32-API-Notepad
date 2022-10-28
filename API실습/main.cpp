#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HINSTANCE hInst;
#define LINE		5
#define MAX_LEN		10

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) // 등록
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindow( // 생성 (WM_CREATE)
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 600,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow); // 출력
	UpdateWindow(hWnd); // WM_PAINT 메시지 즉시 처리

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); // 읽지 않은 메시지 WndProc으로 보냄
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
    static int xPos, yPos;
    static wchar_t str[LINE][MAX_LEN+1];
	static wchar_t counter[LINE];
    static int  line;
    static SIZE size;

    switch (message)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);

		GetTextExtentPoint(hdc, str[line], wcslen(str[line]), &size);

		for (int i = 0; i <= line; i++)
		{
			int idx = i * 20;
			TextOut(hdc, 0, idx, str[i], _tcslen(str[i]));
		}

		SetCaretPos(size.cx, yPos);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_CHAR:
        if (wParam == VK_BACK)
        {
			if (counter[line] <= 0 && line > 0)
			{
				yPos -= 20;
				line--;
			}			
			
			if (counter[line] > 0)
			{
				counter[line]--;
				str[line][counter[line]] = NULL;
			}
        }
        else if (wParam == VK_RETURN)
        {
			if (line < LINE-1) 
			{
				str[line][counter[line]] = NULL;
				line++;
				yPos += 20;
			}
        }
		else
		{
			if (counter[line] < MAX_LEN)
			{
				str[line][counter[line]++] = wParam;
			}
			else
			{
				if (line < LINE - 1)
				{
					str[line][counter[line]] = NULL;
					line++;
					str[line][counter[line]++] = wParam;
					yPos += 20;
				}
			}
		}
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}