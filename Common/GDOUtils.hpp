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

#ifndef _GDOUTILS_HPP_
#define _GDOUTILS_HPP_

#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif
#include "PGSCoordSystems.hpp"

enum CConnStatus
{
    icsClosed, // the connection is closed
    icsNotEnoughPrivileges, // the user has no enough privileges to maintain the database
    icsNoPostGis, // looks like a plain PostgreSQL database
    icsNoMetadata, // no GAliasTable found
    icsStrangeMetadata, // the GAliasTable found but has strange structure or
        // contains suspicious records
    icsSimpleMetaIncomplete, // the GAliasTable exists but seems to contain
        // sparse data
    icsSimpleMetadata, // the GAliasTable only contains records for
        // CoordSystem table, modification log and modified tables
    icsINGRMetaIncomplete, // the GAliasTable contains correct simple
        // metadata and some sparse INGR metadata
    icsINGRMetadata // the GAliasTable contains therecords as simple
        // metadata plus INGRFeatures, INGRAttributeProperties,
        // INGRGeometryProperties, INGRFieldLookup and INGRPickList
};

typedef struct CConnStruct
{
    HINSTANCE hInstance;
    PGconn *pConn;
    CConnStatus iConnStat;
    LONG_PTR pErrors;
    LPWSTR wsServerName;
    long lSessionId;
    LONG_PTR ppTypeLibs;
    CCSArray cCSTable;
    int iSysTrans; // the internal transaction counter
    int iGdoTrans; // the counter of the transactions called by the client
    bool bGeomPrepared; // the name of the statement will be hardcode to "stmtgeom"
    bool bFtrPrepared; // the name of the statement will be hardcode to "stmtftr"
    bool bGraphicPrepared; // the name of the statement will be hardcode to "stmtgrph"
    char sGeomCols[64];
    char sSpatRefSys[64];
    char sGAlias[64];
    char sGFeatures[64];
    char sFieldLookup[64];
    char sAttributeProperties[64];
    char sGeometryProperties[64];
    char sOperatorsTable[64];
    char sCoordSystems[64];
    char sModTables[64];
    char sModLog[64];
    char sPickLists[64];
    char sGraphicColumns[64];
    char sGParameters[64];
    long lDBEnc;
    char sDBEncName[64];
    long lCursors;
    Oid lGeomOid;
    bool bSchemaUpdatable;
    bool bUpdatable;
    char sDecSep;
    int iCurrDigits;
} *PConnStruct;

typedef struct CGeomInfo
{
    int iGdoType;
    long lSubType;
    int iGeomDim;
    unsigned long ulSrid;
    int iPsgType;
} *PGeomInfo;

// If a transaction is active, all PQexec*, PQprepare* statements must be
// enclosed by SAVEPOINTS
void SetSP(PGconn *pConn, bool bTrans);
void RollbackSP(PGconn *pConn, bool bTrans);
void ReleaseSP(PGconn *pConn, bool bTrans);
// maybe in the future
//LPSTR GetSaveSQL(LPCSTR sCmd, bool bTrans);

//UINT GetCodePage(long lEnc);
BSTR DBStrToBSTR(LPCSTR sStr, long lEnc);
BSTR DBStrToBSTRLen(LPCSTR sStr, long lsize, long lEnc);
LPWSTR DBStrToWChar(LPCSTR sStr, long lEnc);
void DBStrToWCharBuf(LPCSTR sStr, long lEnc, LPWSTR wsBuf, int iBufLen);
void DBStrToVar(LPCSTR sStr, long lEnc, VARIANT *pvVal);
void DBStrToVarLen(LPCSTR sStr, long lsize, long lEnc, VARIANT *pvVal);
LPTSTR DBStrToTChar(LPCSTR sStr, long lEnc);

int WCharDBLen(LPWSTR wsStr, long lEnc);
int WCharDBLenNull(LPWSTR wsStr, long lEnc);
void WCharToDBStrBufNull(LPWSTR wsStr, long lEnc, LPSTR sBuf, int iBufLen);
void WCharToDBStrBuf(LPWSTR wsStr, long lEnc, LPSTR sBuf, int iBufLen);
LPSTR WCharToDBStr(LPWSTR wsStr, long lEnc);
LPSTR TCharToDBStr(LPTSTR tsStr, long lEnc);
LPSTR TCharToDBStrLen(LPTSTR tsStr, long lEnc, int iLen);

