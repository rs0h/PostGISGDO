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

#include "PsgDBUTables.hpp"
#include "../Common/VarUtils.hpp"
#include "../Common/PGtoGDOmaps.hpp"
#include "../Common/LogFiles.hpp"
#include "../Common/pg_const.h"
#include "../Common/GDOtoEWKB.hpp"

long GetFieldIdT(PConnStruct pCSt, LPTSTR tsTblName, LPTSTR tsFldName,
    bool bInsert)
{
#ifdef UNICODE
    return(GetFieldId(pCSt, tsTblName, tsFldName, bInsert));
#else
    LPWSTR wsTblName = CharToWChar(tsTblName);
    LPWSTR wsFldName = CharToWChar(tsFldName);
    long lRes = GetFieldId(pCSt, wsTblName, wsFldName, bInsert);
    free(wsFldName);
    free(wsTblName);
    return(lRes);
#endif
}

// CDbuField

CDbuField::CDbuField(PConnStruct pConnStruct, LPSTR sName, Oid lPgType,
    int iSize, long lMod, bool bNullable, bool bHasDefault, LPSTR sOrigSchema,
    LPSTR sOrigTable, LPTSTR tsTblName, Oid lTblOid, int iColPos)
{
    m_pConnStruct = pConnStruct;

    m_sOrigSchema = sOrigSchema;
    m_sOrigTable = sOrigTable;
    m_tsTblName = tsTblName;

    int iLen = strlen(sName);
    m_sOrigName = (LPSTR)malloc((iLen + 1)*sizeof(char));
    strcpy(m_sOrigName, sName);

    m_tsName = DBStrToTChar(sName, pConnStruct->lDBEnc);

    m_iType = OidToGdoType(lPgType, iSize, lMod, &m_lSize);
    m_lAttr = 0;
    m_lSubType = 0;
    m_iDateType = 0;
    if(m_iType == gdbDate)
    {
        switch(lPgType)
        {
        case TIMESTAMPOID:
        case TIMESTAMPTZOID:
            m_iDateType = 1;
            break;
        case TIMEOID:
        case TIMETZOID:
            m_iDateType = 2;
            break;
        case DATEOID:
            m_iDateType = 3;
            break;
        }
    }
    m_bHasDefault = bHasDefault;
    m_bRequired = !bNullable;
    m_lTblOid = lTblOid;
    m_iColPos = iColPos;
    m_bVisible = true;
    m_bHasChanged = false;
    m_bIsKey = false;
    m_tsDescription[0] = 0;
    m_sDefault = NULL;
    m_bHasSpecField = false;
}

CDbuField::~CDbuField()
{
    if(m_sDefault) free(m_sDefault);
    if(m_tsName) free(m_tsName);
    if(m_sOrigName) free(m_sOrigName);
}

void CDbuField::CheckDefault(LPSTR sSchema, LPSTR sTblName, LPSTR sFldName)
{
    if(!m_bHasDefault) return;

    char sParams[32];
    sprintf(sParams, "%d", m_lTblOid);
    sprintf(&sParams[16], "%d", m_iColPos);
    LPSTR pParams[2];
    pParams[0] = sParams;
    pParams[1] = &sParams[16];
    PGresult *res = PQexecParams(m_pConnStruct->pConn,
        "select adsrc from pg_attrdef where adrelid = $1 and adnum = $2", 2,
        NULL, pParams, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select pg_attrdef in 'CDbuField::CheckDefault' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long lrows = PQntuples(res);
    if(lrows < 1)
    {
        PQclear(res);
        return;
    }

    if(PQgetisnull(res, 0, 0))
    {
        PQclear(res);
        return;
    }

    LPSTR sRes = PQgetvalue(res, 0, 0);
    int slen = PQgetlength(res, 0, 0);

    if(m_sDefault) free(m_sDefault);
    m_sDefault = (LPSTR)malloc((slen + 1)*sizeof(char));
    strncpy(m_sDefault, sRes, slen);
    m_sDefault[slen] = 0;

    PQclear(res);

    if(strncmp(m_sDefault, "nextval", 7) == 0)
    {
        LPSTR sFirsQuote = strchr(m_sDefault, '\'');
        if(sFirsQuote)
        {
            LPSTR sSeqName = GetSequenceName(sSchema, sTblName, sFldName);
            sFirsQuote++;
            if(strncmp(sFirsQuote, sSeqName, strlen(sSeqName)) == 0)
            {
                m_lAttr |= 16;
            }
            free(sSeqName);
        }
    }
    return;
}

void CDbuField::CheckAttrMeta(LPTSTR tsTblName, LPTSTR tsFldName)
{
    int ilen = strlen("select t1.* from  t1 \
inner join  t2 on t1.indexid = t2.indexid \
where t2.featurename = $1 and t2.fieldname = $2") +
        strlen(m_pConnStruct->sAttributeProperties) +
        strlen(m_pConnStruct->sFieldLookup);
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "select t1.* from %s t1 \
inner join %s t2 on t1.indexid = t2.indexid \
where t2.featurename = $1 and t2.fieldname = $2",
        m_pConnStruct->sAttributeProperties, m_pConnStruct->sFieldLookup);

    LPSTR psParams[2];
    psParams[0] = TCharToDBStr(tsTblName, m_pConnStruct->lDBEnc);
    psParams[1] = TCharToDBStr(tsFldName, m_pConnStruct->lDBEnc);
    PGresult *res = PQexecParams(m_pConnStruct->pConn, sCmd, 2, NULL, psParams,
        NULL, NULL, 1);
    free(psParams[1]);
    free(psParams[0]);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'CDbuField::CheckAttrMeta' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long lrows = PQntuples(res);
    if(lrows < 1)
    {
        PQclear(res);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);
    DescribeFieldTypes(res, ncols, piTypes);
    GetRecordBin(res, 0, ncols, piTypes, pvRow, m_pConnStruct->lDBEnc,
        m_pConnStruct->lGeomOid, NULL, m_pConnStruct->iCurrDigits);

    int iPos = PQfnumber(res, "fielddescription");
    if(iPos > -1)
    {
        VarToTCharBuf(pvRow[iPos], m_tsDescription, 256);
    }
    iPos = PQfnumber(res, "iskeyfield");
    if(iPos > -1) m_bIsKey = VarToBool(pvRow[iPos]);
    iPos = PQfnumber(res, "isfielddisplayable");
    if(iPos > -1) m_bVisible = VarToBool(pvRow[iPos]);

    if((m_iType == gdbText) || (m_iType == gdbMemo))
    {
        iPos = PQfnumber(res, "fieldformat");
        if(iPos > -1)
        {
            TCHAR tsBuf[256];
            VarToTCharBuf(pvRow[iPos], tsBuf, 256);
            m_bHypertext = (_tcsicmp(tsBuf, _T("Hypertext")) == 0);
        }
    }

    for(int i = 0; i < ncols; i++) VariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    PQclear(res);
    return;
}

