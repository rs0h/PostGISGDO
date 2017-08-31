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

#include "GDOUtils.hpp"
#include <tchar.h>
#if DBGLEVEL > 0
#include "LogFiles.hpp"
#endif
#include "VarUtils.hpp"
#include "pg_const.h"
#include "PGtoGDOmaps.hpp"
#include "EWKBtoGDO.hpp"
#include "GDOtoEWKB.hpp"

void SetSP(PGconn *pConn, bool bTrans)
{
    if(bTrans)
    {
        PGresult *res = PQexec(pConn, "SAVEPOINT locsvpt");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
            WritePQErrorToLog("Spot 100: ", pConn);
        PQclear(res);
    }
    return;
}

void RollbackSP(PGconn *pConn, bool bTrans)
{
    if(bTrans)
    {
        /*PGresult *res = PQexec(pConn, "ROLLBACK TO SAVEPOINT locsvpt");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
            WritePQErrorToLog("Spot 101: ", pConn);
        PQclear(res);
        res = PQexec(pConn, "RELEASE SAVEPOINT locsvpt");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
            WritePQErrorToLog("Spot 102: ", pConn);
        PQclear(res);*/

        PGresult *res = PQexec(pConn,
            "ROLLBACK TO SAVEPOINT locsvpt;RELEASE SAVEPOINT locsvpt;");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
            WritePQErrorToLog("Spot 101: ", pConn);
        PQclear(res);
    }
    return;
}

void ReleaseSP(PGconn *pConn, bool bTrans)
{
    if(bTrans)
    {
        PGresult *res = PQexec(pConn, "RELEASE SAVEPOINT locsvpt");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
            WritePQErrorToLog("Spot 103: ", pConn);
        PQclear(res);
    }
    return;
}

/*LPSTR GetSaveSQL(LPCSTR sCmd, bool bTrans)
{
    LPSTR sRes = NULL;

    int iLen = strlen(sCmd) + 1;
    if(bTrans)
    {
        iLen += 20;
        sRes = (LPSTR)malloc(iLen*sizeof(char));
        strcpy(sRes, "SAVEPOINT locsvpt;");
        strcat(sRes, sCmd);
    }
    else
    {
        sRes = (LPSTR)malloc(iLen*sizeof(char));
        strcpy(sRes, sCmd);
    }
    return sRes;
}*/

bool DbuExecuteCommand(PConnStruct pCSt, LPCSTR sCmd, LPCSTR sDesc)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuExecuteCommand\r\n    ", true);
    WriteLogFile(sCmd, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL
    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;
    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexec(pCSt->pConn, sCmd);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog(sDesc, pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return(false);
    }
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return(true);
}

void GetEncoding(PConnStruct pCSt, LPSTR sDBName, bool bTrans)
{
pCSt->lDBEnc = 6;
return;
    LPSTR psDBName = sDBName;
    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexecParams(pCSt->pConn,
        "select encoding, pg_encoding_to_char(encoding) from pg_database where datname=$1",
        1, NULL, &psDBName, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Query DB Encoding failed\r\n", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }

    Oid piTypes[2];
    VARIANT pvVal[2];
    VariantInit(&pvVal[0]);
    VariantInit(&pvVal[1]);
    DescribeFieldTypes(res, 2, piTypes);
    GetRecord(res, 0, 2, piTypes, pvVal, pCSt->sDecSep);
    pCSt->lDBEnc = VarToLong(pvVal[0]);
    pCSt->sDBEncName[0] = 0;
    if(pvVal[1].vt == VT_LPSTR)
    {
        strcpy(pCSt->sDBEncName, pvVal[1].pcVal);
    }
#if DBGLEVEL > 0
    WriteLogFile("DB Encoding:\r\n  Numeric: ", true);
    char buf[64];
    sprintf(buf, "%ld\r\n  Name: ", pCSt->lDBEnc);
    WriteLogFile(buf, false);
    WriteLogFile(pCSt->sDBEncName, false);
    WriteLogFile("\r\n", false);
#endif
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return;
}

void GetGeomTypeOid(PConnStruct pCSt, bool bTrans)
{
    pCSt->lGeomOid = 0;
    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL("select oid from pg_type where typname='geometry'", bTrans);
    PGresult *res = PQexec(pCSt->pConn, "select oid from pg_type where typname='geometry'");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Query DB Encoding failed\r\n", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        return;
    }

    Oid iType;
    VARIANT vVal;
    VariantInit(&vVal);
    DescribeFieldTypes(res, 1, &iType);
    GetRecord(res, 0, 1, &iType, &vVal, pCSt->sDecSep);
    pCSt->lGeomOid = VarToLong(vVal);
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return;
}

bool GetFullTblName(PGconn *pConn, LPCSTR sTblName, LPCSTR sPrefSchema,
    LPSTR sNameBuf, bool bTrans)
{
    LPCSTR psParams[2];
    psParams[0] = sTblName;
    psParams[1] = sPrefSchema;

    SetSP(pConn, bTrans);
    PGresult *res = PQexecParams(pConn, "select * from information_schema.tables where \
table_name = $1 and table_schema = $2", 2, NULL, psParams, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'GetFullTblName' failed\r\n", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(false);
    }

    long lrows = PQntuples(res);
    PQclear(res);
    ReleaseSP(pConn, bTrans);

    sNameBuf[0] = 0;
    if(lrows > 0)
    {
        if(strcmp(sPrefSchema, "public") != 0)
        {
            strcpy(sNameBuf, sPrefSchema);
            strcat(sNameBuf, ".");
        }
        strcat(sNameBuf, sTblName);
    }
    else
    {
        SetSP(pConn, bTrans);
        res = PQexecParams(pConn, "select * from information_schema.tables where \
table_name = $1", 1, NULL, psParams, NULL, NULL, 0);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Select 2 in 'GetFullTblName' failed\r\n", pConn);
            PQclear(res);
            RollbackSP(pConn, bTrans);
            return(false);
        }
        lrows = PQntuples(res);
        if(lrows < 1)
        {
            PQclear(res);
            ReleaseSP(pConn, bTrans);
            return(false);
        }

        int idx = PQfnumber(res, "table_schema");
        LPSTR sbuf = PQgetvalue(res, 0, idx);

        if(strcmp(sbuf, "public") != 0)
        {
            strcpy(sNameBuf, sbuf);
            strcat(sNameBuf, ".");
        }
        strcat(sNameBuf, sTblName);

        PQclear(res);
        ReleaseSP(pConn, bTrans);
    }
    return(true);
}

bool IsTableWritable(PGconn *pConn, LPCSTR sSchema, LPCSTR sTblName, bool bTrans)
{
    LPCSTR psParams[2];
    psParams[0] = sTblName;
    psParams[1] = sSchema;

    SetSP(pConn, bTrans);
    PGresult *res = PQexecParams(pConn, "select * from information_schema.table_privileges where \
table_name = $1 and table_schema = $2 and privilege_type = 'INSERT'", 2, NULL, psParams, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'IsTableWritable' failed\r\n", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(false);
    }

    long lrows = PQntuples(res);
    PQclear(res);
    ReleaseSP(pConn, bTrans);

    if(lrows < 1)
    {
        SetSP(pConn, bTrans);
        PGresult *res = PQexecParams(pConn, "select * from pg_tables where \
tablename = $1 and schemaname = $2 and tableowner = current_user", 2, NULL, psParams, NULL, NULL, 0);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Select in 'IsTableWritable' failed\r\n", pConn);
            PQclear(res);
            RollbackSP(pConn, bTrans);
            return(false);
        }

        lrows = PQntuples(res);
        PQclear(res);
        ReleaseSP(pConn, bTrans);
    }

    return(lrows > 0);
}

// return values:
// 0 - table does not exists
// 1 - table is read only
// 2 - table is read write
int TableExists(PGconn *pConn, LPCSTR sTblName, bool bTrans)
{
    LPCSTR psParams[2];
    char sSchema[64];
    LPSTR sDot = strchr(sTblName, '.');
    if(sDot)
    {
        strncpy(sSchema, sTblName, sDot - sTblName);
        sSchema[sDot - sTblName] = 0;
        sDot++;
        psParams[0] = sDot;
        psParams[1] = sSchema;
    }
    else
    {
        psParams[0] = sTblName;
        strcpy(sSchema, "public");
        psParams[1] = sSchema;
    }

    SetSP(pConn, bTrans);
    PGresult *res = PQexecPrepared(pConn, "stmttbls", 2, psParams, NULL,
        NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'TableExists' failed\r\n", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(0);
    }
    long lrows = PQntuples(res);
    int iRes = 0;
    PQclear(res);
    ReleaseSP(pConn, bTrans);

    if(lrows > 0)
    {
        iRes = 1;
        if(IsTableWritable(pConn, sSchema, psParams[0], bTrans)) iRes = 2;
    }
    return(iRes);
}

// return values:
// 0 - no record exists or its value is empty
// 1 - record exists but the referenced table does not
// 2 - both the record and referenced table exist
// 3 - table exists and is writable
int CheckGAliasRecord(PGconn *pConn, int iNameIdx, LPCSTR sType, bool bTrans,
    LPCSTR sGAlias)
{
    SetSP(pConn, bTrans);

    char sBuf[128];
    sprintf(sBuf, "select * from %s where TableType = '%s'", sGAlias, sType);
    //LPSTR lsCmd = GetSaveSQL(sBuf, bTrans);
    PGresult *res = PQexec(pConn, sBuf);
    //free(lsCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select galiastable in 'CheckGAliasRecord' failed\r\n",
            pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(0);
    }

    long lcnt = PQntuples(res);
    if(lcnt < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return(0);
    }

    if(PQgetisnull(res, 0, iNameIdx) > 0)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return(0);
    }

    LPSTR sName = PQgetvalue(res, 0, iNameIdx);
    if(!sName)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return(0);
    }

    if(strlen(sName) < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return(0);
    }

    strcpy(sBuf, sName);

    PQclear(res);
    ReleaseSP(pConn, bTrans);

    int iRes = 1 + TableExists(pConn, sBuf, bTrans);

    return(iRes);
}

