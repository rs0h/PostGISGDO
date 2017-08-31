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

#ifndef _PGSCOLS_HPP_
#define _PGSCOLS_HPP_

#include "../Common/CComTypes.hpp"
#include "../Common/GDOUtils.hpp"
#include "../Common/PGSTypes.hpp"
#include "PGSGeom.hpp"
#include "../Common/PGSCoordSystems.hpp"

#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif

enum CCollItemStatus
{
    cisUnknown,
    cisNew,
    cisCreated
};

class CEnumGDO : public CCOMUnknown
{
private:
    int m_iListPos;
    int m_iListLen;
    CCOMDispatch **m_pList;
public:
    CEnumGDO(int iPos, int iLen, CCOMDispatch **pList);
    ~CEnumGDO();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual HRESULT __stdcall Next(ULONG celt, VARIANT *rgelt,
        ULONG *pceltFetchedd);
    virtual HRESULT __stdcall Skip(ULONG celt);
    virtual HRESULT __stdcall Reset();
    virtual HRESULT __stdcall Clone(CEnumGDO **ppenum);
};

class _IGCollection : public CCOMDispatch
{
protected:
    int m_iDataSize, m_iDataLen;
    CCOMDispatch **m_pData;
    bool m_bManageRefs;
public:
    _IGCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib);
    _IGCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~_IGCollection();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Count(short *Count);
    virtual HRESULT __stdcall _NewEnum(IUnknown * *Enum);
    int GetCount();
    IDispatch* GetItem(int iIndx);
    void ClearAll();
    void AddItem(CCOMDispatch *pDisp);
    void DeleteItem(int iIdx);
};

class _IGDynaCollection : public _IGCollection
{
public:
    _IGDynaCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib);
    _IGDynaCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~_IGDynaCollection();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall Append(IDispatch * Object);
    virtual HRESULT __stdcall Delete(BSTR Name) = 0;
};

class GError : public CCOMDispatch
{
private:
    long m_lNumber;
    LPWSTR m_sDesc;
    LPWSTR m_sSrc;
public:
    GError(IUnknown *pUnkOuter, ITypeLib *ALib);
    GError(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GError();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Description(BSTR *pDescription);
    virtual HRESULT __stdcall get_Number(long *pNumber);
    virtual HRESULT __stdcall get_Source(BSTR *pSource);
    virtual HRESULT __stdcall get_HelpFile(BSTR *pHelpFile);
    virtual HRESULT __stdcall get_HelpContext(long *pContext);
    void SetAttributes(long lNumber, LPWSTR sDesc, LPWSTR sSrc);
};

class GErrors : public _IGCollection
{
private:
    void AddError(long lNumber, LPWSTR sDesc, LPWSTR sSrc);
public:
    GErrors(IUnknown *pUnkOuter, ITypeLib *ALib);
    GErrors(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GErrors();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Item(VARIANT index, GError* *ppGError);
    void ClearAll();
    void HandlePQError(PGconn *pConn, HINSTANCE hInstance);
    void HandleGDOError(LPWSTR sMsg, HINSTANCE hInstance);
};

bool ExecuteCommand(PConnStruct pCSt, LPSTR sCmd);
bool ExecuteCommandParams(PConnStruct pCSt, LPSTR sCmd, int iParams,
    VARIANT *pvParams);

#endif
