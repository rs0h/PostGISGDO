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

#include "PGSRecordset.hpp"
#include "../Common/LogFiles.hpp"
#include "../Common/PGtoGDOmaps.hpp"
#include "../Common/VarUtils.hpp"
#include "../Common/GDOtoEWKB.hpp"
#include "../Common/pg_const.h"
#include <stdio.h>
#include "PostGISGDO.hpp"
#include "PostGIS.rh"

LPWSTR GetTableName(LPWSTR sSQL)
{
    LPWSTR sRes = NULL;
    if(!wcschr(sSQL, ' '))
    {
        sRes = (LPWSTR)malloc((wcslen(sSQL) + 1)*sizeof(wchar_t));
        wcscpy(sRes, sSQL);
    }
    else
    {
        LPWSTR sFrom = wcsstr(sSQL, L" FROM ");
        if(!sFrom) sFrom = wcsstr(sSQL, L" from ");
        if(!sFrom) sFrom = wcsstr(sSQL, L" From ");
        if(sFrom)
        {
            sFrom += 6;
            while((sFrom[0] == ' ') && (sFrom[0] != 0))
            {
                sFrom++;
            }
            int slen = wcslen(sFrom);

            LPWSTR sSpace = wcschr(sFrom, ' ');
            LPWSTR sFirstDblQ = wcschr(sFrom, '\"');

            bool bDoubleQuoted = false;
            if(sFirstDblQ)
            {
                if(sSpace)
                {
                    if(sFirstDblQ < sSpace) bDoubleQuoted = true;
                    else slen = sSpace - sFrom;
                }
                else bDoubleQuoted = true;
            }
            else if(sSpace) slen = sSpace - sFrom;

            if(bDoubleQuoted)
            {
                LPWSTR sNextDblQ = wcschr(sFirstDblQ + 1, '\"');
                LPWSTR sDot = wcschr(sFrom, '.');
                if(sDot)
                {
                    if(sDot < sFirstDblQ)
                    {
                        if(sNextDblQ) slen = sNextDblQ - sFrom + 1;
                    }
                    else if(sNextDblQ)
                    {
                        if(sDot == sNextDblQ + 1)
                        {
                            sDot++;
                            if(sDot[0] == '\"')
                            {
                                sDot++;
                                sNextDblQ = wcschr(sDot, '\"');
                                if(sNextDblQ) slen = sNextDblQ - sFrom + 1;
                            }
                            else
                            {
                                sSpace = wcschr(sDot, ' ');
                                if(sSpace) slen = sSpace - sFrom;
                            }
                        }
                        else slen = sNextDblQ - sFrom + 1;
                    }
                }
                else
                {
                    if(sNextDblQ) slen = sNextDblQ - sFrom + 1;
                }
            }

            sRes = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
            wcsncpy(sRes, sFrom, slen);
            sRes[slen] = 0;
#if DBGLEVEL > 2
            WriteMallocReport(1, slen, wcslen(sRes));
#endif
        }
        else
        {
            // the table name contains space(s}. If it is correctly enclosed
            // double quotes, then it should work
            sRes = (LPWSTR)malloc((wcslen(sSQL) + 1)*sizeof(wchar_t));
            wcscpy(sRes, sSQL);
        }
    }
    return(sRes);
}

bool IsMetatable(PConnStruct pConnStruct, LPSTR sName)
{
    if(stricmp(pConnStruct->sGAlias, sName) == 0) return(true);

    bool bTrans = (pConnStruct->iSysTrans | pConnStruct->iGdoTrans);

    int iLen = 40 + strlen(pConnStruct->sGAlias) + strlen(sName);
    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    sprintf(sSql, "select * from %s where tablename = '%s'",
        pConnStruct->sGAlias, sName);
#if DBGLEVEL > 2
    WriteMallocReport(2, iLen - 1, strlen(sSql));
#endif

    SetSP(pConnStruct->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sSql, bTrans);
    PGresult *res = PQexec(pConnStruct->pConn, sSql);
    //free(lsCmd);
    free(sSql);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        RollbackSP(pConnStruct->pConn, bTrans);
        return(false);
    }

    long nrows = PQntuples(res);
    PQclear(res);
    ReleaseSP(pConnStruct->pConn, bTrans);

    return(nrows > 0);

    /*if(stricmp(pConnStruct->sGFeatures, sName) == 0) return(true);
    if(stricmp(pConnStruct->sFieldLookup, sName) == 0) return(true);
    if(stricmp(pConnStruct->sAttributeProperties, sName) == 0) return(true);
    if(stricmp(pConnStruct->sGeometryProperties, sName) == 0) return(true);
    if(stricmp(pConnStruct->sCoordSystems, sName) == 0) return(true);
    if(stricmp(pConnStruct->sModTables, sName) == 0) return(true);
    if(stricmp(pConnStruct->sModLog, sName) == 0) return(true);
    if(stricmp(pConnStruct->sPickLists, sName) == 0) return(true);
    return(false);*/
}


//  FldExtendedPropertySet

FldExtendedPropertySet::FldExtendedPropertySet(IUnknown *pUnkOuter, ITypeLib *ALib) :
    CCOMDispatch(pUnkOuter, ALib, 0)
{
#if DBGLEVEL > 1
    WriteLogFile("FldExtendedPropertySet::FldExtendedPropertySet-1\r\n", true);
#endif // DBGLEVEL
    m_lSinglePass = 0;
    m_lCache = 0;
    m_pField = NULL;
}

FldExtendedPropertySet::FldExtendedPropertySet(IUnknown *pUnkOuter, ITypeLib *ALib,
    int iIndex) : CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("FldExtendedPropertySet::FldExtendedPropertySet-2\r\n", true);
#endif // DBGLEVEL
    m_lSinglePass = 0;
    m_lCache = 0;
    m_pField = NULL;
}

FldExtendedPropertySet::~FldExtendedPropertySet()
{
#if DBGLEVEL > 1
    WriteLogFile("FldExtendedPropertySet::~FldExtendedPropertySet\r\n", true);
#endif // DBGLEVEL
}