CConnStatus GetConnStatus(PGconn *pConn, bool bFixCSRef, bool bTrans,
    LPSTR sGeomCols, LPSTR sSpatRefSys, LPSTR sGAlias, int *piUpdatable)
{
    if(piUpdatable) *piUpdatable = 0;

    ConnStatusType ist = PQstatus(pConn);

    if(ist != CONNECTION_OK) return(icsClosed);

    // check whether we have permission for "pg_tables" table
    SetSP(pConn, bTrans);
    PGresult *res = PQprepare(pConn, "stmttbls",
        "select * from information_schema.tables where table_name = $1 and table_schema = $2", 0,
        NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Select information_schema.tables in 'GetConnStatus' failed\r\n", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(icsNotEnoughPrivileges);
    }
    PQclear(res);
    ReleaseSP(pConn, bTrans);

    // check whether the "spatial_ref_sys" table exists
    if(!GetFullTblName(pConn, "spatial_ref_sys", "public", sSpatRefSys, bTrans))
        return(icsNoPostGis);
    if(!GetFullTblName(pConn, "geometry_columns", "public", sGeomCols, bTrans))
        return(icsNoPostGis);

    // check whether the GAliasTable exists
    if(!GetFullTblName(pConn, "galiastable", "gdo", sGAlias, bTrans))
    {
        strcpy(sGAlias, "galiastable");
        return(icsNoMetadata);
    }

    char sSql[128];
    sprintf(sSql, "select * from %s", sGAlias);

    SetSP(pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sSql, bTrans);
    res = PQexec(pConn, sSql);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select galiastable in 'GetConnStatus' failed\r\n",
            pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        strcpy(sGAlias, "galiastable");
        return(icsNoMetadata);
    }

    CConnStatus cRes = icsNoMetadata;

    // if it exists, check whether the fields are OK:
    int cnt = PQnfields(res);
    int iTypeIdx = -1;
    int iNameIdx = -1;

    LPSTR sName;
    if(cnt == 2)
    {
        for(int i = 0; i < cnt; i++)
        {
            sName = PQfname(res, i);
            if(stricmp(sName, "TableType") == 0) iTypeIdx = i;
            else if(stricmp(sName, "TableName") == 0) iNameIdx = i;
        }
        if((iTypeIdx < 0) || (iNameIdx < 0)) cRes = icsStrangeMetadata;
    }
    else cRes = icsStrangeMetadata;
    PQclear(res);
    ReleaseSP(pConn, bTrans);

    if(cRes != icsStrangeMetadata) // which means that both pFldType
        // and pFldName are assigned
    {
        cRes = icsSimpleMetadata;
        int iCheckRes;
        long lcnt = 0;
        bool bUpdate = false;

        SetSP(pConn, bTrans);
        sprintf(sSql, "select * from %s where TableType = 'GCoordSystemTable'",
            sGAlias);
        //lsCmd = GetSaveSQL(sSql, bTrans);
        res = PQexec(pConn, sSql);
        //free(lsCmd);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Select galiastable with where in 'GetConnStatus' failed\r\n",
                pConn);
            PQclear(res);
            RollbackSP(pConn, bTrans);
        }
        else
        {
            lcnt = PQntuples(res);
            if((lcnt > 0) && bFixCSRef)
            {
                bUpdate = (PQgetisnull(res, 0, iNameIdx) > 0);
                if(!bUpdate)
                {
                    sName = PQgetvalue(res, 0, iNameIdx);
                    if(sName) bUpdate = (strlen(sName) < 1);
                    else bUpdate = true;
                }
            }
            PQclear(res);
            ReleaseSP(pConn, bTrans);
        }

        // we don't care about the GCoordSystemTable name. It is
        // sufficient that it exists
        if(lcnt < 1) cRes = icsSimpleMetaIncomplete;
        // but if we want to fix the CS reference, we have to do it now
        else if(bFixCSRef && bUpdate)
        {
            SetSP(pConn, bTrans);
            sprintf(sSql, "update %s set TableName = 'gcoordsystem' where \
TableType = 'GCoordSystemTable'", sGAlias);
            res = PQexec(pConn, sSql);
            if(PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                WritePQErrorToLog("Update galiastable with CS in 'GetConnStatus' failed\r\n",
                    pConn);
                PQclear(res);
                RollbackSP(pConn, bTrans);
            }
            else
            {
                PQclear(res);
                ReleaseSP(pConn, bTrans);
            }
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            if(CheckGAliasRecord(pConn, iNameIdx, "GModifications",
                bTrans, sGAlias) < 2) cRes = icsSimpleMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx, "GModifiedTables", bTrans,
                sGAlias);
            if(iCheckRes < 2) cRes = icsSimpleMetaIncomplete;
            else if((iCheckRes > 2) && piUpdatable) *piUpdatable = 1;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx, "INGRFeatures",
                bTrans, sGAlias);
            if(iCheckRes > 1)
            {
                cRes = icsINGRMetadata;
                if((iCheckRes > 2) && piUpdatable) *piUpdatable = 2;
            }
            else if(iCheckRes > 0) cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx, "INGRFieldLookup",
                bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx,
                "INGRAttributeProperties", bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx,
                "INGRGeometryProperties", bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx, "INGRPickLists",
                bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx,
                "INGRSQLOperatorsTable", bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsSimpleMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx,
                "INGRDictionaryProperties", bTrans, sGAlias);
            if((cRes == icsINGRMetadata) && (iCheckRes < 1))
                cRes = icsINGRMetaIncomplete;
        }

        if(cRes > icsINGRMetaIncomplete)
        {
            iCheckRes = CheckGAliasRecord(pConn, iNameIdx,
                "GCoordSystemTable", bTrans, sGAlias);
            if((iCheckRes == 1) ||
                ((cRes == icsINGRMetadata) && (iCheckRes < 2)))
                    cRes = icsINGRMetaIncomplete;
        }
    }

    return(cRes);
}

void GetMetaTable(PGconn *pConn, LPCSTR sStmt, LPCSTR sTblType, LPSTR sTblName,
    bool bTrans)
{
    SetSP(pConn, bTrans);
    PGresult *res = PQexecPrepared(pConn, sStmt, 1, &sTblType, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Execute statement in 'GetMetaTable' failed\r\n",
            pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return;
    }

    long lcnt = PQntuples(res);
    if(lcnt < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return;
    }

    if(PQgetisnull(res, 0, 0) > 0)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return;
    }

    LPSTR sName = PQgetvalue(res, 0, 0);
    if(!sName)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return;
    }

    if(strlen(sName) < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return;
    }

    strcpy(sTblName, sName);
    PQclear(res);
    ReleaseSP(pConn, bTrans);
    return;
}

void GetMetaTableNames(PConnStruct pCSt, bool bTrans)
{
    SetSP(pCSt->pConn, bTrans);
    char sSql[128];
    sprintf(sSql, "select TableName from %s where TableType = $1",
        pCSt->sGAlias);
    PGresult *res = PQprepare(pCSt->pConn, "stmtgalias", sSql, 0, NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Prepare statement in 'GetMetaTableNames' failed\r\n",
            pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);

    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRFeatures", pCSt->sGFeatures,
        bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRFieldLookup", pCSt->sFieldLookup,
        bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRAttributeProperties",
        pCSt->sAttributeProperties, bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRGeometryProperties",
        pCSt->sGeometryProperties, bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRSQLOperatorsTable",
        pCSt->sOperatorsTable, bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "GCoordSystemTable", pCSt->sCoordSystems,
        bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "GModifiedTables", pCSt->sModTables,
        bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "GModifications", pCSt->sModLog, bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "INGRPickLists", pCSt->sPickLists,
        bTrans);
    GetMetaTable(pCSt->pConn, "stmtgalias", "GParameters", pCSt->sGParameters,
        bTrans);

    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL("DEALLOCATE stmtgalias", bTrans);
    res = PQexec(pCSt->pConn, "DEALLOCATE stmtgalias");
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Deallocate statement in 'GetMetaTableNames' failed\r\n",
            pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return;
}

void UpdateCSGuids(PGconn *pConn, PCSArray pCSTable, LPSTR sCSTable,
    long lEnc, bool bTrans)
{
    int ilen = strlen("select * from ") + strlen(sCSTable) + 2;
    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sCmd, "select * from %s", sCSTable);

    SetSP(pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexecParams(pConn, sCmd, 0, NULL, NULL, NULL, NULL, 1);
    //free(lsCmd);
    free(sCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 10: ", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    DescribeFieldTypes(res, ncols, piTypes);
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);
    VARIANT *pRow;

    for(long li = 0; li < nrows; li++)
    {
        GetRecordBin(res, li, ncols, piTypes, pvRow, lEnc, 0, NULL, 0);
        pRow = FindCSTableRowByCS(pCSTable, pvRow);
        if(pRow)
        {
            VariantCopy(&pRow[1], &pvRow[0]);
        }
    }

    for(int i = 0; i < ncols; i++) VariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    PQclear(res);
    ReleaseSP(pConn, bTrans);
    return;
}

void InitConnStruct(PConnStruct pCSt, LPSTR sDBName, LPWSTR wsServerName,
    bool bFixCSRef)
{
    pCSt->sGeomCols[0] = 0;
    pCSt->sSpatRefSys[0] = 0;
    pCSt->sGAlias[0] = 0;
    pCSt->sGFeatures[0] = 0;
    pCSt->sFieldLookup[0] = 0;
    pCSt->sAttributeProperties[0] = 0;
    pCSt->sGeometryProperties[0] = 0;
    pCSt->sOperatorsTable[0] = 0;
    strcpy(pCSt->sCoordSystems, "gcoordsystem");
    pCSt->sModTables[0] = 0;
    pCSt->sModLog[0] = 0;
    pCSt->sPickLists[0] = 0;
    pCSt->sGraphicColumns[0] = 0;
    pCSt->iSysTrans = 0;
    pCSt->iGdoTrans = 0;
    pCSt->bFtrPrepared = false;
    pCSt->bGeomPrepared = false;
    pCSt->bGraphicPrepared = false;
    pCSt->bSchemaUpdatable = false;
    pCSt->bUpdatable = false;

    TCHAR sBuf[32];
    pCSt->sDecSep = '.';
    int iInfo = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, sBuf, 32);
    if(iInfo > 0) pCSt->sDecSep = sBuf[0];
    pCSt->iCurrDigits = 2;
    iInfo = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRDIGITS, sBuf, 32);
    if(iInfo > 0) _stscanf(sBuf, _T("%d"), &pCSt->iCurrDigits);

    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;
    int iUpdatable = 0;

    pCSt->iConnStat = GetConnStatus(pCSt->pConn, bFixCSRef, bTrans,
        pCSt->sGeomCols, pCSt->sSpatRefSys, pCSt->sGAlias, &iUpdatable);
    if(pCSt->iConnStat < icsNotEnoughPrivileges) return;

    if(iUpdatable > 0) pCSt->bUpdatable = true;
    if(iUpdatable > 1) pCSt->bSchemaUpdatable = true;

    if(wsServerName)
    {
        int ilen = wcslen(wsServerName) + 1;
        pCSt->wsServerName = (LPWSTR)malloc(ilen*sizeof(wchar_t));
        wcscpy(pCSt->wsServerName, wsServerName);
    }

    GetEncoding(pCSt, sDBName, bTrans);
    GetGeomTypeOid(pCSt, bTrans);

    if(pCSt->iConnStat > icsNoMetadata) GetMetaTableNames(pCSt, bTrans);

    GetFullTblName(pCSt->pConn, "gdo_graphic_columns", "public",
        pCSt->sGraphicColumns, bTrans);

    if(pCSt->iConnStat > icsSimpleMetadata)
        UpdateCSGuids(pCSt->pConn, &pCSt->cCSTable, pCSt->sCoordSystems,
            pCSt->lDBEnc, bTrans);

    //pCSt->bReadWrite = (m_cConnStatus >= icsSimpleMetadata);

    char sSql[256];
    PGresult *res;

    SetSP(pCSt->pConn, bTrans);
    sprintf(sSql, "select coord_dimension, srid, type from %s where \
f_table_schema = $1 and f_table_name = $2 and f_geometry_column = $3",
        pCSt->sGeomCols);
    res = PQprepare(pCSt->pConn, "stmtgeom", sSql, 0, NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Prepare select geometry_columns in 'InitConnStruct' failed\r\n",
            pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
    }
    else
    {
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        pCSt->bGeomPrepared = true;
    }

    if(pCSt->sGraphicColumns[0])
    {
        SetSP(pCSt->pConn, bTrans);
        sprintf(sSql, "select coord_dimension, srid, type, f_native_geom_column, \
f_plain_label_column from %s where \
f_table_schema = $1 and f_table_name = $2 and f_graphic_column = $3",
            pCSt->sGraphicColumns);
        res = PQprepare(pCSt->pConn, "stmtgrph", sSql, 0, NULL);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Prepare select graphic_columns in 'InitConnStruct' failed\r\n",
                pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
            pCSt->bGraphicPrepared = true;
        }
    }

    if(pCSt->iConnStat == icsINGRMetadata)
    {
        SetSP(pCSt->pConn, bTrans);
        sprintf(sSql, "select * from %s where featurename = $1", pCSt->sGFeatures);
        res = PQprepare(pCSt->pConn, "stmtftr", sSql, 0, NULL);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Prepare select GFeatures in 'InitConnStruct' failed\r\n",
                pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
            pCSt->bFtrPrepared = true;
        }
    }
    return;
}

