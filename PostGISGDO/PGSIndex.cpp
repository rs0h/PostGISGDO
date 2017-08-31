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

#include "PGSIndex.hpp"
#include <stdio.h>
#include "PGSCols.hpp"
#include "../Common/LogFiles.hpp"
#include "../Common/PGtoGDOmaps.hpp"
#include "../Common/GDOUtils.hpp"
#include "../Common/VarUtils.hpp"
#include "../Common/pg_const.h"
#include "PostGIS.rh"

LPWSTR GetValidFieldName(BSTR bsName, long lEnc, LPSTR *psName)
{
    int iLen = wcslen(bsName);
    LPWSTR wsRes = (LPWSTR)malloc((iLen + 1)*sizeof(wchar_t));
    wcscpy(wsRes, bsName);
    *psName = WCharToDBStr(wsRes, lEnc);
    return(wsRes);
}

int GetTableNameLen(LPSTR sSchema, LPSTR sTable)
{
    int iRes = strlen(sTable) + 3;
    if(sSchema) iRes += (strlen(sSchema) + 4);
    return(iRes);
}

void CatTableName(LPSTR sCmd, LPSTR sSchema, LPSTR sTable, bool bDecorate)
{
    if(bDecorate)
    {
        if(sSchema)
        {
            strcat(sCmd, "\"");
            strcat(sCmd, sSchema);
            strcat(sCmd, "\"");
            strcat(sCmd, ".");
        }
        strcat(sCmd, "\"");
        strcat(sCmd, sTable);
        strcat(sCmd, "\"");
    }
    else
    {
        if(sSchema)
        {
            strcat(sCmd, sSchema);
            strcat(sCmd, ".");
        }
        strcat(sCmd, sTable);
    }
    return;
}

void CatFieldName(LPSTR sCmd, LPSTR sField, bool bDecorate)
{
    if(bDecorate)
    {
        strcat(sCmd, "\"");
        strcat(sCmd, sField);
        strcat(sCmd, "\"");
    }
    else strcat(sCmd, sField);
    return;
}

void InitFldAttrs(PFldAttrs pfa)
{
    pfa->lAttr = 0;
    pfa->iType = 0;
    pfa->lSize = 0;
    pfa->lSubType = 0;
    pfa->lColOrder = 0;
    pfa->sDefValDef = NULL;
    pfa->wsDefVal = NULL;
    pfa->bRequired = false;
    pfa->bUpdatable = false;
    pfa->iGeomDim = 0;
    pfa->iPsgType = 0;
    pfa->ulSrid = 0;
    pfa->sCSGuid[0] = 0;
    pfa->lTblOid = 0;
    pfa->iTblPos = 0;
    pfa->lTypeOid = 0;
    pfa->sOrigSchema = NULL;
    pfa->sOrigTable = NULL;
    pfa->wsTblName = NULL;
    pfa->lBaseTblOid = 0;
    pfa->iBaseTblPos = -1;
    return;
}

void ClearFldAttrs(PFldAttrs pfa)
{
    if(pfa->sDefValDef) free(pfa->sDefValDef);
    pfa->sDefValDef = NULL;
    if(pfa->wsDefVal) free(pfa->wsDefVal);
    pfa->wsDefVal = NULL;
    pfa->sCSGuid[0] = 0;
    return;
}


// FldLitConvExt

FldLitConvExt::FldLitConvExt(IUnknown *pUnkOuter, ITypeLib *ALib) :
    CCOMDispatch(pUnkOuter, ALib, 0)
{
#if DBGLEVEL > 1
    WriteLogFile("FldLitConvExt::FldLitConvExt-1\r\n", true);
#endif // DBGLEVEL
}

FldLitConvExt::FldLitConvExt(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("FldLitConvExt::FldLitConvExt-2\r\n", true);
#endif // DBGLEVEL
}

FldLitConvExt::~FldLitConvExt()
{
#if DBGLEVEL > 1
    WriteLogFile("FldLitConvExt::~FldLitConvExt\r\n", true);
#endif // DBGLEVEL
}

HRESULT FldLitConvExt::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("FldLitConvExt::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_LiterarConversionExtension))
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

ULONG FldLitConvExt::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "FldLitConvExt::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "FldLitConvExt::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT FldLitConvExt::ConvertToLiteral(VARIANT DataValue, BSTR *LiteralString)
{
#if DBGLEVEL > 0
    WriteLogFile("FldLitConvExt::ConvertToLiteral\r\n", true);
    WriteVariantToLogFile(L"  Data: ", DataValue);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT FldLitConvExt::ConvertFromLiteral(BSTR LiteralString, VARIANT *DataValue)
{
#if DBGLEVEL > 0
    WriteLogFile("FldLitConvExt::ConvertFromLiteral\r\n  Value: ", true);
    if(LiteralString) WriteLogFile(LiteralString, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(!DataValue) return(E_POINTER);
    VariantClear(DataValue);
    DataValue->vt = VT_NULL;
    if(!LiteralString) return(S_OK);

    long lVal = 0;
    if(swscanf(LiteralString, L"%d", &lVal) == 1)
    {
        DataValue->vt = VT_I4;
        DataValue->lVal = lVal;
    }
    return(S_OK);
}


//  GTDField

GTDField::GTDField(IUnknown *pUnkOuter, ITypeLib *pLib) : CCOMDispatch(pUnkOuter, pLib, 20)
{
#if DBGLEVEL > 1
    WriteLogFile("GTDField::GTDField-1\r\n", true);
#endif // DBGLEVEL
    m_iFldType = 0;
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_iStatus = cisUnknown;
    m_bHasDef = false;
    m_bIsKey = false;
    m_pLitConv = NULL;
    m_iSpecType = 0;
    m_sTmpDefVal = NULL;
    InitFldAttrs(&m_cAttrs);
}

GTDField::GTDField(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex) :
    CCOMDispatch(pUnkOuter, pLib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GTDField::GTDField-2\r\n", true);
#endif // DBGLEVEL
    m_iFldType = 0;
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_iStatus = cisUnknown;
    m_bHasDef = false;
    m_bIsKey = false;
    m_pLitConv = NULL;
    m_iSpecType = 0;
    m_sTmpDefVal = NULL;
    InitFldAttrs(&m_cAttrs);
}

GTDField::~GTDField()
{
#if DBGLEVEL > 1
    WriteLogFile("GTDField::~GTDField\r\n", true);
#endif // DBGLEVEL
    ClearFldAttrs(&m_cAttrs);
    if(m_sOrigName) free(m_sOrigName);
    if(m_wsName) free(m_wsName);
    if(m_pLitConv) m_pLitConv->SetOuter(NULL);
    if(m_sTmpDefVal) free(m_sTmpDefVal);
}

HRESULT GTDField::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GTDField::QueryInterface\r\n", true);
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

ULONG GTDField::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GTDField::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GTDField::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GTDField::get_Value(VARIANT *pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Value\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT GTDField::set_Value(VARIANT pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Value\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT GTDField::get_AllowZeroLength(VARIANT_BOOL *pAllow)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_AllowZeroLength\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1) return(S_FALSE);
    if(!pAllow) return(S_FALSE);
    *pAllow = VARIANT_TRUE;
    return(S_OK);
}

HRESULT GTDField::set_AllowZeroLength(VARIANT_BOOL pAllow)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_AllowZeroLength\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1) return(S_FALSE);
    return(S_OK);
}

HRESULT GTDField::get_Attributes(long *pAttr)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Attributes\r\n", true);
#endif // DBGLEVEL
    if(!pAttr) return(E_POINTER);
    *pAttr = m_cAttrs.lAttr;
    return(S_OK);
}

HRESULT GTDField::set_Attributes(long pAttr)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Attributes\r\n", true);
#endif // DBGLEVEL
    if(m_cAttrs.lAttr == pAttr) return(S_OK);

    if(m_iStatus == cisCreated)
    {
        if(m_iFldType > 1)  // parent is Index, we don't allow to
            // do that the user must drop index and recreate, if he wants to
            // alter it
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_FLDMODONIDX, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        bool bRes = true;
        int slen;
        LPSTR sCmd;
        if(m_cAttrs.iType == gdbSpatial)
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_GEOMCOLCHANGE, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }
        else
        {
            char sType[128];
            long lOldAttr = m_cAttrs.lAttr;
            m_cAttrs.lAttr = pAttr;
            GetSQLTypeStr(sType);
            m_cAttrs.lAttr = lOldAttr;
            slen = 36 + GetTableNameLen(m_cAttrs.sOrigSchema,
                m_cAttrs.sOrigTable) + strlen(m_sOrigName) + strlen(sType);
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            strcpy(sCmd, "ALTER TABLE ");
            CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, true);
            strcat(sCmd, " ALTER COLUMN ");
            CatFieldName(sCmd, m_sOrigName, true);
            strcat(sCmd, " TYPE ");
            strcat(sCmd, sType);
            bRes = ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);
        }
        if(!bRes) return(S_FALSE);

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_cAttrs.wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    m_cAttrs.lAttr = pAttr;
    if(m_cAttrs.lAttr & 16)
    {
        m_cAttrs.bUpdatable = false;
        if(m_cAttrs.sDefValDef) free(m_cAttrs.sDefValDef);
        m_cAttrs.sDefValDef = NULL;

        if(m_cAttrs.sOrigTable && m_sOrigName)
        {
            LPSTR sSequence = GetSequenceName(m_cAttrs.sOrigSchema,
                m_cAttrs.sOrigTable, m_sOrigName);
            int ilen = strlen(sSequence) + 14;
            m_cAttrs.sDefValDef = (LPSTR)malloc(ilen*sizeof(char));
            sprintf(m_cAttrs.sDefValDef, "nextval('%s')", sSequence);
            free(sSequence);
        }
    }
    else m_cAttrs.bUpdatable = true;
    return(S_OK);
}

HRESULT GTDField::get_CollatingOrder(long *pOrder)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_CollatingOrder\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!pOrder) return(E_POINTER);
    *pOrder = m_cAttrs.lColOrder;
    return(S_OK);
}

HRESULT GTDField::get_CoordSystemGUID(VARIANT *CSGuid)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_CoordSystemGUID\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType < gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMPROPONLY, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!CSGuid) return(E_POINTER);
    VariantClear(CSGuid);

    if(m_cAttrs.sCSGuid[0])
    {
        CSGuid->vt = VT_BSTR;
        CSGuid->bstrVal = SysAllocString(m_cAttrs.sCSGuid);
    }
    else
    {
        VARIANT *pRow = FindCSTableRow(&m_pConnStruct->cCSTable, m_cAttrs.ulSrid);
        if(pRow) VariantCopy(CSGuid, &pRow[1]);
        else CSGuid->vt = VT_NULL;
    }

    return(S_OK);
}

unsigned long CSGUIDToSrid(PConnStruct pConnStruct, LPWSTR sCSGUID)
{
    unsigned long ulRes = 0;
    VARIANT *pRow = FindCSTableRowByCSGUID(&pConnStruct->cCSTable, sCSGUID);

    if(!pRow)
    {
        GErrors *pErrors = (GErrors*)pConnStruct->pErrors;

        bool bTrans = (pConnStruct->iSysTrans | pConnStruct->iGdoTrans);

        char sCSGUIDBuf[40];
        BStrToCharBuf(sCSGUID, sCSGUIDBuf, 40);
        LPSTR psParams[1] = {sCSGUIDBuf};
        int iParLen = strlen(sCSGUIDBuf);
        int iParType = 0;

        char sBuf[128];
        sprintf(sBuf, "select * from %s where csguid = $1",
            pConnStruct->sCoordSystems);

        SetSP(pConnStruct->pConn, bTrans);
        //LPSTR lsCmd = GetSaveSQL(sBuf, bTrans);
        PGresult *res = PQexecParams(pConnStruct->pConn, sBuf, 1, NULL,
            psParams, &iParLen, &iParType, 1);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            pErrors->HandlePQError(pConnStruct->pConn, pConnStruct->hInstance);
            PQclear(res);
            RollbackSP(pConnStruct->pConn, bTrans);
            return(0);
        }

        int nrows = PQntuples(res);
        if(nrows < 1)
        {
            PQclear(res);
            ReleaseSP(pConnStruct->pConn, bTrans);
            return(0);
        }
        int ncols = PQnfields(res);
        Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
        DescribeFieldTypes(res, ncols, piTypes);

        VARIANT *pvCSRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
        for(int i = 0; i < ncols; i++) VariantInit(&pvCSRow[i]);
        GetRecordBin(res, 0, ncols, piTypes, pvCSRow, pConnStruct->lDBEnc,
            0, NULL, pConnStruct->iCurrDigits);
        PQclear(res);
        ReleaseSP(pConnStruct->pConn, bTrans);

        pRow = FindCSTableRowByCS(&pConnStruct->cCSTable, pvCSRow);
        if(pRow)
        {
            if(pRow[1].vt < VT_I2) VariantCopy(&pRow[1], &pvCSRow[0]);
        }
        for(int i = 0; i < ncols; i++) VariantClear(&pvCSRow[i]);
        free(pvCSRow);
        free(piTypes);
    }

    if(pRow)
    {
        int ires = 0;
        swscanf(pRow[0].bstrVal, L"%d", &ires);
        ulRes = ires;
    }

    return(ulRes);
}

