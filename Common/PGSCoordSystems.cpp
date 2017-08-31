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

#include "PGSCoordSystems.hpp"
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include "VarUtils.hpp"
#include "GDOUtils.hpp"
#include "pg_const.h"

// we will initialize the table to be a little bit more that the number
// of currently known coordinate systems
const int giInitTableSize = 40;

const int pFieldSizes[giCSRowSize - 1] = {40, 4, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 2, 2, 8, 8, 2, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8,
    8, 2, 2, 2, 2, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 8, 8, 1, 8,
    1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 2, 2, 8, 0};

int ReadRow(PCSArray pCSTable, char *pBuf, int iCodeSize)
{
    PCSArrayRow pRow = (PCSArrayRow)malloc(giCSRowSize*sizeof(VARIANT));
    pRow[0].vt = VT_BSTR;
    pRow[0].bstrVal = SysAllocStringLen(NULL, iCodeSize);
    short ilen = 2*iCodeSize;
    int iRes = ilen;
    memcpy(pRow[0].bstrVal, pBuf, ilen);
    pRow[0].bstrVal[iCodeSize] = 0;
    pBuf += ilen;
    VariantInit(&pRow[1]);
    pRow[2].vt = VT_I4;
    pRow[2].lVal = 1;

    // The next code assure that each CS has the name in form EPSG:XXXX
    pRow[3].vt = VT_BSTR;
    pRow[3].bstrVal = SysAllocStringLen(NULL, iCodeSize + 5);
    wcscpy(pRow[3].bstrVal, L"EPSG:");
    wcscat(pRow[3].bstrVal, pRow[0].bstrVal);
    ilen = *(short*)pBuf;
    pBuf += 2;
    iRes += 2;
    if(ilen > 0)
    {
        pBuf += 2*ilen;
        iRes += 2*ilen;
    }

    for(int i = 3; i < giCSRowSize - 1; i++)
    {
        switch(pFieldSizes[i])
        {
        case 0:
            ilen = *(short*)pBuf;
            pBuf += 2;
            iRes += 2;
            if(ilen > 0)
            {
                pRow[i + 1].vt = VT_BSTR;
                pRow[i + 1].bstrVal = SysAllocStringLen(NULL, ilen - 1);
                memcpy(pRow[i + 1].bstrVal, pBuf, 2*ilen);
                //pRow[i + 1].bstrVal[ilen] = 0;
                pBuf += 2*ilen;
                iRes += 2*ilen;
            }
            else pRow[i + 1].vt = VT_NULL;
            break;
        case 1:
            if(*(unsigned char*)pBuf != 0xFF)
            {
                pRow[i + 1].vt = VT_UI1;
                pRow[i + 1].bVal = *(unsigned char*)pBuf;
            }
            else pRow[i + 1].vt = VT_NULL;
            pBuf++;
            iRes++;
            break;
        case 2:
            if(*(unsigned short*)pBuf != 0xFFFF)
            {
                pRow[i + 1].vt = VT_I2;
                pRow[i + 1].iVal = *(short*)pBuf;
            }
            else pRow[i + 1].vt = VT_NULL;
            pBuf += 2;
            iRes += 2;
            break;
        case 4:
            if(*(unsigned long*)pBuf != 0xFFFFFFFF)
            {
                pRow[i + 1].vt = VT_I4;
                pRow[i + 1].lVal = *(long*)pBuf;
            }
            else pRow[i + 1].vt = VT_NULL;
            pBuf += 4;
            iRes += 4;
            break;
        case 8:
            if((*(unsigned long*)pBuf != 0xFFFFFFFF) ||
                (*(unsigned long*)(&pBuf[4]) != 0xFFFFFFFF))
            {
                pRow[i + 1].vt = VT_R8;
                pRow[i + 1].dblVal = *(double*)pBuf;
            }
            else pRow[i + 1].vt = VT_NULL;
            pBuf += 8;
            iRes += 8;
            break;
        }
    }
    if(pCSTable->iDataLen >= pCSTable->iDataSize)
    {
        pCSTable->iDataSize += 16;
        pCSTable->pData = (PCSArrayRow*)realloc(pCSTable->pData,
            pCSTable->iDataSize*sizeof(PCSArrayRow));
    }
    pCSTable->pData[pCSTable->iDataLen++] = pRow;
    return(iRes);
}

