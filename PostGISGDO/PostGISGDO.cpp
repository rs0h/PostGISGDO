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

#include "../Common/LogFiles.hpp"
#include "PostGISGDO.hpp"
#include "PostGIS.rh"
#include "../Common/VarUtils.hpp"

#include <stdio.h>
#include <tchar.h>


//  DBExtendedPropertySet

DBExtendedPropertySet::DBExtendedPropertySet(GDatabase *pDB,
    ITypeLib *ALib) : CCOMDispatch(pDB, ALib, 0)
{
#if DBGLEVEL > 1
    WriteLogFile("DBExtendedPropertySet::DBExtendedPropertySet-1\r\n", true);
#endif // DBGLEVEL
}

DBExtendedPropertySet::DBExtendedPropertySet(GDatabase *pDB,
    ITypeLib *ALib, int iIndex) : CCOMDispatch(pDB, ALib, iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("DBExtendedPropertySet::DBExtendedPropertySet-2\r\n", true);
#endif // DBGLEVEL
}

DBExtendedPropertySet::~DBExtendedPropertySet()
{
#if DBGLEVEL > 1
    WriteLogFile("DBExtendedPropertySet::~DBExtendedPropertySet\r\n", true);
#endif // DBGLEVEL
}

HRESULT DBExtendedPropertySet::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("DBExtendedPropertySet::QueryInterface\r\n", true);
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

ULONG DBExtendedPropertySet::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "DBExtendedPropertySet::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "DBExtendedPropertySet::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(pUnkOuter || (lRest > 0)) return lRest;
    delete(this);
    return(0);
}

