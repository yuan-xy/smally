#include "windows.h"

#include "shlwapi.h"


HRESULT GetBrowserVersion(LPDWORD pdwMajor, LPDWORD
        pdwMinor, LPDWORD pdwBuild)
{
    HINSTANCE   hBrowser;

    if(IsBadWritePtr(pdwMajor, sizeof(DWORD))
        || IsBadWritePtr(pdwMinor, sizeof(DWORD))
        || IsBadWritePtr(pdwBuild, sizeof(DWORD)))
        return E_INVALIDARG;

    *pdwMajor = 0;
    *pdwMinor = 0;
    *pdwBuild = 0;

    //Load the DLL.

    hBrowser = LoadLibrary(TEXT("shdocvw.dll"));

    if(hBrowser) 
    {

        HRESULT  hr = S_OK;
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion =
            (DLLGETVERSIONPROC)GetProcAddress(hBrowser,
                TEXT("DllGetVersion"));

        if(pDllGetVersion) 
        {

            DLLVERSIONINFO    dvi;
            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);
            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr)) 
            {
                *pdwMajor = dvi.dwMajorVersion;
                *pdwMinor = dvi.dwMinorVersion;
                *pdwBuild = dvi.dwBuildNumber;
            }

        } 
        else 
        {
            //If GetProcAddress failed, there is a problem 

            // with the DLL.


            hr = E_FAIL;
        }
        FreeLibrary(hBrowser);
        return hr;
    }
    return E_FAIL;
}

int main(){
	DWORD pdwMajor, pdwMinor, pdwBuild;
	GetBrowserVersion(&pdwMajor, &pdwMinor, &pdwBuild);
	printf("%d,%d,%d\n",pdwMajor, pdwMinor, pdwBuild);
}