bool IsStandardDBNameMB(LPSTR sName);
bool IsStandardDBNameW(LPWSTR wsName);

bool DbuExecuteCommand(PConnStruct pCSt, LPCSTR sCmd, LPCSTR sDesc);
bool IsTableWritable(PGconn *pConn, LPCSTR sSchema, LPCSTR sTblName, bool bTrans);
int TableExists(PGconn *pConn, LPCSTR sTblName, bool bTrans);

// bFixCSRef is false for DBUtils. Ford GDO server, however, we are going to
// generate the CS table in memory, so if the record exists in GAliasTable,
// and it has no string value, let's fix it just at startup

// Furthermore, the method will attemt to create statement "stmttbls"
// to select from pg_tables
void InitConnStruct(PConnStruct pCSt, LPSTR sDBName, LPWSTR wsServerName,
    bool bFixCSRef);

long GetGeometryType(PConnStruct pCSt, LPSTR sSchemaName, LPSTR sTblName,
    LPSTR sFldName, int *piDim, unsigned long *pulSrid, int *piPsgType,
    LPWSTR sCSGuid);
long GetFieldId(PConnStruct pCSt, LPWSTR wsTblName, LPWSTR wsFldName,
    bool bInsert);
void CheckAttrMetaData(PConnStruct pCSt, LPWSTR wsFldName, LPWSTR wsTblName,
    bool *pbIsKey);
bool CheckGeomMetaData(PConnStruct pCSt, LPWSTR wsFldName, LPWSTR wsTblName,
    long *plSubType, LPWSTR sCSGuid);
void PgVariantClear(VARIANT *pV);
void DescribeFieldTypes(PGresult *res, int ncols, Oid *piTypes);
void GetRecord(PGresult *res, long lrow, int ncols, Oid *piTypes, VARIANT *pvRow,
    char sDecSep);
void GetRecordBin(PGresult *res, long lrow, int ncols, Oid *piTypes,
    VARIANT *pvRow, long lEnc, Oid lGeomType, int *piGeomDims, int iCurrDigits);
void GetRecordCtid(PGresult *res, long lrow, int ncols, int iCtidPos,
    Oid *piTypes, VARIANT *pvRow, long lEnc, LPSTR pCtidBuf, int *piCtidLen,
    Oid lGeomType, int *piGeomDims, int iCurrDigits);
void WritePQErrorToLog(LPCSTR sAddMsg, PGconn *pConn);

// some helper functions
void BoolToOidVar(bool bVal, Oid iType, VARIANT *pvVal);
void IntToOidVar(long lVal, Oid iType, VARIANT *pvVal);
void FloatToOidVar(double dblVal, Oid iType, VARIANT *pvVal);
void ReverseBytes(LPSTR sDest, LPSTR sSrc, int iCnt);

//Oid VarTypeToOid(VARIANT vVal);
// plParams is the caller allocated array of long values, which might be
// initializes by the length calculating routine and later passed into
// the VarToBinaryBuf functions. At the moment, the only usage is for
// the geometry values, which will carry four parameters. So it is fully
// sufficient and safe to supply an array of 8 longs
int VarToBinaryLen(VARIANT vVal, Oid lTargetType, long lEnc, PGeomInfo pgInfo,
    long *plParams);
// the actual buffer size must be iBufLen + 1 long;
void VarToBinaryBuf(VARIANT vVal, Oid lTargetType, long lEnc, LPSTR sBuf,
    int iBufLen, PGeomInfo pgInfo, long *plParams, int iCurrDigits);

void InsOrUpdateTable(PGconn *pConn, LPCSTR sTblName, int iFldCount,
    LPCSTR *psFldNames, VARIANT *pvValues, int iKeyField, long lEnc, int iCurrDigits);
void InsertTable(PGconn *pConn, LPSTR sTblName, int iFldCount,
    VARIANT *pvValues, long lEnc, int iCurrDigits);
Oid GetOid(PGconn *pConn, LPSTR sSchema, LPSTR sTable, bool bTrans, long lEnc);
LONGLONG CurrencyFromBSTR(BSTR bsVal, char sDecSep);

LPSTR GetSequenceName(LPSTR sSchema, LPSTR sTable, LPSTR sField);

void GetDefaultCoordinateSystem(PConnStruct pCSt, VARIANT *pvDefCS);
bool SetDefaultCoordinateSystem(PConnStruct pCSt, VARIANT vDefCS,
    VARIANT *pvNewDefCS);

#endif