HRESULT DBExtendedPropertySet::GetValue(BSTR PropertyName, VARIANT *Value)
{
#if DBGLEVEL > 0
    WriteLogFile("DBExtendedPropertySet::GetValue\r\n  Name: ", true);
    WriteLogFile(PropertyName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    GDatabase *pDB = (GDatabase*)m_pUnkOuter;
    if(wcsicmp(PropertyName, L"SessionID") == 0)
    {
        if(!Value) return(E_POINTER);
        VariantClear(Value);
        Value->vt = VT_I4;
        Value->lVal = (long)pDB;
        return(S_OK);
    }
    if(wcsicmp(PropertyName, L"DisableModificationLogging") == 0)
    {
        if(!Value) return(E_POINTER);
        VariantClear(Value);
        Value->vt = VT_BOOL;
        if(pDB->GetModificationLogging()) Value->boolVal = VARIANT_FALSE;
        else Value->boolVal = VARIANT_TRUE;
        return(S_OK);
    }
    if(wcsicmp(PropertyName, L"gdbIndexIntersect") == 0)
    {
        if(!Value) return(E_POINTER);
        VariantClear(Value);
        Value->vt = VT_BOOL;
        Value->boolVal = VARIANT_TRUE;
        return(S_OK);
    }
    if(wcsicmp(PropertyName, L"DefaultCoordinateSystem") == 0)
    {
        if(!Value) return(E_POINTER);
        pDB->GetDefCS(Value);
        if(Value->vt < VT_NULL) Value->vt = VT_NULL;
        return(S_OK);
    }
    if(wcsicmp(PropertyName, L"ConnectPropertyIsDynamic") == 0)
    {
        if(!Value) return(E_POINTER);
        Value->vt = VT_BOOL;
        Value->boolVal = VARIANT_FALSE;
        return(S_OK);
    }
    return(E_NOTIMPL);
}

HRESULT DBExtendedPropertySet::SetValue(BSTR PropertyName, VARIANT Value, long *Status)
{
#if DBGLEVEL > 0
    WriteLogFile("DBExtendedPropertySet::SetValue\r\n  Name: ", true);
    WriteLogFile(PropertyName, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    GDatabase *pDB = (GDatabase*)m_pUnkOuter;
    if(wcsicmp(PropertyName, L"DisableModificationLogging") == 0)
    {
        pDB->SetModificationLogging(!VarToBool(Value));
        return(S_OK);
    }
    if(wcsicmp(PropertyName, L"DefaultCoordinateSystem") == 0)
    {
        if(pDB->SetDefCS(Value)) return(S_OK);
        return(E_NOTIMPL);
    }
    return(E_NOTIMPL);
}


//  GDatabase

GDatabase::GDatabase(HINSTANCE hInstance, IUnknown *pUnkOuter, ITypeLib **ppLibs) :
    CCOMDispatch(pUnkOuter, ppLibs[0], 32)
{
#if DBGLEVEL > 1
    WriteLogFile("GDatabase::GDatabase-1\r\n", true);
#endif // DBGLEVEL
    m_pErrors = new GErrors(this, ppLibs[0]); //, 31);

    m_cConnStruct.hInstance = hInstance;
    m_cConnStruct.pConn = NULL;
    m_cConnStruct.pErrors = (LONG_PTR)m_pErrors;
    LoadCSTable(hInstance, &m_cConnStruct.cCSTable);
    m_cConnStruct.wsServerName = NULL;
    m_cConnStruct.lSessionId = (long)this;
    m_cConnStruct.ppTypeLibs = (LONG_PTR)ppLibs;
    m_cConnStruct.sGFeatures[0] = 0;
    m_cConnStruct.sFieldLookup[0] = 0;
    m_cConnStruct.sAttributeProperties[0] = 0;
    m_cConnStruct.sGeometryProperties[0] = 0;
    m_cConnStruct.sCoordSystems[0] = 0;
    m_cConnStruct.sModTables[0] = 0;
    m_cConnStruct.sModLog[0] = 0;
    m_cConnStruct.sPickLists[0] = 0;
    m_cConnStruct.iConnStat = icsClosed;
    m_cConnStruct.bGeomPrepared = false;
    m_cConnStruct.bFtrPrepared = false;

    m_pRecordsets = new GRecordsets(this, ppLibs[0]);
    m_pTableDefs = new GTableDefs(this, ppLibs[0]);
    m_sConnect = NULL;
    m_pExtPS = new DBExtendedPropertySet(this, ppLibs[1]);

    m_bModLog = VARIANT_TRUE;
    VariantInit(&m_vDefCS);
}

GDatabase::GDatabase(HINSTANCE hInstance, IUnknown *pUnkOuter, ITypeLib **ppLibs,
    int iIndex) : CCOMDispatch(pUnkOuter, ppLibs[0], iIndex)
{
#if DBGLEVEL > 1
    WriteLogFile("GDatabase::GDatabase-2\r\n", true);
#endif // DBGLEVEL
    m_pErrors = new GErrors(this, ppLibs[0]); //, 31);

    m_cConnStruct.hInstance = hInstance;
    m_cConnStruct.pConn = NULL;
    m_cConnStruct.pErrors = (LONG_PTR)m_pErrors;
    LoadCSTable(hInstance, &m_cConnStruct.cCSTable);
    m_cConnStruct.wsServerName = NULL;
    m_cConnStruct.lSessionId = (long)this;
    m_cConnStruct.ppTypeLibs = (LONG_PTR)ppLibs;
    m_cConnStruct.sGFeatures[0] = 0;
    m_cConnStruct.sFieldLookup[0] = 0;
    m_cConnStruct.sAttributeProperties[0] = 0;
    m_cConnStruct.sGeometryProperties[0] = 0;
    m_cConnStruct.sCoordSystems[0] = 0;
    m_cConnStruct.sModTables[0] = 0;
    m_cConnStruct.sModLog[0] = 0;
    m_cConnStruct.sPickLists[0] = 0;
    m_cConnStruct.iConnStat = icsClosed;
    m_cConnStruct.bGeomPrepared = false;
    m_cConnStruct.bFtrPrepared = false;

    m_pRecordsets = new GRecordsets(this, ppLibs[0]);
    m_pTableDefs = new GTableDefs(this, ppLibs[0]);
    m_sConnect = NULL;
    m_pExtPS = new DBExtendedPropertySet(this, ppLibs[1]);

    m_bModLog = VARIANT_TRUE;
    VariantInit(&m_vDefCS);
}

GDatabase::~GDatabase()
{
#if DBGLEVEL > 1
    WriteLogFile("GDatabase::~GDatabase\r\n", true);
#endif // DBGLEVEL
    //if(m_cConnStruct.pGFtrsCmd) m_cConnStruct.pGFtrsCmd->Release();
    //m_cConnStruct.pGeomCmd->Release();

    m_pExtPS->SetOuter(NULL);

    ClearCSTable(&m_cConnStruct.cCSTable);
    if(m_cConnStruct.wsServerName) free(m_cConnStruct.wsServerName);
    if(m_sConnect) free(m_sConnect);
    m_pTableDefs->SetOuter(NULL);
    m_pRecordsets->SetOuter(NULL);
    m_pErrors->SetOuter(NULL);
    for(int i = 0; i < ciTypeLibs; i++)
        ((ITypeLib**)m_cConnStruct.ppTypeLibs)[i]->Release();
    free((void*)m_cConnStruct.ppTypeLibs);
}

HRESULT GDatabase::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("GDatabase::QueryInterface\r\n", true);
#endif // DBGLEVEL
    HRESULT hres = CCOMDispatch::QueryInterface(iid, ppvObject);
    if(hres != S_OK)
    {
        if(IsEqualIID(iid, DIID_GDatabase))
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

ULONG GDatabase::Release()
{
    ULONG lRest;
    IUnknown *pUnkOuter = m_pUnkOuter;
    if(pUnkOuter) lRest = pUnkOuter->Release();
    else lRest = --m_lRefCount;
#if DBGLEVEL > 2
    char buf[64];
    if(pUnkOuter) sprintf(buf, "GDatabase::Release (aggregated) - %d\r\n", lRest);
    else sprintf(buf, "GDatabase::Release - %d\r\n", lRest);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(lRest > 0) return(lRest);
    delete(this);
    return(0);
}

HRESULT GDatabase::get_CollatingOrder(long *pCollatingOrder)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_CollatingOrder\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GDatabase::get_Connect(BSTR *Connect)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_Connect\r\n", true);
#endif // DBGLEVEL
    if(!Connect) return(E_POINTER);
    if(*Connect) SysFreeString(*Connect);
    *Connect = NULL;

    if(!m_sConnect) return(E_FAIL);
    *Connect = SysAllocString(m_sConnect);
    return(S_OK);
}

HRESULT GDatabase::get_GAliasTable(BSTR *AliasTableName)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_GAliasTable\r\n", true);
#endif // DBGLEVEL
    if(!AliasTableName) return(E_POINTER);
    if(*AliasTableName) SysFreeString(*AliasTableName);
    //if(m_cConnStruct.iConnStat > icsSimpleMetaIncomplete)
    //    *AliasTableName = SysAllocString(L"GAliasTable");
    //else *AliasTableName = SysAllocString(L"");

    // We will pretend that the table exists, even if it does not
    // If it does not exist, we will generate a temporary one, with
    // single record - GCoordSystem table

    *AliasTableName = DBStrToBSTR(m_cConnStruct.sGAlias, m_cConnStruct.lDBEnc);
    return(S_OK);
}

HRESULT GDatabase::get_GErrors(GErrors* *ppGErrors)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_GErrors\r\n", true);
#endif // DBGLEVEL
    if(!ppGErrors) return(E_POINTER);
    if(*ppGErrors) (*ppGErrors)->Release();
    m_pErrors->AddRef();
    *ppGErrors = m_pErrors;
    return(S_OK);
}

