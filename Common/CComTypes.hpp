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

#ifndef _CCOMTYPES_HPP_
#define _CCOMTYPES_HPP_

#include <wtypes.h>

#ifdef __GNUC__
#if __GNUC__ < 4
const IID IID_IViewObjectEx = {0x3AF24292,0x0C96,0x11CE,{0xA0,0xCF,0x00,0xAA,0x00,0x60,0x0A,0xB8}};
#endif
#endif

void FormatGuid(LPSTR ABuf, LPCSTR sPref, LPCSTR sPost, GUID iid);
void ValidateVariant(VARIANT *pV);

class CCOMUnknown : public IUnknown
{
protected:
    IUnknown *m_pUnkOuter;
    ULONG m_lRefCount;
public:
    CCOMUnknown(IUnknown *pUnkOuter);
    ~CCOMUnknown();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
};

class CCOMDispatch : public IDispatch
{
protected:
    ULONG m_lRefCount;
    IUnknown *m_pUnkOuter;
    ITypeLib *m_pTypeLib;
    ITypeInfo *m_pTypeInfo;
public:
    CCOMDispatch(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~CCOMDispatch();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetTypeInfoCount(unsigned int *pctinfo);
    virtual HRESULT __stdcall GetTypeInfo(unsigned int iTInfo, LCID lcid,
        ITypeInfo **ppTInfo);
    virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, OLECHAR **rgszNames,
        unsigned int cNames, LCID lcid, DISPID *rgDispId);
    virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid,
        LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo, unsigned int *puArgErr);
    // setting outer to NULL will delete the object
    void SetOuter(IUnknown *pUnkOuter);
};

#endif // _LOGFILES_HPP_
