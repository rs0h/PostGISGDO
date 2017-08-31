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

#include "PGSTableDef.hpp"
#include "../Common/LogFiles.hpp"
#include <stdio.h>
#include "../Common/VarUtils.hpp"
#include "../Common/pg_const.h"
#include "../Common/PGtoGDOmaps.hpp"

LPWSTR GetValidTableName(BSTR bsName, long lEnc, LPSTR *psSchema, LPSTR *psName)
{
    LPWSTR wsSchemaName = NULL;
    LPWSTR wsBuf = bsName;
    LPWSTR wsDot = wcschr(wsBuf, '.');

    int iLen;

    if(wsDot)
    {
        iLen = wsDot - bsName;
        wsBuf = (LPWSTR)malloc((iLen + 1)*sizeof(wchar_t));
        wcsncpy(wsBuf, bsName, iLen);
        wsBuf[iLen] = 0;
        wsSchemaName = GetValidFieldName(wsBuf, lEnc, psSchema);
        free(wsBuf);
        wsBuf = wsDot + 1;
    }

    LPWSTR wsTableName = GetValidFieldName(wsBuf, lEnc, psName);

    LPWSTR wsRes = wsTableName;
    if(wsSchemaName)
    {
        iLen = wcslen(wsTableName) + wcslen(wsSchemaName) + 1;
        wsRes = (LPWSTR)malloc((iLen + 1)*sizeof(wchar_t));
        wcscpy(wsRes, wsSchemaName);
        wcscat(wsRes, L".");
        wcscat(wsRes, wsTableName);
        free(wsTableName);
        free(wsSchemaName);
    }

    return(wsRes);
}


//  GTableDef

GTableDef::GTableDef(IUnknown *pUnkOuter, ITypeLib *pLib) : CCOMDispatch(pUnkOuter, pLib, 27)
{
#if DBGLEVEL > 1
    WriteLogFile("GTableDef::GTableDef-1\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_bBuild = false;
    m_sOrigSchema = NULL;
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_pFields = new GTDFields(this, m_pTypeLib);
    m_pIndices = new GIndexes(this, m_pTypeLib);
    m_wsPrimGeom = NULL;
    m_bHasUniqueIndex = false;
    m_iKeyCnt = 0;
    m_bView = false;
    m_bUpdatable = false;
}

GTableDef::GTableDef(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex) :
    CCOMDispatch(pUnkOuter, pLib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GTableDef::GTableDef-2\r\n", true);
#endif // DBGLEVEL
    m_iStatus = cisUnknown;
    m_bBuild = false;
    m_sOrigSchema = NULL;
    m_sOrigName = NULL;
    m_wsName = NULL;
    m_pConnStruct = NULL;
    m_pFields = new GTDFields(this, m_pTypeLib);
    m_pIndices = new GIndexes(this, m_pTypeLib);
    m_wsPrimGeom = NULL;
    m_bHasUniqueIndex = false;
    m_iKeyCnt = 0;
    m_bView = false;
    m_bUpdatable = false;
}

GTableDef::~GTableDef()
{
#if DBGLEVEL > 1
    if(m_wsName)
    {
        WriteLogFile("GTableDef::~GTableDef (", true);
        WriteLogFile(m_wsName, false);
        WriteLogFile(")\r\n", false);
    }
    else WriteLogFile("GTableDef::~GTableDef\r\n", true);
#endif // DBGLEVEL
    if(m_wsPrimGeom) free(m_wsPrimGeom);
    if(m_sOrigSchema) free(m_sOrigSchema);
    if(m_sOrigName) free(m_sOrigName);
    if(m_wsName) free(m_wsName);
    m_pFields->SetOuter(NULL);
    m_pIndices->SetOuter(NULL);
}

HRESULT GTableDef::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GTableDef::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GTableDef))
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

ULONG GTableDef::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GTableDef::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GTableDef::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GTableDef::get_GFields(GTDFields* *Fields)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_GFields\r\n", true);
#endif // DBGLEVEL
    if(!Fields) return(E_POINTER);
    if(*Fields) (*Fields)->Release();
    m_pFields->AddRef();
    *Fields = m_pFields;
    return(S_OK);
}

HRESULT GTableDef::get_GIndexes(GIndexes* *Indexes)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_GIndexes\r\n", true);
#endif // DBGLEVEL
    if(!Indexes) return(E_POINTER);
    if(*Indexes) (*Indexes)->Release();
    m_pIndices->AddRef();
    *Indexes = m_pIndices;
    return(S_OK);
}