HRESULT FldExtendedPropertySet::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("FldExtendedPropertySet::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_ExtendedPropertySet))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG FldExtendedPropertySet::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "FldExtendedPropertySet::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "FldExtendedPropertySet::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT FldExtendedPropertySet::GetValue(BSTR PropertyName, VARIANT *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("FldExtendedPropertySet::GetValue\r\n  ", true);
    WriteLogFile("Name: ", false);
    WriteLogFile(PropertyName, false);
    WriteLogFile("\r\n", false);
    if(m_pField)
    {
        WriteLogFile("  Field Name: ", false);
        WriteLogFile(m_pField->GetNamePtr(), false);
        WriteLogFile("\r\n", false);
        WriteLogFile("  Field Table Name: ", false);
        WriteLogFile(m_pField->GetTblNamePtr(), false);
        WriteLogFile("\r\n", false);
    }
#endif // DBGLEVEL
    if(!Value) return(E_POINTER);
    if(wcsicmp(PropertyName, L"Single Pass") == 0)
    {
        VariantClear(Value);
        Value->vt = VT_I4;
        Value->lVal = m_lSinglePass;
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Cache") == 0)
    {
        VariantClear(Value);
        Value->vt = VT_I4;
        Value->lVal = m_lCache;
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Key") == 0)
    {
        VariantClear(Value);
        if(m_pField)
        {
            Value->vt = VT_BOOL;
            if(m_pField->IsKeyField()) Value->boolVal = VARIANT_TRUE;
            else Value->boolVal = VARIANT_FALSE;
            return(S_OK);
        }
    }
    /*else if(wcsicmp(PropertyName, L"Description") == 0)
    {
        Value->vt = VT_BSTR;
        Value->bstrVal = SysAllocString(L"Desc");
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"GeometryType") == 0)
    {
        if(m_pField)
        {
            Value->vt = VT_I4;
            Value->lVal = m_pField->GetGeomType();
            return(S_OK);
        }
    }
    else if(wcsicmp(PropertyName, L"PrimaryGeometryFlag") == 0)
    {
        if(m_pField)
        {
            Value->vt = VT_BOOL;
            Value->boolVal = VARIANT_FALSE;
            return(S_OK);
        }
    }
    else if(wcsicmp(PropertyName, L"GCoordSystemGUID") == 0)
    {
        if(m_pField)
        {
            return(m_pField->get_CoordSystemGUID(Value));
        }
    }
    else if(wcsicmp(PropertyName, L"Locatable") == 0)
    {
        Value->vt = VT_BOOL;
        Value->boolVal = VARIANT_TRUE;
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Type") == 0)
    {
        if(m_pField)
        {
            Value->vt = VT_I2;
            Value->iVal = m_pField->GetType();
            return(S_OK);
        }
    }
    else if(wcsicmp(PropertyName, L"Displayable") == 0)
    {
        Value->vt = VT_BOOL;
        Value->boolVal = VARIANT_TRUE;
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Format") == 0)
    {
        Value->vt = VT_BSTR;
        Value->bstrVal = SysAllocString(L"general");
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Precision") == 0)
    {
        if((m_pField->GetType() == 7) || m_pField->GetType() == 8))
        {
            Value->vt = VT_I4;
            Value->lVal = 8;
            return(S_OK);
        }
    }*/
    Value->vt = VT_NULL;
    //return(S_OK);
    return(E_NOTIMPL);
}

HRESULT FldExtendedPropertySet::SetValue(BSTR PropertyName, VARIANT Value, long *Status)
{
#if DBGLEVEL > 0
    WriteLogFile("FldExtendedPropertySet::SetValue\r\n  ", true);
    WriteLogFile("Name: ", false);
    WriteLogFile(PropertyName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(wcsicmp(PropertyName, L"Single Pass") == 0)
    {
        m_lSinglePass = VarToLong(Value);
        *Status = S_OK;
        return(S_OK);
    }
    else if(wcsicmp(PropertyName, L"Cache") == 0)
    {
        m_lCache = VarToLong(Value);
        *Status = S_OK;
        return(S_OK);
    }
    return(E_NOTIMPL);
}

void FldExtendedPropertySet::SetTDField(GTDField *pField)
{
    m_pField = pField;
    return;
}


//  GField

GField::GField(IUnknown *pUnkOuter, ITypeLib *ALib) : CCOMDispatch(pUnkOuter, ALib, 20)
{
#if DBGLEVEL > 1
    WriteLogFile("GField::GField-1\r\n", true);
#endif // DBGLEVEL
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_pExt = NULL;
    m_pLitConv = NULL;
    m_sNewIdCmd = NULL;
    m_sGenIdCmd = NULL;
    m_pvBuffer = NULL;
    InitFldAttrs(&m_cAttrs);
    VariantInit(&m_vNewVal);
    m_vNewVal.vt = VT_EMPTY;
    m_iSpecType = 0;
    m_iSpecField = 0;
    m_pSpecField = NULL;
    m_bIsMetatable = false;
}

GField::GField(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GField::GField-2\r\n", true);
#endif // DBGLEVEL
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_pExt = NULL;
    m_pLitConv = NULL;
    m_sNewIdCmd = NULL;
    m_sGenIdCmd = NULL;
    m_pvBuffer = NULL;
    InitFldAttrs(&m_cAttrs);
    VariantInit(&m_vNewVal);
    m_vNewVal.vt = VT_EMPTY;
    m_iSpecType = 0;
    m_iSpecField = 0;
    m_pSpecField = NULL;
    m_bIsMetatable = false;
}

GField::~GField()
{
#if DBGLEVEL > 1
    WriteLogFile("GField::~GField\r\n", true);
#endif // DBGLEVEL
    if(m_pSpecField) m_pSpecField->Release();
    VariantClear(&m_vNewVal);
    ClearFldAttrs(&m_cAttrs);
    if(m_sGenIdCmd) free(m_sGenIdCmd);
    if(m_sNewIdCmd) free(m_sNewIdCmd);
    if(m_pExt) m_pExt->SetOuter(NULL);
    if(m_pLitConv) m_pLitConv->SetOuter(NULL);
    if(m_wsName) free(m_wsName);
    if(m_sOrigName) free(m_sOrigName);
}

HRESULT GField::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GField::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GField))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG GField::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GField::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GField::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GField::get_Value(VARIANT *pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Value (", true);
    WriteLogFile(m_wsName, false);
    WriteLogFile(")\r\n", false);
    GFields *pFlds = (GFields*)m_pUnkOuter;
    GRecordset *pRS = pFlds->GetParentRecordset();
    WriteLogFile("    Recordset (", false);
    WriteLogFile(pRS->GetNamePtr(), false);
    WriteLogFile(")\r\n", false);
#endif // DBGLEVEL
    if(m_iSpecField == 1)
    {
        HRESULT hres = m_pSpecField->get_Value(pValue);
        if(pValue->vt > VT_NULL) return(hres);
    }

    if(!pValue) return(E_POINTER);
    //if(!m_pvBuffer) return(E_POINTER);

    // It looks like GeoMedia is able to pass an uninitialized VARIANT into
    // this method. When the vt is somehow od (e.g. 83 in our case),
    // the system does not know how to clear it and refuses to copy into it
    // So we try to clear it and if it fails, we simply set the vt to empty
    //VariantClear(pValue);
    pValue->vt = VT_EMPTY;
    // end of hack
    if(m_vNewVal.vt > VT_EMPTY) VariantCopy(pValue, &m_vNewVal);
    else if(m_pvBuffer) VariantCopy(pValue, m_pvBuffer);
/*WriteVariantToLogFile(L"  Value: ", *pValue);
char buf[32];
sprintf(buf, "  VType: %d\r\n", pValue->vt);
WriteLogFile(buf, false);*/
    return(S_OK);
}

HRESULT GField::set_Value(VARIANT pValue)
{
    ValidateVariant(&pValue);

#if DBGLEVEL > 0
    WriteLogFile("GField::set_Value\r\n", true);
    WriteLogFile("  Field name: ", false);
    WriteLogFile(m_wsName, false);
    WriteVariantToLogFile(L"\r\n  Value: ", pValue);
    char buf[32];
    sprintf(buf, "  VType: %d\r\n", pValue.vt);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(m_iSpecField > 0) m_pSpecField->set_Value(pValue);

    // hack for G/I Toolkit
    if((pValue.vt == VT_BSTR) && (m_cAttrs.iType < 8))
    {
        int iVal;
        float fltVal;

        switch(m_cAttrs.iType)
        {
        case 1:
            VariantClear(&m_vNewVal);
            m_vNewVal.vt = VT_BOOL;
            m_vNewVal.boolVal = (wcslen(pValue.bstrVal) > 0) &&
                ((pValue.bstrVal[0] == 'T') || (pValue.bstrVal[0] == 't'));
            return(S_OK);
        case 2:
            VariantClear(&m_vNewVal);
            if(swscanf(pValue.bstrVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_UI1;
                m_vNewVal.bVal = (BYTE)iVal;
            }
            return(S_OK);
        case 3:
            VariantClear(&m_vNewVal);
            if(swscanf(pValue.bstrVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_I2;
                m_vNewVal.iVal = iVal;
            }
            return(S_OK);
        case 4:
            VariantClear(&m_vNewVal);
            if(swscanf(pValue.bstrVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_I4;
                m_vNewVal.lVal = iVal;
            }
            return(S_OK);
        case 5:
            VariantClear(&m_vNewVal);
            m_vNewVal.vt = VT_CY;
            m_vNewVal.cyVal.int64 = CurrencyFromBSTR(pValue.bstrVal, m_pConnStruct->sDecSep);
            return(S_OK);
        case 6:
            VariantClear(&m_vNewVal);
            if(swscanf(pValue.bstrVal, L"%f", &fltVal) == 1)
            {
                m_vNewVal.vt = VT_R4;
                m_vNewVal.fltVal = fltVal;
            }
            return(S_OK);
        case 7:
            VariantClear(&m_vNewVal);
            if(swscanf(pValue.bstrVal, L"%f", &fltVal) == 1)
            {
                m_vNewVal.vt = VT_R8;
                m_vNewVal.dblVal = fltVal;
            }
            return(S_OK);
        }
    }
    // end of hack

    VariantCopy(&m_vNewVal, &pValue);
    return(S_OK);
}

HRESULT GField::get_AllowZeroLength(VARIANT_BOOL *pAllow)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_AllowZeroLength\r\n", true);
#endif // DBGLEVEL
    if(!pAllow) return(E_POINTER);
/*if((m_iSpecType == 2) && (wcsicmp(m_wsName, L"csguid") == 0))
{
    *pAllow = VARIANT_FALSE;
    return(S_OK);
}*/
    *pAllow = VARIANT_TRUE;
    return(S_OK);
}

HRESULT GField::set_AllowZeroLength(VARIANT_BOOL pAllow)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_AllowZeroLength\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_Attributes(long *pAttr)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Attributes\r\n", true);
#endif // DBGLEVEL
    if(!pAttr) return(E_POINTER);
    *pAttr = m_cAttrs.lAttr;
    return(S_OK);
}

HRESULT GField::set_Attributes(long pAttr)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_Attributes\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_CollatingOrder(long *pOrder)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_CollatingOrder\r\n", true);
#endif // DBGLEVEL
    if(!pOrder) return(E_POINTER);
    *pOrder = 0;
    return(S_OK);
}

HRESULT GField::get_CoordSystemGUID(VARIANT *CSGuid)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_CoordSystemGUID\r\n", true);
#endif // DBGLEVEL
    if(!CSGuid) return(E_POINTER);
    if((m_cAttrs.ulSrid == 0) && (!m_cAttrs.sCSGuid[0])) return(E_FAIL);

    VariantClear(CSGuid);

    if(m_cAttrs.sCSGuid[0])
    {
        CSGuid->vt = VT_BSTR;
        CSGuid->bstrVal = SysAllocString(m_cAttrs.sCSGuid);
    }
    else
    {
        VARIANT *pRow = FindCSTableRow(&m_pConnStruct->cCSTable,
            m_cAttrs.ulSrid);
        if(pRow) VariantCopy(CSGuid, &pRow[1]);
        else CSGuid->vt = VT_NULL;
    }
#if DBGLEVEL > 0
    WriteVariantToLogFile(L"CS GUID: ", *CSGuid);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT GField::set_CoordSystemGUID(VARIANT CSGuid)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_CoordSystemGUID\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_DataUpdatable(VARIANT_BOOL *pUpdatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_DataUpdatable\r\n", true);
#endif // DBGLEVEL
    if(!pUpdatable) return(E_POINTER);
    if(m_cAttrs.bUpdatable) *pUpdatable = VARIANT_TRUE;
    else *pUpdatable = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GField::set_DefaultValue(BSTR pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_DefaultValue\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_DefaultValue(BSTR *pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_DefaultValue\r\n", true);
#endif // DBGLEVEL
    if(!pValue) return(E_POINTER);
    //if(*pValue) SysFreeString(*pValue);
    *pValue = NULL;
    if(m_cAttrs.wsDefVal) *pValue = SysAllocString(m_cAttrs.wsDefVal);
    else *pValue = SysAllocString(L"");
    return(S_OK);
}

HRESULT GField::get_Name(BSTR *pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Name\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  FieldName: ", false);
    WriteLogFile(m_wsName, false);
    if(m_cAttrs.wsTblName)
    {
        WriteLogFile("\r\n  TableName: ", false);
        WriteLogFile(m_cAttrs.wsTblName, false);
    }
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    if(!pName) return(E_POINTER);
    //if(*pName) SysFreeString(*pName);

    // Hack for GCoordSystem table
    // at some point, GeoMedia requires that the GCoordSystem table fields
    // are in uppercase
    if(m_iSpecType == 2)
    {
        if(m_wsName)
        {
            wchar_t wsbuf[64];
            wcscpy(wsbuf, m_wsName);
            _wcsupr(wsbuf);
            *pName = SysAllocString(wsbuf);
        }
        else *pName = SysAllocString(L"");
        return(S_OK);
    }

    if(m_wsName) *pName = SysAllocString(m_wsName);
    else *pName = SysAllocString(L"");
    return(S_OK);
}

HRESULT GField::set_Name(BSTR pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_Name\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_Required(VARIANT_BOOL *pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Required\r\n", true);
#endif // DBGLEVEL
    if(!pRequired) return(E_POINTER);

    if(!m_cAttrs.wsTblName)
    {
        *pRequired = VARIANT_FALSE;
        return(S_OK);
    }

    if((m_iSpecType == 2) && (wcsicmp(m_wsName, L"csguid") == 0))
    {
        *pRequired = VARIANT_TRUE;
        return(S_OK);
    }

    if(m_cAttrs.bRequired) *pRequired = VARIANT_TRUE;
    else *pRequired = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GField::set_Required(VARIANT_BOOL pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_Required\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_Size(long *pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Size\r\n", true);
#endif // DBGLEVEL
    if(!pSize) return(E_POINTER);
    *pSize = m_cAttrs.lSize;
    return(S_OK);
}

HRESULT GField::set_Size(long pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_Size\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_SourceDatabase(BSTR *pDatabase)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_SourceDatabase\r\n", true);
#endif // DBGLEVEL
    if(!pDatabase) return(E_POINTER);
    //if(*pDatabase) SysFreeString(*pDatabase);
    *pDatabase = NULL;
    if(m_pConnStruct->wsServerName)
        *pDatabase = SysAllocString(m_pConnStruct->wsServerName);
    return(S_OK);
}

HRESULT GField::get_SourceField(BSTR *pField)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_SourceField\r\n", true);
#endif // DBGLEVEL
    if(!pField) return(E_POINTER);
    //if(*pField) SysFreeString(*pField);
    if(m_wsName) *pField = SysAllocString(m_wsName);
    else *pField = SysAllocString(L"");
    return(S_OK);
}

HRESULT GField::get_SourceTable(BSTR *pTable)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_SourceTable\r\n", true);
#endif // DBGLEVEL
    if(!pTable) return(E_POINTER);
    //if(*pTable) SysFreeString(*pTable);
    *pTable = NULL;
    if(m_cAttrs.wsTblName) *pTable = SysAllocString(m_cAttrs.wsTblName);
    return(S_OK);
}

HRESULT GField::get_SubType(long *pGeometryType)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_SubType\r\n", true);
#endif // DBGLEVEL
    if(!pGeometryType) return(E_POINTER);
    *pGeometryType = m_cAttrs.lSubType;
    return(S_OK);
}

HRESULT GField::set_SubType(long pGeometryType)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_SubType\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::get_Type(short *pType)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::get_Type\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  FieldName: ", false);
    WriteLogFile(m_wsName, false);
    if(m_cAttrs.wsTblName)
    {
        WriteLogFile("\r\n  TableName: ", false);
        WriteLogFile(m_cAttrs.wsTblName, false);
    }
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    char buf[32];
    sprintf(buf, "  Type: %d\r\n", m_cAttrs.iType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(!pType) return(E_POINTER);
    *pType = m_cAttrs.iType;
    return(S_OK);
}

HRESULT GField::set_Type(short pType)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::set_Type\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::AppendChunk(VARIANT Val)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::AppendChunk\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GField::FieldSize(long *pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::FieldSize\r\n", true);
#endif // DBGLEVEL
    if(!pSize) return(E_POINTER);
    if(!m_pvBuffer) return(S_FALSE);
    *pSize = VarGetSize(*m_pvBuffer);
    return(S_OK);
}

HRESULT GField::GetChunk(long Offset, long Bytes, VARIANT *pChunk)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::GetChunk\r\n", true);
#endif // DBGLEVEL
    if(!pChunk) return(E_POINTER);
    return(E_NOTIMPL);
}

HRESULT GField::GetExtension(BSTR Name, IDispatch * *ppGExtension)
{
#if DBGLEVEL > 0
    WriteLogFile("GField::GetExtension\r\n", true);
    WriteLogFile("  FieldName: ", false);
    WriteLogFile(m_wsName, false);
    if(m_cAttrs.wsTblName)
    {
        WriteLogFile("\r\n  TableName: ", false);
        WriteLogFile(m_cAttrs.wsTblName, false);
    }
    WriteLogFile("\r\n  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    //if(m_bIsMetatable && (m_iSpecType != 2)) return(E_NOTIMPL);
    if(m_bIsMetatable) return(E_NOTIMPL);

    if(wcsicmp(Name, L"DefaultValueLiteralConversion") == 0)
    {
        //return(E_NOTIMPL);
        if(!m_pLitConv)
        {
            m_pLitConv = new FldLitConvExt(this,
                ((ITypeLib**)m_pConnStruct->ppTypeLibs)[2]);
        }
        if(!ppGExtension) return(E_POINTER);
        if(*ppGExtension) (*ppGExtension)->Release();
        m_pLitConv->AddRef();
        *ppGExtension = m_pLitConv;
        return(S_OK);
    }
    if(wcsicmp(Name, L"ExtendedPropertySet") == 0)
    {
        if(!m_pExt) return(E_NOTIMPL);
        if(!ppGExtension) return(E_POINTER);
        if(*ppGExtension) (*ppGExtension)->Release();
        m_pExt->AddRef();
        *ppGExtension = m_pExt;
        return(S_OK);
    }
    return(E_NOTIMPL);
}

LPWSTR GField::GetNamePtr()
{
    return(m_wsName);
}

LPSTR GField::GetOrigNamePtr()
{
    return(m_sOrigName);
}

Oid GField::GetTableOid()
{
    return(m_cAttrs.lTblOid);
}

void GField::SetAttrs(PConnStruct pConnStruct, PGresult *pRes, int iPos,
    Oid lType, bool bUpdatable, int iSpecType, bool bMetatable)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iPos = iPos;
    m_iSpecType = iSpecType;
    m_bIsMetatable = bMetatable;

    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = NULL;
    if(m_wsName) free(m_wsName);
    m_wsName = NULL;

    LPSTR sVal = PQfname(pRes, iPos);
    if(sVal)
    {
        m_sOrigName = (LPSTR)malloc((strlen(sVal) + 1)*sizeof(char));
        strcpy(m_sOrigName, sVal);
        m_wsName = DBStrToWChar(sVal, m_pConnStruct->lDBEnc);
    }

    //if(!iSpecType)
    {
        m_pExt = new FldExtendedPropertySet(this,
            ((ITypeLib**)pConnStruct->ppTypeLibs)[1]);
    }

    m_cAttrs.lTblOid = PQftable(pRes, iPos);
    m_cAttrs.iTblPos = PQftablecol(pRes, iPos);
    m_cAttrs.bUpdatable = bUpdatable;

    int iMod = PQfmod(pRes, iPos);
    int iSize = PQfsize(pRes, iPos);

    m_cAttrs.lTypeOid = lType;
    m_cAttrs.iType = OidToGdoType(lType, iSize, iMod, &m_cAttrs.lSize);
#if DBGLEVEL > 1
    WriteLogFile("GField::SetAttrs\r\n", true);
    char sbuf[128];
    sprintf(sbuf, "  Name: %s, PG Type: %d, GDO Type: %d\r\n",
        m_sOrigName, lType, m_cAttrs.iType);
    WriteLogFile(sbuf, false);
#endif // DBGLEVEL
    return;
}

int GField::UpdateField(GTableDefs *pGTbls)
{
    GTableDef *ptd = pGTbls->FindByOid(m_cAttrs.lTblOid);
    GTDField *pfld = NULL;
    if(ptd)
    {
        ptd->BuildFromConnection();
        pfld = ptd->GetFieldPtrByTblPos(m_cAttrs.iTblPos);
    }

    if(pfld)
    {
        pfld->CopyAttributes(&m_cAttrs);
        // this was formerly intended for tables without key, however
        // but the oposite way seems to be usually used in GM
        //if(!ptd->GetHasUniqueIndex()) m_pExt->SetTDField(pfld);
        if(m_pExt && ptd->GetHasUniqueIndex()) m_pExt->SetTDField(pfld);
    }

    if(m_cAttrs.sDefValDef)
    {
        LPSTR sEnd = NULL;
        LPSTR sStart = strchr(m_cAttrs.sDefValDef, '\'');
        if(sStart)
        {
            sStart++;
            sEnd = strchr(sStart, '\'');
        }
        if(sEnd)
        {
            m_cAttrs.bUpdatable = false;
            m_cAttrs.lAttr &= ~32;
            m_cAttrs.lAttr |= 16;

            int ilen = sEnd - sStart;
            m_sNewIdCmd = (LPSTR)malloc((ilen + 38)*sizeof(char));
            strcpy(m_sNewIdCmd, "select last_value, is_called from ");
            int icurlen = strlen(m_sNewIdCmd);
            strncat(m_sNewIdCmd, sStart, ilen);
            m_sNewIdCmd[icurlen + ilen] = 0;
#if DBGLEVEL > 2
            WriteMallocReport(4, ilen + 37, strlen(m_sNewIdCmd));
#endif

            m_sGenIdCmd = (LPSTR)malloc((ilen + 22)*sizeof(char));
            strcpy(m_sGenIdCmd, "select nextval('");
            icurlen = strlen(m_sGenIdCmd);
            strncat(m_sGenIdCmd, sStart, ilen);
            m_sGenIdCmd[icurlen + ilen] = 0;
            strcat(m_sGenIdCmd, "')");
#if DBGLEVEL > 2
            WriteMallocReport(5, ilen + 21, strlen(m_sGenIdCmd));
#endif
        }
    }
    else if(m_cAttrs.wsDefVal)
    {
        if(m_cAttrs.iType == gdbGuid)
        {
            if(wcschr(m_cAttrs.wsDefVal, '(') != NULL)
            {
                m_cAttrs.lAttr &= ~32;
                m_cAttrs.bUpdatable = false;
            }
        }
    }

#if DBGLEVEL > 1
    WriteLogFile("GField::UpdateField\r\n", true);
    char sbuf[128];
    sprintf(sbuf, "  Name: %s, GDO Type: %d, SubType: %ld\r\n",
        m_sOrigName, m_cAttrs.iType, m_cAttrs.lSubType);
#if DBGLEVEL > 2
    WriteMallocReport(6, 127, strlen(sbuf));
#endif
    WriteLogFile(sbuf, false);
#endif // DBGLEVEL
    return(m_cAttrs.iGeomDim);
}

void GField::SetNewVal()
{
    VariantClear(&m_vNewVal);
    m_vNewVal.vt = VT_EMPTY;
    if(m_sNewIdCmd)
    {
        bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

        SetSP(m_pConnStruct->pConn, bTrans);
        //LPSTR lsCmd = GetSaveSQL(m_sNewIdCmd, bTrans);
        PGresult *res = PQexecParams(m_pConnStruct->pConn, m_sNewIdCmd, 0,
            NULL, NULL, NULL, NULL, 1);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Spot 1: ", m_pConnStruct->pConn);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
        }
        else
        {
            long nrows = PQntuples(res);
            if(nrows > 0)
            {
                Oid piTypes[2];
                VARIANT vVals[2];
                VariantInit(&vVals[0]);
                VariantInit(&vVals[1]);
                DescribeFieldTypes(res, 2, piTypes);
                GetRecordBin(res, 0, 2, piTypes, vVals,
                    m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, NULL, m_pConnStruct->iCurrDigits);
                m_vNewVal.vt = VT_I4;
                if(VarToBool(vVals[1]))
                    m_vNewVal.lVal = VarToLong(vVals[0]) + 1;
                else m_vNewVal.lVal = 1;
            }
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
        }
    }
    else if(m_cAttrs.wsDefVal)
    {
        int iVal;
        float fVal;
        SYSTEMTIME sTime;
        switch(m_cAttrs.iType)
        {
        case gdbBoolean:
            m_vNewVal.vt = VT_BOOL;
            m_vNewVal.boolVal = FALSE;
            if((m_cAttrs.wsDefVal[0] == 'T') || (m_cAttrs.wsDefVal[0] == 't'))
                m_vNewVal.boolVal = TRUE;
            break;
        case gdbByte:
            if(swscanf(m_cAttrs.wsDefVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_UI1;
                m_vNewVal.bVal = iVal;
            }
            break;
        case gdbInteger:
            if(swscanf(m_cAttrs.wsDefVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_I2;
                m_vNewVal.iVal = iVal;
            }
            break;
        case gdbLong:
            if(swscanf(m_cAttrs.wsDefVal, L"%d", &iVal) == 1)
            {
                m_vNewVal.vt = VT_I4;
                m_vNewVal.lVal = iVal;
            }
            break;
        case gdbSingle:
            if(swscanf(m_cAttrs.wsDefVal, L"%f", &fVal) == 1)
            {
                m_vNewVal.vt = VT_R4;
                m_vNewVal.fltVal = fVal;
            }
            break;
        case gdbDouble:
            if(swscanf(m_cAttrs.wsDefVal, L"%f", &fVal) == 1)
            {
                m_vNewVal.vt = VT_R8;
                m_vNewVal.dblVal = fVal;
            }
            break;
        case gdbDate:
            if(wcsicmp(m_cAttrs.wsDefVal, L"now()") == 0)
            {
                m_vNewVal.vt = VT_DATE;
                GetLocalTime(&sTime);
                SystemTimeToVariantTime(&sTime, &m_vNewVal.date);
            }
            break;
        default:
            if(wcschr(m_cAttrs.wsDefVal, '(') == NULL)
            {
                m_vNewVal.vt = VT_BSTR;
                m_vNewVal.bstrVal = SysAllocString(m_cAttrs.wsDefVal);
            }
            // else the default value is most likely a function, which result we don't know
        }
    }
    return;
}

void GField::SetBuffer(VARIANT *pBuffer)
{
    m_pvBuffer = pBuffer;
    return;
}

void GField::CancelNewVal()
{
    VariantClear(&m_vNewVal);
    m_vNewVal.vt = VT_EMPTY;
    return;
}

bool GField::Modified()
{
    return(!m_sNewIdCmd && (m_vNewVal.vt > VT_EMPTY));
}

bool GField::HasSeq()
{
    return(m_sNewIdCmd);
}

VARIANT GField::GetModVal()
{
    if(m_vNewVal.vt > VT_EMPTY) return(m_vNewVal);
    if(m_pvBuffer) return(*m_pvBuffer);
    return(m_vNewVal);
}

void GField::UpdateVal(VARIANT *pvVal)
{
    VariantClear(&m_vNewVal);
    m_vNewVal.vt = VT_EMPTY;
    m_pvBuffer = pvVal;
    return;
}

bool GField::GetRequired()
{
    return(m_cAttrs.bRequired);
}

Oid GField::GetTypeOid()
{
    return(m_cAttrs.lTypeOid);
}

LPSTR GField::GetSchemaNamePtr()
{
    return(m_cAttrs.sOrigSchema);
}

LPSTR GField::GetTableNamePtr()
{
    return(m_cAttrs.sOrigTable);
}

void GField::FillGeomInfo(PGeomInfo pgInfo)
{
    pgInfo->iGdoType = m_cAttrs.iType;
    pgInfo->lSubType = m_cAttrs.lSubType;
    pgInfo->iGeomDim = m_cAttrs.iGeomDim;
    pgInfo->ulSrid = m_cAttrs.ulSrid;
    pgInfo->iPsgType = m_cAttrs.iPsgType;
    return;
}

void GField::ChangeCSGuid(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    if(wcsicmp(m_cAttrs.sCSGuid, wsOldCSGuid) == 0)
        wcscpy(m_cAttrs.sCSGuid, wsNewCSGuid);
    return;
}

void GField::SetSpecField(GField *pSpecFld, int iSpecFld)
{
#if DBGLEVEL > 2
    char buf[64];
    WriteLogFile("GField::SetSpecField\r\n", true);
    WriteLogFile("  Field name: ", false);
    WriteLogFile(m_wsName, false);
    sprintf(buf, "\r\n  SpecType: %d\r\n", iSpecFld, false);
    WriteLogFile(buf, false);
#endif // DBGLEVEL

    m_iSpecField = iSpecFld; // 1 - has INGR field, 2 - has Native field;
    m_pSpecField = pSpecFld;
    m_pSpecField->AddRef();
    return;
}

void GField::SetSpecFldAttrs(unsigned long ulSrid, long iGDOGeomType,
    LPSTR sSchema, LPSTR sTable)
{
#if DBGLEVEL > 2
    char buf[64];
    WriteLogFile("GField::SetSpecFldAttrs\r\n", true);
    WriteLogFile("  Field name: ", false);
    WriteLogFile(m_wsName, false);
    sprintf(buf, "\r\n  GeomType: %d\r\n", iGDOGeomType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL

    m_cAttrs.sOrigSchema = sSchema;
    m_cAttrs.sOrigTable = sTable;
    m_cAttrs.iGeomDim = 2;
    m_cAttrs.ulSrid = ulSrid;
    if(iGDOGeomType == 4) // raster
    {
        m_cAttrs.iType = 32;
        m_cAttrs.lSubType = 2;
        m_cAttrs.iPsgType = 3;
    }
    else if(iGDOGeomType == 5) // text
    {
        m_cAttrs.iType = 32;
        m_cAttrs.lSubType = 10;
        m_cAttrs.iPsgType = 1;
    }
    else
    {
        m_cAttrs.iType = 11;
        m_cAttrs.lSubType = 0;
        m_cAttrs.iPsgType = 0;
        m_cAttrs.iGeomDim = 0;
    }
    return;
}

bool GField::IsThisField(LPSTR sSchema, LPSTR sTable, LPSTR sName)
{
    bool bRes = (strcmp(sTable, m_cAttrs.sOrigTable) == 0) &&
        (strcmp(sName, m_sOrigName) == 0);
    if(sSchema)
    {
        if(m_cAttrs.sOrigSchema)
            bRes &= (strcmp(sSchema, m_cAttrs.sOrigSchema) == 0);
        else bRes = false;
    }
    else bRes &= (m_cAttrs.sOrigSchema == NULL);
    return(bRes);
}


//  GFields

GFields::GFields(IUnknown *pUnkOuter, ITypeLib *ALib) :
    _IGDynaCollection(true, pUnkOuter, ALib, 23)
{
#if DBGLEVEL > 1
    WriteLogFile("GFields::GFields-1\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_pTblDefs = NULL;
    m_iCtidCol = -1;
    m_bIsMetatable = false;
    m_iSpecFields = 0;
    m_piListIndex = NULL;
}

GFields::GFields(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    _IGDynaCollection(true, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GFields::GFields-2\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_pTblDefs = NULL;
    m_iCtidCol = -1;
    m_bIsMetatable = false;
    m_iSpecFields = 0;
    m_piListIndex = NULL;
}

GFields::~GFields()
{
#if DBGLEVEL > 1
    WriteLogFile("GFields::~GFields\r\n", true);
#endif // DBGLEVEL
    if(m_piListIndex) free(m_piListIndex);
}

HRESULT GFields::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GFields::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGDynaCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GFields))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG GFields::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GFields::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GFields::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GFields::get_Count(short *Count)
{
#if DBGLEVEL > 0
    WriteLogFile("_IGCollection::get_Count\r\n", true);
#endif // DBGLEVEL
    if(!Count) return(E_POINTER);
    *Count = m_iDataLen - m_iSpecFields;
    return(S_OK);
}

HRESULT GFields::_NewEnum(IUnknown * *Enum)
{
#if DBGLEVEL > 0
    WriteLogFile("GFields::_NewEnum\r\n", true);
#endif // DBGLEVEL
    if(!Enum) return(E_POINTER);
    if(*Enum) (*Enum)->Release();
    *Enum = new CEnumGDO(0, m_iDataLen - m_iSpecFields, m_pData);
    (*Enum)->AddRef();
    return(S_OK);
}

HRESULT GFields::Append(IDispatch * Object)
{
#if DBGLEVEL > 0
    WriteLogFile("GFields::Append\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFields::Delete(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GFields::Delete\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFields::get_Item(VARIANT index, GField* *ppGField)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GFields::get_Item\r\n", true);
    WriteVariantToLogFile(L"  index: ", index);
#endif // DBGLEVEL
    if(!ppGField) return(E_POINTER);
    // a "get_CollectionItem" method is probably an exception from the COM
    // standards - it can accept uninitialized return values, similary as
    // QueryInterface method
    //if(*ppGField) (*ppGField)->Release();

    *ppGField = NULL;
    GField *pFld = NULL;
    if(index.vt == VT_BSTR) pFld = FindByWName(index.bstrVal);
    else if(index.vt == (VT_BYREF | VT_BSTR))
        pFld = FindByWName(*index.pbstrVal);
    else
    {
        int i = VarToInt(index);
        if(m_piListIndex) i = m_piListIndex[i];
        pFld = (GField*)_IGCollection::GetItem(i);
    }
    if(!pFld) return(ResultFromScode(S_FALSE));

    pFld->AddRef();
    *ppGField = pFld;
    return(S_OK);
}

void GFields::ClearAll()
{
    _IGCollection::ClearAll();
    return;
}

void GFields::BuildFromConn(PConnStruct pConnStruct, PGresult *pRes,
    GTableDefs *pGTbls, int iCtidCol, Oid *plTypes, int iSpecType,
    bool bIsMetatable)
{
    m_pConnStruct = pConnStruct;
    m_pTblDefs = pGTbls;
    m_iCtidCol = iCtidCol;
    m_bIsMetatable = bIsMetatable;

    GField *pFld;
    int ncols = PQnfields(pRes);

    for(int i = 0; i < ncols; i++)
    {
        if(i != m_iCtidCol)
        {
            pFld = new GField(NULL, m_pTypeLib);
            pFld->SetAttrs(pConnStruct, pRes, i, plTypes[i], m_iCtidCol > -1,
                iSpecType, bIsMetatable);
            _IGCollection::AddItem(pFld);
        }
    }
    return;
}

GField* FindBaseField(GField *pSpecFld, int iNameLen, int iSpecPos, int iFields,
    GField **ppFields)
{
    bool bFound = false;
    int i = 0;
    GField *pFld = NULL;
    LPSTR sName = pSpecFld->GetOrigNamePtr();
    while(!bFound && (i < iFields))
    {
        if(i != iSpecPos)
        {
            pFld = ppFields[i++];
            bFound = (strncmp(sName, pFld->GetOrigNamePtr(), iNameLen) == 0);
        }
        else i++;
    }
    return(bFound ? pFld : NULL);
}

void GFields::UpdateFields(int *piGeomDims)
{
    bool bBuildIndex = false;

    CGeomInfo gInfo;
    GField *pFld;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pFld = (GField*)m_pData[i];
        piGeomDims[i] = pFld->UpdateField(m_pTblDefs);
        pFld->FillGeomInfo(&gInfo);
        bBuildIndex |= (gInfo.iGdoType > 31);
    }

    if(bBuildIndex)
    {
        m_piListIndex = (int*)malloc(m_iDataLen*sizeof(int));
        int i = 0;
        int j = 0;
        int iSpecFld, iLen;
        LPSTR sName;
        GField *pBaseFld;
        while(i < m_iDataLen)
        {
            pBaseFld = NULL;
            iSpecFld = 0;
            m_piListIndex[i] = -1;
            m_piListIndex[j] = i;
            pFld = (GField*)m_pData[i++];
            sName = pFld->GetOrigNamePtr();
            iLen = strlen(sName);
            if(iLen > 4)
            {
                iLen -= 4;
                if(strcmp(&sName[iLen], "_igr") == 0) iSpecFld = 1;
                else if(strcmp(&sName[iLen], "_nat") == 0) iSpecFld = 2;
            }
            if(iSpecFld > 0)
                pBaseFld = FindBaseField(pFld, iLen, i - 1, m_iDataLen,
                    (GField**)m_pData);
            if(pBaseFld)
            {
                if(iSpecFld == 1) piGeomDims[i - 1] = 0;
                else piGeomDims[i - 1] = 2;
                pBaseFld->SetSpecField(pFld, iSpecFld);
                pBaseFld->FillGeomInfo(&gInfo);
                pFld->SetSpecFldAttrs(gInfo.ulSrid, gInfo.lSubType,
                    pBaseFld->GetSchemaNamePtr(), pBaseFld->GetTableNamePtr());
                m_iSpecFields++;
            }
            else j++;
        }
    }
    return;
}

/*GField* GFields::FindKeyField()
{
    GField *pFld;
    bool bFound = false;
    int i = 0;
    while(!bFound && (i < GetCount()))
    {
        pFld = (GField*)GetItem(i++);
        bFound = pFld->IsKey();
    }
    return(bFound ? pFld : NULL);
}*/

/*GField* GFields::FindByName(LPSTR sVal)
{
    bool bFound = false;
    GField *pFld = NULL;
    int i = 0;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pFld = (GField*)m_pData[i++];
        bFound = (stricmp(pFld->GetNamePtr(), sVal) == 0);
    }
    return(bFound ? pFld : NULL);
}*/

GField* GFields::FindByWName(BSTR bsVal)
{
    bool bFound = false;
    GField *pFld = NULL;
    int i = 0;
    if(m_bIsMetatable)
    {
        while(!bFound && (i < m_iDataLen))
        {
            pFld = (GField*)m_pData[i++];
            bFound = (wcsicmp(pFld->GetNamePtr(), bsVal) == 0);
        }
    }
    else
    {
        while(!bFound && (i < m_iDataLen))
        {
            pFld = (GField*)m_pData[i++];
            bFound = (wcscmp(pFld->GetNamePtr(), bsVal) == 0);
        }
    }
    return(bFound ? pFld : NULL);
}

void GFields::SetNewRow()
{
    GField *pFld;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pFld = (GField*)GetItem(i);
        pFld->SetNewVal();
    }
    return;
}

void GFields::SetBuffer(VARIANT *pBuffer)
{
    GField *pFld;
    if(pBuffer)
    {
        for(int i = 0; i < m_iDataLen; i++)
        {
            pFld = (GField*)GetItem(i);
            pFld->SetBuffer(&pBuffer[i]);
        }
    }
    else
    {
        for(int i = 0; i < m_iDataLen; i++)
        {
            pFld = (GField*)GetItem(i);
            pFld->SetBuffer(NULL);
        }
    }
    return;
}

void GFields::CancelNewRow()
{
    GField *pFld;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pFld = (GField*)GetItem(i);
        pFld->CancelNewVal();
    }
    return;
}

void GFields::UpdateRow(VARIANT *pBuffer)
{
    GField *pFld;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pFld = (GField*)GetItem(i);
        pFld->UpdateVal(&pBuffer[i]);
    }
    return;
}

GTableDefs* GFields::GetTableDefsPtr()
{
    return(m_pTblDefs);
}

void GFields::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    GField *pFld;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pFld = (GField*)GetItem(i);
        pFld->ChangeCSGuid(wsOldCSGuid, wsNewCSGuid);
    }
    return;
}

