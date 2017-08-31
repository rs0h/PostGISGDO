// Copyright 2011 Intergraph Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "../Common/Errors.hpp"
#include "../Common/LogFiles.hpp"
#include "GMPsgCtlIntfc.hpp"
#include "GMPsgCtl.rh"

#include <stdio.h>
#include <tchar.h>
#include <olectl.h>

BOOL CALLBACK ConnPropsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hwndDlg, CTL_SERVEREDT, EM_LIMITTEXT, 63, 0);
        SendDlgItemMessage(hwndDlg, CTL_PORTEDT, EM_LIMITTEXT, 15, 0);
        SendDlgItemMessage(hwndDlg, CTL_DBNAMEEDT, EM_LIMITTEXT, 63, 0);
        SendDlgItemMessage(hwndDlg, CTL_USERNAMEEDT, EM_LIMITTEXT, 63, 0);
        SendDlgItemMessage(hwndDlg, CTL_USERPASSWDEDT, EM_LIMITTEXT, 63, 0);
        return(0);
    /*case WM_GETDLGCODE:
        SetWindowLongPtr(hwndDlg, DWL_MSGRESULT, DLGC_WANTTAB);
        return(0);*/
    default:
        return(0);
    }
}

BOOL CALLBACK AboutBoxDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        return(0);
    /*case WM_GETDLGCODE:
        SetWindowLongPtr(hwndDlg, DWL_MSGRESULT, DLGC_WANTTAB);
        return(0);*/
    case WM_COMMAND:
        if(LOWORD(wParam) == IDOK) EndDialog(hwndDlg, 0);
        return(0);
    default:
        return(0);
    }
}


//  _GMPostGISNativeQueryProps

/*_GMPostGISNativeQueryProps::_GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib) : CCOMDispatch(selfptr, ALib, 0)
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISNativeQueryProps::_GMPostGISNativeQueryProps-1\r\n", true);
#endif // DBGLEVEL
}

_GMPostGISNativeQueryProps::_GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib, int iIndex) : CCOMDispatch(selfptr, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISNativeQueryProps::_GMPostGISNativeQueryProps-2\r\n", true);
#endif // DBGLEVEL
}

_GMPostGISNativeQueryProps::~_GMPostGISNativeQueryProps()
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISNativeQueryProps::~_GMPostGISNativeQueryProps\r\n", true);
#endif // DBGLEVEL
}

HRESULT _GMPostGISNativeQueryProps::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("_GMPostGISNativeQueryProps::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID__GMPostGISNativeQueryProps))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else hres = E_NOINTERFACE;
    }
    return(hres);
}

ULONG _GMPostGISNativeQueryProps::Release()
{
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "_GMPostGISNativeQueryProps::Release - %d\r\n", m_lRefCount);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete(this);
    return(0);
}

HRESULT _GMPostGISNativeQueryProps::AddToCallbackList(_IChangePropertiesCallback* Cmd)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::AddToCallbackList\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::RemoveFromCallbackList(_IChangePropertiesCallback* Cmd)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::RemoveFromCallbackList\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::set_OutputRecordset(GRecordset* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::set_OutputRecordset\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::get_OutputRecordset(GRecordset* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::get_OutputRecordset\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::set_InputConnection(DGMConnection* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::set_InputConnection\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::get_InputConnection(DGMConnection* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::get_InputConnection\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::get_CoordSystemsMgr(DGMCoordSystemsMgr* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::get_CoordSystemsMgr\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::set_CoordSystemsMgr(DGMCoordSystemsMgr* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::set_CoordSystemsMgr\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::get_TemporaryRecordset(GRecordset* *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::get_TemporaryRecordset\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::get_hWnd(long *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::get_hWnd\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::StoreDefaults(BSTR *prefix, IDispatch * *PrefSet)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::StoreDefaults\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::RestoreDefaults(BSTR *prefix, IDispatch * *PrefSet)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::RestoreDefaults\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT _GMPostGISNativeQueryProps::ShowAboutBox()
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISNativeQueryProps::ShowAboutBox\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}*/


// COleBase

COleBase::COleBase(_GMPostGISConnectionProps *pParent)
{
    m_pParent = pParent;
}

COleBase::~COleBase()
{
}

HRESULT COleBase::QueryInterface(REFIID iid, void **ppvObject)
{
    return(m_pParent->QueryInterface(iid, ppvObject));
}

ULONG COleBase::AddRef()
{
    return(m_pParent->AddRef());
}

ULONG COleBase::Release()
{
    return(m_pParent->Release());
}


// COleObject

COleObject::COleObject( _GMPostGISConnectionProps *pParent) : COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("COleObject::COleObject\r\n", true);
#endif // DBGLEVEL
    m_pOleClientSite = NULL;
    m_pAdviseSink = NULL;
    m_dwAdvToken = 0;
}

COleObject::~COleObject()
{
#if DBGLEVEL > 1
    WriteLogFile("COleObject::~COleObject\r\n", true);
#endif // DBGLEVEL
    if(m_pOleClientSite) m_pOleClientSite->Release();
    if(m_pAdviseSink) m_pAdviseSink->Release();
}

HRESULT COleObject::SetClientSite(IOleClientSite *pClientSite)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::SetClientSite\r\n", true);
#endif // DBGLEVEL
    if(m_pOleClientSite) m_pOleClientSite->Release();
    m_pOleClientSite = pClientSite;
    if(m_pOleClientSite) m_pOleClientSite->AddRef();
    return(S_OK);
}

HRESULT COleObject::GetClientSite(IOleClientSite **ppClientSite)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetClientSite\r\n", true);
#endif // DBGLEVEL
    if(m_pOleClientSite) m_pOleClientSite->AddRef();
    *ppClientSite = m_pOleClientSite;
    return(S_OK);
}