void CDbuField::CheckGeomMeta(LPTSTR tsTblName, LPTSTR tsFldName)
{
    int ilen = strlen("select * from  t1 \
inner join  t2 on t1.indexid = t2.indexid \
where t2.featurename = $1 and t2.fieldname = $2") +
        strlen(m_pConnStruct->sGeometryProperties) +
        strlen(m_pConnStruct->sFieldLookup);
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "select * from %s t1 \
inner join %s t2 on t1.indexid = t2.indexid \
where t2.featurename = $1 and t2.fieldname = $2",
        m_pConnStruct->sGeometryProperties, m_pConnStruct->sFieldLookup);

    LPSTR psParams[2];
    psParams[0] = TCharToDBStr(tsTblName, m_pConnStruct->lDBEnc);
    psParams[1] = TCharToDBStr(tsFldName, m_pConnStruct->lDBEnc);
    PGresult *res = PQexecParams(m_pConnStruct->pConn, sCmd, 2, NULL, psParams,
        NULL, NULL, 1);
    free(psParams[1]);
    free(psParams[0]);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'CDbuField::CheckGeomMeta' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long lrows = PQntuples(res);
    if(lrows < 1)
    {
        PQclear(res);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);
    DescribeFieldTypes(res, ncols, piTypes);
    GetRecordBin(res, 0, ncols, piTypes, pvRow, m_pConnStruct->lDBEnc,
        m_pConnStruct->lGeomOid, NULL, m_pConnStruct->iCurrDigits);

    int iPos = PQfnumber(res, "fielddescription");
    if(iPos > -1)
    {
        VarToTCharBuf(pvRow[iPos], m_tsDescription, 256);
    }
    iPos = PQfnumber(res, "primarygeometryflag");
    if(iPos > -1) m_bIsKey = VarToBool(pvRow[iPos]);
    if(m_iType == gdbLongBinary)
    {
        m_iType = 33;
        iPos = PQfnumber(res, "geometrytype");
        if(iPos) m_lSubType = VarToLong(pvRow[iPos]);
        iPos = PQfnumber(res, "gcoordsystemguid");
        if(iPos)
        {
            if(pvRow[iPos].vt == VT_BSTR)
            {
                VARIANT *pRow = FindCSTableRowByCSGUID(&m_pConnStruct->cCSTable,
                    pvRow[iPos].bstrVal);
                m_ulSrid = VarToLong(pRow[0]);
            }
        }
    }

    for(int i = 0; i < ncols; i++) VariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    PQclear(res);
    return;
}

LPTSTR CDbuField::GetNamePtr()
{
    return(m_tsName);
}

LPSTR CDbuField::GetOrigNamePtr()
{
    return(m_sOrigName);
}

bool CDbuField::UpdateAttrs(LPTSTR *psPrimGeomName, bool bPrimGeomSet)
{
    char sSchemaBuf[8];
    strcpy(sSchemaBuf, "public");
    LPSTR sSchemaName = m_sOrigSchema;
    if(!sSchemaName) sSchemaName = sSchemaBuf;

    switch(m_iType)
    {
    //case gdbText:
    //case gdbMemo:
    case gdbLongBinary:
        if(m_pConnStruct->bGeomPrepared)
        {
            m_lSubType = GetGeometryType(m_pConnStruct, sSchemaName,
                m_sOrigTable, m_sOrigName, &m_iGeomDim, &m_ulSrid,
                &m_iPsgType, NULL);
        }
        switch(m_lSubType)
        {
        case 0:
            break;
        case 5:
            m_iType = 33;
            break;
        default:
            m_iType = 32;
        }
        break;
    }

    if(m_iType < 10) m_lAttr |= 1;
    else switch(m_iType)
    {
    case 11:
    case 12:
    case 32:
    case 33:
        m_lAttr |= 2;
        break;
    }

    CheckDefault(m_sOrigSchema, m_sOrigTable, m_sOrigName);
    CheckAttrMeta(m_tsTblName, m_tsName);
    CheckGeomMeta(m_tsTblName, m_tsName);

    if(!bPrimGeomSet && (m_iType > 31))
    {
        if(*psPrimGeomName)
        {
            if(_tcslen(*psPrimGeomName) > 0)
            {
                if(_tcsicmp(m_tsName, *psPrimGeomName) == 0)
                {
                    m_bIsKey = true;
                    bPrimGeomSet = true;
                }
            }
            else
            {
                free(*psPrimGeomName);
                *psPrimGeomName =
                    (LPTSTR)malloc((_tcslen(m_tsName) + 1)*sizeof(TCHAR));
                _tcscpy(*psPrimGeomName, m_tsName);
                m_bIsKey = true;
                bPrimGeomSet = true;
            }
        }
        else
        {
            *psPrimGeomName =
                (LPTSTR)malloc((_tcslen(m_tsName) + 1)*sizeof(TCHAR));
            _tcscpy(*psPrimGeomName, m_tsName);
            m_bIsKey = true;
            bPrimGeomSet = true;
        }
    }
    return(bPrimGeomSet);
}

int CDbuField::GetDateType()
{
    return(m_iDateType);
}

void CDbuField::SetIsKey(bool bIsKey)
{
    m_bIsKey = bIsKey;
    return;
}

bool CDbuField::GetIsKey()
{
    return(m_bIsKey);
}

int CDbuField::GetType()
{
    return(m_iType);
}

void CDbuField::SetType(int iType)
{
    m_iType = iType;
    m_bHasChanged = true;
    return;
}

long CDbuField::GetSubType()
{
    return(m_lSubType);
}

void CDbuField::SetSubType(long lSubType)
{
    m_lSubType = lSubType;
    m_bHasChanged = true;
    return;
}

unsigned long CDbuField::GetSrid()
{
    return(m_ulSrid);
}

void CDbuField::SetSrid(unsigned long ulSrid)
{
    m_ulSrid = ulSrid;
    m_bHasChanged = true;
    return;
}

LPTSTR CDbuField::GetDescription()
{
    return(m_tsDescription);
}

void CDbuField::SetDescription(LPTSTR sDesc)
{
    _tcscpy(m_tsDescription, sDesc);
    m_bHasChanged = true;
    return;
}

bool CDbuField::GetHasSpecField()
{
    return(m_bHasSpecField);
}

bool CDbuField::GetIsHypertext()
{
    return(m_bHypertext);
}

void CDbuField::SetIsHypertext(bool bIsHypertext)
{
    m_bHypertext = bIsHypertext;
}

bool CDbuField::GetVisible()
{
    return(m_bVisible);
}

void CDbuField::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
    m_bHasChanged = true;
    return;
}

void CDbuField::SetHasSpecField(bool bHas)
{
    m_bHasSpecField = bHas;
    return;
}

int CDbuField::GetColPos()
{
    return m_iColPos;
}


// CDbuFields

CDbuFields::CDbuFields(PConnStruct pConnStruct)
{
    m_pConnStruct = pConnStruct;
    m_iDataLen = 0;
    m_iDataSize = 32;
    m_pFields = (CDbuField**)malloc(m_iDataSize*sizeof(CDbuField*));
}

CDbuFields::~CDbuFields()
{
    for(int i = 0; i < m_iDataLen; i++)
    {
        delete m_pFields[i];
    }
    free(m_pFields);
}

void CDbuFields::AddField(CDbuField *pFld)
{
    if(m_iDataLen >= m_iDataSize)
    {
        m_iDataSize += 32;
        m_pFields = (CDbuField**)realloc(m_pFields,
            m_iDataSize*sizeof(CDbuField*));
    }
    m_pFields[m_iDataLen++] = pFld;
    return;
}