int GFields::HasField(LPSTR sSchema, LPSTR sTable, LPSTR sName)
{
    GField *pFld;
    bool bFound = false;
    int i = 0;
    while(!bFound && (i < m_iDataLen))
    {
        pFld = (GField*)GetItem(i++);
        bFound = pFld->IsThisField(sSchema, sTable, sName);
    }
    return(bFound ? i - 1 : -1);
}

#if DBGLEVEL > 0
GRecordset* GFields::GetParentRecordset()
{
    return (GRecordset*)m_pUnkOuter;
}
#endif


// GFeatureClass

GFeatureClassExt::GFeatureClassExt(IUnknown *pUnkOuter, ITypeLib *ALib) :
    CCOMDispatch(pUnkOuter, ALib, 4)
{
#if DBGLEVEL > 2
    WriteLogFile("GFeatureClassExt::GFeatureClassExt-1\r\n", true);
#endif // DBGLEVEL
    m_wsTblName = NULL;
}

GFeatureClassExt::GFeatureClassExt(IUnknown *pUnkOuter, ITypeLib *ALib,
    int iIndex) : CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 2
    WriteLogFile("GFeatureClassExt::GFeatureClassExt-2\r\n", true);
#endif // DBGLEVEL
    m_wsTblName = NULL;
}

GFeatureClassExt::~GFeatureClassExt()
{
#if DBGLEVEL > 2
    WriteLogFile("GFeatureClassExt::~GFeatureClassExt\r\n", true);
#endif // DBGLEVEL
    if(m_wsTblName) free(m_wsTblName);
}

HRESULT GFeatureClassExt::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GFeatureClassExt::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID__GFeatureClass))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG GFeatureClassExt::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GFeatureClassExt::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GFeatureClassExt::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GFeatureClassExt::get_AssociationRoleClasses(
    _GAssociationRoleClasses* *objClasses)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_AssociationRoleClasses\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::get_AttributeClasses(_GAttributeClasses* *objClasses)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_AttributeClasses\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::get_KeyAttributeNames(VARIANT *pvarKeyAttributeNames)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_KeyAttributeNames\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::get_Name(BSTR *strVal)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_Name\r\n", true);
#endif // DBGLEVEL
    if(!strVal) return(E_POINTER);
    if(!m_wsTblName) return(E_NOTIMPL);
    *strVal = SysAllocString(m_wsTblName);
    return(S_OK);
}

HRESULT GFeatureClassExt::get_OperationClasses(_GOperationClasses* *objClasses)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_OperationClasses\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::get_Parent(IDispatch * *objParent)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_Parent\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::get_RuleClasses(_GRuleClasses* *objClasses)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::get_RuleClasses\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::GetFeature(GRecordset* objSelection,
    _GFeature* *objIFeature)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::GetFeature\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GFeatureClassExt::OpenSelection(BSTR bstrFilter, VARIANT vType,
    VARIANT vOptions, VARIANT vSpatialOperator, VARIANT vSpatialGeometryFilter,
    VARIANT vGeometryFieldName, GRecordset* *ppSelection)
{
#if DBGLEVEL > 1
    WriteLogFile("GFeatureClassExt::OpenSelection\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

void GFeatureClassExt::SetTableName(LPWSTR wsTblName)
{
    if(m_wsTblName) free(m_wsTblName);
    int ilen = wcslen(wsTblName);
    m_wsTblName = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
    wcscpy(m_wsTblName, wsTblName);
    return;
}


//  GRecordset

GRecordset::GRecordset(GRecordsets *pParent, ITypeLib *ALib) : CCOMDispatch(NULL, ALib, 26)
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordset::GRecordset-1\r\n", true);
#endif // DBGLEVEL
m_lRefCount++;
    m_pParent = pParent;
    m_sName[0] = 0;
    VariantInit(&m_vType);
    VariantInit(&m_voptions);
    VariantInit(&m_vSpatialOp);
    VariantInit(&m_vSpatialGeometryFilter);
    VariantInit(&m_vGeometryFieldName);
    m_pFields = new GFields(this, m_pTypeLib);
    m_pConnStruct = NULL;
    m_iOpenStatus = 0;
    m_iSpecType = 0;
    m_iEditMode = 0;
    m_sSelect[0] = 0;
    m_plTypes = NULL;
    m_vBuffer.iCtidLen = 0;
    m_lBufSize = 0;
    m_lRecordCount = 0;
    m_lCurPos = -1;
    m_lModPos = 1;
    m_iCtidCol = -1;
    m_pPGres = NULL;
    m_bBOF = false;
    m_bEOF = false;
    m_pTblDef = NULL;
    m_lTblOid = 0;
    m_piGeomDims = NULL;
    m_pFCExt = NULL;
    m_lInserted = 0;
}

GRecordset::GRecordset(GRecordsets *pParent, ITypeLib *ALib, int iIndex) :
    CCOMDispatch(NULL, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordset::GRecordset-2\r\n", true);
#endif // DBGLEVEL
m_lRefCount++;
    m_pParent = pParent;
    m_sName[0] = 0;
    VariantInit(&m_vType);
    VariantInit(&m_voptions);
    VariantInit(&m_vSpatialOp);
    VariantInit(&m_vSpatialGeometryFilter);
    VariantInit(&m_vGeometryFieldName);
    m_pFields = new GFields(this, m_pTypeLib);
    m_iOpenStatus = 0;
    m_pConnStruct = NULL;
    m_iSpecType = 0;
    m_iEditMode = 0;
    m_sSelect[0] = 0;
    m_plTypes = NULL;
    m_vBuffer.iCtidLen = 0;
    m_lBufSize = 0;
    m_lRecordCount = 0;
    m_lCurPos = -1;
    m_lModPos = -1;
    m_iCtidCol = -1;
    m_pPGres = NULL;
    m_bBOF = false;
    m_bEOF = false;
    m_pTblDef = NULL;
    m_lTblOid = 0;
    m_piGeomDims = NULL;
    m_pFCExt = NULL;
    m_lInserted = 0;
}

GRecordset::~GRecordset()
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordset::~GRecordset\r\n  ", true);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(m_iOpenStatus > 0)
    {
        bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
        char sCmd[64];
        sprintf(sCmd, "deallocate %s", m_sStmtName);

        SetSP(m_pConnStruct->pConn, bTrans);
        PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
        }

        for(int i = 0; i < m_iParams; i++)
        {
            if(m_psParams[i]) free(m_psParams[i]);
        }
        free(m_psParams);
        free(m_piParType);
        free(m_piParLenght);

        PQclear(m_pPGres);

        for(int i = 0; i < m_iRowSize; i++)
        {
            VariantClear(&m_vBuffer.pvData[i]);
        }
        free(m_vBuffer.pvData);
    }

    if(m_pFCExt) m_pFCExt->SetOuter(NULL);

    m_pTblDef = NULL;
    if(m_piGeomDims) free(m_piGeomDims);
    if(m_plTypes) free(m_plTypes);
    m_pFields->SetOuter(NULL);
    VariantClear(&m_vType);
    VariantClear(&m_voptions);
    VariantClear(&m_vSpatialOp);
    VariantClear(&m_vSpatialGeometryFilter);
    VariantClear(&m_vGeometryFieldName);

    if(m_pParent) m_pParent->DeletePtr(this);
}

HRESULT GRecordset::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GRecordset::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GRecordset))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG GRecordset::Release()
{
    // GRecordset is never aggregated
    ULONG lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "GRecordset::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(lRest > 0) return(lRest);
    delete(this);
    return(0);
}

HRESULT GRecordset::get_GFields(GFields* *ppGFields)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_GFields\r\n", true);
#endif // DBGLEVEL
    if(!ppGFields) return(E_POINTER);
    if(*ppGFields) (*ppGFields)->Release();
    m_pFields->AddRef();
    *ppGFields = m_pFields;
    return(S_OK);
}

