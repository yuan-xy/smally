// ===========================================================================
// File: W I N S I N K . C P P
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//
//  - CWinSink implements a simple object that traps MSHTML window events such as window::onload.
//    DHTML authors simply do the following in an HTML page:
//        function window_load()
//        {
//           alert("window::onload fired");
//        }
//
//        window.onload = window_load;
//
//    C++ developers do the following:
//        Instantiate a host object that implements IDispatch
//        QI an interface exposed by MSHTML for IConnectionPointContainer (ICPC)
//        Request a specific connection point via ICPC
//        Call Advise on the connect point, passing a reference to the host object
//
//        MSHTML will call the host's IDispatch::Invoke implementation when an event occurs.
//
//  - For a complete list of window events see dispinterface HTMLWindowEvents2 in mshtml.idl.
//
// Copyright 1995-2002 Microsoft Corporation.  All Rights Reserved.

#include <windows.h>
#include <mshtml.h>
#include <mshtmdid.h>
#include "common.h"
#include "winsink.h"
#include <iostream>
using namespace std;

// CODE REVIEW: Safe String Functions.
// Include safe string functions.
#include <strsafe.h>

HRESULT CWinSink::Init(IHTMLWindow2* pWin)
{
  HRESULT hr = NOERROR;
  LPCONNECTIONPOINTCONTAINER pCPC = NULL;

  if (m_pWin)
  {
    m_pWin->Release();
  }
  m_pWin = pWin;

  if (FAILED(hr = pWin->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC)))
  {
    goto Error;
  }

  if (FAILED(hr = pCPC->FindConnectionPoint(DIID_HTMLWindowEvents2, &m_pCP)))
  {
    goto Error;
  }

  m_hrConnected = m_pCP->Advise((LPUNKNOWN)this, &m_dwCookie);

Error:
  if (pCPC) pCPC->Release();
  return hr;
}

STDMETHODIMP CWinSink::QueryInterface(REFIID riid, LPVOID* ppv)
{
  *ppv = NULL;

  if (IID_IUnknown == riid)
  {
    *ppv = (LPUNKNOWN)this;
    AddRef();
    return NOERROR;
  }
  else if (IID_IDispatch == riid)
  {
    *ppv = (IDispatch*)this;
    AddRef();
    return NOERROR;
  }
  else
  {
    OLECHAR wszBuff[39];
    int i = StringFromGUID2(riid, wszBuff, 39);
    TCHAR szBuff[39];
    // CODE REVIEW: Dangerous API. Ensured dest buffer size is OK.
    i = WideCharToMultiByte(CP_ACP, 0, wszBuff, -1, szBuff, 39, NULL, NULL);
    ODS("CWinSink QI: "); ODS(szBuff); ODS("\n");
    return E_NOTIMPL;
  }
}

STDMETHODIMP_(ULONG) CWinSink::AddRef()
{
  TCHAR szBuff[255];
  // CODE REVIEW: Replace Banned API.
  // wsprintf(szBuff, "CWinSink refcount increased to %d\n", m_dwRef+1);
  StringCchPrintf(szBuff, 255, "CWinSink refcount increased to %d\n", m_dwRef+1);
  ODS(szBuff);
  return ++m_dwRef;
}

STDMETHODIMP_(ULONG) CWinSink::Release()
{
  TCHAR szBuff[255];

  if (--m_dwRef == 0)
  {
    ODS("Deleting CWinSink\n");
    delete this;
    return 0;
  }

  // CODE REVIEW: Replace Banned API.
  // wsprintf(szBuff, "CWinSink refcount reduced to %d\n", m_dwRef);
  StringCchPrintf(szBuff, 255, "CWinSink refcount reduced to %d\n", m_dwRef);
  ODS(szBuff);
  return m_dwRef;
}

// OA events are fired through the IDispatch::Invoke of the sink object
STDMETHODIMP CWinSink::Invoke(DISPID dispIdMember,
            REFIID riid,
            LCID lcid,
            WORD wFlags,
            DISPPARAMS __RPC_FAR *pDispParams,
            VARIANT __RPC_FAR *pVarResult,
            EXCEPINFO __RPC_FAR *pExcepInfo,
            UINT __RPC_FAR *puArgErr)
{
  if (!pVarResult)
  {
    return E_POINTER;
  }

  switch(dispIdMember)
  {

  case DISPID_HTMLWINDOWEVENTS2_ONLOAD:
    cout << "HTMLWindowEvents2::onload fired" << endl;
    break;
  default:
    return DISP_E_MEMBERNOTFOUND;
  }

  return NOERROR;
}