HRESULT GDatabase::get_GRecordsets(GRecordsets* *ppGRecordsets)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_GRecordsets\r\n", true);
#endif // DBGLEVEL
    if(!ppGRecordsets) return(E_POINTER);
    if(*ppGRecordsets) (*ppGRecordsets)->Release();
    m_pRecordsets->AddRef();
    *ppGRecordsets = m_pRecordsets;
    return(S_OK);
}

HRESULT GDatabase::get_GTableDefs(GTableDefs* *ppGTabledefs)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_GTableDefs\r\n", true);
#endif // DBGLEVEL
    if(!ppGTabledefs) return(E_POINTER);
    if(*ppGTabledefs) (*ppGTabledefs)->Release();
    m_pTableDefs->AddRef();
    *ppGTabledefs = m_pTableDefs;
    return(S_OK);
}

HRESULT GDatabase::get_Name(BSTR *Name)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_Name\r\n", true);
#endif // DBGLEVEL
    if(!Name) return(E_POINTER);
    if(!m_cConnStruct.wsServerName) return(S_FALSE);
    if(*Name) SysFreeString(*Name);
    *Name = SysAllocString(m_cConnStruct.wsServerName);
    return(S_OK);
}

HRESULT GDatabase::get_SchemaUpdatable(VARIANT_BOOL *Updatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_SchemaUpdatable\r\n", true);
#endif // DBGLEVEL
    if(!Updatable) return(E_POINTER);
    if(m_cConnStruct.bSchemaUpdatable) *Updatable = VARIANT_TRUE;
    else *Updatable = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GDatabase::get_SQLConformance(long *pSQLConformance)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_SQLConformance\r\n", true);
#endif // DBGLEVEL
    if(!pSQLConformance) return(E_POINTER);
    *pSQLConformance = 3;
    return(S_OK);
}