HRESULT COleObject::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::SetHostNames\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  szContainerApp: ", false);
    WriteLogFile((WCHAR*)szContainerApp, false);
    WriteLogFile("\r\n", false);
    WriteLogFile("  szContainerObj: ", false);
    WriteLogFile((WCHAR*)szContainerObj, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::Close(DWORD dwSaveOption)
{
#if DBGLEVEL > 0
    char buf[64];
    sprintf(buf, "COleObject::Close - %d\r\n", dwSaveOption);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    m_pParent->HideDialog(NULL);
    if(m_pAdviseSink)
    {
        m_pAdviseSink->OnClose();
        m_pAdviseSink->Release();
        m_pAdviseSink = NULL;
    }
    if(m_pOleClientSite)
    {
        m_pOleClientSite->OnShowWindow(FALSE);
        m_pOleClientSite->Release();
        m_pOleClientSite = NULL;
    }
    return(S_OK);
}

HRESULT COleObject::SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::SetMoniker\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetMoniker\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::InitFromData(IDataObject *pDataObject, BOOL fCreation,
    DWORD dwReserved)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::InitFromData\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetClipboardData\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite,
    LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
{
#if DBGLEVEL > 0
    char buf[64];
    sprintf(buf, "COleObject::DoVerb - iVerb: %d\r\n", iVerb);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    switch(iVerb)
    {
    case OLEIVERB_SHOW:
    case OLEIVERB_UIACTIVATE:
        m_pParent->ShowDialog(hwndParent, lprcPosRect);
        if(pActiveSite) pActiveSite->OnShowWindow(TRUE);
        return(S_OK);
    case OLEIVERB_INPLACEACTIVATE:
        return m_pParent->DoVerbInPlaceActivate (lpmsg, pActiveSite, lindex, hwndParent, lprcPosRect);
    case OLEIVERB_HIDE:
        m_pParent->HideDialog(hwndParent);
        if(pActiveSite) pActiveSite->OnShowWindow(FALSE);
        return(S_OK);
    }
    return(E_NOTIMPL);
}

#ifdef GCC
HRESULT COleObject::EnumVerbs(IEnumOleVerb **ppEnumOleVerb)
#else
HRESULT COleObject::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb)
#endif
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::EnumVerbs\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::Update()
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::Update\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::IsUpToDate()
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::IsUpToDate\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetUserClassID(CLSID *pClsid)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetUserClassID\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetUserType\r\n", true);
    char buf[32];
    sprintf(buf, "  FrmType: %d\r\n", dwFormOfType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    IMalloc *pMalloc = NULL;
    CoGetMalloc(1, &pMalloc);
    switch(dwFormOfType)
    {
    case USERCLASSTYPE_FULL:
        *pszUserType = (LPOLESTR)pMalloc->Alloc(48*sizeof(OLECHAR));
        wcscpy(*pszUserType, L"Intergraph GeoMedia PostGIS Connection Control");
        break;
    case USERCLASSTYPE_SHORT:
        *pszUserType = (LPOLESTR)pMalloc->Alloc(8*sizeof(OLECHAR));
        wcscpy(*pszUserType, L"PsgCtl");
        break;
    case USERCLASSTYPE_APPNAME:
        *pszUserType = (LPOLESTR)pMalloc->Alloc(10*sizeof(OLECHAR));
        wcscpy(*pszUserType, L"GeoMedia");
        break;
    default:
        *pszUserType = (LPOLESTR)pMalloc->Alloc(16*sizeof(OLECHAR));
        wcscpy(*pszUserType, L"Unknown form");
    }
    pMalloc->Release();
#if DBGLEVEL > 1
    WriteLogFile("    ", false);
    WriteLogFile(*pszUserType, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::SetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::SetExtent\r\n", true);
    char buf[64];
    sprintf(buf, "  w: %d, h: %d\r\n", psizel->cx, psizel->cy);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetExtent\r\n", true);
#endif // DBGLEVEL
    switch(dwDrawAspect)
    {
    case DVASPECT_CONTENT:
        m_pParent->GetExtent(&psizel->cx, &psizel->cy);
        break;
    case DVASPECT_THUMBNAIL:
        psizel->cx = 120;
        psizel->cy = 120;
        break;
    case DVASPECT_ICON:
        psizel->cx = 32;
        psizel->cy = 32;
        break;
    case DVASPECT_DOCPRINT:
        psizel->cx = 200;
        psizel->cy = 100;
        break;
    default:
        m_pParent->GetExtent(&psizel->cx, &psizel->cy);
    }
#if DBGLEVEL > 0
    char buf[64];
    sprintf(buf, "  w: %d, h: %d\r\n", psizel->cx, psizel->cy);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::Advise\r\n", true);
#endif // DBGLEVEL
    if(m_pAdviseSink) m_pAdviseSink->Release();
    m_pAdviseSink = NULL;
    m_dwAdvToken = 0;
    m_pAdviseSink = pAdvSink;
    if(m_pAdviseSink)
    {
        m_pAdviseSink->AddRef();
        m_dwAdvToken = 1;
        m_pAdviseSink->OnViewChange(1, -1);
    }
    if(pdwConnection) *pdwConnection = m_dwAdvToken;
    return(S_OK);
}

HRESULT COleObject::Unadvise(DWORD dwConnection)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::Unadvise\r\n", true);
#endif // DBGLEVEL
    if(m_pAdviseSink) m_pAdviseSink->Release();
    m_pAdviseSink = NULL;
    return(S_OK);
}

HRESULT COleObject::EnumAdvise(IEnumSTATDATA **ppenumAdvise)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::EnumAdvise\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleObject::GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::GetMiscStatus\r\n", true);
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "  dwAspect: %d\r\n", dwAspect);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    if(!pdwStatus) return(E_INVALIDARG);
    if(dwAspect == 1)
    {
        *pdwStatus = OLEMISC_RECOMPOSEONRESIZE | OLEMISC_INSIDEOUT |
            OLEMISC_CANTLINKINSIDE | OLEMISC_ACTIVATEWHENVISIBLE |
            OLEMISC_SETCLIENTSITEFIRST;
        return(S_OK);
    }
    return(S_FALSE);
}