HRESULT GTDField::set_CoordSystemGUID(VARIANT CSGuid)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_CoordSystemGUID\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType < gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMPROPONLY, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMCOLCHANGE, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    ValidateVariant(&CSGuid);

    if(CSGuid.vt == VT_BSTR)
    {
        int ilen = wcslen(CSGuid.bstrVal);
        if(ilen > 39) ilen = 39;
        wcsncpy(m_cAttrs.sCSGuid, CSGuid.bstrVal, ilen);
        m_cAttrs.sCSGuid[ilen] = 0;
        m_cAttrs.ulSrid = CSGUIDToSrid(m_pConnStruct, m_cAttrs.sCSGuid);
    }
    else m_cAttrs.ulSrid = 0;
    return(S_OK);
}

HRESULT GTDField::get_DataUpdatable(VARIANT_BOOL *pUpdatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_DataUpdatable\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!pUpdatable) return(E_POINTER);
    if(m_cAttrs.bUpdatable) *pUpdatable = VARIANT_TRUE;
    else *pUpdatable = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GTDField::set_DefaultValue(BSTR pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_DefaultValue\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType >= gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTGEOMLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!m_cAttrs.wsDefVal && !pValue) return(S_OK);

    LPSTR sCmd;
    int slen;
    bool bRes = true;
    if(pValue)
    {
        if(m_cAttrs.wsDefVal)
        {
            if(wcscmp(m_cAttrs.wsDefVal, pValue) == 0) return(S_OK);
        }

        int ilen = wcslen(pValue);
        if(ilen > 0)
        {
            if(m_iStatus == cisCreated)
            {
                LPSTR sNewValue = WCharToDBStr(pValue, m_pConnStruct->lDBEnc);
                slen = 52 + GetTableNameLen(m_cAttrs.sOrigSchema,
                    m_cAttrs.sOrigTable) + strlen(m_sOrigName) +
                    strlen(sNewValue);
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                strcpy(sCmd, "ALTER TABLE ");
                CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable,
                    true);
                strcat(sCmd, " ALTER COLUMN ");
                CatFieldName(sCmd, m_sOrigName, true);
                strcat(sCmd, " SET DEFAULT '");
                strcat(sCmd, sNewValue);
                strcat(sCmd, "'");
                bRes = ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);
                free(sNewValue);
            }
            if(bRes)
            {
                if(m_cAttrs.wsDefVal) free(m_cAttrs.wsDefVal);
                m_cAttrs.wsDefVal = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
                wcscpy(m_cAttrs.wsDefVal, pValue);
                if(m_cAttrs.sDefValDef) free(m_cAttrs.sDefValDef);
                m_cAttrs.sDefValDef = NULL;
            }
            else return(S_FALSE);
        }
        else if(m_cAttrs.wsDefVal)
        {
            if(m_iStatus == cisCreated)
            {
                slen = 48 + GetTableNameLen(m_cAttrs.sOrigSchema,
                    m_cAttrs.sOrigTable) + strlen(m_sOrigName);
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                strcpy(sCmd, "ALTER TABLE ");
                CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable,
                    true);
                strcat(sCmd, " ALTER COLUMN ");
                CatFieldName(sCmd, m_sOrigName, true);
                strcat(sCmd, " DROP DEFAULT");
                bRes = ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);
            }
            if(bRes)
            {
                free(m_cAttrs.wsDefVal);
                m_cAttrs.wsDefVal = NULL;
                if(m_cAttrs.sDefValDef) free(m_cAttrs.sDefValDef);
                m_cAttrs.sDefValDef = NULL;
            }
            else return(S_FALSE);
        }
    }
    else
    {
        if(m_cAttrs.wsDefVal)
        {
            if(m_iStatus == cisCreated)
            {
                slen = 48 + GetTableNameLen(m_cAttrs.sOrigSchema,
                    m_cAttrs.sOrigTable) + strlen(m_sOrigName);
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                strcpy(sCmd, "ALTER TABLE ");
                CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable,
                    true);
                strcat(sCmd, " ALTER COLUMN ");
                CatFieldName(sCmd, m_sOrigName, true);
                strcat(sCmd, " DROP DEFAULT");
                bRes = ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);
            }
            if(bRes)
            {
                free(m_cAttrs.wsDefVal);
                m_cAttrs.wsDefVal = NULL;
                if(m_cAttrs.sDefValDef) free(m_cAttrs.sDefValDef);
                m_cAttrs.sDefValDef = NULL;
            }
            else return(S_FALSE);
        }
    }
    return(S_OK);
}

HRESULT GTDField::get_DefaultValue(BSTR *pValue)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_DefaultValue\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType >= gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTGEOMLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!pValue) return(E_POINTER);
    if(*pValue) SysFreeString(*pValue);
    *pValue = NULL;
    if(m_cAttrs.wsDefVal) *pValue = SysAllocString(m_cAttrs.wsDefVal);
    return(S_OK);
}

HRESULT GTDField::get_Name(BSTR *pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Name\r\n", true);
#endif // DBGLEVEL
    if(!pName) return(E_POINTER);
    if(*pName) SysFreeString(*pName);
    *pName = SysAllocString(m_wsName);
    return(S_OK);
}

HRESULT GTDField::set_Name(BSTR pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Name\r\n", true);
#endif // DBGLEVEL
    LPSTR sNewName = NULL;
    LPWSTR wsNewName = GetValidFieldName(pName, m_pConnStruct->lDBEnc, &sNewName);

    if(m_wsName)
    {
        if(wcscmp(m_wsName, wsNewName) == 0)
        {
            free(sNewName);
            free(wsNewName);
            return(S_OK);
        }
    }

    if(m_iStatus == cisCreated)
    {
        if(m_iFldType > 1)  // parent is Index, we don't allow to
            // do that the user must drop index and recreate, if he wants to
            // alter it
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_FLDMODONIDX, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        int slen = 44 + GetTableNameLen(m_cAttrs.sOrigSchema,
            m_cAttrs.sOrigTable) + strlen(m_sOrigName) + strlen(sNewName);
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "ALTER TABLE ");
        CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, true);
        strcat(sCmd, " RENAME COLUMN ");
        CatFieldName(sCmd, m_sOrigName, true);
        strcat(sCmd, " TO ");
        CatFieldName(sCmd, sNewName, true);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes)
        {
            free(sNewName);
            free(wsNewName);
            return(S_FALSE);
        }

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_cAttrs.wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    if(m_wsName) free(m_wsName);
    m_wsName = wsNewName;
    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = sNewName;
    return(S_OK);
}

HRESULT GTDField::get_Required(VARIANT_BOOL *pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Required\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if(!pRequired) return(E_POINTER);
    if(m_cAttrs.bRequired) *pRequired = VARIANT_TRUE;
    else *pRequired = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GTDField::set_Required(VARIANT_BOOL pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Required\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if((m_cAttrs.bRequired && pRequired) || !(m_cAttrs.bRequired || pRequired))
        return(S_OK);

    if(m_iStatus == cisCreated)
    {
        int slen = 48 + GetTableNameLen(m_cAttrs.sOrigSchema,
            m_cAttrs.sOrigTable) + strlen(m_sOrigName);
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "ALTER TABLE ");
        CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, true);
        strcat(sCmd, " ALTER COLUMN ");
        CatFieldName(sCmd, m_sOrigName, true);
        if(pRequired) strcat(sCmd, " SET NOT NULL");
        else strcat(sCmd, " DROP NOT NULL");
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes) return(S_FALSE);
    }

    m_cAttrs.bRequired = pRequired;
    return(S_OK);
}

HRESULT GTDField::get_Size(long *pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Size\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if(!pSize) return(E_POINTER);
    *pSize = m_cAttrs.lSize;
    return(S_OK);
}

HRESULT GTDField::set_Size(long pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Size\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if(m_cAttrs.lSize == pSize) return(S_OK);

    if(m_iStatus == cisCreated)
    {
        if(m_cAttrs.iType == gdbSpatial)
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_GEOMCOLCHANGE, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        char sType[128];
        long lOldSize = m_cAttrs.lSize;
        m_cAttrs.lSize = pSize;
        GetSQLTypeStr(sType);
        m_cAttrs.lSize = lOldSize;
        int slen = 36 + GetTableNameLen(m_cAttrs.sOrigSchema,
            m_cAttrs.sOrigTable) + strlen(m_sOrigName) + strlen(sType);
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "ALTER TABLE ");
        CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, true);
        strcat(sCmd, " ALTER COLUMN ");
        CatFieldName(sCmd, m_sOrigName, true);
        strcat(sCmd, " TYPE ");
        strcat(sCmd, sType);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes) return(S_FALSE);

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_cAttrs.wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    m_cAttrs.lSize = pSize;
    return(S_OK);
}

HRESULT GTDField::get_SourceDatabase(BSTR *pDatabase)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_SourceDatabase\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1) return(S_FALSE);
    if(!pDatabase) return(E_POINTER);
    if(*pDatabase) SysFreeString(*pDatabase);
    *pDatabase = SysAllocString(m_pConnStruct->wsServerName);
    return(S_OK);
}

HRESULT GTDField::get_SourceField(BSTR *pField)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_SourceField\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if(!pField) return(E_POINTER);
    if(*pField) SysFreeString(*pField);
    *pField = SysAllocString(m_wsName);
    return(S_OK);
}

HRESULT GTDField::get_SourceTable(BSTR *pTable)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_SourceTable\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if(!pTable) return(E_POINTER);
    if(*pTable) SysFreeString(*pTable);
    *pTable = SysAllocString(m_cAttrs.wsTblName);
    return(S_OK);
}

HRESULT GTDField::get_SubType(long *pGeometryType)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_SubType\r\n", true);
    char buf[32];
    sprintf(buf, "  SubType: %ld\r\n", m_cAttrs.lSubType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType < gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMPROPONLY, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!pGeometryType) return(E_POINTER);
    *pGeometryType = m_cAttrs.lSubType;
    return(S_OK);
}

HRESULT GTDField::set_SubType(long pGeometryType)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_SubType\r\n", true);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType < gdbSpatial)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMPROPONLY, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_GEOMCOLCHANGE, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.lSubType == pGeometryType) return(S_OK);
    m_cAttrs.lSubType = pGeometryType;

    switch(m_cAttrs.lSubType)
    {
    case gdbPoint:
        m_cAttrs.iPsgType = 4;
        break;
    case gdbLinear:
        m_cAttrs.iPsgType = 5;
        break;
    case gdbAreal:
        m_cAttrs.iPsgType = 6;
        break;
    case gdbAnySpatial:
        m_cAttrs.iPsgType = 7;
        break;
    case gdbCoverage:
        m_cAttrs.iPsgType = 21;
        break;
    case gdbGraphicsText:
        m_cAttrs.iPsgType = 20;
        break;
    default:
        m_cAttrs.iPsgType = 0;
    }
    m_cAttrs.iGeomDim = 3;

    return(S_OK);
}

HRESULT GTDField::get_Type(short *pType)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::get_Type\r\n", true);
    char buf[32];
    sprintf(buf, "  Type: %d\r\n", m_cAttrs.iType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(!pType) return(E_POINTER);
    *pType = m_cAttrs.iType;
    return(S_OK);
}