long GetGeometryType(PConnStruct pCSt, LPSTR sSchemaName, LPSTR sTblName,
    LPSTR sFldName, int *piDim, unsigned long *pulSrid, int *piPsgType,
    LPWSTR sCSGuid)
{
    bool bTrans = (pCSt->iSysTrans | pCSt->iGdoTrans);

    char sSchema[8];
    strcpy(sSchema, "public");

    LPSTR psPar[3];
    psPar[0] = sSchema;
    psPar[1] = sTblName;
    psPar[2] = sFldName;

    if(sSchemaName) psPar[0] = sSchemaName;

    PGresult *res;
    Oid piTypes[5];
    VARIANT pvRes[5];
    for(int i = 0; i < 5; i++) VariantInit(&pvRes[i]);
    int ncols, nrows;

    if(pCSt->bGeomPrepared)
    {
        SetSP(pCSt->pConn, bTrans);
        res = PQexecPrepared(pCSt->pConn, "stmtgeom", 3, psPar, NULL, NULL, 0);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Execute statement geometry column in 'GetGeometryType' failed\r\n",
                pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            ncols = PQnfields(res);
            nrows = PQntuples(res);
            if((nrows > 0) && (ncols == 3))
            {
                DescribeFieldTypes(res, ncols, piTypes);
                GetRecord(res, 0, ncols, piTypes, pvRes, pCSt->sDecSep);
            }

            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
        }
    }

    if((pvRes[2].vt != VT_LPSTR) && pCSt->bGraphicPrepared)
    {
        SetSP(pCSt->pConn, bTrans);
        res = PQexecPrepared(pCSt->pConn, "stmtgrph", 3, psPar, NULL, NULL, 0);
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Execute statement graphic column in 'GetGeometryType' failed\r\n",
                pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            ncols = PQnfields(res);
            nrows = PQntuples(res);
            if((nrows > 0) && (ncols == 5))
            {
                DescribeFieldTypes(res, ncols, piTypes);
                GetRecord(res, 0, ncols, piTypes, pvRes, pCSt->sDecSep);
            }

            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
        }
    }

    if(pvRes[2].vt != VT_LPSTR)
    {
        for(int i = 0; i < 5; i++) PgVariantClear(&pvRes[i]);
        return(0);
    }

    if(piDim) *piDim = VarToInt(pvRes[0]);
    unsigned long ulSrid = VarToLong(pvRes[1]);

    VARIANT *pRow = FindCSTableRow(&pCSt->cCSTable, ulSrid);
    if(pulSrid) *pulSrid = ulSrid;

    // this method is used both by PostGIS GDO and PsgDButils. While in
    // GDO we are interesting in CSGUID and we will create a dynamic one,
    // if it does not exists, in DB Utils we don't want to know the CSGUID and
    // we don't want to alter the CS list. So when the sCSGuid is NULL, do not
    // modify the CS row.
    if(pRow && sCSGuid)
    {
        if(pRow[1].vt != VT_BSTR)
        {
            UUID uid;
            UuidCreate(&uid);
            pRow[1].vt = VT_BSTR;
            pRow[1].bstrVal = SysAllocStringLen(NULL, 40);
            swprintf(pRow[1].bstrVal,
                L"{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}",
                uid.Data1, uid.Data2, uid.Data3, uid.Data4[0],
                uid.Data4[1], uid.Data4[2], uid.Data4[3], uid.Data4[4],
                uid.Data4[5], uid.Data4[6], uid.Data4[7]);
        }
        wcscpy(sCSGuid, pRow[1].bstrVal);
    }

    if(pvRes[2].vt == VT_LPSTR)
    {
        if(strcmp(pvRes[2].pcVal, "GEOMETRY") == 0) *piPsgType = 0;
        else if(strcmp(pvRes[2].pcVal, "GEOMETRYCOLLECTION") == 0) *piPsgType = 7;
        else if(strcmp(pvRes[2].pcVal, "POINT") == 0) *piPsgType = 1;
        else if(strcmp(pvRes[2].pcVal, "MULTIPOINT") == 0) *piPsgType = 4;
        else if(strcmp(pvRes[2].pcVal, "POLYGON") == 0) *piPsgType = 3;
        else if(strcmp(pvRes[2].pcVal, "MULTIPOLYGON") == 0) *piPsgType = 6;
        else if(strcmp(pvRes[2].pcVal, "LINESTRING") == 0) *piPsgType = 2;
        else if(strcmp(pvRes[2].pcVal, "MULTILINESTRING") == 0) *piPsgType = 5;
        else if(strcmp(pvRes[2].pcVal, "CIRCULARSTRING") == 0) *piPsgType = 8;
        else if(strcmp(pvRes[2].pcVal, "COMPOUNDCURVE") == 0) *piPsgType = 9;
        else if(strcmp(pvRes[2].pcVal, "MULTICURVE") == 0) *piPsgType = 11;
        else if(strcmp(pvRes[2].pcVal, "CURVEPOLYGON") == 0) *piPsgType = 13;
        //else if(strcmp(pvRes[2].pcVal, "MULTICURVE") == 0) *piPsgType = 14;
        else if(strcmp(pvRes[2].pcVal, "MULTISURFACE") == 0) *piPsgType = 15;
        else if(strcmp(pvRes[2].pcVal, "GEOMETRYCOLLECTIONM") == 0) *piPsgType = 7;
        else if(strcmp(pvRes[2].pcVal, "POINTM") == 0) *piPsgType = 1;
        else if(strcmp(pvRes[2].pcVal, "MULTIPOINTM") == 0) *piPsgType = 4;
        else if(strcmp(pvRes[2].pcVal, "POLYGONM") == 0) *piPsgType = 3;
        else if(strcmp(pvRes[2].pcVal, "MULTIPOLYGONM") == 0) *piPsgType = 6;
        else if(strcmp(pvRes[2].pcVal, "LINESTRINGM") == 0) *piPsgType = 2;
        else if(strcmp(pvRes[2].pcVal, "MULTILINESTRINGM") == 0) *piPsgType = 5;
        else if(strcmp(pvRes[2].pcVal, "CIRCULARSTRINGM") == 0) *piPsgType = 8;
        else if(strcmp(pvRes[2].pcVal, "COMPOUNDCURVEM") == 0) *piPsgType = 9;
        else if(strcmp(pvRes[2].pcVal, "MULTICURVEM") == 0) *piPsgType = 11;
        else if(strcmp(pvRes[2].pcVal, "CURVEPOLYGONM") == 0) *piPsgType = 13;
        //else if(strcmp(pvRes[2].pcVal, "MULTICURVEM") == 0) *piPsgType = 14;
        else if(strcmp(pvRes[2].pcVal, "MULTISURFACEM") == 0) *piPsgType = 15;
        else if(strcmp(pvRes[2].pcVal, "GRAPHICTEXT") == 0) *piPsgType = 20;
        else if(strcmp(pvRes[2].pcVal, "IMAGE") == 0) *piPsgType = 21;
    }
    for(int i = 0; i < 5; i++) PgVariantClear(&pvRes[i]);

    long lType = 0;
    switch(*piPsgType)
    {
    case 0:
    case 7:
        lType = 3;
        break;
    case 1:
    case 4:
        lType = 10;
        break;
    case 3:
    case 6:
    case 13:
    case 15:
        lType = 2;
        break;
    case 2:
    case 5:
    case 8:
    case 9:
    case 11:
    case 14:
        lType = 1;
        break;
    case 20:
        lType = 5;
        break;
    case 21:
        lType = 4;
        break;
    }

    return(lType);
}

long GetFieldId(PConnStruct pCSt, LPWSTR wsTblName, LPWSTR wsFldName,
    bool bInsert)
{
    bool bTrans = (pCSt->iSysTrans | pCSt->iGdoTrans);

    int ilen;
    LPSTR sCmd;
    PGresult *res;
    LPSTR psParam[2];
    psParam[0] = WCharToDBStr(wsTblName, pCSt->lDBEnc);
    psParam[1] = WCharToDBStr(wsFldName, pCSt->lDBEnc);

    if(bInsert)
    {
        ilen = strlen("insert into (featurename, fieldname) values ($1, $2)") +
            strlen(pCSt->sFieldLookup);
        sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
        sprintf(sCmd, "insert into %s(featurename, fieldname) values ($1, $2)",
            pCSt->sFieldLookup);

        SetSP(pCSt->pConn, bTrans);
        PGresult *res = PQexecParams(pCSt->pConn, sCmd, 2, NULL, psParam, NULL,
            NULL, 0);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Execute in 'GetFieldId' failed\r\n",
                pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
        }
        free(sCmd);
    }

    ilen = strlen("select indexid from  where featurename = $1 and fieldname = $2") +
        strlen(pCSt->sFieldLookup);
    sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "select indexid from %s where featurename = $1 and fieldname = $2",
        pCSt->sFieldLookup);
    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    res = PQexecParams(pCSt->pConn, sCmd, 2, NULL, psParam, NULL, NULL, 1);
    //free(lsCmd);
    free(sCmd);

    free(psParam[1]);
    free(psParam[0]);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'GetFieldId' failed\r\n",
            pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return(-1);
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        return(-1);
    }

    Oid iType = PQftype(res, 0);
    VARIANT vVal;
    VariantInit(&vVal);
    GetRecordBin(res, 0, 1, &iType, &vVal, 0, pCSt->lGeomOid, NULL, pCSt->iCurrDigits);
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);

    return(VarToLong(vVal));
}

void CheckAttrMetaData(PConnStruct pCSt, LPWSTR wsFldName, LPWSTR wsTblName,
    bool *pbIsKey)
{
    if(!pCSt->sAttributeProperties[0]) return;

    long lFldIdx = GetFieldId(pCSt, wsTblName, wsFldName, false);
    if(lFldIdx < 1) return;

    int ilen = strlen("select * from  where indexid = ") +
        strlen(pCSt->sAttributeProperties) + 32;
    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sCmd, "select * from %s where indexid = %ld",
        pCSt->sAttributeProperties, lFldIdx);

    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;

    SetSP(pCSt->pConn, bTrans);
    PGresult *res = PQexecParams(pCSt->pConn, sCmd, 0, NULL, NULL, NULL,
        NULL, 1);
    free(sCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 11b: ", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvRows = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRows[i]);

    DescribeFieldTypes(res, ncols, piTypes);
    GetRecordBin(res, 0, ncols, piTypes, pvRows, pCSt->lDBEnc, pCSt->lGeomOid,
        NULL, pCSt->iCurrDigits);

    int idx = PQfnumber(res, "iskeyfield");
    if(idx > -1) *pbIsKey = VarToLong(pvRows[idx]);

    for(int i = 0; i < ncols; i++) VariantClear(&pvRows[i]);

    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);
    return;
}

bool CheckGeomMetaData(PConnStruct pCSt, LPWSTR wsFldName, LPWSTR wsTblName,
    long *plSubType, LPWSTR sCSGuid)
{
    if(!pCSt->sGeometryProperties[0]) return(false);

    long lFldIdx = GetFieldId(pCSt, wsTblName, wsFldName, false);
    if(lFldIdx < 1) return(false);

    int ilen = strlen("select * from  where indexid = ") +
        strlen(pCSt->sGeometryProperties) + 32;
    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    sprintf(sCmd, "select * from %s where indexid = %ld",
        pCSt->sGeometryProperties, lFldIdx);

    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;

    SetSP(pCSt->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexecParams(pCSt->pConn, sCmd, 0, NULL, NULL, NULL,
        NULL, 1);
    //free(lsCmd);
    free(sCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 11: ", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return(false);
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        return(false);
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvRows = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRows[i]);

    DescribeFieldTypes(res, ncols, piTypes);
    GetRecordBin(res, 0, ncols, piTypes, pvRows, pCSt->lDBEnc, pCSt->lGeomOid,
        NULL, pCSt->iCurrDigits);

    int idx = PQfnumber(res, "geometrytype");
    if(idx > -1) *plSubType = VarToLong(pvRows[idx]);

    bool bUpdateCS = false;
    idx = PQfnumber(res, "gcoordsystemguid");
    if(idx > -1)
    {
        if(sCSGuid[0])
        {
            if(pvRows[idx].vt != VT_BSTR) bUpdateCS = true;
            else bUpdateCS = (wcsicmp(pvRows[idx].bstrVal, sCSGuid) != 0);
        }
        else VarToWideChar(pvRows[idx], sCSGuid, 40, false, false);
    }

    for(int i = 0; i < ncols; i++) VariantClear(&pvRows[i]);

    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);

    if(bUpdateCS)
    {
        ilen = strlen("update  set gcoordsystemguid = $1 where indexid = $2") +
            strlen(pCSt->sGeometryProperties) + 2;
        sCmd = (LPSTR)malloc(ilen*sizeof(char));
        sprintf(sCmd, "update %s set gcoordsystemguid = $1 where indexid = $2",
            pCSt->sGeometryProperties);
        LPSTR sPar1 = BStrToChar(sCSGuid);
        char sPar2[32];
        sprintf(sPar2, "%ld", lFldIdx);
        LPSTR psPars[2];
        psPars[0] = sPar1;
        psPars[1] = sPar2;

        SetSP(pCSt->pConn, bTrans);
        res = PQexecParams(pCSt->pConn, sCmd, 2, NULL, psPars, NULL, NULL, 0);
        free(sPar1);
        free(sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Spot 12: ", pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
        }
        else
        {
            PQclear(res);
            ReleaseSP(pCSt->pConn, bTrans);
        }
    }
    return(true);
}

void DescribeFieldTypes(PGresult *res, int ncols, Oid *piTypes)
{
    for(int i = 0; i < ncols; i++)
    {
        piTypes[i] = PQftype(res, i);
    }
    return;
}

