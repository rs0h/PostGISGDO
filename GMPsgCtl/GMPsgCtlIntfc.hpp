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

#ifndef _GMPsgCtlIntfc_HPP_
#define _GMPsgCtlIntfc_HPP_

#include <basetyps.h>
#include <wtypes.h>
#include <ocidl.h>
#include "../Common/CComTypes.hpp"

const CLSID CLASS_GMPostGISConnectionProps = {0x4A7804EE,0x49D8,0x4354,{0x80,0x90,0x48,0x89,0xEA,0xDB,0x86,0xDF}};
//const CLSID CLASS_GMPostGISNativeQueryProps = {0x4A7804EE,0x49D8,0x4354,{0x80,0x90,0x48,0x89,0xEA,0xDB,0x86,0xDE}};

const IID DIID__GMPostGISConnectionProps = {0x64FF2B65,0x9C02,0x11DF,{0x95,0x7E,0x00,0x50,0x56,0x10,0xF7,0xA2}};
const IID DIID___GMPostGISConnectionProps = {0x64FF2B66,0x9C02,0x11DF,{0x95,0x7E,0x00,0x50,0x56,0x10,0xF7,0xA2}};
//const IID DIID__GMPostGISNativeQueryProps = {0x64FF2B67,0x9C02,0x11DF,{0x95,0x7E,0x00,0x50,0x56,0x10,0xF7,0xA2}};
//const IID DIID___GMPostGISNativeQueryProps = {0x64FF2B68,0x9C02,0x11DF,{0x95,0x7E,0x00,0x50,0x56,0x10,0xF7,0xA2}};

class _GMPostGISConnectionProps;
class __GMPostGISConnectionProps : public IDispatch
{
};
//class _GMPostGISNativeQueryProps;
//class __GMPostGISNativeQueryProps;

/*class _GMPostGISNativeQueryProps : public CCOMDispatch
{
public:
    _GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib);
    _GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib, int iIndex);
    ~_GMPostGISNativeQueryProps();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall AddToCallbackList(_IChangePropertiesCallback* Cmd);
    virtual HRESULT __stdcall RemoveFromCallbackList(_IChangePropertiesCallback* Cmd);
    virtual HRESULT __stdcall set_OutputRecordset(GRecordset* *Value);
    virtual HRESULT __stdcall get_OutputRecordset(GRecordset* *Value);
    virtual HRESULT __stdcall set_InputConnection(DGMConnection* *Value);
    virtual HRESULT __stdcall get_InputConnection(DGMConnection* *Value);
    virtual HRESULT __stdcall get_CoordSystemsMgr(DGMCoordSystemsMgr* *Value);
    virtual HRESULT __stdcall set_CoordSystemsMgr(DGMCoordSystemsMgr* *Value);
    virtual HRESULT __stdcall get_TemporaryRecordset(GRecordset* *Value);
    virtual HRESULT __stdcall get_hWnd(long *Value);
    virtual HRESULT __stdcall StoreDefaults(BSTR *prefix, IDispatch * *PrefSet);
    virtual HRESULT __stdcall RestoreDefaults(BSTR *prefix, IDispatch * *PrefSet);
    virtual HRESULT __stdcall ShowAboutBox();
};*/

class COleBase : public IUnknown
{
protected:
    _GMPostGISConnectionProps *m_pParent;
public:
    COleBase(_GMPostGISConnectionProps *pParent);
    ~COleBase();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
};

class COleObject : public COleBase
{
private:
    IOleClientSite *m_pOleClientSite;
    bool m_bInPlaceActive;
    IAdviseSink *m_pAdviseSink;
    DWORD m_dwAdvToken;
public:
    COleObject(_GMPostGISConnectionProps *pParent);
    ~COleObject();
    virtual HRESULT __stdcall SetClientSite(IOleClientSite *pClientSite);
    virtual HRESULT __stdcall GetClientSite(IOleClientSite **ppClientSite);
    virtual HRESULT __stdcall SetHostNames(LPCOLESTR szContainerApp,
        LPCOLESTR szContainerObj);
    virtual HRESULT __stdcall Close(DWORD dwSaveOption);
    virtual HRESULT __stdcall SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk);
    virtual HRESULT __stdcall GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker,
        IMoniker **ppmk);
    virtual HRESULT __stdcall InitFromData(IDataObject *pDataObject,
        BOOL fCreation, DWORD dwReserved);
    virtual HRESULT __stdcall GetClipboardData(DWORD dwReserved,
        IDataObject **ppDataObject);
    virtual HRESULT __stdcall DoVerb(LONG iVerb, LPMSG lpmsg,
        IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent,
        LPCRECT lprcPosRect);
