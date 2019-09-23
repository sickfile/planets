#define STRICT
#include <windows.h>
#include "resource.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "sframe.h"

#pragma resource "planets.res"

typedef enum {sNone, sConfig, sPassword, sPreview, sSaver} TScrMode;
TScrMode ScrMode = sNone;

int WINAPI WinMain (HINSTANCE h, HINSTANCE hp, char *CmdLine, int CmdShow)
{
	HWND hwind;
	MSG messages;
	DWORD i = 0;
	SetLastError (0);
	SetPriorityClass (GetCurrentProcess (), IDLE_PRIORITY_CLASS);
	SetThreadPriority (GetCurrentThread (), THREAD_BASE_PRIORITY_IDLE);
	if (!ldRAW ())
	{
		MessageBox (0, "Mission Failure", "CHEERS", MB_OK);
		return 0;
	}
	if (CmdLine)
	{
		while (CmdLine[i])
		{
			switch (CmdLine[i])
			{
				case 'c': case 'C':
				{
					ScrMode = sConfig;
				}
				break;
				case 's': case 'S':
				{
					ScrMode = sSaver;
				}
				break;
				case 'p': case 'P': case 'l': case 'L':
				{
					ScrMode = sPreview;
				}
				break;
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				{
					DWORD n = 0;
					for (; CmdLine[i] >= '0' && CmdLine[i] <= '9'; i++)
						n = 10 * n + CmdLine[i] - '0';
					prevWnd = (HWND)n;
				}
				break;
			}
			++i;
		}
	}
	if (!i)
	{
		ScrMode = sConfig;
	}
	else
	switch (ScrMode)
	{
		case sPreview:
		{
			HDC DeskDC = GetDC (NULL);
			int b = GetDeviceCaps (DeskDC, BITSPIXEL);
			GetClientRect (prevWnd, &wr);
			ReleaseDC (hwind, DeskDC);
			if (!CreateGLWindow(h, b, FALSE))
			{
				return 0;									// Quit If Window Was Not Created
			}
		}
		break;
		case sSaver:
		{
			HDC DeskDC = GetDC (NULL);
			int b = GetDeviceCaps (DeskDC, BITSPIXEL);
			wr.top = wr.left = 0;
			wr.right = GetDeviceCaps (DeskDC, HORZRES);
			wr.bottom = GetDeviceCaps (DeskDC, VERTRES);
			ReleaseDC (hwind, DeskDC);
			if (!CreateGLWindow(h, b, TRUE))
			{
				return 0;									// Quit If Window Was Not Created
			}
		}
		break;
		case sConfig:
		{
			MessageBox (0, "nothing to configure", "NOTHING", MB_OK);
			return 0;
		}
		default:
		{
			MessageBox (0, CmdLine, "Trt Mrt", MB_OK);
			return -1;
		}
	}
	if (ScrMode == sSaver)
	{
		while (working)
		{
			if (PeekMessage (&messages, NULL, 0, 0, PM_REMOVE))
			{
				if (messages.message == WM_QUIT)
				{
					working = FALSE;
				}
				else
				{
					TranslateMessage (&messages);
					DispatchMessage (&messages);
				}
			}
			else
			{
				if (ok)
				{
					DrawGLScene ();
					SwapBuffers (hDC);
				}
			}
		}
	}
	else
	{
		while (GetMessage (&messages, NULL, 0, 0))
		{
			TranslateMessage (&messages);
			DispatchMessage (&messages);
		}
	}
	return messages.wParam;
}

LRESULT CALLBACK ScreenSaverProc (HWND hwind, UINT messag, WPARAM wParam, LPARAM lParam)
{
	static int IgnoreMouse = 10;

	switch (messag)
	{
		case WM_ACTIVATE:
		{
			if (!HIWORD (wParam))
			{
				active = TRUE;
			}
			else
			{
				active = FALSE;
			}
		}
		break;
		case WM_TIMER:
		{
			DrawGLScene ();
			SwapBuffers (hDC);
		}
		return 0;
		case WM_CREATE:
		{
			if (ScrMode == sSaver)
			{
				ShowCursor (NULL);
			}
			else
			{
				SetTimer (hwind, 1, 100, NULL);
			}
			ok = TRUE;
		}
		return 0;
		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
		}
		return 0;								// Jump Back
		case WM_MOUSEMOVE:
		{
			if (ScrMode == sSaver)
			{
				if (IgnoreMouse)
					IgnoreMouse--;
				else
					PostMessage (hwind, WM_CLOSE, 0, 0);
				return 0;
			}
		}
		break;
		case WM_KEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			if (ScrMode == sSaver) PostMessage (hwind, WM_CLOSE, 0, 0);
		}
		break;
		case WM_DESTROY:
		{
			if (ScrMode == sPreview)
				KillTimer (hwind, 1);
			PostQuitMessage (0);
		}
		return 0;
	}
	return DefWindowProc (hwind, messag, wParam, lParam);
}
