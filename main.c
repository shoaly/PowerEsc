
#define WINVER          0x0500
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#ifdef DEBUG 
	#define trace printf
#else
	#define trace(...) (void)0
#endif

#define APPNAME "PowerESC"

static HHOOK mHook = NULL;
static WPARAM mTriggerKeyStatus = WM_KEYUP;

static uint8_t mPassthroughOnce = 0;
static uint8_t mIgnoreTriggerKey = 0;

static void SendKey(WPARAM wParam, WORD wVk)
{
    INPUT input = {0};
	if (wParam == WM_KEYUP) input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = wVk;
    input.ki.wScan = MapVirtualKeyExW(wVk, 0, 0);
    SendInput(1, &input, sizeof(INPUT));
}
/**
 * @param wParam  WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP.
 * @param lParam  A pointer to a KBDLLHOOKSTRUCT structure.
 */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	KBDLLHOOKSTRUCT *p;
	DWORD vkCode;
	
	if (nCode >= 0) {
		p = (KBDLLHOOKSTRUCT *) lParam;
		trace("nCode=%d, wParam=%d, lParam=0x%ld, vk=0x%02x, sc=0x%02x\n", 
				nCode, wParam, lParam, p->vkCode, p->scanCode);
		vkCode = p->vkCode;
		if (mPassthroughOnce) {
			trace("passthrough any key\n");
			mPassthroughOnce = 0; 
		} else if (vkCode == VK_ESCAPE) { // 触发键 or 'F'
			// 触发键弹起
			if (wParam == WM_KEYUP) {
				if (mIgnoreTriggerKey) {
					mIgnoreTriggerKey = 0;
				} else { // 这个地方在触发键弹起的时候才真正触发一次, 也就是抹去了触发键按下就触发的功能,  
					     //也抹杀了触发键按下之后连续触发的功能
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, vkCode);
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, vkCode);
				}
			}
			mTriggerKeyStatus = wParam;
			return 1;
		} else if (mTriggerKeyStatus == WM_KEYDOWN) {
			mIgnoreTriggerKey = 1;
			/** replace vkCode and scanCode not work, intercepting it and resend an INPUT event. */
			switch (vkCode)
			{
			case 'H':
				trace("H\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_LEFT);
				break;
			case 'J':
				trace("J\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_DOWN);
				break;
			case 'K':
				trace("K\n")            ;
				mPassthroughOnce = 1;
				SendKey(wParam, VK_UP);
				break;
			case 'L':
				trace("L\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_RIGHT);
				break;
			case 'A':
				trace("A\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_HOME);
				break;
			case 'E':
				trace("E\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_END);
				break;
			default:
				mPassthroughOnce = 1;
				SendKey(wParam, vkCode);
				break;
			}
			return 1;
		}
	}
	return CallNextHookEx(mHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MainLoop(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hwnd, message, wParam, lParam );
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    static TCHAR szAppName[] = TEXT(APPNAME);
	HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = MainLoop;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    RegisterClass( &wndclass );
    hwnd = CreateWindow(szAppName,
						 szAppName,
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         NULL,
                         NULL,
                         hInstance,
                         NULL );
    SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

	mHook = SetWindowsHookEx( 
        WH_KEYBOARD_LL, 
        KeyboardProc,
        hInstance,
		0
    ); 

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return msg.wParam;
}
