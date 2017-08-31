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

#ifndef _PostGIS_CLSFACTORY_HPP_
#define _PostGIS_CLSFACTORY_HPP_

#include <stdio.h>
#include <windows.h>
#include "PostGISGDO.hpp"
//#include "PostGISExtendedPS.hpp"

class CPostGISClassFactory
{
private:
    ULONG m_lRefCount;
    CPostGISClassFactory **m_SelfPtr;
    int m_iListLen;
    int m_iListSize;
    GDatabase **m_ClsObjList;
    ITypeLib *m_pTypeLibs[ciTypeLibs];
    HINSTANCE m_hInstance;
public:
    CLSID m_Class;
    CPostGISClassFactory(HINSTANCE hInstance, CPostGISClassFactory **selfptr,
        CLSID rclsid);
    ~CPostGISClassFactory();
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
    CPostGISClassFactory **m_List;
    HINSTANCE m_hInstance;
public:
    CClsFacList(HINSTANCE hInstance);
    ~CClsFacList();
    CPostGISClassFactory* GetClassFactory(CLSID rclsid);
    BOOL IsEmpty();
    HINSTANCE GetInstance();
};

#endif // _PostGIS_CLSFACTORY_HPP_