HRESULT GTDField::set_Type(short pType)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::set_Type (", true);
    WriteLogFile(m_wsName, false);
    char buf[64];
    sprintf(buf, "\r\n  Value: %d\r\n", pType);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
    if(m_iFldType > 1)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_NOTINDEXFLDPROP, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(m_cAttrs.iType == pType) return(S_OK);

    if(m_iStatus == cisCreated)
    {
        if(m_cAttrs.iType == gdbSpatial)
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_GEOMCOLCHANGE, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        char sType[128];
        int iOldType = m_cAttrs.iType;
        m_cAttrs.iType = pType;
        GetSQLTypeStr(sType);
        m_cAttrs.iType = iOldType;
        int slen = 36 + GetTableNameLen(m_cAttrs.sOrigSchema,
            m_cAttrs.sOrigTable) + strlen(m_sOrigName) + strlen(sType);
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "ALTER TABLE ");
        CatTableName(sCmd, m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, true);
        strcat(sCmd, " ALTER COLUMN ");
        CatFieldName(sCmd, m_sOrigName, true);
        strcat(sCmd, " TYPE ");
        strcat(sCmd, sType);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes) return(S_FALSE);

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_cAttrs.wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    m_cAttrs.iType = pType;
    m_cAttrs.lTypeOid = GdoTypeToOid(m_cAttrs.iType, m_pConnStruct->lGeomOid);
    return(S_OK);
}

HRESULT GTDField::AppendChunk(VARIANT Val)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::AppendChunk\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT GTDField::FieldSize(long *pSize)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::FieldSize\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT GTDField::GetChunk(long Offset, long Bytes, VARIANT *pChunk)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::GetChunk\r\n", true);
#endif // DBGLEVEL
    return(S_FALSE);
}

HRESULT GTDField::GetExtension(BSTR Name, IDispatch * *ppGExtension)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDField::GetExtension\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(wcsicmp(Name, L"DefaultValueLiteralConversion") == 0)
    {
        if(!m_pLitConv) //return(E_NOTIMPL);
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
    return(E_NOTIMPL);
}

LPSTR GTDField::GetOrigNamePtr()
{
    return(m_sOrigName);
}

void GTDField::SetNewProps(PConnStruct pConnStruct, VARIANT Name, VARIANT Type,
    VARIANT Size, int iFldType, LPSTR sOrigSchema, LPSTR sOrigTable,
    LPWSTR wsTblName)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iFldType = iFldType;
    m_iStatus = cisNew;
    if(Name.vt == VT_BSTR)
    {
        if(m_wsName) free(m_wsName);
        if(m_sOrigName) free(m_sOrigName);
        m_wsName = GetValidFieldName(Name.bstrVal, pConnStruct->lDBEnc,
            &m_sOrigName);
    }
    else if(Name.vt == (VT_BSTR | VT_BYREF))
    {
        if(m_wsName) free(m_wsName);
        if(m_sOrigName) free(m_sOrigName);
        m_wsName = GetValidFieldName(*Name.pbstrVal, pConnStruct->lDBEnc,
            &m_sOrigName);
    }
    m_cAttrs.iType = VarToInt(Type);
    m_cAttrs.lSize = VarToLong(Size);
    m_cAttrs.lTypeOid = GdoTypeToOid(m_cAttrs.iType, m_pConnStruct->lGeomOid);
    m_cAttrs.sOrigSchema = sOrigSchema;
    m_cAttrs.sOrigTable = sOrigTable;
    m_cAttrs.wsTblName = wsTblName;
    return;
}

void GTDField::CheckDefault()
{
    m_cAttrs.bUpdatable = true;
    m_cAttrs.lAttr |= 32;

    if(!m_bHasDef && (m_cAttrs.iBaseTblPos < 1)) return;

    if(m_cAttrs.iBaseTblPos > 0)
    {
        bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

        SetSP(m_pConnStruct->pConn, bTrans);
        char sParams[32];
        sprintf(sParams, "%d", m_cAttrs.lBaseTblOid);
        sprintf(&sParams[16], "%d", m_cAttrs.iBaseTblPos);

        LPSTR pParams[2];
        pParams[0] = sParams;
        pParams[1] = &sParams[16];

        //LPSTR lsCmd = GetSaveSQL("select adsrc from pg_attrdef where adrelid = $1 and adnum = $2",
        //    bTrans);
        PGresult *res = PQexecParams(m_pConnStruct->pConn,
            "select adsrc from pg_attrdef where adrelid = $1 and adnum = $2", 2, NULL, pParams, NULL, NULL, 0);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Spot 7: ", m_pConnStruct->pConn);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
            return;
        }

        long lrows = PQntuples(res);
        if(lrows < 1)
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
            return;
        }

        if(PQgetisnull(res, 0, 0))
        {
            PQclear(res);
            ReleaseSP(m_pConnStruct->pConn, bTrans);
            return;
        }

        Oid lType;
        VARIANT vVal;
        VariantInit(&vVal);
        DescribeFieldTypes(res, 1, &lType);
        GetRecord(res, 0, 1, &lType, &vVal, m_pConnStruct->sDecSep);

        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);

        if(m_sTmpDefVal) free(m_sTmpDefVal);
        m_sTmpDefVal = NULL;
        if(vVal.vt == VT_LPSTR)
        {
            m_sTmpDefVal = (LPSTR)malloc((strlen(vVal.pcVal) + 1)*sizeof(char));
            strcpy(m_sTmpDefVal, vVal.pcVal);
        }
    }

    if(m_cAttrs.wsDefVal) free(m_cAttrs.wsDefVal);
    m_cAttrs.wsDefVal = NULL;

    if(!m_sTmpDefVal) return;

    if(m_cAttrs.sDefValDef) free(m_cAttrs.sDefValDef);
    m_cAttrs.sDefValDef = NULL;

    if(strncmp(m_sTmpDefVal, "nextval", 7) == 0)
    {
        m_cAttrs.bUpdatable = false;
        m_cAttrs.lAttr &= ~32;
        m_cAttrs.sDefValDef = m_sTmpDefVal;
        LPSTR sFirstQuote = strchr(m_cAttrs.sDefValDef, '\'');
        if(sFirstQuote)
        {
            LPSTR sSeqName = GetSequenceName(m_cAttrs.sOrigSchema,
                m_cAttrs.sOrigTable, m_sOrigName);
            sFirstQuote++;
            if(strncmp(sFirstQuote, sSeqName, strlen(sSeqName)) == 0)
            {
                m_cAttrs.lAttr |= 16;
            }
            free(sSeqName);
        }
    }
    else
    {
        BSTR bsBuf = DBStrToBSTR(m_sTmpDefVal, m_pConnStruct->lDBEnc);

        LPWSTR sFirstQuote = wcschr(bsBuf, '\'');
        LPWSTR sSecQuote = NULL;
        if(sFirstQuote)
        {
            sFirstQuote++;
            sSecQuote = wcschr(sFirstQuote, '\'');
        }
        if(sSecQuote)
        {
            int slen = sSecQuote - sFirstQuote;
            m_cAttrs.wsDefVal = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
            wcsncpy(m_cAttrs.wsDefVal, sFirstQuote, slen);
            m_cAttrs.wsDefVal[slen] = 0;
        }
        else
        {
            int slen = wcslen(bsBuf);
            m_cAttrs.wsDefVal = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
            wcscpy(m_cAttrs.wsDefVal, bsBuf);
        }

        SysFreeString(bsBuf);
        free(m_sTmpDefVal);

        if(m_cAttrs.wsDefVal)
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
    }
    m_sTmpDefVal = NULL;
    return;
}

void GTDField::UpdateAttrs()
{
    long lTmpSubType = 0;
    wchar_t wsTmpCsGuid[40];

    switch(m_cAttrs.iType)
    {
    //case gdbText:
    //case gdbMemo:
    case gdbLongBinary:
        if(m_pConnStruct->bGeomPrepared)
        {
            lTmpSubType = GetGeometryType(m_pConnStruct,
                m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName,
                &m_cAttrs.iGeomDim, &m_cAttrs.ulSrid, &m_cAttrs.iPsgType,
                wsTmpCsGuid);
        }
        switch(lTmpSubType)
        {
        case 0:
            break;
        case 5:
            m_cAttrs.iType = 33;
            break;
        default:
            m_cAttrs.iType = 32;
        }
        break;
    }

    CheckDefault();

    if(m_cAttrs.lSubType > 0)
    {
        if(m_cAttrs.iType < 32)
        {
            m_cAttrs.ulSrid = SRIDfromCSGUID(&m_pConnStruct->cCSTable,
                m_cAttrs.sCSGuid);
            if(m_cAttrs.lSubType == 5)
            {
                m_cAttrs.iType = 33;
                m_cAttrs.iPsgType = 20;
            }
            else
            {
                m_cAttrs.iType = 32;
                m_cAttrs.iPsgType = 21;
            }
        }
    }
    else if(lTmpSubType > 0)
    {
        m_cAttrs.lSubType = lTmpSubType;
        wcscpy(m_cAttrs.sCSGuid, wsTmpCsGuid);
    }

    if(m_cAttrs.iType < 10) m_cAttrs.lAttr |= 1;
    else switch(m_cAttrs.iType)
    {
    case 10:
    case 11:
    case 12:
    case 32:
    case 33:
        m_cAttrs.lAttr |= 2;
        break;
    }
    return;
}

void GTDField::SetTDProps(PConnStruct pConnStruct, LPCSTR sName, int iColPos,
    Oid lTypeOid, int iSize, long lMod, bool bRequired, bool bHasDef,
    Oid lTblOid, LPSTR sOrigSchema, LPSTR sOrigTable, LPWSTR wsTblName,
    PGresult *pViewRes, LPSTR sDefault, bool bExtInfo, bool bIsKey,
    long lGeomType, LPSTR sCSGuid)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iFldType = 1;
    m_iStatus = cisCreated;
    if(m_sOrigName) free(m_sOrigName);
    if(m_wsName) free(m_wsName);

    int iLen = strlen(sName);
    m_sOrigName = (LPSTR)malloc((iLen + 1)*sizeof(char));
    strcpy(m_sOrigName, sName);

    m_wsName = DBStrToWChar(sName, m_pConnStruct->lDBEnc);
    m_cAttrs.iTblPos = iColPos;
    m_cAttrs.lTblOid = lTblOid;
    m_cAttrs.iType = OidToGdoType(lTypeOid, iSize, lMod, &m_cAttrs.lSize);
    m_bHasDef = bHasDef;
    m_cAttrs.bRequired = bRequired;
    m_cAttrs.lAttr = 0;
    m_cAttrs.lTypeOid = lTypeOid;
    m_cAttrs.sOrigSchema = sOrigSchema;
    m_cAttrs.sOrigTable = sOrigTable;
    m_cAttrs.wsTblName = wsTblName;

    if(pViewRes)
    {
        int idx = PQfnumber(pViewRes, m_sOrigName);
        if(idx > -1)
        {
            m_cAttrs.lBaseTblOid = PQftable(pViewRes, idx);
            m_cAttrs.iBaseTblPos = PQftablecol(pViewRes, idx);
        }
    }

    if(sDefault)
    {
        if(m_sTmpDefVal) free(m_sTmpDefVal);
        m_sTmpDefVal = (LPSTR)malloc((strlen(sDefault) + 1)*sizeof(char));
        strcpy(m_sTmpDefVal, sDefault);
    }

    if(bExtInfo)
    {
        m_bIsKey = bIsKey;
        m_cAttrs.lSubType = lGeomType;
        if(sCSGuid)
            DBStrToWCharBuf(sCSGuid, m_pConnStruct->lDBEnc, m_cAttrs.sCSGuid, 40);
    }
    return;
}

void GTDField::SetIdxProps(PConnStruct pConnStruct, LPCSTR sName, bool bDesc,
    int iType)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iFldType = 2;
    m_iStatus = cisCreated;

    if(m_sOrigName) free(m_sOrigName);
    if(m_wsName) free(m_wsName);

    int iLen = strlen(sName);
    m_sOrigName = (LPSTR)malloc((iLen + 1)*sizeof(char));
    strcpy(m_sOrigName, sName);

    m_wsName = DBStrToWChar(sName, m_pConnStruct->lDBEnc);

    if(bDesc) m_cAttrs.lAttr = 1;
    m_cAttrs.iType = iType;
    return;
}