void CDbuFields::BuildFromConnection(Oid lOid, LPSTR sOrigSchema,
    LPSTR sOrigTable, LPTSTR tsTblName, LPTSTR *ptsPrimGeom)
{
    CDbuField *pfd;

    char sOidBuf[32];
    sprintf(sOidBuf, "%d", lOid);
    LPSTR psBuf = sOidBuf;
    PGresult *res = PQexecParams(m_pConnStruct->pConn,
        "select attname, atttypid, attlen, attnum, attnotnull, atttypmod, \
atthasdef, attisdropped from pg_attribute where attrelid = $1 and attnum > 0 \
order by attnum", 1,
        NULL, &psBuf, NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'CDbuFields::BuildFromConnection' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int j = 0; j < ncols; j++) VariantInit(&pvRow[j]);

    bool bPrimGeomSet = false;

    for(long li = 0; li < nrows; li++)
    {
        GetRecord(res, li, ncols, piTypes, pvRow, '.');
        if(!VarToBool(pvRow[7]) && (pvRow[0].vt == VT_LPSTR))
        {
            pfd = new CDbuField(m_pConnStruct, pvRow[0].pcVal,
                VarToLong(pvRow[1]), VarToInt(pvRow[2]), VarToLong(pvRow[5]),
                !VarToBool(pvRow[4]), VarToBool(pvRow[6]), sOrigSchema,
                sOrigTable, tsTblName, lOid,
                VarToInt(pvRow[3]));
            AddField(pfd);
        }
    }
    PQclear(res);

    for(int j = 0; j < ncols; j++) PgVariantClear(&pvRow[j]);
    free(pvRow);
    free(piTypes);

    BuildPKsFromConnection(sOidBuf);

    for(int i = 0; i < m_iDataLen; i++)
    {
        bPrimGeomSet = m_pFields[i]->UpdateAttrs(ptsPrimGeom, bPrimGeomSet);
    }

    RemSpecFields();

    return;
}

CDbuField* CDbuFields::FindByColPos(int iColPos)
{
    if(iColPos < 0) return NULL;

    bool bFound = false;
    int i = 0;
    CDbuField* fldRes = NULL;

    while(!bFound && (i < m_iDataLen))
    {
        fldRes = m_pFields[i++];
        bFound = (fldRes->GetColPos() == iColPos);
    }

    return bFound ? fldRes : NULL;
}

void CDbuFields::BuildPKsFromConnection(LPSTR sOid)
{
    PGresult *res = PQexecParams(m_pConnStruct->pConn,
        "select indnatts, indisunique, indisprimary, indkey \
from pg_index where indrelid = $1", 1, NULL, &sOid, NULL, NULL, 1);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select in 'CDbuFields::BuildPKsFromConnection' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int j = 0; j < ncols; j++) VariantInit(&pvRow[j]);

    bool bFound = false;
    long li = 0;

    while(!bFound && (li < nrows))
    {
        GetRecordBin(res, li++, ncols, piTypes, pvRow, m_pConnStruct->lDBEnc,
            m_pConnStruct->lGeomOid, NULL, m_pConnStruct->iCurrDigits);
        bFound = VarToBool(pvRow[2]);
    }

    if(bFound)
    {
        CDbuField *pFld;
        int iIdx;
        int iflds = VarToInt(pvRow[0]);
        if((pvRow[3].vt & VT_ARRAY) > 0)
        {
            long lL, lU;
            SafeArrayGetLBound(pvRow[3].parray, 1, &lL);
            SafeArrayGetUBound(pvRow[3].parray, 1, &lU);
            if(lU - lL + 1 == iflds)
            {
                short *piFlds;
                SafeArrayAccessData(pvRow[3].parray, (void**)&piFlds);
                for(int j = 0; j < iflds; j++)
                {
                    pFld = FindByColPos(piFlds[j]);
                    if(pFld) pFld->SetIsKey(true);
                }
                SafeArrayUnaccessData(pvRow[3].parray);
            }
        }
        else
        {
            iIdx = VarToInt(pvRow[3]);
            if(iIdx > 0)
            {
                pFld = m_pFields[iIdx - 1];
                pFld->SetIsKey(true);
            }
        }
    }

    for(int j = 0; j < ncols; j++) VariantClear(&pvRow[j]);
    free(pvRow);
    free(piTypes);
    PQclear(res);

    return;
}

LPTSTR CDbuFields::GetNextGeomName(int *piStart)
{
    bool bFound = false;
    int i = *piStart;
    while(!bFound && (i < m_iDataLen))
    {
        bFound = (m_pFields[i++]->GetType() > 31);
    }
    *piStart = i;
    return(bFound ? m_pFields[i - 1]->GetNamePtr() : NULL);
}

int CDbuFields::GetCount()
{
    return(m_iDataLen);
}

IDbuField* CDbuFields::GetItem(int idx)
{
    return((IDbuField*)m_pFields[idx]);
}

int CDbuFields::FindPosByName(LPTSTR tsName)
{
    bool bFound = false;
    int i = 0;
    while(!bFound && (i < m_iDataLen))
    {
        bFound = (_tcsicmp(m_pFields[i++]->GetNamePtr(), tsName) == 0);
    }
    return(bFound ? i - 1 : -1);
}

IDbuField* CDbuFields::FindByName(LPTSTR tsName)
{
    int iPos = FindPosByName(tsName);
    if(iPos < 0) return NULL;
    return (IDbuField*)m_pFields[iPos];
}

void CDbuFields::DelField(int iIndx)
{
    delete m_pFields[iIndx];
    m_iDataLen--;
    if(iIndx < m_iDataLen)
        memmove(&m_pFields[iIndx], &m_pFields[iIndx + 1],
            (m_iDataLen - iIndx)*sizeof(CDbuField*));
    return;
}

void CDbuFields::RemSpecFields()
{
    int i = 0, j;
    LPSTR sName, sGeomName;
    int iLen;
    bool bIsSpec, bHasGeom;
    CDbuField *pFld;
    while(i < m_iDataLen)
    {
        bIsSpec = false;
        bHasGeom = false;

        sName = m_pFields[i]->GetOrigNamePtr();
        iLen = strlen(sName);

        if(iLen > 4)
        {
            iLen -= 4;
            bIsSpec = ((strcmp(&sName[iLen], "_igr") == 0) ||
                (strcmp(&sName[iLen], "_nat") == 0));
        }

        if(bIsSpec)
        {
            j = 0;
            while(!bHasGeom && (j < m_iDataLen))
            {
                pFld = m_pFields[j++];
                sGeomName = pFld->GetOrigNamePtr();
                if((int)strlen(sGeomName) == iLen)
                {
                    bHasGeom = (strncmp(sName, sGeomName, iLen) == 0);
                }
            }
        }

        if(bHasGeom)
        {
            pFld->SetHasSpecField(true);
            DelField(i);
        }
        else i++;
    }
    return;
}


// CDbuTable

CDbuTable::CDbuTable(PConnStruct pConnStruct, LPSTR sSchema, LPSTR sTable,
    bool bHasIndex)
{
    m_pConnStruct = pConnStruct;
    m_lOid = 0;
    m_sOrigSchema = NULL;

    LPWSTR wsSchema = NULL;
    LPWSTR wsTable = DBStrToTChar(sTable, pConnStruct->lDBEnc);

    if(sSchema)
    {
        wsSchema = DBStrToTChar(sSchema, pConnStruct->lDBEnc);
        m_sOrigSchema = (LPSTR)malloc((strlen(sSchema) + 1)*sizeof(char));
        strcpy(m_sOrigSchema, sSchema);
    }
    m_sOrigName = (LPSTR)malloc((strlen(sTable) + 1)*sizeof(char));
    strcpy(m_sOrigName, sTable);

    int iLen = wcslen(wsTable);
    if(wsSchema)
    {
        iLen += (wcslen(wsSchema) + 1);
        m_tsName = (LPTSTR)malloc((iLen + 1)*sizeof(TCHAR));

        WCharToTCharBuf(wsSchema, m_tsName, iLen + 1);
        _tcscat(m_tsName, _T("."));

        int iPos = _tcslen(m_tsName);
        WCharToTCharBuf(wsTable, &m_tsName[iPos], iLen + 1 - iPos);

        free(wsSchema);
    }
    else
    {
        m_tsName = (LPTSTR)malloc((iLen + 1)*sizeof(TCHAR));
        WCharToTCharBuf(wsTable, m_tsName, iLen + 1);
    }
    free(wsTable);

    m_tsPrimGeom = NULL;
    m_tsDescription[0] = 0;
    m_bVisible = false;
    m_bHasMetadata = false;
    m_bLoaded = false;
    m_bHasIndex = bHasIndex;
    m_pFields = new CDbuFields(pConnStruct);
    m_bIsView = false;
}

