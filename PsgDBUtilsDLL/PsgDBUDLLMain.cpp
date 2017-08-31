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

#include "PsgDBUDLLMain.hpp"
#include "../Common/GDOUtils.hpp"
#include "../Common/VarUtils.hpp"
#include "../Common/pg_const.h"
#include "../Common/LogFiles.hpp"

// CConnList

CConnList::CConnList(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_iListSize = 16;
    m_iListLen = 0;
    m_ppData = (PConnStruct*)malloc(m_iListSize*sizeof(PConnStruct));
    m_ppTables = (PDbuTables*)malloc(m_iListSize*sizeof(PDbuTables));
}

CConnList::~CConnList()
{
    while(m_iListLen > 0) CloseConnection(m_iListLen);
    free(m_ppData);
}

long CConnList::AddConnection(HINSTANCE hInstance, LPWSTR sServer,
    LPWSTR sDatabase, LPWSTR sUser, LPWSTR sPassword, int iPort)
{
    char sHost[64];
    char sPort[16];
    char sDBName[64];
    char sUserName[64];
    char sPasswd[64];
    WCharToCharBuf(sServer, sHost, 64);
    WCharToCharBuf(sDatabase, sDBName, 64);
    WCharToCharBuf(sUser, sUserName, 64);
    WCharToCharBuf(sPassword, sPasswd, 64);

    LPCSTR sConnMask = "host=%s dbname=%s user=%s password=%s  sslmode=allow";
    int slen = strlen(sHost) + strlen(sDBName) + strlen(sUserName) +
        strlen(sPasswd) + strlen(sConnMask);
    if(iPort > 0)
    {
        sprintf(sPort, "%d", iPort);
        slen += (8 + strlen(sPort)); // "port=%s"
    }
    LPSTR sConn = (LPSTR)malloc((slen + 1)*sizeof(char));
    sprintf(sConn, sConnMask, sHost, sDBName, sUserName, sPasswd);

    if(iPort > 0)
    {
        strcat(sConn, " port=");
        strcat(sConn, sPort);
    }

    PConnStruct pCStr = (PConnStruct)malloc(sizeof(CConnStruct));
    pCStr->hInstance = hInstance;
    pCStr->pConn = NULL;
    pCStr->wsServerName = NULL;
    pCStr->iSysTrans = 0;
    pCStr->iGdoTrans = 0;
    pCStr->lSessionId = 0;
    pCStr->ppTypeLibs = (LONG_PTR)NULL;
    pCStr->pErrors = (LONG_PTR)NULL;
    pCStr->iConnStat = icsClosed;
    pCStr->bGeomPrepared = false;
    pCStr->bFtrPrepared = false;
    LoadCSTable(hInstance, &pCStr->cCSTable);

    pCStr->pConn = PQconnectdb(sConn);
    free(sConn);
    ConnStatusType ist = PQstatus(pCStr->pConn);

    if(ist == CONNECTION_OK)
    {
        PQsetClientEncoding(pCStr->pConn, "UTF8");
        InitConnStruct(pCStr, sDBName, NULL, false);

        if(m_iListLen >= m_iListSize)
        {
            m_iListSize += 16;
            m_ppData = (PConnStruct*)realloc(m_ppData,
                m_iListSize*sizeof(PConnStruct));
            m_ppTables = (PDbuTables*)realloc(m_ppTables,
                m_iListSize*sizeof(PDbuTables));
        }

        m_ppTables[m_iListLen] = NULL;
        m_ppData[m_iListLen++] = pCStr;
        return(m_iListLen);
    }

    free(pCStr);
    return(0);
}

bool CConnList::CloseConnection(long lHandle)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];
    if(pCStr->iConnStat > icsClosed)
    {
        if(m_ppTables[lHandle - 1])
        {
            delete m_ppTables[lHandle - 1];
            m_ppTables[lHandle - 1] = NULL;
        }

        if(pCStr->bGeomPrepared)
        {
            DbuExecuteCommand(pCStr, "DEALLOCATE stmtgeom",
                "Deallocate Geometry Statement failed\r\n");
            pCStr->bGeomPrepared = false;
        }
        if(pCStr->bFtrPrepared)
        {
            DbuExecuteCommand(pCStr, "DEALLOCATE stmtftr",
                "Deallocate GFeatures Statement failed\r\n");
            pCStr->bFtrPrepared = false;
        }
        DbuExecuteCommand(pCStr, "DEALLOCATE stmttbls",
            "Deallocate pg_tables Statement failed\r\n");
        PQfinish(pCStr->pConn);
        pCStr->iSysTrans = 0;
        pCStr->iGdoTrans = 0;
        pCStr->pConn = NULL;
    }

    if(lHandle == m_iListLen)
    {
        free(pCStr);
        m_iListLen--;
    }
    return(true);
}

CConnStatus CConnList::GetConnStatus(long lHandle)
{
    if(lHandle > m_iListLen) return(icsClosed);
    if(lHandle < 1) return(icsClosed);

    PConnStruct pCStr = m_ppData[lHandle - 1];
    return(pCStr->iConnStat);
}