LPSTR GTDField::GetOrigSchemaPtr()
{
    return(m_cAttrs.sOrigSchema);
}

LPSTR GTDField::GetOrigTablePtr()
{
    return(m_cAttrs.sOrigTable);
}

LPWSTR GTDField::GetNamePtr()
{
    return(m_wsName);
}

LPWSTR GTDField::GetTblNamePtr()
{
    return(m_cAttrs.wsTblName);
}

void GTDField::GetSQLTypeStr(LPSTR sBuf)
{
//char buf[64];
    switch(m_cAttrs.iType)
    {
    case gdbBoolean:
        strcpy(sBuf, "boolean");
        break;
    case gdbByte:
        strcpy(sBuf, "\"char\"");
        break;
    case gdbInteger:
        strcpy(sBuf, "smallint");
        break;
    case gdbLong:
        if(m_cAttrs.lAttr & 16) strcpy(sBuf, "serial");
        else strcpy(sBuf, "integer");
        break;
    case gdbCurrency:
        strcpy(sBuf, "money");
        break;
    case gdbSingle:
        strcpy(sBuf, "real");
        break;
    case gdbDouble:
        strcpy(sBuf, "double precision");
        break;
    case gdbDate:
//sprintf(buf, "Type: %d, Subtype: %d, Size: %d, Attr: %d\r\n", m_cAttrs.iType,
//m_cAttrs.lSubType, m_cAttrs.lSize, m_cAttrs.lAttr);
//WriteLogFile(buf, false);
        strcpy(sBuf, "timestamp");
        break;
    case gdbText:
        if(m_cAttrs.lSize > 0) sprintf(sBuf, "varchar(%ld)", m_cAttrs.lSize);
        else strcpy(sBuf, "text");
        break;
    case gdbLongBinary:
        strcpy(sBuf, "bytea");
        break;
    case gdbMemo:
        strcpy(sBuf, "text");
        break;
    case gdbGuid:
        strcpy(sBuf, "uuid");
        break;
    case gdbSpatial:
        //strcpy(sBuf, "geometry");
        //break;
    case gdbGraphic:
        strcpy(sBuf, "bytea");
        break;
    default:
        strcpy(sBuf, "text");
    }
    if(m_cAttrs.bRequired) strcat(sBuf, " not null");
    if(m_cAttrs.wsDefVal)
    {
        strcat(sBuf, " default '");
        int ilen = strlen(sBuf);
        WCharToDBStrBufNull(m_cAttrs.wsDefVal, m_pConnStruct->lDBEnc,
            &sBuf[ilen], 128 - ilen);
        strcat(sBuf, "'");
    }
    return;
}

void GTDField::SetStatus(CCollItemStatus iStatus)
{
    m_iStatus = iStatus;
    return;
}

int GTDField::GetType()
{
    return(m_cAttrs.iType);
}

void GTDField::GetGeomTypeStr(LPSTR sBuf)
{
    switch(m_cAttrs.lSubType)
    {
    case gdbPoint:
        //swprintf(sBuf, L"POINT");
        strcpy(sBuf, "MULTIPOINT");
        break;
    case gdbLinear:
        strcpy(sBuf, "MULTILINESTRING");
        //swprintf(sBuf, L"MULTICURVE");
        break;
    case gdbAreal:
        strcpy(sBuf, "MULTIPOLYGON");
        //swprintf(sBuf, L"MULTISURFACE");
        break;
    case gdbAnySpatial:
        strcpy(sBuf, "GEOMETRYCOLLECTION");
        break;
    case gdbCoverage:
        strcpy(sBuf, "IMAGE");
        break;
    case gdbGraphicsText:
        strcpy(sBuf, "GRAPHICTEXT");
        break;
    default:
        strcpy(sBuf, "GEOMETRY");
    }
    return;
}

bool GTDField::GetDesc()
{
    return((m_cAttrs.lAttr & 1) > 0);
}

bool GTDField::GetRequired()
{
    return(m_cAttrs.bRequired);
}

long GTDField::GetGeomType()
{
    return(m_cAttrs.lSubType);
}

int GTDField::GetPsgType()
{
    return(m_cAttrs.iPsgType);
}

unsigned long GTDField::GetSrid()
{
    return(m_cAttrs.ulSrid);
}

bool GTDField::IsNativeGeometry()
{
    return((m_cAttrs.iType == gdbSpatial) &&
        !((m_cAttrs.lSubType == gdbGraphicsText) ||
        (m_cAttrs.lSubType == gdbCoverage)));
}

bool GTDField::IsEmulGeometry()
{
    return((m_cAttrs.iType == gdbGraphic) ||
        (m_cAttrs.lSubType == gdbGraphicsText) ||
        (m_cAttrs.lSubType == gdbCoverage));
}

void GTDField::CopyAttributes(PFldAttrs pAttrs)
{
#if DBGLEVEL > 1
    WriteLogFile("GTDField::CopyAttributes\r\n", true);
    wchar_t wsbuf[128];
    swprintf(wsbuf, L"  Name: %s, PG Type: %d, GDO Type: %d\r\n",
        m_wsName, m_cAttrs.lSubType, m_cAttrs.iType);
    WriteLogFile(wsbuf, false);
#endif // DBGLEVEL
    pAttrs->lAttr = m_cAttrs.lAttr;
    pAttrs->iType = m_cAttrs.iType;
    pAttrs->lSubType = m_cAttrs.lSubType;
    pAttrs->lColOrder = m_cAttrs.lColOrder;
    if(m_cAttrs.sDefValDef)
    {
        pAttrs->sDefValDef =
            (LPSTR)malloc((strlen(m_cAttrs.sDefValDef) + 1)*sizeof(char));
        strcpy(pAttrs->sDefValDef, m_cAttrs.sDefValDef);
    }
    else pAttrs->sDefValDef = NULL;
    if(m_cAttrs.wsDefVal)
    {
        pAttrs->wsDefVal =
            (LPWSTR)malloc((wcslen(m_cAttrs.wsDefVal) + 1)*sizeof(wchar_t));
        wcscpy(pAttrs->wsDefVal, m_cAttrs.wsDefVal);
    }
    else pAttrs->wsDefVal = NULL;
    pAttrs->bRequired = m_cAttrs.bRequired;
    //pAttrs->bUpdatable = m_cAttrs.bUpdatable;
    pAttrs->iGeomDim = m_cAttrs.iGeomDim;
    pAttrs->iPsgType = m_cAttrs.iPsgType;
    pAttrs->ulSrid = m_cAttrs.ulSrid;
    wcscpy(pAttrs->sCSGuid, m_cAttrs.sCSGuid);
    pAttrs->lTblOid = m_cAttrs.lTblOid;
    pAttrs->iTblPos = m_cAttrs.iTblPos;
    pAttrs->sOrigSchema = m_cAttrs.sOrigSchema;
    pAttrs->sOrigTable = m_cAttrs.sOrigTable;
    pAttrs->wsTblName = m_cAttrs.wsTblName;
    return;
}

void GTDField::ChangeCSGuid(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    if(wcsicmp(m_cAttrs.sCSGuid, wsOldCSGuid) == 0)
        wcscpy(m_cAttrs.sCSGuid, wsNewCSGuid);
    return;
}

void GTDField::SetTblOid(Oid lTblOid)
{
    m_cAttrs.lTblOid = lTblOid;
    return;
}

int GTDField::GetTblPos()
{
    return(m_cAttrs.iTblPos);
}

void GTDField::SetTblPos(int iPos)
{
    m_cAttrs.iTblPos = iPos;
    return;
}

bool GTDField::IsAutoNum()
{
    return(m_cAttrs.sDefValDef);
}

bool GTDField::IsSmallField()
{
    return((m_cAttrs.iType < 9) || ((m_cAttrs.iType == 10) &&
        (m_cAttrs.lSize < 256)));
}

bool GTDField::IsKeyField()
{
    return(m_bIsKey);
}

void GTDField::SetKeyField(bool bKey)
{
    m_bIsKey = bKey;
    return;
}

void GTDField::UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
    LPWSTR wsNewTblName)
{
    m_cAttrs.sOrigSchema = sOrigSchema;
    m_cAttrs.sOrigTable = sOrigTable;
    m_cAttrs.wsTblName = wsNewTblName;
    return;
}

void GTDField::SetHasSpecField(int iSpecType)
{
    m_iSpecType = iSpecType;
    return;
}

LPSTR GTDField::GetFilterFieldName()
{
    LPSTR sRes = NULL;
    int ilen;
    bool bHasGeom = (m_cAttrs.ulSrid > 0) && (m_cAttrs.iType == 32) &&
        (m_cAttrs.lSubType != 4) && (m_cAttrs.lSubType != 5) &&
        (m_cAttrs.iPsgType < 14);
    if(!bHasGeom && (m_iSpecType > 1))
    {
        ilen = strlen(m_sOrigName) + 5;
        sRes = (LPSTR)malloc(ilen*sizeof(char));
        strcpy(sRes, m_sOrigName);
        strcat(sRes, "_nat");
    }
    else if(bHasGeom)
    {
        ilen = strlen(m_sOrigName) + 1;
        sRes = (LPSTR)malloc(ilen*sizeof(char));
        strcpy(sRes, m_sOrigName);
    }
    return(sRes);
}

