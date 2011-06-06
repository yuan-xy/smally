// ===========================================================================
// File: W I N S I N K . H
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright 1995-2002 Microsoft Corporation.  All Rights Reserved.

#ifndef _WinSink_h_
#define _WinSink_h_

class CWinSink : public IDispatch
{
public:
	CWinSink() : m_pWin(NULL), m_dwRef(1),
			m_hrConnected(CONNECT_E_CANNOTCONNECT),
			m_dwCookie(0), m_pCP(NULL)
	{
	}

	~CWinSink()
	{
	}

	HRESULT Init(IHTMLWindow2* pWin);
	HRESULT Passivate()
	{
		HRESULT hr = NOERROR;
		if (m_pCP)
		{
			if (m_dwCookie)
			{
				hr = m_pCP->Unadvise(m_dwCookie);
				m_dwCookie = 0;
			}

			 m_pCP->Release();
			 m_pCP = NULL;
		}

		if (m_pWin)
		{
			m_pWin->Release();
			m_pWin = NULL;
		}

		return NOERROR;
	}

	// IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)(); 

	// IDispatch method
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
			{ ODS("GetTypeInfoCount\n"); return E_NOTIMPL; }

	STDMETHOD(GetTypeInfo)(UINT iTInfo,
            LCID lcid,
            ITypeInfo** ppTInfo)
			{ ODS("GetTypeInfo\n"); return E_NOTIMPL; }

	STDMETHOD(GetIDsOfNames)(REFIID riid,
            LPOLESTR* rgszNames,
            UINT cNames,
            LCID lcid,
            DISPID* rgDispId)
			{ ODS("GetIDsOfNames\n"); return E_NOTIMPL; }
        
	STDMETHOD(Invoke)(DISPID dispIdMember,
            REFIID riid,
            LCID lcid,
            WORD wFlags,
            DISPPARAMS __RPC_FAR *pDispParams,
            VARIANT __RPC_FAR *pVarResult,
            EXCEPINFO __RPC_FAR *pExcepInfo,
            UINT __RPC_FAR *puArgErr);

protected:
	IHTMLWindow2* m_pWin;
	DWORD m_dwRef;

	LPCONNECTIONPOINT m_pCP;
	HRESULT m_hrConnected;
	DWORD m_dwCookie;

};

#endif