HRESULT GDatabase::get_Transactions(VARIANT_BOOL *Transactions)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_Transactions\r\n", true);
#endif // DBGLEVEL
    if(!Transactions) return(E_POINTER);
    *Transactions = VARIANT_TRUE;
    return(S_OK);
}

HRESULT GDatabase::get_Updatable(VARIANT_BOOL *Updatable)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_Updatable\r\n", true);
#endif // DBGLEVEL
    if(!Updatable) return(E_POINTER);
    if(m_cConnStruct.bUpdatable) *Updatable = VARIANT_TRUE;
    else *Updatable = VARIANT_FALSE;
    return(S_OK);
}

HRESULT GDatabase::get_Version(BSTR *Version)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::get_Version\r\n", true);
#endif // DBGLEVEL
    if(!Version) return(E_POINTER);
    if(*Version) SysFreeString(*Version);
    *Version = SysAllocString(L"1.0");
    return(S_OK);
}

HRESULT GDatabase::BeginTrans()
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::BeginTrans\r\n", true);
#endif // DBGLEVEL
    if(m_cConnStruct.iGdoTrans < 1)
    {
        PGresult *res = PQexec(m_cConnStruct.pConn, "BEGIN");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            m_pErrors->HandlePQError(m_cConnStruct.pConn,
                m_cConnStruct.hInstance);
            PQclear(res);
            return(S_FALSE);
        }
        PQclear(res);
    }
    m_cConnStruct.iGdoTrans++;
    return(S_OK);
}

HRESULT GDatabase::ClearModificationLog(VARIANT Entry)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::ClearModificationLog\r\n", true);
#endif // DBGLEVEL
    return(S_OK);
}

HRESULT GDatabase::Close()
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::Close\r\n", true);
#endif // DBGLEVEL
    if(!CloseDatabase()) return(S_FALSE);
    return(S_OK);
}

HRESULT GDatabase::CommitTrans()
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::CommitTrans\r\n", true);
#endif // DBGLEVEL
    if(m_cConnStruct.iGdoTrans > 0) m_cConnStruct.iGdoTrans--;
    if(m_cConnStruct.iGdoTrans < 1)
    {
        PGresult *res = PQexec(m_cConnStruct.pConn, "COMMIT");
        PQclear(res);
    }
    return(S_OK);
}