bool CConnList::CheckGAliasTable(PConnStruct pCStr)
{
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    PGresult *res;
    bool bGoOn = true;
    char sSql[128];
    //LPSTR lsCmd;

    if(pCStr->iConnStat < icsSimpleMetaIncomplete)
    {
        if(pCStr->iConnStat > icsNoMetadata)
        {
            //SetSP(pCStr->pConn, bTrans);
            sprintf(sSql, "drop table %s", pCStr->sGAlias);
            //lsCmd = GetSaveSQL(sSql, bTrans);
            res = PQexec(pCStr->pConn, sSql);
            //free(lsCmd);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                bGoOn = false;
                WritePQErrorToLog("Drop galiastable in 'CheckGAliasTable' failed\r\n",
                    pCStr->pConn);
                //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
                RollbackSP(pCStr->pConn, bTrans);
            }
            else
            {
                PQclear(res);
                ReleaseSP(pCStr->pConn, bTrans);
            }
        }

        if(bGoOn)
        {
            SetSP(pCStr->pConn, bTrans);
            //lsCmd = GetSaveSQL("create schema gdo", bTrans);
            res = PQexec(pCStr->pConn, "create schema gdo");
            //free(lsCmd);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                WritePQErrorToLog("Create schema in 'CheckGAliasTable' failed\r\n",
                    pCStr->pConn);
                //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
                RollbackSP(pCStr->pConn, bTrans);
            }
            else
            {
                PQclear(res);
                ReleaseSP(pCStr->pConn, bTrans);
            }
        }

        if(bGoOn)
        {
            pCStr->iConnStat = icsNoMetadata;
            SetSP(pCStr->pConn, bTrans);
            res = PQexec(pCStr->pConn, "create table gdo.galiastable(\
tabletype varchar(128) not null primary key, \
tablename varchar(128) not null)");
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                bGoOn = false;
                WritePQErrorToLog("Create galiastable in 'CheckGAliasTable' failed\r\n",
                    pCStr->pConn);
                //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
                PQclear(res);
                RollbackSP(pCStr->pConn, bTrans);
            }
            else
            {
                strcpy(pCStr->sGAlias, "gdo.galiastable");
                pCStr->iConnStat = icsSimpleMetaIncomplete;
                PQclear(res);
                ReleaseSP(pCStr->pConn, bTrans);
            }
        }
    }
    return(bGoOn);
}

bool CConnList::InsertSimpleMetadata(PConnStruct pCStr)
{
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    SetSP(pCStr->pConn, bTrans);
    LPCSTR sStmt = "stmtgalias";
    char sSql[128];
    sprintf(sSql, "select tablename from %s where tabletype = $1",
        pCStr->sGAlias);
    PGresult *res = PQprepare(pCStr->pConn, sStmt, sSql, 1, NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Prepare statement in 'InsertSimpleMetadata' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    CheckGraphicColumnsTable(pCStr);

    bool bRes = CheckRefTable(pCStr, sStmt, "GCoordSystemTable",
        "gcoordsystem", NULL, NULL);

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "GModifications",
            "gdo.modificationlog", "create table \
%s(ModificationNumber serial not null primary key, \
Type smallint, ModifiedTableID integer, KeyValue1 varchar(255), \
KeyValue2 varchar(255), KeyValue3 varchar(255), KeyValue4 varchar(255), \
KeyValue5 varchar(255), KeyValue6 varchar(255), KeyValue7 varchar(255), \
KeyValue8 varchar(255), KeyValue9 varchar(255), KeyValue10 varchar(255), \
SESSIONID integer not null, ModifiedDate timestamp not null default now())",
            pCStr->sModLog);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "GModifiedTables",
            "gdo.modifiedtables", "create table \
%s(ModifiedTableID serial not null primary key, \
TableName varchar(127) not null unique, KeyValue1 varchar(127), \
KeyValue2 varchar(127), KeyValue3 varchar(127), KeyValue4 varchar(127), \
KeyValue5 varchar(127), KeyValue6 varchar(127), KeyValue7 varchar(127), \
KeyValue8 varchar(127), KeyValue9 varchar(127), KeyValue10 varchar(127))",
            pCStr->sModTables);
    }

    SetSP(pCStr->pConn, bTrans);
    char sCmd[64];
    strcpy(sCmd, "DEALLOCATE ");
    strcat(sCmd, sStmt);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    res = PQexec(pCStr->pConn, sCmd);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Deallocate statement in 'InsertSimpleMetadata' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
    }

    return(bRes);
}

bool CConnList::CreateSimpleMetadata(long lHandle)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    if(!CheckGAliasTable(pCStr)) return(false);

    if(InsertSimpleMetadata(pCStr))
    {
        pCStr->iConnStat = icsSimpleMetadata;
        return(true);
    }

    return(false);
}

void CConnList::PopulateCoordSystemTable(PConnStruct pCStr, LPSTR sTblName)
{
    PGresult *res;
    if((pCStr->iSysTrans | pCStr->iGdoTrans) < 1)
    {
        res = PQexec(pCStr->pConn, "begin");
        PQclear(res);
    }
    pCStr->iSysTrans++;

    SetSP(pCStr->pConn, true);
    UUID uid;
    res = PQexec(pCStr->pConn, "declare sridcur no scroll cursor for select distinct(srid) from geometry_columns");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
        WritePQErrorToLog("Declare cursor in 'PopulateCoordSystemTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, true);

        pCStr->iSysTrans--;
        if((pCStr->iSysTrans | pCStr->iGdoTrans) < 1)
        {
            res = PQexec(pCStr->pConn, "end");
            PQclear(res);
        }
        return;
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, true);

    VARIANT V, *pRow;
    VariantInit(&V);
    Oid iType = INT4OID;

    res = PQexec(pCStr->pConn, "fetch in sridcur");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Fecth sridcur in 'PopulateCoordSystemTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);

        pCStr->iSysTrans--;
        if((pCStr->iSysTrans | pCStr->iGdoTrans) < 1)
        {
            res = PQexec(pCStr->pConn, "end");
            PQclear(res);
        }
        return;
    }

    // this gives some strange number. We are happy with LONG
    //DescribeFieldTypes(res, 0, &iType);

    long nrows = PQntuples(res);
    while(nrows > 0)
    {
        GetRecord(res, 0, 1, &iType, &V, pCStr->sDecSep);
        PQclear(res);
        pRow = FindCSTableRow(&pCStr->cCSTable, VarToLong(V));
        if(pRow)
        {
            if(pRow[1].vt < VT_I2)
            {
                UuidCreate(&uid);
                pRow[1].vt = VT_BSTR;
                pRow[1].bstrVal = SysAllocStringLen(NULL, 40);
                swprintf(pRow[1].bstrVal,
                    L"{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}",
                    uid.Data1, uid.Data2, uid.Data3, uid.Data4[0],
                    uid.Data4[1], uid.Data4[2], uid.Data4[3], uid.Data4[4],
                    uid.Data4[5], uid.Data4[6], uid.Data4[7]);
            }
            pRow[2].vt = VT_I4;
            pRow[2].iVal = 1;

            InsertTable(pCStr->pConn, pCStr->sCoordSystems,
                119, &pRow[1], pCStr->lDBEnc, pCStr->iCurrDigits);
        }
        VariantClear(&V);
        res = PQexec(pCStr->pConn, "fetch in sridcur");
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Fecth loop sridcur in 'PopulateCoordSystemTable' failed\r\n",
                pCStr->pConn);
            nrows = 0;
        }
        else nrows = PQntuples(res);
    }
    PQclear(res);

    SetSP(pCStr->pConn, true);
    //LPSTR lsCmd = GetSaveSQL("close sridcur", true);
    res = PQexec(pCStr->pConn, "close sridcur");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Close sridcur in 'PopulateCoordSystemTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, true);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, true);
    }

    pCStr->iSysTrans--;
    if((pCStr->iSysTrans | pCStr->iGdoTrans) < 1)
    {
        res = PQexec(pCStr->pConn, "end");
        PQclear(res);
    }
    return;
}