//#ifdef GCC
//    virtual HRESULT __stdcall EnumVerbs(IEnumOleVerb **ppEnumOleVerb);
//#else
    virtual HRESULT __stdcall EnumVerbs(IEnumOLEVERB **ppEnumOleVerb);
//#endif
    virtual HRESULT __stdcall Update();
    virtual HRESULT __stdcall IsUpToDate();
    virtual HRESULT __stdcall GetUserClassID(CLSID *pClsid);
    virtual HRESULT __stdcall GetUserType(DWORD dwFormOfType,
        LPOLESTR *pszUserType);
    virtual HRESULT __stdcall SetExtent(DWORD dwDrawAspect, SIZEL *psizel);
    virtual HRESULT __stdcall GetExtent(DWORD dwDrawAspect, SIZEL *psizel);
    virtual HRESULT __stdcall Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection);
    virtual HRESULT __stdcall Unadvise(DWORD dwConnection);
    virtual HRESULT __stdcall EnumAdvise(IEnumSTATDATA **ppenumAdvise);
    virtual HRESULT __stdcall GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus);
    virtual HRESULT __stdcall SetColorScheme(LOGPALETTE *pLogpal);
    IOleControlSite* GetControlSite();
    IOleInPlaceSite* GetOleInPlaceSite();
};

class COleControl : public COleBase
{
public:
    COleControl(_GMPostGISConnectionProps *pParent);
    ~COleControl();
    virtual HRESULT __stdcall GetControlInfo(CONTROLINFO* pCI);
    virtual HRESULT __stdcall OnMnemonic(LPMSG pMsg);
    virtual HRESULT __stdcall OnAmbientPropertyChange(DISPID dispID);
    virtual HRESULT __stdcall FreezeEvents(BOOL bFreeze);
};

class COleInPlaceObject : public COleBase
{
public:
    COleInPlaceObject(_GMPostGISConnectionProps *pParent);
    ~COleInPlaceObject();
    virtual HRESULT __stdcall GetWindow(HWND * phwnd);
    virtual HRESULT __stdcall ContextSensitiveHelp(BOOL fEnterMode);
    virtual HRESULT __stdcall InPlaceDeactivate();
    virtual HRESULT __stdcall UIDeactivate();
    virtual HRESULT __stdcall SetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect);
    virtual HRESULT __stdcall ReactivateAndUndo();
};

class COleInPlaceActiveObject : public COleBase
{
public:
    COleInPlaceActiveObject(_GMPostGISConnectionProps *pParent);
    ~COleInPlaceActiveObject();
    virtual HRESULT __stdcall GetWindow(HWND * phwnd);
    virtual HRESULT __stdcall ContextSensitiveHelp(BOOL fEnterMode);
    virtual HRESULT __stdcall TranslateAccelerator(LPMSG lpmsg);
    virtual HRESULT __stdcall OnFrameWindowActivate(BOOL fActivate);
    virtual HRESULT __stdcall OnDocWindowActivate(BOOL fActivate);
    virtual HRESULT __stdcall ResizeBorder(LPCRECT prcBorder,
        IOleInPlaceUIWindow *pUIWindow, BOOL fFrameWindow);
    virtual HRESULT __stdcall EnableModeless(BOOL fEnable);
};