HRESULT GRecordset::get_BOF(VARIANT_BOOL *pb)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_BOF\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(!pb) return(E_POINTER);
    if(m_bBOF) *pb = VARIANT_TRUE;
    else *pb = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GRecordset::get_Bookmark(SAFEARRAY **ppsach)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_Bookmark\r\n", true);
#if DBGLEVEL > 2
        WriteLogFile("  ", false);
        WriteLogFile(m_sName, false);
        WriteLogFile("\r\n", false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    if(!ppsach) return(E_POINTER);
    if(!m_pPGres) return(S_FALSE);
    // we also know situations when this throws an exception
    //if(*ppsach) SafeArrayDestroy(*ppsach);

    SAFEARRAYBOUND sab = {4, 0};
    *ppsach = SafeArrayCreate(VT_UI1, 1, &sab);
    long *plng = NULL;
    SafeArrayAccessData(*ppsach, (void**)&plng);
    *plng = m_lCurPos + 1;
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "get_Bookmark - %d\r\n", *plng);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    SafeArrayUnaccessData(*ppsach);
    return(S_OK);
}

HRESULT GRecordset::set_Bookmark(SAFEARRAY **ppsach)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::set_Bookmark\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    if(!m_pPGres)
    {
#if DBGLEVEL > 2
        WriteLogFile("Invalid recordset buffer\r\n", true);
#endif // DBGLEVEL
        return(S_FALSE);
    }

    long lNewPos;

    long *plng = NULL;
    SafeArrayAccessData(*ppsach, (void**)&plng);
    lNewPos = *plng;
    SafeArrayUnaccessData(*ppsach);
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "set_Bookmark - %d\r\n", lNewPos);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(lNewPos > 0)
    {
        m_lCurPos = lNewPos - 1;
        SyncBuf();
        m_pFields->SetBuffer(m_vBuffer.pvData);
        return(S_OK);
    }

    m_lCurPos = -1;
    m_pFields->SetBuffer(NULL);
    return(S_FALSE);
}

HRESULT GRecordset::get_Bookmarkable(VARIANT_BOOL *pb)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_Bookmarkable\r\n", true);
#endif // DBGLEVEL
    if(!pb) return(E_POINTER);
    *pb = VARIANT_TRUE;
    return(S_OK);
}

HRESULT GRecordset::get_EditMode(short *pi)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_EditMode\r\n", true);
#endif // DBGLEVEL
    if(!pi) return(E_POINTER);
    *pi = m_iEditMode;
    return(S_OK);
}

HRESULT GRecordset::get_EOF(VARIANT_BOOL *pb)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_EOF\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(!pb) return(E_POINTER);
    if(m_bEOF) *pb = VARIANT_TRUE;
    else *pb = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GRecordset::get_LastModified(SAFEARRAY * *LastModified)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_LastModified\r\n", true);
#endif // DBGLEVEL
    if(!LastModified) return(E_POINTER);
    if(!m_pPGres) return(S_FALSE);
    if(m_lModPos < 0) return(S_FALSE);
    if(*LastModified) SafeArrayDestroy(*LastModified);

    SAFEARRAYBOUND sab = {4, 0};
    *LastModified = SafeArrayCreate(VT_UI1, 1, &sab);
    long *plng = NULL;
    SafeArrayAccessData(*LastModified, (void**)&plng);
    *plng = m_lModPos + 1;
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "get_LastModified - %d\r\n", *plng);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    SafeArrayUnaccessData(*LastModified);
    return(S_OK);
}

HRESULT GRecordset::get_Name(BSTR *Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_Name\r\n  ", true);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(!Name) return(E_POINTER);
    if(*Name) SysFreeString(*Name);
    *Name = SysAllocString(m_sName);
    return(S_OK);
}

HRESULT GRecordset::get_RecordCount(long *RecordCount)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_RecordCount\r\n", true);
#endif // DBGLEVEL
    if(!RecordCount) return(E_POINTER);
    if(!m_pPGres) return(E_NOTIMPL);
    *RecordCount = m_lRecordCount;
    return(S_OK);
}

HRESULT GRecordset::get_Type(short *Type)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_Type\r\n", true);
#endif // DBGLEVEL
    if(!Type) return(E_POINTER);
    *Type = VarToInt(m_vType);
    return(S_OK);
}

HRESULT GRecordset::get_Updatable(VARIANT_BOOL *Updatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::get_Updatable\r\n", true);
#endif // DBGLEVEL
    if(!Updatable) return(E_POINTER);
    if(m_bUpdatable) *Updatable = VARIANT_TRUE;
    else *Updatable = VARIANT_FALSE;
    return(S_OK);
}

bool GRecordset::IsCurDeleted()
{
    if(m_iCtidCol < 0) return false;
    if(m_lCurPos < 0) return true;
    if(m_vBuffer.iCtidLen < 1) return true;
    return false;
}

HRESULT GRecordset::AddNew()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::AddNew\r\n", true);
    if(m_iSpecType == 2) WriteLogFile("  GCoordSystem\r\n", false);
#if DBGLEVEL > 2
    else
    {
        WriteLogFile("  ", false);
        WriteLogFile(m_sName, false);
        WriteLogFile("\r\n", false);
    }
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(!m_pPGres) return(S_FALSE);
    if(!m_bUpdatable) return(S_FALSE);
    m_pFields->SetNewRow();
    m_iEditMode = 2;
    return(S_OK);
}

HRESULT GRecordset::CancelUpdate()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::CancelUpdate\r\n", true);
    if(m_iSpecType == 2) WriteLogFile("  GCoordSystem\r\n", false);
#endif // DBGLEVEL
    if(!m_bUpdatable) return(S_FALSE);
    m_pFields->CancelNewRow();
    return(S_OK);
}

HRESULT GRecordset::Close()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::Close\r\n  (", true);
    WriteLogFile(m_sName, false);
    WriteLogFile(")\r\n", false);
#endif // DBGLEVEL
    if(!m_pPGres) return(E_NOTIMPL);

    if(m_iOpenStatus > 0)
    {
        bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
        char sCmd[64];
        sprintf(sCmd, "deallocate %s", m_sStmtName);

        SetSP(m_pConnStruct->pConn, bTrans);
        PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
        }

        for(int i = 0; i < m_iParams; i++)
        {
            if(m_psParams[i]) free(m_psParams[i]);
        }
        free(m_psParams);
        free(m_piParType);
        free(m_piParLenght);

        PQclear(m_pPGres);

        for(int i = 0; i < m_iRowSize; i++)
        {
            VariantClear(&m_vBuffer.pvData[i]);
        }
        free(m_vBuffer.pvData);
    }

    m_pPGres = NULL;
    m_iOpenStatus = 0;
    m_lInserted = 0;
    return(S_OK);
}

long WriteModLog(PConnStruct pConnStruct, Oid lTblOid, GFields *pFields,
    int iMode)
{
    long lRes = -1;

    GTableDefs *pTDs = pFields->GetTableDefsPtr();
    GTableDef *pTD = pTDs->FindByOid(lTblOid);
    if(!pTD) return(lRes);

    pTD->BuildFromConnection();
    int nKeys = pTD->GetKeyCount();
    if(nKeys < 1) return(lRes);

    GTDFields *ptdFlds = pTD->GetFieldsPtr();
    int nFlds = ptdFlds->GetCount();
    GTDField *ptdFld;
    GField *pFld;

    LPSTR sTblName = WCharToDBStr(pTD->GetNamePtr(), pConnStruct->lDBEnc);

    int ilen = 52 + strlen(pConnStruct->sModTables) + strlen(sTblName);
    LPSTR sSql = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sSql, "select modifiedtableid from %s where tablename = '%s'",
        pConnStruct->sModTables, sTblName);
#if DBGLEVEL > 2
    WriteMallocReport(7, ilen - 1, strlen(sSql));
#endif

    bool bTrans = (pConnStruct->iSysTrans | pConnStruct->iGdoTrans);

    SetSP(pConnStruct->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sSql, bTrans);
    PGresult *res = PQexec(pConnStruct->pConn, sSql);
    //free(lsCmd);
    free(sSql);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 2: ", pConnStruct->pConn);
        PQclear(res);
        RollbackSP(pConnStruct->pConn, bTrans);
        return(lRes);
    }

    long nrows = PQntuples(res);
    char sBuf[32];
    LPSTR sParams[13];
    int i, j;
    for(i = 0; i < 13; i++) sParams[i] = NULL;

    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pConnStruct->pConn, bTrans);

        ilen = 64 + strlen(pConnStruct->sModTables) + nKeys*18;
        sSql = (LPSTR)malloc(ilen*sizeof(char));

        sprintf(sSql, "insert into %s(tablename", pConnStruct->sModTables);
        for(i = 0; i < nKeys; i++)
        {
            sprintf(sBuf, ", keyvalue%d", i + 1);
            strcat(sSql, sBuf);
        }
        strcat(sSql, ") values ($1");
        for(i = 0; i < nKeys; i++)
        {
            sprintf(sBuf, ", $%d", i + 2);
            strcat(sSql, sBuf);
        }
        strcat(sSql, ") returning modifiedtableid");
#if DBGLEVEL > 2
        WriteMallocReport(8, ilen - 1, strlen(sSql));
#endif

        sParams[0] = sTblName;

        i = 0;
        j = 1;
        while((i < nFlds) && (j < nKeys + 1))
        {
            ptdFld = (GTDField*)ptdFlds->GetItem(i++);
            if(ptdFld->IsKeyField())
            {
                sParams[j++] = WCharToDBStr(ptdFld->GetNamePtr(),
                    pConnStruct->lDBEnc);
            }
        }

        SetSP(pConnStruct->pConn, bTrans);
        res = PQexecParams(pConnStruct->pConn, sSql, nKeys + 1, NULL, sParams,
            NULL, NULL, 0);
        free(sSql);

        for(i = 1; i < 13; i++)
        {
            if(sParams[i]) free(sParams[i]);
            sParams[i] = NULL;
        }

        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Spot 3: ", pConnStruct->pConn);
            PQclear(res);
            RollbackSP(pConnStruct->pConn, bTrans);
            return(lRes);
        }
        else nrows = PQntuples(res);
    }
    else free(sTblName);

    sParams[0] = NULL;

    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pConnStruct->pConn, bTrans);
        return(lRes);
    }

    Oid lType;
    VARIANT vTblId;
    VariantInit(&vTblId);
    DescribeFieldTypes(res, 1, &lType);
    GetRecord(res, 0, 1, &lType, &vTblId, pConnStruct->sDecSep);
    PQclear(res);
    ReleaseSP(pConnStruct->pConn, bTrans);

    ilen = 100 + strlen(pConnStruct->sModLog) + nKeys*18;
    sSql = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sSql, "insert into %s(type, modifiedtableid", pConnStruct->sModLog);
    for(int i = 0; i < nKeys; i++)
    {
        sprintf(sBuf, ", keyvalue%d", i + 1);
        strcat(sSql, sBuf);
    }
    strcat(sSql, ", sessionid) values ($1, $2");
    for(int i = 0; i < nKeys; i++)
    {
        sprintf(sBuf, ", $%d", i + 3);
        strcat(sSql, sBuf);
    }
    sprintf(sBuf, ", $%d)", nKeys + 3);
    strcat(sSql, sBuf);
    strcat(sSql, " returning modificationnumber");
#if DBGLEVEL > 0
    WriteLogFile("Mod log sql: ", true);
    WriteLogFile(sSql, false);
    WriteLogFile("\r\n", false);
    WriteMallocReport(9, ilen - 1, strlen(sSql));
#endif

    int piLen[13];
    int piTypes[13];

    CGeomInfo cgInfo = {0, 0, 0, 0, 0};
    long lParams[8];

    VARIANT V;

    V.vt = VT_I2;
    V.iVal = iMode;
    piLen[0] = VarToBinaryLen(V, INT2OID, pConnStruct->lDBEnc, &cgInfo, lParams);
    sParams[0] = (LPSTR)malloc((piLen[0] + 1)*sizeof(char));
    VarToBinaryBuf(V, INT2OID, pConnStruct->lDBEnc, sParams[0], piLen[0],
        &cgInfo, lParams, pConnStruct->iCurrDigits);
    piTypes[0] = 1;

    piLen[1] = VarToBinaryLen(vTblId, INT4OID, pConnStruct->lDBEnc, &cgInfo,
        lParams);
    sParams[1] = (LPSTR)malloc((piLen[1] + 1)*sizeof(char));
    VarToBinaryBuf(vTblId, INT4OID, pConnStruct->lDBEnc, sParams[1], piLen[1],
        &cgInfo, lParams, pConnStruct->iCurrDigits);
    piTypes[1] = 1;

    VARIANT vVal;
    VariantInit(&vVal);

    i = 0;
    j = 2;
    while((i < nFlds) && (j < nKeys + 2))
    {
        ptdFld = (GTDField*)ptdFlds->GetItem(i++);
        if(ptdFld->IsKeyField())
        {
            pFld = pFields->FindByWName(ptdFld->GetNamePtr());
            if(pFld)
            {
                pFld->get_Value(&vVal);
                V.vt = VT_BSTR;
                V.bstrVal = VarToBSTR(vVal);
            }
            else V.vt = VT_NULL;

            piLen[j] = VarToBinaryLen(V, VARCHAROID, pConnStruct->lDBEnc,
                &cgInfo, lParams);
            sParams[j] = (LPSTR)malloc((piLen[j] + 1)*sizeof(char));
            VarToBinaryBuf(V, VARCHAROID, pConnStruct->lDBEnc, sParams[j],
                piLen[j], &cgInfo, lParams, pConnStruct->iCurrDigits);
            piTypes[j++] = 1;

            VariantClear(&V);
        }
    }

    VariantClear(&V);
    V.vt = VT_I4;
    V.lVal = pConnStruct->lSessionId;
    piLen[nKeys + 2] = VarToBinaryLen(V, INT4OID, pConnStruct->lDBEnc, &cgInfo,
        lParams);
    sParams[nKeys + 2] = (LPSTR)malloc((piLen[nKeys + 2] + 1)*sizeof(char));
    VarToBinaryBuf(V, INT4OID, pConnStruct->lDBEnc, sParams[nKeys + 2],
        piLen[nKeys + 2], &cgInfo, lParams, pConnStruct->iCurrDigits);
    piTypes[nKeys + 2] = 1;

    SetSP(pConnStruct->pConn, bTrans);
    res = PQexecParams(pConnStruct->pConn, sSql, nKeys + 3, NULL, sParams,
        piLen, piTypes, 0);
    free(sSql);

    for(i = 0; i < 13; i++)
    {
        if(sParams[i]) free(sParams[i]);
    }

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 4: ", pConnStruct->pConn);
        PQclear(res);
        RollbackSP(pConnStruct->pConn, bTrans);
        return(lRes);
    }

    nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pConnStruct->pConn, bTrans);
        return(lRes);
    }

    DescribeFieldTypes(res, 1, &lType);
    GetRecord(res, 0, 1, &lType, &V, pConnStruct->sDecSep);
    PQclear(res);
    ReleaseSP(pConnStruct->pConn, bTrans);

    lRes = VarToLong(V);
    return(lRes);
}

void DelFromModLog(long lModId, PConnStruct pConnStruct)
{
    char buf[128];
    sprintf(buf, "delete from %s where modificationnumber = %ld",
        pConnStruct->sModLog, lModId);
    ExecuteCommand(pConnStruct, buf);
    return;
}

HRESULT GRecordset::Delete()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::Delete\r\n", true);
    if(m_iSpecType == 2) WriteLogFile("  GCoordSystem\r\n", false);
#endif // DBGLEVEL
    if(!m_bUpdatable) return(S_FALSE);
    if(m_pFields->GetCount() < 1) return(S_FALSE);
    if(IsCurDeleted()) return S_FALSE;
    if(m_lCurPos < 0) return S_FALSE;

    int ilen = 36 + GetTableNameLen(m_pTblDef->GetOrigSchemaPtr(),
        m_pTblDef->GetOrigNamePtr());
    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    strcpy(sCmd, "delete from ");
    CatTableName(sCmd, m_pTblDef->GetOrigSchemaPtr(),
        m_pTblDef->GetOrigNamePtr(), !m_bIsMetatable);
    strcat(sCmd, " where ctid = $1");

    int iParType = 1;
    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
    SetSP(m_pConnStruct->pConn, bTrans);
    LPSTR sParams[1] = {m_vBuffer.sCtid};
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexecParams(m_pConnStruct->pConn, sCmd, 1, NULL,
        sParams, &m_vBuffer.iCtidLen, &iParType, 0);
    //free(lsCmd);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        m_pErrors->HandlePQError(m_pConnStruct->pConn, m_pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return(S_FALSE);
    }
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);
    GDatabase *pDB = (GDatabase*)m_pConnStruct->lSessionId;
    if(pDB->GetModificationLogging())
        WriteModLog(m_pConnStruct, m_lTblOid, m_pFields, 3);

    //pDB->BroadcastDelRecord(m_sTblName, pRow->iCtidLen, pRow->pCtid);
    m_vBuffer.iCtidLen = 0;
    PQsetvalue(m_pPGres, m_lCurPos, m_iCtidCol, NULL, -1);
    m_pFields->SetBuffer(NULL);

    m_lRecordCount--;
    if(m_lRecordCount < 1)
    {
        m_lCurPos = -1;
        m_bBOF = true;
        m_bEOF = true;
    }

    m_lModPos = m_lCurPos;
    return(S_OK);
}

HRESULT GRecordset::Edit()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::Edit\r\n", true);
    if(m_iSpecType == 2) WriteLogFile("  GCoordSystem\r\n", false);
#if DBGLEVEL > 2
    else
    {
        WriteLogFile("  ", false);
        WriteLogFile(m_sName, false);
        WriteLogFile("\r\n", false);
    }
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(!m_bUpdatable) return(S_FALSE);
    if(IsCurDeleted()) return S_FALSE;
    if(m_lCurPos < 0) return S_FALSE;

    m_iEditMode = 1;
    return(S_OK);
}