bool GTDField::AddIngrField()
{
    if((m_cAttrs.lSubType != gdbPoint) && (m_cAttrs.lSubType != gdbAnySpatial))
        return(true);

    int iLen = strlen("alter table  add column _igr bytea") +
        strlen(m_cAttrs.sOrigTable) + strlen(m_sOrigName) + 5;
    if(m_cAttrs.sOrigSchema) iLen += (strlen(m_cAttrs.sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_cAttrs.sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" add column \"%s_igr\" bytea",
            m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName);
    else
        sprintf(sSql, "alter table \"%s\" add column \"%s_igr\" bytea",
            m_cAttrs.sOrigTable, m_sOrigName);

    bool bRes = ExecuteCommand(m_pConnStruct, sSql);
    free(sSql);

    if(bRes) m_iSpecType = 1;
    return(bRes);
}

void GTDField::DropIngrField()
{
    int iLen = strlen("alter table  drop column _igr") +
        strlen(m_cAttrs.sOrigTable) + strlen(m_sOrigName) + 5;
    if(m_cAttrs.sOrigSchema) iLen += (strlen(m_cAttrs.sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_cAttrs.sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" drop column \"%s_igr\"",
            m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName);
    else
        sprintf(sSql, "alter table \"%s\" drop column \"%s_igr\"",
            m_cAttrs.sOrigTable, m_sOrigName);

    ExecuteCommand(m_pConnStruct, sSql);
    free(sSql);
    m_iSpecType = 0;
    return;
}

bool GTDField::AddNativeField()
{
    int iLen = 0;

    if(m_cAttrs.lSubType == 4) // coverage
    {
        iLen = strlen("SELECT AddGeometryColumn('', '', '', , 'POLYGON', 2)")
            + strlen(m_cAttrs.sOrigTable) + strlen(m_sOrigName) + 16;
    }
    else if(m_cAttrs.lSubType == 5) // grephic text
    {
        iLen = strlen("SELECT AddGeometryColumn('', '', '', , 'POINT', 2)")
            + strlen(m_cAttrs.sOrigTable) + strlen(m_sOrigName) + 16;
    }
    else return(false);

    if(m_cAttrs.sOrigSchema) iLen += (strlen(m_cAttrs.sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));

    if(m_cAttrs.lSubType == 4) // coverage
    {
        if(m_cAttrs.sOrigSchema)
            sprintf(sSql, "SELECT AddGeometryColumn('%s', '%s', '%s_nat', %ld, 'POLYGON', 2)",
                m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName, m_cAttrs.ulSrid);
        else
            sprintf(sSql, "SELECT AddGeometryColumn('', '%s', '%s_nat', %ld, 'POLYGON', 2)",
                m_cAttrs.sOrigTable, m_sOrigName, m_cAttrs.ulSrid);
    }
    else if(m_cAttrs.lSubType == 5) // grephic text
    {
        if(m_cAttrs.sOrigSchema)
            sprintf(sSql, "SELECT AddGeometryColumn('%s', '%s', '%s_nat', %ld, 'POINT', 2)",
                m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName, m_cAttrs.ulSrid);
        else
            sprintf(sSql, "SELECT AddGeometryColumn('', '%s', '%s_nat', %ld, 'POINT', 2)",
                m_cAttrs.sOrigTable, m_sOrigName, m_cAttrs.ulSrid);
    }

    bool bRes = ExecuteCommand(m_pConnStruct, sSql);
    free(sSql);

    if(bRes) m_iSpecType = 2;

    return(bRes);
}

void GTDField::DropNativeField()
{
    int iLen = strlen("alter table  drop column _nat") +
        strlen(m_cAttrs.sOrigTable) + strlen(m_sOrigName) + 5;
    if(m_cAttrs.sOrigSchema) iLen += (strlen(m_cAttrs.sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_cAttrs.sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" drop column \"%s_nat\"",
            m_cAttrs.sOrigSchema, m_cAttrs.sOrigTable, m_sOrigName);
    else
        sprintf(sSql, "alter table \"%s\" drop column \"%s_nat\"",
            m_cAttrs.sOrigTable, m_sOrigName);

    ExecuteCommand(m_pConnStruct, sSql);
    free(sSql);

    m_iSpecType = 0;
    return;
}


//  GTDFields

GTDFields::GTDFields(IUnknown *pUnkOuter, ITypeLib *pLib) :
    _IGDynaCollection(true, pUnkOuter, pLib, 23)
{
#if DBGLEVEL > 1
    WriteLogFile("GTDFields::GTDFields-1\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_iStatus = cisUnknown;
    m_wsTblName = NULL;
    m_wsIdxName = NULL;
}

GTDFields::GTDFields(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex) :
    _IGDynaCollection(true, pUnkOuter, pLib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GTDFields::GTDFields-2\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
    m_iStatus = cisUnknown;
    m_wsTblName = NULL;
    m_wsIdxName = NULL;
}

GTDFields::~GTDFields()
{
#if DBGLEVEL > 1
    WriteLogFile("GTDFields::~GTDFields\r\n", true);
#endif // DBGLEVEL
}

HRESULT GTDFields::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GTDFields::QueryInterface\r\n", true);
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

ULONG GTDFields::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GTDFields::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GTDFields::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GTDFields::Append(IDispatch * Object)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDFields::Append\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        if(m_wsIdxName)  // parent is Index, we don't allow to
            // do that the user must drop index and recreate, if he wants to
            // alter it
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_FLDMODONIDX, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(E_FAIL);
        }

        int slen;
        LPSTR sCmd;
        GTDField *pFld = (GTDField*)Object;
        bool bRes = true;
        char sGeomType[32];
        char sSchema[64];
        strcpy(sSchema, "public");

        LPSTR sSchemaName = pFld->GetOrigSchemaPtr();
        LPSTR sTblName = pFld->GetOrigTablePtr();

        if(!sSchemaName) sSchemaName = sSchema;

        if(pFld->IsNativeGeometry())
        {
            pFld->GetGeomTypeStr(sGeomType);

            slen = 50 + strlen(sSchema) + strlen(sTblName) +
                strlen(pFld->GetOrigNamePtr()) + 32 + strlen(sGeomType);
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "SELECT AddGeometryColumn('%s', '%s', '%s', %ld, '%s', 3)",
                sSchemaName, sTblName, pFld->GetOrigNamePtr(), pFld->GetSrid(),
                sGeomType);
            bRes = ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);

            if(bRes) bRes = pFld->AddIngrField();
        }
        else
        {
            char sType[128];
            pFld->GetSQLTypeStr(sType);
            slen = 34 + GetTableNameLen(pFld->GetOrigSchemaPtr(),
                pFld->GetOrigTablePtr()) + strlen(pFld->GetOrigNamePtr()) +
                strlen(sType);
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            strcpy(sCmd, "ALTER TABLE ");
            CatTableName(sCmd, pFld->GetOrigSchemaPtr(),
                pFld->GetOrigTablePtr(), true);
            strcat(sCmd, " ADD COLUMN ");
            CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
            strcat(sCmd, " ");
            strcat(sCmd, sType);
            bRes = ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);

            if(bRes && pFld->IsEmulGeometry() && m_pConnStruct->sGraphicColumns[0])
            {
                slen = strlen("delete from  where \
f_table_schema = '' and f_table_name = '' and f_graphic_column = ''") +
                    strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                    strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 1;
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                sprintf(sCmd, "delete from %s where \
f_table_schema = '%s' and f_table_name = '%s' and f_graphic_column = '%s'",
                    m_pConnStruct->sGraphicColumns, sSchemaName, sTblName,
                    pFld->GetOrigNamePtr());
                ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);

                pFld->GetGeomTypeStr(sGeomType);

                slen = strlen("insert into  values('', '', '', 3, , '', '', '')") +
                    strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                    strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 32 +
                    strlen(sGeomType);
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                sprintf(sCmd, "insert into %s values('%s', '%s', '%s', 3, %ld, \
'%s', '', '')", m_pConnStruct->sGraphicColumns, sSchemaName, sTblName,
                    pFld->GetOrigNamePtr(), pFld->GetSrid(), sGeomType);
                ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);

                pFld->AddNativeField();
            }
        }
        if(bRes)
        {
            pFld->SetStatus(cisCreated);
            pFld->SetTblPos(GetCount() + 1);

            slen = strlen("delete from  where tablename = $1") +
                strlen(m_pConnStruct->sModTables) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "delete from %s where tablename = $1",
                m_pConnStruct->sModTables);

            VARIANT vPar;
            vPar.vt = VT_BSTR;
            vPar.bstrVal = pFld->GetTblNamePtr();
            ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
            free(sCmd);
        }
        else return(E_FAIL);
    }
    _IGCollection::AddItem((CCOMDispatch*)Object);
    return(S_OK);
}

HRESULT GTDFields::Delete(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GTDFields::Delete\r\n", true);
#endif // DBGLEVEL
    int idx = FindByWName(Name);
    if(idx < 0) return(S_FALSE);

    if(m_iStatus == cisCreated)
    {
        if(m_wsIdxName)  // parent is Index, we don't allow to
            // do that the user must drop index and recreate, if he wants to
            // alter it
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_FLDMODONIDX, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        int slen;
        GTDField *pFld = (GTDField*)m_pData[idx];
        slen = 34 + GetTableNameLen(pFld->GetOrigSchemaPtr(),
            pFld->GetOrigTablePtr()) + strlen(pFld->GetOrigNamePtr());
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "ALTER TABLE ");
        CatTableName(sCmd, pFld->GetOrigSchemaPtr(), pFld->GetOrigTablePtr(),
            true);
        strcat(sCmd, " DROP COLUMN ");
        CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes) return(S_FALSE);

        if(pFld->IsEmulGeometry() && m_pConnStruct->sGraphicColumns[0])
        {
            char sSchema[64];
            strcpy(sSchema, "public");

            LPSTR sSchemaName = pFld->GetOrigSchemaPtr();
            LPSTR sTblName = pFld->GetOrigTablePtr();

            if(!sSchemaName) sSchemaName = sSchema;

            slen = strlen("delete from  where \
f_table_schema = '' and f_table_name = '' and f_graphic_column = ''") +
                strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(wchar_t));
            sprintf(sCmd, "delete from %s where \
f_table_schema = '%s' and f_table_name = '%s' and f_graphic_column = '%s'",
                m_pConnStruct->sGraphicColumns, sSchemaName, sTblName,
                pFld->GetOrigNamePtr());
            ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);

            pFld->DropNativeField();
        }
        else if(pFld->IsNativeGeometry())
        {
            pFld->DropIngrField();
        }

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    _IGDynaCollection::DeleteItem(idx);
    return(S_OK);
}

HRESULT GTDFields::get_Item(VARIANT index, GTDField* *ppGField)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GTDFields::get_Item\r\n", true);
    WriteVariantToLogFile(L"  index: ", index);
#endif // DBGLEVEL
    if(!ppGField) return(E_POINTER);
    // a "get_CollectionItem" method is probably an exception from the COM
    // standards - it can accept uninitialized return values, similary as
    // QueryInterface method
    //if(*ppGField) (*ppGField)->Release();

    *ppGField = NULL;
    int i = -1;
    if(index.vt == VT_BSTR)
        i = FindByWName(index.bstrVal);
    else if(index.vt == (VT_BYREF | VT_BSTR))
        i = FindByWName(*index.pbstrVal);
    else i = VarToInt(index);

    GTDField *pFld = (GTDField*)_IGCollection::GetItem(i);
    if(!pFld) return(S_FALSE);

    pFld->AddRef();
    *ppGField = pFld;
    return(S_OK);
}

void GTDFields::ClearAll()
{
    _IGCollection::ClearAll();
    return;
}

GTDField* GTDFields::AddField(LPCSTR sName, bool bDesc, int iType)
{
    GTDField *pfld = new GTDField(NULL, m_pTypeLib);
    pfld->SetIdxProps(m_pConnStruct, sName, bDesc, iType);
    _IGCollection::AddItem(pfld);
    return(pfld);
}

int GTDFields::FindByName(LPCSTR sName)
{
    int i = 0;
    bool bFound = false;
    GTDField *pfld;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pfld = (GTDField*)m_pData[i++];
        bFound = strcmp(pfld->GetOrigNamePtr(), sName) == 0;
    }
    return(bFound ? i - 1 : -1);
}

GTDField* GTDFields::GetByName(LPCSTR sName)
{
    int i = FindByName(sName);
    if(i < 0) return(NULL);
    return((GTDField*)_IGCollection::GetItem(i));
}

int GTDFields::FindByWName(LPWSTR wsName)
{
    int i = 0;
    bool bFound = false;
    GTDField *pfld;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pfld = (GTDField*)m_pData[i++];
        bFound = wcscmp(pfld->GetNamePtr(), wsName) == 0;
    }
    return(bFound ? i - 1 : -1);
}

GTDField* GTDFields::GetByWName(LPWSTR wsName)
{
    int i = FindByWName(wsName);
    if(i < 0) return(NULL);
    return((GTDField*)_IGCollection::GetItem(i));
}

GTDField* GTDFields::GetByTblPos(int iPos)
{
    bool bFound = false;
    int i = 0;
    GTDField *pfld;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pfld = (GTDField*)m_pData[i++];
        bFound = (pfld->GetTblPos() == iPos);
    }
    return(bFound ? pfld : NULL);
}