void LoadCSTable(HINSTANCE hInstance, PCSArray pCSTable)
{
    pCSTable->iDataLen = 0;
    pCSTable->iDataSize = giInitTableSize;
    pCSTable->pData =
        (PCSArrayRow*)malloc(pCSTable->iDataSize*sizeof(PCSArrayRow));

    long dwsize = 0;
    HRSRC hrc = FindResource(hInstance, _T("COORDSYSTEMS"), _T("CSTABLE"));
    if(hrc) dwsize = SizeofResource(hInstance, hrc);
    if(dwsize > 0)
    {
        HGLOBAL hgb = LoadResource(hInstance, hrc);
        char *pData = (char*)LockResource(hgb);
        short *piCodeSize = (short*)pData;
        short *piColumnCount = (short*)&pData[2];
        if(*piColumnCount != giCSRowSize - 1) return; // some problem
        long dwPos = 4;
        dwsize -= 4;
        int iread;
        while(dwsize > 0)
        {
            iread = ReadRow(pCSTable, &pData[dwPos], *piCodeSize);
            dwPos += iread;
            dwsize -= iread;
        }
    }

    return;
}

void ClearCSTable(PCSArray pCSTable)
{
    int i = 0;
    PCSArrayRow pRow;
    while(i < pCSTable->iDataLen)
    {
        pRow = pCSTable->pData[i++];
        for(int j = 0; j < giCSRowSize; j++)
        {
            VariantClear(&pRow[j]);
        }
        free(pRow);
    }
    free(pCSTable->pData);
    return;
}

VARIANT *FindCSTableRow(PCSArray pCSTable, long lSrid)
{
    int i = 0;
    int id;
    bool bFound = false;
    VARIANT *pRes;
    while(!bFound && (i < pCSTable->iDataLen))
    {
        pRes = pCSTable->pData[i++];
        if(pRes[0].vt == VT_BSTR)
        {
            swscanf(pRes[0].bstrVal, L"%d", &id);
            bFound = (id == lSrid);
        }
    }
    return(bFound ? pRes : NULL);
}

void FldTypeToChar(short iType, long lSize, LPSTR sBuf, int iBufLen)
{
    switch(iType)
    {
    case 2:
        strcpy(sBuf, "\"char\"");
        break;
    case 3:
        strcpy(sBuf, "smallint");
        break;
    case 4:
        strcpy(sBuf, "integer");
        break;
    case 7:
        strcpy(sBuf, "double precision");
        break;
    case 10:
        sprintf(sBuf, "varchar(%ld)", lSize);
        break;
    case 15:
        //strcpy(sBuf, "uuid"); // no at this time
        strcpy(sBuf, "varchar(40)");
        break;
    }
    return;
}

void MarkUsedCSs(PGconn *pConn, PCSArray pSCTable)
{
    LPCSTR sSql = "select distinct(srid) from geometry_columns";
    PGresult *res = PQexecParams(pConn, sSql, 0, NULL, NULL, NULL, NULL, 1);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        PQclear(res);
        return;
    }

    long nrows = PQntuples(res);
    if(nrows < 1)
    {
        PQclear(res);
        return;
    }

    Oid iType;
    DescribeFieldTypes(res, 1, &iType);
    VARIANT vVal;
    VariantInit(&vVal);
    UUID uid;
    VARIANT *pRow;

    for(long li = 0; li < nrows; li++)
    {
        GetRecordBin(res, li, 1, &iType, &vVal, 0, 0, NULL, 0);
        pRow = FindCSTableRow(pSCTable, VarToLong(vVal));

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
        }
    }

    PQclear(res);
    return;
}

void UnmarkUsedCSs(PCSArray pSCTable)
{
    VARIANT *pRes;
    for(int i = 0; i < pSCTable->iDataLen; i++)
    {
        pRes = pSCTable->pData[i];
        VariantClear(&pRes[1]);
    }
    return;
}

int GetNamedCSGuidCount(PCSArray pSCTable)
{
    int iRes = 0;
    VARIANT *pRes;
    for(int i = 0; i < pSCTable->iDataLen; i++)
    {
        pRes = pSCTable->pData[i];
        if(pRes[1].vt == VT_BSTR) iRes++;
    }
    return(iRes);
}

Oid ShortTypeToOid(short iType)
{
    switch(iType)
    {
    case 2:
        return(CHAROID);
    case 3:
        return(INT2OID);
    case 4:
        return(INT4OID);
    case 7:
        return(FLOAT8OID);
    case 10:
    case 15:
        return(VARCHAROID);
    default:
        return(INT2OID);
    }
}

LPSTR GetParam(VARIANT vVal, short iDtType, int *piParLen, Oid *plDtType,
    long lEnc, PGeomInfo pgInfo)
{
    LPSTR psRes = NULL;
    *plDtType = ShortTypeToOid(iDtType);
    *piParLen = VarToBinaryLen(vVal, *plDtType, lEnc, pgInfo, NULL);
    if(*piParLen > 0)
    {
        psRes = (LPSTR)malloc(*piParLen + 1);
        VarToBinaryBuf(vVal, 0, lEnc, psRes, *piParLen, pgInfo, NULL, 0);
    }
    return(psRes);
}

