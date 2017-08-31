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

#include "PsgDBUtilsClsFactory.hpp"
#ifdef DBGLEVEL
#include "../Common/LogFiles.hpp"
#endif // DBGLEVEL

extern wchar_t g_ModuleName[MAX_PATH];

// CPsgDBUtilsClassFactory

CPsgDBUtilsClassFactory::CPsgDBUtilsClassFactory(HINSTANCE hInstance,
    CPsgDBUtilsClassFactory **selfptr, CLSID rclsid, ITypeLib *pTypeLib, PConnList pConnList)
{
#if DBGLEVEL > 1
    WriteLogFile("CPsgDBUtilsClassFactory::CPsgDBUtilsClassFactory\r\n", true);
#endif // DBGLEVEL
    m_hInstance = hInstance;
    m_lRefCount = 0;
    m_SelfPtr = selfptr;
    if(m_SelfPtr) *m_SelfPtr = this;
    m_Class = rclsid;
    m_pTypeLib = pTypeLib;
    m_pConnList = pConnList;
    m_iListLen = 0;
    m_iListSize = 16;
    m_ClsObjList = (CDbuConnectionDisp**)malloc(m_iListSize*sizeof(CDbuConnectionDisp*));
}

CPsgDBUtilsClassFactory::~CPsgDBUtilsClassFactory()
{
#if DBGLEVEL > 1
    WriteLogFile("CPsgDBUtilsClassFactory::~CPsgDBUtilsClassFactory\r\n", true);
#endif // DBGLEVEL
    free(m_ClsObjList);
    if(m_SelfPtr) *m_SelfPtr = NULL;
}

HRESULT CPsgDBUtilsClassFactory::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CPsgDBUtilsClassFactory::QueryInterface\r\n", true);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IClassFactory))
    {
        *ppvObject = this;
        return(S_OK);
    }
#if DBGLEVEL > 2
    char buf[128];
    FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(E_NOINTERFACE);
}

ULONG CPsgDBUtilsClassFactory::AddRef()
{
#if DBGLEVEL > 2
    WriteLogFile("CPsgDBUtilsClassFactory::AddRef\r\n", true);
#endif // DBGLEVEL
    return(++m_lRefCount);
}

ULONG CPsgDBUtilsClassFactory::Release()
{
#if DBGLEVEL > 2
    WriteLogFile("CPsgDBUtilsClassFactory::Release\r\n", true);
#endif // DBGLEVEL
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete this;
    return(0);
}

HRESULT CPsgDBUtilsClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
#if DBGLEVEL > 0
    WriteLogFile("CPsgDBUtilsClassFactory::CreateInstance\r\n", true);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualCLSID(m_Class, CLASS_DbuConnection))
    {
        if(m_iListLen >= m_iListSize)
        {
            m_iListSize += 4;
            m_ClsObjList = (CDbuConnectionDisp**)realloc(m_ClsObjList,
                m_iListSize*sizeof(CDbuConnectionDisp*));
        }
        m_pTypeLib->AddRef();
        CDbuConnectionDisp *pDB = new CDbuConnectionDisp(pUnkOuter,
            m_pTypeLib, 4, m_hInstance, m_pConnList);
        if(pDB->QueryInterface(riid, ppvObject) == S_OK)
        {
            m_ClsObjList[m_iListLen++] = pDB;
            return(S_OK);
        }
        delete pDB;
    }
#if DBGLEVEL > 2
    char buf[128];
    FormatGuid(buf, "Unknown Interface: ", "\r\n", riid);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(E_NOINTERFACE);
}

HRESULT CPsgDBUtilsClassFactory::LockServer(BOOL fLock)
{
#if DBGLEVEL > 0
    WriteLogFile("CPsgDBUtilsClassFactory::LockServer\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

void CPsgDBUtilsClassFactory::SetSelfPtr(void **ppPtr)
{
    m_SelfPtr = (CPsgDBUtilsClassFactory**)ppPtr;
    return;
}


// CClsFacList

CClsFacList::CClsFacList(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_CurSize = 16;
    m_CurLen = 0;
    m_Incr = 4;
    m_List = (CPsgDBUtilsClassFactory**)malloc(m_CurSize*sizeof(CPsgDBUtilsClassFactory*));
    m_pConnList = NULL;
    m_pTypeLib = NULL;
}

CClsFacList::~CClsFacList()
{
    if(m_pTypeLib) m_pTypeLib->Release();
    for(int i = 0; i < m_CurLen; i++)
    {
        if(m_List[i])
        {
            m_List[i]->SetSelfPtr(NULL);
            m_List[i]->Release();
        }
    }
    free(m_List);
    delete m_pConnList;
}

CPsgDBUtilsClassFactory* CClsFacList::GetClassFactory(CLSID rclsid)
{
    if(!m_pConnList)
    {
        m_pConnList = new CConnList(m_hInstance);
    }

    if(!m_pTypeLib)
    {
        wchar_t buf[MAX_PATH];
        swprintf(buf, L"%s\\%d", g_ModuleName, 1);
        LoadTypeLib(buf, &m_pTypeLib);
    }

    BOOL found = FALSE;
    int i = 0, emptyspace = -1;
    CPsgDBUtilsClassFactory *res = NULL;
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
                m_List = (CPsgDBUtilsClassFactory**)realloc(m_List,
                    m_CurSize*sizeof(CPsgDBUtilsClassFactory*));
                for(i = 0; i < m_CurLen; i++)
                {
                    m_List[i]->SetSelfPtr((void**)&m_List[i]);
                }
            }
            i = m_CurLen++;
        }
        res = new CPsgDBUtilsClassFactory(m_hInstance, &m_List[i], rclsid, m_pTypeLib, m_pConnList);
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

HINSTANCE CClsFacList::GetInstance()
{
    return(m_hInstance);
}

ITypeLib* CClsFacList::GetTypeLib()
{
    if(!m_pTypeLib)
    {
        wchar_t buf[MAX_PATH];
        swprintf(buf, L"%s\\%d", g_ModuleName, 1);
        LoadTypeLib(buf, &m_pTypeLib);
    }
    return(m_pTypeLib);
}

PConnList CClsFacList::GetConnList()
{
    if(!m_pConnList)
    {
        m_pConnList = new CConnList(m_hInstance);
    }
    return(m_pConnList);
}