void CConnList::PopulateSQLOperatorTable(PConnStruct pCStr, LPSTR sTblName)
{
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    LPCSTR sMask = "insert into %s(operator, operatorclass, description) values($1, $2, $3)";
    int slen = strlen(sTblName) + strlen(sMask) + 1;
    LPSTR sCmd = (LPSTR)malloc(slen*sizeof(char));
    sprintf(sCmd, sMask, sTblName);
    LPCSTR sStmt = "stmtsqloper";

    SetSP(pCStr->pConn, bTrans);

    PGresult *res = PQprepare(pCStr->pConn, sStmt, sCmd, 3, NULL);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Prepare statement in 'PopulateSQLOperatorTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return;
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    LPCSTR vData[20][3] = {
        {"%",       "gdbPattern",    "Zero or more characters may fill this position."},
        {"*",       "gdbArithmetic", "This is the multiplication sign."},
        {"+",       "gdbArithmetic", "This is the plus sign."},
        {"-",       "gdbArithmetic", "This is the minus sign."},
        {"/",       "gdbArithmetic", "This is the division sign."},
        {"_",       "gdbPattern",    "Single character may fill this position."},
        {"||",      "gdbArithmetic", "This is the concatenation operator."},
        {"AVG",     "gdbStatistics", "Finds the average of a column's values."},
        {"BETWEEN", "gdbSelection",  "Allows specification of a range of values associated with a column's value."},
        {"COUNT",   "gdbStatistics", "Finds the number of rows in the query."},
        {"FALSE",   "gdbConstants",  "Indicator for negative boolean values."},
        {"IN",      "gdbSelection",  "Preceeds a list of values from which the column's value shall be included."},
        {"IS",      "gdbSelection",  "Preceeds a constant value for comparison to a column's value."},
        {"LIKE",    "gdbPattern",    "Used in conjunction with wildcard characters to look for patterns."},
        {"MAX",     "gdbStatistics", "Finds the maximum value within a column."},
        {"MIN",     "gdbStatistics", "Finds the minimum value within a column."},
        {"NOT",     "gdbLogical",    "This is the logical negation operator."},
        {"NULL",    "gdbConstants",  "Indicator for NULL (undefined) values."},
        {"SUM",     "gdbStatistics", "Finds the sum of a column's testing common values."},
        {"TRUE",    "gdbConstants",  "Indicator for positive boolean values."}};
    for(int i = 0; i < 20; i++)
    {
        SetSP(pCStr->pConn, bTrans);
        res = PQexecPrepared(pCStr->pConn, sStmt, 3, vData[i], NULL,
            NULL, 0);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Execute statement in 'PopulateSQLOperatorTable' failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    SetSP(pCStr->pConn, bTrans);
    sCmd = (LPSTR)malloc(64*sizeof(char));
    strcpy(sCmd, "DEALLOCATE ");
    strcat(sCmd, sStmt);
    res = PQexec(pCStr->pConn, sCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Deallocate statement in 'PopulateSQLOperatorTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
    }
    free(sCmd);
    return;
}

bool CConnList::CheckRefTable(PConnStruct pCStr, LPCSTR sStmt, LPCSTR sTypeName,
    LPCSTR sTblName, LPCSTR sTblDef, LPSTR sNewNameBuf)
{
    bool bRes = true;
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    SetSP(pCStr->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sStmt, bTrans);
    PGresult *res = PQexecPrepared(pCStr->pConn, sStmt, 1, &sTypeName,
        NULL, NULL, 0);
    //free(lsCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Execute statement in 'CheckRefTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }

    int iUpdate = 0;
    bool bCreate = false;
    char sTblNameBuf[64];
    sTblNameBuf[0] = 0;

    if(PQntuples(res) < 1)
    {
        strcpy(sTblNameBuf, sTblName);
        iUpdate = 1;
    }
    else
    {
        if(!PQgetisnull(res, 0, 0)) strcpy(sTblNameBuf, PQgetvalue(res, 0, 0));
        if((strlen(sTblNameBuf) < 1) || (strcmp(sTblNameBuf, sTblName) != 0))
        {
            strcpy(sTblNameBuf, sTblName);
            iUpdate = 2;
        }
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    if(TableExists(pCStr->pConn, sTblNameBuf, bTrans) < 1) bCreate = true;

    LPSTR sCmd;
    int slen;
    LPCSTR psParam[2];
    psParam[0] = sTypeName;
    psParam[1] = sTblNameBuf;
    if(iUpdate > 0)
    {
        SetSP(pCStr->pConn, bTrans);
        if(iUpdate == 1)
        {
            slen = strlen("insert into (tabletype, tablename) values($1, $2)") +
                strlen(pCStr->sGAlias) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "insert into %s(tabletype, tablename) values($1, $2)",
                pCStr->sGAlias);
            res = PQexecParams(pCStr->pConn, sCmd, 2, NULL, psParam,
                NULL, NULL, 0);
            free(sCmd);
        }
        else if(iUpdate == 2)
        {
            slen = strlen("update  set tablename = $2 where tabletype = $1") +
                strlen(pCStr->sGAlias) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "update %s set tablename = $2 where tabletype = $1",
                pCStr->sGAlias);
            res = PQexecParams(pCStr->pConn, sCmd, 2, NULL, psParam,
                NULL, NULL, 0);
            free(sCmd);
        }
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bCreate = false;
            bRes = false;
            //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
            WritePQErrorToLog("Insert or update galiastable in 'CheckRefTable' failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    if(bCreate && sTblDef)
    {
        int slen = strlen(sTblDef) + strlen(sTblNameBuf) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sTblDef, sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
            WritePQErrorToLog("Create table in 'CheckRefTable' failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    if(bRes)
    {
        if(sNewNameBuf) strcpy(sNewNameBuf, sTblNameBuf);
        if(strcmp(sTypeName, "INGRSQLOperatorsTable") == 0)
        {
            PopulateSQLOperatorTable(pCStr, sTblNameBuf);
        }
        else if(sTblDef && (strcmp(sTypeName, "GCoordSystemTable") == 0))
        {
            PopulateCoordSystemTable(pCStr, sTblNameBuf);
        }
    }

    return(bRes);
}

bool CConnList::CheckGraphicColumnsTable(PConnStruct pCStr)
{

    bool bRes = true;
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    if(TableExists(pCStr->pConn, "gdo_graphic_columns", bTrans) < 1)
    {
        LPCSTR sTblDef = "create table gdo_graphic_columns(\
f_table_schema name not null, \
f_table_name name not null, \
f_graphic_column name not null, \
coord_dimension integer not null, \
srid integer not null, \
type name not null, \
f_native_geom_column name, \
f_plain_label_column name, \
primary key (f_table_schema, f_table_name, f_graphic_column))";

        SetSP(pCStr->pConn, bTrans);
        //LPSTR lsCmd = GetSaveSQL(sTblDef, bTrans);
        PGresult *res = PQexec(pCStr->pConn, sTblDef);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gdo_graphic_columns table failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    return(bRes);
}

bool CConnList::CheckGFeaturesTable(PConnStruct pCStr, LPCSTR sStmt,
    LPCSTR sTypeName, LPCSTR sTblName, LPSTR sNewNameBuf)
{
    bool bRes = true;
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    SetSP(pCStr->pConn, bTrans);
    PGresult *res = PQexecPrepared(pCStr->pConn, sStmt, 1, &sTypeName,
        NULL, NULL, 0);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Execute statement in 'CheckGFeaturesTable' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }

    int iUpdate = 0;
    bool bCreate = false;
    char sTblNameBuf[64];
    sTblNameBuf[0] = 0;

    if(PQntuples(res) < 1)
    {
        strcpy(sTblNameBuf, sTblName);
        iUpdate = 1;
    }
    else
    {
        if(!PQgetisnull(res, 0, 0)) strcpy(sTblNameBuf, PQgetvalue(res, 0, 0));
        if((strlen(sTblNameBuf) < 1) || (strcmp(sTblNameBuf, sTblName) != 0))
        {
            strcpy(sTblNameBuf, sTblName);
            iUpdate = 2;
        }
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    if(TableExists(pCStr->pConn, sTblNameBuf, bTrans) < 1) bCreate = true;

    LPSTR sCmd;
    int slen;
    LPCSTR psParam[2];
    psParam[0] = sTypeName;
    psParam[1] = sTblNameBuf;
    if(iUpdate > 0)
    {
        SetSP(pCStr->pConn, bTrans);
        if(iUpdate == 1)
        {
            slen = strlen("insert into (tabletype, tablename) values($1, $2)") +
                strlen(pCStr->sGAlias) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "insert into %s(tabletype, tablename) values($1, $2)",
                pCStr->sGAlias);
            res = PQexecParams(pCStr->pConn, sCmd, 2, NULL, psParam,
                NULL, NULL, 0);
            free(sCmd);
        }
        else if(iUpdate == 2)
        {
            slen = strlen("update  set tablename = $2 where tabletype = $1") +
                strlen(pCStr->sGAlias) + 1;
            sCmd = (LPSTR)malloc(slen*sizeof(char));
            sprintf(sCmd, "update %s set tablename = $2 where tabletype = $1",
                pCStr->sGAlias);
            res = PQexecParams(pCStr->pConn, sCmd, 2, NULL, psParam,
                NULL, NULL, 0);
            free(sCmd);
        }
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bCreate = false;
            bRes = false;
            //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
            WritePQErrorToLog("Insert or update galiastable in 'CheckRefTable' failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    if(bCreate)
    {
        LPCSTR sTblDef = "create table %sbase(featurename varchar(127) not null primary key, \
geometrytype integer, primarygeometryfieldname varchar(127), \
featuredescription varchar(255))";

        int slen = strlen(sTblDef) + strlen(sTblNameBuf) + 1;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sTblDef, sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gfeaturesbase table failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }

        LPCSTR sViewDef = "create view %s as select ctid, * from %sbase where \
featurename in (select table_schema || '.' || table_name from information_schema.tables) or \
featurename in (select table_name from information_schema.tables where table_schema = 'public') or not \
(featurename in (select schemaname || '.' || tablename from pg_tables) or \
featurename in (select tablename from pg_tables where schemaname = 'public') or \
featurename in (select schemaname || '.' || viewname from pg_views) or \
featurename in (select viewname from pg_views where schemaname = 'public'))";

        slen = strlen(sViewDef) + 2*strlen(sTblNameBuf) + 2;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sViewDef, sTblNameBuf, sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gfeatures view failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }

        LPCSTR sUpdRule = "create rule gfeatures_upd as on update to %s \
do instead update %sbase set featurename = new.featurename, \
geometrytype = new.geometrytype, primarygeometryfieldname = new.primarygeometryfieldname, \
featuredescription = new.featuredescription where %sbase.ctid = old.ctid \
returning %sbase.ctid, %sbase.*";

        slen = strlen(sUpdRule) + 5*strlen(sTblNameBuf) + 2;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sUpdRule, sTblNameBuf, sTblNameBuf, sTblNameBuf,
            sTblNameBuf, sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gfeatures update rule failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }

        LPCSTR sInsRule = "create rule gfeatures_ins as on insert to %s \
do instead insert into %sbase values (new.featurename, \
new.geometrytype, new.primarygeometryfieldname, new.featuredescription) \
returning %sbase.ctid, %sbase.*";

        slen = strlen(sInsRule) + 4*strlen(sTblNameBuf) + 2;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sInsRule, sTblNameBuf, sTblNameBuf, sTblNameBuf,
            sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gfeatures insert rule failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }

        LPCSTR sDelRule = "create rule gfeatures_del as on delete to %s \
do instead delete from %sbase where %sbase.ctid = old.ctid";

        slen = strlen(sDelRule) + 3*strlen(sTblNameBuf) + 2;
        sCmd = (LPSTR)malloc(slen*sizeof(char));
        sprintf(sCmd, sDelRule, sTblNameBuf, sTblNameBuf, sTblNameBuf);

        SetSP(pCStr->pConn, bTrans);
        //lsCmd = GetSaveSQL(sCmd, bTrans);
        res = PQexec(pCStr->pConn, sCmd);
        //free(lsCmd);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            bRes = false;
            WritePQErrorToLog("Create gfeatures delete rule failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
        }
    }

    if(bRes)
    {
        if(sNewNameBuf) strcpy(sNewNameBuf, sTblNameBuf);
    }

    return(bRes);
}

bool CConnList::InsertINGRMetadata(PConnStruct pCStr)
{
    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    SetSP(pCStr->pConn, bTrans);
    LPCSTR sStmt = "stmtgalias";
    char sSql[128];
    sprintf(sSql, "select tablename from %s where tabletype = $1",
        pCStr->sGAlias);
    PGresult *res = PQprepare(pCStr->pConn, sStmt, sSql, 1, NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Prepare statement in 'InsertINGRMetadata' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    bool bRes = CheckRefTable(pCStr, sStmt, "GCoordSystemTable",
        "gdo.gcoordsystem", "create table \
%s(csguid varchar(40) primary key, csguidtype integer, \
name varchar(100), description varchar(256), basestoragetype \"char\", \
stor2compmatrix1 double precision, stor2compmatrix2 double precision, \
stor2compmatrix3 double precision, stor2compmatrix4 double precision, \
stor2compmatrix5 double precision, stor2compmatrix6 double precision, \
stor2compmatrix7 double precision, stor2compmatrix8 double precision, \
stor2compmatrix9 double precision, stor2compmatrix10 double precision, \
stor2compmatrix11 double precision, stor2compmatrix12 double precision, \
stor2compmatrix13 double precision, stor2compmatrix14 double precision, \
stor2compmatrix15 double precision, stor2compmatrix16 double precision, \
heightstoragetype \"char\", lonnormstorageopt \"char\", \
geodeticdatum smallint, ellipsoid smallint, \
equatorialradius double precision, inverseflattening double precision, \
projalgorithm smallint, azimuthangle double precision, \
falsex double precision, falsey double precision, hemisphere \"char\", \
latoforigin double precision, latoftruescale double precision, \
lonoforigin double precision, radofstandcircle double precision, \
scalereductfact double precision, standpar1 double precision, \
standpar2 double precision, zone smallint, pathnumber smallint, \
rownumber smallint, satellite smallint, xazdefopt \"char\", \
geomheightoforig double precision, geomheightofpoint1 double precision, \
geomheightofpoint2 double precision, latofpoint1 double precision, \
latofpoint2 double precision, lonofpoint1 double precision, \
lonofpoint2 double precision, argumentofperigee double precision, \
earthrotperiod double precision, fourierexpansiondegree \"char\", \
nodesinsimpsonintegration \"char\", orbearthrotperiodratio double precision, \
orbecc double precision, orbinclination double precision, \
orboff double precision, orbperiod double precision, \
orbsemimajaxis double precision, oblmercdefmode \"char\", \
latofmapcenter double precision, obllamconfcondefmode \"char\", \
rotnorthpolelat double precision, rotnorthpolelon double precision, \
gaussianlat double precision, sphermodel double precision, \
spherradius double precision, latofbasispointa double precision, \
latofbasispointb double precision, latofbasispointc double precision, \
lonofbasispointa double precision, lonofbasispointb double precision, \
lonofbasispointc double precision, chamtriorigindefmode \"char\", \
angorientationprojplanedefmode \"char\", azofupwardtilt double precision, \
focallength double precision, heightaboveellipatnadir double precision, \
heightorigoflocalhorizsys double precision, \
latorigoflocalhorizsys double precision, locationofprojplanedefmode smallint, \
lonorigoflocalhorizsys double precision, perspcentergeocx double precision, \
perspcentergeocy double precision, perspcentergeocz double precision, \
perspcenterheight double precision, perspcenterlat double precision, \
perspcenterlon double precision, perspcenterxeast double precision, \
perspcenterynorth double precision, perspcenterzup double precision, \
refcoordsysdefmode \"char\", rotaboutxaxis double precision, \
rotaboutyaxis double precision, rotaboutzaxis double precision, \
swingang double precision, tiltang double precision, \
extendprojmatrix1 double precision, extendprojmatrix2 double precision, \
extendprojmatrix3 double precision, extendprojmatrix4 double precision, \
extendprojmatrix5 double precision, extendprojmatrix6 double precision, \
extendprojmatrix7 double precision, extendprojmatrix8 double precision, \
extendprojmatrix9 double precision, extendprojmatrix10 double precision, \
extendprojmatrix11 double precision, extendprojmatrix12 double precision, \
extendprojmatrix13 double precision, extendprojmatrix14 double precision, \
extendprojmatrix15 double precision, extendprojmatrix16 double precision, \
verticaldatum smallint, undulationmodel smallint, \
averageundulation double precision, namedgeodeticdatum varchar(100))",
            pCStr->sCoordSystems);

    if(bRes)
    {
        bRes = CheckGFeaturesTable(pCStr, sStmt, "INGRFeatures",
         "gdo.gfeatures", pCStr->sGFeatures);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRFieldLookup", "gdo.fieldlookup",
            "create table %s(indexid serial not null primary key, \
featurename varchar(127), fieldname varchar(127))", pCStr->sFieldLookup);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRAttributeProperties",
            "gdo.attributeproperties", "create table %s(IsKeyField boolean, \
fielddescription varchar(255), indexid integer not null primary key, \
fieldformat varchar(255), fieldtype integer, isfielddisplayable boolean, \
fieldprecision integer)", pCStr->sAttributeProperties);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRGeometryProperties",
            "gdo.geometryproperties",
            "create table %s(primarygeometryflag boolean, \
geometrytype integer, gcoordsystemguid varchar(255), \
fielddescription varchar(255), indexid integer not null primary key)",
            pCStr->sGeometryProperties);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRPickLists", "gdo.gpicklisttable",
            "create table %s(featurename varchar(127), \
fieldname varchar(127), picklisttablename varchar(127), \
valuefieldname varchar(127), descriptionfieldname varchar(127), \
filterclause varchar(255))", pCStr->sPickLists);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRSQLOperatorsTable",
            "gdo.gsqloperatortable", "create table %s(operator varchar(32), \
operatorclass varchar(100), description text)", NULL);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "GParameters",
            "gdo.gparameters", "create table %s(gparameter varchar(63) not null primary key, \
gvalue varchar(255) not null)", NULL);
    }

    if(bRes)
    {
        bRes = CheckRefTable(pCStr, sStmt, "INGRDictionaryProperties",
            "gdo.gdictionaryproperties", NULL, NULL);
    }

    char sCmd[64];
    strcpy(sCmd, "DEALLOCATE ");
    strcat(sCmd, sStmt);
    res = PQexec(pCStr->pConn, sCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Deallocate statement in 'InsertINGRMetadata' failed\r\n",
            pCStr->pConn);
    }
    PQclear(res);

    if(bRes)
    {
        SetSP(pCStr->pConn, bTrans);
        char buf[128];
        sprintf(buf, "select * from %s where featurename = $1",
            pCStr->sGFeatures);
        res = PQprepare(pCStr->pConn, "stmtftr", buf, 0, NULL);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Prepare select GFeatures in 'CDBMainDlg::InsertINGRMetadata' failed\r\n",
                pCStr->pConn);
            PQclear(res);
            RollbackSP(pCStr->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCStr->pConn, bTrans);
            pCStr->bFtrPrepared = true;
        }
    }

    return(bRes);
}