CDbuTable::~CDbuTable()
{
    delete m_pFields;
    if(m_tsPrimGeom) free(m_tsPrimGeom);
    if(m_tsName) free(m_tsName);
    if(m_sOrigSchema) free(m_sOrigSchema);
    if(m_sOrigName) free(m_sOrigName);
}

LPTSTR CDbuTable::GetNamePtr()
{
    return(m_tsName);
}

bool CDbuTable::GetVisible()
{
    return(m_bVisible);
}

void CDbuTable::RefreshFromConn(LPCSTR stmtOid, LPCSTR stmtFtr)
{
    char sSchemaBuf[8];
    sprintf(sSchemaBuf, "public");
    LPSTR sSchema = m_sOrigSchema;
    if(!sSchema) sSchema = sSchemaBuf;

    LPSTR psBuf[2] = {m_sOrigName, sSchema};
    PGresult *res = PQexecPrepared(m_pConnStruct->pConn, stmtOid, 2, psBuf,
        NULL, NULL, 0);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Exec Oid statement in 'CDbuTable::RefreshFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    long lrows = PQntuples(res);
    if(lrows < 1)
    {
        PQclear(res);
        return;
    }

    LPSTR sVal = PQgetvalue(res, 0, 0);
    sscanf(sVal, "%d", &m_lOid);
    PQclear(res);

    lrows = 0;
    psBuf[0] = TCharToDBStr(m_tsName, m_pConnStruct->lDBEnc);
    res = PQexecPrepared(m_pConnStruct->pConn, stmtFtr, 1, psBuf, NULL, NULL, 1);
    free(psBuf[0]);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Exec Ftr statement in 'CDbuTable::RefreshFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        return;
    }

    lrows = PQntuples(res);

    if(lrows > 0)
    {
        m_bVisible = true;
        m_bHasMetadata = true;

        int ncols = PQnfields(res);
        Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
        DescribeFieldTypes(res, ncols, piTypes);

        VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
        for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);
        GetRecordBin(res, 0, ncols, piTypes, pvRow, m_pConnStruct->lDBEnc,
            m_pConnStruct->lGeomOid, NULL, m_pConnStruct->iCurrDigits);

        int iPos = PQfnumber(res, "primarygeometryfieldname");
        if(iPos > -1)
        {
            if(m_tsPrimGeom) free(m_tsPrimGeom);
            m_tsPrimGeom = VarToTChar(pvRow[iPos]);
        }
        iPos = PQfnumber(res, "featuredescription");
        if(iPos > -1)
        {
            VarToTCharBuf(pvRow[iPos], m_tsDescription, 256);
        }

        for(int i = 0; i < ncols; i++) VariantClear(&pvRow[i]);
        free(pvRow);
        free(piTypes);
    }
    PQclear(res);

    m_bHasChanged = false;
    return;
}

void CDbuTable::LoadFields()
{
    if(m_bLoaded) return;
    m_pFields->BuildFromConnection(m_lOid, m_sOrigSchema, m_sOrigName,
        m_tsName, &m_tsPrimGeom);
    m_bLoaded = true;
    return;
}

void CDbuTable::InsertAttrMetadata(LPTSTR tsTblName, LPSTR sSchema,
    LPSTR sTable, CDbuField *pfld)
{
    LPSTR psParam[3] = {sSchema, sTable, pfld->GetOrigNamePtr()};

    char sSql[128];

    sprintf(sSql, "delete from %s where f_table_schema = $1 and \
f_table_name = $2 and f_geometry_column = $3", m_pConnStruct->sGeomCols);
    PGresult *res = PQexecParams(m_pConnStruct->pConn, sSql, 3, NULL, psParam,
        NULL, NULL, 0);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Delete from geometry_columns in 'CDbuTable::InsertAttrMetadata' failed\r\n",
            m_pConnStruct->pConn);
    }
    PQclear(res);

    long lId = GetFieldIdT(m_pConnStruct, tsTblName, pfld->GetNamePtr(), false);
    if(lId < 1)
        lId = GetFieldIdT(m_pConnStruct, tsTblName, pfld->GetNamePtr(), true);
    if(lId < 1) return;

    int ilen = strlen("delete from  where indexid = ") +
        strlen(m_pConnStruct->sGeometryProperties) + 32;
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "delete from %s where indexid = %ld",
        m_pConnStruct->sGeometryProperties, lId);
    res = PQexec(m_pConnStruct->pConn, sCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Delete from GeometryProperties in 'CDbuTable::InsertAttrMetadata' failed\r\n",
            m_pConnStruct->pConn);
    }
    PQclear(res);
    free(sCmd);

    LPCSTR sFldNames[7] = {"indexid", "fielddescription", "fieldformat",
        "fieldprecision", "fieldtype", "iskeyfield", "isfielddisplayable"};
    VARIANT vVals[7];
    vVals[0].vt = VT_I4;
    vVals[0].lVal = lId;

    vVals[1].vt = VT_NULL;
    if(_tcslen(pfld->GetDescription()) > 0)
    {
        vVals[1].vt = VT_BSTR;
        vVals[1].bstrVal = TCharToBStr(pfld->GetDescription());
    }

    int iDateType = pfld->GetDateType();

    switch(pfld->GetType())
    {
    case gdbByte:
    case gdbInteger:
    case gdbLong:
    case gdbSingle:
    case gdbDouble:
        vVals[2].vt = VT_BSTR;
        vVals[2].bstrVal = SysAllocString(L"General Number");
        break;
    case gdbBoolean:
        vVals[2].vt = VT_BSTR;
        vVals[2].bstrVal = SysAllocString(L"True/False");
        break;
    case gdbDate:
        vVals[2].vt = VT_BSTR;
        if(iDateType == 1)
            vVals[2].bstrVal = SysAllocString(L"Date/Time");
        else if(iDateType == 2)
            vVals[2].bstrVal = SysAllocString(L"Time");
        else if(iDateType == 3)
            vVals[2].bstrVal = SysAllocString(L"Date");
        else vVals[2].vt = VT_NULL;
        break;
    case gdbText:
    case gdbMemo:
        if(pfld->GetIsHypertext())
        {
            vVals[2].vt = VT_BSTR;
            vVals[2].bstrVal = SysAllocString(L"Hypertext");
        }
        else vVals[2].vt = VT_NULL;
        break;
    default:
        vVals[2].vt = VT_NULL;
    }
    vVals[3].vt = VT_I4;
    switch(pfld->GetType())
    {
    case gdbSingle:
    case gdbDouble:
        vVals[3].lVal = 6;
        break;
    default:
        vVals[3].lVal = 0;
    }
    vVals[4].vt = VT_I4;
    vVals[4].lVal = pfld->GetType();
    vVals[5].vt = VT_BOOL;
    if(pfld->GetIsKey()) vVals[5].boolVal = VARIANT_TRUE;
    else vVals[5].boolVal = VARIANT_FALSE;
    vVals[6].vt = VT_BOOL;
    if(pfld->GetVisible()) vVals[6].boolVal = VARIANT_TRUE;
    else vVals[6].boolVal = VARIANT_FALSE;

    InsOrUpdateTable(m_pConnStruct->pConn, m_pConnStruct->sAttributeProperties,
        7, sFldNames, vVals, 0, m_pConnStruct->lDBEnc, m_pConnStruct->iCurrDigits);

    for(int i = 0; i < 7; i++) VariantClear(&vVals[i]);
    return;
}

