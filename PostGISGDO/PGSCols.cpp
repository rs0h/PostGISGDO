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

#include "PGSCols.hpp"
#include "../Common/LogFiles.hpp"
#include "PostGIS.rh"
#include <stdio.h>

// CEnumGDO

CEnumGDO::CEnumGDO(int iPos, int iLen, CCOMDispatch **pList) : CCOMUnknown(NULL)
{
#if DBGLEVEL > 1
    WriteLogFile("CEnumGDO::CEnumGDO\r\n", true);
#endif // DBGLEVEL
    m_iListPos = iPos;
    m_iListLen = iLen;
    m_pList = pList;
}

CEnumGDO::~CEnumGDO()
{
#if DBGLEVEL > 1
    WriteLogFile("CEnumGDO::~CEnumGDO\r\n", true);
#endif // DBGLEVEL
}

HRESULT CEnumGDO::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CEnumGDO::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMUnknown::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, IID_IEnumVARIANT))
        {
            m_lRefCount++;
            *ppvObject = this;
            hres = S_OK;
        }
        else hres = E_NOINTERFACE;
    }
    return(hres);
}

HRESULT CEnumGDO::Next(ULONG celt, VARIANT *rgelt, ULONG *pceltFetched)
{
#if DBGLEVEL > 2
    WriteLogFile("CEnumGDO::Next\r\n", true);
    char buf[64];
    sprintf(buf, "  celt: %d, pos: %d, len: %d\r\n", celt, m_iListPos, m_iListLen);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    HRESULT hres = S_OK;
    ULONG iMaxSkip = m_iListLen - m_iListPos;
    if(celt > iMaxSkip)
    {
        celt = iMaxSkip;
        hres = S_FALSE;
    }

    for(int i = 0; i < (int)celt; i++)
    {
        VariantClear(&rgelt[i]);
        m_pList[m_iListPos + i]->AddRef();
        rgelt[i].vt = VT_DISPATCH;
        rgelt[i].pdispVal = m_pList[m_iListPos + i];
    }

    if(pceltFetched) *pceltFetched = celt;
    m_iListPos += celt;
    return(hres);
}

HRESULT CEnumGDO::Skip(ULONG celt)
{
#if DBGLEVEL > 0
    WriteLogFile("CEnumGDO::Skip\r\n", true);
#endif // DBGLEVEL
    ULONG iMaxSkip = m_iListLen - m_iListPos;
    if(celt > iMaxSkip)
    {
        m_iListPos = m_iListLen;
        return(S_FALSE);
    }

    m_iListPos += celt;
    return(S_OK);
}

HRESULT CEnumGDO::Reset()
{
#if DBGLEVEL > 0
    WriteLogFile("CEnumGDO::Reset\r\n", true);
#endif // DBGLEVEL
    m_iListPos = 0;
    return(S_OK);
}

HRESULT CEnumGDO::Clone(CEnumGDO **ppenum)
{
#if DBGLEVEL > 0
    WriteLogFile("CEnumGDO::Clone\r\n", true);
#endif // DBGLEVEL
    if(!ppenum) return(S_FALSE);
    if(*ppenum) (*ppenum)->Release();
    *ppenum = new CEnumGDO(m_iListPos, m_iListLen, m_pList);
    (*ppenum)->AddRef();
    return(S_OK);
}


//  _IGCollection

_IGCollection::_IGCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib) :
    CCOMDispatch(pUnkOuter, ALib, 25)
{
#if DBGLEVEL > 1
    WriteLogFile("_IGCollection::_IGCollection-1\r\n", true);
#endif // DBGLEVEL
    m_bManageRefs = bManageRefs;
    m_iDataSize = 1024;//64;
    m_iDataLen = 0;
    m_pData = (CCOMDispatch**)malloc(m_iDataSize*sizeof(CCOMDispatch*));
}

_IGCollection::_IGCollection(bool bManageRefs, IUnknown *pUnkOuter, ITypeLib *ALib,
    int iIndex) : CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("_IGCollection::_IGCollection-2\r\n", true);
