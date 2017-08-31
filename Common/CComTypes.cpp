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

#include "CComTypes.hpp"
#include "LogFiles.hpp"
#include <stdio.h>
#include <ocidl.h>

void FormatGuid(LPSTR ABuf, LPCSTR sPref, LPCSTR sPost, GUID iid)
{
    if(IsEqualIID(iid, IID_IUnknown))
        sprintf(ABuf, "%sIID_IUnknown%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IClassFactory))
        sprintf(ABuf, "%sIID_IClassFactory%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IMarshal))
        sprintf(ABuf, "%sIID_IMarshal%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IPersistStream))
        sprintf(ABuf, "%sIID_IPersistStream%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IPersistStreamInit))
        sprintf(ABuf, "%sIID_IPersistStreamInit%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IPersistStorage))
        sprintf(ABuf, "%sIID_IPersistStorage%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IRunnableObject))
        sprintf(ABuf, "%sIID_IRunnableObject%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IViewObject))
        sprintf(ABuf, "%sIID_IViewObject%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IViewObject2))
        sprintf(ABuf, "%sIID_IViewObject2%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IViewObjectEx))
        sprintf(ABuf, "%sIID_IViewObjectEx%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IDispatch))
        sprintf(ABuf, "%sIID_IDispatch%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleControl))
        sprintf(ABuf, "%sIID_IOleControl%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleLink))
        sprintf(ABuf, "%sIID_IOleLink%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleObject))
        sprintf(ABuf, "%sIID_IOleObject%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IDataObject))
        sprintf(ABuf, "%sIID_IDataObject%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleWindow))
        sprintf(ABuf, "%sIID_IOleWindow%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleCache))
        sprintf(ABuf, "%sIID_IOleCache%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleInPlaceObject))
        sprintf(ABuf, "%sIID_IOleInPlaceObject%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IQuickActivate))
        sprintf(ABuf, "%sIID_IQuickActivate%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IProvideClassInfo))
        sprintf(ABuf, "%sIID_IProvideClassInfo%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IConnectionPointContainer))
        sprintf(ABuf, "%sIID_IConnectionPointContainer%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IConnectionPoint))
        sprintf(ABuf, "%sIID_IConnectionPoint%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IPointerInactive))
        sprintf(ABuf, "%sIID_IPointerInactive%s", sPref, sPost);
    else if(IsEqualIID(iid, IID_IOleInPlaceActiveObject))
        sprintf(ABuf, "%sIID_IOleInPlaceActiveObject%s", sPref, sPost);
    else
        sprintf(ABuf, "%s{%.8lX-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}%s",
            sPref, iid.Data1, iid.Data2, iid.Data3, iid.Data4[0], iid.Data4[1],
            iid.Data4[2], iid.Data4[3], iid.Data4[4], iid.Data4[5],
            iid.Data4[6], iid.Data4[7], sPost);
    return;
}

void ValidateVariant(VARIANT *pV)
{
    if(pV->vt & VT_BYREF)
    {
        if(!(pV->pbstrVal)) pV->vt = VT_NULL;
    }
    else if(pV->vt == VT_BSTR)
    {
        if(!(pV->bstrVal)) pV->vt = VT_NULL;
    }
    else if(pV->vt == VT_UNKNOWN)
    {
        if(!(pV->punkVal)) pV->vt = VT_NULL;
    }
    else if(pV->vt == VT_DISPATCH)
    {
        if(!(pV->pdispVal)) pV->vt = VT_NULL;
    }
    return;
}

// CCOMUnknown

CCOMUnknown::CCOMUnknown(IUnknown *pUnkOuter)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMUnknown::CCOMUnknown\r\n", true);
#endif // DBGLEVEL
    m_lRefCount = 0;
    m_pUnkOuter = pUnkOuter;
}

CCOMUnknown::~CCOMUnknown()
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMUnknown::~CCOMUnknown\r\n", true);
#endif // DBGLEVEL
}

HRESULT CCOMUnknown::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMUnknown::QueryInterface\r\n", true);
    char buf[64];
    FormatGuid(buf, "  iid: ", "\r\n", iid);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualIID(iid, IID_IUnknown)) *ppvObject = this;

    if(!*ppvObject) return(E_NOINTERFACE);

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CCOMUnknown::AddRef()
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMUnknown::AddRef\r\n", true);
#endif // DBGLEVEL
    if(m_pUnkOuter) return m_pUnkOuter->AddRef();
    return(++m_lRefCount);
}

ULONG CCOMUnknown::Release()
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMUnknown::Release\r\n", true);
#endif // DBGLEVEL
    if(m_pUnkOuter) return m_pUnkOuter->Release();
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete(this);
    return(0);
}


// CCOMDispatch

CCOMDispatch::CCOMDispatch(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::CCOMDispatch\r\n", true);
#endif // DBGLEVEL
    m_lRefCount = 0;
    m_pTypeInfo = NULL;
    m_pUnkOuter = pUnkOuter;
    m_pTypeLib = ALib;
    if(m_pTypeLib)
    {
        m_pTypeLib->AddRef();
        m_pTypeLib->GetTypeInfo(iIndex, &m_pTypeInfo);
    }
}

CCOMDispatch::~CCOMDispatch()
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::~CCOMDispatch\r\n", true);
#endif // DBGLEVEL
    if(m_pTypeLib) m_pTypeLib->Release();
    if(m_pTypeInfo) m_pTypeInfo->Release();
}

HRESULT CCOMDispatch::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::QueryInterface\r\n", true);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDispatch)) *ppvObject = this;

    if(!*ppvObject) return(E_NOINTERFACE);

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CCOMDispatch::AddRef()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->AddRef();
    else lRest = ++m_lRefCount;
#if DBGLEVEL > 2
    BSTR sName = NULL;
    char buf[32];
    if(m_pTypeInfo)
    {
        m_pTypeInfo->GetDocumentation(-1, &sName, NULL, NULL, NULL);
        WriteLogFile(sName, true);
        SysFreeString(sName);
    }
    else WriteLogFile("CCOMDispatch", true);
    sprintf(buf, "::AddRef - %d\r\n", lRest);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    return(lRest);
}

ULONG CCOMDispatch::Release()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    BSTR sName = NULL;
    char buf[32];
    if(m_pTypeInfo)
    {
        m_pTypeInfo->GetDocumentation(-1, &sName, NULL, NULL, NULL);
        WriteLogFile(sName, true);
        SysFreeString(sName);
    }
    else WriteLogFile("CCOMDispatch", true);
    sprintf(buf, "::Release - %d\r\n", lRest);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(lRest > 0) return(lRest);
    delete(this);
    return(0);
}

HRESULT CCOMDispatch::GetIDsOfNames(REFIID riid, OLECHAR **rgszNames,
    unsigned int cNames, LCID lcid, DISPID *rgDispId)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::GetIDsOfNames\r\n", true);
#endif // DBGLEVEL
    return(DispGetIDsOfNames(m_pTypeInfo, rgszNames, cNames, rgDispId));
}

HRESULT CCOMDispatch::GetTypeInfo(unsigned int iTInfo, LCID lcid,
    ITypeInfo **ppTInfo)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::GetTypeInfo\r\n", true);
#endif // DBGLEVEL
    if(m_pTypeInfo) m_pTypeInfo->AddRef();
    *ppTInfo = m_pTypeInfo;
    return(S_OK);
}

HRESULT CCOMDispatch::GetTypeInfoCount(unsigned int *pctinfo)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::GetTypeInfoCount\r\n", true);
#endif // DBGLEVEL
    (m_pTypeInfo != NULL) ? *pctinfo = 1 : *pctinfo = 0;
    return(S_OK);
}

HRESULT CCOMDispatch::Invoke(DISPID dispIdMember, REFIID riid,
    LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
    EXCEPINFO *pExcepInfo, unsigned int *puArgErr)
{
#if DBGLEVEL > 2
    WriteLogFile("CCOMDispatch::Invoke\r\n", true);
#endif // DBGLEVEL
    return(DispInvoke(this, m_pTypeInfo, dispIdMember, wFlags, pDispParams,
        pVarResult, pExcepInfo, puArgErr));
}

void CCOMDispatch::SetOuter(IUnknown *pUnkOuter)
{
    if(!m_pUnkOuter && pUnkOuter)
    {
        while(m_lRefCount > 0)
        {
            pUnkOuter->AddRef();
            m_lRefCount--;
        }
    }
    m_pUnkOuter = pUnkOuter;
    if(!m_pUnkOuter)
    {
        m_lRefCount = 1;
        Release();
    }
    return;
}