BSTR SridToCSGUID(PGconn *pConn, PCSArray pCSTable, LPSTR sCSTable,
    unsigned long ulSrid, long lEnc)
{
    BSTR bsRes = NULL;
    VARIANT *pRow = FindCSTableRow(pCSTable, ulSrid);
    if(!pRow)
    {
        bsRes = SysAllocString(L"");
        return(bsRes);
    }

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

        InsertTable(pConn, sCSTable, 119, &pRow[1], lEnc, 2);
    }

    bsRes = SysAllocString(pRow[1].bstrVal);
    return(bsRes);
}

void CDbuTable::InsertGeomMetadata(LPTSTR tsTblName, LPSTR sSchema,
    LPSTR sTable, CDbuField *pfld)
{
    long lId = GetFieldIdT(m_pConnStruct, tsTblName, pfld->GetNamePtr(), false);
    if(lId < 1)
        lId = GetFieldIdT(m_pConnStruct, tsTblName, pfld->GetNamePtr(), true);
    if(lId < 1) return;

    int ilen = strlen("delete from  where indexid = ") +
        strlen(m_pConnStruct->sAttributeProperties) + 32;
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "delete from %s where indexid = %ld",
        m_pConnStruct->sAttributeProperties, lId);

    PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Delete from AttributeProperties in 'CDbuTable::InsertGeomMetadata' failed\r\n",
            m_pConnStruct->pConn);
    }
    PQclear(res);

    LPCSTR sFldNames[5] = {"indexid", "gcoordsystemguid", "fielddescription",
        "geometrytype", "primarygeometryflag"};
    VARIANT vVals[5];
    vVals[0].vt = VT_I4;
    vVals[0].lVal = lId;
    vVals[1].vt = VT_BSTR;
    vVals[1].bstrVal = SridToCSGUID(m_pConnStruct->pConn,
        &m_pConnStruct->cCSTable, m_pConnStruct->sCoordSystems, pfld->GetSrid(),
        m_pConnStruct->lDBEnc);

    vVals[2].vt = VT_NULL;
    if(_tcslen(pfld->GetDescription()) > 0)
    {
        vVals[2].vt = VT_BSTR;
        vVals[2].bstrVal = TCharToBStr(pfld->GetDescription());
    }

    vVals[3].vt = VT_I4;
    vVals[3].lVal = pfld->GetSubType();
    vVals[4].vt = VT_BOOL;
    if(pfld->GetIsKey()) vVals[4].boolVal = VARIANT_TRUE;
    else vVals[4].boolVal = VARIANT_FALSE;

    InsOrUpdateTable(m_pConnStruct->pConn, m_pConnStruct->sGeometryProperties,
        5, sFldNames, vVals, 0, m_pConnStruct->lDBEnc, m_pConnStruct->iCurrDigits);

    for(int i = 0; i < 5; i++) VariantClear(&vVals[i]);
    return;
}

void CDbuTable::InsertFeatureMetadata()
{
    LPCSTR psFldNames[4] = {"featurename", "geometrytype",
        "primarygeometryfieldname", "featuredescription"};
    VARIANT vValues[4];
    vValues[0].vt = VT_BSTR;
    vValues[0].bstrVal = TCharToBStr(m_tsName);

    vValues[1].vt = VT_I4;
    CDbuField *pfld = NULL;
    if(m_tsPrimGeom) pfld = (CDbuField*)m_pFields->FindByName(m_tsPrimGeom);
    if(pfld)
    {
        if(pfld->GetSubType() == gdbGraphicsText) vValues[1].lVal = 33;
        else vValues[1].lVal = pfld->GetSubType();

        vValues[2].vt = VT_BSTR;
        vValues[2].bstrVal = TCharToBStr(pfld->GetNamePtr());
    }
    else
    {
        vValues[1].lVal = -1;
        vValues[2].vt = VT_NULL;
    }

    if(_tcslen(m_tsDescription) > 0)
    {
        vValues[3].vt = VT_BSTR;
        vValues[3].bstrVal = TCharToBStr(m_tsDescription);
    }
    else vValues[3].vt = VT_NULL;

    InsOrUpdateTable(m_pConnStruct->pConn, m_pConnStruct->sGFeatures, 4,
        psFldNames, vValues, 0, m_pConnStruct->lDBEnc, m_pConnStruct->iCurrDigits);

    for(int i = 0; i < 4; i++) VariantClear(&vValues[i]);

    return;
}

void CDbuTable::SaveTableMetadata()
{
    if(!m_bHasChanged) return;

    LoadFields();
    if(m_bVisible)
    {
        InsertFeatureMetadata();
        int iCnt = m_pFields->GetCount();
        CDbuField *pfld;

        char sSchemaBuf[8];
        sprintf(sSchemaBuf, "public");
        LPSTR sSchema = m_sOrigSchema;
        if(!sSchema) sSchema = sSchemaBuf;

        for(int i = 0; i < iCnt; i++)
        {
            pfld = (CDbuField*)m_pFields->GetItem(i);
            if(pfld->GetType() < 31)
                InsertAttrMetadata(m_tsName, sSchema, m_sOrigName, pfld);
            else InsertGeomMetadata(m_tsName, sSchema, m_sOrigName, pfld);
        }
    }
    else
    {
        LPSTR sFullName = TCharToDBStr(m_tsName, m_pConnStruct->lDBEnc);
        int ibaselen = strlen("delete from  where indexid in (select indexid from  where featurename = '')") +
            strlen(sFullName) + strlen(m_pConnStruct->sFieldLookup);
        int ilen = ibaselen + strlen(m_pConnStruct->sAttributeProperties);
        LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
        sprintf(sCmd, "delete from %s where indexid in (select indexid from %s where featurename = '%s')",
            m_pConnStruct->sAttributeProperties, m_pConnStruct->sFieldLookup,
            sFullName);
        PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Delete from AttributeProperties in 'CDbuTable::SaveTableMetadata' failed\r\n",
                m_pConnStruct->pConn);
        }
        PQclear(res);
        free(sCmd);

        ilen = ibaselen + strlen(m_pConnStruct->sGeometryProperties);
        sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
        sprintf(sCmd, "delete from %s where indexid in (select indexid from %s where featurename = '%s')",
            m_pConnStruct->sGeometryProperties, m_pConnStruct->sFieldLookup,
            sFullName);
        res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Delete from GeometryProperties in 'CDbuTable::SaveTableMetadata' failed\r\n",
                m_pConnStruct->pConn);
        }
        PQclear(res);
        free(sCmd);

        ibaselen = strlen("delete from  where featurename = ''") +
            strlen(sFullName);
        ilen = ibaselen + strlen(m_pConnStruct->sFieldLookup);
        sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
        sprintf(sCmd, "delete from %s where featurename = '%s'",
            m_pConnStruct->sFieldLookup, sFullName);
        res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Delete from FieldLookup in 'CDbuTable::SaveTableMetadata' failed\r\n",
                m_pConnStruct->pConn);
        }
        PQclear(res);
        free(sCmd);

        ilen = ibaselen + strlen(m_pConnStruct->sGFeatures);
        sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
        sprintf(sCmd, "delete from %s where featurename = '%s'",
            m_pConnStruct->sGFeatures, sFullName);
        res = PQexec(m_pConnStruct->pConn, sCmd);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Delete from GFeatures in 'CDbuTable::SaveTableMetadata' failed\r\n",
                m_pConnStruct->pConn);
        }
        PQclear(res);
        free(sCmd);

        free(sFullName);
    }
    return;
}