#endif // DBGLEVEL
    m_bManageRefs = bManageRefs;
    m_iDataSize = 1024;//64;
    m_iDataLen = 0;
    m_pData = (CCOMDispatch**)malloc(m_iDataSize*sizeof(CCOMDispatch*));
}

_IGCollection::~_IGCollection()
{
#if DBGLEVEL > 1
    WriteLogFile("_IGCollection::~_IGCollection\r\n", true);
#endif // DBGLEVEL
    ClearAll();
    free(m_pData);
}

HRESULT _IGCollection::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("_IGCollection::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID__IGCollection))
        {
            *ppvObject = this;
            hres = S_OK;
        }
        else hres = E_NOINTERFACE;

        if(hres == S_OK) ((IUnknown*)*ppvObject)->AddRef();
    }
    return(hres);
}

ULONG _IGCollection::Release()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(m_pUnkOuter) sprintf(buf, "_IGCollection::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "_IGCollection::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(m_pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT _IGCollection::get_Count(short *Count)
{
#if DBGLEVEL > 0
    WriteLogFile("_IGCollection::get_Count\r\n", true);
#endif // DBGLEVEL
    if(!Count) return(E_POINTER);
    *Count = m_iDataLen;
    return(S_OK);
}

HRESULT _IGCollection::_NewEnum(IUnknown * *Enum)
{
#if DBGLEVEL > 0
    WriteLogFile("_IGCollection::_NewEnum\r\n", true);
#endif // DBGLEVEL
    if(!Enum) return(E_POINTER);
    if(*Enum) (*Enum)->Release();
    *Enum = new CEnumGDO(0, m_iDataLen, m_pData);
    (*Enum)->AddRef();
    return(S_OK);
}

int _IGCollection::GetCount()
{
    return(m_iDataLen);
}

IDispatch* _IGCollection::GetItem(int iIndx)
{
    if((iIndx < 0) || (iIndx >= m_iDataLen)) return(NULL);
    return(m_pData[iIndx]);
}

void _IGCollection::ClearAll()
{
    if(m_bManageRefs)
    {
        for(int i = 0; i < m_iDataLen; i++)
        {
            m_pData[i]->SetOuter(NULL);
        }
    }
    m_iDataLen = 0;
    return;
}

void _IGCollection::AddItem(CCOMDispatch *pDisp)
{
    if(m_iDataLen >= m_iDataSize)
    {
        m_iDataSize += 16;
        m_pData = (CCOMDispatch**)realloc(m_pData, m_iDataSize*sizeof(CCOMDispatch*));
    }
    m_pData[m_iDataLen++] = pDisp;
    if(m_bManageRefs) pDisp->SetOuter(this);
    return;
}

void _IGCollection::DeleteItem(int iIdx)
{
    CCOMDispatch *pobj = m_pData[iIdx];
    if(m_bManageRefs) pobj->SetOuter(NULL);

    m_iDataLen--;
    int icnt = m_iDataLen - iIdx;
    if(icnt > 0)
    {
        memmove(&m_pData[iIdx], &m_pData[iIdx + 1], icnt*sizeof(CCOMDispatch*));
    }
    return;
}


//  _IGDynaCollection

_IGDynaCollection::_IGDynaCollection(bool bManageRefs, IUnknown *pUnkOuter,
    ITypeLib *ALib) : _IGCollection(bManageRefs, pUnkOuter, ALib, 24)
{
#if DBGLEVEL > 1
    WriteLogFile("_IGDynaCollection::_IGDynaCollection-1\r\n", true);
#endif // DBGLEVEL
}

_IGDynaCollection::_IGDynaCollection(bool bManageRefs, IUnknown *pUnkOuter,
    ITypeLib *ALib, int iIndex) :
    _IGCollection(bManageRefs, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("_IGDynaCollection::_IGDynaCollection-2\r\n", true);
#endif // DBGLEVEL
}

_IGDynaCollection::~_IGDynaCollection()
{
#if DBGLEVEL > 1
    WriteLogFile("_IGDynaCollection::~_IGDynaCollection\r\n", true);
#endif // DBGLEVEL
}

HRESULT _IGDynaCollection::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("_IGDynaCollection::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID__IGDynaCollection))
        {
            *ppvObject = this;
            hres = S_OK;
        }
        else hres = E_NOINTERFACE;

        if(hres == S_OK) ((IUnknown*)*ppvObject)->AddRef();
    }
    return(hres);
}

ULONG _IGDynaCollection::Release()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(m_pUnkOuter) sprintf(buf, "_IGDynaCollection::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "_IGDynaCollection::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(m_pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT _IGDynaCollection::Append(IDispatch * Object)
{
#if DBGLEVEL > 0
    WriteLogFile("_IGDynaCollection::Append\r\n", true);
#endif // DBGLEVEL
    if(!Object) return(E_POINTER);
    _IGCollection::AddItem((CCOMDispatch*)Object);
    return(S_OK);
}

// We declare the _IGDynaCollection::Delete method as pure virtual, because
// of the Name index
/*HRESULT _IGDynaCollection::Delete(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("_IGDynaCollection::Delete\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}*/


//  GError

GError::GError(IUnknown *pUnkOuter, ITypeLib *ALib) : CCOMDispatch(pUnkOuter, ALib, 21)
{
#if DBGLEVEL > 1
    WriteLogFile("GError::GError-1\r\n", true);
#endif // DBGLEVEL
    m_lNumber = 0;
    m_sDesc = NULL;
    m_sSrc = NULL;
}

GError::GError(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GError::GError-2\r\n", true);
#endif // DBGLEVEL
    m_lNumber = 0;
    m_sDesc = NULL;
    m_sSrc = NULL;
}

GError::~GError()
{
#if DBGLEVEL > 1
    WriteLogFile("GError::~GError\r\n", true);
#endif // DBGLEVEL
    if(m_sSrc) free(m_sSrc);
    if(m_sDesc) free(m_sDesc);
}

HRESULT GError::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GError::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GError))
        {
            *ppvObject = this;
            hres = S_OK;
        }
        else hres = E_NOINTERFACE;

        if(hres == S_OK) ((IUnknown*)*ppvObject)->AddRef();
    }
    return(hres);
}