HRESULT GRecordset::GetExtension(BSTR Name, IDispatch * *ppGExtension)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::GetExtension\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
#if DBGLEVEL > 2
    WriteLogFile("  Recordset: ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    /*if(wcsicmp(Name, L"FeatureClass") == 0)
    {
        if(!m_pFCExt) return(E_NOTIMPL);
        if(!ppGExtension) return(E_POINTER);
        if(*ppGExtension) (*ppGExtension)->Release();
        m_pFCExt->AddRef();
        *ppGExtension = m_pFCExt;
        return(S_OK);
    }*/
    return(E_NOTIMPL);
}

void GRecordset::SyncBuf()
{
    if(m_iCtidCol < 0)
    {
        GetRecordBin(m_pPGres, m_lCurPos, m_iRowSize, m_plTypes, m_vBuffer.pvData,
            m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
            m_pConnStruct->iCurrDigits);
    }
    else
    {
        GetRecordCtid(m_pPGres, m_lCurPos, m_iRowSize + 1, m_iCtidCol, m_plTypes,
            m_vBuffer.pvData, m_pConnStruct->lDBEnc, m_vBuffer.sCtid,
            &m_vBuffer.iCtidLen, m_pConnStruct->lGeomOid, m_piGeomDims,
            m_pConnStruct->iCurrDigits);
    }
    return;
}

void GRecordset::SyncFields()
{
    if(m_lCurPos < 0)
    {
        m_pFields->SetBuffer(NULL);
    }
    else
    {
        SyncBuf();
        m_pFields->SetBuffer(m_vBuffer.pvData);
    }
    return;
}

HRESULT GRecordset::GetRows(VARIANT Rows, VARIANT *pVar)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::GetRows\r\n", true);
#endif // DBGLEVEL
    if(!m_pPGres) return(S_FALSE);
    pVar->vt = VT_NULL;
    int iRows = VarToInt(Rows);
    if(iRows < 1) return(S_OK);

    long lCurPos = m_lCurPos;
    long lAvailRows = 0;

    m_bBOF = false;

    if(m_iCtidCol < 0)
    {
        lAvailRows = m_lBufSize - lCurPos;
        if(lAvailRows > iRows) lAvailRows = iRows;
    }
    else
    {
        while((lAvailRows < iRows) && (lCurPos < m_lBufSize))
        {
            if(!PQgetisnull(m_pPGres, lCurPos, m_iCtidCol))
            {
                lAvailRows++;
            }
            lCurPos++;
        }
    }

    if(lAvailRows < iRows) m_bEOF = true;
    if(lAvailRows < 1) return(S_OK);

    CBufRow cRow;
    cRow.pvData = (VARIANT*)malloc(m_iRowSize*sizeof(VARIANT));
    for(int i = 0; i < m_iRowSize; i++)
    {
        VariantInit(&cRow.pvData[i]);
    }

    SAFEARRAYBOUND sab[2];
    sab[0].lLbound = 0;
    sab[0].cElements = m_pFields->GetCount();
    sab[1].lLbound = 0;
    sab[1].cElements = lAvailRows;

    pVar->vt = VT_ARRAY | VT_VARIANT;
    pVar->parray = SafeArrayCreate(VT_VARIANT, 2, sab);

    VARIANT *pvArray;
    SafeArrayAccessData(pVar->parray, (void**)&pvArray);

    lCurPos = m_lCurPos;
    GField *pFld;

    while(lAvailRows > 0)
    {
        if(GetRow(lCurPos++, &cRow))
        {
            lAvailRows--;
            m_pFields->SetBuffer(cRow.pvData);
            for(unsigned int j = 0; j < sab[0].cElements; j++)
            {
                pFld = (GField*)m_pFields->GetItem(j);
                pFld->get_Value(pvArray++);
            }
        }
    }

    SafeArrayUnaccessData(pVar->parray);

    for(int i = 0; i < m_iRowSize; i++)
    {
        VariantClear(&cRow.pvData[i]);
    }
    free(cRow.pvData);

    m_lCurPos = lCurPos;
    SyncFields();
    return(S_OK);
}

HRESULT GRecordset::Move(long Rows, VARIANT StartBookmark)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::Move\r\n", true);
#endif // DBGLEVEL
    if(!m_pPGres) return(S_FALSE);

    m_bBOF = false;

    if(StartBookmark.vt == (VT_ARRAY | VT_UI1))
    {
        long *plng = NULL;
        SafeArrayAccessData(StartBookmark.parray, (void**)&plng);
        m_lCurPos = *plng;
        SafeArrayUnaccessData(StartBookmark.parray);
    }

    if(m_iCtidCol < 0)
    {
        m_lCurPos += Rows;
        if(m_lCurPos >= m_lBufSize) m_lCurPos = m_lBufSize - 1;
    }
    else
    {
        long lSavePos = m_lCurPos;
        while((Rows > 0) && (m_lCurPos < m_lBufSize - 1))
        {
            m_lCurPos++;
            if(!PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol))
            {
                lSavePos = m_lCurPos;
                Rows--;
            }
        }
        if(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol))
        {
            m_lCurPos = lSavePos;
            m_bEOF = true;
        }
    }

    SyncFields();
    return(S_OK);
}

HRESULT GRecordset::MoveFirst()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::MoveFirst\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(m_lRecordCount > 0)
    {
        m_bBOF = true;
        m_bEOF = false;
        m_lCurPos = 0;

        if(m_iCtidCol < 0)
        {
            GetRecordBin(m_pPGres, m_lCurPos, m_iRowSize, m_plTypes, m_vBuffer.pvData,
                m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
                m_pConnStruct->iCurrDigits);
        }
        else
        {
            while(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol) &&
                (m_lCurPos < m_lBufSize - 1))
            {
                m_lCurPos++;
            }

            GetRecordCtid(m_pPGres, m_lCurPos, m_iRowSize + 1, m_iCtidCol, m_plTypes,
                m_vBuffer.pvData, m_pConnStruct->lDBEnc, m_vBuffer.sCtid,
                &m_vBuffer.iCtidLen, m_pConnStruct->lGeomOid, m_piGeomDims,
                m_pConnStruct->iCurrDigits);
        }
        m_pFields->SetBuffer(m_vBuffer.pvData);
    }
    else
    {
        m_bBOF = true;
        m_bEOF = true;
        m_lCurPos = -1;
        m_pFields->SetBuffer(NULL);
    }

    if(m_lCurPos < 0) return(S_FALSE);
    return S_OK;
}

HRESULT GRecordset::MoveLast()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::MoveLast\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(m_lRecordCount > 0)
    {
        m_bBOF = false;
        m_bEOF = true;
        m_lCurPos = m_lBufSize - 1;

        if(m_iCtidCol < 0)
        {
            GetRecordBin(m_pPGres, m_lCurPos, m_iRowSize, m_plTypes, m_vBuffer.pvData,
                m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
                m_pConnStruct->iCurrDigits);
        }
        else
        {
            while(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol) &&
                (m_lCurPos > 0))
            {
                m_lCurPos--;
            }

            GetRecordCtid(m_pPGres, m_lCurPos, m_iRowSize + 1, m_iCtidCol, m_plTypes,
                m_vBuffer.pvData, m_pConnStruct->lDBEnc, m_vBuffer.sCtid,
                &m_vBuffer.iCtidLen, m_pConnStruct->lGeomOid, m_piGeomDims,
                m_pConnStruct->iCurrDigits);
        }
        m_pFields->SetBuffer(m_vBuffer.pvData);
    }
    else
    {
        m_bBOF = true;
        m_bEOF = true;
        m_lCurPos = -1;
        m_pFields->SetBuffer(NULL);
    }

    if(m_lCurPos < 0) return(S_FALSE);
    return S_OK;
}

HRESULT GRecordset::MoveNext()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::MoveNext\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(m_lRecordCount > 0)
    {
        long lLastPos = m_lCurPos;
        m_bBOF = false;
        if(m_lCurPos < m_lBufSize - 1)
        {
            m_lCurPos++;
            if(m_iCtidCol < 0)
            {
                GetRecordBin(m_pPGres, m_lCurPos, m_iRowSize, m_plTypes, m_vBuffer.pvData,
                    m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
                    m_pConnStruct->iCurrDigits);
            }
            else
            {
                while(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol) &&
                    (m_lCurPos < m_lBufSize - 1))
                {
                    m_lCurPos++;
                }

                if(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol))
                {
                    m_lCurPos = lLastPos;
                    m_bEOF = true;
                }
                else
                {
                    GetRecordCtid(m_pPGres, m_lCurPos, m_iRowSize + 1, m_iCtidCol,
                        m_plTypes, m_vBuffer.pvData, m_pConnStruct->lDBEnc,
                        m_vBuffer.sCtid, &m_vBuffer.iCtidLen, m_pConnStruct->lGeomOid,
                        m_piGeomDims, m_pConnStruct->iCurrDigits);
                }
            }
            m_pFields->SetBuffer(m_vBuffer.pvData);
        }
        else m_bEOF = true;
    }
    else
    {
        m_bBOF = true;
        m_bEOF = true;
        m_lCurPos = -1;
        m_pFields->SetBuffer(NULL);
    }

    if(m_lCurPos < 0) return(S_FALSE);
    return S_OK;
}

HRESULT GRecordset::MovePrevious()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::MovePrevious\r\n", true);
#if DBGLEVEL > 2
    WriteLogFile("  ", false);
    WriteLogFile(m_sName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(m_lRecordCount > 0)
    {
        long lLastPos = m_lCurPos;
        m_bEOF = false;
        if(m_lCurPos > 0)
        {
            m_lCurPos--;

            if(m_iCtidCol < 0)
            {
                GetRecordBin(m_pPGres, m_lCurPos, m_iRowSize, m_plTypes, m_vBuffer.pvData,
                    m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
                    m_pConnStruct->iCurrDigits);
            }
            else
            {
                while(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol) &&
                    (m_lCurPos > 0))
                {
                    m_lCurPos--;
                }

                if(PQgetisnull(m_pPGres, m_lCurPos, m_iCtidCol))
                {
                    m_lCurPos = lLastPos;
                    m_bBOF = true;
                }
                else
                {
                    GetRecordCtid(m_pPGres, m_lCurPos, m_iRowSize + 1, m_iCtidCol,
                        m_plTypes, m_vBuffer.pvData, m_pConnStruct->lDBEnc,
                        m_vBuffer.sCtid, &m_vBuffer.iCtidLen, m_pConnStruct->lGeomOid,
                        m_piGeomDims, m_pConnStruct->iCurrDigits);
                }
            }
            m_pFields->SetBuffer(m_vBuffer.pvData);
        }
        else m_bBOF = true;
    }
    else
    {
        m_bBOF = true;
        m_bEOF = true;
        m_lCurPos = -1;
        m_pFields->SetBuffer(NULL);
    }

    if(m_lCurPos < 0) return(S_FALSE);
    return S_OK;
}

HRESULT GRecordset::Update()
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordset::Update\r\n", true);
    if(m_iSpecType == 2) WriteLogFile("  GCoordSystem\r\n", false);
#if DBGLEVEL > 2
    else
    {
        WriteLogFile("  ", false);
        WriteLogFile(m_sName, false);
        WriteLogFile("\r\n", false);
    }
#endif // DBGLEVEL > 2
#endif // DBGLEVEL
    if(!m_bUpdatable) return(S_FALSE);
    if(m_iEditMode < 1) return S_FALSE;

    bool bFound = false;
    int i = 0, j;
    int n = m_pFields->GetCount();
    int nParams = 0;
    char sBuf[64];
    GField *pfld;

    wchar_t wsOldCSGuid[40];
    wchar_t wsNewCSGuid[40];
    wsOldCSGuid[0] = 0;
    wsNewCSGuid[0] = 0;
    VARIANT *pvSCRow = NULL;
    bool bCS = (m_iSpecType == 2);
    if(bCS && (n == 119))
    {
        VARIANT pvSCTblRow[119];
        for(i = 0; i < 119; i++)
        {
            pfld = (GField*)m_pFields->GetItem(i);
            pvSCTblRow[i] = pfld->GetModVal();
        }
        VarToWideChar(pvSCTblRow[0], wsNewCSGuid, 40, false, false);
        pvSCRow = FindCSTableRowByCS(&m_pConnStruct->cCSTable, pvSCTblRow);
        if(!pvSCRow)
        {
            wchar_t wBuf[128];
            LoadString(m_pConnStruct->hInstance, IDE_UNKNOWNCS, wBuf, 128);
            m_pErrors->HandleGDOError(wBuf, m_pConnStruct->hInstance);
            return(E_FAIL);
        }
        VarToWideChar(pvSCRow[1], wsOldCSGuid, 40, false, false);
    }

    int ilen = 72 + 14*n + 15 + GetTableNameLen(m_pTblDef->GetOrigSchemaPtr(),
        m_pTblDef->GetOrigNamePtr());

    for(j = 0; j < n; j++)
    {
        pfld = (GField*)m_pFields->GetItem(j);
        ilen += 2*strlen(pfld->GetOrigNamePtr());
    }

    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    sCmd[0] = 0;

    Oid *plTypes = NULL;
    LPSTR *psParams = NULL;
    int *piLens = NULL;
    int *piForms = NULL;
    VARIANT vVal;
    VariantInit(&vVal);

    i = 0;
    while(!bFound && (i < n))
    {
        pfld = (GField*)m_pFields->GetItem(i++);
        bFound = pfld->Modified();
    }

    long lParams[8];
    CGeomInfo cgInfo;

    if(m_iEditMode == 1)
    {
        if(!bFound)
        {
            // it looks like nothing has changed, so nothing happens
            m_iEditMode = 0;
            m_lModPos = m_lCurPos;
            return(S_OK);
        }

        strcpy(sCmd, "update ");
        CatTableName(sCmd, m_pTblDef->GetOrigSchemaPtr(),
            m_pTblDef->GetOrigNamePtr(), !m_bIsMetatable);
        strcat(sCmd, " set ");
        CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
        strcat(sCmd, " = $1");

        nParams = 1;
        while(i < n)
        {
            pfld = (GField*)m_pFields->GetItem(i++);
            if(pfld->Modified())
            {
                nParams++;
                strcat(sCmd, ", ");
                CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
                sprintf(sBuf, " = $%d", nParams);
                strcat(sCmd, sBuf);
            }
        }
        nParams++;
        sprintf(sBuf, " where ctid = $%d", nParams);
        strcat(sCmd, sBuf);
        strcat(sCmd, " returning ctid");

        plTypes = (Oid*)malloc(nParams*sizeof(Oid));
        psParams = (LPSTR*)malloc(nParams*sizeof(LPSTR));
        piLens = (int*)malloc(nParams*sizeof(int));
        piForms = (int*)malloc(nParams*sizeof(int));

        j = 0;
        for(i = 0; i < n; i++)
        {
            pfld = (GField*)m_pFields->GetItem(i);
            if(pfld->Modified())
            {
                pfld->FillGeomInfo(&cgInfo);
                vVal = pfld->GetModVal();
                //plTypes[j] = VarTypeToOid(vVal);
                plTypes[j] = pfld->GetTypeOid();
                if(pfld->GetTypeOid() == NUMERICOID) plTypes[j] = FLOAT8OID;
                piLens[j] = VarToBinaryLen(vVal, plTypes[j],
                    m_pConnStruct->lDBEnc, &cgInfo, lParams);
                if((piLens[j] > 0) || pfld->GetRequired())
                {
                    psParams[j] = (LPSTR)malloc((piLens[j] + 1)*sizeof(char));
                    VarToBinaryBuf(vVal, plTypes[j], m_pConnStruct->lDBEnc,
                        psParams[j], piLens[j], &cgInfo, lParams, m_pConnStruct->iCurrDigits);
                }
                else psParams[j] = NULL;
                piForms[j++] = 1;
            }
        }
        plTypes[j] = 0;
        piLens[j] = m_vBuffer.iCtidLen;
        psParams[j] = (LPSTR)malloc((piLens[j] + 1)*sizeof(char));
        memcpy(psParams[j], m_vBuffer.sCtid, piLens[j]);
        (psParams[j])[piLens[j]] = 0;
        piForms[j] = 1;
    }
    else if(m_iEditMode == 2)
    {
        if(!bFound)
        {
            // insert without modification can be a problem
            // we have to insert at least one sequenced field with default
            i = 0;
            while(!bFound && (i < n))
            {
                pfld = (GField*)m_pFields->GetItem(i++);
                bFound = pfld->HasSeq();
            }
            if(!bFound)
            {
                return(S_FALSE);
            }

            strcpy(sCmd, "insert into ");
            CatTableName(sCmd, m_pTblDef->GetOrigSchemaPtr(),
                m_pTblDef->GetOrigNamePtr(), !m_bIsMetatable);
            strcat(sCmd, "(");
            CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
            strcat(sCmd, ") values (default) returning ctid");
        }
        else
        {
            strcpy(sCmd, "insert into ");
            CatTableName(sCmd, m_pTblDef->GetOrigSchemaPtr(),
                m_pTblDef->GetOrigNamePtr(), !m_bIsMetatable);
            strcat(sCmd, "(");
            CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
            j = i;
            while(i < n)
            {
                pfld = (GField*)m_pFields->GetItem(i++);
                if(pfld->Modified())
                {
                    strcat(sCmd, ", ");
                    CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
                }
            }
            strcat(sCmd, ") values ($1");
            i = j;
            nParams = 1;
            while(i < n)
            {
                pfld = (GField*)m_pFields->GetItem(i++);
                if(pfld->Modified())
                {
                    nParams++;
                    sprintf(sBuf, ", $%d", nParams);
                    strcat(sCmd, sBuf);
                }
            }
            strcat(sCmd, ") returning ctid");

            plTypes = (Oid*)malloc(nParams*sizeof(Oid));
            psParams = (LPSTR*)malloc(nParams*sizeof(LPSTR));
            piLens = (int*)malloc(nParams*sizeof(int));
            piForms = (int*)malloc(nParams*sizeof(int));

            j = 0;
            for(i = 0; i < n; i++)
            {
                pfld = (GField*)m_pFields->GetItem(i);
                if(pfld->Modified())
                {
                    pfld->FillGeomInfo(&cgInfo);
                    vVal = pfld->GetModVal();
                    //plTypes[j] = VarTypeToOid(vVal);
                    plTypes[j] = pfld->GetTypeOid();
                    if(pfld->GetTypeOid() == NUMERICOID) plTypes[j] = FLOAT8OID;
                    piLens[j] = VarToBinaryLen(vVal, plTypes[j],
                        m_pConnStruct->lDBEnc, &cgInfo, lParams);
                    if((piLens[j] > 0) || pfld->GetRequired())
                    {
                        psParams[j] = (LPSTR)malloc((piLens[j] + 1)*sizeof(char));
                        VarToBinaryBuf(vVal, plTypes[j], m_pConnStruct->lDBEnc,
                            psParams[j], piLens[j], &cgInfo, lParams, m_pConnStruct->iCurrDigits);
                    }
                    else psParams[j] = NULL;
                    piForms[j++] = 1;
                }
            }
        }
        //for(i = 0; i < n; i++)
        //{
        //    pfld = (GField*)m_pFields->GetItem(i);
        //    if(pfld->HasSeq())
        //    {
        //        strcat(sCmd, ", ");
        //        CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
        //    }
        //}
    }

    for(i = 0; i < n; i++)
    {
        pfld = (GField*)m_pFields->GetItem(i);
        strcat(sCmd, ", ");
        CatFieldName(sCmd, pfld->GetOrigNamePtr(), !m_bIsMetatable);
    }

    if(!sCmd[0])
    {
        free(sCmd);
        return(S_FALSE);
    }

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
#if DBGLEVEL > 0
    WriteLogFile(sCmd, true);
    WriteLogFile("\r\n", false);
    WriteMallocReport(10, ilen, strlen(sCmd));