LPTSTR CDbuTable::GetPrimaryGeometry()
{
    return(m_tsPrimGeom);
}

void CDbuTable::SetPrimaryGeometry(LPTSTR sGeomName)
{
    if(m_tsPrimGeom) free(m_tsPrimGeom);
    m_tsPrimGeom = NULL;
    m_bHasChanged = true;

    if(!sGeomName) return;

    int ilen = _tcslen(sGeomName);
    if(ilen > 0)
    {
        m_tsPrimGeom = (LPTSTR)malloc((ilen + 1)*sizeof(TCHAR));
        _tcscpy(m_tsPrimGeom, sGeomName);
    }

    CDbuField *pfld;
    if(m_tsPrimGeom)
    {
        for(int i = 0; i < m_pFields->GetCount(); i++)
        {
            pfld = (CDbuField*)m_pFields->GetItem(i);
            if(pfld->GetType() > 31)
                pfld->SetIsKey(_tcscmp(pfld->GetNamePtr(), m_tsPrimGeom) == 0);
        }
    }
    else
    {
        for(int i = 0; i < m_pFields->GetCount(); i++)
        {
            pfld = (CDbuField*)m_pFields->GetItem(i);
            if(pfld->GetType() > 31) pfld->SetIsKey(false);
        }
    }
    return;
}

LPTSTR CDbuTable::GetDescription()
{
    return(m_tsDescription);
}

void CDbuTable::SetDescription(LPTSTR sDesc)
{
    _tcscpy(m_tsDescription, sDesc);
    m_bHasChanged = true;
    return;
}

void CDbuTable::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
    m_bHasChanged = true;
    return;
}

bool CDbuTable::GetIsView()
{
    return(m_bIsView);
}

void CDbuTable::SetIsView(bool bIsView)
{
    m_bIsView = bIsView;
    return;
}

void CDbuTable::SetChanged(bool bChanged)
{
    m_bHasChanged = bChanged;
    return;
}

bool CDbuTable::DropINGRField(CDbuField *pField)
{
    bool bRes = true;
    LPSTR sFldName = pField->GetOrigNamePtr();

    int iLen = strlen("alter table  drop column _igr") + strlen(m_sOrigName) +
        strlen(sFldName) + 5;
    if(m_sOrigSchema) iLen += (strlen(m_sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" drop column \"%s_igr\"",
            m_sOrigSchema, m_sOrigName, sFldName);
    else
        sprintf(sSql, "alter table \"%s\" drop column \"%s_igr\"",
            m_sOrigName, sFldName);

    PGresult *res = PQexec(m_pConnStruct->pConn, sSql);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Drop column in 'CDbuTable::DropINGRField' failed\r\n",
            m_pConnStruct->pConn);
        bRes = false;
    }
    else pField->SetHasSpecField(false);
    PQclear(res);
    free(sSql);
    return(bRes);
}

bool CDbuTable::CreateINGRField(CDbuField *pField)
{
    bool bRes = true;
    LPSTR sFldName = pField->GetOrigNamePtr();

    int iLen = strlen("alter table  add column _igr bytea") +
        strlen(m_sOrigName) + strlen(sFldName) + 5;
    if(m_sOrigSchema) iLen += (strlen(m_sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" add column \"%s_igr\" bytea",
            m_sOrigSchema, m_sOrigName, sFldName);
    else
        sprintf(sSql, "alter table \"%s\" add column \"%s_igr\" bytea",
            m_sOrigName, sFldName);

    PGresult *res = PQexec(m_pConnStruct->pConn, sSql);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Add column in 'CDbuTable::CreateINGRField' failed\r\n",
            m_pConnStruct->pConn);
        bRes = false;
    }
    else pField->SetHasSpecField(true);
    PQclear(res);
    free(sSql);
    return(bRes);
}

bool CDbuTable::DropNativeField(CDbuField *pField)
{
    bool bRes = true;
    LPSTR sFldName = pField->GetOrigNamePtr();

    int iLen = strlen("alter table  drop column _nat") + strlen(m_sOrigName) +
        strlen(sFldName) + 5;
    if(m_sOrigSchema) iLen += (strlen(m_sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));
    if(m_sOrigSchema)
        sprintf(sSql, "alter table \"%s\".\"%s\" drop column \"%s_nat\"",
            m_sOrigSchema, m_sOrigName, sFldName);
    else
        sprintf(sSql, "alter table \"%s\" drop column \"%s_nat\"",
            m_sOrigName, sFldName);

    PGresult *res = PQexec(m_pConnStruct->pConn, sSql);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Drop column in 'CDbuTable::DropNativeField' failed\r\n",
            m_pConnStruct->pConn);
        bRes = false;
    }
    else pField->SetHasSpecField(false);
    PQclear(res);
    free(sSql);
    return(bRes);
}

bool CDbuTable::CreateNativeField(CDbuField *pField)
{
    bool bRes = true;
    LPSTR sFldName = pField->GetOrigNamePtr();
    long lSubType = pField->GetSubType();
    int iLen = 0;

    if(lSubType == 4) // coverage
    {
        iLen = strlen("SELECT AddGeometryColumn('', '', '', , 'POLYGON', 2)")
            + strlen(m_sOrigName) + strlen(sFldName) + 16;
    }
    else if(lSubType == 5) // grephic text
    {
        iLen = strlen("SELECT AddGeometryColumn('', '', '', , 'POINT', 2)")
            + strlen(m_sOrigName) + strlen(sFldName) + 16;
    }
    else return(false);

    if(m_sOrigSchema) iLen += (strlen(m_sOrigSchema) + 3);

    LPSTR sSql = (LPSTR)malloc(iLen*sizeof(char));

    if(lSubType == 4) // coverage
    {
        if(m_sOrigSchema)
            sprintf(sSql, "SELECT AddGeometryColumn('%s', '%s', '%s_nat', %ld, 'POLYGON', 2)",
                m_sOrigSchema, m_sOrigName, sFldName, pField->GetSrid());
        else
            sprintf(sSql, "SELECT AddGeometryColumn('', '%s', '%s_nat', %ld, 'POLYGON', 2)",
                m_sOrigName, sFldName, pField->GetSrid());
    }
    else if(lSubType == 5) // grephic text
    {
        if(m_sOrigSchema)
            sprintf(sSql, "SELECT AddGeometryColumn('%s', '%s', '%s_nat', %ld, 'POINT', 2)",
                m_sOrigSchema, m_sOrigName, sFldName, pField->GetSrid());
        else
            sprintf(sSql, "SELECT AddGeometryColumn('', '%s', '%s_nat', %ld, 'POINT', 2)",
                m_sOrigName, sFldName, pField->GetSrid());
    }

    PGresult *res = PQexec(m_pConnStruct->pConn, sSql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Add column in 'CDbuTable::CreateNativeField' failed\r\n",
            m_pConnStruct->pConn);
        bRes = false;
    }
    else pField->SetHasSpecField(true);
    PQclear(res);
    free(sSql);

    if(bRes) PopulateNatField(pField, lSubType);

    return(bRes);
}