bool CConnList::CreateINGRMetadata(long lHandle)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    if(!CheckGAliasTable(pCStr)) return(false);

    if(pCStr->iConnStat < icsSimpleMetadata)
    {
        if(InsertSimpleMetadata(pCStr)) pCStr->iConnStat = icsSimpleMetadata;
    }
    if(pCStr->iConnStat >= icsSimpleMetadata)
    {
        if(InsertINGRMetadata(pCStr))
        {
            pCStr->iConnStat = icsINGRMetadata;
            return(true);
        }
    }
    return(false);
}

bool CConnList::DropMetadata(long lHandle)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;
    char sCmd[128];

    SetSP(pCStr->pConn, bTrans);
    sprintf(sCmd, "declare galcur no scroll cursor with hold for select * from %s",
        pCStr->sGAlias);
    PGresult *res = PQexec(pCStr->pConn, sCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Declare galcur in 'DropMetadataTables' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(0);
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    SetSP(pCStr->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL("fetch in galcur", bTrans);
    res = PQexec(pCStr->pConn, "fetch in galcur");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Fetch galcur in 'DropMetadataTables' faled\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(0);
    }

    int iNameIdx = PQfnumber(res, "tablename");
    if(iNameIdx < 0)
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
        return(0);
    }

    long nrows = PQntuples(res);
    char *sTblName;
    while(nrows > 0)
    {
        sTblName = PQgetvalue(res, 0, iNameIdx);
        PQclear(res);

        if(stricmp(sTblName, pCStr->sGFeatures) != 0)
        {
            strcpy(sCmd, "drop table ");
            strcat(sCmd, sTblName);

            res = PQexec(pCStr->pConn, sCmd);
#if DBGLEVEL > 1
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
                WritePQErrorToLog("Drop table in 'DropMetadataTables' failed\r\n",
                    pCStr->pConn);
#endif
            PQclear(res);
        }
        else
        {
            strcpy(sCmd, "drop view ");
            strcat(sCmd, sTblName);

            res = PQexec(pCStr->pConn, sCmd);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
#if DBGLEVEL > 1
                WritePQErrorToLog("Drop gfeatures view in 'DropMetadataTables' failed\r\n",
                    pCStr->pConn);
#endif
                PQclear(res);

                strcpy(sCmd, "drop table ");
                strcat(sCmd, sTblName);

                res = PQexec(pCStr->pConn, sCmd);
#if DBGLEVEL > 1
                if(PQresultStatus(res) != PGRES_COMMAND_OK)
                    WritePQErrorToLog("Drop gfeatures table in 'DropMetadataTables' failed\r\n",
                        pCStr->pConn);
#endif
                PQclear(res);
            }

            PQclear(res);

            strcpy(sCmd, "drop table ");
            strcat(sCmd, sTblName);
            strcat(sCmd, "base");

            res = PQexec(pCStr->pConn, sCmd);
#if DBGLEVEL > 1
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
                WritePQErrorToLog("Drop gfeaturesbase table in 'DropMetadataTables' failed\r\n",
                    pCStr->pConn);
#endif
            PQclear(res);
        }

        res = PQexec(pCStr->pConn, "fetch in galcur");
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            nrows = 0;
            WritePQErrorToLog("Fetch loop galcur in 'DropMetadataTables' failed\r\n",
                pCStr->pConn);
        }
        else nrows = PQntuples(res);
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);

    SetSP(pCStr->pConn, bTrans);
    //lsCmd = GetSaveSQL("close galcur", bTrans);
    res = PQexec(pCStr->pConn, "close galcur");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Close galcur in 'DropMetadataTables' failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
    }

    SetSP(pCStr->pConn, bTrans);
    sprintf(sCmd, "drop table %s", pCStr->sGAlias);
    //lsCmd = GetSaveSQL(sCmd, bTrans);
    res = PQexec(pCStr->pConn, sCmd);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        pCStr->iConnStat = icsStrangeMetadata;
        WritePQErrorToLog("Drop galiastable in 'DropMetadataTables' failed\r\n",
            pCStr->pConn);
        //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
    }
    else
    {
        pCStr->iConnStat = icsNoMetadata;
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
    }

    SetSP(pCStr->pConn, bTrans);
    //lsCmd = GetSaveSQL("drop schema gdo", bTrans);
    res = PQexec(pCStr->pConn, "drop schema gdo");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        pCStr->iConnStat = icsStrangeMetadata;
        WritePQErrorToLog("Drop schema in 'DropMetadataTables' failed\r\n",
            pCStr->pConn);
        //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCStr->pConn, bTrans);
    }
    return(true);
}