#endif // DBGLEVEL

    SetSP(m_pConnStruct->pConn, bTrans);
    PGresult *res = PQexecParams(m_pConnStruct->pConn, sCmd, nParams, plTypes,
        psParams, piLens, piForms, 1);
    free(sCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        m_pErrors->HandlePQError(m_pConnStruct->pConn, m_pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        for(i = 0; i < nParams; i++)
        {
            if(psParams[i]) free(psParams[i]);
        }
        free(plTypes);
        free(psParams);
        free(piLens);
        free(piForms);
        return(E_FAIL);
    }

    long nrows = PQntuples(res);
//char buf[64];
    if(nrows > 0)
    {
        int ncols = PQnfields(res);
        m_lModPos = m_lCurPos;

        if(m_iEditMode == 2)
        {
            m_lModPos = m_lBufSize;
            m_lCurPos = -1;
            m_lBufSize++;
            m_lRecordCount++;
            m_bEOF = false;
            m_lInserted++;
        }

        if(m_lInserted > 10000)
        {
            // simply reopen the recordset
            PQclear(m_pPGres);
            m_pPGres = PQexecPrepared(m_pConnStruct->pConn, m_sStmtName, m_iParams,
                m_psParams, m_piParLenght, m_piParType, 1);
            m_lBufSize = PQntuples(m_pPGres);
            m_lRecordCount = m_lBufSize;
            m_lInserted = 0;
        }
        else
        {
            int iRes = PQsetvalue(m_pPGres, m_lModPos, m_iCtidCol, PQgetvalue(res, 0, 0),
                PQgetlength(res, 0, 0));

            i = 0;

            while((i < m_iCtidCol) && (iRes != 0))
            {
                if(PQgetisnull(res, 0, i + 1))
                    iRes = PQsetvalue(m_pPGres, m_lModPos, i, NULL, -1);
                else
                    iRes = PQsetvalue(m_pPGres, m_lModPos, i, PQgetvalue(res, 0, i + 1),
                        PQgetlength(res, 0, i + 1));
                i++;
            }
            i = m_iCtidCol + 1;
            while((i < ncols) && (iRes != 0))
            {
                if(PQgetisnull(res, 0, i))
                    iRes = PQsetvalue(m_pPGres, m_lModPos, i, NULL, -1);
                else
                    iRes = PQsetvalue(m_pPGres, m_lModPos, i, PQgetvalue(res, 0, i),
                        PQgetlength(res, 0, i));
                i++;
            }

            if(iRes == 0)
                m_pErrors->HandlePQError(m_pConnStruct->pConn,
                    m_pConnStruct->hInstance);
        }
    }

    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);
    for(i = 0; i < nParams; i++)
    {
        if(psParams[i]) free(psParams[i]);
    }
    free(piForms);
    free(piLens);
    free(psParams);
    free(plTypes);

    if(m_lCurPos < 0) m_lCurPos = m_lModPos;
    SyncBuf();
    m_pFields->UpdateRow(m_vBuffer.pvData);

    GDatabase *pDB = (GDatabase*)m_pConnStruct->lSessionId;
    if(pDB->GetModificationLogging())
        WriteModLog(m_pConnStruct, m_lTblOid, m_pFields, 3 - m_iEditMode);
    m_iEditMode = 0;

    if(bCS && wsOldCSGuid[0])
    {
        VariantClear(&pvSCRow[1]);
        pvSCRow[1].vt = VT_BSTR;
        pvSCRow[1].bstrVal = SysAllocString(wsNewCSGuid);
        pDB->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    }

    return(S_OK);
}

LPWSTR GRecordset::GetNamePtr()
{
    return(m_sName);
}

bool IsDelim(char c)
{
    return((c == 0) || (c == ' ') || (c == '.') || (c == '\'') ||
        (c == '\"') || (c == '(') || (c == ')') || (c == '=') ||
        (c == '<') || (c == '>') || (c == ','));
}

int GetNextWorld(LPSTR sStr, LPSTR sWordBuf, int iBufLen, int *piToSkip,
    int *piToCopy)
{
    int iRes = 0;

    *piToSkip = 0;
    LPSTR sStart = sStr;
    while(sStart[0] == ' ')
    {
        sStart++;
        (*piToSkip)++;
    }
    LPSTR sEnd = sStart;
    while(!IsDelim(sEnd[0])) sEnd++;

    if(sEnd[0] == 0)
    {
        iRes = sEnd - sStart;
        *piToCopy = 0;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        return(iRes);
    }

    if((sEnd[0] == ' ') || (sEnd[0] == '.'))
    {
        iRes = sEnd - sStart;
        *piToCopy = 1;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        return(iRes);
    }

    if(sEnd[0] == '(')
    {
        iRes = sEnd - sStart;
        *piToCopy = 1;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        return(iRes);
    }

    if((sEnd[0] == ')') || (sEnd[0] == ','))
    {
        iRes = sEnd - sStart;
        *piToCopy = 1;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        sEnd++;
        if(sEnd[0] == ' ') *piToCopy = 2;
        return(iRes);
    }

    if(sEnd[0] == '\'')
    {
        iRes = sEnd - sStart;
        *piToCopy = 0;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        sStart = sEnd++;
        sEnd = strchr(sEnd, '\'');
        if(sEnd)
        {
            sEnd++;
            *piToCopy = sEnd - sStart;
            if(sEnd[0] == ' ') (*piToCopy)++;
        }
        return(iRes);
    }

    if(sEnd[0] == '\"')
    {
        iRes = sEnd - sStart;
        *piToCopy = 0;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        sStart = sEnd++;
        sEnd = strchr(sEnd, '\"');
        if(sEnd)
        {
            sEnd++;
            *piToCopy = sEnd - sStart;
            if(sEnd[0] == ' ') (*piToCopy)++;
        }
        return(iRes);
    }

    if((sEnd[0] == '=') || (sEnd[0] == '<') || (sEnd[0] == '>'))
    {
        iRes = sEnd - sStart;
        *piToCopy = 1;
        if(iRes < iBufLen)
        {
            strncpy(sWordBuf, sStart, iRes);
            sWordBuf[iRes] = 0;
        }
        sEnd++;
        if(sEnd[0] == ' ') *piToCopy = 2;
        return(iRes);
    }

    return(iRes);
}

bool IsSqlKeyWord(LPSTR sWord)
{
    if(stricmp(sWord, "select") == 0) return(true);
    if(stricmp(sWord, "*") == 0) return(true);
    if(stricmp(sWord, "from") == 0) return(true);
    if(stricmp(sWord, "where") == 0) return(true);
    if(stricmp(sWord, "like") == 0) return(true);
    if(stricmp(sWord, "distinct") == 0) return(true);
    if(stricmp(sWord, "upper") == 0) return(true);
    if(stricmp(sWord, "lower") == 0) return(true);
    if(stricmp(sWord, "left") == 0) return(true);
    if(stricmp(sWord, "right") == 0) return(true);
    if(stricmp(sWord, "outer") == 0) return(true);
    if(stricmp(sWord, "inner") == 0) return(true);
    if(stricmp(sWord, "join") == 0) return(true);
    if(stricmp(sWord, "order") == 0) return(true);
    if(stricmp(sWord, "group") == 0) return(true);
    if(stricmp(sWord, "by") == 0) return(true);
    if(stricmp(sWord, "asc") == 0) return(true);
    if(stricmp(sWord, "desc") == 0) return(true);
    if(stricmp(sWord, "max") == 0) return(true);
    if(stricmp(sWord, "min") == 0) return(true);
    if(stricmp(sWord, "count") == 0) return(true);
    if(stricmp(sWord, "and") == 0) return(true);
    if(stricmp(sWord, "or") == 0) return(true);
    if(stricmp(sWord, "between") == 0) return(true);
    if(stricmp(sWord, "begin") == 0) return(true);
    if(stricmp(sWord, "end") == 0) return(true);
    if(stricmp(sWord, "in") == 0) return(true);
    if(stricmp(sWord, "not") == 0) return(true);
    if(stricmp(sWord, "is") == 0) return(true);
    if(stricmp(sWord, "null") == 0) return(true);
    if(stricmp(sWord, "true") == 0) return(true);
    if(stricmp(sWord, "false") == 0) return(true);
    if(strcmp(sWord, "DATE") == 0) return(true);
    if(strcmp(sWord, "TIME") == 0) return(true);
    if(strcmp(sWord, "DATETIME") == 0) return(true);
    if(strcmp(sWord, "TIMESTAMP") == 0) return(true);

    int iLen = strlen(sWord);
    int i = 0;
    bool bDot = false;
    if((i < iLen) && (sWord[i] == '-')) i++;
    if((i < iLen) && (sWord[i] == '+')) i++;
    while(((i < iLen) && (sWord[i] >= '0') && (sWord[i] <= '9')) ||
        (!bDot && (sWord[i] == '.')))
    {
        if(sWord[i] == '.') bDot = true;
        i++;
    }
    if(i >= iLen) return(true);

    return(false);
}

LPSTR CopyWithSelect(LPSTR sOrig, bool bDecorate, GTDFields *pFlds)
{
    LPSTR sRes = NULL;
    LPSTR sSelect = strstr(sOrig, "SELECT ");
    if(!sSelect) sSelect = strstr(sOrig, "select ");
    if(!sSelect) sSelect = strstr(sOrig, "Select ");

    int ilen = 0;
    if(sSelect)
    {
        ilen = strlen(sOrig) + 1;
        if(bDecorate)
        {
            bool bWhere = false;
            bool bIsBoolField = false;
            GTDField *pFld = NULL;
            int iBoolVal;

            ilen += 20;
            sRes = (LPSTR)malloc(ilen*sizeof(char));
            sRes[0] = 0;

            int iCurLen = 0;
            LPSTR sCurPos = sOrig;

            int iWordBufLen = 128;
            LPSTR sWordBuf = (LPSTR)malloc(iWordBufLen*sizeof(char));

            int iToSkip, iToCopy;
            int iWorldLen = GetNextWorld(sCurPos, sWordBuf, iWordBufLen,
                &iToSkip, &iToCopy);
            if(iWorldLen >= iWordBufLen)
            {
                free(sWordBuf);
                iWordBufLen = iWorldLen + 1;
                sWordBuf = (LPSTR)malloc(iWordBufLen*sizeof(char));
                iWorldLen = GetNextWorld(sCurPos, sWordBuf, iWordBufLen,
                    &iToSkip, &iToCopy);
            }

            while((iWorldLen > 0) || (iToCopy > 0))
            {
                sCurPos += iToSkip;
                if(ilen < (iCurLen + iWorldLen + iToCopy + 4))
                {
                    ilen = iCurLen + iWorldLen + iToCopy + 4;
                    sRes = (LPSTR)realloc(sRes, ilen*sizeof(char));
                }

                if(iWorldLen > 0)
                {
                    if(!IsSqlKeyWord(sWordBuf))
                    {
                        strcat(sRes, "\"");
                        strcat(sRes, sWordBuf);
                        strcat(sRes, "\"");
                        iCurLen += 2;
                        if(bWhere && pFlds)
                        {
                            pFld = pFlds->GetByName(sWordBuf);
                            if(pFld)
                            {
                                bIsBoolField = (pFld->GetType() == gdbBoolean);
                            }
                            else bIsBoolField = false;
                        }
                    }
                    else
                    {
                        if(bWhere)
                        {
                            if(bIsBoolField)
                            {
                                if(sscanf(sWordBuf, "%d", &iBoolVal) == 1)
                                {
                                    if(iBoolVal)
                                    {
                                        strcat(sRes, "true");
                                        iCurLen += (4 - iWorldLen);
                                    }
                                    else
                                    {
                                        strcat(sRes, "false");
                                        iCurLen += (5 - iWorldLen);
                                    }
                                }
                                else strcat(sRes, sWordBuf);
                            }
                            else strcat(sRes, sWordBuf);
                        }
                        else
                        {
                            bWhere = (stricmp(sWordBuf, "where") == 0);
                            strcat(sRes, sWordBuf);
                        }
                    }
                    sCurPos += iWorldLen;
                    iCurLen += iWorldLen;
                }
                if(iToCopy)
                {
                    strncat(sRes, sCurPos, iToCopy);
                    sCurPos += iToCopy;
                    iCurLen += iToCopy;
                    sRes[iCurLen] = 0;
                }

                iWorldLen = GetNextWorld(sCurPos, sWordBuf, iWordBufLen,
                    &iToSkip, &iToCopy);
                if(iWorldLen >= iWordBufLen)
                {
                    free(sWordBuf);
                    iWordBufLen = iWorldLen + 1;
                    sWordBuf = (LPSTR)malloc(iWordBufLen*sizeof(char));
                    iWorldLen = GetNextWorld(sCurPos, sWordBuf, iWordBufLen,
                        &iToSkip, &iToCopy);
                }
            }

            free(sWordBuf);
        }
        else
        {
            sRes = (LPSTR)malloc(ilen*sizeof(char));
            strcpy(sRes, sOrig);
        }
    }
    else
    {
        ilen = strlen(sOrig) + 20; // "select * from "
        sRes = (LPSTR)malloc(ilen*sizeof(char));
        strcpy(sRes, "select * from ");
        if(bDecorate)
        {
            strcat(sRes, "\"");
            LPSTR sDot = strchr(sOrig, '.');
            if(sDot)
            {
                strncat(sRes, sOrig, sDot - sOrig);
                sRes[sDot - sOrig + 15] = 0;
                strcat(sRes, "\".\"");
                sDot++;
                strcat(sRes, sDot);
            }
            else strcat(sRes, sOrig);
            strcat(sRes, "\"");
        }
        else strcat(sRes, sOrig);
    }
    return(sRes);
}

int GetCtidColumn(PGresult *res)
{
    int iRes = -1;
    int ncols = PQnfields(res);
    int i = 0;
    while((iRes < 0) && (i < ncols))
    {
        if(stricmp(PQfname(res, i), "ctid") == 0) iRes = i;
        i++;
    }
    return(iRes);
}