HRESULT GTableDef::get_DateCreated(VARIANT *pDate)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_DateCreated\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GTableDef::get_LastUpdated(VARIANT *pLastUpdated)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_LastUpdated\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GTableDef::get_Name(BSTR *pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_Name\r\n", true);
#endif // DBGLEVEL
    if(!pName) return(E_POINTER);

    // probably another place where GeoMedia can pass an uninitialized
    // argument, thus increasing of probability of a potential memory leek
    //if(*pName) SysFreeString(*pName);

    *pName = SysAllocString(m_wsName);

#if DBGLEVEL > 0
    WriteLogFile("  ", false);
    WriteLogFile(*pName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT GTableDef::set_Name(BSTR pName)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::set_Name\r\n", true);
#endif // DBGLEVEL
    LPSTR sNewSchema = NULL;
    LPSTR sNewName = NULL;
    LPWSTR wsNewName = GetValidTableName(pName, m_pConnStruct->lDBEnc,
        &sNewSchema, &sNewName);

    if(m_wsName)
    {
        if(wcscmp(m_wsName, wsNewName) == 0)
        {
            if(sNewSchema) free(sNewSchema);
            free(sNewName);
            free(wsNewName);
            return(S_OK);
        }
    }

    if(m_iStatus == cisCreated)
    {
        // just make sure that all fields are populated
        BuildFromConnection();

        int slen = strlen(m_sOrigName) + 2;
        if(m_sOrigSchema) slen += (strlen(m_sOrigSchema) + 3);
        LPSTR sOldFullName = (LPSTR)malloc((slen + 1)*sizeof(char));
        sOldFullName[0] = '\"';
        if(m_sOrigSchema)
        {
            strcat(sOldFullName, m_sOrigSchema);
            strcat(sOldFullName, "\".\"");
        }
        strcat(sOldFullName, m_sOrigName);
        strcat(sOldFullName, "\"");

        slen = strlen(sNewName) + 2;
        if(sNewSchema) slen += (strlen(sNewSchema) + 3);
        LPSTR sNewFullName = (LPSTR)malloc((slen + 1)*sizeof(char));
        sNewFullName[0] = '\"';
        if(sNewSchema)
        {
            strcat(sNewFullName, sNewSchema);
            strcat(sNewFullName, "\".\"");
        }
        strcat(sNewFullName, sNewName);
        strcat(sNewFullName, "\"");

        slen = 40 + strlen(sOldFullName) + strlen(sNewFullName);
        LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "ALTER TABLE %s RENAME TO %s", sOldFullName, sNewFullName);
        bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
        free(sCmd);
        free(sNewFullName);
        free(sOldFullName);

        if(!bRes)
        {
            free(sNewSchema);
            free(sNewName);
            free(wsNewName);
            return(S_FALSE);
        }

        // if successfull, alter also related records in geometry_columns table
        slen = 105 + strlen(m_pConnStruct->sGeomCols);
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, "UPDATE %s SET f_table_schema = $1, f_table_name = $2 where \
f_table_schema = $3 and f_table_name = $4", m_pConnStruct->sGeomCols);

        char sPublic[8];
        strcpy(sPublic, "public");

        LPSTR sOldSchemaBuf = m_sOrigSchema;
        if(!sOldSchemaBuf) sOldSchemaBuf = sPublic;
        LPSTR sNewSchemaBuf = sNewSchema;
        if(!sNewSchemaBuf) sNewSchemaBuf = sPublic;

        Oid iParamTypes[4] = {VARCHAROID, VARCHAROID, VARCHAROID, VARCHAROID};
        char *ppParams[4] = {sNewSchemaBuf, sNewName, sOldSchemaBuf, m_sOrigName};

        bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
        SetSP(m_pConnStruct->pConn, bTrans);
        //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
        PGresult *res = PQexecParams(m_pConnStruct->pConn, sCmd, 4, iParamTypes,
            ppParams, NULL, NULL, 0);
        //free(lsCmd);
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
        free(sCmd);

        // if successfull, alter also related records in gdo_graphic_columns table
        slen = 130;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        strcpy(sCmd, "UPDATE gdo_graphic_columns SET f_table_schema = $1, \
f_table_name = $2 where f_table_schema = $3 and f_table_name = $4");

        SetSP(m_pConnStruct->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexecParams(m_pConnStruct->pConn, sCmd, 4, iParamTypes, ppParams,
            NULL, NULL, 0);
        //free(lsCmd);
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
        free(sCmd);
    }

    if(m_sOrigSchema) free(m_sOrigSchema);
    m_sOrigSchema = sNewSchema;
    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = sNewName;
    if(m_wsName) free(m_wsName);
    m_wsName = wsNewName;

    if(m_iStatus == cisCreated)
    {
        m_pFields->UpdateTableName(m_sOrigSchema, m_sOrigName, m_wsName);
        m_pIndices->UpdateTableName(m_sOrigSchema, m_sOrigName, m_wsName);
    }
    return(S_OK);
}

HRESULT GTableDef::get_RecordCount(long *pCount)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_RecordCount\r\n", true);
#endif // DBGLEVEL
    if(!pCount) return(E_POINTER);

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
    //SetSP(m_pConnStruct->pConn, bTrans);
    LPCWSTR wsMask = L"select count(*) from \"";
    int slen = wcslen(wsMask) + wcslen(m_wsName) + 4;
    LPWSTR wsCmd = (LPWSTR)malloc(slen*sizeof(wchar_t));
    wcscpy(wsCmd, wsMask);
    wcscat(wsCmd, m_wsName);
    wcscat(wsCmd, L"\"");
    LPSTR sCmd = WCharToDBStr(wsCmd, m_pConnStruct->lDBEnc);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
    //free(lsCmd);
    free(sCmd);
    free(wsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        m_pErrors->HandlePQError(m_pConnStruct->pConn,
            m_pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return(E_FAIL);
    }
    VARIANT V;
    VariantInit(&V);
    Oid iType;
    DescribeFieldTypes(res, 1, &iType);
    GetRecord(res, 0, 1, &iType, &V, m_pConnStruct->sDecSep);

    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);
    *pCount = VarToLong(V);
    PgVariantClear(&V);
    return(S_OK);
}

