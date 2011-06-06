#include <windows.h>
#include "webform.h"

HINSTANCE hInstance;
HWND hMain; // Our main window
HWND hwebf; // We declare this handle globally, just for convenience


bool loaded, has_clicked, isquit = false; // we declare it as a global variable, for convenience

// This is the code for the message pump
void PumpMessages() {
	MSG msg;
	while (true) {
		BOOL res = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
		if (!res)
			return;
		if (msg.message == WM_QUIT) {
			isquit = true;
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK PlainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: { // We'll create a browser with a scroll-bar, so we use CreateWindow with WS_VSCROLL.
		// Otherwise WebformCreate is shorthand for the same thing but without scroll-bar.
		// hwebf = WebformCreate(hwnd,103);  // We pick 103 as the id for our child control
		hwebf = CreateWindow(WEBFORM_CLASS, _T("about:blank"),
				WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_VSCROLL, 0, 0,
				100, 100, hwnd, (HMENU) 103, hInstance, 0);
		WebformGo(hwebf,
				_T("E:\\firebreath-1.4\\projects\\PluginGigaso\\search.htm"));
		//SetTimer(hwnd,1,20000,0);
	}
		break;
	case WM_TIMER: {
		KillTimer(hwnd, 1);
		WebformReady(hwebf);
		WebformSet(hwebf,_T("<html><head><script type='text/javascript'>\n")
				_T("function MyJavascriptFunc(arg) {alert(arg);}\n")
				_T("</script></head><body><form>\n")
				_T("<input name='cx' type='checkbox' checked> Do you want some?<br>\n")
				_T("<input name='tx' type='text' value='Come and get it!'><br>\n")
				_T("<input name='sub' type='submit' value='Ok'>\n")
				_T("<input name='sub' type='submit' value='Cancel'>\n")
				_T("<br><div id='id'>hello<div>\n")
				_T("</form></body></html>\n"));

		loaded = false;
		has_clicked = false;
		WebformGo(hwebf, 0);
		while (!loaded && !isquit) {
			PumpMessages();
			Sleep(5);
		}
		if (isquit) {
			PostQuitMessage(0);
			return 0;
		}

		// Let's alter a property of something on the form
		IHTMLDivElement *e = WebformGetElement < IHTMLDivElement > (hwebf, _T(
				"id"));
		if (e != 0) {
			e->put_align(L"right");
			e->Release();
		}
		//
		// And alter the text: document.all["id"].innerHTML="fred<br>,ary";
		IHTMLElement *e2 = WebformGetElement < IHTMLElement > (hwebf, _T("id"));
		if (e2 != 0) {
			e2->put_innerHTML(L"fred<br>mary");
			e2->Release();
		}

		// First thing we'll do, once the page is loaded, is run a script on it.
		// Note: we used the loaded/PumpMessages trick to make it look as if WebformGo
		// is synchronous. But it's not, really, and hence the isquit trick.
		IHTMLDocument2 *doc = WebformGetDoc(hwebf);
		IHTMLWindow2 *win = 0;
		doc->get_parentWindow(&win);
		if (win != 0) {
			BSTR cmd = SysAllocString(
					L"MyJavascriptFunc('hello from javascript')");
			VARIANT v;
			VariantInit(&v);
			win->execScript(cmd, NULL, &v);
			VariantClear(&v);
			SysFreeString(cmd);
			win->Release();
		}
		doc->Release();

		// Now let's wait until the user submits the form, and query the results
		while (!has_clicked && !isquit) {
			PumpMessages();
			Sleep(5);
		} // Now it's been clicked!
		while (!loaded && !isquit) {
			PumpMessages();
			Sleep(5);
		}
		//
		const TCHAR *cx = WebformGetItem(hwebf, _T("cx")); // cx will point to "on" or be empty
		const TCHAR *tx = WebformGetItem(hwebf, _T("tx")); // tx will point to the text of the input
		const TCHAR *sub = WebformGetItem(hwebf, _T("sub")); // sub will point to "Ok"
		if (sub != 0 && _tcscmp(sub, _T("Ok")) == 0)
			MessageBox(hwnd, tx ? tx : _T(""), cx ? cx : _T(""), MB_OK);
		//
		// We could also have queried the inputs using DOM:
		IHTMLInputTextElement *inptxt = WebformGetElement
				< IHTMLInputTextElement > (hwebf, _T("tx"));
		if (inptxt != 0) {
			IHTMLTxtRange *range = 0;
			inptxt->createTextRange(&range);
			if (range != 0) {
				BSTR b = 0;
				range->get_text(&b);
				if (b != 0) {
					MessageBoxW(hwnd, b, L"Contents of text box", MB_OK);
					SysFreeString(b);
				}
				range->Release();
			}
			inptxt->Release();
		}

		// Here's another page. We're going to query the title of the page.
		loaded = false;
		WebformGo(hwebf, _T("http://www.wischik.com/lu/Programmer"));
		while (!loaded && !isquit) {
			PumpMessages();
			Sleep(5);
		}
		if (isquit) {
			PostQuitMessage(0);
			return 0;
		}
		doc = WebformGetDoc(hwebf);
		BSTR b = 0;
		doc->get_title(&b);
		MessageBoxW(hwnd, b, L"Title:", MB_OK);
		if (b != 0)
			SysFreeString(b);
		doc->Release();

	}
		break;
	case WM_SIZE: {
		MoveWindow(hwebf, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	}
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		FillRect(ps.hdc, &ps.rcPaint, (HBRUSH) GetStockObject(WHITE_BRUSH));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_COMMAND: {
		int id = LOWORD(wParam), code = HIWORD(wParam);
		// The webf control does not auto-navigate. We must do that manually.
		// This gives us precise control over its behaviour. In this case,
		// we redirect any CNN-visits with the BBC. Hooray for quality broadcasting!
		if (id == 103 && code == WEBFN_LOADED) {
			loaded = true;
		} else if (id == 103 && code == WEBFN_CLICKED) {
			if (!has_clicked) {
				has_clicked = true;
				break;
			} // see the dialog code above.
			//
			const TCHAR *url = WebformLastClick(hwebf);
			if (_tcscmp(url, _T("http://cnn.com")) == 0)
				WebformGo(hwebf, _T("http://bbc.co.uk"));
			else
				WebformGo(hwebf, url);
		}
	}
		break;
	case WM_SYSKEYDOWN: // respond to keypresses
	{
		WPARAM key = wParam;
		bool isalt = (lParam & 0x20000000) != 0;
		if (isalt && key == VK_LEFT) {
			IWebBrowser2 *ibrowser = WebformGetBrowser(hwebf);
			ibrowser->GoBack();
			ibrowser->Release();
		} else if (isalt && key == VK_RIGHT) {
			IWebBrowser2 *ibrowser = WebformGetBrowser(hwebf);
			ibrowser->GoForward();
			ibrowser->Release();
		}
	}
		break;
	case WM_KEYDOWN: {
		WPARAM key = wParam; // 0xA6=VK_BROWSER_BACK, 0xA7=VK_BROWSER_FORWARD
		if (key == 0xA6) {
			IWebBrowser2 *ibrowser = WebformGetBrowser(hwebf);
			ibrowser->GoBack();
			ibrowser->Release();
		}
		if (key == 0xA7) {
			IWebBrowser2 *ibrowser = WebformGetBrowser(hwebf);
			ibrowser->GoForward();
			ibrowser->Release();
		}
		if (key == VK_BACK) { // If focus is in a text-element, then Backspace will delete characters.
			// Otherwise, it will go to the previous page.
			bool hastextfocus = false;
			IHTMLDocument2 *doc = WebformGetDoc(hwebf);
			IHTMLElement *focus = 0;
			doc->get_activeElement(&focus);
			if (focus != 0) {
				IHTMLTextElement *text = 0;
				focus->QueryInterface(IID_IHTMLInputTextElement, (void**) &text);
				if (text != 0) {
					hastextfocus = true;
					text->Release();
				}
				focus->Release();
			}
			doc->Release();
			if (!hastextfocus) {
				IWebBrowser2 *ibrowser = WebformGetBrowser(hwebf);
				ibrowser->GoBack();
				ibrowser->Release();
			}
		}
	}
		break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE h,HINSTANCE,LPSTR,int)
{	hInstance=h;
	OleInitialize(0);
	//
	WNDCLASSEX wcex; ZeroMemory(&wcex,sizeof(wcex)); wcex.cbSize = sizeof(WNDCLASSEX);
	BOOL res=GetClassInfoEx(hInstance,_T("PlainClass"),&wcex);
	if (!res)
	{	wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)PlainWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = _T("PlainClass");
		wcex.hIconSm = NULL;
		ATOM res=RegisterClassEx(&wcex);
		if (res==0) {MessageBox(NULL,_T("Failed to register class"),_T("Error"),MB_OK); return 0;}
	}
	//
	hMain = CreateWindowEx(0,_T("PlainClass"), _T("Plain Window"), WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
			CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
	if (hMain==NULL) {MessageBox(NULL,_T("Failed to create window"),_T("Error"),MB_OK); return 0;}
	ShowWindow(hMain,SW_SHOW);
	//
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{	TranslateMessage(&msg);
		// The problem is that keypresses won't always go to the main window.
		// So we insert this into our global message loop, to route them there.
		if (msg.hwnd!=hMain && msg.message>=WM_KEYFIRST && msg.message<=WM_KEYLAST) SendMessage(hMain, msg.message, msg.wParam, msg.lParam);
		DispatchMessage(&msg);
	}
	//
	OleUninitialize();
	return (int)msg.wParam;
}