bool GRecordset::SetAttributes(BSTR Name, VARIANT Type, VARIANT options,
    VARIANT SpatialOp, VARIANT SpatialGeometryFilter, VARIANT GeometryFieldName,
    GTableDefs *pGTbls, PConnStruct pConnStruct)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;

    m_iParams = 0;
    m_psParams = NULL;
    m_piParType = NULL;
    m_piParLenght = NULL;

    m_pFields->ClearAll();

    m_iOpenStatus = 0;
    m_pConnStruct->lCursors++;
    sprintf(m_sStmtName, "recset%.4ld", m_pConnStruct->lCursors);

    wcsncpy(m_sName, Name, 255);
    m_sName[255] = 0;

    VariantCopy(&m_vType, &Type);
    VariantCopy(&m_voptions, &options);
    VariantCopy(&m_vSpatialOp, &SpatialOp);
    VariantCopy(&m_vSpatialGeometryFilter, &SpatialGeometryFilter);
    VariantCopy(&m_vGeometryFieldName, &GeometryFieldName);

    if(m_vType.vt == VT_ERROR)
    {
        m_vType.vt = VT_I4;
        m_vType.lVal = gdbOpenDynaset;
    }

    if(m_voptions.vt == VT_ERROR)
    {
        m_voptions.vt = VT_I4;
        m_voptions.lVal = 0;
    }

    long lopt = VarToLong(m_voptions);
    if(lopt > -1) m_bUpdatable = !(lopt & 4);
    else m_bUpdatable = true;

    if(m_bUpdatable) m_bUpdatable = pConnStruct->bUpdatable;

    LPWSTR wsName = GetTableName(Name);
    LPSTR sName = WCharToDBStr(wsName, pConnStruct->lDBEnc);

    //m_pFCExt = new GFeatureClassExt(this,
    //    ((ITypeLib**)pConnStruct->ppTypeLibs)[1]);
    //m_pFCExt->SetTableName(wsName);

    GTableDef *pTBD = NULL;
    GTDFields *pTBFlds = NULL;
    int idx = pGTbls->FindByWName(wsName);
    if(idx > -1)
    {
        pTBD = (GTableDef*)pGTbls->GetItem(idx);
        pTBD->BuildFromConnection();
        pTBFlds = pTBD->GetFieldsPtr();
        if(m_bUpdatable) m_bUpdatable = pTBD->GetUpdatable();
    }

    LPSTR sOpenStr = NULL;
    int ilen;
    LPSTR sGATStart;
    bool bNull;
    int iGeomParOffs = 0;

    int iCSRows = 0;

    if(stricmp(sName, pConnStruct->sCoordSystems) == 0) // coordsystem
    {
        if(pConnStruct->iConnStat <= icsINGRMetaIncomplete)
        {
            iCSRows = GetNamedCSGuidCount(&m_pConnStruct->cCSTable);
            m_iParams = 119*iCSRows;
            iGeomParOffs = m_iParams;
        }
    }

    iGeomParOffs = m_iParams;

    LPSTR sGeomName = NULL;
    unsigned long ulSrid = 0;

    if(SpatialGeometryFilter.vt == (VT_ARRAY | VT_UI1))
    {
        BSTR bsGeom = NULL;

        if(GeometryFieldName.vt == VT_BSTR) bsGeom = GeometryFieldName.bstrVal;
        else if(GeometryFieldName.vt == (VT_BYREF | VT_BSTR))
            bsGeom = *GeometryFieldName.pbstrVal;

        if(bsGeom)
        {
            GTDField *pFld = NULL;
            if(pTBD) pFld = pTBFlds->GetByWName(bsGeom);
            if(pFld)
            {
                ulSrid = pFld->GetSrid();
                sGeomName = pFld->GetFilterFieldName();
            }
        }
    }
    if(sGeomName) m_iParams++;

    Oid *plDataTypes = NULL;
    if(m_iParams > 0)
    {
        m_psParams = (LPSTR*)malloc(m_iParams*sizeof(LPSTR));
        m_piParType = (int*)malloc(m_iParams*sizeof(int));
        m_piParLenght = (int*)malloc(m_iParams*sizeof(int));
        plDataTypes = (Oid*)malloc(m_iParams*sizeof(Oid));
        for(int i = 0; i < m_iParams; i++) m_piParType[i] = 1;
        for(int i = iGeomParOffs; i < m_iParams; i++) plDataTypes[i] = BYTEAOID;
    }

    LPSTR sOrigName = WCharToDBStr(Name, pConnStruct->lDBEnc);

    LPSTR sRest = strstr(sOrigName, sName);
    if(sRest)
    {
        sRest += strlen(sName);
        if(*sRest == ']') sRest++;
        if(*sRest == 0) sRest = NULL;
    }

    PGresult *res;
    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) > 0;

    m_iCtidCol = -1;
    m_bIsMetatable = false;

    if(stricmp(sName, "GAliasTable") == 0)
    {
        m_bIsMetatable = true;
        m_iSpecType = 1;
        if(pConnStruct->iConnStat > icsNoMetadata)
            sOpenStr = CopyWithSelect(sOrigName, false, NULL);
        else
        {
            m_bUpdatable = false;

            LPCSTR sGATAlias = "select \
cast('GCoordSystemTable' as varchar(255)) as TableType, \
cast('GCoordSystem' as varchar(255)) as TableName";
            sGATStart = strstr(sOrigName, sName);
            ilen = sGATStart - sOrigName;
            if(ilen > 0)
            {
                int il1 = ilen;
                ilen += (strlen(sGATAlias) + 20); // strlen("() as GAliasTable");
                if(sRest) ilen += strlen(sRest);
                sOpenStr = (LPSTR)malloc((ilen + 1)*sizeof(char));
                strncpy(sOpenStr, sOrigName, il1);
                sOpenStr[il1] = 0;
                strcat(sOpenStr, "(");
                strcat(sOpenStr, sGATAlias);
                strcat(sOpenStr, ") as GAliasTable");
                if(sRest) strcat(sOpenStr, sRest);
            }
            else
            {
                ilen = 35; // strlen("select * from () as GAliasTable");
                // this should not happen, but for sure:
                ilen += strlen(sGATAlias);
                if(sRest) ilen += strlen(sRest);
                sOpenStr = (LPSTR)malloc((ilen + 1)*sizeof(char));
                strcpy(sOpenStr, "select * from (");
                strcat(sOpenStr, sGATAlias);
                strcat(sOpenStr, ") as GAliasTable");
                // again this should not happen, but for sure:
                if(sRest) strcat(sOpenStr, sRest);
            }
        }
    }
    else if(stricmp(sName, "FieldLookup") == 0)
    {
        // another hack for GI Toolkit
        m_bIsMetatable = true;
        LPSTR sTblName = strstr(sOrigName, " FieldLookup ");
        if(sTblName)
        {
            int slen = (sTblName - sOrigName) +
                strlen(pConnStruct->sFieldLookup) + strlen(sTblName) - 10;
            sOpenStr = (LPSTR)malloc(slen*sizeof(char));
            strncpy(sOpenStr, sOrigName, sTblName - sOrigName + 1);
            sOpenStr[sTblName - sOrigName + 1] = 0;
            sTblName += 12;
            strcat(sOpenStr, pConnStruct->sFieldLookup);
            strcat(sOpenStr, sTblName);
        }
        else sOpenStr = CopyWithSelect(pConnStruct->sFieldLookup, false, NULL);
    }
    else if(stricmp(sName, "AttributeProperties") == 0)
    {
        // another hack for GI Toolkit
        m_bIsMetatable = true;
        LPSTR sTblName = strstr(sOrigName, " AttributeProperties ");
        if(sTblName)
        {
            int slen = (sTblName - sOrigName) +
                strlen(pConnStruct->sAttributeProperties) + strlen(sTblName) - 18;
            sOpenStr = (LPSTR)malloc(slen*sizeof(char));
            strncpy(sOpenStr, sOrigName, sTblName - sOrigName + 1);
            sOpenStr[sTblName - sOrigName + 1] = 0;
            sTblName += 20;
            strcat(sOpenStr, pConnStruct->sAttributeProperties);
            strcat(sOpenStr, sTblName);
        }
        else sOpenStr = CopyWithSelect(pConnStruct->sAttributeProperties,
            false, pTBFlds);
    }
    else if(stricmp(sName, pConnStruct->sCoordSystems) == 0) // coordsystem
    {
        m_bIsMetatable = true;
        m_iSpecType = 2;
        if(pConnStruct->iConnStat > icsINGRMetaIncomplete)
            sOpenStr = CopyWithSelect(sOrigName, false, NULL);
        else
        {
            m_bUpdatable = false;

            LPSTR sTmp = GetCSSelectString(&m_pConnStruct->cCSTable, &bNull,
                m_psParams, m_piParLenght, plDataTypes, pConnStruct->lDBEnc,
                iCSRows);

            sGATStart = strstr(sOrigName, sName);
            ilen = sGATStart - sOrigName;
            if(ilen > 0)
            {
                int il1 = ilen;
                ilen += (strlen(sTmp) + strlen(sName) + 8); // strlen("() as ");
                if(bNull) ilen += 16; // " where 0 = 1"
                else if(sRest) ilen += strlen(sRest);
                sOpenStr = (LPSTR)malloc((ilen + 1)*sizeof(char));
                strncpy(sOpenStr, sOrigName, il1);
                sOpenStr[il1] = 0;
                strcat(sOpenStr, "(");
                strcat(sOpenStr, sTmp);
                strcat(sOpenStr, ") as ");
                strcat(sOpenStr, sName);
                if(bNull) strcat(sOpenStr, " where 0 = 1");
                else if(sRest) strcat(sOpenStr, sRest);
            }
            else
            {
                ilen = 24 + strlen(sName);  // strlen("select * from () as ");
                // this should not happen, but for sure:
                ilen += strlen(sTmp);
                if(bNull) ilen += 16;
                else if(sRest) ilen += strlen(sRest);
                sOpenStr = (LPSTR)malloc((ilen + 1)*sizeof(char));
                strcpy(sOpenStr, "select * from (");
                strcat(sOpenStr, sTmp);
                strcat(sOpenStr, ") as ");
                strcat(sOpenStr, sName);
                if(bNull) strcat(sOpenStr, " where 0 = 1");
                // again this should not happen, but for sure:
                else if(sRest) strcat(sOpenStr, sRest);
            }

            free(sTmp);
        }
    }
    else if(stricmp(sName, pConnStruct->sPickLists) == 0) // gpicklist
    {
        m_bIsMetatable = true;
        LPSTR sFtrName = NULL;
        LPSTR sFtrEnd = NULL;
        LPSTR sWhere = strstr(sOrigName, " WHERE ");
        if(!sWhere) sWhere = strstr(sOrigName, " where ");
        if(!sWhere) sWhere = strstr(sOrigName, " Where ");
        if(sWhere)
        {
            sWhere += 6;
            sFtrName = strstr(sWhere, " FeatureName");
            if(!sFtrName) sFtrName = strstr(sWhere, " FEATURENAME");
            if(!sFtrName) sFtrName = strstr(sWhere, " featurename");
        }
        if(sFtrName)
        {
            sFtrEnd = sFtrName + 12;
            while((*sFtrEnd == ' ') && (*sFtrEnd != 0)) sFtrEnd++;
            if(*sFtrEnd != '=') sFtrEnd = NULL;
        }

        if(sFtrEnd)
        {
            int slen = sFtrName - sOrigName + strlen(sFtrEnd) + 21; // " upper(featurename) "
            sOpenStr = (LPSTR)malloc(slen*sizeof(char));
            strncpy(sOpenStr, sOrigName, sFtrName - sOrigName);
            sOpenStr[sFtrName - sOrigName] = 0;
            strcat(sOpenStr, " upper(featurename) ");
            strcat(sOpenStr, sFtrEnd);
        }
        else sOpenStr = CopyWithSelect(sOrigName, false, NULL);
    }
    else
    {
        m_bIsMetatable = IsMetatable(pConnStruct, sName);

        SetSP(m_pConnStruct->pConn, bTrans);
        //res = PQprepare(m_pConnStruct->pConn, m_sStmtName, sOrigName,
        //    iGeomParOffs, plDataTypes);
        sOpenStr = CopyWithSelect(sOrigName, !m_bIsMetatable, pTBFlds);
        res = PQprepare(m_pConnStruct->pConn, m_sStmtName, sOpenStr,
            iGeomParOffs, plDataTypes);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            // The query is most likely not in the for select ... from
            // so skip the error logging
            //m_pErrors->HandlePQError(m_pConnStruct->pConn,
            //    m_pConnStruct->hInstance);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);

            SetSP(m_pConnStruct->pConn, bTrans);
            res = PQdescribePrepared(m_pConnStruct->pConn, m_sStmtName);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                PQclear(res);
                RollbackSP(m_pConnStruct->pConn, bTrans);
            }
            else
            {
                m_iCtidCol = GetCtidColumn(res);
                PQclear(res);
                ReleaseSP(m_pConnStruct->pConn, bTrans);
            }

            char sCmd[64];
            sprintf(sCmd, "deallocate %s", m_sStmtName);

            SetSP(m_pConnStruct->pConn, bTrans);
            res = PQexec(m_pConnStruct->pConn, sCmd);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                PQclear(res);
                RollbackSP(m_pConnStruct->pConn, bTrans);
            }
            else
            {
                PQclear(res);
                ReleaseSP(m_pConnStruct->pConn, bTrans);
            }
        }

        if(sGeomName)
        {
            //free(sOpenStr);
            //sOpenStr = ParseSQLWithSpatialFilter(sOrigName, SpatialOp,
            //    SpatialGeometryFilter, sGeomName, ulSrid,
            //    &m_psParams[iGeomParOffs], &m_piParLenght[iGeomParOffs], m_iParams);
            LPSTR sNewStr = ParseSQLWithSpatialFilter(sOpenStr, SpatialOp,
                SpatialGeometryFilter, sGeomName, ulSrid,
                &m_psParams[iGeomParOffs], &m_piParLenght[iGeomParOffs], m_iParams);
            free(sOpenStr);
            sOpenStr = sNewStr;
        }
    }

    bool bModLog = (stricmp(pConnStruct->sModLog, sName) == 0);

    if(sGeomName) free(sGeomName);

    if(m_bUpdatable && (m_iCtidCol < 0))
    {
        LPSTR sOldOpen = sOpenStr;

        LPSTR sSelect = strstr(sOldOpen, "SELECT ");
        if(!sSelect) sSelect = strstr(sOldOpen, "select ");
        if(!sSelect) sSelect = strstr(sOldOpen, "Select ");
        if(sSelect)
        {
            sSelect += 7;
            ilen = strlen(sOldOpen) + 8; // "ctid, "
            sOpenStr = (LPSTR)malloc(ilen*sizeof(char));
            strncpy(sOpenStr, sOldOpen, sSelect - sOldOpen);
            sOpenStr[sSelect - sOldOpen] = 0;
            strcat(sOpenStr, "ctid, ");
            strcat(sOpenStr, sSelect);
        }
        else
        {
            ilen = strlen(sOldOpen) + 24; // "select ctid, * from "
            sOpenStr = (LPSTR)malloc(ilen*sizeof(char));
            strcpy(sOpenStr, "select ctid, * from ");
            strcat(sOpenStr, sOldOpen);
        }

        SetSP(m_pConnStruct->pConn, bTrans);
        res = PQprepare(m_pConnStruct->pConn, m_sStmtName, sOpenStr,
            m_iParams, plDataTypes);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Spot 5: ", m_pConnStruct->pConn);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
            m_bUpdatable = false;
            free(sOpenStr);
            sOpenStr = sOldOpen;
        }
        else
        {
            m_iCtidCol = 0;
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
            m_iOpenStatus = 1;
            free(sOldOpen);
        }
    }

    if(m_iOpenStatus < 1)
    {
        SetSP(m_pConnStruct->pConn, bTrans);
        res = PQprepare(m_pConnStruct->pConn, m_sStmtName, sOpenStr,
            m_iParams, plDataTypes);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            m_pErrors->HandlePQError(m_pConnStruct->pConn,
                m_pConnStruct->hInstance);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
            m_iOpenStatus = 1;
        }
    }

    if(m_iOpenStatus > 0)
    {
        m_pPGres = PQexecPrepared(m_pConnStruct->pConn, m_sStmtName, m_iParams,
            m_psParams, m_piParLenght, m_piParType, 1);
        if(PQresultStatus(m_pPGres) != PGRES_TUPLES_OK)
        {
            m_pErrors->HandlePQError(m_pConnStruct->pConn,
                m_pConnStruct->hInstance);
            PQclear(m_pPGres);
            m_pPGres = NULL;
            m_iOpenStatus = 0;
        }
    }

    if(plDataTypes) free(plDataTypes);
    free(sOrigName);

    if(m_iOpenStatus < 1)
    {
        for(int i = 0; i < m_iParams; i++)
        {
            if(m_psParams[i]) free(m_psParams[i]);
        }
        free(m_psParams);
        free(m_piParType);
        free(m_piParLenght);

        free(sOpenStr);
        free(sName);
        free(wsName);
        return(false);
    }

#if DBGLEVEL > 0
    WriteLogFile("GRecordset::SetAttributes\r\n", true);
    WriteLogFile("  request: ", false);
    WriteLogFile(sOpenStr, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    free(sOpenStr);

    m_iRowSize = PQnfields(m_pPGres);
    m_vBuffer.pvData = (VARIANT*)malloc(m_iRowSize*sizeof(VARIANT));
    for(int i = 0; i < m_iRowSize; i++)
    {
        VariantInit(&m_vBuffer.pvData[i]);
    }
    m_plTypes = (Oid*)malloc(m_iRowSize*sizeof(Oid));
    DescribeFieldTypes(m_pPGres, m_iRowSize, m_plTypes);
    if(m_bUpdatable)
    {
        m_lTblOid = PQftable(m_pPGres, 0);
        m_pTblDef = pGTbls->FindByOid(m_lTblOid);
        if(!m_pTblDef) m_bUpdatable = false;
    }

    m_pFields->BuildFromConn(m_pConnStruct, m_pPGres, pGTbls, m_iCtidCol,
        m_plTypes, m_iSpecType, m_bIsMetatable);

    free(sName);
    free(wsName);

    m_piGeomDims = (int*)malloc(m_pFields->GetCount()*sizeof(int));
    m_pFields->UpdateFields(m_piGeomDims);

    if((m_iSpecType > 0) && (pConnStruct->iConnStat < icsINGRMetadata))
    {
        m_vType.vt = VT_I4;
        m_vType.lVal = 4;
        m_voptions.vt = VT_I4;
        m_voptions.lVal = 4;
    }

    m_lRecordCount = PQntuples(m_pPGres);
    m_lBufSize = m_lRecordCount;

    if(m_lRecordCount > 0)
    {
        m_bBOF = false;
        m_bEOF = false;
        m_lCurPos = 0;
    }
    else
    {
        m_bBOF = true;
        m_bEOF = true;
        m_lCurPos = -1;
    }

    SyncFields();

    // this is very tricky part and we are not sure whether it is even needed
    /*if(bModLog && (m_iCtidCol > -1))
    {
        m_pParent->BroadcastDataChanges(this);
    }*/

    return(true);
}

void GRecordset::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    m_pFields->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    return;
}

/*void GRecordset::RecordDeleted(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid)
{
    if(!m_pTblDef) return;
    if(wcscmp(wsTblName, m_pTblDef->GetNamePtr()) != 0) return;

    PBufRow pRow = m_pBuffer->FindByCtid(iCtidLen, sCtid);
    if(pRow)
    {
        pRow->iState = 2;
        if(pRow == m_pBuffer->GetCurRow()) m_pFields->SetBuffer(NULL);
    }

    return;
}*/

bool GRecordset::GetIsMetatable()
{
    return(m_bIsMetatable);
}