void PgVariantClear(VARIANT *pV)
{
    if(pV->vt == VT_LPSTR) free(pV->pcVal);
    else VariantClear(pV);
    //if(pV->vt != VT_LPSTR) VariantClear(pV);
    pV->vt = VT_EMPTY;
    return;
}

LONGLONG CurrencyFromDBString(LPSTR psVal, char sDecSep)
{
    LONGLONG llRes = 0;
    int slen = strlen(psVal);
    LPSTR sBuf = (LPSTR)malloc((slen + 6)*sizeof(char));
    int j = 0;
    int iDecSepPos = -1;
    for(int i = 0; i < slen; i++)
    {
        if((psVal[i] >= '0') && (psVal[i] <= '9')) sBuf[j++] = psVal[i];
        else if((psVal[i] == sDecSep) && (iDecSepPos < 0)) iDecSepPos = i;
    }
    while(j < slen + 6)
    {
        sBuf[j++] = '0';
    }
    int iBufEnd = slen + 4;
    if(iDecSepPos > -1) iBufEnd = iDecSepPos + 4;
    sBuf[iBufEnd] = 0;
#ifdef __GNUC__
    sscanf(sBuf, "%I64d", &llRes);
#else
    sscanf(sBuf, "%lld", &llRes);
#endif
    free(sBuf);
    return llRes;
}

LONGLONG CurrencyFromBSTR(BSTR bsVal, char sDecSep)
{
    LONGLONG llRes = 0;
    int slen = wcslen(bsVal);
    LPSTR sBuf = (LPSTR)malloc((slen + 6)*sizeof(char));
    int j = 0;
    int iDecSepPos = -1;
    for(int i = 0; i < slen; i++)
    {
        if((bsVal[i] >= '0') && (bsVal[i] <= '9')) sBuf[j++] = bsVal[i];
        else if((bsVal[i] == sDecSep) && (iDecSepPos < 0)) iDecSepPos = i;
    }
    while(j < slen + 6)
    {
        sBuf[j++] = '0';
    }
    int iBufEnd = slen + 4;
    if(iDecSepPos > -1) iBufEnd = iDecSepPos + 4;
    sBuf[iBufEnd] = 0;
#ifdef __GNUC__
    sscanf(sBuf, "%I64d", &llRes);
#else
    sscanf(sBuf, "%lld", &llRes);
#endif
    free(sBuf);
    return llRes;
}

void GetSingleValue(PGresult *res, long lrow, int icol, Oid piType,
    VARIANT *pvVal, char sDecSep)
{
    PgVariantClear(pvVal);
    pvVal->vt = VT_NULL;
    if(PQgetisnull(res, lrow, icol) > 0) return;

    LPSTR psVal = PQgetvalue(res, lrow, icol);
    long lsize = PQgetlength(res, lrow, icol);
    SAFEARRAYBOUND sab;
    LPSTR sBuf;
    float f;

    switch(piType)
    {
    case BOOLOID:
        pvVal->vt = VT_BOOL;
        if((*psVal == 't') || (*psVal == 'T')) pvVal->boolVal = VARIANT_TRUE;
        else pvVal->boolVal = VARIANT_FALSE;
        break;
    case BYTEAOID:
        sab.cElements = lsize;
        sab.lLbound = 0;
        pvVal->vt = VT_ARRAY | VT_UI1;
        pvVal->parray = SafeArrayCreate(VT_UI1, 1, &sab);
        SafeArrayAccessData(pvVal->parray, (void**)&sBuf);
        memcpy(sBuf, psVal, lsize);
        SafeArrayUnaccessData(pvVal->parray);
        break;
    case NAMEOID:
        pvVal->vt = VT_LPSTR;
        pvVal->pcVal = (LPSTR)malloc((lsize + 1)*sizeof(char));
        strncpy(pvVal->pcVal, psVal, lsize);
        pvVal->pcVal[lsize] = 0;
        break;
    case CHAROID:
        pvVal->vt = VT_UI1;
        pvVal->bVal = *psVal;
        break;
    case INT2OID:
        pvVal->vt = VT_I2;
        sscanf(psVal, "%hd", &pvVal->iVal);
        break;
    case INT4OID:
    case OIDOID:
        pvVal->vt = VT_I4;
        sscanf(psVal, "%ld", &pvVal->lVal);
        break;
    case FLOAT4OID:
        pvVal->vt = VT_R4;
        sscanf(psVal, "%f", &pvVal->fltVal);
        break;
    case FLOAT8OID:
        pvVal->vt = VT_R8;
        sscanf(psVal, "%f", &f);
        pvVal->dblVal = f;
        break;
    case VARCHAROID:
    case TEXTOID:
    case UUIDOID:
        pvVal->vt = VT_LPSTR;
        pvVal->pcVal = (LPSTR)malloc(lsize + 1);
        strncpy(pvVal->pcVal, psVal, lsize);
        pvVal->pcVal[lsize] = 0;
        break;
    case NUMERICOID:
        pvVal->vt = VT_R8;
        sscanf(psVal, "%f", &f);
        pvVal->dblVal = f;
        break;
    case INT8OID:
        pvVal->vt = VT_I4;
        sscanf(psVal, "%ld", &pvVal->lVal);
        break;
    case CASHOID:
        pvVal->vt = VT_CY;
        pvVal->cyVal.int64 = CurrencyFromDBString(psVal, sDecSep);
        break;
#if DBGLEVEL > 2
    default:
        WriteLogFile("GetSingleValue\r\n  FieldName: ", true);
        WriteLogFile(PQfname(res, icol), false);
        WriteLogFile("\r\n  FieldType: ", false);
        char buf[32];
        sprintf(buf, "%d\r\n", piType);
        WriteLogFile(buf, false);
#endif // DBGLEVEL
    }
    return;
}

/*UINT GetCodePage(long lEnc)
{
    switch(lEnc)
    {
    case 6:
        return(CP_UTF8);
    default:
        return(CP_ACP);
    }
}*/

BSTR DBStrToBSTR(LPCSTR sStr, long lEnc)
{
    /*int ilen;
    BSTR bsRes = NULL;
    switch(lEnc)
    {
    case 6:
        ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, -1, NULL, 0);
        if(ilen > 0)
        {
            bsRes = SysAllocStringLen(NULL, ilen);
            ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, -1, bsRes, ilen + 1);
            bsRes[ilen - 1] = 0;
        }
        break;
    }*/

    LPWSTR wsStr = DBStrToWChar(sStr, lEnc);
    BSTR bsRes = SysAllocString(wsStr);
    free(wsStr);
    return(bsRes);
}

BSTR DBStrToBSTRLen(LPCSTR sStr, long lsize, long lEnc)
{
    int ilen;
    BSTR bsRes = NULL;
    switch(lEnc)
    {
    case 6:
        ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, lsize, NULL, 0);
        if(ilen > 0)
        {
            bsRes = SysAllocStringLen(NULL, ilen);
            MultiByteToWideChar(CP_UTF8, 0, sStr, lsize, bsRes, ilen + 1);
            bsRes[ilen] = 0;
        }
        break;
    }
    return(bsRes);
}

LPWSTR DBStrToWChar(LPCSTR sStr, long lEnc)
{
    int ilen;
    LPWSTR wsRes = NULL;
    switch(lEnc)
    {
    case 6:
        ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, -1, NULL, 0);
        if(ilen > 0)
        {
            wsRes = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
            MultiByteToWideChar(CP_UTF8, 0, sStr, -1, wsRes, ilen + 1);
            wsRes[ilen] = 0;
        }
        break;
    }
    return(wsRes);
}

void DBStrToWCharBuf(LPCSTR sStr, long lEnc, LPWSTR wsBuf, int iBufLen)
{
    int ilen;
    switch(lEnc)
    {
    case 6:
        ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, -1, NULL, 0);
        if(ilen > 0)
        {
            if(ilen > iBufLen - 1) ilen = iBufLen - 1;
            MultiByteToWideChar(CP_UTF8, 0, sStr, -1, wsBuf, ilen + 1);
            wsBuf[ilen] = 0;
        }
        break;
    }
}

void DBStrToVar(LPCSTR sStr, long lEnc, VARIANT *pvVal)
{
    pvVal->bstrVal = DBStrToBSTR(sStr, lEnc);
    if(pvVal->bstrVal) pvVal->vt = VT_BSTR;
    return;
}

void DBStrToVarLen(LPCSTR sStr, long lsize, long lEnc, VARIANT *pvVal)
{
    pvVal->bstrVal = DBStrToBSTRLen(sStr, lsize, lEnc);
    if(pvVal->bstrVal) pvVal->vt = VT_BSTR;
    return;
}

LPTSTR DBStrToTChar(LPCSTR sStr, long lEnc)
{
    int ilen;
    LPTSTR tsRes = NULL;
    switch(lEnc)
    {
    case 6:
        ilen = MultiByteToWideChar(CP_UTF8, 0, sStr, -1, NULL, 0);
        if(ilen > 0)
        {
#ifdef UNICODE
            tsRes = (LPWSTR)malloc(ilen*sizeof(wchar_t));
            MultiByteToWideChar(CP_UTF8, 0, sStr, -1, tsRes, ilen);
#else
            wsBuf = (LPWSTR)malloc(ilen*sizeof(wchar_t));
            MultiByteToWideChar(CP_UTF8, 0, sStr, -1, wsBuf, ilen);
            ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wsBuf, -1,
                NULL, 0, NULL, NULL);
            tsRes = (LPSTR)malloc(ilen*sizeof(char));
            WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wsBuf, -1,
                tsRes, ilen, NULL, NULL);
#endif
        }
        else
        {
            tsRes = (LPTSTR)malloc(sizeof(TCHAR));
            tsRes[0] = 0;
        }
        break;
    }
    return(tsRes);
}

int WCharDBLen(LPWSTR wsStr, long lEnc)
{
    int iRes = 0;
    switch(lEnc)
    {
    case 6:
        iRes = WideCharToMultiByte(CP_UTF8, 0, wsStr, wcslen(wsStr),
            NULL, 0, NULL, NULL);
        break;
    }
    return(iRes);
}

int WCharDBLenNull(LPWSTR wsStr, long lEnc)
{
    int iRes = 0;
    switch(lEnc)
    {
    case 6:
        iRes = WideCharToMultiByte(CP_UTF8, 0, wsStr, -1, NULL, 0, NULL, NULL);
        break;
    }
    return(iRes);
}

void WCharToDBStrBufNull(LPWSTR wsStr, long lEnc, LPSTR sBuf, int iBufLen)
{
    switch(lEnc)
    {
    case 6:
        WideCharToMultiByte(CP_UTF8, 0, wsStr, -1, sBuf, iBufLen, NULL, NULL);
        break;
    }
    return;
}

void WCharToDBStrBuf(LPWSTR wsStr, long lEnc, LPSTR sBuf, int iBufLen)
{
    switch(lEnc)
    {
    case 6:
        WideCharToMultiByte(CP_UTF8, 0, wsStr, wcslen(wsStr), sBuf,
            iBufLen + 1, NULL, NULL);
        sBuf[iBufLen] = 0;
        break;
    }
    return;
}

LPSTR WCharToDBStr(LPWSTR wsStr, long lEnc)
{
    LPSTR sRes = NULL;
    int iLen;
    switch(lEnc)
    {
    case 6:
        iLen = WideCharToMultiByte(CP_UTF8, 0, wsStr, -1, NULL, 0, NULL, NULL);
        sRes = (LPSTR)malloc(iLen*sizeof(char));
        WideCharToMultiByte(CP_UTF8, 0, wsStr, -1, sRes, iLen, NULL, NULL);
        break;
    }
    return(sRes);
}

LPSTR TCharToDBStr(LPTSTR tsStr, long lEnc)
{
    int ilen;
    LPWSTR wsBuf;
    LPSTR sRes = NULL;
    switch(lEnc)
    {
    case 6:
#ifdef UNICODE
        wsBuf = tsStr;
#else
        ilen = MultiByteToWideChar(CP_ACP, WC_COMPOSITECHECK, tsStr, -1,
            NULL, 0);
        wsBuf = (LPSTR)malloc(ilen*sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, WC_COMPOSITECHECK, tsStr, -1,
            wsBuf, ilen);
#endif
        ilen = WideCharToMultiByte(CP_UTF8, 0, wsBuf, -1, NULL, 0, NULL, NULL);
        if(ilen > 0)
        {
            sRes = (LPSTR)malloc(ilen*sizeof(char));
            WideCharToMultiByte(CP_UTF8, 0, wsBuf, -1, sRes, ilen, NULL, NULL);
        }
        else
        {
            sRes = (LPSTR)malloc(sizeof(char));
            sRes[0] = 0;
        }
#ifndef UNICODE
        free(wsBuf);
#endif
        break;
    }
    return(sRes);
}