HRESULT COleObject::SetColorScheme(LOGPALETTE *pLogpal)
{
#if DBGLEVEL > 0
    WriteLogFile("COleObject::SetColorScheme\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

IOleControlSite* COleObject::GetControlSite()
{
    IOleControlSite *pres = NULL;
    if(m_pOleClientSite)
        m_pOleClientSite->QueryInterface(IID_IOleControlSite, (void**)&pres);
    return(pres);
}

IOleInPlaceSite* COleObject::GetOleInPlaceSite()
{
    IOleInPlaceSite *pres = NULL;
    if(m_pOleClientSite)
        m_pOleClientSite->QueryInterface(IID_IOleInPlaceSite, (void**)&pres);
    return(pres);
}


// COleControl
COleControl::COleControl(_GMPostGISConnectionProps *pParent) : COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("COleControl::COleControl\r\n", true);
#endif // DBGLEVEL
}

COleControl::~COleControl()
{
#if DBGLEVEL > 1
    WriteLogFile("COleControl::~COleControl\r\n", true);
#endif // DBGLEVEL
}

HRESULT COleControl::GetControlInfo(CONTROLINFO* pCI)
{
#if DBGLEVEL > 0
    WriteLogFile("COleControl::GetControlInfo\r\n", true);
#endif // DBGLEVEL
    m_pParent->GetControlInfo(pCI);
    return(S_OK);
}

HRESULT COleControl::OnMnemonic(LPMSG pMsg)
{
#if DBGLEVEL > 0
    WriteLogFile("COleControl::OnMnemonic\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleControl::OnAmbientPropertyChange(DISPID dispID)
{
#if DBGLEVEL > 0
    WriteLogFile("COleControl::OnAmbientPropertyChange\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleControl::FreezeEvents(BOOL bFreeze)
{
#if DBGLEVEL > 0
    WriteLogFile("COleControl::FreezeEvents\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}


// COleInPlaceObject

COleInPlaceObject::COleInPlaceObject(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("COleInPlaceObject::COleInPlaceObject\r\n", true);
#endif // DBGLEVEL
}

COleInPlaceObject::~COleInPlaceObject()
{
#if DBGLEVEL > 1
    WriteLogFile("COleInPlaceObject::~COleInPlaceObject\r\n", true);
#endif // DBGLEVEL
}

HRESULT COleInPlaceObject::GetWindow(HWND * phwnd)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::GetWindow\r\n", true);
#endif // DBGLEVEL
    *phwnd = m_pParent->GetWindow();
    return(S_OK);
}

HRESULT COleInPlaceObject::ContextSensitiveHelp(BOOL fEnterMode)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::ContextSensitiveHelp\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceObject::InPlaceDeactivate()
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::InPlaceDeactivate\r\n", true);
#endif // DBGLEVEL
    return(m_pParent->InPlaceDeactivate());
}

HRESULT COleInPlaceObject::UIDeactivate()
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::UIDeactivate\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceObject::SetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::SetObjectRects\r\n", true);
#endif // DBGLEVEL
    m_pParent->SetPos(lprcPosRect->left, lprcPosRect->top);
    return(S_OK);
}

HRESULT COleInPlaceObject::ReactivateAndUndo()
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceObject::ReactivateAndUndo\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}


// COleInPlaceActiveObject

COleInPlaceActiveObject::COleInPlaceActiveObject(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("COleInPlaceActiveObject::COleInPlaceActiveObject\r\n", true);
#endif // DBGLEVEL
}

COleInPlaceActiveObject::~COleInPlaceActiveObject()
{
#if DBGLEVEL > 1
    WriteLogFile("COleInPlaceActiveObject::~COleInPlaceActiveObject\r\n", true);
#endif // DBGLEVEL
}

HRESULT COleInPlaceActiveObject::GetWindow(HWND * phwnd)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::GetWindow\r\n", true);
#endif // DBGLEVEL
    if(!phwnd) return(E_INVALIDARG);
    *phwnd = m_pParent->GetWindow();
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::ContextSensitiveHelp(BOOL fEnterMode)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::ContextSensitiveHelp\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::TranslateAccelerator(LPMSG lpmsg)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::TranslateAccelerator\r\n", true);
#endif // DBGLEVEL
    /*SendMessage(m_pParent->GetWindow(), lpmsg->message, lpmsg->wParam,
        lpmsg->lParam);*/
    m_pParent->TransAccel(lpmsg);
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::OnFrameWindowActivate(BOOL fActivate)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::OnFrameWindowActivate\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::OnDocWindowActivate(BOOL fActivate)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::OnDocWindowActivate\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::ResizeBorder(LPCRECT prcBorder,
    IOleInPlaceUIWindow *pUIWindow, BOOL fFrameWindow)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::ResizeBorder\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleInPlaceActiveObject::EnableModeless(BOOL fEnable)
{
#if DBGLEVEL > 0
    WriteLogFile("COleInPlaceActiveObject::EnableModeless\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}


// CViewObject2

CViewObject2::CViewObject2(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CViewObject2::CViewObject2\r\n", true);
#endif // DBGLEVEL
    m_pAdviseSink = NULL;
    m_dwAspect = 0;
    m_dwadvf = 0;
    m_dwFreeze = 0;
}

CViewObject2::~CViewObject2()
{
#if DBGLEVEL > 1
    WriteLogFile("CViewObject2::~CViewObject2\r\n", true);
#endif // DBGLEVEL
    if(m_pAdviseSink) m_pAdviseSink->Release();
}

#ifdef GCC
HRESULT CViewObject2::Draw(DWORD dwAspect, LONG lindex, void * pvAspect,
    DVTARGETDEVICE * ptd, HDC hicTargetDev, HDC hdcDraw,
    const LPRECTL lprcBounds, const LPRECTL lprcWBounds,
    __IView_pfncont pfnContinue, DWORD dwContinue)
#else
HRESULT CViewObject2::Draw(DWORD dwAspect, LONG lindex, void * pvAspect,
    DVTARGETDEVICE * ptd, HDC hicTargetDev, HDC hdcDraw,
    const LPRECTL lprcBounds, const LPRECTL lprcWBounds,
    BOOL (STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
    DWORD dwContinue)
#endif
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::Draw\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CViewObject2::GetColorSet(DWORD dwAspect, LONG lindex, void * pvAspect,
    DVTARGETDEVICE * ptd, HDC hicTargetDev, LOGPALETTE ** ppColorSet)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::GetColorSet\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CViewObject2::Freeze(DWORD dwAspect, LONG lindex, void * pvAspect,
    DWORD * pdwFreeze)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::Freeze\r\n", true);
#endif // DBGLEVEL
    m_dwFreeze++;
    if(pdwFreeze) *pdwFreeze = m_dwFreeze;
    return(S_OK);
}

HRESULT CViewObject2::Unfreeze(DWORD dwFreeze)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::Unfreeze\r\n", true);
#endif // DBGLEVEL
    m_dwFreeze--;
    return(S_OK);
}

HRESULT CViewObject2::SetAdvise(DWORD dwAspect, DWORD advf, IAdviseSink * pAdvSink)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::SetAdvise\r\n", true);
#endif // DBGLEVEL
    if(m_pAdviseSink) m_pAdviseSink->Release();
    m_pAdviseSink = NULL;
    m_dwAspect = 0;
    m_dwadvf = 0;

    if(dwAspect != DVASPECT_CONTENT) return(DV_E_DVASPECT);

    m_dwAspect = dwAspect;
    m_dwadvf = advf;
    m_pAdviseSink = pAdvSink;
    if(m_pAdviseSink)
    {
        m_pAdviseSink->AddRef();
        if(advf & ADVF_PRIMEFIRST) m_pAdviseSink->OnViewChange(dwAspect, -1);
    }
    return(S_OK);
}

HRESULT CViewObject2::GetAdvise(DWORD * pdwAspect, DWORD * padvf,
    IAdviseSink ** ppAdvSink)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::GetAdvise\r\n", true);
#endif // DBGLEVEL
    if(!*ppAdvSink) return(E_INVALIDARG);

    if(pdwAspect) *pdwAspect = m_dwAspect;
    if(padvf) *padvf = m_dwadvf;
    if(m_pAdviseSink) m_pAdviseSink->AddRef();
    *ppAdvSink = m_pAdviseSink;
    return(S_OK);
}

HRESULT CViewObject2::GetExtent(DWORD dwAspect, DWORD lindex,
    DVTARGETDEVICE *ptd, LPSIZEL lpsizel)
{
#if DBGLEVEL > 0
    WriteLogFile("CViewObject2::GetExtent\r\n", true);
#endif // DBGLEVEL
    if(!lpsizel) return(OLE_E_BLANK);
    switch(dwAspect)
    {
    case DVASPECT_CONTENT:
        m_pParent->GetExtent(&lpsizel->cx, &lpsizel->cy);
        break;
    case DVASPECT_THUMBNAIL:
        lpsizel->cx = 120;
        lpsizel->cy = 120;
        break;
    case DVASPECT_ICON:
        lpsizel->cx = 32;
        lpsizel->cy = 32;
        break;
    case DVASPECT_DOCPRINT:
        lpsizel->cx = 200;
        lpsizel->cy = 100;
        break;
    default:
        m_pParent->GetExtent(&lpsizel->cx, &lpsizel->cy);
    }
#if DBGLEVEL > 0
    char buf[64];
    sprintf(buf, "  w: %d, h: %d\r\n", lpsizel->cx, lpsizel->cy);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    return(S_OK);
}


// CPersist
CPersist::CPersist(_GMPostGISConnectionProps *pParent) : COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CPersist::CPersist\r\n", true);
#endif // DBGLEVEL
}

CPersist::~CPersist()
{
#if DBGLEVEL > 1
    WriteLogFile("CPersist::~CPersist\r\n", true);
#endif // DBGLEVEL
}

HRESULT CPersist::GetClassID(CLSID *pClassID)
{
#if DBGLEVEL > 0
    WriteLogFile("CPersist::GetClassID\r\n", true);
#endif // DBGLEVEL
    if(!pClassID) return(E_INVALIDARG);
    *pClassID = CLASS_GMPostGISConnectionProps;
    return(S_OK);
}


// CPersistStreamInit : public CPersist

CPersistStreamInit::CPersistStreamInit(_GMPostGISConnectionProps *pParent) :
    CPersist(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CPersistStreamInit::CPersistStreamInit\r\n", true);
#endif // DBGLEVEL
    m_bLoaded = false;
}

CPersistStreamInit::~CPersistStreamInit()
{
#if DBGLEVEL > 1
    WriteLogFile("CPersistStreamInit::~CPersistStreamInit\r\n", true);
#endif // DBGLEVEL
}

HRESULT CPersistStreamInit::IsDirty()
{
#if DBGLEVEL > 0
    WriteLogFile("CPersistStreamInit::IsDirty\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT CPersistStreamInit::Load(LPSTREAM pStm)
{
#if DBGLEVEL > 0
    WriteLogFile("CPersistStreamInit::Load\r\n", true);
#endif // DBGLEVEL
    if(m_bLoaded) return(E_UNEXPECTED);
    m_bLoaded = true;
    return(S_OK);
}

HRESULT CPersistStreamInit::Save(LPSTREAM pStm, BOOL fClearDirty)
{
#if DBGLEVEL > 0
    WriteLogFile("CPersistStreamInit::Save\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CPersistStreamInit::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
#if DBGLEVEL > 0
    WriteLogFile("CPersistStreamInit::GetSizeMax\r\n", true);
#endif // DBGLEVEL
    if(!pcbSize) return(E_INVALIDARG);
    pcbSize->LowPart = 0;
    pcbSize->HighPart = 0;
    return(S_OK);
}

HRESULT CPersistStreamInit::InitNew()
{
#if DBGLEVEL > 0
    WriteLogFile("CPersistStreamInit::InitNew\r\n", true);
#endif // DBGLEVEL
    if(m_bLoaded) return(E_UNEXPECTED);
    m_bLoaded = true;
    return(S_OK);
}

/*
// CRunnableObject : public

CRunnableObject::CRunnableObject(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CRunnableObject::CRunnableObject\r\n", true);
#endif // DBGLEVEL
}

CRunnableObject::~CRunnableObject()
{
#if DBGLEVEL > 1
    WriteLogFile("CRunnableObject::~CRunnableObject\r\n", true);
#endif // DBGLEVEL
}

HRESULT CRunnableObject::GetRunningClass(LPCLSID lpClsid)
{
#if DBGLEVEL > 0
    WriteLogFile("CRunnableObject::GetRunningClass\r\n", true);
#endif // DBGLEVEL
    *lpClsid = CLASS_GMPostGISConnectionProps;
    return(S_OK);
}

HRESULT CRunnableObject::Run(LPBC lpbc)
{
#if DBGLEVEL > 0
    WriteLogFile("CRunnableObject::Run\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

BOOL CRunnableObject::IsRunning()
{
#if DBGLEVEL > 0
    WriteLogFile("CRunnableObject::IsRunning\r\n", true);
#endif // DBGLEVEL
    return(TRUE);
}

HRESULT CRunnableObject::LockRunning(BOOL fLock, BOOL fLastUnlockCloses)
{
#if DBGLEVEL > 0
    WriteLogFile("CRunnableObject::LockRunning\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CRunnableObject::SetContainedObject(BOOL fContained)
{
#if DBGLEVEL > 0
    WriteLogFile("CRunnableObject::SetContainedObject\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}


// COleCache

COleCache::COleCache(_GMPostGISConnectionProps *pParent) : COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("COleCache::COleCache\r\n", true);
#endif // DBGLEVEL
}

COleCache::~COleCache()
{
#if DBGLEVEL > 1
    WriteLogFile("COleCache::~COleCache\r\n", true);
#endif // DBGLEVEL
}

HRESULT COleCache::Cache(FORMATETC * pFormatetc, DWORD advf, DWORD * pdwConnection)
{
#if DBGLEVEL > 0
    WriteLogFile("COleCache::Cache\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleCache::Uncache(DWORD dwConnection)
{
#if DBGLEVEL > 0
    WriteLogFile("COleCache::Uncache\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleCache::EnumCache(IEnumSTATDATA ** ppenumSTATDATA)
{
#if DBGLEVEL > 0
    WriteLogFile("COleCache::EnumCache\r\n", true);
#endif // DBGLEVEL
    *ppenumSTATDATA = NULL;
    return(S_OK);
}

HRESULT COleCache::InitCache(IDataObject * pDataObject)
{
#if DBGLEVEL > 0
    WriteLogFile("COleCache::InitCache\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT COleCache::SetData(FORMATETC * pFormatetc, STGMEDIUM * pmedium, BOOL fRelease)
{
#if DBGLEVEL > 0
    WriteLogFile("COleCache::SetData\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}


// CConnectionPointContainer

CConnectionPointContainer::CConnectionPointContainer(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CConnectionPointContainer::CConnectionPointContainer\r\n", true);
#endif // DBGLEVEL
}

CConnectionPointContainer::~CConnectionPointContainer()
{
#if DBGLEVEL > 1
    WriteLogFile("CConnectionPointContainer::~CConnectionPointContainer\r\n", true);
#endif // DBGLEVEL
}

HRESULT CConnectionPointContainer::EnumConnectionPoints(IEnumConnectionPoints **ppEnum)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPointContainer::EnumConnectionPoints\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CConnectionPointContainer::FindConnectionPoint(REFIID riid,
    IConnectionPoint **ppCP)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPointContainer::FindConnectionPoint\r\n", true);
    char buf[64];
    FormatGuid(buf, "  riid: ", "\r\n", riid);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    return(S_OK);
}


// CConnectionPoint
CConnectionPoint::CConnectionPoint(_GMPostGISConnectionProps *pParent) :
    COleBase(pParent)
{
#if DBGLEVEL > 1
    WriteLogFile("CConnectionPoint::CConnectionPoint\r\n", true);
#endif // DBGLEVEL
}

CConnectionPoint::~CConnectionPoint()
{
#if DBGLEVEL > 1
    WriteLogFile("CConnectionPoint::~CConnectionPoint\r\n", true);
#endif // DBGLEVEL
}

HRESULT CConnectionPoint::GetConnectionInterface(IID *pIID)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPoint::GetConnectionInterface\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CConnectionPoint::GetConnectionPointContainer(IConnectionPointContainer **ppCPC)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPoint::GetConnectionPointContainer\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CConnectionPoint::Advise(IUnknown *pUnk, DWORD *pdwCookie)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPoint::Advise\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CConnectionPoint::Unadvise(DWORD dwCookie)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPoint::Unadvise\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT CConnectionPoint::EnumConnections(IEnumConnections **ppEnum)
{
#if DBGLEVEL > 0
    WriteLogFile("CConnectionPoint::EnumConnections\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}
*/

//  _GMPostGISConnectionProps

_GMPostGISConnectionProps::_GMPostGISConnectionProps(HINSTANCE hInstance,
    IUnknown *pUnkOuter, ITypeLib *ALib) : CCOMDispatch(pUnkOuter, ALib, 0)
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISConnectionProps::_GMPostGISConnectionProps-1\r\n", true);
#endif // DBGLEVEL
    m_hInstance = hInstance;
    m_hWndDlg = NULL;
    m_bVisible = false;
    m_iWidth = 8328;
    m_iHeight = 3172;

    m_sAuxiliaryPath = NULL;
    wcscpy(m_sConnectType, L"GMPsgCtl.GMPostGISConnectionProps");
    m_bEnabled = VARIANT_TRUE;
    m_bInPlaceActive = VARIANT_FALSE;
    m_sServer[0] = 0;
    m_sPort[0] = 0;
    m_sDBName[0] = 0;
    m_sUserName[0] = 0;
    m_sPasswd[0] = 0;

    m_pOleObject = new COleObject(this);
    m_pOleControl = new COleControl(this);
    m_pOleInPlaceObject = new COleInPlaceObject(this);
    m_pOleInPlaceActiveObject = new COleInPlaceActiveObject(this);
    m_pViewObject = new CViewObject2(this);
    m_pPersistStreamInit = new CPersistStreamInit(this);
    /*m_pRunnableObject = new CRunnableObject(this);
    m_pOleCache = new COleCache(this);
    m_pConnectionPointContainer = new CConnectionPointContainer(this);*/
}

_GMPostGISConnectionProps::_GMPostGISConnectionProps(HINSTANCE hInstance,
    IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) : CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISConnectionProps::_GMPostGISConnectionProps-2\r\n", true);
#endif // DBGLEVEL
    m_hInstance = hInstance;
    m_hWndDlg = NULL;
    m_bVisible = false;
    m_iWidth = 8328;
    m_iHeight = 3172;

    m_sAuxiliaryPath = NULL;
    wcscpy(m_sConnectType, L"GMPsgCtl.GMPostGISConnectionProps");
    m_bEnabled = VARIANT_TRUE;
    m_bInPlaceActive = VARIANT_FALSE;
    m_sServer[0] = 0;
    m_sPort[0] = 0;
    m_sDBName[0] = 0;
    m_sUserName[0] = 0;
    m_sPasswd[0] = 0;

    m_pOleObject = new COleObject(this);
    m_pOleControl = new COleControl(this);
    m_pOleInPlaceObject = new COleInPlaceObject(this);
    m_pOleInPlaceActiveObject = new COleInPlaceActiveObject(this);
    m_pViewObject = new CViewObject2(this);
    m_pPersistStreamInit = new CPersistStreamInit(this);
    /*m_pRunnableObject = new CRunnableObject(this);
    m_pOleCache = new COleCache(this);
    m_pConnectionPointContainer = new CConnectionPointContainer(this);*/
}

_GMPostGISConnectionProps::~_GMPostGISConnectionProps()
{
#if DBGLEVEL > 1
    WriteLogFile("_GMPostGISConnectionProps::~_GMPostGISConnectionProps\r\n", true);
#endif // DBGLEVEL
    /*delete m_pConnectionPointContainer;
    delete m_pOleCache;
    delete m_pRunnableObject;*/
    delete m_pPersistStreamInit;
    delete m_pViewObject;
    delete m_pOleInPlaceActiveObject;
    delete m_pOleInPlaceObject;
    delete m_pOleControl;
    delete m_pOleObject;

    if(!m_bVisible && m_hWndDlg) DestroyWindow(m_hWndDlg);

    if(m_sAuxiliaryPath) free(m_sAuxiliaryPath);
}

HRESULT _GMPostGISConnectionProps::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("_GMPostGISConnectionProps::QueryInterface\r\n", true);
    char buf[64];
    FormatGuid(buf, "  iid: ", "\r\n", iid);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        *ppvObject = NULL;

        if(IsEqualIID(iid, DIID__GMPostGISConnectionProps))
            *ppvObject = this;
        else if(IsEqualIID(iid, IID_IOleObject))
            *ppvObject = m_pOleObject;
        else if(IsEqualIID(iid, IID_IOleControl))
            *ppvObject = m_pOleControl;
        else if(IsEqualIID(iid, IID_IOleWindow) ||
            IsEqualIID(iid, IID_IOleInPlaceObject))
            *ppvObject = m_pOleInPlaceObject;
        else if(IsEqualIID(iid, IID_IOleInPlaceActiveObject))
            *ppvObject = m_pOleInPlaceActiveObject;
        else if(IsEqualIID(iid, IID_IViewObject) ||
            IsEqualIID(iid, IID_IViewObject2))
            *ppvObject = m_pViewObject;
        else if(IsEqualIID(iid, IID_IPersist) ||
            IsEqualIID(iid, IID_IPersistStreamInit))
            *ppvObject = m_pPersistStreamInit;
        /*else if(IsEqualIID(iid, IID_IRunnableObject))
            *ppvObject = m_pRunnableObject;
        else if(IsEqualIID(iid, IID_IOleCache))
            *ppvObject = m_pOleCache;
        else if(IsEqualIID(iid, IID_IConnectionPointContainer))
            *ppvObject = m_pConnectionPointContainer;
        }*/

        if(*ppvObject)
        {
            hres = S_OK;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "UNSUPORTED INTERFACE iid: ", "\r\n", iid);
            WriteLogFile(buf, false);
#endif // DBGLEVEL
            hres = E_NOINTERFACE;
        }
    }
    return(hres);
}

