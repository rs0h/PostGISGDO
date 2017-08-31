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

#ifndef _REGISTRAR_HPP_
#define _REGISTRAR_HPP_

#include <basetyps.h>
#include <wtypes.h>
#include <ocidl.h>

const CLSID CLASS_Registrar = {0x44EC053A,0x400F,0x11D0,{0x9D,0xCD,0x00,0xA0,0xC9,0x03,0x91,0xD3}};
const IID IID_IRegistrar = {0x44EC053B,0x400F,0x11D0,{0x9D,0xCD,0x00,0xA0,0xC9,0x03,0x91,0xD3}};

class IRegistrar
{
public:
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject) = 0;
    virtual ULONG __stdcall AddRef() = 0;
    virtual ULONG __stdcall Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE AddReplacement(LPCOLESTR key, LPCOLESTR item) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClearReplacements() = 0;
    virtual HRESULT STDMETHODCALLTYPE ResourceRegisterSz(LPCOLESTR resFileName,
        LPCOLESTR szID, LPCOLESTR szType) = 0;
    virtual HRESULT STDMETHODCALLTYPE ResourceUnregisterSz(LPCOLESTR resFileName,
        LPCOLESTR szID, LPCOLESTR szType) = 0;
    virtual HRESULT STDMETHODCALLTYPE FileRegister(LPCOLESTR fileName) = 0;
    virtual HRESULT STDMETHODCALLTYPE FileUnregister(LPCOLESTR fileName) = 0;
    virtual HRESULT STDMETHODCALLTYPE StringRegister(LPCOLESTR data) = 0;
    virtual HRESULT STDMETHODCALLTYPE StringUnregister(LPCOLESTR data) = 0;
    virtual HRESULT STDMETHODCALLTYPE ResourceRegister(LPCOLESTR resFileName,
        UINT nID, LPCOLESTR szType) = 0;
    virtual HRESULT STDMETHODCALLTYPE ResourceUnregister(LPCOLESTR resFileName,
        UINT nID, LPCOLESTR szType) = 0;
};

#endif