IDbuTables* CConnList::GetTables(long lHandle)
{
    if(lHandle > m_iListLen) return(NULL);
    if(lHandle < 1) return(NULL);

    PDbuTables pTbls = m_ppTables[lHandle - 1];
    if(!pTbls)
    {
        PConnStruct pCStr = m_ppData[lHandle - 1];
        pTbls = new CDbuTables(pCStr);
        m_ppTables[lHandle - 1] = pTbls;
        pTbls->BuildFromConn();
    }

    return((IDbuTables*)pTbls);
}

bool CConnList::UpdateAllTables(long lHandle, bool bVisible)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PDbuTables pTbls = m_ppTables[lHandle - 1];
    if(!pTbls)
    {
        PConnStruct pCStr = m_ppData[lHandle - 1];
        pTbls = new CDbuTables(pCStr);
        m_ppTables[lHandle - 1] = pTbls;
        pTbls->BuildFromConn();
    }

    pTbls->UpdateAllTables(bVisible);

    return(true);
}

bool CConnList::FillCtrlWithUserNames(long lHandle, HWND hwnd, UINT iMsg)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    TCHAR sBuf[64];
    _tcscpy(sBuf, _T("public"));
    SendMessage(hwnd, iMsg, 0, (LPARAM)sBuf);

    LPCSTR sCmd = "select rolname from pg_roles where not rolsuper and \
not rolname = current_user order by rolname";

    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;
    SetSP(pCStr->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexec(pCStr->pConn, sCmd);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Error reading user names: ", pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }

    Oid lType;
    VARIANT vName;
    VariantInit(&vName);
    DescribeFieldTypes(res, 1, &lType);

    int nrow = PQntuples(res);
    for(int i = 0; i < nrow; i++)
    {
        GetRecord(res, i, 1, &lType, &vName, pCStr->sDecSep);
        VarToTCharBuf(vName, sBuf, 64);
        SendMessage(hwnd, iMsg, 0, (LPARAM)sBuf);
        PgVariantClear(&vName);
    }

    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);
    return(true);
}