HRESULT GDatabase::CreateDatabase(BSTR Name, BSTR locale)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::CreateDatabase\r\n", true);
#endif // DBGLEVEL
    return(E_NOTIMPL);
}

HRESULT GDatabase::CreateTableDef(VARIANT Name, GTableDef* *ppGTableDef)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::CreateTableDef\r\n", true);
#endif // DBGLEVEL
    if(!ppGTableDef) return(E_POINTER);
    ValidateVariant(&Name);
    if(*ppGTableDef) (*ppGTableDef)->Release();
    *ppGTableDef = NULL;
    //if(m_pTableDefs->FindByWName(Name.bstrVal) > -1) return(S_FALSE);

    *ppGTableDef = new GTableDef(NULL, m_pTypeLib);
    (*ppGTableDef)->SetNewAttrs(&m_cConnStruct, Name);
    (*ppGTableDef)->AddRef();
    return(S_OK);
}

HRESULT GDatabase::GetExtension(BSTR Name, IDispatch * *ppGExtension)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::GetExtension\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(wcsicmp(Name, L"ExtendedPropertySet") == 0)
    {
        if(!ppGExtension) return(E_POINTER);
        if(*ppGExtension) (*ppGExtension)->Release();
        m_pExtPS->AddRef();
        *ppGExtension = m_pExtPS;
        return(S_OK);
    }
    return(E_NOTIMPL);
}