HRESULT GTableDef::get_Updatable(VARIANT_BOOL *pUpdatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::get_Updatable\r\n", true);
#endif // DBGLEVEL
    if(!pUpdatable) return(E_POINTER);
    *pUpdatable = VARIANT_TRUE;
    return(S_OK);
}

HRESULT GTableDef::CreateField(VARIANT Name, VARIANT Type, VARIANT Size,
    GTDField* *ppField)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::CreateField\r\n", true);
#endif // DBGLEVEL
    if(!ppField) return(E_POINTER);
    if(*ppField) (*ppField)->Release();

    ValidateVariant(&Name);

    *ppField = new GTDField(NULL, m_pTypeLib);
    (*ppField)->SetNewProps(m_pConnStruct, Name, Type, Size, 1, m_sOrigSchema,
        m_sOrigName, m_wsName);
    (*ppField)->AddRef();
    return(S_OK);
}

HRESULT GTableDef::CreateIndex(VARIANT Name, GIndex* *ppIndex)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::CreateIndex\r\n", true);
#endif // DBGLEVEL
    if(!ppIndex) return(E_POINTER);
    if(*ppIndex) (*ppIndex)->Release();

    ValidateVariant(&Name);

    *ppIndex = new GIndex(NULL, m_pTypeLib);
    (*ppIndex)->SetNewAttrs(m_pConnStruct, Name, m_wsName, m_sOrigSchema, m_sOrigName);
    (*ppIndex)->AddRef();
    return(S_OK);
}

HRESULT GTableDef::GetExtension(BSTR Name, IDispatch * *ppGExtension)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDef::GetExtension\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

void GTableDef::SetNewAttrs(PConnStruct pConnStruct, VARIANT Name)
{
    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;
    LPSTR sNewSchema = NULL;
    LPSTR sNewName = NULL;
    LPWSTR wsNewName = NULL;
    if(Name.vt == VT_BSTR)
    {
        wsNewName = GetValidTableName(Name.bstrVal, m_pConnStruct->lDBEnc,
            &sNewSchema, &sNewName);
    }
    else if(Name.vt == (VT_BSTR | VT_BYREF))
    {
        wsNewName = GetValidTableName(*Name.pbstrVal, m_pConnStruct->lDBEnc,
            &sNewSchema, &sNewName);
    }

    if(!wsNewName) return;

    if(m_wsName)
    {
        if(wcscmp(m_wsName, wsNewName) == 0)
        {
            free(sNewSchema);
            free(sNewName);
            free(wsNewName);
            return;
        }
    }

    if(m_sOrigSchema) free(m_sOrigSchema);
    m_sOrigSchema = sNewSchema;
    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = sNewName;
    if(m_wsName) free(m_wsName);
    m_wsName = wsNewName;

    m_pFields->SetNewAttrs(pConnStruct, m_wsName);
    m_pIndices->SetNewAttrs(pConnStruct, m_wsName);
    m_iStatus = cisNew;
    m_bUpdatable = true;
    return;
}