void CDbuTable::PopulateNatField(CDbuField *pField, long lSubType)
{
    LPSTR sFldName = pField->GetOrigNamePtr();
    int iNamesLen = strlen(m_sOrigName) + strlen(sFldName) + 5;
    if(m_sOrigSchema) iNamesLen += (strlen(m_sOrigSchema) + 3);
    int iSelLen = strlen("select ctid,  from ") + iNamesLen;
    int iInsLen = strlen("update  set _nat=$1 where ctid=$2") + iNamesLen;
    LPSTR sSql = (LPSTR)malloc(iSelLen*sizeof(char));
    if(m_sOrigSchema)
        sprintf(sSql, "select ctid, \"%s\" from \"%s\".\"%s\"", sFldName,
            m_sOrigSchema, m_sOrigName);
    else
        sprintf(sSql, "select ctid, \"%s\" from \"%s\"", sFldName, m_sOrigName);

    PGresult *res = PQexecParams(m_pConnStruct->pConn, sSql, 0, NULL, NULL,
        NULL, NULL, 1);
    free(sSql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        return;
    }

    sSql = (LPSTR)malloc(iInsLen*sizeof(char));
    if(m_sOrigSchema)
        sprintf(sSql, "update \"%s\".\"%s\" set \"%s_nat\"=$1 where ctid=$2",
            m_sOrigSchema, m_sOrigName, sFldName);
    else
        sprintf(sSql, "update \"%s\" set \"%s_nat\"=$1 where ctid=$2",
            m_sOrigName, sFldName);

    Oid iParTypes[2] = {BYTEAOID, 0};
    PGresult *insres = PQprepare(m_pConnStruct->pConn, "insnatgeomstmt", sSql,
        2, iParTypes);
    if(PQresultStatus(insres) != PGRES_COMMAND_OK)
    {
        PQclear(insres);
        PQclear(res);
        return;
    }
    PQclear(insres);

    int iRows = PQntuples(res);
    LPSTR psParams[2];
    int piParLen[2];
    int piParForms[2] = {1, 1};
    unsigned char *psGeom;
    int iPsgType = 1;
    if(lSubType == 4) iPsgType = 3;
    long lBufSize;
    unsigned long ulSrid = pField->GetSrid();
    long plParams[4];
    long lErrMsgId;
    int iGeomLen;

    for(int i = 0; i < iRows; i++)
    {
        iGeomLen = PQgetlength(res, i, 1);
        if(iGeomLen > 0)
        {
            psGeom = (unsigned char*)PQgetvalue(res, i, 1);
            lBufSize = GeomBlobtoEWKBLen(psGeom, 2, ulSrid, iPsgType, plParams,
                &lErrMsgId);

            piParLen[0] = lBufSize;
            psParams[0] = (LPSTR)malloc(lBufSize);
            GeomBlobtoEWKB(psGeom, 2, ulSrid, iPsgType,
                (unsigned char*)psParams[0], plParams, &lErrMsgId);
        }
        else
        {
            piParLen[0] = 0;
            psParams[0] = NULL;
        }

        piParLen[1] = PQgetlength(res, i, 0);
        psParams[1] = PQgetvalue(res, i, 0);
        insres = PQexecPrepared(m_pConnStruct->pConn, "insnatgeomstmt", 2,
            psParams, piParLen, piParForms, 1);
        PQclear(insres);

        if(psParams[0]) free(psParams[0]);
    }

    PQclear(res);
    res = PQexec(m_pConnStruct->pConn, "DEALLOCATE insnatgeomstmt");
    PQclear(res);
    return;
}

bool CDbuTable::ChangeSpecField(IDbuField *pField)
{
    long lSubType = pField->GetSubType();
    bool bHasField = pField->GetHasSpecField();
    bool bRes = false;

    if((lSubType == 3) || (lSubType == 10))
    {
        if(bHasField) bRes = DropINGRField((CDbuField*)pField);
        else bRes = CreateINGRField((CDbuField*)pField);
    }
    else
    {
        if(bHasField) bRes = DropNativeField((CDbuField*)pField);
        else bRes = CreateNativeField((CDbuField*)pField);
    }
    return(bRes);
}

IDbuFields* CDbuTable::GetFields()
{
    LoadFields();
    return((IDbuFields*)m_pFields);
}


// CDbuTables

CDbuTables::CDbuTables(PConnStruct pConnStruct)
{
    m_pConnStruct = pConnStruct;
    m_iDataSize = 32;
    m_iDataLen = 0;
    m_pTables = (CDbuTable**)malloc(m_iDataSize*sizeof(CDbuTable*));
}

CDbuTables::~CDbuTables()
{
    for(int i = 0; i < m_iDataLen; i++)
    {
        delete m_pTables[i];
    }
    free(m_pTables);
    return;
}

void CDbuTables::AddTable(CDbuTable *pTbl)
{
    if(m_iDataLen >= m_iDataSize)
    {
        m_iDataSize += 32;
        m_pTables = (CDbuTable**)realloc(m_pTables,
            m_iDataSize*sizeof(CDbuTable*));
    }
    m_pTables[m_iDataLen++] = pTbl;
    return;
}

bool CDbuTables::IsNotMetaTable(LPSTR sName, bool bTrans)
{
    if(stricmp(sName, "geography_columns") == 0) return(false);
    if(stricmp(sName, "raster_columns") == 0) return(false);
    if(stricmp(sName, "raster_overviews") == 0) return(false);
    if(stricmp(sName, "gdo_graphic_columns") == 0) return(false);
    if(stricmp(sName, m_pConnStruct->sGeomCols) == 0) return(false);
    if(stricmp(sName, m_pConnStruct->sSpatRefSys) == 0) return(false);
    if(stricmp(sName, m_pConnStruct->sGAlias) == 0) return(false);
    if(stricmp(sName, m_pConnStruct->sGraphicColumns) == 0) return(false);

    LPSTR sDot = strchr(sName, '.');
    if(sDot) sDot++;
    else sDot = sName;
    if(stricmp(sDot, "gfeaturesbase") == 0) return(false);

    int ilen = strlen("select * from  where TableName = ''") +
        strlen(sName) + strlen(m_pConnStruct->sGAlias);
    LPSTR sCmd = (LPSTR)malloc((ilen + 1)*sizeof(char));
    sprintf(sCmd, "select * from %s where tablename = '%s'",
        m_pConnStruct->sGAlias, sName);

    SetSP(m_pConnStruct->pConn, bTrans);
    //LPSTR lsCmd = GetSaveSQL(sCmd, bTrans);
    PGresult *res = PQexec(m_pConnStruct->pConn, sCmd);
    //free(lsCmd);
    free(sCmd);

    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Select from galiastable in 'CDbuTable::IsNotMetaTable' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, bTrans);
        return(true);
    }

    long lrows = PQntuples(res);
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, bTrans);
    return(lrows < 1);
}

int CDbuTables::GetCount()
{
    return(m_iDataLen);
}

IDbuTable* CDbuTables::GetItem(int idx)
{
    return((IDbuTable*)m_pTables[idx]);
}