class CViewObject2 : public COleBase
{
private:
    IAdviseSink *m_pAdviseSink;
    DWORD m_dwAspect;
    DWORD m_dwadvf;
    DWORD m_dwFreeze;
public:
    CViewObject2(_GMPostGISConnectionProps *pParent);
    ~CViewObject2();
#ifdef GCC
    virtual HRESULT __stdcall Draw(DWORD dwAspect, LONG lindex, void * pvAspect,
        DVTARGETDEVICE * ptd, HDC hicTargetDev, HDC hdcDraw,
        const LPRECTL lprcBounds, const LPRECTL lprcWBounds,
        __IView_pfncont pfnContinue, DWORD dwContinue);
#else
    virtual HRESULT __stdcall Draw(DWORD dwAspect, LONG lindex, void * pvAspect,
        DVTARGETDEVICE * ptd, HDC hicTargetDev, HDC hdcDraw,
        const LPRECTL lprcBounds, const LPRECTL lprcWBounds,
        BOOL (STDMETHODCALLTYPE *pfnContinue)(ULONG_PTR dwContinue),
        DWORD dwContinue);
#endif
    virtual HRESULT __stdcall GetColorSet(DWORD dwAspect, LONG lindex,
        void * pvAspect, DVTARGETDEVICE * ptd, HDC hicTargetDev,
        LOGPALETTE ** ppColorSet);
    virtual HRESULT __stdcall Freeze(DWORD dwAspect, LONG lindex,
        void * pvAspect, DWORD * pdwFreeze);
    virtual HRESULT __stdcall Unfreeze(DWORD dwFreeze);
    virtual HRESULT __stdcall SetAdvise(DWORD dwAspect, DWORD advf,
        IAdviseSink * pAdvSink);
    virtual HRESULT __stdcall GetAdvise(DWORD * pdwAspect, DWORD * padvf,
        IAdviseSink ** ppAdvSink);
    // --- IViewObject2
    virtual HRESULT __stdcall GetExtent(DWORD dwAspect, DWORD lindex,
        DVTARGETDEVICE *ptd, LPSIZEL lpsizel);
    // ---
};

class CPersist : public COleBase
{
public:
    CPersist(_GMPostGISConnectionProps *pParent);
    ~CPersist();
    virtual HRESULT __stdcall GetClassID(CLSID *pClassID);
};

class CPersistStreamInit : public CPersist
{
private:
    bool m_bLoaded;
public:
    CPersistStreamInit(_GMPostGISConnectionProps *pParent);
    ~CPersistStreamInit();
    virtual HRESULT __stdcall IsDirty();
    virtual HRESULT __stdcall Load(LPSTREAM pStm);
    virtual HRESULT __stdcall Save(LPSTREAM pStm, BOOL fClearDirty);
    virtual HRESULT __stdcall GetSizeMax(ULARGE_INTEGER* pcbSize);
    virtual HRESULT __stdcall InitNew();
};

/*class CRunnableObject : public COleBase
{
public:
    CRunnableObject(_GMPostGISConnectionProps *pParent);
    ~CRunnableObject();
    virtual HRESULT __stdcall GetRunningClass(LPCLSID lpClsid);
    virtual HRESULT __stdcall Run(LPBC lpbc);
    virtual BOOL __stdcall IsRunning();
    virtual HRESULT __stdcall LockRunning(BOOL fLock, BOOL fLastUnlockCloses);
    virtual HRESULT __stdcall SetContainedObject(BOOL fContained);
};

class COleCache : public COleBase
{
public:
    COleCache(_GMPostGISConnectionProps *pParent);
    ~COleCache();
    virtual HRESULT __stdcall Cache(FORMATETC * pFormatetc, DWORD advf,
        DWORD * pdwConnection);
    virtual HRESULT __stdcall Uncache(DWORD dwConnection);
    virtual HRESULT __stdcall EnumCache(IEnumSTATDATA ** ppenumSTATDATA);
    virtual HRESULT __stdcall InitCache(IDataObject * pDataObject);
    virtual HRESULT __stdcall SetData(FORMATETC * pFormatetc,
        STGMEDIUM * pmedium, BOOL fRelease);
};

class CConnectionPointContainer : public COleBase
{
public:
    CConnectionPointContainer(_GMPostGISConnectionProps *pParent);
    ~CConnectionPointContainer();
    virtual HRESULT __stdcall EnumConnectionPoints(IEnumConnectionPoints **ppEnum);
    virtual HRESULT __stdcall FindConnectionPoint(REFIID riid,
        IConnectionPoint **ppCP);
};

class CConnectionPoint : public COleBase
{
public:
    CConnectionPoint(_GMPostGISConnectionProps *pParent);
    ~CConnectionPoint();
    virtual HRESULT __stdcall GetConnectionInterface(IID *pIID);
    virtual HRESULT __stdcall GetConnectionPointContainer(IConnectionPointContainer **ppCPC);
    virtual HRESULT __stdcall Advise(IUnknown *pUnk, DWORD *pdwCookie);
    virtual HRESULT __stdcall Unadvise(DWORD dwCookie);
    virtual HRESULT __stdcall EnumConnections(IEnumConnections **ppEnum);
};*/