void GTableDef::SetTDAttrs(PConnStruct pConnStruct, LPCSTR sSchema,
    LPCSTR sName, bool bView)
{
    m_pConnStruct = pConnStruct;
    m_bView = bView;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;

    if(m_sOrigSchema) free(m_sOrigSchema);
    m_sOrigSchema = NULL;
    if(m_sOrigName) free(m_sOrigName);
    m_sOrigName = NULL;
    if(m_wsName) free(m_wsName);
    m_wsName = NULL;

    LPWSTR wsSchema = NULL;
    LPWSTR wsName = NULL;

    int ilen = strlen(sSchema);
    if(ilen > 0)
    {
        if(strcmp(sSchema, "public") != 0)
        {
            m_sOrigSchema = (LPSTR)malloc((ilen + 1)*sizeof(char));
            strcpy(m_sOrigSchema, sSchema);
            wsSchema = DBStrToWChar(m_sOrigSchema, m_pConnStruct->lDBEnc);
        }
    }

    ilen = strlen(sName);
    if(ilen > 0)
    {
        m_sOrigName = (LPSTR)malloc((ilen + 1)*sizeof(char));
        strcpy(m_sOrigName, sName);
        wsName = DBStrToWChar(m_sOrigName, m_pConnStruct->lDBEnc);
    }

    if(!wsName)
    {
        if(wsSchema) free(wsSchema);
        return;
    }

    ilen = wcslen(wsName);
    if(wsSchema) ilen += (wcslen(wsSchema) + 1);

    m_wsName = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
    if(wsSchema)
    {
        wcscpy(m_wsName, wsSchema);
        wcscat(m_wsName, L".");
        wcscat(m_wsName, wsName);
        free(wsSchema);
    }
    else wcscpy(m_wsName, wsName);
    free(wsName);

#if DBGLEVEL > 0
    WriteLogFile("GTableDef::SetTDAttrs\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(m_wsName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    m_iStatus = cisCreated;

    return;
}

void GTableDef::BuildKeys()
{
    m_iKeyCnt = 0;
    GIndex *pIdx = m_pIndices->GetPrimaryKey();
    if(!pIdx) pIdx = m_pIndices->GetUniqueIdx();

    m_bHasUniqueIndex = true;

    if(pIdx)
    {
        m_pFields->ResetKeyFields();
        m_iKeyCnt = m_pFields->SetKeyFields(pIdx);
    }
    else
    {
        m_bHasUniqueIndex = false;
        if(!m_pFields->HasKeyField())
        {
            GTDField *ptdFld = m_pFields->GetAutoNumFld();
            if(ptdFld)
            {
                ptdFld->SetKeyField(true);
                //m_iKeyCnt = 1;
            }
        }
        if(m_pFields->HasKeyField())
        {
            m_iKeyCnt = m_pIndices->CreatePseudoIndex(m_pConnStruct,
                m_sOrigName, m_wsName, m_pFields);
            m_bHasUniqueIndex = (m_iKeyCnt > 0);
        }
    }
    if(m_iKeyCnt > 10) m_iKeyCnt = 10;
    return;
}

int GTableDef::BuildFromConnection()
{
    if(m_bBuild) return(0);

    if(m_wsPrimGeom) free(m_wsPrimGeom);
    m_wsPrimGeom = NULL;

    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
    //m_lOid = GetOid(m_pConnStruct->pConn, m_sName, bTrans,
    //    m_pConnStruct->lDBEnc);

    if(m_pConnStruct->bFtrPrepared)
    {
        LPSTR sPar = WCharToDBStr(m_wsName, m_pConnStruct->lDBEnc);
        SetSP(m_pConnStruct->pConn, bTrans);
        PGresult *res = PQexecPrepared(m_pConnStruct->pConn, "stmtftr", 1,
            &sPar, NULL, NULL, 1);
        free(sPar);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Spot 6: ", m_pConnStruct->pConn);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
            return(0);
        }

        long nrows = PQntuples(res);
        if(nrows > 0)
        {
            int ncols = PQnfields(res);
            Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
            DescribeFieldTypes(res, ncols, piTypes);

            VARIANT *pvVals = (VARIANT*)malloc(ncols*sizeof(VARIANT));
            for(int i = 0; i < ncols; i++) VariantInit(&pvVals[i]);
            GetRecordBin(res, 0, ncols, piTypes, pvVals, m_pConnStruct->lDBEnc,
                m_pConnStruct->lGeomOid, NULL, m_pConnStruct->sDecSep);

            int iPos = PQfnumber(res, "primarygeometryfieldname");
            if(iPos > -1)
            {
                if(pvVals[iPos].vt == VT_BSTR)
                {
                    LPSTR sName = NULL;
                    m_wsPrimGeom = GetValidFieldName(pvVals[iPos].bstrVal,
                        m_pConnStruct->lDBEnc, &sName);
                    if(sName) free(sName);
                }
            }

            for(int i = 0; i < ncols; i++) VariantClear(&pvVals[i]);
            free(pvVals);
            free(piTypes);
        }
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);
    }

    m_pFields->BuildFromConnection(m_pConnStruct, m_lOid, m_sOrigSchema,
        m_sOrigName, m_wsName, m_bView);
    m_pIndices->BuildFromConnection(m_pConnStruct, m_lOid, m_wsName, m_pFields);

    BuildKeys();

    // if there is no index, we can create a fake one. We can do it in the
    // future, if requested
    //if(!m_bHasUniqueIndex)
    //{
    //}

    char sSchema[128];
    if(m_sOrigSchema) strcpy(sSchema, m_sOrigSchema);
    else strcpy(sSchema, "public");

    m_bUpdatable = IsTableWritable(m_pConnStruct->pConn, sSchema, m_sOrigName, bTrans);

    m_bBuild = true;
    return(0);
}

LPWSTR GTableDef::GetNamePtr()
{
    return(m_wsName);
}

LPSTR GTableDef::GetOrigSchemaPtr()
{
    return(m_sOrigSchema);
}

LPSTR GTableDef::GetOrigNamePtr()
{
    return(m_sOrigName);
}

GIndexes* GTableDef::GetIndexesPtr()
{
    return(m_pIndices);
}

GTDFields* GTableDef::GetFieldsPtr()
{
    return(m_pFields);
}

void GTableDef::SetStatus(CCollItemStatus iStatus)
{
    m_iStatus = iStatus;
    GTDField *pFld;
    for(int i = 0; i < m_pFields->GetCount(); i++)
    {
        pFld = (GTDField*)m_pFields->GetItem(i);
        if(!pFld->IsNativeGeometry()) pFld->SetStatus(iStatus);
    }
    return;
}

GTDField* GTableDef::GetPrimGeomFldPtr()
{
    GTDField *pFld = NULL;
    bool bFound = false;
    int i = 0;
    if(m_wsPrimGeom)
    {
        pFld = m_pFields->GetByWName(m_wsPrimGeom);
        bFound = (pFld != NULL);
    }
    else
    {
        while(!bFound && (i < m_pFields->GetCount()))
        {
            pFld = (GTDField*)m_pFields->GetItem(i++);
            bFound = (pFld->GetType() > 31);
        }
    }
    return(bFound ? pFld : NULL);
}

void GTableDef::BuildGAlias()
{
    if(m_bBuild) return;
    m_lOid = (Oid)-1;

    m_pFields->BuildFromGAlias(m_pConnStruct, m_sOrigSchema, m_sOrigName,
        m_wsName);
    m_pIndices->BuildFromGAlias(m_pConnStruct, m_wsName, m_pFields);

    m_bBuild = true;
    return;
}

unsigned long GTableDef::GetSrid(LPSTR sGeomName)
{
    GTDField *pFld = m_pFields->GetByName(sGeomName);
    if(pFld) return(pFld->GetSrid());
    return(0);
}