LPSTR GetCSSelectString(PCSArray pSCTable, bool *pbNull, LPSTR *psParams,
    int *piParLens, Oid *plDataTypes, long lEnc, int iRows)
{
    *pbNull = true;
    LPSTR sRes = NULL;
    LPCSTR sFldNames[119] =
        {"CSGUID", "CSGUIDTYPE", "Name", "Description", "BaseStorageType",
        "Stor2CompMatrix1", "Stor2CompMatrix2", "Stor2CompMatrix3",
        "Stor2CompMatrix4", "Stor2CompMatrix5", "Stor2CompMatrix6",
        "Stor2CompMatrix7", "Stor2CompMatrix8", "Stor2CompMatrix9",
        "Stor2CompMatrix10", "Stor2CompMatrix11", "Stor2CompMatrix12",
        "Stor2CompMatrix13", "Stor2CompMatrix14", "Stor2CompMatrix15",
        "Stor2CompMatrix16", "HeightStorageType", "LonNormStorageOpt",
        "GeodeticDatum", "Ellipsoid", "EquatorialRadius",
        "InverseFlattening", "ProjAlgorithm", "AzimuthAngle", "FalseX",
        "FalseY", "Hemisphere", "LatOfOrigin", "LatOfTrueScale",
        "LonOfOrigin", "RadOfStandCircle", "ScaleReductFact", "StandPar1",
        "StandPar2", "Zone", "PathNumber", "RowNumber", "Satellite",
        "XAzDefOpt", "GeomHeightOfOrig", "GeomHeightOfPoint1",
        "GeomHeightOfPoint2", "LatOfPoint1", "LatOfPoint2", "LonOfPoint1",
        "LonOfPoint2", "ArgumentOfPerigee", "EarthRotPeriod",
        "FourierExpansionDegree", "NodesInSimpsonIntegration",
        "OrbEarthRotPeriodRatio", "OrbEcc", "OrbInclination", "OrbOff",
        "OrbPeriod", "OrbSemimajAxis", "OblMercDefMode", "LatOfMapCenter",
        "OblLamConfConDefMode", "RotNorthPoleLat", "RotNorthPoleLon",
        "GaussianLat", "SpherModel", "SpherRadius", "LatOfBasisPointA",
        "LatOfBasisPointB", "LatOfBasisPointC", "LonOfBasisPointA",
        "LonOfBasisPointB", "LonOfBasisPointC", "ChamTriOriginDefMode",
        "AngOrientationProjPlaneDefMode", "AzOfUpwardTilt", "FocalLength",
        "HeightAboveEllipAtNadir", "HeightOrigOfLocalHorizSys",
        "LatOrigOfLocalHorizSys", "LocationOfProjPlaneDefMode",
        "LonOrigOfLocalHorizSys", "PerspCenterGeocX", "PerspCenterGeocY",
        "PerspCenterGeocZ", "PerspCenterHeight", "PerspCenterLat",
        "PerspCenterLon", "PerspCenterXEast", "PerspCenterYNorth",
        "PerspCenterZUp", "RefCoordSysDefMode", "RotAboutXAxis",
        "RotAboutYAxis", "RotAboutZAxis", "SwingAng", "TiltAng",
        "ExtendProjMatrix1", "ExtendProjMatrix2", "ExtendProjMatrix3",
        "ExtendProjMatrix4", "ExtendProjMatrix5", "ExtendProjMatrix6",
        "ExtendProjMatrix7", "ExtendProjMatrix8", "ExtendProjMatrix9",
        "ExtendProjMatrix10", "ExtendProjMatrix11", "ExtendProjMatrix12",
        "ExtendProjMatrix13", "ExtendProjMatrix14", "ExtendProjMatrix15",
        "ExtendProjMatrix16", "VerticalDatum", "UndulationModel",
        "AverageUndulation", "NamedGeodeticDatum"};
    short tps[119] =
        {15, 4, 10, 10, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 2, 2, 3, 3, 7, 7, 3, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 3,
        3, 3, 3, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 7, 7, 7, 7, 7,
        7, 2, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 7, 7, 7,
        7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3, 7, 10};
    long sizes[119] =
        {40, 0, 100, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100};

    int iFldLen = 0;
    for(int i = 0; i < 119; i++) iFldLen += strlen(sFldNames[i]);

    char sBuf[128];
    if(iRows > 0)
    {
        *pbNull = false;
        VARIANT *pRow = NULL;

        /*int iTotLen = strlen("select cast($1 as ") + 16 + strlen(") as ") +
            118*(strlen(", cast($%d as ") + 3 + 16 + strlen(") as ")) + iFldLen +
            (iRows - 1)*(strlen(" union all select ") + strlen("$%d") + 3 +
            118*(strlen(", $%d") + 3));
        int iTotLen = 18 + 16 + 5 + 118*(15 + 3 + 16 + 5) + iFldLen +
            (iRows - 1)*(18 + 3 + 3 + 118*(3 + 3));*/
        int iTotLen = iFldLen + 4641 + 732*(iRows - 1);

        sRes = (LPSTR)malloc(iTotLen*sizeof(char));

        int i = 0;
        bool bFound = false;
        while(!bFound && (i < pSCTable->iDataLen))
        {
            pRow = pSCTable->pData[i++];
            bFound = (pRow[1].vt == VT_BSTR);
        }

        CGeomInfo cgInfo = {0, 0, 0, 0, 0};

        psParams[0] = GetParam(pRow[1], tps[0], &piParLens[0], &plDataTypes[0],
            lEnc, &cgInfo);
        strcpy(sRes, "select cast($1 as ");

        int kPar = 1;

        FldTypeToChar(tps[0], sizes[0], sBuf, 128);
        strcat(sRes, sBuf);
        strcat(sRes, ") as ");
        strcat(sRes, sFldNames[0]);
        for(int j = 1; j < 119; j++)
        {
            psParams[kPar] = GetParam(pRow[j + 1], tps[j], &piParLens[kPar],
                &plDataTypes[kPar], lEnc, &cgInfo);
            kPar++;
            sprintf(sBuf, ", cast($%d as ", kPar);
            strcat(sRes, sBuf);
            FldTypeToChar(tps[j], sizes[j], sBuf, 128);
            strcat(sRes, sBuf);
            strcat(sRes, ") as ");
            strcat(sRes, sFldNames[j]);
        }

        while(i < pSCTable->iDataLen)
        {
            pRow = pSCTable->pData[i++];
            if(pRow[1].vt == VT_BSTR)
            {
                strcat(sRes, " union all select ");
                psParams[kPar] = GetParam(pRow[1], tps[0], &piParLens[kPar],
                    &plDataTypes[kPar], lEnc, &cgInfo);
                kPar++;
                sprintf(sBuf, "$%d", kPar);
                strcat(sRes, sBuf);
                for(int j = 1; j < 119; j++)
                {
                    psParams[kPar] = GetParam(pRow[j + 1], tps[j],
                        &piParLens[kPar], &plDataTypes[kPar], lEnc, &cgInfo);
                    kPar++;
                    sprintf(sBuf, ", $%d", kPar);
                    strcat(sRes, sBuf);
                }
            }
        }
    }
    else
    {
        // fill all values with NULL and add filter WHERE 0 = 1;
        //sRes = (LPWSTR)malloc((iFldLen + 119*(strlen(", cast(null as ...) as ") + 32)*...)
        //sRes = (LPWSTR)malloc((iFldLen + 1900 + strlen("select  where 0 = 1"))*...)
        sRes = (LPSTR)malloc((iFldLen + 6426)*sizeof(char));
        strcpy(sRes, "select cast(null as ");
        FldTypeToChar(tps[0], sizes[0], sBuf, 128);
        strcat(sRes, sBuf);
        strcat(sRes, ") as ");
        strcat(sRes, sFldNames[0]);
        for(int j = 1; j < 119; j++)
        {
            strcat(sRes, ", cast(null as ");
            FldTypeToChar(tps[j], sizes[j], sBuf, 128);
            strcat(sRes, sBuf);
            strcat(sRes, ") as ");
            strcat(sRes, sFldNames[j]);
        }
        //strcat(sRes, " where 0 = 1");
    }
    return(sRes);
}

