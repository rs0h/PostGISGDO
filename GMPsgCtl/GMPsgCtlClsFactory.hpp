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

#ifndef _GMPsgCtl_CLSFACTORY_HPP_
#define _GMPsgCtl_CLSFACTORY_HPP_

#include <stdio.h>
#include <windows.h>
#include "GMPsgCtlIntfc.hpp"

class CGMPsgCtlClassFactory
{
private:
    ULONG m_lRefCount;
    CGMPsgCtlClassFactory **m_SelfPtr;
    int m_iListLen;
    int m_iListSize;
    _GMPostGISConnectionProps **m_ClsObjList;
    ITypeLib *m_pTypeLib;
    HINSTANCE m_hInstance;
public:
    CLSID m_Class;
    CGMPsgCtlClassFactory(HINSTANCE hInstance, CGMPsgCtlClassFactory **selfptr, CLSID rclsid);
    ~CGMPsgCtlClassFactory();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
    virtual HRESULT __stdcall LockServer(BOOL fLock);
};

class CClsFacList
{
private:
    int m_CurSize;
    int m_CurLen;
    int m_Incr;
    CGMPsgCtlClassFactory **m_List;
    HINSTANCE m_hInstance;
public:
    CClsFacList(HINSTANCE hInstance);
    ~CClsFacList();
    CGMPsgCtlClassFactory* GetClassFactory(CLSID rclsid);
    BOOL IsEmpty();
};

#endif // _GMPsgCtl_CLSFACTORY_HPP_