ULONG GError::Release()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(m_pUnkOuter) sprintf(buf, "GError::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GError::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(m_pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GError::get_Description(BSTR *pDescription)
{
#if DBGLEVEL > 0
    WriteLogFile("GError::get_Description\r\n", true);
#endif // DBGLEVEL
    if(!pDescription) return(E_POINTER);

    if(m_sDesc) *pDescription = SysAllocString(m_sDesc);
    else *pDescription = SysAllocString(L"");
    return(S_OK);
}

HRESULT GError::get_Number(long *pNumber)
{
#if DBGLEVEL > 0
    WriteLogFile("GError::get_Number\r\n", true);
#endif // DBGLEVEL
    if(!pNumber) return(E_POINTER);

    *pNumber = m_lNumber;
    return(S_OK);
}

HRESULT GError::get_Source(BSTR *pSource)
{
#if DBGLEVEL > 0
    WriteLogFile("GError::get_Source\r\n", true);
#endif // DBGLEVEL
    if(!pSource) return(E_POINTER);

    if(m_sSrc) *pSource = SysAllocString(m_sSrc);
    else *pSource = SysAllocString(L"");
    return(S_OK);
}

HRESULT GError::get_HelpFile(BSTR *pHelpFile)
{
#if DBGLEVEL > 0
    WriteLogFile("GError::get_HelpFile\r\n", true);
#endif // DBGLEVEL
    if(!pHelpFile) return(E_POINTER);

    *pHelpFile = SysAllocString(L"");
    return(S_OK);
}

HRESULT GError::get_HelpContext(long *pContext)
{
#if DBGLEVEL > 0
    WriteLogFile("GError::get_HelpContext\r\n", true);
#endif // DBGLEVEL
    if(!pContext) return(E_POINTER);

    *pContext = 0;
    return(S_OK);
}

void GError::SetAttributes(long lNumber, LPWSTR sDesc, LPWSTR sSrc)
{
    if(m_sSrc) free(m_sSrc);
    if(m_sDesc) free(m_sDesc);
    m_sSrc = NULL;
    m_sDesc = NULL;

    m_lNumber = lNumber;
    if(sDesc)
    {
        m_sDesc = (LPWSTR)malloc((wcslen(sDesc) + 1)*sizeof(wchar_t));
        wcscpy(m_sDesc, sDesc);
    }
    if(sSrc)
    {
        m_sSrc = (LPWSTR)malloc((wcslen(sSrc) + 1)*sizeof(wchar_t));
        wcscpy(m_sSrc, sSrc);
    }
    return;
}


//  GErrors

GErrors::GErrors(IUnknown *pUnkOuter, ITypeLib *ALib) :
    _IGCollection(true, pUnkOuter, ALib, 31)
{
#if DBGLEVEL > 1
    WriteLogFile("GErrors::GErrors-1\r\n", true);
#endif // DBGLEVEL
}

GErrors::GErrors(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    _IGCollection(true, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GErrors::GErrors-2\r\n", true);
#endif // DBGLEVEL
}

GErrors::~GErrors()
{
#if DBGLEVEL > 1
    WriteLogFile("GErrors::~GErrors\r\n", true);
#endif // DBGLEVEL
}

HRESULT GErrors::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GErrors::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GErrors))
        {
            *ppvObject = this;
            hres = S_OK;
        }
        else hres = E_NOINTERFACE;

        if(hres == S_OK) ((IUnknown*)*ppvObject)->AddRef();
    }
    return(hres);
}