bool CSRowsMatch(VARIANT *pRow1, VARIANT *pRow2)
{
    bool bRes = true;
    bool bDoCompare;
    unsigned char bStorageType = 0;
    int iGeodDatum = 0, iUndulModel = 0;

    int i = 4;
    while(bRes && (i < giCSRowSize - 1))
    {
        bDoCompare = true;
        switch(i)
        {
        case 4:
            bStorageType = (unsigned char)VarToInt(pRow1[i]);
            break;
        case 22:
            bDoCompare = (bStorageType == 1);
            break;
        case 23:
            iGeodDatum = (int)VarToInt(pRow1[i]);
            break;
        case 24:
            bDoCompare = (iGeodDatum == 0);
            break;
        case 115:
            bDoCompare = false; // do not compare vertical datums in this version
            break;
        case 116:
            iUndulModel = (int)VarToInt(pRow1[i]);
            break;
        case 117:
            bDoCompare = (iUndulModel == 1);
            break;
        case 118:
            bDoCompare = (iGeodDatum == -2);
            break;
        }
        if((i > 27) && (i < 115)) bDoCompare = (bStorageType == 0);

        if(bDoCompare && ((pRow1[i].vt > VT_NULL) || (pRow2[i].vt > VT_NULL)))
        {
            switch(pFieldSizes[i])
            {
            case 0:
                if((pRow1[i].vt == VT_BSTR) && (pRow2[i].vt == VT_BSTR))
                {
                    bRes = (wcscmp(pRow1[i].bstrVal, pRow2[i].bstrVal) == 0);
                }
                else bRes = (pRow1[i].vt == pRow2[i].vt);
                break;
            case 1:
            case 2:
            case 4:
                bRes = (VarToLong(pRow1[i]) == VarToLong(pRow2[i]));
                break;
            case 8:
                bRes = (fabs(VarToDouble(pRow1[i]) - VarToDouble(pRow2[i])) < 0.0001);
                break;
            }
        }
        i++;
    }
    return(bRes);
}

