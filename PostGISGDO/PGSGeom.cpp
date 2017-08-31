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

#include "PGSGeom.hpp"
#include <stdio.h>
#include <tchar.h>
//#include <float.h>
#include <rpcdce.h>
//#include "../Common/PGtoGDOmaps.hpp"
#include "../Common/LogFiles.hpp"
#include "../Common/VarUtils.hpp"
#include "../Common/GDOtoEWKB.hpp"
#include "PostGIS.rh"

long EwkbErrToResIg(long lEwkbErr)
{
    switch(lEwkbErr)
    {
    case EWKB_INVALIDGEOMETRYTYPE:
        return(IDE_INVALIDGEOMETRYTYPE);
    case EWKB_TEXTNOTSUPPORTED:
        return(IDE_TEXTNOTSUPPORTED);
    case EWKB_RASTERNOTSUPPORTED:
        return(IDE_RASTERNOTSUPPORTED);
    default:
        return(0);
    }
}

LPSTR ParseSQLWithSpatialFilter(LPSTR Name, VARIANT SpatialOp,
    VARIANT SpatialGeometryFilter, LPSTR GeometryFieldName,
    unsigned long ulSrid, LPSTR *psGeomBuf, int *piGeomLen, int iParam)
{
    LPSTR sRes = NULL;
    int iTotalLen = strlen(Name);

    if(SpatialGeometryFilter.vt != (VT_ARRAY | VT_UI1))
    {
        sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
        strcpy(sRes, Name);
        return(sRes);
    }

    long lL, lU;
    SafeArrayGetLBound(SpatialGeometryFilter.parray, 1, &lL);
    SafeArrayGetUBound(SpatialGeometryFilter.parray, 1, &lU);
    unsigned char *pBuf;
    SafeArrayAccessData(SpatialGeometryFilter.parray, (void**)&pBuf);

    long lErr = 0;
    long lParams[8];
    long llen = GeomBlobtoEWKBLen(pBuf, 2, ulSrid, 0, lParams, &lErr);
    *psGeomBuf = (LPSTR)malloc((llen + 1)*sizeof(char));
    GeomBlobtoEWKB(pBuf, 2, ulSrid, 0, (unsigned char*)*psGeomBuf, lParams,
        &lErr);
    SafeArrayUnaccessData(SpatialGeometryFilter.parray);
    *piGeomLen = llen;

    if(llen > 0)
    {
        int iSpatOp = VarToInt(SpatialOp);
        int iFilterLen = strlen(GeometryFieldName);

        switch(iSpatOp)
        {
        case 1:
        case 2:
            iFilterLen += strlen("ST_Within('\"\"', $xxxx)");
            break;
        case 3:
            iFilterLen += strlen("ST_Intersects('\"\"', $xxxx)");
            break;
        case 4:
            iFilterLen += strlen("not ST_Intersects('\"\"', $xxxx)");
            break;
        case 5:
            iFilterLen += strlen("'\"\"' && $xxxx)");
            break;
        case 6:
            iFilterLen += strlen("ST_Contains('\"\"', $xxxx)");
            break;
        }

        LPSTR sFilter = (LPSTR)malloc((iFilterLen + 1)*sizeof(wchar_t));

        switch(iSpatOp)
        {
        case 1:
            sprintf(sFilter, "ST_Within(\"%s\", $%d)", GeometryFieldName, iParam);
            break;
        case 2:
            sprintf(sFilter, "ST_Within($%d, \"%s\")", iParam, GeometryFieldName);
            break;
        case 3:
            sprintf(sFilter, "ST_Intersects(\"%s\", $%d)", GeometryFieldName, iParam);
            break;
        case 4:
            sprintf(sFilter, "not ST_Intersects(\"%s\", $%d)", GeometryFieldName, iParam);
            break;
        case 5:
            sprintf(sFilter, "$%d && \"%s\"", iParam, GeometryFieldName);
            break;
        case 6:
            sprintf(sFilter, "ST_Contains($%d, \"%s\")", iParam, GeometryFieldName);
            break;
        }

        LPSTR sSelect;
        sSelect = strstr(Name, "SELECT ");
        if(!sSelect) sSelect = strstr(Name, "select ");
        if(!sSelect) sSelect = strstr(Name, "Select ");

        if(sSelect)
        {
            LPSTR sWhere = strstr(Name, " where ");
            if(!sWhere) sWhere = strstr(Name, " WHERE ");
            if(!sWhere) sWhere = strstr(Name, " Where ");

            if(sWhere)
            {
                sWhere += 7;
                iTotalLen += (iFilterLen + strlen(" and "));
                sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
                strncpy(sRes, Name, sWhere - Name);
                sRes[sWhere - Name] = 0;
                strcat(sRes, sFilter);
                strcat(sRes, " and ");
                strcat(sRes, sWhere);
            }
            else
            {
                LPSTR sOrderBy = strstr(Name, " order ");
                if(!sOrderBy) sOrderBy = strstr(Name, " ORDER ");
                if(!sOrderBy) sOrderBy = strstr(Name, " Order ");

                if(sOrderBy)
                {
                    LPSTR sTmp = sOrderBy + 6;
                    while((*sTmp == ' ') && (*sTmp != 0)) sTmp++;
                    if((*sTmp == 'b') || (*sTmp == 'B'))
                    {
                        sTmp++;
                        if((*sTmp != 'y') && (*sTmp != 'Y')) sOrderBy = NULL;
                    }
                    else sOrderBy = NULL;
                }

                if(sOrderBy)
                {
                    iTotalLen += (iFilterLen + strlen(" where "));
                    sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
                    strncpy(sRes, Name, sOrderBy - Name);
                    sRes[sOrderBy - Name] = 0;
                    strcat(sRes, " where ");
                    strcat(sRes, sFilter);
                    strcat(sRes, sOrderBy);
                }
                else
                {
                    iTotalLen += (iFilterLen + strlen(" where "));
                    sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
                    strcpy(sRes, Name);
                    strcat(sRes, " where ");
                    strcat(sRes, sFilter);
                }
            }
        }
        else
        {
            iTotalLen += (iFilterLen + strlen("select * from \"\" where "));
            sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
            strcpy(sRes, "select * from \"");
            strcat(sRes, Name);
            strcat(sRes, "\" where ");
            strcat(sRes, sFilter);
        }

        free(sFilter);
    }
    else
    {
        sRes = (LPSTR)malloc((iTotalLen + 1)*sizeof(char));
        strcpy(sRes, Name);
    }

#if DBGLEVEL > 1
    WriteLogFile(sRes, true);
    WriteLogFile("\r\n", false);
#endif
    return(sRes);
}