ULONG GErrors::Release()
{
    ULONG lRest;
    if(m_pUnkOuter) lRest = m_pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(m_pUnkOuter) sprintf(buf, "GErrors::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GErrors::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(m_pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GErrors::get_Item(VARIANT index, GError* *ppGError)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GErrors::get_Item\r\n", true);
#endif // DBGLEVEL
    if(!ppGError) return(E_POINTER);

    switch(index.vt)
    {
    case VT_UI1:
        *ppGError = (GError*)_IGCollection::GetItem(index.bVal);
        break;
    case VT_I2:
        *ppGError = (GError*)_IGCollection::GetItem(index.iVal);
        break;
    case VT_I4:
        *ppGError = (GError*)_IGCollection::GetItem(index.lVal);
        break;
    default:
        return(E_NOTIMPL);
    }
    return(S_OK);
}

void GErrors::AddError(long lNumber, LPWSTR sDesc, LPWSTR sSrc)
{
    GError *pErr = new GError(this, m_pTypeLib); //, 21);
    pErr->SetAttributes(lNumber, sDesc, sSrc);
    _IGCollection::AddItem(pErr);
    return;
}

void GErrors::ClearAll()
{
    _IGCollection::ClearAll();
    return;
}

void GErrors::HandleGDOError(LPWSTR sMsg, HINSTANCE hInstance)
{
#if DBGLEVEL > 0
    WriteLogFile("GErrors::HandleGDOError\r\n", true);
#endif // DBGLEVEL
    ClearAll();

    wchar_t sBuf[64];
    LoadString(hInstance, IDE_GDOERROR, sBuf, 64);
    AddError(1001, sMsg, sBuf);
    return;
}