/*long GRecordset::FindByCtid(int iCtidLen, LPSTR pCtid)
{
    if(m_iCtidCol < 0) return -2;
    long lPos = 0;
    bool bFound = false;
    int iLen;

    while(!bFound && (lPos < m_lBufSize))
    {
        if(!PQgetisnull(m_pPGres, lPos, m_iCtidCol))
        {
            iLen = PQgetlength(m_pPGres, lPos, m_iCtidCol);
            if(iCtidLen == iLen)
            {
                bFound = (memcmp(PQgetvalue(m_pPGres, lPos, m_iCtidCol), pCtid, iCtidLen) == 0);
            }
        }
        lPos++;
    }
    return bFound ? lPos - 1 : -1;
}

void GRecordset::UpdateWithDBChanges(int iModType, VARIANT *pvModTable,
    VARIANT *pvModData, PGresult *res, LPSTR sSchema, LPSTR sTable,
    bool bHasCtid, int iCtidLen, LPSTR pCtid)
{
    if(!m_pPGres) return;

    long lRecPos = -1;

    if(bHasCtid) lRecPos = FindByCtid(iCtidLen, pCtid);

    bool bHasKey = true;
    int i = 2;
    int piKeyMap[10];
    int piFldMap[10];
    char sNameBuf[64];
    while(bHasKey && (i < 12))
    {
        piKeyMap[i - 2] = -1;
        piFldMap[i - 2] = -1;
        if(pvModTable[i].vt == VT_LPSTR)
        {
            piKeyMap[i - 2] =
                m_pFields->HasField(sSchema, sTable, pvModTable[i].pcVal);
            strcpy(sNameBuf, "\"");
            strcat(sNameBuf, pvModTable[i].pcVal);
            strcat(sNameBuf, "\"");
            piFldMap[i - 2] = PQfnumber(res, sNameBuf);
            bHasKey = (piKeyMap[i - 2] > -1) && (piFldMap[i - 2] > -1);
        }
        i++;
    }

    if(!bHasKey) return;

    int nCols = PQnfields(res);
    int iStart = 0;
    if(bHasCtid) iStart++;
    int iPos;
    long lPos;
    int iValSize;
    LPSTR sVal;
    CGeomInfo cgInfo;
    GField *pFld;
    long lParams[8];

    if(iModType == 1) // new record
    {
        lPos = m_lBufSize;
        m_lBufSize++;
        m_lRecordCount++;

        if(m_iCtidCol > -1)
        {
            PQsetvalue(m_pPGres, lPos, m_iCtidCol, pCtid, iCtidLen);
        }

        for(i = iStart; i < nCols; i++)
        {
            iPos = m_pFields->HasField(sSchema, sTable, PQfname(res, i));
            if(iPos > - 1)
            {
                pFld = (GField*)m_pFields->GetItem(iPos);
                pFld->FillGeomInfo(&cgInfo);
                iValSize = VarToBinaryLen(pvModData[i - iStart], m_plTypes[iPos],
                    m_pConnStruct->lDBEnc, &cgInfo, lParams);
                if(iValSize > 0)
                {
                    sVal = (LPSTR)malloc(iValSize + 1);
                    VarToBinaryBuf(pvModData[i - iStart], m_plTypes[iPos], m_pConnStruct->lDBEnc,
                        sVal, iValSize, &cgInfo, lParams, m_pConnStruct->iCurrDigits);
                }
                else sVal = NULL;

                if(m_iCtidCol > -1)
                {
                    if(iPos >= m_iCtidCol) iPos++;
                }

                PQsetvalue(m_pPGres, lPos, iPos, sVal, iValSize);
                if(sVal) free(sVal);
            }
        }

        SyncFields();
        return;
    }

    lPos = 0;
    if(m_iCtidCol > -1)
    {
        while(PQgetisnull(m_pPGres, lPos, m_iCtidCol) && (lPos < m_lBufSize - 1))
        {
            lPos++;
        }
        if(PQgetisnull(m_pPGres, lPos, m_iCtidCol)) return;
    }

    if(lPos >= m_lBufSize) return;

    bool bFound = true;
    CBufRow cRow;
    cRow.pvData = (VARIANT*)malloc(m_iRowSize*sizeof(VARIANT));
    for(i = 0; i < m_iRowSize; i++) VariantInit(&cRow.pvData[i]);

    GetRow(lPos, &cRow);

    i = 0;
    while(bFound && (i < 10))
    {
        if(piKeyMap[i] > -1)
        {
            bFound = VarArgCompare(cRow.pvData[piKeyMap[i]],
                pvModData[piFldMap[i] - iStart]);
        }
        i++;
    }

    while(!bFound && (lPos < m_lBufSize - 1))
    {
        lPos++;
        if(GetRow(lPos, &cRow))
        {
            bFound = true;
            i = 0;
            while(bFound && (i < 10))
            {
                if(piKeyMap[i] > -1)
                {
                    bFound = VarArgCompare(cRow.pvData[piKeyMap[i]],
                        pvModData[piFldMap[i] - iStart]);
                }
                i++;
            }
        }
    }

    for(i = 0; i < m_iRowSize; i++) VariantClear(&cRow.pvData[i]);
    free(cRow.pvData);

    if(!bFound) return;

    if(iModType == 3)
    {
        if(m_iCtidCol > -1)
        {
            PQsetvalue(m_pPGres, lPos, m_iCtidCol, NULL, -1);
            m_lRecordCount--;
            if(m_lRecordCount < 1)
            {
                m_bBOF = true;
                m_bEOF = true;
                m_lCurPos = -1;
            }
        }
        return;
    }

    if(m_iCtidCol > -1)
    {
        PQsetvalue(m_pPGres, lPos, m_iCtidCol, pCtid, iCtidLen);
    }

    for(i = iStart; i < nCols; i++)
    {
        iPos = m_pFields->HasField(sSchema, sTable, PQfname(res, i));
        if(iPos > - 1)
        {
            pFld = (GField*)m_pFields->GetItem(iPos);
            pFld->FillGeomInfo(&cgInfo);
            iValSize = VarToBinaryLen(pvModData[i - iStart], m_plTypes[iPos],
                m_pConnStruct->lDBEnc, &cgInfo, lParams);
            if(iValSize > 0)
            {
                sVal = (LPSTR)malloc(iValSize + 1);
                VarToBinaryBuf(pvModData[i - iStart], m_plTypes[iPos], m_pConnStruct->lDBEnc,
                    sVal, iValSize, &cgInfo, lParams, m_pConnStruct->iCurrDigits);
            }
            else sVal = NULL;

            if(m_iCtidCol > -1)
            {
                if(iPos >= m_iCtidCol) iPos++;
            }

            PQsetvalue(m_pPGres, lPos, iPos, sVal, iValSize);
            if(sVal) free(sVal);
        }
    }

    SyncFields();
    return;
}*/

int GRecordset::GetFieldsCount()
{
    return m_iRowSize;
}

long GRecordset::GetBufSize()
{
    return m_lBufSize;
}

bool GRecordset::GetRow(long lPos, PBufRow pBuf)
{
    if(m_iCtidCol < 0)
    {
        GetRecordBin(m_pPGres, lPos, m_iRowSize, m_plTypes, pBuf->pvData,
            m_pConnStruct->lDBEnc, m_pConnStruct->lGeomOid, m_piGeomDims,
            m_pConnStruct->iCurrDigits);
        return true;
    }

    if(PQgetisnull(m_pPGres, lPos, m_iCtidCol)) return false;

    GetRecordCtid(m_pPGres, lPos, m_iRowSize + 1, m_iCtidCol, m_plTypes,
        pBuf->pvData, m_pConnStruct->lDBEnc, pBuf->sCtid,
        &pBuf->iCtidLen, m_pConnStruct->lGeomOid, m_piGeomDims,
        m_pConnStruct->iCurrDigits);

    return true;
}


//  GRecordsets

GRecordsets::GRecordsets(IUnknown *pUnkOuter, ITypeLib *ALib) :
    _IGCollection(false, pUnkOuter, ALib, 29)
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordsets::GRecordsets-1\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_pGTbls = NULL;
}

GRecordsets::GRecordsets(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    _IGCollection(false, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordsets::GRecordsets-2\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_pGTbls = NULL;
}

GRecordsets::~GRecordsets()
{
#if DBGLEVEL > 1
    WriteLogFile("GRecordsets::~GRecordsets\r\n", true);
#endif // DBGLEVEL
}

HRESULT GRecordsets::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GRecordsets::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GRecordsets))
        {
            hres = S_OK;
            *ppvObject = this;
            ((IUnknown*)*ppvObject)->AddRef();
        }
        else
        {
            hres = E_NOINTERFACE;
#if DBGLEVEL > 2
            char buf[128];
            FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
            WriteLogFile(buf, true);
#endif // DBGLEVEL
        }
    }
    return(hres);
}

ULONG GRecordsets::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GRecordsets::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GRecordsets::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GRecordsets::get_Item(VARIANT index, GRecordset* *ppGRecordset)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GRecordsets::get_Item\r\n", true);
#endif // DBGLEVEL
    if(!ppGRecordset) return(E_POINTER);
    // a "get_CollectionItem" method is probably an exception from the COM
    // standards - it can accept uninitialized return values, similary as
    // QueryInterface method
    //if(*ppGRecordset) (*ppGRecordset)->Release();
    *ppGRecordset = NULL;

    bool bFound = false;
    int i = 0;
    GRecordset *pRS = NULL;
    if(index.vt == VT_BSTR)
    {
        while(!bFound && (i < m_iDataLen))
        {
            pRS = (GRecordset*)m_pData[i++];
            bFound = (wcsncmp(pRS->GetNamePtr(), index.bstrVal, 256) == 0);
        }
        if(!bFound) return(S_FALSE);
    }
    else if(index.vt == (VT_BYREF | VT_BSTR))
    {
        while(!bFound && (i < m_iDataLen))
        {
            pRS = (GRecordset*)m_pData[i++];
            bFound = (wcsncmp(pRS->GetNamePtr(), *index.pbstrVal, 256) == 0);
        }
        if(!bFound) return(S_FALSE);
    }
    else
    {
        i = VarToInt(index);
        pRS = (GRecordset*)_IGCollection::GetItem(i);
        if(!pRS) return(S_FALSE);
    }

    pRS->AddRef();
    *ppGRecordset = pRS;
    return(S_OK);
}

HRESULT GRecordsets::Append(GRecordset* pObject)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordsets::Append\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GRecordsets::Remove(VARIANT vIndex)
{
#if DBGLEVEL > 0
    WriteLogFile("GRecordsets::Remove\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

void GRecordsets::ClearAll()
{
    _IGCollection::ClearAll();
    return;
}

GRecordset* GRecordsets::FindRecordset(BSTR Name, VARIANT Type, VARIANT options,
    VARIANT SpatialOp, VARIANT SpatialGeometryFilter, VARIANT GeometryFieldName,
    GTableDefs *pGTbls, PConnStruct pConnStruct)
{
    m_pConnStruct = pConnStruct;
    m_pGTbls = pGTbls;

    GRecordset *pRes = new GRecordset(this, m_pTypeLib);
    if(pRes->SetAttributes(Name, Type, options, SpatialOp,
        SpatialGeometryFilter, GeometryFieldName, pGTbls, pConnStruct))
    {
        pRes->AddRef();
        _IGCollection::AddItem(pRes);
    }
    else
    {
        pRes->AddRef();
        pRes->Release();
        pRes = NULL;
    }

    return(pRes);
}

void GRecordsets::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    GRecordset *pRS;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pRS = (GRecordset*)m_pData[i];
        pRS->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    }
    return;
}

/*void GRecordsets::BroadcastDelRecord(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid)
{
    GRecordset *pRS;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pRS = (GRecordset*)m_pData[i];
        pRS->RecordDeleted(wsTblName, iCtidLen, sCtid);
    }
    return;
}*/

void GRecordsets::DeletePtr(GRecordset *pRS)
{
    bool bFound = false;
    int i = 0;
    while(!bFound && (i < m_iDataLen))
    {
        bFound = (pRS == m_pData[i++]);
    }
    if(bFound) _IGCollection::DeleteItem(i - 1);
    return;
}

/*void GRecordsets::ProcessModLogRec(PBufRow pRow)
{
    int ilen = 64 + strlen(m_pConnStruct->sModTables);
    LPSTR sSql = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sSql, "select * from %s where modifiedtableid = %ld",
        m_pConnStruct->sModTables, VarToLong(pRow->pvData[2]));

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

    SetSP(m_pConnStruct->pConn, bTrans);
    PGresult *res = PQexec(m_pConnStruct->pConn, sSql);
    free(sSql);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 87: ", m_pConnStruct->pConn);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
        return;
    }

    VARIANT pvRes[12];
    for(int i = 0; i < 12; i++) VariantInit(&pvRes[i]);

    Oid plType[12];
    DescribeFieldTypes(res, 12, plType);

    GetRecord(res, 0, 12, plType, pvRes, m_pConnStruct->sDecSep);

    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    if(pvRes[1].vt != VT_LPSTR) return;
    if(pvRes[2].vt != VT_LPSTR) return;

    int iFldLen = strlen(pvRes[2].pcVal);
    if(iFldLen < 1) return;

    int nParams = 1;
    char* ppParams[10];

    if(pRow->pvData[3].vt == VT_BSTR)
        ppParams[0] = WCharToDBStr(pRow->pvData[3].bstrVal, m_pConnStruct->lDBEnc);
    else ppParams[0] = NULL;

    ilen = 40 + strlen(pvRes[1].pcVal) + iFldLen;
    for(int i = 3; i < 12; i++)
    {
        if(pvRes[i].vt == VT_LPSTR)
        {
            iFldLen = strlen(pvRes[i].pcVal);
            if(iFldLen > 0)
            {
                ilen += (iFldLen + 12);
                if(pRow->pvData[i + 1].vt == VT_LPSTR)
                    ppParams[nParams] = WCharToDBStr(pRow->pvData[i + 1].bstrVal,
                        m_pConnStruct->lDBEnc);
                else ppParams[nParams] = NULL;
                nParams++;
            }
        }
    }

    LPSTR sTblName = pvRes[1].pcVal;
    LPSTR sSchema = NULL;
    LPSTR sDot = strchr(sTblName, '.');
    if(sDot)
    {
        iFldLen = sDot - sTblName;
        sSchema = (LPSTR)malloc((iFldLen + 1)*sizeof(char));
        strncpy(sSchema, sTblName, iFldLen);
        sSchema[iFldLen] = 0;
        sTblName = sDot + 1;
    }

    char sNumBuf[32];
    int iCtidCol = 0;
    sSql = (LPSTR)malloc(ilen*sizeof(char));
    if(sSchema)
        sprintf(sSql, "select ctid, * from \"%s\".\"%s\" where \"%s\" = $1",
            sSchema, sTblName, pvRes[2].pcVal);
    else
        sprintf(sSql, "select ctid, * from \"%s\" where \"%s\" = $1", sTblName,
            pvRes[2].pcVal);
    for(int i = 3; i < 12; i++)
    {
        if(pvRes[i].vt == VT_LPSTR)
        {
            strcat(sSql, " and \"");
            strcat(sSql, pvRes[i].pcVal);
            sprintf(sNumBuf, "\" = $%d", i - 1);
            strcat(sSql, sNumBuf);
        }
    }

    SetSP(m_pConnStruct->pConn, bTrans);
    res = PQexecParams(m_pConnStruct->pConn, sSql, nParams, NULL, ppParams,
        NULL, NULL, 1);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);

        iCtidCol = -1;
        if(sSchema)
            sprintf(sSql, "select * from \"%s\".\"%s\" where \"%s\" = $1",
                sSchema, sTblName, pvRes[2].pcVal);
        else
            sprintf(sSql, "select * from \"%s\" where \"%s\" = $1", sTblName,
                pvRes[2].pcVal);
        for(int i = 3; i < 12; i++)
        {
            if(pvRes[i].vt == VT_LPSTR)
            {
                strcat(sSql, " and \"");
                strcat(sSql, pvRes[i].pcVal);
                sprintf(sNumBuf, "\" = $%d", i - 1);
                strcat(sSql, sNumBuf);
            }
        }

        SetSP(m_pConnStruct->pConn, bTrans);
        res = PQexecParams(m_pConnStruct->pConn, sSql, nParams, NULL, ppParams,
            NULL, NULL, 1);
    }

    free(sSql);
    for(int i = 0; i < nParams; i++)
    {
        if(ppParams[i]) free(ppParams[i]);
    }

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        WritePQErrorToLog("Spot 88: ", m_pConnStruct->pConn);
        return;
    }

    nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
        return;
    }

    int iModType = VarToInt(pRow->pvData[1]);

    int ncols = PQnfields(res);
    Oid *plTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, plTypes);

    GFields *pFields = new GFields(NULL, m_pTypeLib);
    pFields->AddRef();
    pFields->BuildFromConn(m_pConnStruct, res, m_pGTbls, iCtidCol, plTypes,
        0, false);

    int *piGeomDims = (int*)malloc(pFields->GetCount()*sizeof(int));
    pFields->UpdateFields(piGeomDims);

    int iCtidLen = 0;
    char sCtid[16];
    int iRowSize = ncols;
    if(iCtidCol > -1) iRowSize--;
    VARIANT *pvData = (VARIANT*)malloc(iRowSize*sizeof(VARIANT));
    for(int i = 0; i < iRowSize; i++) VariantInit(&pvData[i]);

    if(iCtidCol < 0)
        GetRecordBin(res, 0, ncols, plTypes, pvData, m_pConnStruct->lDBEnc,
            m_pConnStruct->lGeomOid, piGeomDims, m_pConnStruct->iCurrDigits);
    else
        GetRecordCtid(res, 0, ncols, iCtidCol, plTypes, pvData,
            m_pConnStruct->lDBEnc, sCtid, &iCtidLen, m_pConnStruct->lGeomOid,
            piGeomDims, m_pConnStruct->iCurrDigits);

    GRecordset *pRS;
    for(int i = 0; i < m_iDataLen; i++)
    {
        pRS = (GRecordset*)m_pData[i];
        if(pRS) pRS->UpdateWithDBChanges(iModType, pvRes, pvData, res, sSchema,
            sTblName, iCtidCol > -1, iCtidLen, sCtid);
    }

    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    if(sSchema) free(sSchema);

    for(int i = 0; i < iRowSize; i++) VariantClear(&pvData[i]);
    free(pvData);

    free(piGeomDims);
    pFields->Release();
    free(plTypes);

    for(int i = 0; i < 12; i++)
    {
        if(pvRes[i].vt == VT_LPSTR) free(pvRes[i].pcVal);
    }
    return;
}

void GRecordsets::BroadcastDataChanges(GRecordset* pRS)
{
    long lRecords = pRS->GetBufSize();
    int iFlds = pRS->GetFieldsCount();
    CBufRow cRow;
    cRow.pvData = (VARIANT*)malloc(iFlds*sizeof(VARIANT));
    for(int i = 0; i < iFlds; i++) VariantInit(&cRow.pvData[i]);

    for(long l = 0; l < lRecords; l++)
    {
        if(pRS->GetRow(l, &cRow)) ProcessModLogRec(&cRow);
    }

    for(int i = 0; i < iFlds; i++) VariantClear(&cRow.pvData[i]);
    free(cRow.pvData);
    return;
}*/