int GTDFields::BuildFromConnection(PConnStruct pConnStruct, Oid lTblOid,
    LPSTR sOrigSchema, LPSTR sOrigName, LPWSTR wsTblName, bool bView)
{
    if(m_pConnStruct) return(_IGCollection::GetCount());

    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;

    m_iFldType = 1;
    m_iStatus = cisCreated;
    m_wsTblName = wsTblName;

    GTDField *pfd;

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

    SetSP(m_pConnStruct->pConn, bTrans);

    PGresult *res = NULL;
    PGresult *pViewRes = NULL;

    if(bView)
    {
        char sSchema[128];
        if(sOrigSchema) strcpy(sSchema, sOrigSchema);
        else strcpy(sSchema, "public");
        LPSTR psViewParams[2];
        psViewParams[0] = sSchema;
        psViewParams[1] = sOrigName;

        res = PQexecParams(m_pConnStruct->pConn, "select view_definition from information_schema.views where \
table_schema = $1 and table_name = $2", 2, NULL, psViewParams, NULL, NULL, 0);
        if(PQresultStatus(res) == PGRES_TUPLES_OK)
        {
            VARIANT vRow;
            VariantInit(&vRow);
            Oid iType;
            DescribeFieldTypes(res, 1, &iType);
            GetRecord(res, 0, 1, &iType, &vRow, m_pConnStruct->sDecSep);
            PQclear(res);

            if(vRow.vt == VT_LPSTR)
            {
                res = PQprepare(m_pConnStruct->pConn, "viewteststmt",
                    vRow.pcVal, 0, NULL);
                if(PQresultStatus(res) == PGRES_COMMAND_OK)
                {
                    pViewRes = PQdescribePrepared(m_pConnStruct->pConn,
                        "viewteststmt");
                }
            }
        }
        else
        {
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
            SetSP(m_pConnStruct->pConn, bTrans);
        }
    }

    char sOidBuf[32];
    sprintf(sOidBuf, "%d", lTblOid);

    LPSTR psParam[2];
    psParam[0] = sOidBuf;

    char sNoMetaSel[] = "select t1.attname, t1.atttypid, t1.attlen, t1.attnum, t1.attnotnull, \
t1.atttypmod, t1.atthasdef, t1.attisdropped, t2.adsrc \
from pg_attribute t1 \
left outer join pg_attrdef t2 on t1.attrelid = t2.adrelid and \
t1.attnum = t2.adnum \
where t1.attrelid = $1 and t1.attnum > 0 \
order by t1.attnum";

    char sIngrMetaSel[] = "select \
t1.attname, t1.atttypid, t1.attlen, t1.attnum, t1.attnotnull, t1.atttypmod, t1.atthasdef, \
t1.attisdropped, t2.adsrc, t3.iskeyfield, t3.geometrytype, t3.gcoordsystemguid \
from pg_attribute t1 \
left outer join pg_attrdef t2 on t1.attrelid = t2.adrelid and \
t1.attnum = t2.adnum \
left join \
( \
    select iskeyfield, null as geometrytype, fl.fieldname, null as gcoordsystemguid from %s ap \
    inner join %s fl on ap.indexid = fl.indexid \
    where fl.featurename = $2 \
    union \
    select null as iskeyfield, geometrytype, fl.fieldname, gcoordsystemguid from %s gp \
    inner join %s fl on gp.indexid = fl.indexid \
    where fl.featurename = $2 \
) \
t3 on t3.fieldname = t1.attname \
where t1.attrelid = $1 and t1.attnum > 0 \
order by t1.attnum";
// gdo.attributeproperties, gdo.fieldlookup, gdo.geometryproperties, gdo.fieldlookup

    char *psSelect = NULL;
    int iLen = 0;
    if(m_pConnStruct->iConnStat >icsINGRMetaIncomplete)
    {
        iLen = strlen(sIngrMetaSel) + strlen(m_pConnStruct->sAttributeProperties) +
            strlen(m_pConnStruct->sGeometryProperties) +
            2*strlen(m_pConnStruct->sFieldLookup) + 1;
        psSelect = (char*)malloc(iLen*sizeof(char));
        sprintf(psSelect, sIngrMetaSel, m_pConnStruct->sAttributeProperties,
            m_pConnStruct->sFieldLookup, m_pConnStruct->sGeometryProperties,
            m_pConnStruct->sFieldLookup);

        psParam[1] = WCharToDBStr(wsTblName, m_pConnStruct->lDBEnc);
        res = PQexecParams(m_pConnStruct->pConn, psSelect, 2, NULL, psParam, NULL, NULL, 0);
        free(psParam[1]);
    }
    else
    {
        iLen = strlen(sNoMetaSel) + 1;
        psSelect = (char*)malloc(iLen*sizeof(char));
        strcpy(psSelect, sNoMetaSel);
        res = PQexecParams(m_pConnStruct->pConn, psSelect, 1, NULL, psParam, NULL, NULL, 0);
    }
    free(psSelect);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        m_pErrors->HandlePQError(pConnStruct->pConn, pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        if(pViewRes)
        {
            PQclear(pViewRes);
            PQexec(m_pConnStruct->pConn, "deallocate viewteststmt");
        }
        return(_IGCollection::GetCount());
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        if(pViewRes)
        {
            PQclear(pViewRes);
            PQexec(m_pConnStruct->pConn, "deallocate viewteststmt");
        }
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
        return(_IGCollection::GetCount());
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int j = 0; j < ncols; j++) VariantInit(&pvRow[j]);

    LPSTR sDefault = NULL;
    if(m_pConnStruct->iConnStat >icsINGRMetaIncomplete)
    {
        LPSTR sCSGuid = NULL;

        for(long li = 0; li < nrows; li++)
        {
            GetRecord(res, li, ncols, piTypes, pvRow, pConnStruct->sDecSep);
            if(!VarToBool(pvRow[7]) && (pvRow[0].vt == VT_LPSTR))
            {
                sDefault = NULL;
                if(pvRow[8].vt == VT_LPSTR) sDefault = pvRow[8].pcVal;
                sCSGuid = NULL;
                if(pvRow[11].vt == VT_LPSTR) sCSGuid = pvRow[11].pcVal;

                pfd = new GTDField(NULL, m_pTypeLib);
                pfd->SetTDProps(m_pConnStruct, pvRow[0].pcVal, VarToInt(pvRow[3]),
                    VarToLong(pvRow[1]), VarToInt(pvRow[2]), VarToLong(pvRow[5]),
                    VarToBool(pvRow[4]), VarToBool(pvRow[6]), lTblOid, sOrigSchema,
                    sOrigName, wsTblName, pViewRes, sDefault, true, VarToBool(pvRow[9]),
                    VarToLong(pvRow[10]), sCSGuid);
                _IGCollection::AddItem(pfd);
            }
        }
    }
    else
    {
        for(long li = 0; li < nrows; li++)
        {
            GetRecord(res, li, ncols, piTypes, pvRow, pConnStruct->sDecSep);
            if(!VarToBool(pvRow[7]) && (pvRow[0].vt == VT_LPSTR))
            {
                sDefault = NULL;
                if(pvRow[8].vt == VT_LPSTR) sDefault = pvRow[8].pcVal;

                pfd = new GTDField(NULL, m_pTypeLib);
                pfd->SetTDProps(m_pConnStruct, pvRow[0].pcVal, VarToInt(pvRow[3]),
                    VarToLong(pvRow[1]), VarToInt(pvRow[2]), VarToLong(pvRow[5]),
                    VarToBool(pvRow[4]), VarToBool(pvRow[6]), lTblOid, sOrigSchema,
                    sOrigName, wsTblName, pViewRes, sDefault, false, false, 0, NULL);
                _IGCollection::AddItem(pfd);
            }
        }
    }

    PQclear(res);
    if(pViewRes)
    {
        PQclear(pViewRes);
        PQexec(m_pConnStruct->pConn, "deallocate viewteststmt");
    }
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    for(int j = 0; j < ncols; j++) PgVariantClear(&pvRow[j]);
    free(pvRow);
    free(piTypes);

    for(int i = 0; i < m_iDataLen; i++)
    {
        ((GTDField*)GetItem(i))->UpdateAttrs();
    }

    RemSpecFields();
    return(_IGCollection::GetCount());
}

void GTDFields::RemSpecFields()
{
    int i = 0, j;
    LPSTR sName, sGeomName;
    int iLen;
    bool bHasGeom;
    int iSpecType;
    GTDField *pFld;
    while(i < m_iDataLen)
    {
        iSpecType = 0;
        bHasGeom = false;

        sName = ((GTDField*)GetItem(i))->GetOrigNamePtr();
        iLen = strlen(sName);

        if(iLen > 4)
        {
            iLen -= 4;
            if(strcmp(&sName[iLen], "_igr") == 0) iSpecType = 1;
            else if(strcmp(&sName[iLen], "_nat") == 0) iSpecType = 2;
        }

        if(iSpecType > 0)
        {
            j = 0;
            while(!bHasGeom && (j < m_iDataLen))
            {
                pFld = (GTDField*)GetItem(j++);
                sGeomName = pFld->GetOrigNamePtr();
                if((int)strlen(sGeomName) == iLen)
                {
                    bHasGeom = (strncmp(sName, sGeomName, iLen) == 0);
                }
            }
        }

        if(bHasGeom)
        {
            pFld->SetHasSpecField(iSpecType);
            DeleteItem(i);
        }
        else i++;
    }
    return;
}

void GTDFields::BuildFromIndex(PConnStruct pConnStruct, LPWSTR wsTblName,
    LPWSTR wsIdxName)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;
    m_wsIdxName = wsIdxName;
    m_iStatus = cisCreated;
    return;
}

void GTDFields::UpdateName(LPWSTR wsIdxName)
{
    m_wsIdxName = wsIdxName;
    return;
}

void GTDFields::SetNewAttrs(PConnStruct pConnStruct, LPWSTR wsTblName)
{
    m_wsTblName = wsTblName;
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iStatus = cisNew;
    return;
}

int GTDFields::BuildFromGAlias(PConnStruct pConnStruct, LPSTR sOrigSchema,
    LPSTR sOrigTable, LPWSTR wsTblName)
{
    if(m_pConnStruct) return(_IGCollection::GetCount());

    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;

    m_iFldType = 1;
    m_iStatus = cisCreated;

    GTDField *pfd = new GTDField(NULL, m_pTypeLib);
    pfd->SetTDProps(pConnStruct, "TableType", _IGCollection::GetCount(),
        VARCHAROID, 0, 259, false, false, 0, sOrigSchema, sOrigTable, wsTblName,
        NULL, NULL, false, false, 0, NULL);
    _IGCollection::AddItem(pfd);
    pfd = new GTDField(NULL, m_pTypeLib);
    pfd->SetTDProps(pConnStruct, "TableName", _IGCollection::GetCount(),
        VARCHAROID, 0, 259, false, false, 0, sOrigSchema, sOrigTable, wsTblName,
        NULL, NULL, false, false, 0, NULL);
    _IGCollection::AddItem(pfd);

    return(2);
}

void GTDFields::SetStatus(CCollItemStatus iStatus)
{
    m_iStatus = iStatus;
    return;
}

void GTDFields::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    GTDField *pFld;
    for(int i = 0; i < GetCount(); i++)
    {
        pFld = (GTDField*)GetItem(i);
        pFld->ChangeCSGuid(wsOldCSGuid, wsNewCSGuid);
    }
    return;
}

void GTDFields::SetTblOids(Oid lTblOid)
{
    GTDField *pFld;
    for(int i = 0; i < GetCount(); i++)
    {
        pFld = (GTDField*)GetItem(i);
        pFld->SetTblOid(lTblOid);
    }
    return;
}

// We know that the fields has positions numbered by 1, 2, 3 etc. possibly
// with some fields on position 0 - these do not physically exist in the
// database, so we will move them to the end. If anything like that occurs,
// it will cause some other troubles anyway

int __cdecl FieldsCompare(const void *elem1, const void *elem2)
{
    GTDField *pFld1 = (GTDField*)elem1;
    GTDField *pFld2 = (GTDField*)elem2;
    int iPos1 = pFld1->GetTblPos();
    int iPos2 = pFld2->GetTblPos();
    if(iPos1 < 1) return(1);
    if(iPos2 < 1) return(-1);
    return(iPos1 - iPos2);
}

void GTDFields::Sort()
{
    qsort(m_pData, GetCount(), sizeof(GTDField*), FieldsCompare);
    return;
}

GTDField* GTDFields::GetAutoNumFld()
{
    int i = 0;
    bool bFound = false;
    GTDField *pfld;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pfld = (GTDField*)m_pData[i++];
        bFound = pfld->IsAutoNum();
    }
    return(bFound ? pfld : NULL);
}

void GTDFields::ResetKeyFields()
{
    GTDField *pFld;
    for(int i = 0; i < GetCount(); i++)
    {
        pFld = (GTDField*)GetItem(i);
        pFld->SetKeyField(false);
    }
    return;
}

int GTDFields::SetKeyFields(GIndex *pIdx)
{
    int iCnt = 0;
    GTDField *pFld;
    if(pIdx)
    {
        GTDField *pIdxFld;
        GTDFields *pIdxFlds = pIdx->GetFieldsPtr();
        iCnt = pIdxFlds->GetCount();
        if(iCnt > 10) iCnt = 10;
        for(int i = 0; i < iCnt; i++)
        {
            pIdxFld = (GTDField*)pIdxFlds->GetItem(i);
            pFld = GetByWName(pIdxFld->GetNamePtr());
            if(pFld) pFld->SetKeyField(true);
        }
    }
    else
    {
        int i = 0;
        int n = GetCount();
        while((i < n) && (iCnt < 10))
        {
            pFld = (GTDField*)GetItem(i++);
            if(pFld->IsSmallField())
            {
                pFld->SetKeyField(true);
                iCnt++;
            }
        }
    }
    return(iCnt);
}