void GTableDef::SetBuild(bool bBuild)
{
    m_bBuild = bBuild;
}

Oid GTableDef::GetTableOid()
{
    return(m_lOid);
}

void GTableDef::RefreshOid()
{
    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);
    m_lOid = GetOid(m_pConnStruct->pConn, m_sOrigSchema, m_sOrigName, bTrans,
        m_pConnStruct->lDBEnc);
    return;
}

void GTableDef::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    m_pFields->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    return;
}

bool GTableDef::GetHasUniqueIndex()
{
    return(m_bHasUniqueIndex);
}

int GTableDef::GetKeyCount()
{
    return(m_iKeyCnt);
}

GTDField* GTableDef::GetFieldPtrByTblPos(int iPos)
{
    return(m_pFields->GetByTblPos(iPos));
}

bool GTableDef::GetUpdatable()
{
    return(m_bUpdatable);
}


//  GTableDefs

GTableDefs::GTableDefs(IUnknown *pUnkOuter, ITypeLib *ALib) :
    _IGDynaCollection(true, pUnkOuter, ALib, 30)
{
#if DBGLEVEL > 1
    WriteLogFile("GTableDefs::GTableDefs-1\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
}

GTableDefs::GTableDefs(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex) :
    _IGDynaCollection(true, pUnkOuter, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GTableDefs::GTableDefs-2\r\n", true);
#endif // DBGLEVEL
    m_pConnStruct = NULL;
}

GTableDefs::~GTableDefs()
{
#if DBGLEVEL > 1
    WriteLogFile("GTableDefs::~GTableDefs\r\n", true);
#endif // DBGLEVEL
}

HRESULT GTableDefs::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GTableDefs::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = _IGDynaCollection::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GTableDefs))
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