HRESULT GDatabase::OpenDatabase(BSTR Name, VARIANT exclusive, VARIANT readonly,
  VARIANT ModTrack, VARIANT Source)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::OpenDatabase\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
    WriteLogFile("  Source: ", false);
    WriteLogFile(Source);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    if(!CloseDatabase()) return(S_FALSE);

    if(Source.vt != VT_BSTR)
    {
        return(E_NOTIMPL);
    }

    int ilen = wcslen(Source.bstrVal);
    m_cConnStruct.lCursors = 0;

    wchar_t sBuf[200];

    if(ilen > 0)
    {
        m_sConnect = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
        wcscpy(m_sConnect, Source.bstrVal);

        wchar_t wsDBName[64];
        wchar_t wsUserName[64];
        wchar_t wsPasswd[64];
        wsDBName[0] = 0;
        wsUserName[0] = 0;
        wsPasswd[0] = 0;

        LPWSTR pstr = wcschr(Source.bstrVal, '/'), pstr2;
        ilen = 64;
        if(pstr) ilen = pstr - Source.bstrVal;
        if(ilen < 64)
        {
            wcsncpy(wsUserName, Source.bstrVal, ilen);
            wsUserName[ilen] = 0;
            pstr++;
            pstr2 = wcschr(pstr, '@');
            ilen = 64;
            if(pstr2) ilen = pstr2 - pstr;
        }
        if(ilen < 64)
        {
            wcsncpy(wsPasswd, pstr, ilen);
            wsPasswd[ilen] = 0;
            pstr2++;
            ilen = wcslen(pstr2);
        }
        if(ilen < 64)
        {
            wcsncpy(wsDBName, pstr2, ilen);
            wsDBName[ilen] = 0;
        }

        if(Name[0] && wsDBName[0] && wsUserName[0] && wsPasswd[0])
        {
            char sHost[64];
            char sPort[16];
            char sDBName[64];
            char sUserName[64];
            char sPasswd[64];

            BStrToCharBuf(Name, sHost, 64);
            sPort[0] = 0;
            LPWSTR wsDblDot = wcschr(Name, ':');
            if(wsDblDot)
            {
                ilen = wsDblDot - Name;
                sHost[ilen] = 0;
                wsDblDot++;
                BStrToCharBuf(wsDblDot, sPort, 16);
            }

            BStrToCharBuf(wsDBName, sDBName, 64);
            BStrToCharBuf(wsUserName, sUserName, 64);
            BStrToCharBuf(wsPasswd, sPasswd, 64);

            LPCSTR sConnMask = "host=%s dbname=%s user=%s password=%s sslmode=allow";
            int slen = strlen(sHost) + strlen(sDBName) + strlen(sUserName) +
                strlen(sPasswd) + strlen(sConnMask);
            if(sPort[0]) slen += (8 + strlen(sPort));
            LPSTR sConn = (LPSTR)malloc((slen + 1)*sizeof(char));
            sprintf(sConn, sConnMask, sHost, sDBName, sUserName, sPasswd);
            if(sPort[0])
            {
                strcat(sConn, " port=");
                strcat(sConn, sPort);
            }

#if DBGLEVEL > 0
            WriteLogFile("  trying to connect with: ", true);
            WriteLogFile(sConn, false);
            WriteLogFile("\r\n", false);
#endif // DBGLEVEL

            m_cConnStruct.pConn = PQconnectdb(sConn);
            free(sConn);
            ConnStatusType ist = PQstatus(m_cConnStruct.pConn);

            if(ist == CONNECTION_OK)
            {
                PQsetClientEncoding(m_cConnStruct.pConn, "UTF8");
                InitConnStruct(&m_cConnStruct, sDBName, Name, true);

                if(m_cConnStruct.iConnStat == icsNotEnoughPrivileges)
                {
                    LoadString(m_cConnStruct.hInstance, IDE_INSUFFPRIVILEGES,
                        sBuf, 200);
                    m_pErrors->HandleGDOError(sBuf, m_cConnStruct.hInstance);
                    return(E_INVALIDARG);
                }

                if(m_cConnStruct.iConnStat == icsNoPostGis)
                {
                    LoadString(m_cConnStruct.hInstance, IDE_NOTPOSTGIS, sBuf,
                        200);
                    m_pErrors->HandleGDOError(sBuf, m_cConnStruct.hInstance);
                    return(E_INVALIDARG);
                }

                GetDefaultCoordinateSystem(&m_cConnStruct, &m_vDefCS);
                MarkUsedCSs(m_cConnStruct.pConn, &m_cConnStruct.cCSTable);

                m_pTableDefs->BuildFromConnection(&m_cConnStruct);

                return(S_OK);
            }
            else
            {
                m_pErrors->HandlePQError(m_cConnStruct.pConn, m_cConnStruct.hInstance);
                return(E_FAIL);
            }
        }
        else
        {
            LoadString(m_cConnStruct.hInstance, IDE_INVALIDCONNPARAMS, sBuf, 200);
            m_pErrors->HandleGDOError(sBuf, m_cConnStruct.hInstance);
            return(E_INVALIDARG);
        }
    }
    else
    {
        LoadString(m_cConnStruct.hInstance, IDE_INVALIDCONNPARAMS, sBuf, 200);
        m_pErrors->HandleGDOError(sBuf, m_cConnStruct.hInstance);
        return(E_INVALIDARG);
    }

    return(E_INVALIDARG);
}

HRESULT GDatabase::OpenRecordset(BSTR Name, VARIANT Type, VARIANT options, VARIANT SpatialOp,
  VARIANT SpatialGeometryFilter, VARIANT GeometryFieldName, GRecordset* *ppGRecordset)
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::OpenRecordset\r\n", true);
    WriteLogFile("  Name: ", false);
    WriteLogFile(Name, false);
    WriteLogFile("\r\n", false);
    WriteVariantToLogFile(L"  Type: ", Type);
    WriteVariantToLogFile(L"  options: ", options);
    WriteVariantToLogFile(L"  SpatialOp: ", SpatialOp);
    WriteVariantToLogFile(L"  SpatialGeometryFilter: ", SpatialGeometryFilter);
    WriteVariantToLogFile(L"  GeometryFieldName: ", GeometryFieldName);