LPSTR TCharToDBStrLen(LPTSTR tsStr, long lEnc, int iLen)
{
    int isize;
    LPWSTR wsBuf;
    LPSTR sRes = NULL;
    switch(lEnc)
    {
    case 6:
#ifdef UNICODE
        wsBuf = (LPWSTR)malloc((iLen + 1)*sizeof(wchar_t));
        wcsncpy(wsBuf, tsStr, iLen);
        wsBuf[iLen] = 0;
#else
        isize = MultiByteToWideChar(CP_ACP, WC_COMPOSITECHECK, iLen, -1,
            NULL, 0);
        wsBuf = (LPSTR)malloc((isize + 1)*sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, WC_COMPOSITECHECK, tsStr, iLen,
            wsBuf, isize + 1);
#endif
        isize = WideCharToMultiByte(CP_UTF8, 0, wsBuf, -1, NULL, 0, NULL, NULL);
        if(isize > 0)
        {
            sRes = (LPSTR)malloc(isize*sizeof(char));
            WideCharToMultiByte(CP_UTF8, 0, wsBuf, -1, sRes, isize, NULL, NULL);
        }
        else
        {
            sRes = (LPSTR)malloc(sizeof(char));
            sRes[0] = 0;
        }
        free(wsBuf);
        break;
    }
    return(sRes);
}

bool IsStandardDBNameMB(LPSTR sName)
{
    int iLen = strlen(sName);
    if(iLen < 1) return(false);

    bool bRes = (sName[0] >= 'a') && (sName[0] <= 'z');
    int i = 1;
    while(bRes && (i < iLen))
    {
        bRes = ((sName[i] >= 'a') && (sName[i] <= 'z'));
        bRes |= ((sName[i] >= '0') && (sName[i] <= '9'));
        bRes |= (sName[i] == '_');
        i++;
    }

    if(bRes)
    {
        if(strcmp(sName, "offset") == 0) bRes = false;
    }

    return(bRes);
}

bool IsStandardDBNameW(LPWSTR wsName)
{
    int iLen = wcslen(wsName);
    if(iLen < 1) return(false);

    bool bRes = (wsName[0] >= 'a') && (wsName[0] <= 'z');
    int i = 1;
    while(bRes && (i < iLen))
    {
        bRes = ((wsName[i] >= 'a') && (wsName[i] <= 'z'));
        bRes |= ((wsName[i] >= '0') && (wsName[i] <= '9'));
        bRes |= (wsName[i] == '_');
        i++;
    }

    if(bRes)
    {
        if(wcscmp(wsName, L"offset") == 0) bRes = false;
    }

    return(bRes);
}

short CharToInt(LPSTR sVal)
{
    char pnumbuf[2];
    pnumbuf[0] = sVal[1];
    pnumbuf[1] = sVal[0];
    return(*(short*)pnumbuf);
}

long CharToLong(LPSTR sVal)
{
    char pnumbuf[4];
    for(int i = 0; i < 4; i++) pnumbuf[i] = sVal[3 - i];
    return(*(long*)pnumbuf);
}

float CharToFloat(LPSTR sVal)
{
    char pnumbuf[4];
    for(int i = 0; i < 4; i++) pnumbuf[i] = sVal[3 - i];
    return(*(float*)pnumbuf);
}

double CharToDouble(LPSTR sVal)
{
    char pnumbuf[8];
    for(int i = 0; i < 8; i++) pnumbuf[i] = sVal[7 - i];
    return(*(double*)pnumbuf);
}

unsigned short GetNextNum(LPSTR *ppsVal, int *pslen)
{
    char sbuf[2];
    sbuf[0] = 0;
    sbuf[1] = 0;
    LPSTR pstr = *ppsVal;
    int slen = *pslen;
    if(slen > 0)
    {
        sbuf[1] = *pstr++;
        slen--;
    }
    if(slen > 0)
    {
        sbuf[0] = *pstr++;
        slen--;
    }
    *ppsVal = pstr;
    *pslen = slen;
    return(*(unsigned short*)sbuf);
}

double NumericToDouble(LPSTR psVal, int lsize)
{
    unsigned short ndigits = GetNextNum(&psVal, &lsize);
    short weight = (short)GetNextNum(&psVal, &lsize);
    unsigned short sign = GetNextNum(&psVal, &lsize);
    //short dscale = (short)GetNextNum(&psVal, &lsize);
    if(lsize < 1) return(0.0);

    unsigned short *pdisgits =
        (unsigned short*)malloc(ndigits*sizeof(unsigned short));
    for(int i = 0; i < ndigits; i++)
    {
        pdisgits[i] = GetNextNum(&psVal, &lsize);
    }
    int j = 0;
    double dRes = (double)pdisgits[j++];
    while(j < ndigits)
    {
        weight--;
        dRes *= 10000.0;
        dRes += (double)pdisgits[j++];
    }
    while(weight > 0)
    {
        weight--;
        dRes *= 10000.0;
    }
    while(weight < 0)
    {
        weight++;
        dRes /= 10000.0;
    }
    free(pdisgits);

    if(sign > 0) dRes *= -1.0;
    // actually sign = 0x4000 means negative, sign = 0xC000 means NaN
    // sign = 0xF000 means NULL
    return(dRes);
}

void EncodeDateTime(LPSTR sDest, double dDate)
{
    bool bNeg = (dDate < 0.0);
    long long lAbsDays = (long long)dDate;
    double dFrac = dDate - lAbsDays;
    lAbsDays -= 36526;
    double dNewDate = (double)lAbsDays;
    if(bNeg) dNewDate -= dFrac;
    else dNewDate += dFrac;
    long long lNewDate = (long long)(dNewDate*86400000000.0);
    ReverseBytes(sDest, (LPSTR)&lNewDate, 8);
    return;
}

void EncodeTime(LPSTR sDest, double dDate)
{
    bool bNeg = (dDate < 0.0);
    long long lAbsDays = (long long)dDate;
    double dFrac = dDate - lAbsDays;
    lAbsDays -= 36526;
    double dNewDate = (double)0.0;
    if(bNeg) dNewDate -= dFrac;
    else dNewDate += dFrac;
    long long lNewDate = (long long)(dNewDate*86400000000.0);
    ReverseBytes(sDest, (LPSTR)&lNewDate, 8);
    return;
}

double DecodeDateTime(LPSTR sSrc)
{
    long long mcSec;
    ReverseBytes((LPSTR)&mcSec, sSrc, 8);
    double dmcSec = (double)mcSec;
    bool bNeg = (dmcSec < 0.0);
    if(bNeg) dmcSec *= -1.0;
    double dDays = dmcSec/86400000000.0;
    long long lAbsDays = (long long)dDays;
    double dDate = lAbsDays;
    if(bNeg) dDate *= -1.0;
    dDate += 36526.0;
    if(bNeg && (dDate > 0.0)) dDate -= (dDays - lAbsDays);
    else
    {
        if(bNeg) dDate -= 2.0;
        dDate += (dDays - lAbsDays);
    }
    return(dDate);
}

void EncodeDate(LPSTR sDest, double dDate)
{
    long long lAbsDays = (long long)dDate;
    lAbsDays -= 36526;
    long lNewDate = (long)lAbsDays;
    ReverseBytes(sDest, (LPSTR)&lNewDate, 4);
    return;
}

double DecodeDate(LPSTR sSrc)
{
    long lDays;
    ReverseBytes((LPSTR)&lDays, sSrc, 4);
    double dDate = lDays;
    dDate += 36526.0;
    return(dDate);
}

void UUIDBufToString(LPSTR sBuf, LPWSTR wsBuf)
{
    unsigned char *psBuf = (unsigned char *)sBuf;
    swprintf(wsBuf, L"%.2x%.2x%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x",
        psBuf[0], psBuf[1], psBuf[2], psBuf[3], psBuf[4], psBuf[5],
        psBuf[6], psBuf[7], psBuf[8], psBuf[9], psBuf[10], psBuf[11], psBuf[12],
        psBuf[13], psBuf[14], psBuf[15]);
    return;
}

void GetSingleValueBin(PGresult *res, long lrow, int icol, Oid piType,
    VARIANT *pvVal, long lEnc, Oid lGeomType, int iGeomDim, int iCurrDigits)
{
#if DBGLEVEL > 2
    char buf[32];
#endif
    VariantClear(pvVal);
    pvVal->vt = VT_NULL;
    if(PQgetisnull(res, lrow, icol) > 0) return;

    LPSTR psVal = PQgetvalue(res, lrow, icol);
    long lsize = PQgetlength(res, lrow, icol);
    SAFEARRAYBOUND sab;
    unsigned char *sBuf = NULL;
    int ilen;
    short *pshbuf;
    long lDim;
    LPSTR psBuf;
    long long llVal;
    wchar_t wsllBuf[32];
//char tmpbuf[32];

    if((piType == lGeomType) && (iGeomDim > 0))
    {
/*for(int i = 0; i < lsize; i++)
{
sprintf(tmpbuf, "%.2X", (unsigned char)psVal[i]);
WriteLogFile(tmpbuf, false);
}
WriteLogFile("\r\n", false);*/
        sab.cElements = EWKBtoGeomBlobSize((unsigned char*)psVal, lsize,
            iGeomDim);
        if(sab.cElements > 0)
        {
            sab.lLbound = 0;
            pvVal->vt = VT_ARRAY | VT_UI1;
            pvVal->parray = SafeArrayCreate(VT_UI1, 1, &sab);
            SafeArrayAccessData(pvVal->parray, (void**)&sBuf);
            EWKBtoGeomBlob((unsigned char*)psVal, lsize, iGeomDim, sBuf);
            SafeArrayUnaccessData(pvVal->parray);
        }
        else pvVal->vt = VT_NULL;
        return;
    }

    switch(piType)
    {
    case BOOLOID:
        pvVal->vt = VT_BOOL;
        if(*psVal) pvVal->boolVal = VARIANT_TRUE;
        else pvVal->boolVal = VARIANT_FALSE;
        break;
    case BYTEAOID:
        sab.cElements = lsize;
        sab.lLbound = 0;
        pvVal->vt = VT_ARRAY | VT_UI1;
        pvVal->parray = SafeArrayCreate(VT_UI1, 1, &sab);
        SafeArrayAccessData(pvVal->parray, (void**)&sBuf);
        memcpy(sBuf, psVal, lsize);
        SafeArrayUnaccessData(pvVal->parray);
        break;
    /*case NAMEOID:
        pvVal->vt = VT_LPSTR;
        //pvVal->pcVal = (LPSTR)malloc((lsize + 1)*sizeof(char));
        //strcpy(pvVal->pcVal, psVal);
        pvVal->pcVal = psVal;
        break;*/
    case CHAROID:
        pvVal->vt = VT_UI1;
        pvVal->bVal = *psVal;
        break;
    case INT2OID:
        pvVal->vt = VT_I2;
        ReverseBytes((LPSTR)&pvVal->iVal, psVal, 2);
        break;
    case INT2VECTOROID:
// this will print the array value
//sprintf(buf, "Array size: %d\r\n", lsize);
//WriteLogFile(buf, false);
//for(int i = 0; i < lsize; i++)
//{
//sprintf(buf, "%.2X", psVal[i]);
//WriteLogFile(buf, false);
//}
//WriteLogFile("\r\n", false);
        lDim = CharToLong(psVal);
        psBuf = psVal + 8;
        if((lDim == 1) // we cannot handle higher dimension yet
            && (CharToLong(psBuf) == INT2OID))
        {
            psBuf += 4;
            sab.cElements = CharToLong(psBuf);
            sab.lLbound = 0;
            pvVal->vt = VT_ARRAY | VT_I2;
            pvVal->parray = SafeArrayCreate(VT_I2, 1, &sab);
            SafeArrayAccessData(pvVal->parray, (void**)&pshbuf);

            psBuf += 8;
            for(unsigned int i = 0; i < sab.cElements; i++)
            {
                psBuf += 4;
                pshbuf[i] = CharToInt(psBuf);
                psBuf += 2;
            }
            SafeArrayUnaccessData(pvVal->parray);
        }
        break;
    case INT4OID:
    case OIDOID:
        pvVal->vt = VT_I4;
        ReverseBytes((LPSTR)&pvVal->lVal, psVal, 4);
        break;
    case FLOAT4OID:
        pvVal->vt = VT_R4;
        ReverseBytes((LPSTR)&pvVal->fltVal, psVal, 4);
        break;
    case FLOAT8OID:
        pvVal->vt = VT_R8;
        ReverseBytes((LPSTR)&pvVal->dblVal, psVal, 8);
        break;
    case BPCHAROID:
    case VARCHAROID:
    case TEXTOID:
        DBStrToVarLen(psVal, lsize, lEnc, pvVal);
        break;
    case NUMERICOID:
        pvVal->vt = VT_R8;
        pvVal->dblVal = NumericToDouble(psVal, lsize);
        break;
    case DATEOID:
        pvVal->vt = VT_DATE;
        pvVal->date = DecodeDate(psVal);
        break;
    case TIMESTAMPOID:
    case TIMESTAMPTZOID:
    case TIMEOID:
        pvVal->vt = VT_DATE;
        pvVal->date = DecodeDateTime(psVal);
        break;
    case INT8OID:
        //pvVal->vt = VT_I4;
        //ReverseBytes((LPSTR)&pvVal->lVal, &psVal[4], 4);
        ReverseBytes((LPSTR)&llVal, psVal, 8);
        swprintf(wsllBuf, L"%lld", llVal);
        pvVal->vt = VT_BSTR;
        pvVal->bstrVal = SysAllocString(wsllBuf);
        break;
    case UUIDOID:
        pvVal->vt = VT_BSTR;
        pvVal->bstrVal = SysAllocStringLen(NULL, 38);
        UUIDBufToString(psVal, pvVal->bstrVal);
        break;
    case CASHOID:
        pvVal->vt = VT_CY;
        ReverseBytes((LPSTR)&llVal, psVal, 8);
        ilen = 10000;
        while(iCurrDigits > 0)
        {
            ilen /= 10;
            iCurrDigits--;
        }
        pvVal->cyVal.int64 = llVal*ilen;
        break;
#if DBGLEVEL > 2
    default:
        WriteLogFile("GetSingleValueBin\r\n  FieldName: ", true);
        WriteLogFile(PQfname(res, icol), false);
        WriteLogFile("\r\n  FieldType: ", false);
        sprintf(buf, "%d\r\n", piType);
        WriteLogFile(buf, false);
        WriteLogFile("  FieldSize: ", false);
        sprintf(buf, "%d\r\n", lsize);
        WriteLogFile(buf, false);
#endif // DBGLEVEL
    }
    return;
}