ULONG GTableDefs::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GTableDefs::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GTableDefs::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT GTableDefs::Append(IDispatch * Object)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDefs::Append\r\n", true);
#endif // DBGLEVEL
    GTableDef *pTbl = (GTableDef*)Object;
    GIndexes *pIdxs = pTbl->GetIndexesPtr();
    GIndex *pPrimIdx = pIdxs->GetPrimaryKey();
    GIndex *pIdx;
    GTDFields *pFlds = pTbl->GetFieldsPtr();
    GTDFields *pIdxFlds = NULL;
    if(pPrimIdx) pIdxFlds = pPrimIdx->GetFieldsPtr();
    GTDField *pIdxFld = NULL;
    if(pIdxFlds)
    {
        if(pIdxFlds->GetCount() == 1)
            pIdxFld = (GTDField*)pIdxFlds->GetItem(0);
    }
    GTDField *pFld = NULL;
    char sType[128];
    long lGeomType;

    int slen = 20;
    slen += GetTableNameLen(pTbl->GetOrigSchemaPtr(), pTbl->GetOrigNamePtr());
    int i = 0;
    bool bFound = false;
    while(!bFound && (i < pFlds->GetCount()))
    {
        pFld = (GTDField*)pFlds->GetItem(i++);
        bFound = !pFld->IsNativeGeometry();
    }

    if(bFound)
    {
        slen += strlen(pFld->GetOrigNamePtr());
        slen += 5;
        pFld->GetSQLTypeStr(sType);
        slen += strlen(sType);
        if(pIdxFld)
        {
            if(wcscmp(pIdxFld->GetNamePtr(), pFld->GetNamePtr()) == 0)
            {
                if(!pFld->GetRequired()) slen += 9;
                slen += 12;
            }
        }
    }

    while(i < pFlds->GetCount())
    {
        pFld = (GTDField*)pFlds->GetItem(i++);
        if(!pFld->IsNativeGeometry())
        {
            slen += strlen(pFld->GetOrigNamePtr());
            slen += 5;
            pFld->GetSQLTypeStr(sType);
            slen += strlen(sType);
            if(pIdxFld)
            {
                if(wcscmp(pIdxFld->GetNamePtr(), pFld->GetNamePtr()) == 0)
                {
                    if(!pFld->GetRequired()) slen += 9;
                    slen += 12;
                }
            }
        }
        else
        {
            lGeomType = pFld->GetGeomType();
            if((lGeomType == gdbPoint) || (lGeomType == gdbAnySpatial))
            {
                slen += strlen(pFld->GetOrigNamePtr());
                slen += 5;
                slen += 10; // "_igr bytea"
            }
        }
    }

    if(pIdxFlds && !pIdxFld)
    {
        slen += 16;
        if(pIdxFlds->GetCount() > 0)
        {
            pIdxFld = (GTDField*)pIdxFlds->GetItem(0);
            slen += (strlen(pIdxFld->GetOrigNamePtr()) + 4);
        }
        for(i = 1; i < pIdxFlds->GetCount(); i++)
        {
            slen += 2;
            pIdxFld = (GTDField*)pIdxFlds->GetItem(i);
            slen += (strlen(pIdxFld->GetOrigNamePtr()) + 4);
        }
        pIdxFld = NULL;
    }

    int iTblPos = 1;
    LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));

    strcpy(sCmd, "CREATE TABLE ");
    CatTableName(sCmd, pTbl->GetOrigSchemaPtr(), pTbl->GetOrigNamePtr(), true);
    strcat(sCmd, "(");

    i = 0;
    bFound = false;
    while(!bFound && (i < pFlds->GetCount()))
    {
        pFld = (GTDField*)pFlds->GetItem(i++);
        bFound = !pFld->IsNativeGeometry();
    }

    if(bFound)
    {
        pFld->SetTblPos(iTblPos++);
        CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
        strcat(sCmd, " ");
        pFld->GetSQLTypeStr(sType);
        strcat(sCmd, sType);
        if(pIdxFld)
        {
            if(wcscmp(pIdxFld->GetNamePtr(), pFld->GetNamePtr()) == 0)
            {
                if(!pFld->GetRequired())
                {
                    strcat(sCmd, " NOT NULL");
                }
                strcat(sCmd, " PRIMARY KEY");
            }
        }
    }

    while(i < pFlds->GetCount())
    {
        pFld = (GTDField*)pFlds->GetItem(i++);
        if(!pFld->IsNativeGeometry())
        {
            pFld->SetTblPos(iTblPos++);
            strcat(sCmd, ", ");
            CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
            strcat(sCmd, " ");
            pFld->GetSQLTypeStr(sType);
            strcat(sCmd, sType);
            if(pIdxFld)
            {
                if(wcscmp(pIdxFld->GetNamePtr(), pFld->GetNamePtr()) == 0)
                {
                    if(!pFld->GetRequired())
                    {
                        strcat(sCmd, " NOT NULL");
                    }
                    strcat(sCmd, " PRIMARY KEY");
                }
            }
        }
        else
        {
            lGeomType = pFld->GetGeomType();
            if((lGeomType == gdbPoint) || (lGeomType == gdbAnySpatial))
            {
                strcat(sCmd, ", \"");
                strcat(sCmd, pFld->GetOrigNamePtr());
                strcat(sCmd, "_igr\" bytea");
                pFld->SetHasSpecField(1);
                iTblPos++;
            }
        }
    }

    if(pIdxFlds && !pIdxFld)
    {
        strcat(sCmd, ", PRIMARY KEY (");
        if(pIdxFlds->GetCount() > 0)
        {
            pIdxFld = (GTDField*)pIdxFlds->GetItem(0);
            CatFieldName(sCmd, pIdxFld->GetOrigNamePtr(), true);
        }
        for(i = 1; i < pIdxFlds->GetCount(); i++)
        {
            strcat(sCmd, ", ");
            pIdxFld = (GTDField*)pIdxFlds->GetItem(i);
            CatFieldName(sCmd, pIdxFld->GetOrigNamePtr(), true);
        }
        strcat(sCmd, ")");
        pIdxFlds = NULL;
    }

    strcat(sCmd, ")");

    bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
    free(sCmd);
    if(!bRes) return(S_FALSE);

    pTbl->SetStatus(cisCreated);
    pTbl->SetBuild(true);
    pTbl->RefreshOid();
    if(pPrimIdx) pPrimIdx->SetStatus(cisCreated);
    pIdxs->SetStatus(cisCreated);
    pFlds->SetStatus(cisCreated);

    char sSchemaBuf[8];
    strcpy(sSchemaBuf, "public");

    LPSTR sSchemaName = pTbl->GetOrigSchemaPtr();
    if(!sSchemaName) sSchemaName = sSchemaBuf;
    LPSTR sTblName = pTbl->GetOrigNamePtr();

    char sGeomType[32];
    bRes = true;
    i = 0;
    while(bRes && (i < pFlds->GetCount()))
    {
        pFld = (GTDField*)pFlds->GetItem(i++);
        if(pFld->IsNativeGeometry())
        {
            pFld->GetGeomTypeStr(sGeomType);

            slen = 50 + strlen(sSchemaName) + strlen(sTblName) +
                strlen(pFld->GetOrigNamePtr()) + 32 + strlen(sGeomType);
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "SELECT AddGeometryColumn('%s', '%s', '%s', %ld, '%s', 3)",
                sSchemaName, sTblName, pFld->GetOrigNamePtr(), pFld->GetSrid(),
                sGeomType);
            bRes = ExecuteCommand(m_pConnStruct, sCmd);
            if(bRes)
            {
                pFld->SetTblPos(iTblPos++);
                pFld->SetStatus(cisCreated);
            }
            free(sCmd);
        }
        else if(pFld->IsEmulGeometry() && m_pConnStruct->sGraphicColumns[0])
        {
            slen = strlen("delete from  where f_table_schema = '' and \
f_table_name = '' and f_graphic_column = ''") +
                strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "delete from %s where f_table_schema = '%s' and \
f_table_name = '%s' and f_graphic_column = '%s'", m_pConnStruct->sGraphicColumns,
                sSchemaName, sTblName, pFld->GetOrigNamePtr());
            ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);

            pFld->GetGeomTypeStr(sGeomType);

            slen = strlen("insert into  values('', '', '', 3, , '', '', '')") +
                strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 32 +
                strlen(sGeomType);
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "insert into %s values('%s', '%s', '%s', 3, %ld, '%s', '', '')",
                m_pConnStruct->sGraphicColumns, sSchemaName, sTblName,
                pFld->GetOrigNamePtr(), pFld->GetSrid(), sGeomType);
            ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);

            if(pFld->AddNativeField()) iTblPos++;
        }
    }
    if(!bRes) return(S_FALSE);

    for(int j = 0; j < pIdxs->GetCount(); j++)
    {
        pIdx = (GIndex*)pIdxs->GetItem(j);
        if(pIdx != pPrimIdx)
        {
            pFlds = pIdx->GetFieldsPtr();

            slen = 24;
            if(pIdx->GetUnique()) slen += 8;
            slen += (strlen(pIdx->GetOrigNamePtr()) + 2);
            slen += GetTableNameLen(pTbl->GetOrigSchemaPtr(),
                pTbl->GetOrigNamePtr());
            if(pFlds->GetCount() > 0)
            {
                pFld = (GTDField*)pFlds->GetItem(0);
                slen += (strlen(pFld->GetOrigNamePtr()) + 2);
                if(pFld->GetDesc()) slen += 5;
            }
            for(i = 1; i < pFlds->GetCount(); i++)
            {
                slen += 2;
                pFld = (GTDField*)pFlds->GetItem(i);
                slen += (strlen(pFld->GetOrigNamePtr()) + 2);
                if(pFld->GetDesc()) slen += 5;
            }

            sCmd = (LPSTR)malloc(slen*sizeof(char));

            strcpy(sCmd, "CREATE ");
            if(pIdx->GetUnique()) strcat(sCmd, "UNIQUE ");
            strcat(sCmd, "INDEX ");
            CatFieldName(sCmd, pIdx->GetOrigNamePtr(), true);
            strcat(sCmd, " ON ");
            CatTableName(sCmd, pTbl->GetOrigSchemaPtr(),
                pTbl->GetOrigNamePtr(), true);
            strcat(sCmd, " (");

            if(pFlds->GetCount() > 0)
            {
                pFld = (GTDField*)pFlds->GetItem(0);
                CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
                if(pFld->GetDesc()) strcat(sCmd, " DESC");
            }
            for(i = 1; i < pFlds->GetCount(); i++)
            {
                strcat(sCmd, ", ");
                pFld = (GTDField*)pFlds->GetItem(i);
                CatFieldName(sCmd, pFld->GetOrigNamePtr(), true);
                if(pFld->GetDesc()) strcat(sCmd, " DESC");
            }
            strcat(sCmd, ")");

            ExecuteCommand(m_pConnStruct, sCmd);
            free(sCmd);
        }
    }
    pFlds->Sort();

    pTbl->BuildKeys();

    _IGCollection::AddItem((CCOMDispatch*)Object);
    return(S_OK);
}