void CDbuTables::BuildFromConn()
{
    for(int i = 0; i < m_iDataLen; i++)
    {
        delete m_pTables[i];
    }
    m_iDataLen = 0;

    CDbuTable *ptd;
    char sNameBuf[128];

    PGresult *res;
    if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
    {
        res = PQexec(m_pConnStruct->pConn, "begin");
        PQclear(res);
    }
    m_pConnStruct->iSysTrans++;

    SetSP(m_pConnStruct->pConn, true);
    res = PQexec(m_pConnStruct->pConn, "declare tblcur no scroll cursor for \
select * from pg_tables where not schemaname='pg_catalog' and not schemaname='information_schema'");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Declare tblcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, true);
        m_pConnStruct->iSysTrans--;
        if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
        {
            res = PQexec(m_pConnStruct->pConn, "end");
            PQclear(res);
        }
        return;
    }
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, true);

    res = PQexec(m_pConnStruct->pConn, "fetch in tblcur");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Fetch tblcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        m_pConnStruct->iSysTrans--;
        if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
        {
            res = PQexec(m_pConnStruct->pConn, "end");
            PQclear(res);
        }
        return;
    }

    int ncols = PQnfields(res);
    Oid *piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    int iSchmPos = PQfnumber(res, "schemaname");
    int iNamePos = PQfnumber(res, "tablename");
    int iIdxPos = PQfnumber(res, "hasindexes");

    VARIANT *pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);

    LPSTR sSchemaPtr;

    long nrows = PQntuples(res);
    while(nrows > 0)
    {
        GetRecord(res, 0, ncols, piTypes, pvRow, m_pConnStruct->sDecSep);
        PQclear(res);
        if(pvRow[iNamePos].vt == VT_LPSTR)
        {
            sNameBuf[0] = 0;
            if(strcmp(pvRow[iSchmPos].pcVal, "public") != 0)
            {
                sSchemaPtr = pvRow[iSchmPos].pcVal;
                strcpy(sNameBuf, pvRow[iSchmPos].pcVal);
                strcat(sNameBuf, ".");
            }
            else sSchemaPtr = NULL;
            strcat(sNameBuf, pvRow[iNamePos].pcVal);

            if((strcmp(pvRow[iSchmPos].pcVal, "topology") != 0) &&
                IsNotMetaTable(sNameBuf, true))
            {
                ptd = new CDbuTable(m_pConnStruct, sSchemaPtr,
                    pvRow[iNamePos].pcVal, VarToBool(pvRow[iIdxPos]));
                AddTable(ptd);
            }
        }
        res = PQexec(m_pConnStruct->pConn, "fetch in tblcur");
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Fetch loop tblcur in 'CDbuTables::BuildFromConn' failed\r\n",
                m_pConnStruct->pConn);
            nrows = 0;
        }
        else nrows = PQntuples(res);
    }
    PQclear(res);

    for(int i = 0; i < ncols; i++) PgVariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    res = PQexec(m_pConnStruct->pConn, "close tblcur");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Close tblcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
    }
    PQclear(res);

    SetSP(m_pConnStruct->pConn, true);
    res = PQexec(m_pConnStruct->pConn, "declare viewcur no scroll cursor for select schemaname, viewname from pg_views \
where not schemaname='pg_catalog' and not schemaname='information_schema'");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Declare viewcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);
        RollbackSP(m_pConnStruct->pConn, true);

        m_pConnStruct->iSysTrans--;
        if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
        {
            res = PQexec(m_pConnStruct->pConn, "end");
            PQclear(res);
        }
        return;
    }
    PQclear(res);
    ReleaseSP(m_pConnStruct->pConn, true);

    res = PQexec(m_pConnStruct->pConn, "fetch in viewcur");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        WritePQErrorToLog("Fetch viewcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
        PQclear(res);

        m_pConnStruct->iSysTrans--;
        if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
        {
            res = PQexec(m_pConnStruct->pConn, "end");
            PQclear(res);
        }
        return;
    }

    ncols = PQnfields(res);
    piTypes = (Oid*)malloc(ncols*sizeof(Oid));
    DescribeFieldTypes(res, ncols, piTypes);

    iSchmPos = PQfnumber(res, "schemaname");
    iNamePos = PQfnumber(res, "viewname");

    pvRow = (VARIANT*)malloc(ncols*sizeof(VARIANT));
    for(int i = 0; i < ncols; i++) VariantInit(&pvRow[i]);

    nrows = PQntuples(res);
    while(nrows > 0)
    {
        GetRecord(res, 0, ncols, piTypes, pvRow, m_pConnStruct->sDecSep);
        PQclear(res);
        if(pvRow[iNamePos].vt == VT_LPSTR)
        {
            sNameBuf[0] = 0;
            if(strcmp(pvRow[iSchmPos].pcVal, "public") != 0)
            {
                sSchemaPtr = pvRow[iSchmPos].pcVal;
                strcpy(sNameBuf, pvRow[iSchmPos].pcVal);
                strcat(sNameBuf, ".");
            }
            else sSchemaPtr = NULL;
            strcat(sNameBuf, pvRow[iNamePos].pcVal);

            if(IsNotMetaTable(sNameBuf, true))
            {
                ptd = new CDbuTable(m_pConnStruct, sSchemaPtr,
                    pvRow[iNamePos].pcVal, false);
                ptd->SetIsView(true);
                AddTable(ptd);
            }
        }
        res = PQexec(m_pConnStruct->pConn, "fetch in viewcur");
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            WritePQErrorToLog("Fetch loop viewcur in 'CDbuTables::BuildFromConn' failed\r\n",
                m_pConnStruct->pConn);
            nrows = 0;
        }
        else nrows = PQntuples(res);
    }
    PQclear(res);

    for(int i = 0; i < ncols; i++) PgVariantClear(&pvRow[i]);
    free(pvRow);
    free(piTypes);

    res = PQexec(m_pConnStruct->pConn, "close viewcur");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        WritePQErrorToLog("Close viewcur in 'CDbuTables::BuildFromConn' failed\r\n",
            m_pConnStruct->pConn);
    }
    PQclear(res);

    bool bTrans = true;
    m_pConnStruct->iSysTrans--;
    if((m_pConnStruct->iSysTrans | m_pConnStruct->iGdoTrans) < 1)
    {
        res = PQexec(m_pConnStruct->pConn, "end");
        PQclear(res);
        bTrans = false;
    }

    //if(m_pConnStruct->bFtrPrepared)
    {
        SetSP(m_pConnStruct->pConn, bTrans);
        res = PQprepare(m_pConnStruct->pConn, "soidstmt",
            "select t1.oid from pg_class t1 inner join pg_namespace t2 \
on t1.relnamespace = t2.oid where t1.relname = $1 and t2.nspname = $2", 2, NULL);
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Prepare sOidStmt in 'CDbuTables::BuildFromConn' failed\r\n",
                m_pConnStruct->pConn);
            PQclear(res);
            RollbackSP(m_pConnStruct->pConn, bTrans);
            return;
        }
        PQclear(res);
        ReleaseSP(m_pConnStruct->pConn, bTrans);

        for(int i = 0; i < m_iDataLen; i++)
        {
            m_pTables[i]->RefreshFromConn("soidstmt", "stmtftr");
        }

        res = PQexec(m_pConnStruct->pConn, "DEALLOCATE soidstmt");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            WritePQErrorToLog("Deallocate sOidStmt in 'CDbuTables::BuildFromConn' failed\r\n",
                m_pConnStruct->pConn);
        }
        PQclear(res);
    }
    return;
}

void CDbuTables::UpdateAllTables(bool bVisible)
{
    for(int i = 0; i < m_iDataLen; i++)
    {
        m_pTables[i]->SetVisible(bVisible);
        m_pTables[i]->SaveTableMetadata();
    }
    return;
}