bool CConnList::FillCtrlWithSrids(long lHandle, HWND hwnd, UINT iMsg)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    PGresult *res = PQexec(pCStr->pConn,
        "select distinct(srid) from geometry_columns");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
        PQclear(res);
        return(false);
    }

    VARIANT V;
    VariantInit(&V);
    long lcnt = PQntuples(res);
    Oid iType;
    DescribeFieldTypes(res, 1, &iType);
    TCHAR sBuf[64];

    for(long li = 0; li < lcnt; li++)
    {
        GetRecord(res, li, 1, &iType, &V, '.');
        VarToTCharBuf(V, sBuf, 64);
        SendMessage(hwnd, iMsg, 0, (LPARAM)sBuf);
        VariantClear(&V);
    }
    PQclear(res);
    return(true);
}

bool CConnList::GrantGdoPrivileges(long lHandle, int iMode, LPWSTR sUser)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    char sCmdMask[128];
    char sCmd[256];
    char sName[64];
    WCharToCharBuf(sUser, sName, 64);

    char *pDot = strchr(pCStr->sGAlias, '.');
    if(pDot)
    {
        char sSchema[64];
        int iLen = pDot - pCStr->sGAlias;
        strncpy(sSchema, pCStr->sGAlias, iLen);
        sSchema[iLen] = 0;

        if(iMode < 1)
            strcpy(sCmdMask, "revoke all on all sequences in schema %s from %s");
        sprintf(sCmd, sCmdMask, sSchema, sName);
        DbuExecuteCommand(pCStr, sCmd, "Revoking sequences usage");

        switch(iMode)
        {
        case 0:
            strcpy(sCmdMask, "revoke usage on schema %s from %s");
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            strcpy(sCmdMask, "grant usage on schema %s to %s");
            break;
        default:
            return(1);
        }

        sprintf(sCmd, sCmdMask, sSchema, sName);
        DbuExecuteCommand(pCStr, sCmd, "Granting schema usage");

        if(iMode > 0)
            strcpy(sCmdMask, "grant all privileges on all sequences in schema %s to %s");
        else strcpy(sCmdMask, "revoke all privileges on all sequences in schema %s from %s");
        sprintf(sCmd, sCmdMask, sSchema, sName);
        DbuExecuteCommand(pCStr, sCmd, "Granting sequences usage");
    }

    switch(iMode)
    {
    case 0:
        strcpy(sCmdMask, "revoke all on %s from %s");
        break;
    case 1:
    case 2:
        strcpy(sCmdMask, "grant select on %s to %s");
        break;
    case 3:
        strcpy(sCmdMask, "grant all on %s to %s");
        break;
    case 4:
        strcpy(sCmdMask, "grant all on %s to %s with grant option");
        break;
    default:
        return(1);
    }

    sprintf(sCmd, sCmdMask, pCStr->sGAlias, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting galiastable");

    sprintf(sCmd, sCmdMask, pCStr->sGFeatures, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting gfeatures");

    sprintf(sCmd, sCmdMask, pCStr->sFieldLookup, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting fieldlookup");

    sprintf(sCmd, sCmdMask, pCStr->sAttributeProperties, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting attributeproperties");

    sprintf(sCmd, sCmdMask, pCStr->sGeometryProperties, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting geometryproperties");

    sprintf(sCmd, sCmdMask, pCStr->sOperatorsTable, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting sqloperatorstable");

    sprintf(sCmd, sCmdMask, pCStr->sCoordSystems, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting gcoordsystems");

    sprintf(sCmd, sCmdMask, pCStr->sPickLists, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting picklisttable");

    sprintf(sCmd, sCmdMask, pCStr->sGParameters, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting gparameters");

    if(iMode > 1)
    {
        if(iMode > 3)
            strcpy(sCmdMask, "grant all on %s to %s with grant option");
        else strcpy(sCmdMask, "grant all on %s to %s");
    }

    sprintf(sCmd, sCmdMask, pCStr->sGeomCols, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting geometry_columns");

    sprintf(sCmd, sCmdMask, pCStr->sSpatRefSys, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting spatial_ref_sys");

    sprintf(sCmd, sCmdMask, pCStr->sGraphicColumns, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting gdo_graphic_columns");

    sprintf(sCmd, sCmdMask, pCStr->sModTables, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting modifiedtables");

    sprintf(sCmd, sCmdMask, pCStr->sModLog, sName);
    DbuExecuteCommand(pCStr, sCmd, "Granting modificationlog");

    if(iMode > 1)
    {
        if(iMode > 3)
            sprintf(sCmd, "grant all on modificationlog_modificationnumber_seq to %s with grant option", sName);
        else
            sprintf(sCmd, "grant all on modificationlog_modificationnumber_seq to %s", sName);
        DbuExecuteCommand(pCStr, sCmd, "Granting modificationlog sequence usage");
        if(iMode > 3)
            sprintf(sCmd, "grant all on modifiedtables_modifiedtableid_seq to %s with grant option", sName);
        else
            sprintf(sCmd, "grant all on modifiedtables_modifiedtableid_seq to %s", sName);
        DbuExecuteCommand(pCStr, sCmd, "Granting modifiedtables sequence usage");
    }

    if(iMode > 2)
    {
        if(iMode > 3)
            sprintf(sCmd, "grant all on fieldlookup_indexid_seq to %s with grant option", sName);
        else
            sprintf(sCmd, "grant all on fieldlookup_indexid_seq to %s", sName);
        DbuExecuteCommand(pCStr, sCmd, "Granting fieldlookup sequence usage");
    }

    return(true);
}

bool CConnList::RunScript(long lHandle, LPWSTR sFileName)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    FILE *fp = _wfopen(sFileName, L"rb");
    fseek(fp, 0, SEEK_END);
    long flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    LPSTR sBuf = (LPSTR)malloc((flen + 1)*sizeof(char));
    fread(sBuf, 1, flen, fp);
    fclose(fp);
    sBuf[flen] = 0;

    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;
    SetSP(pCStr->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sBuf, bTrans);
    PGresult *res = PQexec(pCStr->pConn, sBuf);
    //free(lsCmd);
    free(sBuf);

    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Error custom script: ", pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }

    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);
    return(true);
}

bool CConnList::ClearModificationLog(long lHandle)
{
    if(lHandle > m_iListLen) return(false);
    if(lHandle < 1) return(false);

    PConnStruct pCStr = m_ppData[lHandle - 1];

    bool bTrans = pCStr->iSysTrans | pCStr->iGdoTrans;

    SetSP(pCStr->pConn, bTrans);

    char sSql[128];
    sprintf(sSql, "delete from %s", pCStr->sModLog);
    //LPSTR lsCmd = GetSaveSQL(sSql, bTrans);
    PGresult *res = PQexec(pCStr->pConn, sSql);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        //HandlePQError(m_hInstance, hwndDlg, pCStr->pConn);
        WritePQErrorToLog("Delete from modificationlog failed\r\n",
            pCStr->pConn);
        PQclear(res);
        RollbackSP(pCStr->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCStr->pConn, bTrans);
    return(true);
}