VARIANT *FindCSTableRowByCS(PCSArray pCSTable, VARIANT *PCSRow)
{
    int i = 0;
    bool bFound = false;
    VARIANT *pRes;
    while(!bFound && (i < pCSTable->iDataLen))
    {
        pRes = pCSTable->pData[i++];
        bFound = CSRowsMatch(&pRes[1], PCSRow);
    }
    return(bFound ? pRes : NULL);
}

VARIANT *FindCSTableRowByCSGUID(PCSArray pCSTable, BSTR sCSGUID)
{
    int i = 0;
    bool bFound = false;
    VARIANT *pRes;
    while(!bFound && (i < pCSTable->iDataLen))
    {
        pRes = pCSTable->pData[i++];
        if(pRes[1].vt == VT_BSTR)
        {
            bFound = (wcsicmp(pRes[1].bstrVal, sCSGUID) == 0);
        }
    }
    return(bFound ? pRes : NULL);
}

unsigned long SRIDfromCSGUID(PCSArray pCSTable, BSTR sCSGUID)
{
    int i = 0;
    bool bFound = false;
    VARIANT *pRes;
    while(!bFound && (i < pCSTable->iDataLen))
    {
        pRes = pCSTable->pData[i++];
        if(pRes[1].vt == VT_BSTR)
        {
            bFound = (wcsicmp(pRes[1].bstrVal, sCSGUID) == 0);
        }
    }

    i = 0;
    if(bFound)
    {
        if(pRes[0].vt == VT_BSTR) swscanf(pRes[0].bstrVal, L"%d", &i);
    }

    return(i);
}

/*void UpdateCSGuids(PGconn *pConn, PCSArray pCSTable, LPSTR sCSTable)
{
    Recordset *pRS = CreateRecordset();
    Fields *pFlds = NULL;
    Field *pFld = NULL;
    VARIANT *pRow, pCSRow[119];
    UUID uid;

    VARIANT vSrc, vConn, vIdx;
    vSrc.vt = VT_BSTR;
    vSrc.bstrVal = SysAllocString(sCSTable);
    vConn.vt = VT_DISPATCH;
    vConn.pdispVal = pConn;

    HRESULT hres = pRS->Open(vSrc, vConn, adOpenStatic, adLockOptimistic,
        adCmdTable);
    SysFreeString(vSrc.bstrVal);

    if(hres == S_OK)
    {
        for(int i = 0; i < 119; i++) VariantInit(&pCSRow[i]);

        pRS->get_Fields(&pFlds);
        vIdx.vt = VT_I4;

        pRS->MoveFirst();
        VARIANT_BOOL beof;
        pRS->get_EOF(&beof);
        while(!beof)
        {
            for(int i = 0; i < 119; i++)
            {
                vIdx.lVal = i;
                pFlds->get_Item(vIdx, &pFld);
                pFld->get_Value(&pCSRow[i]);
            }
            pRow = FindCSTableRowByCS(pCSTable, pCSRow);
            if(pRow)
            {
                VariantCopy(&pRow[1], &pCSRow[0]);
            }

            pRS->MoveNext();
            pRS->get_EOF(&beof);

        }
        if(pFld) pFld->Release();
        pFlds->Release();
        pRS->Close();
    }
    pRS->Release();

    return;
}*/
