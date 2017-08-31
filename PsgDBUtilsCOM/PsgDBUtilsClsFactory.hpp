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

#ifndef _PsgDBUtils_CLSFACTORY_HPP_
#define _PsgDBUtils_CLSFACTORY_HPP_

#include <stdio.h>
#include <windows.h>
#include "PsgDBUtilsCls.hpp"
#include "../PsgDBUtilsDLL/PsgDBUDLLMain.hpp"

class CPsgDBUtilsClassFactory
{
private:
    ULONG m_lRefCount;
    CPsgDBUtilsClassFactory **m_SelfPtr;
    int m_iListLen;
    int m_iListSize;
    CDbuConnectionDisp **m_ClsObjList;
    ITypeLib *m_pTypeLib;
    HINSTANCE m_hInstance;
    PConnList m_pConnList;
public:
    CLSID m_Class;
    CPsgDBUtilsClassFactory(HINSTANCE hInstance, CPsgDBUtilsClassFactory **selfptr,
        CLSID rclsid, ITypeLib *pTypeLib, PConnList pConnList);
    ~CPsgDBUtilsClassFactory();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
    virtual HRESULT __stdcall LockServer(BOOL fLock);
    void SetSelfPtr(void **ppPtr);
};

class CClsFacList
{
private:
    int m_CurSize;
    int m_CurLen;
    int m_Incr;
    CPsgDBUtilsClassFactory **m_List;
    HINSTANCE m_hInstance;
    ITypeLib *m_pTypeLib;
    PConnList m_pConnList;
public:
    CClsFacList(HINSTANCE hInstance);
    ~CClsFacList();
    CPsgDBUtilsClassFactory* GetClassFactory(CLSID rclsid);
    BOOL IsEmpty();
    HINSTANCE GetInstance();
    ITypeLib* GetTypeLib();
    PConnList GetConnList();
};

#endif // _PostGIS_CLSFACTORY_HPP_