ULONG _GMPostGISConnectionProps::Release()
{
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "_GMPostGISConnectionProps::Release - %d\r\n", m_lRefCount);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
    if(lRest > 0) return(lRest);
    delete(this);
    return(0);
}

HRESULT _GMPostGISConnectionProps::Invoke(DISPID dispIdMember, REFIID riid,
    LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
    EXCEPINFO *pExcepInfo, unsigned int *puArgErr)
{
#if DBGLEVEL > 2
    char buf[64];
    if(dispIdMember < 1000)
        sprintf(buf, "CCOMDispatch::Invoke - %d\r\n", dispIdMember);
    else sprintf(buf, "CCOMDispatch::Invoke - %.8X\r\n", dispIdMember);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(dispIdMember < 100)
    {
        switch(dispIdMember)
        {
        case DISPID_BACKCOLOR:
            pVarResult->vt = VT_I4;
            pVarResult->lVal = GetSysColor(COLOR_WINDOW);
            return(S_OK);
        case DISPID_FORECOLOR:
            pVarResult->vt = VT_I4;
            pVarResult->lVal = GetSysColor(COLOR_WINDOWTEXT);
            return(S_OK);
        case DISPID_ENABLED:
        case DISPID_ABOUTBOX:
        case DISPID_HWND:
            return(CCOMDispatch::Invoke(dispIdMember, riid, lcid, wFlags,
                pDispParams, pVarResult, pExcepInfo, puArgErr));
        }
        return(DISP_E_MEMBERNOTFOUND);
    }

    return(CCOMDispatch::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams,
        pVarResult, pExcepInfo, puArgErr));
}