void GetRecord(PGresult *res, long lrow, int ncols, Oid *piTypes, VARIANT *pvRow, char sDecSep)
{
    for(int i = 0; i < ncols; i++)
    {
        GetSingleValue(res, lrow, i, piTypes[i], &pvRow[i], sDecSep);
    }
    return;
}

void GetRecordBin(PGresult *res, long lrow, int ncols, Oid *piTypes,
    VARIANT *pvRow, long lEnc, Oid lGeomType, int *piGeomDims, int iCurrDigits)
{
    int iGeomDim;
    for(int i = 0; i < ncols; i++)
    {
        iGeomDim = 0;
        if(piGeomDims) iGeomDim = piGeomDims[i];
        GetSingleValueBin(res, lrow, i, piTypes[i], &pvRow[i], lEnc, lGeomType,
            iGeomDim, iCurrDigits);
    }
    return;
}

void GetRecordCtid(PGresult *res, long lrow, int ncols, int iCtidPos,
    Oid *piTypes, VARIANT *pvRow, long lEnc, LPSTR pCtidBuf, int *piCtidLen,
    Oid lGeomType, int *piGeomDims, int iCurrDigits)
{
    if(PQgetisnull(res, lrow, iCtidPos))
    {
        *piCtidLen = 0;
    }
    else
    {
        *piCtidLen = PQgetlength(res, lrow, iCtidPos);
        LPSTR psVal = PQgetvalue(res, lrow, iCtidPos);
        memcpy(pCtidBuf, psVal, *piCtidLen);
        pCtidBuf[*piCtidLen] = 0;
    }
    int iGeomDim;
    int j = 0;
    for(int i = 0; i < ncols; i++)
    {
        if(i != iCtidPos)
        {
            iGeomDim = 0;
            if(piGeomDims) iGeomDim = piGeomDims[j++];
            GetSingleValueBin(res, lrow, i, piTypes[i], &pvRow[i - 1], lEnc,
                lGeomType, iGeomDim, iCurrDigits);
        }
    }
    return;
}

void WritePQErrorToLog(LPCSTR sAddMsg, PGconn *pConn)
{
#ifdef DBGLEVEL
    WriteLogFile(sAddMsg, true);
    WriteLogFile("PQ Error:\r\n  ", false);
    LPSTR sErrMsg = PQerrorMessage(pConn);
    char sBuf[512];
    int ilen;
    LPSTR sStart = sErrMsg;
    LPSTR sEnd = strchr(sStart, '\n');
    while(sEnd)
    {
        ilen = sEnd - sStart;
        if(ilen > 255) ilen = 255;
        strncpy(sBuf, sStart, ilen);
        sBuf[ilen] = 0;
        WriteLogFile(sBuf, false);
        WriteLogFile("\r\n  ", false);
        sStart = sEnd + 1;
        sEnd = strchr(sStart, '\n');
    }
    WriteLogFile(sStart, false);
    WriteLogFile("\r\n", false);
#endif
    return;
}

void BoolToOidVar(bool bVal, Oid iType, VARIANT *pvVal)
{
    switch(iType)
    {
    case BOOLOID:
        pvVal->vt = VT_BOOL;
        if(bVal) pvVal->boolVal = VARIANT_TRUE;
        else pvVal->boolVal = VARIANT_FALSE;
        break;
    case CHAROID:
        pvVal->vt = VT_UI1;
        pvVal->bVal = (unsigned char)bVal;
        break;
    case INT2OID:
        pvVal->vt = VT_I2;
        pvVal->iVal = (short)bVal;
        break;
    case INT4OID:
    case OIDOID:
        pvVal->vt = VT_I4;
        pvVal->lVal = (long)bVal;
        break;
    case FLOAT4OID:
        pvVal->vt = VT_R4;
        pvVal->fltVal = (float)bVal;
        break;
    case FLOAT8OID:
        pvVal->vt = VT_R8;
        pvVal->dblVal = (double)bVal;
        break;
    default:
        pvVal->vt = VT_NULL;
    }
    return;
}

void IntToOidVar(long lVal, Oid iType, VARIANT *pvVal)
{
    switch(iType)
    {
    case BOOLOID:
        pvVal->vt = VT_BOOL;
        if(lVal) pvVal->boolVal = VARIANT_TRUE;
        else pvVal->boolVal = VARIANT_FALSE;
        break;
    case CHAROID:
        pvVal->vt = VT_UI1;
        pvVal->bVal = (unsigned char)lVal;
        break;
    case INT2OID:
        pvVal->vt = VT_I2;
        pvVal->iVal = (short)lVal;
        break;
    case INT4OID:
    case OIDOID:
        pvVal->vt = VT_I4;
        pvVal->lVal = lVal;
        break;
    case FLOAT4OID:
        pvVal->vt = VT_R4;
        pvVal->fltVal = (float)lVal;
        break;
    case FLOAT8OID:
        pvVal->vt = VT_R8;
        pvVal->dblVal = (double)lVal;
        break;
    default:
        pvVal->vt = VT_NULL;
    }
    return;
}

void FloatToOidVar(double dblVal, Oid iType, VARIANT *pvVal)
{
    switch(iType)
    {
    case BOOLOID:
        pvVal->vt = VT_BOOL;
        if(dblVal > 0.0) pvVal->boolVal = VARIANT_TRUE;
        else pvVal->boolVal = VARIANT_FALSE;
        break;
    case CHAROID:
        pvVal->vt = VT_UI1;
        pvVal->bVal = (unsigned char)dblVal;
        break;
    case INT2OID:
        pvVal->vt = VT_I2;
        pvVal->iVal = (short)dblVal;
        break;
    case INT4OID:
    case OIDOID:
        pvVal->vt = VT_I4;
        pvVal->lVal = (long)dblVal;
        break;
    case FLOAT4OID:
        pvVal->vt = VT_R4;
        pvVal->fltVal = (float)dblVal;
        break;
    case FLOAT8OID:
        pvVal->vt = VT_R8;
        pvVal->dblVal = dblVal;
        break;
    default:
        pvVal->vt = VT_NULL;
    }
    return;
}

void ReverseBytes(LPSTR sDest, LPSTR sSrc, int iCnt)
{
    for(int i = 0; i < iCnt; i++) sDest[i] = sSrc[iCnt - i - 1];
    return;
}

/*Oid VarTypeToOid(VARIANT vVal)
{
    switch(vVal.vt)
    {
    case VT_BOOL:
        return(BOOLOID);
    case VT_I1:
    case VT_UI1:
    case VT_I2:
    case VT_UI2:
        return(INT2OID);
    case VT_I4:
    case VT_UI4:
        return(INT4OID);
    case VT_I8:
    case VT_UI8:
        return(INT8OID);
    case VT_R4:
        return(FLOAT4OID);
    case VT_R8:
        return(FLOAT8OID);
    case VT_BSTR:
    case VT_BSTR | VT_BYREF:
        return(VARCHAROID);
    default:
        return(0);
    }
}*/

int GetI1TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case INT2OID:
        return(2);
    case INT4OID:
    case FLOAT4OID:
        return(4);
    case INT8OID:
    case FLOAT8OID:
        return(8);
    default:
        return(1);
    }
}

int GetI2TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        return(1);
    case INT4OID:
    case FLOAT4OID:
        return(4);
    case INT8OID:
    case FLOAT8OID:
        return(8);
    default:
        return(2);
    }
}

int GetI4TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        return(1);
    case INT2OID:
        return(2);
    case INT8OID:
    case FLOAT8OID:
        return(8);
    default:
        return(4);
    }
}

int GetI8TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        return(1);
    case INT2OID:
        return(2);
    case INT4OID:
    case FLOAT4OID:
        return(4);
    default:
        return(8);
    }
}

int GetR4TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        return(1);
    case INT2OID:
        return(2);
    case INT8OID:
    case FLOAT8OID:
        return(8);
    default:
        return(4);
    }
}

int GetR8TypeLen(Oid lTargetType)
{
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        return(1);
    case INT2OID:
        return(2);
    case INT4OID:
    case FLOAT4OID:
        return(4);
    default:
        return(8);
    }
}

int VarToBinaryLen(VARIANT vVal, Oid lTargetType, long lEnc, PGeomInfo pgInfo,
    long *plParams)
{
#if DBGLEVEL > 2
    char buf[64];
#endif
    int iRes = 0;

    long lL, lU;
    unsigned char *pucBuf;
    long lErr;

    if(vVal.vt == (VT_ARRAY | VT_UI1))
    {
        SafeArrayGetLBound(vVal.parray, 1, &lL);
        SafeArrayGetUBound(vVal.parray, 1, &lU);

        if((pgInfo->iGdoType == gdbSpatial) &&
            !((pgInfo->lSubType == gdbGraphicsText) ||
            (pgInfo->lSubType == gdbCoverage)))
        {
            SafeArrayAccessData(vVal.parray, (void**)&pucBuf);
            iRes = GeomBlobtoEWKBLen(pucBuf, pgInfo->iGeomDim, pgInfo->ulSrid,
                pgInfo->iPsgType, plParams, &lErr);
            SafeArrayUnaccessData(vVal.parray);
            return(iRes);
        }

        iRes = lU - lL + 1;
        return(iRes);
    }

    switch(vVal.vt)
    {
    case VT_EMPTY:
    case VT_NULL:
        break;
    case VT_I1:
    case VT_UI1:
        iRes = GetI1TypeLen(lTargetType);
        break;
    case VT_I2:
    case VT_UI2:
        iRes = GetI2TypeLen(lTargetType);
        break;
    case VT_I4:
    case VT_UI4:
        iRes = GetI4TypeLen(lTargetType);;
        break;
    case VT_R4:
        iRes = GetR4TypeLen(lTargetType);;
        break;
    case VT_I8:
    case VT_UI8:
        iRes = GetI8TypeLen(lTargetType);;
        break;
    case VT_R8:
        iRes = GetR8TypeLen(lTargetType);;
        break;
    case VT_DATE:
        iRes = 8;
        break;
    case VT_BOOL:
        iRes = 1;
        break;
    case VT_LPSTR:
        iRes = strlen(vVal.pcVal);
        break;
    case VT_BSTR:
        if(lTargetType == UUIDOID) iRes = 16;
        else if(lTargetType == INT8OID) iRes = 8;
        else iRes = WCharDBLen(vVal.bstrVal, lEnc);
        break;
    case VT_BSTR | VT_BYREF:
        if(lTargetType == UUIDOID) iRes = 16;
        else if(lTargetType == INT8OID) iRes = 8;
        else iRes = WCharDBLen(*vVal.pbstrVal, lEnc);
        break;
    case VT_CY:
        iRes = 8;
        break;
#if DBGLEVEL > 2
    default:
        sprintf(buf, "VarToBinaryLen - type: %d\r\n", vVal.vt);
        WriteLogFile(buf, true);
#endif
    }

    return(iRes);
}

