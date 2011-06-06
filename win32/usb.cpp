#include <windows.h>
#include <dbt.h>
#include <stdio.h>

const char winClass[] = "MyNotifyWindow";
const char winTitle[] = "WindowTitle";


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DEVICECHANGE:
		{
			if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
			{
					DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
                    if (header->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        DEV_BROADCAST_VOLUME* devNot = reinterpret_cast<DEV_BROADCAST_VOLUME*>(lParam);
						printf("%d\n",devNot->dbcv_unitmask);
                    }
			}
		}
		break;
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

void NotifyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= NULL;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= winClass;
    wcex.hIconSm		= NULL;

    RegisterClassEx(&wcex);
}

int main(int argc,char **argv)
{
    DEV_BROADCAST_DEVICEINTERFACE notificationFilter;

GUID guidForModemDevices = {0x2c7089aa, 0x2e0e, 0x11d1, 
	{0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4}};


    ZeroMemory( &notificationFilter, sizeof(notificationFilter) );
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid = guidForModemDevices;


 HINSTANCE hInstance = GetModuleHandle(NULL);
 NotifyRegisterClass(hInstance);
 HWND hWnd = CreateWindow(winClass, winTitle, WS_ICONIC, 0, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

 ShowWindow(hWnd, SW_NORMAL);

    HDEVNOTIFY hDevNotify = RegisterDeviceNotification( hWnd, 
        &notificationFilter,
        DEVICE_NOTIFY_WINDOW_HANDLE 
        );

    if(hDevNotify == NULL) 
    {
		return NULL;
    }
	MSG msg;
	int ret;
loop:

	WaitMessage();
	
	// update windows
	while(PeekMessage(&msg,NULL,0,0,0)) 
	{
		ret = (int)GetMessage(&msg,0,0,0);
		if (ret == -1) goto exit;
		if (!ret) goto exit;
		
		// let windows handle it.
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}			
	
	goto loop;

exit:

    return 0;
}