bool GTDFields::HasKeyField()
{
    bool bFound = false;
    int i = 0;
    GTDField *pFld;
    while(!bFound && (i < GetCount()))
    {
        pFld = (GTDField*)GetItem(i++);
        bFound = pFld->IsKeyField();
    }
    return(bFound);
}

void GTDFields::UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigName,
    LPWSTR wsNewTblName)
{
    m_wsTblName = wsNewTblName;
    GTDField *pFld;
    for(int i = 0; i < GetCount(); i++)
    {
        pFld = (GTDField*)GetItem(i);
        pFld->UpdateTableName(sOrigSchema, sOrigName, wsNewTblName);
    }
    return;
}


//  GIndex

GIndex::GIndex(IUnknown *pUnkOuter, ITypeLib *ALib) : CCOMDispatch(pUnkOuter, ALib, 22)
{
#if DBGLEVEL > 1
    WriteLogFile("GIndex::GIndex-1\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_pConnStruct = NULL;
    m_pFields = new GTDFields(this, ALib);
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_wsTblName = NULL;
    m_bPrimary = false;
    m_bUnique = false;
    m_bNulls = false;
    m_sOrigSchema = NULL;
    m_sOrigTable = NULL;
}

GIndex::GIndex(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    CCOMDispatch(pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GIndex::GIndex-2\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_pConnStruct = NULL;
    m_pFields = new GTDFields(this, ALib);
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_wsTblName = NULL;
    m_bPrimary = false;
    m_bUnique = false;
    m_bNulls = false;
    m_sOrigSchema = NULL;
    m_sOrigTable = NULL;
}

GIndex::~GIndex()
{
#if DBGLEVEL > 1
    WriteLogFile("GIndex::~GIndex\r\n", true);
#endif // DBGLEVEL
    if(m_wsName) free(m_wsName);
    if(m_sOrigName) free(m_sOrigName);
    m_pFields->SetOuter(NULL);
}

HRESULT GIndex::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GIndex::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GIndex))
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

ULONG GIndex::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GIndex::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GIndex::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GIndex::get_GFields(GTDFields* *pVar)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_GFields\r\n", true);
#endif // DBGLEVEL
    if(!pVar) return(E_POINTER);
    if(*pVar) (*pVar)->Release();
    m_pFields->AddRef();
    *pVar = m_pFields;
    return(S_OK);
}

HRESULT GIndex::get_IgnoreNulls(VARIANT_BOOL *pb)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_IgnoreNulls\r\n", true);
#endif // DBGLEVEL
    if(!pb) return(E_POINTER);
    if(m_bNulls) *pb = VARIANT_TRUE;
    else *pb = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GIndex::set_IgnoreNulls(VARIANT_BOOL pb)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::set_IgnoreNulls\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    if((m_bNulls && pb) || !(m_bNulls || pb)) return(S_OK);
    m_bNulls = pb;
    return(S_OK);
}

HRESULT GIndex::get_Name(BSTR *Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_Name\r\n", true);
#endif // DBGLEVEL
    if(!Name) return(E_POINTER);
    if(*Name) SysFreeString(*Name);
    if(m_wsName) *Name = SysAllocString(m_wsName);
    else *Name = SysAllocString(L"");
    return(S_OK);
}

HRESULT GIndex::set_Name(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::set_Name\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    LPSTR sNewName = NULL;
    LPWSTR wsNewName = GetValidFieldName(Name, m_pConnStruct->lDBEnc, &sNewName);

    if(m_wsName)
    {
        if(wcscmp(m_wsName, wsNewName) == 0)
        {
            free(wsNewName);
            free(sNewName);
            return(S_OK);
        }
    }

    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = sNewName;
    if(m_wsName) free(m_wsName);
    m_wsName = wsNewName;
    return(S_OK);
}

HRESULT GIndex::get_Primary(VARIANT_BOOL *pPrimary)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_Primary\r\n", true);
#endif // DBGLEVEL
    if(!pPrimary) return(E_POINTER);
    if(m_bPrimary) *pPrimary = VARIANT_TRUE;
    else *pPrimary = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GIndex::set_Primary(VARIANT_BOOL pPrimary)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::set_Primary\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if((m_bPrimary && pPrimary) || !(m_bPrimary || pPrimary)) return(S_OK);
    m_bPrimary = pPrimary;
    return(S_OK);
}

HRESULT GIndex::set_Required(VARIANT_BOOL pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::set_Required\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }
    return(S_OK);
}

HRESULT GIndex::get_Required(VARIANT_BOOL *pRequired)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_Required\r\n", true);
#endif // DBGLEVEL
    if(!pRequired) return(E_POINTER);
    *pRequired = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GIndex::get_Unique(VARIANT_BOOL *pUnique)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::get_Unique\r\n", true);
#endif // DBGLEVEL
    if(!pUnique) return(E_POINTER);
    if(m_bUnique) *pUnique = VARIANT_TRUE;
    else *pUnique = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GIndex::set_Unique(VARIANT_BOOL pUnique)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::set_Unique\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if((m_bUnique && pUnique) || !(m_bUnique || pUnique)) return(S_OK);
    m_bUnique = pUnique;
    return(S_OK);
}

HRESULT GIndex::CreateField(VARIANT Name, VARIANT Type, VARIANT Size,
    GTDField* *ppField)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndex::CreateField\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        TCHAR sMsg[128];
        LoadString(m_pConnStruct->hInstance, IDE_INDEXCREATED, sMsg, 128);
        m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
        return(S_FALSE);
    }

    if(!ppField) return(E_POINTER);

    ValidateVariant(&Name);
    if(*ppField) (*ppField)->Release();
    *ppField = new GTDField(NULL, m_pTypeLib);

    // we put NULL for original schema name and original table name
    // the index field is not supposed to make a use of them
    (*ppField)->SetNewProps(m_pConnStruct, Name, Type, Size, 2, m_sOrigSchema,
        m_sOrigTable, m_wsTblName);

    (*ppField)->AddRef();
    return(S_OK);
}

void GIndex::SetNewAttrs(PConnStruct pConnStruct, VARIANT Name, LPWSTR wsTblName,
    LPSTR sOrigSchema, LPSTR sOrigTable)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;
    m_sOrigSchema = sOrigSchema;
    m_sOrigTable = sOrigTable;

    LPSTR sNewName = NULL;
    LPWSTR wsNewName = NULL;
    if(Name.vt == VT_BSTR)
    {
        wsNewName = GetValidFieldName(Name.bstrVal, m_pConnStruct->lDBEnc,
            &sNewName);
    }
    else if(Name.vt == (VT_BSTR | VT_BYREF))
    {
        wsNewName = GetValidFieldName(*Name.pbstrVal, m_pConnStruct->lDBEnc,
            &sNewName);
    }
    if(!wsNewName) return;

    if(m_wsName)
    {
        if(wcscmp(m_wsName, wsNewName) == 0)
        {
            free(sNewName);
            free(wsNewName);
            return;
        }
        free(m_wsName);
    }
    m_wsName = wsNewName;
    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = sNewName;

    m_iStatus = cisNew;
    return;
}

void GIndex::UpdateName()
{
    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

    SetSP(m_pConnStruct->pConn, bTrans);

    char sCmd[128];
    sprintf(sCmd, "select relname from pg_class where oid = %d", m_lOid);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
    //free(lsCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 8: ", m_pConnStruct->pConn);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return;
    }

    if(PQntuples(res) < 1)
    {
        WritePQErrorToLog("Spot 9: ", m_pConnStruct->pConn);
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
        return;
    }

    LPSTR psName = PQgetvalue(res, 0, 0);

    if(m_wsName) free(m_wsName);
    if(m_sOrigName) free(m_sOrigName);

    int ilen = strlen(psName);
    m_sOrigName = (LPSTR)malloc((ilen + 1)*sizeof(char));
    strcpy(m_sOrigName, psName);

    m_wsName = DBStrToWChar(psName, m_pConnStruct->lDBEnc);

    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    m_pFields->UpdateName(m_wsName);
    return;
}

void GIndex::SetTDAttrs(PConnStruct pConnStruct, Oid lIdxOid, bool bPrimary,
    bool bUnique, bool bNulls, LPWSTR wsTblName)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;
    m_lOid = lIdxOid;
    m_bPrimary = bPrimary;
    m_bUnique = bUnique;
    m_bNulls = bNulls;
    if(m_bPrimary)
    {
        m_bUnique = true;
        m_bNulls = false;
    }

    m_pFields->BuildFromIndex(m_pConnStruct, m_wsTblName, NULL);

    m_iStatus = cisCreated;
    return;
}

void GIndex::SetNameAttrs(PConnStruct pConnStruct, LPCSTR sName, bool bPrimary,
    bool bUnique, bool bNulls, LPWSTR wsTblName)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;
    m_lOid = 0;
    m_bPrimary = bPrimary;
    m_bUnique = bUnique;
    m_bNulls = bNulls;

    if(m_sOrigName) free(m_sOrigName);
    if(m_wsName) free(m_wsName);

    int ilen = strlen(sName);
    m_sOrigName = (LPSTR)malloc((ilen + 1)*sizeof(char));
    strcpy(m_sOrigName, sName);

    m_wsName = DBStrToWChar(sName, m_pConnStruct->lDBEnc);

    m_pFields->BuildFromIndex(pConnStruct, wsTblName, m_wsName);

    m_iStatus = cisCreated;
    return;
}

void GIndex::AddFieldName(LPCSTR sName, bool bDesc, GTDFields *pFields)
{
    short iType = 0;
    GTDField *fld = pFields->GetByName(sName);
    if(fld) iType = fld->GetType();

    m_pFields->AddField(sName, bDesc, iType);
    return;
}

void GIndex::AddFieldPos(int iPos, bool bDesc, GTDFields *pFields)
{
    GTDField *fld = pFields->GetByTblPos(iPos);
    if(fld)
    {
        short iType = fld->GetType();
        m_pFields->AddField(fld->GetOrigNamePtr(), bDesc, iType);
    }
    return;
}

LPWSTR GIndex::GetNamePtr()
{
    return(m_wsName);
}

LPSTR GIndex::GetOrigNamePtr()
{
    return(m_sOrigName);
}

bool GIndex::GetPrimary()
{
    return(m_bPrimary);
}

bool GIndex::GetUnique()
{
    return(m_bUnique);
}

void GIndex::SetPrimary(bool bPrimary)
{
    m_bPrimary = bPrimary;
    if(m_bPrimary) m_bUnique = true;
    return;
}

GTDFields* GIndex::GetFieldsPtr()
{
    return(m_pFields);
}

void GIndex::SetStatus(CCollItemStatus iStatus)
{
    m_iStatus = iStatus;
}

void GIndex::UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
    LPWSTR wsNewTblName)
{
    m_wsTblName = wsNewTblName;
    m_pFields->UpdateTableName(sOrigSchema, sOrigTable, wsNewTblName);
    return;
}


//  GIndexes

GIndexes::GIndexes(IUnknown *pUnkOuter, ITypeLib *ALib) :
    _IGDynaCollection(true, pUnkOuter, ALib, 28)
{
#if DBGLEVEL > 1
    WriteLogFile("GIndexes::GIndexes-1\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_pConnStruct = NULL;
    m_wsTblName = NULL;
}

GIndexes::GIndexes(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    _IGDynaCollection(true, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GIndexes::GIndexes-2\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_pConnStruct = NULL;
    m_wsTblName = NULL;
}

GIndexes::~GIndexes()
{
#if DBGLEVEL > 1
    WriteLogFile("GIndexes::~GIndexes\r\n", true);
#endif // DBGLEVEL
}

HRESULT GIndexes::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GIndexes::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGDynaCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GIndexes))
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