HRESULT GTableDefs::Delete(BSTR Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GTableDefs::Delete\r\n", true);
#endif // DBGLEVEL
    int idx = FindByWName(Name);
    if(idx < 0) return(S_FALSE);

    int slen;
    GTableDef *pTbl = (GTableDef*)m_pData[idx];

    slen = 32 + GetTableNameLen(pTbl->GetOrigSchemaPtr(),
        pTbl->GetOrigNamePtr());

    LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
    strcpy(sCmd, "DROP TABLE ");
    CatTableName(sCmd, pTbl->GetOrigSchemaPtr(), pTbl->GetOrigNamePtr(), true);
    bool bRes = ExecuteCommand(m_pConnStruct, sCmd);
    free(sCmd);
    if(!bRes) return(S_FALSE);

    char sSchemaBuf[8];
    strcpy(sSchemaBuf, "public");

    LPSTR sSchemaName = pTbl->GetOrigSchemaPtr();
    if(!sSchemaName) sSchemaName = sSchemaBuf;

    LPSTR sTblName = pTbl->GetOrigNamePtr();

    if(m_pConnStruct->sGraphicColumns[0])
    {
        GTDField *pFld = NULL;
        GTDFields *pFlds = pTbl->GetFieldsPtr();
        for(int i = 0; i < pFlds->GetCount(); i++)
        {
            pFld = (GTDField*)pFlds->GetItem(i);
            if(pFld->IsEmulGeometry())
            {
                slen = strlen("delete from  where f_table_schema = '' and \
f_table_name = '' and f_graphic_column = ''") +
                    strlen(m_pConnStruct->sGraphicColumns) + strlen(sSchemaName) +
                    strlen(sTblName) + strlen(pFld->GetOrigNamePtr()) + 1;
                sCmd = (LPSTR)malloc(slen*sizeof(char));
                sprintf(sCmd, "delete from %s where f_table_schema = '%s' and \
f_table_name = '%s' and f_graphic_column = '%s'", m_pConnStruct->sGraphicColumns,
                    sSchemaName, sTblName, pFld->GetOrigNamePtr());
                ExecuteCommand(m_pConnStruct, sCmd);
                free(sCmd);
            }
        }
    }

    slen = strlen("delete from  where tablename = $1") +
        strlen(m_pConnStruct->sModTables) + 1;
    sCmd = (LPSTR)malloc(slen*sizeof(char));
    sprintf(sCmd, "delete from %s where tablename = $1",
        m_pConnStruct->sModTables);

    VARIANT vPar;
    vPar.vt = VT_BSTR;
    vPar.bstrVal = pTbl->GetNamePtr();
    ExecuteCommandParams(m_pConnStruct, sCmd, 1, &vPar);
    free(sCmd);

    _IGDynaCollection::DeleteItem(idx);
    return(S_OK);
}

