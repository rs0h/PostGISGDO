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

#include "GMPsgCtlClsFactory.hpp"
#ifdef DBGLEVEL
#include "../Common/LogFiles.hpp"
#endif // DBGLEVEL

extern wchar_t g_ModuleName[MAX_PATH];

// CGMPsgCtlClassFactory

CGMPsgCtlClassFactory::CGMPsgCtlClassFactory(HINSTANCE hInstance, CGMPsgCtlClassFactory **selfptr,
    CLSID rclsid)
{
#if DBGLEVEL > 1
    WriteLogFile("CGMPsgCtlClassFactory::CGMPsgCtlClassFactory\r\n", true);
#endif // DBGLEVEL
    m_hInstance = hInstance;
    m_lRefCount = 0;
    m_SelfPtr = selfptr;
    if(m_SelfPtr) *m_SelfPtr = this;
    m_Class = rclsid;
    m_pTypeLib = NULL;
    m_iListLen = 0;
    m_iListSize = 16;
    m_ClsObjList = (_GMPostGISConnectionProps**)
        malloc(m_iListSize*sizeof(_GMPostGISConnectionProps*));
}

CGMPsgCtlClassFactory::~CGMPsgCtlClassFactory()
{
#if DBGLEVEL > 1
    WriteLogFile("CGMPsgCtlClassFactory::~CGMPsgCtlClassFactory\r\n", true);
#endif // DBGLEVEL
    free(m_ClsObjList);
    if(m_SelfPtr) *m_SelfPtr = NULL;
    if(m_pTypeLib) m_pTypeLib->Release();
}

HRESULT CGMPsgCtlClassFactory::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CGMPsgCtlClassFactory::QueryInterface\r\n", true);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IClassFactory))
    {
        *ppvObject = this;
        return(S_OK);
    }
    else return(E_NOINTERFACE);
}

ULONG CGMPsgCtlClassFactory::AddRef()
{
#if DBGLEVEL > 2
    WriteLogFile("CGMPsgCtlClassFactory::AddRef\r\n", true);
#endif // DBGLEVEL
    return(++m_lRefCount);
}

ULONG CGMPsgCtlClassFactory::Release()
{
#if DBGLEVEL > 2
    WriteLogFile("CGMPsgCtlClassFactory::Release\r\n", true);
#endif // DBGLEVEL
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete this;
    return(0);
}

HRESULT CGMPsgCtlClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
#if DBGLEVEL > 0
    WriteLogFile("CGMPsgCtlClassFactory::CreateInstance\r\n", true);
    char buf[64];
    FormatGuid(buf, "  riid: ", "\r\n", riid);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    *ppvObject = NULL;
    bool bExtraIntfc = IsEqualIID(riid, IID_IOleObject) ||
        IsEqualIID(riid, IID_IOleControl) || IsEqualIID(riid, IID_IRunnableObject) ||
        IsEqualIID(riid, IID_IOleWindow) || IsEqualIID(riid, IID_IOleInPlaceObject) ||
        IsEqualIID(riid, IID_IViewObject) || IsEqualIID(riid, IID_IViewObject2) ||
        IsEqualIID(riid, IID_IPersist) || IsEqualIID(riid, IID_IPersistStreamInit);
    bool bKnownIID = IsEqualCLSID(m_Class, CLASS_GMPostGISConnectionProps) &&
        (IsEqualIID(riid, DIID__GMPostGISConnectionProps) ||
        IsEqualIID(riid, DIID___GMPostGISConnectionProps) ||
        IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch) ||
        bExtraIntfc);
    if(bKnownIID)
    {
        if(!m_pTypeLib)
        {
            wchar_t buf[MAX_PATH];
            wcscpy(buf, g_ModuleName);
            wcscat(buf, L"\\1");
            LoadTypeLib(buf, &m_pTypeLib);
        }
        if(m_iListLen >= m_iListSize)
        {
            m_iListSize += 4;
            m_ClsObjList = (_GMPostGISConnectionProps**)realloc(m_ClsObjList,
                m_iListSize*sizeof(_GMPostGISConnectionProps*));
        }
        m_ClsObjList[m_iListLen] = new _GMPostGISConnectionProps(m_hInstance,
            pUnkOuter, m_pTypeLib);
        m_ClsObjList[m_iListLen++]->QueryInterface(riid, ppvObject);
    }
    if(*ppvObject)
    {
        //((IUnknown*)*ppvObject)->AddRef();
        return(S_OK);
    }
    else
    {
#if DBGLEVEL > 0
        WriteLogFile("CGMPsgCtlClassFactory::CreateInstance - unknown iid\r\n", true);
        FormatGuid(buf, "  riid: ", "\r\n", riid);
        WriteLogFile(buf, false);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }
}

HRESULT CGMPsgCtlClassFactory::LockServer(BOOL fLock)
{
#if DBGLEVEL > 0
    WriteLogFile("CGMPsgCtlClassFactory::LockServer\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

// CClsFacList

CClsFacList::CClsFacList(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_CurSize = 16;
    m_CurLen = 0;
    m_Incr = 4;
    m_List = (CGMPsgCtlClassFactory**)malloc(m_CurSize*sizeof(CGMPsgCtlClassFactory*));
}

CClsFacList::~CClsFacList()
{
    free(m_List);
}

CGMPsgCtlClassFactory* CClsFacList::GetClassFactory(CLSID rclsid)
{
    BOOL found = FALSE;
    int i = 0, emptyspace = -1;
    CGMPsgCtlClassFactory *res = NULL;
    while(!found && (i < m_CurLen))
    {
        if(m_List[i] != NULL) found = IsEqualCLSID(m_List[i++]->m_Class, rclsid);
        else emptyspace = i++;
    }
    if(found) res = m_List[--i];
    else
    {
        if(emptyspace > -1) i = emptyspace;
        else
        {
            if(m_CurSize <= m_CurLen)
            {
                m_CurSize += m_Incr;
                m_List = (CGMPsgCtlClassFactory**)realloc(m_List,
                    m_CurSize*sizeof(CGMPsgCtlClassFactory*));
            }
            i = m_CurLen++;
        }
        res = new CGMPsgCtlClassFactory(m_hInstance, &m_List[i], rclsid);
        res->AddRef();
    }
    return(res);
}

BOOL CClsFacList::IsEmpty()
{
    BOOL found = FALSE;
    int i = 0;
    while(!found && (i < m_CurLen))
    {
        found = (m_List[i++] != NULL);
    }
    return(!found);
}