void I1TypeToBuf(char cVal, Oid lTargetType, LPSTR sBuf)
{
    short iVal;
    long lVal;
    long long llVal;
    float fltVal;
    double dblVal;
    switch(lTargetType)
    {
    case INT2OID:
        iVal = (short)cVal;
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
        break;
    case INT4OID:
        lVal = (long)cVal;
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
        break;
    case FLOAT4OID:
        fltVal = (float)cVal;
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
        break;
    case INT8OID:
        llVal = (long long)cVal;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
    case FLOAT8OID:
        dblVal = (double)cVal;
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
        break;
    default:
        sBuf[0] = cVal;
        sBuf[1] = 0;
    }
    return;
}

void I2TypeToBuf(short iVal, Oid lTargetType, LPSTR sBuf)
{
    char cVal;
    long lVal;
    long long llVal;
    float fltVal;
    double dblVal;
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        cVal = (char)iVal;
        sBuf[0] = cVal;
        sBuf[1] = 0;
        break;
    case INT4OID:
        lVal = (long)iVal;
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
        break;
    case FLOAT4OID:
        fltVal = (float)iVal;
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
        break;
    case INT8OID:
        llVal = (long long)iVal;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
    case FLOAT8OID:
        dblVal = (double)iVal;
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
        break;
    default:
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
    }
    return;
}

void I4TypeToBuf(long lVal, Oid lTargetType, LPSTR sBuf)
{
    char cVal;
    short iVal;
    long long llVal;
    float fltVal;
    double dblVal;
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        cVal = (char)lVal;
        sBuf[0] = cVal;
        sBuf[1] = 0;
        break;
    case INT2OID:
        iVal = (short)lVal;
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
        break;
    case FLOAT4OID:
        fltVal = (float)lVal;
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
        break;
    case INT8OID:
        llVal = (long long)lVal;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
    case FLOAT8OID:
        dblVal = (double)lVal;
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
        break;
    default:
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
    }
    return;
}

void I8TypeToBuf(long long llVal, Oid lTargetType, LPSTR sBuf)
{
    char cVal;
    short iVal;
    long lVal;
    float fltVal;
    double dblVal;
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        cVal = (char)llVal;
        sBuf[0] = cVal;
        sBuf[1] = 0;
        break;
    case INT2OID:
        iVal = (short)llVal;
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
        break;
    case INT4OID:
        lVal = (long)llVal;
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
        break;
    case FLOAT4OID:
        fltVal = (float)llVal;
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
        break;
    case FLOAT8OID:
        dblVal = (double)llVal;
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
        break;
    default:
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
    }
    return;
}

void R4TypeToBuf(float fltVal, Oid lTargetType, LPSTR sBuf)
{
    char cVal;
    short iVal;
    long lVal;
    long long llVal;
    double dblVal;
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        cVal = (char)(long)fltVal;
        sBuf[0] = cVal;
        sBuf[1] = 0;
        break;
    case INT2OID:
        iVal = (short)fltVal;
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
        break;
    case INT4OID:
        lVal = (long)fltVal;
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
        break;
    case INT8OID:
        llVal = (long long)fltVal;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
    case FLOAT8OID:
        dblVal = (double)fltVal;
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
        break;
    default:
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
    }
    return;
}

void R8TypeToBuf(double dblVal, Oid lTargetType, LPSTR sBuf)
{
    char cVal;
    short iVal;
    long lVal;
    float fltVal;
    long long llVal;
    switch(lTargetType)
    {
    case CHAROID:
    case BOOLOID:
        cVal = (char)(long)dblVal;
        sBuf[0] = cVal;
        sBuf[1] = 0;
        break;
    case INT2OID:
        iVal = (short)dblVal;
        ReverseBytes(sBuf, (LPSTR)&iVal, 2);
        break;
    case INT4OID:
        lVal = (long)dblVal;
        ReverseBytes(sBuf, (LPSTR)&lVal, 4);
        break;
    case FLOAT4OID:
        fltVal = (double)dblVal;
        ReverseBytes(sBuf, (LPSTR)&fltVal, 4);
        break;
    case INT8OID:
        llVal = (long long)dblVal;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
    default:
        ReverseBytes(sBuf, (LPSTR)&dblVal, 8);
    }
    return;
}

void DateTypeToBuf(DATE date, Oid lTargetType, LPSTR sBuf)
{
    switch(lTargetType)
    {
    case TIMESTAMPOID:
    case TIMESTAMPTZOID:
        EncodeDateTime(sBuf, date);
        sBuf[8] = 0;
        break;
    case TIMEOID:
        EncodeTime(sBuf, date);
        sBuf[8] = 0;
        break;
    case DATEOID:
        EncodeDate(sBuf, date);
        sBuf[4] = 0;
        break;
    }
    return;
}

void WCharToDBUUID(LPWSTR wsUUID, LPSTR sBuf)
{
    LPWSTR wsStart = wsUUID;
    memset(sBuf, 0, 16);
    if(*wsStart == '{') wsStart++;
    if((*wsStart == 'g') || (*wsStart == 'G'))
    {
        while(*wsStart != '{') wsStart++;
    }
    int iBuf = 0;
    int iNum;
    wchar_t wsBuf[3];
    wsBuf[2] = 0;
    for(int i = 0; i < 4; i++)
    {
        wsBuf[0] = *wsStart++;
        wsBuf[1] = *wsStart++;
        swscanf(wsBuf, L"%x", &iNum);
        sBuf[iBuf++] = iNum;
    }
    if(*wsStart == '-') wsStart++;
    for(int i = 0; i < 2; i++)
    {
        wsBuf[0] = *wsStart++;
        wsBuf[1] = *wsStart++;
        swscanf(wsBuf, L"%x", &iNum);
        sBuf[iBuf++] = iNum;
    }
    if(*wsStart == '-') wsStart++;
    for(int i = 0; i < 2; i++)
    {
        wsBuf[0] = *wsStart++;
        wsBuf[1] = *wsStart++;
        swscanf(wsBuf, L"%x", &iNum);
        sBuf[iBuf++] = iNum;
    }
    if(*wsStart == '-') wsStart++;
    for(int i = 0; i < 2; i++)
    {
        wsBuf[0] = *wsStart++;
        wsBuf[1] = *wsStart++;
        swscanf(wsBuf, L"%x", &iNum);
        sBuf[iBuf++] = iNum;
    }
    if(*wsStart == '-') wsStart++;
    for(int i = 0; i < 6; i++)
    {
        wsBuf[0] = *wsStart++;
        wsBuf[1] = *wsStart++;
        swscanf(wsBuf, L"%x", &iNum);
        sBuf[iBuf++] = iNum;
    }
    return;
}

void WCharToI8(LPWSTR wsVal, LPSTR sBuf)
{
    long long llVal = 0;
    swscanf(wsVal, L"%lld", &llVal);
    ReverseBytes(sBuf, (LPSTR)&llVal, 8);
    return;
}

void VarToBinaryBuf(VARIANT vVal, Oid lTargetType, long lEnc, LPSTR sBuf,
    int iBufLen, PGeomInfo pgInfo, long *plParams, int iCurrDigits)
{
#if DBGLEVEL > 2
    char buf[64];
#endif
    sBuf[0] = 0;

    unsigned char *pucBuf;
    long lErr;
    LONGLONG llVal;
    int iDenom;

    if(vVal.vt == (VT_ARRAY | VT_UI1))
    {
        SafeArrayAccessData(vVal.parray, (void**)&pucBuf);

        if((pgInfo->iGdoType == gdbSpatial) &&
            !((pgInfo->lSubType == gdbGraphicsText) ||
            (pgInfo->lSubType == gdbCoverage)))
        {
            GeomBlobtoEWKB(pucBuf, pgInfo->iGeomDim, pgInfo->ulSrid,
                pgInfo->iPsgType, (unsigned char*)sBuf, plParams, &lErr);
            SafeArrayUnaccessData(vVal.parray);
            return;
        }

        memcpy(sBuf, pucBuf, iBufLen);
        SafeArrayUnaccessData(vVal.parray);
        sBuf[iBufLen] = 0;
        return;
    }

    switch(vVal.vt)
    {
    case VT_EMPTY:
    case VT_NULL:
        sBuf[0] = 0;
        break;
    case VT_I1:
        I1TypeToBuf(vVal.cVal, lTargetType, sBuf);
        break;
    case VT_UI1:
        I1TypeToBuf((char)vVal.bVal, lTargetType, sBuf);
        break;
    case VT_I2:
        I2TypeToBuf(vVal.iVal, lTargetType, sBuf);
        break;
    case VT_UI2:
        I2TypeToBuf((short)vVal.uiVal, lTargetType, sBuf);
        break;
    case VT_I4:
        I4TypeToBuf(vVal.lVal, lTargetType, sBuf);
        break;
    case VT_UI4:
        I4TypeToBuf((long)vVal.ulVal, lTargetType, sBuf);
        break;
    case VT_I8:
        I8TypeToBuf(vVal.llVal, lTargetType, sBuf);
        break;
    case VT_UI8:
        I8TypeToBuf((long long)vVal.ullVal, lTargetType, sBuf);
        break;
    case VT_R4:
        R4TypeToBuf(vVal.fltVal, lTargetType, sBuf);
        break;
    case VT_R8:
        R8TypeToBuf(vVal.dblVal, lTargetType, sBuf);
        break;
    case VT_DATE:
        DateTypeToBuf(vVal.date, lTargetType, sBuf);
        break;
    case VT_BOOL:
        if(vVal.boolVal) sBuf[0] = 1;
        else sBuf[0] = 0;
        sBuf[1] = 0;
        break;
    case VT_LPSTR:
        strcpy(sBuf, vVal.pcVal);
        break;
    case VT_BSTR:
        if(lTargetType == UUIDOID) WCharToDBUUID(vVal.bstrVal, sBuf);
        else if(lTargetType == INT8OID) WCharToI8(vVal.bstrVal, sBuf);
        else WCharToDBStrBuf(vVal.bstrVal, lEnc, sBuf, iBufLen);
        break;
    case VT_BSTR | VT_BYREF:
        if(lTargetType == UUIDOID) WCharToDBUUID(*vVal.pbstrVal, sBuf);
        else if(lTargetType == INT8OID) WCharToI8(*vVal.pbstrVal, sBuf);
        else WCharToDBStrBuf(*vVal.pbstrVal, lEnc, sBuf, iBufLen);
        break;
    case VT_CY:
        iDenom = 10000;
        while(iCurrDigits > 0)
        {
            iDenom /= 10;
            iCurrDigits--;
        }
        llVal = vVal.cyVal.int64/iDenom;
        ReverseBytes(sBuf, (LPSTR)&llVal, 8);
        break;
#if DBGLEVEL > 2
    default:
        sprintf(buf, "VarToBinaryBuf - type: %d\r\n", vVal.vt);
        WriteLogFile(buf, true);
#endif
    }
    return;
}