HRESULT GTableDefs::get_Item(VARIANT index, GTableDef* *ppGTableDef)
{
    ValidateVariant(&index);

#if DBGLEVEL > 0
    WriteLogFile("GTableDefs::get_Item\r\n", true);
    WriteVariantToLogFile(L"  index: ", index);
#endif // DBGLEVEL
    if(!ppGTableDef) return(E_POINTER);
    // a "get_CollectionItem" method is probably an exception from the COM
    // standards - it can accept uninitialized return values, similary as
    // QueryInterface method
    //if(*ppGTableDef) (*ppGTableDef)->Release();

    *ppGTableDef = NULL;
    int i = -1;
    if(index.vt == VT_BSTR) i = FindByWName(index.bstrVal);
    else if(index.vt == (VT_BYREF | VT_BSTR)) i = FindByWName(*index.pbstrVal);
    else i = VarToInt(index);

    GTableDef *pTD = (GTableDef*)_IGCollection::GetItem(i);
    if(!pTD) return(DISP_E_MEMBERNOTFOUND);

    pTD->AddRef();
    pTD->BuildFromConnection();
    *ppGTableDef = pTD;
    return(S_OK);
}

void GTableDefs::ClearAll()
{
    _IGCollection::ClearAll();
    m_pConnStruct = NULL;
    return;
}

int GTableDefs::BuildFromConnection(PConnStruct pConnStruct)
{
    if(m_pConnStruct) return(_IGCollection::GetCount());

    m_pConnStruct = pConnStruct;
    m_pErrors = (GErrors*)m_pConnStruct->pErrors;

    GTableDef *ptd;
    bool bAliasTable = false;
    bool bTrans = (m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans);

    SetSP(m_pConnStruct->pConn, bTrans);
    PGresult *res = PQexec(m_pConnStruct->pConn, "select * from information_schema.tables where \
not table_schema='pg_catalog' and not table_schema='information_schema'");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        m_pErrors->HandlePQError(pConnStruct->pConn, m_pConnStruct->hInstance);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return(0);
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    int iSchmPos = PQfnumber(res, "table_schema");
    int iNamePos = PQfnumber(res, "table_name");
    int iTypePos = PQfnumber(res, "table_type");
    bool bView;

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);

    long nrows = PQntuples(res);
    for(long li = 0; li < nrows; li++)
    {
        GetRecord(res, li, ncols, piTypes, pvRow, pConnStruct->sDecSep);
        if(pvRow[iNamePos].vt == VT_LPSTR)
        {
            if((stricmp(pvRow[iNamePos].pcVal, "geography_columns") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "geometry_columns") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "gdo_graphic_columns") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "spatial_ref_sys") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "gfeaturesbase") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "raster_columns") != 0) &&
                (stricmp(pvRow[iNamePos].pcVal, "raster_overviews") != 0) &&
                (stricmp(pvRow[iSchmPos].pcVal, "topology") != 0))
            {
                bView = false;
                if(pvRow[iTypePos].vt == VT_LPSTR)
                {
                    bView = (stricmp(pvRow[iTypePos].pcVal, "VIEW") == 0);
                }

                if(stricmp(pvRow[iNamePos].pcVal, "galiastable") == 0)
                    bAliasTable = true;
                ptd = new GTableDef(NULL, m_pTypeLib);
                ptd->SetTDAttrs(pConnStruct, pvRow[iSchmPos].pcVal,
                    pvRow[iNamePos].pcVal, bView);
                _IGCollection::AddItem(ptd);
            }
        }
    }
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);

    for(int i = 0; i < ncols; i++) PgVariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    for(int i = 0; i < GetCount(); i++)
    {
        ptd = (GTableDef*)GetItem(i);
        ptd->RefreshOid();
    }

    if(!bAliasTable)
    {
        ptd = new GTableDef(NULL, m_pTypeLib);
        ptd->SetTDAttrs(pConnStruct, "public", pConnStruct->sGAlias, false);
        _IGCollection::AddItem(ptd);
        ptd->BuildGAlias();
    }

    return(_IGCollection::GetCount());
}

/*int GTableDefs::FindByName(LPSTR sName)
{
    int i = 0;
    bool bFound = false;
    GTableDef *pTbl;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pTbl = (GTableDef*)m_pData[i++];
        bFound = strcmp(pTbl->GetNamePtr(), sName) == 0;
    }
    return(bFound ? i - 1 : -1);
}*/

int GTableDefs::FindByWName(BSTR bsName)
{
    int i = 0;
    bool bFound = false;
    GTableDef *pTbl;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pTbl = (GTableDef*)m_pData[i++];
        bFound = wcscmp(pTbl->GetNamePtr(), bsName) == 0;
    }
    return(bFound ? i - 1 : -1);
}

/*unsigned long GTableDefs::GetGeomSrid(LPSTR sName, LPSTR sGeomName)
{
    int idx = FindByName(sName);
    if(idx < 0) return(0);

    GTableDef *pTbl = (GTableDef*)m_pData[idx];
    pTbl->BuildFromConnection();
    return(pTbl->GetSrid(sGeomName));
}*/

GTableDef* GTableDefs::FindByOid(Oid lTblOid)
{
    int i = 0;
    bool bFound = false;
    GTableDef *pTbl;
    while(!bFound && (i < _IGCollection::GetCount()))
    {
        pTbl = (GTableDef*)m_pData[i++];
        bFound = (pTbl->GetTableOid() == lTblOid);
    }
    return(bFound ? pTbl : NULL);
}

void GTableDefs::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    GTableDef *pTD;
    for(int i = 0; i < GetCount(); i++)
    {
        pTD = (GTableDef*)GetItem(i);
        pTD->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    }
    return;
}