ULONG GIndexes::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GIndexes::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GIndexes::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GIndexes::Append(IDispatch * Object)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndexes::Append\r\n", true);
#endif // DBGLEVEL
    if(m_iStatus == cisCreated)
    {
        GIndex *pIdx = (GIndex*)Object;
        int slen;
        LPSTR sCmd;
        if(pIdx->GetPrimary())
        {
            //TCHAR sMsg[128];
            //LoadString(m_pConnStruct->hInstance, IDE_CANNOTADDPRIMARYKEY, sMsg, 128);
            //m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            //return(S_FALSE);

            GTDFields* pFlds = pIdx->GetFieldsPtr();
            GTDField *pFld;

            slen = 32;
            if(pFlds->GetCount() > 0)
            {
                pFld = (GTDField*)pFlds->GetItem(0);
                slen += GetTableNameLen(pFld->GetOrigSchemaPtr(),
                    pFld->GetOrigTablePtr()) + strlen(pFld->GetOrigNamePtr()) + 8;
            }
            else return(S_FALSE);

            for(int i = 1; i < pFlds->GetCount(); i++)
            {
                slen += 4;
                pFld = (GTDField*)pFlds->GetItem(i);
                slen += (strlen(pFld->GetOrigNamePtr()) + 2);
            }

            sCmd = (LPSTR)malloc(slen*sizeof(char));

            pFld = (GTDField*)pFlds->GetItem(0);
            strcpy(sCmd, "ALTER TABLE ");
            CatTableName(sCmd, pFld->GetOrigSchemaPtr(),
                pFld->GetOrigTablePtr(), true);
            strcat(sCmd, " ADD PRIMARY KEY(");
            CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);

            for(int i = 1; i < pFlds->GetCount(); i++)
            {
                strcat(sCmd, ", ");
                pFld = (GTDField*)pFlds->GetItem(i);
                CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
            }
            strcat(sCmd, ")");

            bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);
            if(bRes) pIdx->SetStatus(cisCreated);
            else return(S_FALSE);
        }
        else
        {
            GTDFields* pFlds = pIdx->GetFieldsPtr();
            GTDField *pFld;

            slen = 24;
            if(pIdx->GetUnique()) slen += 8;
            slen += (strlen(pIdx->GetOrigNamePtr()) + 2);
            if(pFlds->GetCount() > 0)
            {
                pFld = (GTDField*)pFlds->GetItem(0);
                slen += (GetTableNameLen(pFld->GetOrigSchemaPtr(),
                    pFld->GetOrigTablePtr()) +
                    strlen(pFld->GetOrigNamePtr()) + 8);
                if(pFld->GetDesc()) slen += 5;
            }
            else return(S_FALSE);

            for(int i = 1; i < pFlds->GetCount(); i++)
            {
                slen += 4;
                pFld = (GTDField*)pFlds->GetItem(i);
                slen += (strlen(pFld->GetOrigNamePtr()) + 2);
                if(pFld->GetDesc()) slen += 5;
            }

            sCmd = (LPSTR)malloc(slen*sizeof(char));

            pFld = (GTDField*)pFlds->GetItem(0);
            strcpy(sCmd, "CREATE ");
            if(pIdx->GetUnique()) strcat(sCmd, "UNIQUE ");
            strcat(sCmd, "INDEX ");
            CatFieldName(sCmd, pIdx->GetOrigNamePtr(), true);
            strcat(sCmd, " ON ");
            CatTableName(sCmd, pFld->GetOrigSchemaPtr(),
                pFld->GetOrigTablePtr(), true);
            strcat(sCmd, " (");
            CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
            if(pFld->GetDesc()) strcat(sCmd, " DESC");

            for(int i = 1; i < pFlds->GetCount(); i++)
            {
                strcat(sCmd, ", ");
                pFld = (GTDField*)pFlds->GetItem(i);
                CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
                if(pFld->GetDesc()) strcat(sCmd, " DESC");
            }
            strcat(sCmd, ")");

            bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);
            if(bRes) pIdx->SetStatus(cisCreated);
            else return(S_FALSE);
        }

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }
    _IGCollection::AddItem((CCOMDispatch*)Object);
    return(S_OK);
}

HRESULT GIndexes::Delete(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GIndexes::Delete\r\n", true);
#endif // DBGLEVEL
    int idx = FindByWName(Name);
    if(idx < 0) return(S_FALSE);

    if(m_iStatus == cisCreated)
    {
        GIndex *pIdx = (GIndex*)_IGDynaCollection::GetItem(idx);

        if(pIdx->GetPrimary())
        {
            TCHAR sMsg[128];
            LoadString(m_pConnStruct->hInstance, IDE_CANNOTDELPRIMARYKEY, sMsg, 128);
            m_pErrors->HandleGDOError(sMsg, m_pConnStruct->hInstance);
            return(S_FALSE);
        }

        int slen = 18 + strlen(pIdx->GetOrigNamePtr());
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "DROP INDEX ");
        CatFieldName(sCmd, pIdx->GetOrigNamePtr(), true);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        if(!bRes) return(S_FALSE);

        slen = strlen("delete from  where tablename = $1") +
            strlen(m_pConnStruct->sModTables) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "delete from %s where tablename = $1",
            m_pConnStruct->sModTables);
        VARIANT vPar;
        vPar.vt = VT_BSTR;
        vPar.bstrVal = m_wsTblName;
        ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
        free(sCmd);
    }

    _IGDynaCollection::DeleteItem(idx);
    return(S_OK);
}

HRESULT GIndexes::get_Item(VARIANT index, GIndex* *ppGIndex)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GIndexes::get_Item\r\n", true);
    WriteVariantToLogFile(L"  index: ", index);
#endif // DBGLEVEL
    if(!ppGIndex) return(E_POINTER);
    // a "get_CollectionItem" method is probably an exception from the COM
    // standards - it can accept uninitialized return values, similary as
    // QueryInterface method
    //if(*ppGIndex) (*ppGIndex)->Release();

    *ppGIndex = NULL;
    int i = -1;
    GIndex *pIdx = NULL;
    if(index.vt == VT_BSTR)
        i = FindByWName(index.bstrVal);
    else if(index.vt == (VT_BYREF | VT_BSTR))
        i = FindByWName(*index.pbstrVal);
    else i = VarToInt(index);

    pIdx = (GIndex*)_IGCollection::GetItem(i);
    if(!pIdx) return(DISP_E_MEMBERNOTFOUND);

    pIdx->AddRef();
    *ppGIndex = pIdx;
    return(S_OK);
}

void GIndexes::ClearAll()
{
    _IGCollection::ClearAll();
    return;
}

int GIndexes::FindByWName(LPWSTR wsName)
{
    int i = 0;
    bool bFound = false;
    GIndex *pidx;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pidx = (GIndex*)m_pData[i++];
        if(pidx)
        {
            bFound = wcscmp(pidx->GetNamePtr(), wsName) == 0;
        }
    }
    return(bFound ? i - 1 : -1);
}

GIndex* GIndexes::GetByWName(LPWSTR wsName)
{
    int i = FindByWName(wsName);
    return((GIndex*)_IGCollection::GetItem(i));
}

int GIndexes::BuildFromConnection(PConnStruct pConnStruct, Oid lTblOid,
    LPWSTR wsTblName, GTDFields *pFields)
{
    m_iStatus = cisCreated;
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;

    GIndex *pid = NULL;

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

    SetSP(m_pConnStruct->pConn, bTrans);

    char sOid[32];
    sprintf(sOid, "%d", lTblOid);
    LPSTR psOid = sOid;
    PGresult *res = PQexecParams(m_pConnStruct->pConn, "select indexrelid, indnatts, indisunique, indisprimary, indkey \
from pg_index where indrelid = $1", 1, NULL, &psOid, NULL, NULL, 1);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        m_pErrors->HandlePQError(pConnStruct->pConn, pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return(0);
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
        return(0);
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int j = 0; j < ncols; j++) VariantInit(&pvRow[j]);

    int iIdx;
    long lL, lU;
    short *piFlds;

    for(long li = 0; li < nrows; li++)
    {
        GetRecordBin(res, li, ncols, piTypes, pvRow, m_pConnStruct->lDBEnc,
            m_pConnStruct->lGeomOid, NULL, m_pConnStruct->sDecSep);

        pid = new GIndex(NULL, m_pTypeLib);
        pid->SetTDAttrs(pConnStruct, VarToLong(pvRow[0]), VarToBool(pvRow[3]),
            VarToBool(pvRow[2]), true, wsTblName);

        int iflds = VarToInt(pvRow[1]);
        if((pvRow[4].vt & VT_ARRAY) > 0)
        {
            SafeArrayGetLBound(pvRow[4].parray, 1, &lL);
            SafeArrayGetUBound(pvRow[4].parray, 1, &lU);
            if(lU - lL + 1 == iflds)
            {
                SafeArrayAccessData(pvRow[4].parray, (void**)&piFlds);
                for(int j = 0; j < iflds; j++)
                {
                    if(piFlds[j] > 0)
                    {
                        pid->AddFieldPos(piFlds[j], false, pFields);
                    }
                }
                SafeArrayUnaccessData(pvRow[4].parray);
            }
        }
        else
        {
            iIdx = VarToInt(pvRow[4]);
            if(iIdx > 0)
            {
                pid->AddFieldPos(iIdx, false, pFields);
            }
        }
        _IGCollection::AddItem(pid);
    }

    for(int j = 0; j < ncols; j++) VariantClear(&pvRow[j]);
    free(pvRow);
    free(piTypes);
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    for(int i = 0; i < _IGCollection::GetCount(); i++)
    {
        pid = (GIndex*)_IGCollection::GetItem(i);
        pid->UpdateName();
    }

    return(_IGCollection::GetCount());
}

void GIndexes::SetNewAttrs(PConnStruct pConnStruct, LPWSTR wsTblName)
{
    m_wsTblName = wsTblName;
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_iStatus = cisNew;
    return;
}

GIndex* GIndexes::GetPrimaryKey()
{
    bool bFound = false;
    int i = 0;
    GIndex *pIdx;
    while(!bFound && (i < GetCount()))
    {
        pIdx = (GIndex*)GetItem(i++);
        bFound = pIdx->GetPrimary();
    }
    return(bFound ? pIdx : NULL);
}

GIndex* GIndexes::GetUniqueIdx()
{
    bool bFound = false;
    int i = 0;
    GIndex *pIdx;
    while(!bFound && (i < GetCount()))
    {
        pIdx = (GIndex*)GetItem(i++);
        bFound = pIdx->GetUnique();
    }
    return(bFound ? pIdx : NULL);
}

int GIndexes::BuildFromGAlias(PConnStruct pConnStruct, LPWSTR wsTblName,
    GTDFields *pFields)
{
    m_iStatus = cisCreated;
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    m_wsTblName = wsTblName;

    GIndex *pid = new GIndex(NULL, m_pTypeLib);
    pid->SetNameAttrs(pConnStruct, "GAlias_pkidx", true, true, false, wsTblName);
    pid->AddFieldName("TableType", false, pFields);
    _IGCollection::AddItem(pid);

    return(1);
}

void GIndexes::SetStatus(CCollItemStatus iStatus)
{
    m_iStatus = iStatus;
    return;
}

int GIndexes::CreatePseudoIndex(PConnStruct pConnStruct, LPSTR sOrigTblName,
    LPWSTR wsTblName, GTDFields *pFields)
{
    int iRes = 0;
    GIndex *pid = new GIndex(NULL, m_pTypeLib);

    LPSTR buf = (LPSTR)malloc((strlen(sOrigTblName) + 16)*sizeof(char));
    sprintf(buf, "%s_pkidx", sOrigTblName);
    pid->SetNameAttrs(pConnStruct, buf, true, true, false, wsTblName);
    free(buf);

    GTDField *pFld;
    for(int i = 0; i < pFields->GetCount(); i++)
    {
        pFld = (GTDField*)pFields->GetItem(i);
        if(pFld->IsKeyField())
        {
            pid->AddFieldName(pFld->GetOrigNamePtr(), false, pFields);
            iRes++;
        }
    }
    _IGCollection::AddItem(pid);
    return(iRes);
}

void GIndexes::UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
    LPWSTR wsNewTblName)
{
    m_wsTblName = wsNewTblName;
    GIndex *pid;

    for(int i = 0; i < _IGCollection::GetCount(); i++)
    {
        pid = (GIndex*)_IGCollection::GetItem(i);
        pid->UpdateTableName(sOrigSchema, sOrigTable, wsNewTblName);
    }

    return;
}