class _GMPostGISConnectionProps : public CCOMDispatch
{
private:
    HINSTANCE m_hInstance;
    HWND m_hWndDlg;
    bool m_bVisible;
    int m_iWidth, m_iHeight;
    COleObject *m_pOleObject;
    COleControl *m_pOleControl;
    COleInPlaceObject *m_pOleInPlaceObject;
    COleInPlaceActiveObject *m_pOleInPlaceActiveObject;
    CViewObject2 *m_pViewObject;
    CPersistStreamInit *m_pPersistStreamInit;
    /*CRunnableObject *m_pRunnableObject;
    COleCache *m_pOleCache;
    CConnectionPointContainer *m_pConnectionPointContainer;*/

    LPWSTR m_sAuxiliaryPath;
    wchar_t m_sConnectType[36];
    wchar_t m_sServer[64];
    wchar_t m_sPort[16];
    wchar_t m_sDBName[64];
    wchar_t m_sUserName[64];
    wchar_t m_sPasswd[64];
    VARIANT_BOOL m_bInPlaceActive;
    VARIANT_BOOL m_bEnabled;
public:
    _GMPostGISConnectionProps(HINSTANCE hInstance, IUnknown *pUnkOuter,
        ITypeLib *ALib);
    _GMPostGISConnectionProps(HINSTANCE hInstance, IUnknown *pUnkOuter,
        ITypeLib *ALib, int iIndex);
    ~_GMPostGISConnectionProps();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid,
        LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo, unsigned int *puArgErr);
    virtual HRESULT __stdcall StoreDefaults(BSTR *prefix, IDispatch * *PrefSet);
    virtual HRESULT __stdcall RestoreDefaults(BSTR *prefix, IDispatch * *PrefSet);
    virtual HRESULT __stdcall get_Location(BSTR *Value);
    virtual HRESULT __stdcall set_Location(BSTR *Value);
    virtual HRESULT __stdcall get_ConnectInfo(BSTR *Value);
    virtual HRESULT __stdcall set_ConnectInfo(BSTR *Value);
    virtual HRESULT __stdcall get_AuxiliaryPath(BSTR *Value);
    virtual HRESULT __stdcall set_AuxiliaryPath(BSTR *Value);
    virtual HRESULT __stdcall get_ConnectionType(BSTR *Value);
    virtual HRESULT __stdcall set_ConnectionType(BSTR *Value);
    virtual HRESULT __stdcall get_Enabled(VARIANT_BOOL *Value);
    virtual HRESULT __stdcall set_Enabled(VARIANT_BOOL *Value);
    virtual HRESULT __stdcall get_hWnd(long *Value);
    virtual HRESULT __stdcall ShowAboutBox();
    virtual HRESULT __stdcall ValidateUserData(VARIANT_BOOL *bIsValid);
    void InitDialog(HWND hwndParent);
    void GetControlInfo(CONTROLINFO* pCI);
    void GetExtent(LONG *dx, LONG *dy);
    HWND GetWindow();
    void ShowDialog(HWND hwndParent, LPCRECT lprcPosRect);
    void HideDialog(HWND hwndParent);
    void SetPos(LONG x, LONG y);
    void TransAccel(LPMSG lpmsg);
    virtual HRESULT __stdcall DoVerbInPlaceActivate(LPMSG lpmsg,
        IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent,
        LPCRECT lprcPosRect);
    virtual HRESULT __stdcall InPlaceDeactivate ();
};

/*class __GMPostGISNativeQueryProps : public CCOMDispatch
{
public:
    __GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib);
    __GMPostGISNativeQueryProps(void **selfptr, ITypeLib *ALib, int iIndex);
    ~__GMPostGISNativeQueryProps();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    HRESULT Change();
};*/

#endif