HRESULT _GMPostGISConnectionProps::StoreDefaults(BSTR *prefix, IDispatch * *PrefSet)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::StoreDefaults\r\n", true);
#endif // DBGLEVEL
    // we don't need any defaults at the moment
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::RestoreDefaults(BSTR *prefix, IDispatch * *PrefSet)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::RestoreDefaults\r\n", true);
#endif // DBGLEVEL
    // we don't need any defaults at the moment
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_Location(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_Location\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);
    if(m_bVisible)
    {
        SendDlgItemMessage(m_hWndDlg, CTL_SERVEREDT, WM_GETTEXT, 64,
            (LPARAM)m_sServer);
        SendDlgItemMessage(m_hWndDlg, CTL_PORTEDT, WM_GETTEXT, 16,
            (LPARAM)m_sPort);
    }

    LPWSTR psPort = NULL;
    if(m_sPort[0])
    {
        int ilen = wcslen(m_sPort) - 1;
        int i = ilen - 1;
        while((i >= 0) && (m_sPort[i] == ' '))
        {
            m_sPort[i--] = 0;
            ilen--;
        }
        i = 0;
        while((i < ilen) && (m_sPort[i] == ' '))
        {
            i++;
        }
        ilen -= i;
        if(ilen > 0) psPort = &m_sPort[i];
    }

    if(psPort)
    {
        wchar_t buf[128];
        wcscpy(buf, m_sServer);
        wcscat(buf, L":");
        wcscat(buf, psPort);
        *Value = SysAllocString(buf);
    }
    else
    {
        *Value = SysAllocString(m_sServer);
        m_sPort[0] = 0;
    }
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::set_Location(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::set_Location\r\n", true);
#endif // DBGLEVEL
    if(!*Value)
    {
        m_sServer[0] = 0;
        m_sPort[0] = 0;
        return(S_OK);
    }

    int ilen = wcslen(*Value);
    LPWSTR sDblDot = wcschr(*Value, ':');

    if(sDblDot)
    {
        ilen = sDblDot - *Value;
        if(ilen > 63) ilen = 63;
        wcsncpy(m_sServer, *Value, ilen);
        m_sServer[ilen] = 0;
        sDblDot++;
        ilen = wcslen(sDblDot);
        if(ilen > 15) ilen = 15;
        wcsncpy(m_sPort, sDblDot, ilen);
        m_sPort[ilen] = 0;
    }
    else
    {
        m_sPort[0] = 0;
        if(ilen > 0)
        {
            if(ilen > 63) ilen = 63;
            wcsncpy(m_sServer, *Value, ilen);
            m_sServer[ilen] = 0;
        }
        else m_sServer[0] = 0;
    }
    if(m_bVisible)
    {
        SendDlgItemMessage(m_hWndDlg, CTL_SERVEREDT, WM_SETTEXT, 0,
            (LPARAM)m_sServer);
        SendDlgItemMessage(m_hWndDlg, CTL_PORTEDT, WM_SETTEXT, 0,
            (LPARAM)m_sPort);
    }
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_ConnectInfo(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_ConnectInfo\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);

    if(m_bVisible)
    {
        SendDlgItemMessage(m_hWndDlg, CTL_DBNAMEEDT, WM_GETTEXT, 64,
            (LPARAM)m_sDBName);
        SendDlgItemMessage(m_hWndDlg, CTL_USERNAMEEDT, WM_GETTEXT, 64,
            (LPARAM)m_sUserName);
        SendDlgItemMessage(m_hWndDlg, CTL_USERPASSWDEDT, WM_GETTEXT, 64,
            (LPARAM)m_sPasswd);
    }
    int slen = wcslen(m_sDBName) + wcslen(m_sUserName) + wcslen(m_sPasswd) + 2;
    LPWSTR sbuf = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
    swprintf(sbuf, L"%s/%s@%s", m_sUserName, m_sPasswd, m_sDBName);

    *Value = SysAllocString(sbuf);
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::set_ConnectInfo(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::set_ConnectInfo\r\n", true);
#endif // DBGLEVEL
    if(!*Value)
    {
        m_sDBName[0] = 0;
        m_sUserName[0] = 0;
        m_sPasswd[0] = 0;
        return(S_OK);
    }

    int ilen = wcslen(*Value);
    if(ilen > 0)
    {
        LPWSTR pstr = wcschr(*Value, '/'), pstr2;
        ilen = 64;
        if(pstr) ilen = pstr - *Value;
        if(ilen < 64)
        {
            wcsncpy(m_sUserName, *Value, ilen);
            m_sUserName[ilen] = 0;
            pstr++;
            pstr2 = wcschr(pstr, '@');
            ilen = 64;
            if(pstr2) ilen = pstr2 - pstr;
        }
        if(ilen < 64)
        {
            wcsncpy(m_sPasswd, pstr, ilen);
            m_sPasswd[ilen] = 0;
            pstr2++;
            ilen = wcslen(pstr2);
        }
        if(ilen < 64)
        {
            wcsncpy(m_sDBName, pstr2, ilen);
            m_sDBName[ilen] = 0;
        }
        else return(E_INVALIDARG);
    }
    else
    {
        m_sDBName[0] = 0;
        m_sUserName[0] = 0;
        m_sPasswd[0] = 0;
    }
    if(m_bVisible)
    {
        SendDlgItemMessage(m_hWndDlg, CTL_DBNAMEEDT, WM_SETTEXT, 0,
            (LPARAM)m_sDBName);
        SendDlgItemMessage(m_hWndDlg, CTL_USERNAMEEDT, WM_SETTEXT, 0,
            (LPARAM)m_sUserName);
        SendDlgItemMessage(m_hWndDlg, CTL_USERPASSWDEDT, WM_SETTEXT, 0,
            (LPARAM)m_sPasswd);
    }
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_AuxiliaryPath(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_AuxiliaryPath\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);
    if(m_sAuxiliaryPath) *Value = SysAllocString(m_sAuxiliaryPath);
    else *Value = SysAllocString(L"");
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::set_AuxiliaryPath(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::set_AuxiliaryPath\r\n", true);
#endif // DBGLEVEL
    if(m_sAuxiliaryPath) free(m_sAuxiliaryPath);
    m_sAuxiliaryPath = NULL;
    int ilen = wcslen(*Value);
    if(ilen > 0)
    {
        m_sAuxiliaryPath = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
        wcscpy(m_sAuxiliaryPath, *Value);
    }
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_ConnectionType(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_ConnectionType\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);
    *Value = SysAllocString(m_sConnectType);
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::set_ConnectionType(BSTR *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::set_ConnectionType\r\n", true);
#endif // DBGLEVEL
    int ilen = wcslen(*Value);
    if(ilen > 35) return(E_NOTIMPL);
    if(ilen > 0) wcscpy(m_sConnectType, *Value);
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_Enabled(VARIANT_BOOL *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_Enabled\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);
    *Value = m_bEnabled;
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::set_Enabled(VARIANT_BOOL *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::set_Enabled\r\n", true);
#endif // DBGLEVEL
    m_bEnabled = *Value;
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::get_hWnd(long *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::get_hWnd\r\n", true);
#endif // DBGLEVEL
    if(!Value) return(E_INVALIDARG);
    *Value = (long)m_hWndDlg;
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::ShowAboutBox()
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::ShowAboutBox\r\n", true);
#endif // DBGLEVEL
    DialogBox(m_hInstance, L"ABOUTBOX", m_hWndDlg, AboutBoxDialogProc);
    return(S_OK);
}

HRESULT _GMPostGISConnectionProps::ValidateUserData(VARIANT_BOOL *bIsValid)
{
#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::ValidateUserData\r\n", true);
#endif // DBGLEVEL
    SendDlgItemMessage(m_hWndDlg, CTL_SERVEREDT, WM_GETTEXT, 64,
        (LPARAM)m_sServer);
    SendDlgItemMessage(m_hWndDlg, CTL_PORTEDT, WM_GETTEXT, 16,
        (LPARAM)m_sPort);
    SendDlgItemMessage(m_hWndDlg, CTL_DBNAMEEDT, WM_GETTEXT, 64,
        (LPARAM)m_sDBName);
    SendDlgItemMessage(m_hWndDlg, CTL_USERNAMEEDT, WM_GETTEXT, 64,
        (LPARAM)m_sUserName);
    SendDlgItemMessage(m_hWndDlg, CTL_USERPASSWDEDT, WM_GETTEXT, 64,
        (LPARAM)m_sPasswd);
    *bIsValid = m_sServer[0] && m_sDBName[0] && m_sUserName[0] && m_sPasswd[0];
    return(S_OK);
}

void _GMPostGISConnectionProps::InitDialog(HWND hwndParent)
{
    if(m_hWndDlg) return;
    m_hWndDlg = CreateDialog(m_hInstance, _T("PSGCTLPAGE"), hwndParent,
        ConnPropsDialogProc);

    RECT R;
    GetClientRect(m_hWndDlg, &R);
    HDC dc = GetDC(0);
    int ppix = GetDeviceCaps(dc, LOGPIXELSX);
    int ppiy = GetDeviceCaps(dc, LOGPIXELSY);
    ReleaseDC(0, dc);

    const double mmperinch = 25.38071;
    m_iWidth = (int)(100.0*mmperinch*R.right)/ppix;
    m_iHeight = (int)(100.0*mmperinch*R.bottom)/ppiy;

    SendDlgItemMessage(m_hWndDlg, CTL_SERVEREDT, WM_SETTEXT, 0,
        (LPARAM)m_sServer);
    SendDlgItemMessage(m_hWndDlg, CTL_PORTEDT, WM_SETTEXT, 0,
        (LPARAM)m_sPort);
    SendDlgItemMessage(m_hWndDlg, CTL_DBNAMEEDT, WM_SETTEXT, 0,
        (LPARAM)m_sDBName);
    SendDlgItemMessage(m_hWndDlg, CTL_USERNAMEEDT, WM_SETTEXT, 0,
        (LPARAM)m_sUserName);
    SendDlgItemMessage(m_hWndDlg, CTL_USERPASSWDEDT, WM_SETTEXT, 0,
        (LPARAM)m_sPasswd);
    return;
}

void _GMPostGISConnectionProps::GetControlInfo(CONTROLINFO* pCI)
{
    pCI->cb = sizeof(CONTROLINFO);
    pCI->hAccel = 0;
    pCI->cAccel = 0;
    pCI->dwFlags = 0;

    IOleControlSite *pcs = m_pOleObject->GetControlSite();
    if(pcs)
    {
        pcs->OnControlInfoChanged();
        pcs->Release();
    }
    return;
}

void _GMPostGISConnectionProps::GetExtent(LONG *dx, LONG *dy)
{
    {
        *dx = m_iWidth;
        *dy = m_iHeight;
        return;
    }
}

HWND _GMPostGISConnectionProps::GetWindow()
{
    HWND res = m_hWndDlg;
    return(res);
}

void _GMPostGISConnectionProps::ShowDialog(HWND hwndParent, LPCRECT lprcPosRect)
{
    if(m_bVisible) return;

    IOleInPlaceSite *pOIPS = m_pOleObject->GetOleInPlaceSite();
    if(pOIPS)
    {
#if DBGLEVEL > 0
        WriteLogFile("  setting the InPlace Active Object\r\n", true);
#endif // DBGLEVEL
        IOleInPlaceFrame *pOIPF = NULL;
        IOleInPlaceUIWindow *pOIPW = NULL;
        OLEINPLACEFRAMEINFO oipf;
        oipf.cb = sizeof(OLEINPLACEFRAMEINFO);
        RECT R1, R2;
        pOIPS->GetWindowContext(&pOIPF, &pOIPW, &R1, &R2, &oipf);
        if(!pOIPW && pOIPF)
        {
            pOIPW = (IOleInPlaceUIWindow*)pOIPF;
            pOIPF = NULL;
        }
        if(pOIPF) pOIPF->Release();
        if(pOIPW)
        {
            pOIPW->SetActiveObject((IOleInPlaceActiveObject*)m_pOleInPlaceActiveObject, NULL);
            pOIPW->Release();
#if DBGLEVEL > 0
            WriteLogFile("  InPlace Active Object set\r\n", false);
#endif // DBGLEVEL
        }
        else
        {
            if(pOIPS->QueryInterface(IID_IOleInPlaceUIWindow, (void**)&pOIPW) == S_OK)
            {
                if(pOIPW)
                {
                    pOIPW->SetActiveObject((IOleInPlaceActiveObject*)m_pOleInPlaceActiveObject, NULL);
                    pOIPW->Release();
#if DBGLEVEL > 0
                    WriteLogFile("  InPlace Active Object set - 2nd attempt\r\n", false);
#endif // DBGLEVEL
                }
            }
        }
        pOIPS->Release();
    }

    InitDialog(hwndParent);
    SetWindowPos(m_hWndDlg, 0, lprcPosRect->left, lprcPosRect->top, 0, 0,
        SWP_NOSIZE);
    ShowWindow(m_hWndDlg, SW_SHOW);
    m_bVisible = true;
    return;
}

void _GMPostGISConnectionProps::HideDialog(HWND hwndParent)
{
    if(!m_bVisible) return;

    IOleInPlaceSite *pOIPS = m_pOleObject->GetOleInPlaceSite();
    if(pOIPS)
    {
#if DBGLEVEL > 0
        WriteLogFile("  unsetting the InPlace Active Object\r\n", true);
#endif // DBGLEVEL
        IOleInPlaceFrame *pOIPF = NULL;
        IOleInPlaceUIWindow *pOIPW = NULL;
        OLEINPLACEFRAMEINFO oipf;
        oipf.cb = sizeof(OLEINPLACEFRAMEINFO);
        RECT R1, R2;
        pOIPS->GetWindowContext(&pOIPF, &pOIPW, &R1, &R2, &oipf);
        if(!pOIPW && pOIPF)
        {
            pOIPW = (IOleInPlaceUIWindow*)pOIPF;
            pOIPF = NULL;
        }
        if(pOIPF) pOIPF->Release();
        if(pOIPW)
        {
            pOIPW->SetActiveObject(NULL, NULL);
            pOIPW->Release();
#if DBGLEVEL > 0
            WriteLogFile("  InPlace Active Object unset\r\n", false);
#endif // DBGLEVEL
        }
        else
        {
            if(pOIPS->QueryInterface(IID_IOleInPlaceUIWindow, (void**)&pOIPW) == S_OK)
            {
                if(pOIPW)
                {
                    pOIPW->SetActiveObject(NULL, NULL);
                    pOIPW->Release();
#if DBGLEVEL > 0
                    WriteLogFile("  InPlace Active Object unset - 2nd attempt\r\n", false);
#endif // DBGLEVEL
                }
            }
        }
        pOIPS->Release();
    }

    ShowWindow(m_hWndDlg, SW_HIDE);
    SetParent(m_hWndDlg, 0);
    DestroyWindow(m_hWndDlg);
    m_hWndDlg = NULL;
    m_bVisible = false;
    return;
}

void _GMPostGISConnectionProps::SetPos(LONG x, LONG y)
{
    SetWindowPos(m_hWndDlg, 0, x, y, 0, 0, SWP_NOSIZE);
    return;
}

HRESULT _GMPostGISConnectionProps::DoVerbInPlaceActivate(
    LPMSG lpmsg, IOleClientSite *pActiveSite,
    LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
{
    BEGINHR
    IOleInPlaceSite *pOleInPlaceSite = NULL;

#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::DoVerbInPlaceActivate\r\n", true);
#endif // DBGLEVEL

    pOleInPlaceSite = m_pOleObject->GetOleInPlaceSite ();
    TESTHR(pOleInPlaceSite->CanInPlaceActivate ());
    if (hr == S_FALSE)
        TESTHR(E_FAIL);
    TESTHR(pOleInPlaceSite->OnInPlaceActivate());
    m_bInPlaceActive = TRUE;

    ShowDialog(hwndParent, lprcPosRect);
    if(pActiveSite) pActiveSite->OnShowWindow(TRUE);

    ENDHR_PRE
    if (pOleInPlaceSite != NULL)
        pOleInPlaceSite->Release ();
    ENDHR_POST
}

HRESULT _GMPostGISConnectionProps::InPlaceDeactivate ()
{
    BEGINHR
    IOleInPlaceSite *pOleInPlaceSite = NULL;

#if DBGLEVEL > 0
    WriteLogFile("_GMPostGISConnectionProps::InPlaceDeactivate\r\n", true);
#endif // DBGLEVEL

    if (m_bInPlaceActive == TRUE)
    {
        TESTHR(m_pOleInPlaceObject->UIDeactivate ());
        m_bInPlaceActive = FALSE;

        pOleInPlaceSite = m_pOleObject->GetOleInPlaceSite ();
        TESTHR(pOleInPlaceSite->OnInPlaceDeactivate());
    }

    ENDHR
}

void _GMPostGISConnectionProps::TransAccel(LPMSG lpmsg)
{
    // This is the best hack for handling tabs etc. I was able to make

    HWND hwndPar = GetParent(lpmsg->hwnd);
    IOleControlSite *pCS = m_pOleObject->GetControlSite();
    BOOL bPrev = GetKeyState(VK_SHIFT) & 0x8000;

    if((lpmsg->message == WM_KEYDOWN) && (lpmsg->wParam == VK_TAB))
    {
        HWND wndNext;
        if(hwndPar != m_hWndDlg)
        {
            if(pCS)
            {
                if(bPrev) pCS->TranslateAccelerator(lpmsg, 1);
                else pCS->TranslateAccelerator(lpmsg, 0);
            }

            wndNext = GetFocus();
            if(!IsWindowVisible(wndNext) || (GetParent(wndNext) == m_hWndDlg))
            {
                if(bPrev) SetFocus(GetDlgItem(m_hWndDlg, CTL_USERPASSWDEDT));
                else SetFocus(GetDlgItem(m_hWndDlg, CTL_SERVEREDT));
            }
        }
        else
        {
            hwndPar = GetParent(m_hWndDlg);
            if(bPrev && (GetDlgCtrlID(lpmsg->hwnd) == CTL_SERVEREDT))
            {
                // Universal hack for "normal" containers
                /*wndNext = GetNextDlgTabItem(hwndPar, lpmsg->hwnd, TRUE);
                if(!wndNext)
                    wndNext = GetNextDlgTabItem(hwndPar, m_hWndDlg, TRUE);
                if(wndNext) SetFocus(wndNext);*/

                // Special hack for GeoMedia
                if(pCS)
                {
                    if(bPrev) pCS->TranslateAccelerator(lpmsg, 1);
                    else pCS->TranslateAccelerator(lpmsg, 0);
                }
            }
            else if(!bPrev && (GetDlgCtrlID(lpmsg->hwnd) == CTL_USERPASSWDEDT))
            {
                // Universal hack for "normal" containers
                /*wndNext = GetNextDlgTabItem(hwndPar, lpmsg->hwnd, FALSE);
                if(!wndNext)
                    wndNext = GetNextDlgTabItem(hwndPar, m_hWndDlg, FALSE);
                if(wndNext) SetFocus(wndNext);*/

                // Special hack for GeoMedia
                if(pCS)
                {
                    if(bPrev) pCS->TranslateAccelerator(lpmsg, 1);
                    else pCS->TranslateAccelerator(lpmsg, 0);
                }
            }
            else if(!IsDialogMessage(m_hWndDlg, lpmsg)) TranslateMessage(lpmsg);
        }
    }
    else
    {
        // It looks like if the following check is disabled, all accelerators are
        // handled correctly

        //if((lpmsg->message == WM_KEYDOWN) && ((lpmsg->wParam == VK_ESCAPE) ||
        //    (lpmsg->wParam == VK_RETURN)))
        {
            if(pCS)
            {
                if(bPrev) pCS->TranslateAccelerator(lpmsg, 1);
                else pCS->TranslateAccelerator(lpmsg, 0);
            }
        }

        HWND wndTop = GetParent(hwndPar);
        if(!wndTop) wndTop = hwndPar;
        if(IsWindowUnicode(wndTop))
        {
            if(!IsDialogMessageW(hwndPar, lpmsg)) TranslateMessage(lpmsg);
        }
        else
        {
            if(!IsDialogMessageA(hwndPar, lpmsg)) TranslateMessage(lpmsg);
        }
    }
    if(pCS) pCS->Release();
    return;
}


/*void _GMPostGISConnectionProps::GetConnectionPoint(IConnectionPoint **ppCP)
{
    QueryInterface(IID_IConnectionPoint, (void**)ppCP);
    return;
}*/


//  __GMPostGISNativeQueryProps

/*__GMPostGISNativeQueryProps::__GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib) : CCOMDispatch(selfptr, ALib, 4)
{
#if DBGLEVEL > 1
    WriteLogFile("__GMPostGISNativeQueryProps::__GMPostGISNativeQueryProps-1\r\n", true);
#endif // DBGLEVEL
}

__GMPostGISNativeQueryProps::__GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib, int iIndex) : CCOMDispatch(selfptr, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("__GMPostGISNativeQueryProps::__GMPostGISNativeQueryProps-2\r\n", true);
#endif // DBGLEVEL
}

__GMPostGISNativeQueryProps::~__GMPostGISNativeQueryProps()
{
#if DBGLEVEL > 1
    WriteLogFile("__GMPostGISNativeQueryProps::~__GMPostGISNativeQueryProps\r\n", true);
#endif // DBGLEVEL
}

HRESULT __GMPostGISNativeQueryProps::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("__GMPostGISNativeQueryProps::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID___GMPostGISNativeQueryProps))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else hres = E_NOINTERFACE;
    }
    return(hres);
}

ULONG __GMPostGISNativeQueryProps::Release()
{
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "__GMPostGISNativeQueryProps::Release - %d\r\n", m_lRefCount);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
    if(lRest > 0) return(lRest);
    delete(this);
    return(0);
}

HRESULT __GMPostGISNativeQueryProps::Change()
{
#if DBGLEVEL > 0
    WriteLogFile("__GMPostGISNativeQueryProps::Change\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}*/