void GErrors::HandlePQError(PGconn *pConn, HINSTANCE hInstance)
{
#if DBGLEVEL > 0
    WriteLogFile("GErrors::HandlePQError\r\n", true);
#endif // DBGLEVEL
    LPSTR sErr = PQerrorMessage(pConn);
    if(!sErr) return;

    if(sErr[0])
    {
#if DBGLEVEL > 0
        WriteLogFile("  Message: ", false);
        if(sErr) WriteLogFile(sErr, false);
        WriteLogFile("\r\n", false);
#endif // DBGLEVEL
        ClearAll();
        wchar_t sBuf[64];
        LoadString(hInstance, IDE_GDOERROR, sBuf, 64);
        int slen = strlen(sErr) + 1;
        LPWSTR wsErr = (LPWSTR)malloc(slen*sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sErr, -1, wsErr, slen);
        AddError(1002, wsErr, sBuf);

        ICreateErrorInfo *pCrErrInfo = NULL;
        IErrorInfo *pErrInfo = NULL;

        CreateErrorInfo(&pCrErrInfo);
        pCrErrInfo->SetGUID(DIID_GDatabase);
        pCrErrInfo->SetDescription(wsErr);
        pCrErrInfo->SetSource(sBuf);

        pCrErrInfo->QueryInterface(IID_IErrorInfo, (void**)&pErrInfo);
        SetErrorInfo(0, pErrInfo);
        pErrInfo->Release();
        pCrErrInfo->Release();

        free(wsErr);
    }
    return;
}

bool ExecuteCommand(PConnStruct pCSt, LPSTR sCmd)
{
#if DBGLEVEL > 0
    WriteLogFile("ExecuteCommand\r\n    ", true);
    WriteLogFile(sCmd, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    bool bTrans = (pCSt->iSysTrans | pCSt->iGdoTrans);
    SetSP(pCSt->pConn, bTrans);
    PGresult *res = PQexec(pCSt->pConn, sCmd);
    ExecStatusType esres = PQresultStatus(res);
    if((esres != PGRES_COMMAND_OK) && (esres != PGRES_TUPLES_OK))
    {
        ((GErrors*)pCSt->pErrors)->HandlePQError(pCSt->pConn, pCSt->hInstance);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return(true);
}

bool ExecuteCommandParams(PConnStruct pCSt, LPSTR sCmd, int iParams,
    VARIANT *pvParams)
{
#if DBGLEVEL > 0
    WriteLogFile("ExecuteCommandParams\r\n    ", true);
    WriteLogFile(sCmd, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL

    char **ppParams = (char**)malloc(iParams*sizeof(char*));
    int *piParLengths = (int*)malloc(iParams*sizeof(int));
    int *piParFormats = (int*)malloc(iParams*sizeof(int));

    CGeomInfo cgInfo = {0, 0, 0, 0, 0};
    long lParams[8];

    for(int i = 0; i < iParams; i++)
    {
        piParLengths[i] = VarToBinaryLen(pvParams[i], 0, pCSt->lDBEnc, &cgInfo,
            lParams);
        if(piParLengths[i] > 0)
        {
            ppParams[i] = (LPSTR)malloc(piParLengths[i] + 1);
            VarToBinaryBuf(pvParams[i], 0, pCSt->lDBEnc, ppParams[i],
                piParLengths[i], &cgInfo, lParams, pCSt->iCurrDigits);
        }
        else ppParams[i] = NULL;
        piParFormats[i] = 1;
    }

    bool bTrans = (pCSt->iSysTrans | pCSt->iGdoTrans);
    SetSP(pCSt->pConn, bTrans);
    PGresult *res = PQexecParams(pCSt->pConn, sCmd, iParams, NULL, ppParams,
        piParLengths, piParFormats, 1);
    ExecStatusType esres = PQresultStatus(res);

    for(int i = 0; i < iParams; i++)
    {
        free(ppParams[i]);
    }
    free(piParFormats);
    free(piParLengths);
    free(ppParams);

    if((esres != PGRES_COMMAND_OK) && (esres != PGRES_TUPLES_OK))
    {
        ((GErrors*)pCSt->pErrors)->HandlePQError(pCSt->pConn, pCSt->hInstance);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return(true);
}