#endif // DBGLEVEL
    if(!ppGRecordset) return(E_POINTER);
    if(*ppGRecordset) (*ppGRecordset)->Release();

    ValidateVariant(&SpatialGeometryFilter);
    ValidateVariant(&GeometryFieldName);

    if(m_cConnStruct.iConnStat < icsSimpleMetadata)
    {
        switch(options.vt)
        {
        case VT_I2:
            options.iVal |= 4;
            break;
        case VT_I4:
            options.lVal |= 4;
            break;
        default:
            options.vt = VT_I4;
            options.lVal = 4;
        }
    }

    *ppGRecordset = m_pRecordsets->FindRecordset(Name, Type, options,
        SpatialOp, SpatialGeometryFilter, GeometryFieldName, m_pTableDefs,
        &m_cConnStruct);
    if(*ppGRecordset) return(S_OK);
    return(E_FAIL);
}

HRESULT GDatabase::Rollback()
{
#if DBGLEVEL > 0
    WriteLogFile("GDatabase::Rollback\r\n", true);
#endif // DBGLEVEL
    if(m_cConnStruct.iGdoTrans > 0) m_cConnStruct.iGdoTrans--;
    if(m_cConnStruct.iGdoTrans < 1)
    {
        PGresult *res = PQexec(m_cConnStruct.pConn, "ROLLBACK");
        PQclear(res);
    }
    return(S_OK);
}

bool GDatabase::CloseDatabase()
{
    if(m_pErrors) m_pErrors->ClearAll();
    if(m_pRecordsets) m_pRecordsets->ClearAll();
    if(m_pTableDefs) m_pTableDefs->ClearAll();
    if(m_cConnStruct.pConn)
    {
        if(m_cConnStruct.bGeomPrepared)
        {
            DbuExecuteCommand(&m_cConnStruct, "DEALLOCATE stmtgeom",
                "Deallocate Geometry Statement failed\r\n");
            m_cConnStruct.bGeomPrepared = false;
        }
        if(m_cConnStruct.bFtrPrepared)
        {
            DbuExecuteCommand(&m_cConnStruct, "DEALLOCATE stmtftr",
                "Deallocate GFeatures Statement failed\r\n");
            m_cConnStruct.bFtrPrepared = false;
        }
        DbuExecuteCommand(&m_cConnStruct, "DEALLOCATE stmttbls",
            "Deallocate pg_tables Statement failed\r\n");
        if((m_cConnStruct.iSysTrans | m_cConnStruct.iGdoTrans) > 0)
        {
            PGresult *res = PQexec(m_cConnStruct.pConn, "commit");
            PQclear(res);
        }
        PQfinish(m_cConnStruct.pConn);
        m_cConnStruct.pConn = NULL;
    }
    UnmarkUsedCSs(&m_cConnStruct.cCSTable);
    VariantClear(&m_vDefCS);
    return(true);
}

VARIANT_BOOL GDatabase::GetModificationLogging()
{
    return(m_bModLog);
}

void GDatabase::SetModificationLogging(VARIANT_BOOL bAllow)
{
    m_bModLog = bAllow;
    return;
}

void GDatabase::BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid)
{
    m_pRecordsets->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    m_pTableDefs->BroadcastCSChange(wsOldCSGuid, wsNewCSGuid);
    return;
}

/*void GDatabase::BroadcastDelRecord(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid)
{
    m_pRecordsets->BroadcastDelRecord(wsTblName, iCtidLen, sCtid);
    return;
}*/

bool GDatabase::SetDefCS(VARIANT vDefCS)
{
    return(SetDefaultCoordinateSystem(&m_cConnStruct, vDefCS, &m_vDefCS));
}

void GDatabase::GetDefCS(VARIANT *pvDefCS)
{
    VariantCopy(pvDefCS, &m_vDefCS);
    return;
}
