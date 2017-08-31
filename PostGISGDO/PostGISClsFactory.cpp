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

#include "PostGISClsFactory.hpp"
#ifdef DBGLEVEL
#include "../Common/LogFiles.hpp"
#endif // DBGLEVEL

extern wchar_t g_ModuleName[MAX_PATH];

// CPostGISClassFactory

CPostGISClassFactory::CPostGISClassFactory(HINSTANCE hInstance,
    CPostGISClassFactory **selfptr, CLSID rclsid)
{
#if DBGLEVEL > 1
    WriteLogFile("CPostGISClassFactory::CPostGISClassFactory\r\n", true);
#endif // DBGLEVEL
    m_hInstance = hInstance;
    m_lRefCount = 0;
    m_SelfPtr = selfptr;
    if(m_SelfPtr) *m_SelfPtr = this;
    m_Class = rclsid;
    for(int i = 0; i < ciTypeLibs; i++) m_pTypeLibs[i] = NULL;
    m_iListLen = 0;
    m_iListSize = 16;
    m_ClsObjList = (GDatabase**)malloc(m_iListSize*sizeof(GDatabase*));
}

CPostGISClassFactory::~CPostGISClassFactory()
{
#if DBGLEVEL > 1
    WriteLogFile("CPostGISClassFactory::~CPostGISClassFactory\r\n", true);
#endif // DBGLEVEL
    free(m_ClsObjList);
    if(m_SelfPtr) *m_SelfPtr = NULL;
    for(int i = 0; i < ciTypeLibs; i++)
    {
        if(m_pTypeLibs[i]) m_pTypeLibs[i]->Release();
    }
}

HRESULT CPostGISClassFactory::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CPostGISClassFactory::QueryInterface\r\n", true);
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

ULONG CPostGISClassFactory::AddRef()
{
#if DBGLEVEL > 2
    WriteLogFile("CPostGISClassFactory::AddRef\r\n", true);
#endif // DBGLEVEL
    return(++m_lRefCount);
}

ULONG CPostGISClassFactory::Release()
{
#if DBGLEVEL > 2
    WriteLogFile("CPostGISClassFactory::Release\r\n", true);
#endif // DBGLEVEL
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete this;
    return(0);
}

HRESULT CPostGISClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
#if DBGLEVEL > 0
    WriteLogFile("CPostGISClassFactory::CreateInstance\r\n", true);
#endif // DBGLEVEL
    *ppvObject = NULL;
    if(IsEqualCLSID(m_Class, CLASS_GDatabase))
    {
        wchar_t buf[MAX_PATH];
        for(int i = 0; i < ciTypeLibs; i++)
        {
            if(!m_pTypeLibs[i])
            {
                swprintf(buf, L"%s\\%d", g_ModuleName, 101 + i);
                LoadTypeLib(buf, &m_pTypeLibs[i]);
            }
        }

        if(m_iListLen >= m_iListSize)
        {
            m_iListSize += 4;
            m_ClsObjList = (GDatabase**)realloc(m_ClsObjList,
                m_iListSize*sizeof(GDatabase*));
        }

        ITypeLib **ppLibs = (ITypeLib**)malloc(ciTypeLibs*sizeof(ITypeLib*));
        for(int i = 0; i < ciTypeLibs; i++)
        {
            ppLibs[i] = m_pTypeLibs[i];
            ppLibs[i]->AddRef();
        }
        GDatabase *pDB = new GDatabase(m_hInstance, pUnkOuter, ppLibs);
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

HRESULT CPostGISClassFactory::LockServer(BOOL fLock)
{
#if DBGLEVEL > 0
    WriteLogFile("CPostGISClassFactory::LockServer\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

void CPostGISClassFactory::SetSelfPtr(void **ppPtr)
{
    m_SelfPtr = (CPostGISClassFactory**)ppPtr;
    return;
}


// CClsFacList

CClsFacList::CClsFacList(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_CurSize = 16;
    m_CurLen = 0;
    m_Incr = 4;
    m_List = (CPostGISClassFactory**)malloc(m_CurSize*sizeof(CPostGISClassFactory*));
}

CClsFacList::~CClsFacList()
{
    for(int i = 0; i < m_CurLen; i++)
    {
        if(m_List[i])
        {
            m_List[i]->SetSelfPtr(NULL);
            m_List[i]->Release();
        }
    }
    free(m_List);
}

CPostGISClassFactory* CClsFacList::GetClassFactory(CLSID rclsid)
{
    BOOL found = FALSE;
    int i = 0, emptyspace = -1;
    CPostGISClassFactory *res = NULL;
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
                m_List = (CPostGISClassFactory**)realloc(m_List,
                    m_CurSize*sizeof(CPostGISClassFactory*));
                for(i = 0; i < m_CurLen; i++)
                {
                    m_List[i]->SetSelfPtr((void**)&m_List[i]);
                }
            }
            i = m_CurLen++;
        }
        res = new CPostGISClassFactory(m_hInstance, &m_List[i], rclsid);
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