void InsOrUpdateTable(PGconn *pConn, LPCSTR sTblName, int iFldCount,
    LPCSTR *psFldNames, VARIANT *pvValues, int iKeyField, long lEnc, int iCurrDigits)
{
    Oid *piTypes = (Oid*)malloc(iFldCount*sizeof(Oid));
    LPSTR *psParams = (LPSTR*)malloc(iFldCount*sizeof(LPSTR));
    int *piParLengths = (int*)malloc(iFldCount*sizeof(int));
    int *piParForms = (int*)malloc(iFldCount*sizeof(int));

    CGeomInfo cgInfo = {0, 0, 0, 0, 0};
    long lParams[8];

    for(int i = 0; i < iFldCount; i++)
    {
        piTypes[i] = 0;
        piParLengths[i] = VarToBinaryLen(pvValues[i], 0, lEnc, &cgInfo, lParams);
        if(piParLengths[i] > 0)
        {
            psParams[i] = (LPSTR)malloc(piParLengths[i] + 1);
            VarToBinaryBuf(pvValues[i], 0, lEnc, psParams[i], piParLengths[i],
                &cgInfo, lParams, iCurrDigits);
        }
        else psParams[i] = NULL;
        piParForms[i] = 1;
    }

    int ilen = strlen("select * from  where  = $1") + strlen(sTblName) +
        strlen(psFldNames[iKeyField]);
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "select * from %s where %s = $1",
        sTblName, psFldNames[iKeyField]);
    PGresult *res = PQexecParams(pConn, sCmd, 1, &piTypes[iKeyField],
        &psParams[iKeyField], &piParLengths[iKeyField], piParForms, 1);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'InsOrUpdateTable' failed\r\n", pConn);
        PQclear(res);
        for(int i = 0; i < iFldCount; i++)
        {
            if(psParams[i]) free(psParams[i]);
        }
        free(piParForms);
        free(piParLengths);
        free(psParams);
        free(piTypes);
        return;
    }

    long nrows = PQntuples(res);

    char sNumBuf[32];
    ilen = 24 + strlen(sTblName);
    int iIdx;
    for(int i = 0; i < iFldCount; i++)
    {
        ilen += (strlen(psFldNames[i]) + 16);
        iIdx = PQfnumber(res, psFldNames[i]);
        if(iIdx > -1) piTypes[i] = PQftype(res, iIdx);
    }

    PQclear(res);

    sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));

    int iFirst = 0;
    if(iKeyField == iFirst) iFirst++;

    if(nrows > 0)
    {
        strcpy(sCmd, "update ");
        strcat(sCmd, sTblName);
        strcat(sCmd, " set ");
        if(iKeyField == 0)
        {
            strcat(sCmd, psFldNames[1]);
            strcat(sCmd, " = $2");
            for(int i = 2; i < iFldCount; i++)
            {
                strcat(sCmd, ", ");
                strcat(sCmd, psFldNames[i]);
                sprintf(sNumBuf, " = $%d", i + 1);
                strcat(sCmd, sNumBuf);
            }
        }
        else
        {
            strcat(sCmd, psFldNames[0]);
            strcat(sCmd, " = $1");
            for(int i = 1; i < iKeyField; i++)
            {
                strcat(sCmd, ", ");
                strcat(sCmd, psFldNames[i]);
                sprintf(sNumBuf, " = $%d", i + 1);
                strcat(sCmd, sNumBuf);
            }
            for(int i = iKeyField + 1; i < iFldCount; i++)
            {
                strcat(sCmd, ", ");
                strcat(sCmd, psFldNames[i]);
                sprintf(sNumBuf, " = $%d", i + 1);
                strcat(sCmd, sNumBuf);
            }
        }
        strcat(sCmd, " where ");
        strcat(sCmd, psFldNames[iKeyField]);
        sprintf(sNumBuf, " = $%d", iKeyField + 1);
        strcat(sCmd, sNumBuf);
    }
    else
    {
        strcpy(sCmd, "insert into ");
        strcat(sCmd, sTblName);
        strcat(sCmd, "(");
        strcat(sCmd, psFldNames[0]);
        for(int i = 1; i < iFldCount; i++)
        {
            strcat(sCmd, ", ");
            strcat(sCmd, psFldNames[i]);
        }
        strcat(sCmd, ") values ($1");
        for(int i = 1; i < iFldCount; i++)
        {
            sprintf(sNumBuf, ", $%d", i + 1);
            strcat(sCmd, sNumBuf);
        }
        strcat(sCmd, ")");
    }

    res = PQexecParams(pConn, sCmd, iFldCount, piTypes, psParams,
        piParLengths, piParForms, 1);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Update in 'InsOrUpdateTable' failed\r\n", pConn);
        PQclear(res);
        PQexec(pConn, "end");
    }
    PQclear(res);
    free(sCmd);

    for(int i = 0; i < iFldCount; i++)
    {
        if(psParams[i]) free(psParams[i]);
    }
    free(piParForms);
    free(piParLengths);
    free(psParams);
    free(piTypes);

    return;
}

void InsertTable(PGconn *pConn, LPSTR sTblName, int iFldCount,
    VARIANT *pvValues, long lEnc, int iCurrDigits)
{
    // "insert into  values()", iFldCount*", $x"
    int ilen = 32 + strlen(sTblName) + iFldCount*16;
    LPSTR sCmd = (LPSTR)malloc(ilen*sizeof(char));
    strcpy(sCmd, "insert into ");
    strcat(sCmd, sTblName);
    strcat(sCmd, " values($1");

    char sNumBuf[32];
    for(int i = 1; i < iFldCount; i++)
    {
        sprintf(sNumBuf, ", $%d", i + 1);
        strcat(sCmd, sNumBuf);
    }
    strcat(sCmd, ")");

    LPSTR *psParams = (LPSTR*)malloc(iFldCount*sizeof(LPSTR));
    int *piParLengths = (int*)malloc(iFldCount*sizeof(int));
    int *piParForms = (int*)malloc(iFldCount*sizeof(int));

    CGeomInfo cgInfo = {0, 0, 0, 0, 0};
    long lParams[8];

    for(int i = 0; i < iFldCount; i++)
    {
        piParLengths[i] = VarToBinaryLen(pvValues[i], 0, lEnc, &cgInfo, lParams);
        if(piParLengths[i] > 0)
        {
            psParams[i] = (LPSTR)malloc(piParLengths[i] + 1);
            VarToBinaryBuf(pvValues[i], 0, lEnc, psParams[i], piParLengths[i],
                &cgInfo, lParams, iCurrDigits);
        }
        else psParams[i] = NULL;
        piParForms[i] = 1;
    }

    PGresult *res = PQexecParams(pConn, sCmd, iFldCount, NULL, psParams,
        piParLengths, piParForms, 1);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Insert in 'InsertTable' failed\r\n", pConn);
        PQclear(res);
        PQexec(pConn, "end");
    }
    PQclear(res);
    free(sCmd);

    for(int i = 0; i < iFldCount; i++)
    {
        if(psParams[i]) free(psParams[i]);
    }
    free(piParForms);
    free(piParLengths);
    free(psParams);

    return;
}

Oid GetOid(PGconn *pConn, LPSTR sSchema, LPSTR sTable, bool bTrans, long lEnc)
{
    LPSTR psParams[2];
    char sSchemaBuf[8];
    strcpy(sSchemaBuf, "public");

    psParams[0] = sTable;
    if(sSchema) psParams[1] = sSchema;
    else psParams[1] = sSchemaBuf;

    SetSP(pConn, bTrans);
    /*PGresult *res = PQexecParams(pConn,
        "select oid from pg_class where relname = $1", 1, NULL, &sName,
        NULL, NULL, 1);*/

    PGresult *res = PQexecParams(pConn, "select t1.oid from pg_class t1 inner join pg_namespace t2 \
on t1.relnamespace = t2.oid where t1.relname = $1 and t2.nspname = $2", 2, NULL, psParams, NULL, NULL, 1);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Spot 13: ", pConn);
        PQclear(res);
        RollbackSP(pConn, bTrans);
        return(0);
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        ReleaseSP(pConn, bTrans);
        return(0);
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvVals = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvVals[i]);
    DescribeFieldTypes(res, ncols, piTypes);
    GetRecordBin(res, 0, ncols, piTypes, pvVals, lEnc, 0, NULL, 0);
    Oid lOid = VarToLong(pvVals[0]);
    for(int i = 0; i < ncols; i++) VariantClear(&pvVals[i]);
    free(pvVals);
    free(piTypes);

    PQclear(res);
    ReleaseSP(pConn, bTrans);

    return(lOid);
}

LPSTR GetSequenceName(LPSTR sSchema, LPSTR sTable, LPSTR sField)
{
    int ilen = strlen(sField) + strlen(sTable) + 12;
    if(sSchema) ilen += (strlen(sSchema) + 1);
    LPSTR sRes = (LPSTR)malloc(ilen*sizeof(char));

    if(sSchema)
    {
        if(IsStandardDBNameMB(sSchema))
        {
            if(IsStandardDBNameMB(sTable) && IsStandardDBNameMB(sField))
            {
                sprintf(sRes, "%s.%s_%s_seq", sSchema, sTable, sField);
            }
            else
            {
                sprintf(sRes, "%s.\"%s_%s_seq\"", sSchema, sTable, sField);
            }
        }
        else
        {
            if(IsStandardDBNameMB(sTable) && IsStandardDBNameMB(sField))
            {
                sprintf(sRes, "\"%s\".%s_%s_seq", sSchema, sTable, sField);
            }
            else
            {
                sprintf(sRes, "\"%s\".\"%s_%s_seq\"", sSchema, sTable, sField);
            }
        }
    }
    else
    {
        if(IsStandardDBNameMB(sTable) && IsStandardDBNameMB(sField))
        {
            sprintf(sRes, "%s_%s_seq", sTable, sField);
        }
        else
        {
            sprintf(sRes, "\"%s_%s_seq\"", sTable, sField);
        }
    }
    return(sRes);
}

void GetDefaultCoordinateSystem(PConnStruct pCSt, VARIANT *pvDefCS)
{
    VariantClear(pvDefCS);
    if(!pCSt->sGParameters[0]) return;

    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;
    SetSP(pCSt->pConn, bTrans);

    char sQuery[128];
    sprintf(sQuery,
        "select gvalue from %s where gparameter='DefaultCoordinateSystem'",
        pCSt->sGParameters);

    //LPSTR lsCmd = GetSaveSQL(sQuery, bTrans);
    PGresult *res = PQexec(pCSt->pConn, sQuery);
    //free(lsCmd);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Query DefaultCoordinateSystem failed\r\n", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        pvDefCS->vt = VT_NULL;
        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        return;
    }

    Oid iType;
    DescribeFieldTypes(res, 1, &iType);
    GetRecordBin(res, 0, 1, &iType, pvDefCS, pCSt->lDBEnc, 0, NULL, pCSt->iCurrDigits);
    PQclear(res);
    ReleaseSP(pCSt->pConn, bTrans);

    return;
}

bool SetDefaultCoordinateSystem(PConnStruct pCSt, VARIANT vDefCS,
    VARIANT *pvNewDefCS)
{
    if(!pCSt->sGParameters[0]) return(false);

    bool bTrans = pCSt->iSysTrans | pCSt->iGdoTrans;

    char sQuery[128];
    PGresult *res;

    VARIANT *pVal = &vDefCS;
    if(vDefCS.vt & VT_BYREF)
    {
        if(vDefCS.vt == (VT_BYREF | VT_VARIANT))
        {
            pVal = vDefCS.pvarVal;
        }
    }

    SetSP(pCSt->pConn, bTrans);

    if(pVal->vt != VT_BSTR)
    {
        sprintf(sQuery,
            "delete from %s where gparameter='DefaultCoordinateSystem'",
            pCSt->sGParameters);
        res = PQexec(pCSt->pConn, sQuery);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Clearing DefaultCoordinateSystem failed\r\n", pCSt->pConn);
            PQclear(res);
            RollbackSP(pCSt->pConn, bTrans);
            return(false);
        }

        PQclear(res);
        ReleaseSP(pCSt->pConn, bTrans);
        VariantClear(pvNewDefCS);
        pvNewDefCS->vt = VT_NULL;
        return(true);
    }

    LPSTR sParam = WCharToDBStr(pVal->bstrVal, pCSt->lDBEnc);

    sprintf(sQuery,
        "insert into %s values('DefaultCoordinateSystem', $1)",
        pCSt->sGParameters);

    res = PQexecParams(pCSt->pConn, sQuery, 1, NULL, &sParam, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        SetSP(pCSt->pConn, bTrans);

        sprintf(sQuery,
            "update %s set gvalue = $1 where gparameter='DefaultCoordinateSystem'",
            pCSt->sGParameters);

        res = PQexecParams(pCSt->pConn, sQuery, 1, NULL, &sParam, NULL, NULL, 0);
    }

    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Setting DefaultCoordinateSystem failed\r\n", pCSt->pConn);
        PQclear(res);
        RollbackSP(pCSt->pConn, bTrans);
        free(sParam);
        return(false);
    }

    PQclear(res);
    free(sParam);
    ReleaseSP(pCSt->pConn, bTrans);

    VariantCopy(pvNewDefCS, pVal);
    return(true);
}
