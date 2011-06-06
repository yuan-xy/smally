#include <windows.h>
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("drag") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
 //    OleInitialize(0);
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Program requires Windows NT!"),  szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("drag and drop demo"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;
          
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     DragAcceptFiles(hwnd,TRUE);
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

void f(HDROP  hdrop){
	UINT size = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
	TCHAR str[1000];
	sprintf(str,"%d",size);
    MessageBox(0,TEXT("receiving WM_DROPFILES message!"),str,0);
	{
		int i;
		for(i=0;i<size;i++){
			DragQueryFile(hdrop, i, str,1000);
			MessageBox(0,str,str,0);
		}
	}
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL fFlipFlop = FALSE ;
     HBRUSH      hBrush ;
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rc ;
     
     switch (message)
     {
          case WM_DROPFILES:
				 f((HDROP)wParam);
                 return 0;      
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
