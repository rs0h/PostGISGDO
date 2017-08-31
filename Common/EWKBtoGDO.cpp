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

#include "EWKBtoGDO.hpp"
#include <math.h>
#include "PGSTypes.hpp"
#include "../Common/LogFiles.hpp"
#include <stdio.h>

unsigned char GetByte(unsigned char **psBuf, int *piCurLen)
{
    unsigned char ucRes = **psBuf;
    (*psBuf)++;
    (*piCurLen)++;
    return(ucRes);
}

unsigned long GetGeomType(unsigned char **psBuf, unsigned char uiByteOrder,
    int *piCurLen)
{
    unsigned long ulRes = 0;
    unsigned char *pbyte = (unsigned char*)&ulRes;
    if(uiByteOrder)
    {
        for(int i = 0; i < 4; i++) pbyte[i] = GetByte(psBuf, piCurLen);
    }
    else
    {
        for(int i = 0; i < 4; i++) pbyte[3 - i] = GetByte(psBuf, piCurLen);
    }
    // EWKB has a strange last byte, containing what? Dimension?
    pbyte[3] = 0;
    return(ulRes);
}

unsigned long GetLong(unsigned char **psBuf, unsigned char uiByteOrder,
    int *piCurLen)
{
    unsigned long ulRes = 0;
    unsigned char *pbyte = (unsigned char*)&ulRes;
    if(uiByteOrder)
    {
        for(int i = 0; i < 4; i++) pbyte[i] = GetByte(psBuf, piCurLen);
    }
    else
    {
        for(int i = 0; i < 4; i++) pbyte[3 - i] = GetByte(psBuf, piCurLen);
    }
    return(ulRes);
}

double GetDouble(unsigned char **psBuf, unsigned char uiByteOrder, int *piCurLen)
{
    double dblRes = 0;
    unsigned char *pbyte = (unsigned char*)&dblRes;
    if(uiByteOrder)
    {
        for(int i = 0; i < 8; i++) pbyte[i] = GetByte(psBuf, piCurLen);
    }
    else
    {
        for(int i = 0; i < 8; i++) pbyte[7 - i] = GetByte(psBuf, piCurLen);
    }
    return(dblRes);
}

void GetTriple(unsigned char **psBuf, unsigned char ucByteOrder, int iDim,
    int *piCurLen, double *px)
{
    px[0] = GetDouble(psBuf, ucByteOrder, piCurLen);
    px[1] = GetDouble(psBuf, ucByteOrder, piCurLen);
    if(iDim > 2)
    {
        px[2] = GetDouble(psBuf, ucByteOrder, piCurLen);
        if(_isnan(px[2])) px[2] = -9999.0;
    }
    else px[2] = 0.0;
    if(iDim > 3)
    {
        *psBuf += 8;
        *piCurLen += 8;
    }
    return;
}

int GetArcGeomSize(double *dStart, double *dMiddle, double *dEnd)
{
    double dMag = (dEnd[0] - dStart[0])*(dEnd[0] - dStart[0]) +
        (dEnd[1] - dStart[1])*(dEnd[1] - dStart[1]) +
        (dEnd[2] - dStart[2])*(dEnd[2] - dStart[2]);
    double dPrec = gdPrecLim*sqrt(dMag);

    double alpha[3];
    alpha[0] = (dEnd[1] - dStart[1])*(dMiddle[2] - dStart[2]) -
        (dEnd[2] - dStart[2])*(dMiddle[1] - dStart[1]);
    alpha[1] = (dEnd[2] - dStart[2])*(dMiddle[0] - dStart[0]) -
        (dEnd[0] - dStart[0])*(dMiddle[2] - dStart[2]);
    alpha[2] = (dEnd[0] - dStart[0])*(dMiddle[1] - dStart[1]) -
        (dEnd[1] - dStart[1])*(dMiddle[0] - dStart[0]);
    if((fabs(alpha[0]) < dPrec) && (fabs(alpha[1]) < dPrec) &&
        (fabs(alpha[2]) < dPrec))
    {
        return(64);
    }

    double A[2][2];
    A[0][0] = 0.0;
    A[0][1] = 0.0;
    A[1][0] = 0.0;
    A[1][1] = 0.0;
    for(int i = 0; i < 3; i++)
    {
        A[0][0] += (dMiddle[i] - dStart[i])*(dMiddle[i] - dStart[i]);
        A[0][1] += (dMiddle[i] - dStart[i])*(dMiddle[i] - dEnd[i]);
        A[1][0] += (dMiddle[i] - dEnd[i])*(dMiddle[i] - dStart[i]);
        A[1][1] += (dMiddle[i] - dEnd[i])*(dMiddle[i] - dEnd[i]);
    }
    double D = A[0][0]*A[1][1] - A[0][1]*A[1][0];

    if(fabs(D) > dPrec) return(96); // arc geometry
    else return(64); // line geometry
}

unsigned char* WriteArcGeometry(unsigned char *pBuf, double *dStart,
    double *dMiddle, double *dEnd)
{
    double dMag = (dEnd[0] - dStart[0])*(dEnd[0] - dStart[0]) +
        (dEnd[1] - dStart[1])*(dEnd[1] - dStart[1]) +
        (dEnd[2] - dStart[2])*(dEnd[2] - dStart[2]);
    double dPrec = gdPrecLim*sqrt(dMag);

    double alpha[3];
    alpha[0] = (dEnd[1] - dStart[1])*(dMiddle[2] - dStart[2]) -
        (dEnd[2] - dStart[2])*(dMiddle[1] - dStart[1]);
    alpha[1] = (dEnd[2] - dStart[2])*(dMiddle[0] - dStart[0]) -
        (dEnd[0] - dStart[0])*(dMiddle[2] - dStart[2]);
    alpha[2] = (dEnd[0] - dStart[0])*(dMiddle[1] - dStart[1]) -
        (dEnd[1] - dStart[1])*(dMiddle[0] - dStart[0]);
    if((fabs(alpha[0]) < dPrec) && (fabs(alpha[1]) < dPrec) &&
        (fabs(alpha[2]) < dPrec))
    {
        memcpy(pBuf, &GID_LineGeometry, 16);
        pBuf += 16;
        *(double*)pBuf = dStart[0];
        pBuf += 8;
        *(double*)pBuf = dStart[1];
        pBuf += 8;
        *(double*)pBuf = dStart[2];
        pBuf += 8;
        *(double*)pBuf = dEnd[0];
        pBuf += 8;
        *(double*)pBuf = dEnd[1];
        pBuf += 8;
        *(double*)pBuf = dEnd[2];
        pBuf += 8;
        return(pBuf);
    }

    double A[2][2], B[2];
    A[0][0] = 0.0;
    A[0][1] = 0.0;
    A[1][0] = 0.0;
    A[1][1] = 0.0;
    B[0] = 0.0;
    B[1] = 0.0;
    for(int i = 0; i < 3; i++)
    {
        A[0][0] += (dMiddle[i] - dStart[i])*(dMiddle[i] - dStart[i]);
        A[0][1] += (dMiddle[i] - dStart[i])*(dMiddle[i] - dEnd[i]);
        A[1][0] += (dMiddle[i] - dEnd[i])*(dMiddle[i] - dStart[i]);
        A[1][1] += (dMiddle[i] - dEnd[i])*(dMiddle[i] - dEnd[i]);
        B[0] += (dMiddle[i]*dMiddle[i] + dStart[i]*dStart[i])/2.0 -
            dMiddle[i]*dStart[i];
        B[1] += (dMiddle[i]*dMiddle[i] + dEnd[i]*dEnd[i])/2.0 -
            dMiddle[i]*dEnd[i];
    }
    double D = A[0][0]*A[1][1] - A[0][1]*A[1][0];

    if(fabs(D) > dPrec)
    {
        memcpy(pBuf, &GID_ArcGeometry, 16);
        pBuf += 16;
        *(double*)pBuf = dStart[0];
        pBuf += 8;
        *(double*)pBuf = dStart[1];
        pBuf += 8;
        *(double*)pBuf = dStart[2];
        pBuf += 8;
        *(double*)pBuf = dEnd[0];
        pBuf += 8;
        *(double*)pBuf = dEnd[1];
        pBuf += 8;
        *(double*)pBuf = dEnd[2];
        pBuf += 8;

        double gamma[2];
        gamma[0] = (A[1][1]*B[0] - A[0][1]*B[1])/D;
        gamma[1] = (A[0][0]*B[1] - A[1][0]*B[0])/D;
        double S[3];
        double r = 0.0;
        for(int i = 0; i < 3; i++)
        {
            //S[i] = sMiddle[i] + gamma[0]*(xStart[i] - xMiddle[i]) +
            //    gamma[1]*(xEnd[i] - xMiddle[i]);
            //r += (sMiddle[i] - S[i])*(sMiddle[i] - S[i]);
            S[i] = gamma[0]*(dStart[i] - dMiddle[i]) +
                gamma[1]*(dEnd[i] - dMiddle[i]);
            r += S[i]*S[i];
            S[i] += dMiddle[i];
        }
        r = sqrt(r);
        double normal1[3], normal2[3], normal3[3];
        normal1[0] = (dStart[1] - S[1])*(dMiddle[2] - S[2]) -
            (dStart[2] - S[2])*(dMiddle[1] - S[1]);
        normal1[1] = (dStart[2] - S[2])*(dMiddle[0] - S[0]) -
            (dStart[0] - S[0])*(dMiddle[2] - S[2]);
        normal1[2] = (dStart[0] - S[0])*(dMiddle[1] - S[1]) -
            (dStart[1] - S[1])*(dMiddle[0] - S[0]);

        normal2[0] = (dMiddle[1] - S[1])*(dEnd[2] - S[2]) -
            (dMiddle[2] - S[2])*(dEnd[1] - S[1]);
        normal2[1] = (dMiddle[2] - S[2])*(dEnd[0] - S[0]) -
            (dMiddle[0] - S[0])*(dEnd[2] - S[2]);
        normal2[2] = (dMiddle[0] - S[0])*(dEnd[1] - S[1]) -
            (dMiddle[1] - S[1])*(dEnd[0] - S[0]);

        normal3[0] = (dStart[1] - S[1])*(dEnd[2] - S[2]) -
            (dStart[2] - S[2])*(dEnd[1] - S[1]);
        normal3[1] = (dStart[2] - S[2])*(dEnd[0] - S[0]) -
            (dStart[0] - S[0])*(dEnd[2] - S[2]);
        normal3[2] = (dStart[0] - S[0])*(dEnd[1] - S[1]) -
            (dStart[1] - S[1])*(dEnd[0] - S[0]);

        double norms[3];
        norms[0] = normal1[0]*normal1[0] + normal1[1]*normal1[1]
            + normal1[2]*normal1[2];
        norms[1] = normal2[0]*normal2[0] + normal2[1]*normal2[1]
            + normal2[2]*normal2[2];
        norms[2] = normal3[0]*normal3[0] + normal3[1]*normal3[1]
            + normal3[2]*normal3[2];

        int ori1 = GetOrient(normal1, normal2);
        if(ori1 > 0)
        {
            if(fabs(norms[1]) > fabs(norms[0]))
            {
                norms[1] = sqrt(norms[1]);
                *(double*)pBuf = normal2[0]/norms[1];
                pBuf += 8;
                *(double*)pBuf = normal2[1]/norms[1];
                pBuf += 8;
                *(double*)pBuf = normal2[2]/norms[1];
                pBuf += 8;
            }
            else
            {
                norms[0] = sqrt(norms[0]);
                *(double*)pBuf = normal1[0]/norms[0];
                pBuf += 8;
                *(double*)pBuf = normal1[1]/norms[0];
                pBuf += 8;
                *(double*)pBuf = normal1[2]/norms[0];
                pBuf += 8;
            }
            if((GetOrient(normal1, normal3) >= 0) &&
                (GetOrient(normal2, normal3) >= 0)) r = -r;
        }
        else
        {
            if(GetOrient(normal1, normal3) > 0)
            {
                norms[1] = sqrt(norms[1]);
                *(double*)pBuf = normal2[0]/norms[1];
                pBuf += 8;
                *(double*)pBuf = normal2[1]/norms[1];
                pBuf += 8;
                *(double*)pBuf = normal2[2]/norms[1];
                pBuf += 8;
            }
            else
            {
                norms[0] = sqrt(norms[0]);
                *(double*)pBuf = normal1[0]/norms[0];
                pBuf += 8;
                *(double*)pBuf = normal1[1]/norms[0];
                pBuf += 8;
                *(double*)pBuf = normal1[2]/norms[0];
                pBuf += 8;
            }
        }
        *(double*)pBuf = r;
        pBuf += 8;
    }
    else
    {
        memcpy(pBuf, &GID_LineGeometry, 16);
        pBuf += 16;
        *(double*)pBuf = dStart[0];
        pBuf += 8;
        *(double*)pBuf = dStart[1];
        pBuf += 8;
        *(double*)pBuf = dStart[2];
        pBuf += 8;
        *(double*)pBuf = dEnd[0];
        pBuf += 8;
        *(double*)pBuf = dEnd[1];
        pBuf += 8;
        *(double*)pBuf = dEnd[2];
        pBuf += 8;
    }

    return(pBuf);
}

int GetEWKBtoPointLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    (*plen) += 16; // 16 bytes for Geom GUID

    (*plen) += 48; // 24 bytes for location, 24 bytes for orientation
    int iRes = 8*iDim;

    return(iRes);
}

int EWKBtoPoint(unsigned char *sBuf, unsigned char *pBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    memcpy(pBuf, &GID_OrientedPointGeometry, 16);
    (*plen) += 16; // 16 bytes for Geom GUID
    pBuf += 16;

    (*plen) += 48; // 24 bytes for location, 24 bytes for orientation
    int iRes = 0;

    GetTriple(&sBuf, ucByteOrder, iDim, &iRes, (double*)pBuf);
    pBuf += 24;
    *(double*)pBuf = 1.0;
    pBuf += 8;
    *(double*)pBuf = 0.0;
    pBuf += 8;
    *(double*)pBuf = 0.0;
    pBuf += 8;

    return(iRes);
}

int GetEWKBtoLineStringLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder, bool bWriteGUID)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    if(bWriteGUID)
        (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for num points

    (*plen) += ulLen*24; // 24 bytes for each 3 doubles
    iRes += ulLen*8*iDim;

    return(iRes);
}

int EWKBtoLineString(unsigned char *sBuf, unsigned char *pBuf, int iDim, long *plen,
    unsigned char ucByteOrder, unsigned long *plItemCount)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoLineString\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    if(plItemCount) (*plItemCount) += ulLen;
    else
    {
        memcpy(pBuf, &GID_PolylineGeometry, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = ulLen;
        pBuf += 4;
        (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for num points
    }

    (*plen) += ulLen*24; // 24 bytes for each 3 doubles

    for(int i = 0; i < (int)ulLen; i++)
    {
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, (double*)pBuf);
        pBuf += 24;
    }

    return(iRes);
}

int GetEWKBtoPolygonLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    if(ulLen > 1) // boundary geometry
    {
        (*plen) += 16; // 16 bytes for Geom GUID

        unsigned long lulLen = GetLong(&sBuf, ucByteOrder, &iRes);
        int ilen = lulLen*8*iDim;

        // 4 bytes for boundary size, 16 bytes for polgon GUID,
        // 4 bytes for points count
        (*plen) += (24 + 24*lulLen);
        iRes += ilen;
        sBuf += ilen;

        // 4 bytes for collection size, 16 bytes for collection GUID,
        // 4 bytes for collection count
        (*plen) += 24;
        int i = 1;
        while(i < (int)ulLen)
        {
            lulLen = GetLong(&sBuf, ucByteOrder, &iRes);
            ilen = lulLen*8*iDim;

            // 4 bytes for polygon size, 16 bytes for polgon GUID,
            // 4 bytes for points count
            (*plen) += (24 + 24*lulLen);
            iRes += ilen;
            sBuf += ilen;
            i++;
        }
    }
    else if(ulLen == 1) // polygon geometry
    {
        (*plen) += 16; // 16 bytes for Geom GUID

        ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

        (*plen) += (4 + ulLen*24); // 4 bytes for num points, 24 bytes for each 3 doubles
        iRes += ulLen*8*iDim;
    }

    return(iRes);
}

int EWKBtoPolygon(unsigned char *sBuf, unsigned char *pBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    if(ulLen > 1) // boundary geometry
    {
        memcpy(pBuf, &GID_BoundaryGeometry, 16);
        pBuf += 16;
        (*plen) += 16; // 16 bytes for Geom GUID

        unsigned long lulLen = GetLong(&sBuf, ucByteOrder, &iRes);

        // 4 bytes for boundary size, 16 bytes for polgon GUID,
        // 4 bytes for points count
        *(unsigned long*)pBuf = (20 + 24*lulLen);
        pBuf += 4;
        (*plen) += (24 + 24*lulLen);

        memcpy(pBuf, &GID_PolygonGeometry, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = lulLen;
        pBuf += 4;

        for(int j = 0; j < (int)lulLen; j++)
        {
            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, (double*)pBuf);
            pBuf += 24;
        }

        unsigned char *pSaveBuf = pBuf;
        pBuf += 4;

        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = (ulLen - 1);
        pBuf += 4;

        unsigned long llsize = 20; // 16 bytes GUID, 4 bztes col count

        // 4 bytes for collection size, 16 bytes for collection GUID,
        // 4 bytes for collection count
        (*plen) += 24;
        int i = 1;
        while(i < (int)ulLen)
        {
            lulLen = GetLong(&sBuf, ucByteOrder, &iRes);

            // 4 bytes for polygon size, 16 bytes for polgon GUID,
            // 4 bytes for points count
            llsize += (24 + 24*lulLen);
            (*plen) += (24 + 24*lulLen);

            *(unsigned long*)pBuf = (20 + 24*lulLen);
            pBuf += 4;
            memcpy(pBuf, &GID_PolygonGeometry, 16);
            pBuf += 16;
            *(unsigned long*)pBuf = lulLen;
            pBuf += 4;

            for(int j = 0; j < (int)lulLen; j++)
            {
                GetTriple(&sBuf, ucByteOrder, iDim, &iRes, (double*)pBuf);
                pBuf += 24;
            }

            i++;
        }

        *(unsigned long*)pSaveBuf = llsize;
    }
    else if(ulLen == 1) // polygon geometry
    {
        memcpy(pBuf, &GID_PolygonGeometry, 16);
        (*plen) += 16; // 16 bytes for Geom GUID
        pBuf += 16;

        ulLen = GetLong(&sBuf, ucByteOrder, &iRes);
        *(unsigned long*)pBuf = ulLen;
        pBuf += 4;

        (*plen) += (4 + ulLen*24); // 4 bytes for num points, 24 bytes for each 3 doubles

        for(int i = 0; i < (int)ulLen; i++)
        {
            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, (double*)pBuf);
            pBuf += 24;
        }
    }

    return(iRes);
}

int GetEWKBtoMultiPointLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for collection count
        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            if(iType == 1)
            {
                ilen = GetEWKBtoPointLen(sBuf, iDim, &lsize, byteorder);
                sBuf += ilen;
                iRes += ilen;
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;

        if(iType == 1)
        {
            iRes += GetEWKBtoPointLen(sBuf, iDim, &lsize, byteorder);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int EWKBtoMultiPoint(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = lcnt;
        pBuf += 4;
        (*plen) += 20; // 16 vytes for geometry GUID amd 4 bytes for collection count

        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;
            if(iType == 1)
            {
                ilen = EWKBtoPoint(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                *(unsigned long*)pBuf = lsize;
                sBuf += ilen;
                iRes += ilen;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // point geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        if(iType == 1)
        {
            iRes += EWKBtoPoint(sBuf, pBuf, iDim, &lsize, byteorder);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int GetEWKBtoMultiLineStringLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for collection count
        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            if(iType == 2)
            {
                ilen = GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                sBuf += ilen;
                iRes += ilen;
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;

        if(iType == 2)
        {
            iRes += GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                true);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int EWKBtoMultiLineString(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = lcnt;
        pBuf += 4;
        (*plen) += 20; // 16 vytes for geometry GUID amd 4 bytes for collection count

        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;
            if(iType == 2)
            {
                ilen = EWKBtoLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                *(unsigned long*)pBuf = lsize;
                sBuf += ilen;
                iRes += ilen;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        if(iType == 2)
        {
            iRes += EWKBtoLineString(sBuf, pBuf, iDim, &lsize, byteorder,
                NULL);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int GetEWKBtoMultiPolygonLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for collection count
        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            if(iType == 3)
            {
                ilen = GetEWKBtoPolygonLen(sBuf, iDim, &lsize, byteorder);
                sBuf += ilen;
                iRes += ilen;
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // boundary or polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;

        if(iType == 3)
        {
            iRes += GetEWKBtoPolygonLen(sBuf, iDim, &lsize, byteorder);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int EWKBtoMultiPolygon(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = lcnt;
        pBuf += 4;
        (*plen) += 20; // 16 vytes for geometry GUID amd 4 bytes for collection count

        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            if(iType == 3)
            {
                ilen = EWKBtoPolygon(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                *(unsigned long*)pBuf = lsize;
                sBuf += ilen;
                iRes += ilen;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            else ilen = 0;
            i++;
        }
    }
    else if(lcnt == 1) // boundary or polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        if(iType == 3)
        {
            iRes += EWKBtoPolygon(sBuf, pBuf, iDim, &lsize, byteorder);
            (*plen) += lsize;
        }
    }
    return(iRes);
}

int GetEWKBtoCircularStringLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder, bool bWriteGUID)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    double xStart[3], xMiddle[3], xEnd[3];

    if(lcnt > 3) // composite polyline geometry
    {
        int k = lcnt/2;
        if(bWriteGUID)
        {
            (*plen) += 16; // 16 bytes for geometry GUID
            (*plen) += 4; // 4 bytes for count
        }

        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

        for(int i = 0; i < k; i++)
        {
            xStart[0] = xEnd[0];
            xStart[1] = xEnd[1];
            xStart[2] = xEnd[2];

            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xMiddle);
            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

            (*plen) += (4 + GetArcGeomSize(xStart, xMiddle, xEnd));
        }
    }
    else if(lcnt == 3) // arc geometry or composite polyline, if it is a circle
        // or can be also line geometry
    {
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xStart);
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xMiddle);
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

        if((fabs(xEnd[0] - xStart[0]) < gdPrecLim) &&
            (fabs(xEnd[1] - xStart[1]) < gdPrecLim))
        {
            if(bWriteGUID)
            {
                (*plen) += 16; // 16 bytes for geometry GUID
                (*plen) += 4; // 4 bytes for count
            }

            // the geometry is whole circle, so we must represent it as
            // composite polyline geometry with two arcs
            (*plen) += 200; // 2x4 bytes for item size and 2x96 for two arcs
        }
        else
        {
            // the geometry is a pure arc
            (*plen) += GetArcGeomSize(xStart, xMiddle, xEnd);
        }
    }
    return(iRes);
}

// if plItemCount is NULL, then we are not interested in it, which in turn
// means that we want to write also the base GUID
int EWKBtoCircularString(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder, unsigned long *plItemCount)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoCircularString\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);
    unsigned char *tmpBuf;

    double xStart[3], xMiddle[3], xEnd[3];

    if(lcnt > 3) // composite polyline geometry
    {
        int k = lcnt/2;

        if(plItemCount) (*plItemCount) += k;
        else
        {
            memcpy(pBuf, &GID_CompositePolylineGeometry, 16);
            pBuf += 16;
            (*plen) += 16; // 16 bytes for geometry GUID
            *(long*)pBuf = k;
            pBuf += 4;
            (*plen) += 4; // 4 bytes for count
        }

        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

        long llen;

        for(int i = 0; i < k; i++)
        {
            xStart[0] = xEnd[0];
            xStart[1] = xEnd[1];
            xStart[2] = xEnd[2];

            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xMiddle);
            GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

            tmpBuf = WriteArcGeometry(&pBuf[4], xStart, xMiddle, xEnd);
            llen = (tmpBuf - &pBuf[4]);
            *(long*)pBuf = llen;
            (*plen) += (4 + llen);
            pBuf = tmpBuf;
        }
    }
    else if(lcnt == 3) // arc geometry or composite polyline, if it is a circle
        // or can be also line geometry
    {
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xStart);
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xMiddle);
        GetTriple(&sBuf, ucByteOrder, iDim, &iRes, xEnd);

        if((fabs(xEnd[0] - xStart[0]) < gdPrecLim) &&
            (fabs(xEnd[1] - xStart[1]) < gdPrecLim))
        {
            // the geometry is whole circle, so we must represent it as
            // composite polyline geometry with two arcs
            if(plItemCount) (*plItemCount) += 2;
            else
            {
                memcpy(pBuf, &GID_CompositePolylineGeometry, 16);
                pBuf += 16;
                (*plen) += 16; // 16 bytes for geometry GUID
                *(unsigned long*)pBuf = 2;
                pBuf += 4;
                (*plen) += 4; // 4 bytes for count
            }

            (*plen) += 200; // 2x4 bytes for item size and 2x96 for two arcs

            *(unsigned long*)pBuf = 96;
            pBuf += 4;

            memcpy(pBuf, &GID_ArcGeometry, 16);
            pBuf += 16;
            *(double*)pBuf = xStart[0];
            pBuf += 8;
            *(double*)pBuf = xStart[1];
            pBuf += 8;
            *(double*)pBuf = xStart[2];
            pBuf += 8;
            *(double*)pBuf = xMiddle[0];
            pBuf += 8;
            *(double*)pBuf = xMiddle[1];
            pBuf += 8;
            *(double*)pBuf = xMiddle[2];
            pBuf += 8;

            double D = (xMiddle[0] - xStart[0])*(xMiddle[0] - xStart[0]) +
                (xMiddle[1] - xStart[1])*(xMiddle[1] - xStart[1]);
            double dnorm[3];
            double r = D;
            if(D > gdPrecLim)
            {
                r = sqrt(D);
                double vtmp[3];
                vtmp[0] = (xMiddle[0] - xStart[0])/r;
                vtmp[1] = (xStart[1] - xMiddle[1])/r;
                vtmp[2] = 0.0;

                dnorm[0] = vtmp[1]*(xMiddle[2] - xStart[2]);
                    // - vtmp[2]*(xMiddle[1] - xStart[1]) = 0
                dnorm[1] = // vtmp[2]*(xMiddle[0] - xStart[0]) = 0
                     -vtmp[0]*(xMiddle[2] - xStart[2]);
                dnorm[2] = vtmp[0]*(xMiddle[1] - xStart[1]) -
                     vtmp[1]*(xMiddle[0] - xStart[0]);
                D = dnorm[0]*dnorm[0] + dnorm[1]*dnorm[1] + dnorm[2]*dnorm[2];
                D = sqrt(D);
                dnorm[0] /= D;
                dnorm[1] /= D;
                dnorm[2] /= D;
            }
            else
            {
                dnorm[0] = 0.0;
                dnorm[1] = 0.0;
                dnorm[2] = 1.0;
            }

            r /= 2.0;

            *(double*)pBuf = dnorm[0];
            pBuf += 8;
            *(double*)pBuf = dnorm[1];
            pBuf += 8;
            *(double*)pBuf = dnorm[2];
            pBuf += 8;

            *(double*)pBuf = r;
            pBuf += 8;

            *(unsigned long*)pBuf = 96;
            pBuf += 4;

            memcpy(pBuf, &GID_ArcGeometry, 16);
            pBuf += 16;

            *(double*)pBuf = xMiddle[0];
            pBuf += 8;
            *(double*)pBuf = xMiddle[1];
            pBuf += 8;
            *(double*)pBuf = xMiddle[2];
            pBuf += 8;
            *(double*)pBuf = xStart[0];
            pBuf += 8;
            *(double*)pBuf = xStart[1];
            pBuf += 8;
            *(double*)pBuf = xStart[2];
            pBuf += 8;

            *(double*)pBuf = dnorm[0];
            pBuf += 8;
            *(double*)pBuf = dnorm[1];
            pBuf += 8;
            *(double*)pBuf = dnorm[2];
            pBuf += 8;

            *(double*)pBuf = r;
            pBuf += 8;
        }
        else
        {
            if(plItemCount) (*plItemCount) += 1;
            tmpBuf = WriteArcGeometry(pBuf, xStart, xMiddle, xEnd);
            (*plen) += (tmpBuf - pBuf);
            pBuf = tmpBuf;
        }
    }
    return(iRes);
}

int GetEWKBtoCompoundCurveLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder, bool bWriteGUID)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 1) // composite polyline
    {
        if(bWriteGUID)
            (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for collection count
        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 2:
                ilen = GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                sBuf += ilen;
                iRes += ilen;
                (*plen) += (4 + lsize); // 4 bytes for collection item size
                break;
            case 8:
                ilen = GetEWKBtoCircularStringLen(sBuf, iDim, &lsize, byteorder,
                    false);
                sBuf += ilen;
                iRes += ilen;
                (*plen) += (4 + lsize);
                break;
            default:
                ilen = 0;
            }
            i++;
        }
    }
    else if(lcnt == 1) // polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;

        switch(iType)
        {
        case 2:
            iRes += GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder, true);
            break;
        case 8:
            iRes += GetEWKBtoCircularStringLen(sBuf, iDim, &lsize, byteorder,
                bWriteGUID);
            break;
        }

        (*plen) += lsize;
    }
    return(iRes);
}

// if plItemCount is NULL, then we are not interested in it, which in turn
// means that we want to write also the base GUID
int EWKBtoCompoundCurve(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder, unsigned long *plItemCount)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoCompoundCurve\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType, ltruecount;
    int ilocDim;
    int iType;
    unsigned char *pSaveBuf = NULL;

    if(lcnt > 1) // composite polyline
    {
        if(!plItemCount)
        {
            memcpy(pBuf, &GID_CompositePolylineGeometry, 16);
            pBuf += 16;

            pSaveBuf = pBuf;
            ltruecount = 0;
            //*(unsigned long*)pBuf = lcnt;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for geometry GUID amd 4 bytes for collection count
        }

        int i = 0;
        int ilen = 1;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;
            switch(iType)
            {
            case 2:
                ilen = EWKBtoLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                *(unsigned long*)pBuf = lsize;
                sBuf += ilen;
                iRes += ilen;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize); // 4 bytes for collection item size
                ltruecount += 1;
                break;
            case 8:
                ilen = EWKBtoCircularString(sBuf, &pBuf[4], iDim, &lsize,
                    byteorder, &ltruecount);
                *(unsigned long*)pBuf = lsize;
                sBuf += ilen;
                iRes += ilen;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize);
                break;
            default:
                ilen = 0;
            }
            i++;
        }
        if(plItemCount) (*plItemCount) += ltruecount;
        else *(unsigned long*)pSaveBuf = ltruecount;
    }
    else if(lcnt == 1) // polyline geometry or arc
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;

        switch(iType)
        {
        case 2:
            iRes += EWKBtoLineString(sBuf, pBuf, iDim, &lsize, byteorder,
                NULL);
            if(plItemCount) (*plItemCount) += 1;
            break;
        case 8:
            iRes += EWKBtoCircularString(sBuf, pBuf, iDim, &lsize, byteorder,
                plItemCount);
            break;
        }

        (*plen) += lsize;
    }
    return(iRes);
}

int GetEWKBtoCurvePolygonLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(ulLen > 1) // boundary geometry
    {
        (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for boundary
            // size, 4 bytes for holes size are covered in the while loop

        // 16 bytes for collection GUID, 4 bytes for collection count
        (*plen) += 20;

        int i = 0;
        int ilen = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            (*plen) += 4; // 4 bytes for collection item size

            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 2:
                ilen = GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                    false);
                break;
            case 8:
                ilen = GetEWKBtoCircularStringLen(sBuf, iDim, &lsize, byteorder,
                    false);
                break;
            case 9:
                ilen = GetEWKBtoCompoundCurveLen(sBuf, iDim, &lsize, byteorder,
                    false);
                break;
            default:
                ilen = 0;
            }
            sBuf += ilen;
            iRes += ilen;
            (*plen) += (20 + lsize); // 4 bytes for collection item count
            i++;
        }
    }
    else if(ulLen == 1) // composite polygon geometry or polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 2:
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for count
            iRes += GetEWKBtoLineStringLen(sBuf, iDim, plen, byteorder, false);
            break;
        case 8:
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for count
            iRes += GetEWKBtoCircularStringLen(sBuf, iDim, plen, byteorder,
                false);
            break;
        case 9:
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for count
            iRes += GetEWKBtoCompoundCurveLen(sBuf, iDim, plen, byteorder,
                false);
            break;
        }
    }

    return(iRes);
}

int EWKBtoCurvePolygon(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoCurvePolygon\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    unsigned char *pSaveBuf = NULL, *pSaveBuf2 = NULL;
    unsigned long ltruecount = 0;

    if(ulLen > 1) // boundary geometry
    {
        memcpy(pBuf, &GID_BoundaryGeometry, 16);
        pBuf += 16;
        (*plen) += 16; // 16 bytes for Geom GUID

        pSaveBuf = pBuf;
        pBuf += 4;
        (*plen) += 4; // 4 bytes for boundary size

        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        lsize = 0;
        int ilen = 1;
        ltruecount = 0;

        switch(iType)
        {
        case 2:
            memcpy(pBuf, &GID_PolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf2 = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for count
            ilen = EWKBtoLineString(sBuf, pBuf, iDim, &lsize, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf2 = ltruecount;
            break;
        case 8:
            memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf2 = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count
            ilen = EWKBtoCircularString(sBuf, pBuf, iDim, &lsize, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf2 = ltruecount;
            break;
        case 9:
            memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf2 = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count
            ilen = EWKBtoCompoundCurve(sBuf, pBuf, iDim, &lsize, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf2 = ltruecount;
            break;
        default:
            ilen = 0;
        }
        *(unsigned long*)pSaveBuf = (20 + lsize);
        sBuf += ilen;
        iRes += ilen;
        pBuf += lsize;
        (*plen) += lsize;

        pSaveBuf = pBuf;
        pBuf += 4;
        (*plen) += 4;

        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = ulLen - 1;
        pBuf += 4;
        (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count
        int ltotsize = 20;

        int i = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            pSaveBuf2 = pBuf;
            pBuf += 4;
            (*plen) += 4;

            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;
            ltruecount = 0;

            switch(iType)
            {
            case 2:
                memcpy(pBuf, &GID_PolygonGeometry, 16);
                pBuf += 16;
                (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for count
                ilen = EWKBtoLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    &ltruecount);
                *(unsigned long*)pBuf = ltruecount;
                break;
            case 8:
                memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
                pBuf += 16;
                (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count
                ilen = EWKBtoCircularString(sBuf, &pBuf[4], iDim, &lsize,
                    byteorder, &ltruecount);
                *(unsigned long*)pBuf = ltruecount;
                break;
            case 9:
                memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
                pBuf += 16;
                (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count
                ilen = EWKBtoCompoundCurve(sBuf, pBuf, iDim, &lsize, byteorder,
                    &ltruecount);
                *(unsigned long*)pBuf = ltruecount;
                break;
            default:
                ilen = 0;
            }
            *(unsigned long*)pSaveBuf2 = (20 + lsize);
            sBuf += ilen;
            iRes += ilen;
            pBuf += (4 + lsize);
            (*plen) += lsize;
            ltotsize += (24 + lsize);
            i++;
        }

        *(unsigned long*)pSaveBuf = ltotsize;
    }
    else if(ulLen == 1) // composite polygon geometry or polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 2:
            memcpy(pBuf, &GID_PolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for point count

            ltruecount = 0;
            iRes += EWKBtoLineString(sBuf, pBuf, iDim, plen, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf = ltruecount;
            break;
        case 8:
            memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count

            ltruecount = 0;
            iRes += EWKBtoCircularString(sBuf, pBuf, iDim, plen, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf = ltruecount;
            break;
        case 9:
            memcpy(pBuf, &GID_CompositePolygonGeometry, 16);
            pBuf += 16;
            pSaveBuf = pBuf;
            pBuf += 4;
            (*plen) += 20; // 16 bytes for Geom GUID, 4 bytes for item count

            ltruecount = 0;
            iRes += EWKBtoCompoundCurve(sBuf, pBuf, iDim, plen, byteorder,
                &ltruecount);
            *(unsigned long*)pSaveBuf = ltruecount;
            break;
        }
    }

    return(iRes);
}

int GetEWKBtoMultiCurveLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(ulLen > 1) // geometry collection
    {
        // 16 bytes for collection GUID, 4 bytes for collection count
        (*plen) += 20;

        int i = 0;
        int ilen = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 2:
                ilen = GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            case 8:
                ilen = GetEWKBtoCircularStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            case 9:
                ilen = GetEWKBtoCompoundCurveLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            default:
                ilen = 0;
            }
            sBuf += ilen;
            iRes += ilen;
            (*plen) += (4 + lsize); // 4 bytes for collection item size
            i++;
        }
    }
    else if(ulLen == 1) // composite polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 2:
            iRes += GetEWKBtoLineStringLen(sBuf, iDim, plen, byteorder,
                true);
            break;
        case 8:
            iRes += GetEWKBtoCircularStringLen(sBuf, iDim, plen, byteorder,
                true);
            break;
        case 9:
            iRes += GetEWKBtoCompoundCurveLen(sBuf, iDim, plen, byteorder,
                true);
            break;
        }
    }

    return(iRes);
}

int EWKBtoMultiCurve(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoMultiCurve\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(ulLen > 1) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        (*plen) += 16; // 16 bytes for Geom GUID
        pBuf += 16;
        *(unsigned long*)pBuf = ulLen;
        pBuf += 4;
        (*plen) += 4;

        int i = 0;
        int ilen = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 2:
                ilen = EWKBtoLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                break;
            case 8:
                ilen = EWKBtoCircularString(sBuf, &pBuf[4], iDim, &lsize,
                    byteorder, NULL);
                break;
            case 9:
                ilen = EWKBtoCompoundCurve(sBuf, &pBuf[4], iDim, &lsize,
                    byteorder, NULL);
                break;
            default:
                ilen = 0;
            }
            *(unsigned long*)pBuf = lsize;
            sBuf += ilen;
            iRes += ilen;
            pBuf += (4 + lsize);
            (*plen) += (4 + lsize); // 4 bytes for collection item size
            i++;
        }
    }
    else if(ulLen == 1) // composite polyline geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 2:
            iRes += EWKBtoLineString(sBuf, pBuf, iDim, plen, byteorder, NULL);
            break;
        case 8:
            iRes += EWKBtoCircularString(sBuf, pBuf, iDim, plen, byteorder,
                NULL);
            break;
        case 9:
            iRes += EWKBtoCompoundCurve(sBuf, pBuf, iDim, plen, byteorder,
                NULL);
            break;
        }
    }

    return(iRes);
}

int GetEWKBtoMultiSurfaceLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(ulLen > 1) // geometry collection
    {
        (*plen) += 20; // 16 bytes for geometry GUID and 4 bytes for count

        int i = 0;
        int ilen = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 3:
                ilen = GetEWKBtoPolygonLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 13:
                ilen = GetEWKBtoCurvePolygonLen(sBuf, iDim, &lsize, byteorder);
                break;
            default:
                ilen = 0;
            }
            sBuf += ilen;
            iRes += ilen;
            (*plen) += (4 + lsize); // 4 bytes for collection item size
            i++;
        }
    }
    else if(ulLen == 1) // polygon or composite polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 3:
            iRes += GetEWKBtoPolygonLen(sBuf, iDim, plen, byteorder);
            break;
        case 13:
            iRes += GetEWKBtoCurvePolygonLen(sBuf, iDim, plen, byteorder);
            break;
        }
    }

    return(iRes);
}

int EWKBtoMultiSurface(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoMultiSurface\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long ulLen = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(ulLen > 1) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        (*plen) += 16; // 16 bytes for Geom GUID
        pBuf += 16;
        *(unsigned long*)pBuf = ulLen;
        pBuf += 4;
        (*plen) += 4;

        int i = 0;
        int ilen = 1;
        while((i < (int)ulLen) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 3:
                ilen = EWKBtoPolygon(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 13:
                ilen = EWKBtoCurvePolygon(sBuf, &pBuf[4], iDim, &lsize,
                    byteorder);
                break;
            default:
                ilen = 0;
            }
            *(unsigned long*)pBuf = lsize;
            sBuf += ilen;
            iRes += ilen;
            pBuf += (4 + lsize);
            (*plen) += (4 + lsize); // 4 bytes for collection item size
            i++;
        }
    }
    else if(ulLen == 1) // composite polygon geometry
    {
        byteorder = GetByte(&sBuf, &iRes);
        ulType = GetGeomType(&sBuf, byteorder, &iRes);

        ilocDim = ulType / 1000;
        iType = ulType - 1000*ilocDim;
        ilocDim += 2;

        switch(iType)
        {
        case 3:
            iRes += EWKBtoPolygon(sBuf, pBuf, iDim, plen, byteorder);
            break;
        case 13:
            iRes += EWKBtoCurvePolygon(sBuf, pBuf, iDim, plen, byteorder);
            break;
        }
    }

    return(iRes);
}

int GetEWKBtoGeometryCollectionLen(unsigned char *sBuf, int iDim, long *plen,
    unsigned char ucByteOrder)
{
    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;
    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 0) // geometry collection
    {
        (*plen) += 20; // 16 vytes for geometry GUID amd 4 bytes for collection count
        int ilen = 1;
        int i = 0;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 1:
                ilen = GetEWKBtoPointLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 2:
                ilen = GetEWKBtoLineStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            case 3:
                ilen = GetEWKBtoPolygonLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 4:
                ilen = GetEWKBtoMultiPointLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 5:
                ilen = GetEWKBtoMultiLineStringLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 6:
                ilen = GetEWKBtoMultiPolygonLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 7:
                ilen = GetEWKBtoGeometryCollectionLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 8:
                ilen = GetEWKBtoCircularStringLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            case 9:
                ilen = GetEWKBtoCompoundCurveLen(sBuf, iDim, &lsize, byteorder,
                    true);
                break;
            case 13:
                ilen = GetEWKBtoCurvePolygonLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 14:
                ilen = GetEWKBtoMultiCurveLen(sBuf, iDim, &lsize, byteorder);
                break;
            case 15:
                ilen = GetEWKBtoMultiSurfaceLen(sBuf, iDim, &lsize, byteorder);
                break;
            default:
                ilen = 0;
            }

            (*plen) += (4 + lsize);
            iRes += ilen;
            sBuf += ilen;
            i++;
        }
    }
    return(iRes);
}

int EWKBtoGeometryCollection(unsigned char *sBuf, unsigned char *pBuf, int iDim,
    long *plen, unsigned char ucByteOrder)
{
#if DBGLEVEL > 2
    WriteLogFile("EWKBtoGeometryCollection\r\n", true);
#endif // DBGLEVEL

    int iRes = 0;
    unsigned long lcnt = GetLong(&sBuf, ucByteOrder, &iRes);

    unsigned char byteorder;

    long lsize = 0;
    unsigned long ulType;
    int ilocDim;
    int iType;

    if(lcnt > 0) // geometry collection
    {
        memcpy(pBuf, &GID_GeometryCollection, 16);
        pBuf += 16;
        *(unsigned long*)pBuf = lcnt;
        pBuf += 4;
        (*plen) += 20; // 16 bytes for geometry GUID amd 4 bytes for collection count

        int ilen = 1;
        int i = 0;
        while((i < (int)lcnt) && (ilen > 0))
        {
            byteorder = GetByte(&sBuf, &iRes);
            ulType = GetGeomType(&sBuf, byteorder, &iRes);

            ilocDim = ulType / 1000;
            iType = ulType - 1000*ilocDim;
            ilocDim += 2;

            lsize = 0;

            switch(iType)
            {
            case 1:
                ilen = EWKBtoPoint(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 2:
                ilen = EWKBtoLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                break;
            case 3:
                ilen = EWKBtoPolygon(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 4:
                ilen = EWKBtoMultiPoint(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 5:
                ilen = EWKBtoMultiLineString(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 6:
                ilen = EWKBtoMultiPolygon(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 7:
                ilen = EWKBtoGeometryCollection(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 8:
                ilen = EWKBtoCircularString(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                break;
            case 9:
                ilen = EWKBtoCompoundCurve(sBuf, &pBuf[4], iDim, &lsize, byteorder,
                    NULL);
                break;
            case 13:
                ilen = EWKBtoCurvePolygon(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 14:
                ilen = EWKBtoMultiCurve(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            case 15:
                ilen = EWKBtoMultiSurface(sBuf, &pBuf[4], iDim, &lsize, byteorder);
                break;
            default:
                ilen = 0;
            }

            iRes += ilen;
            sBuf += ilen;

            if(ilen > 0)
            {
                *(unsigned long*)pBuf = lsize;
                pBuf += (4 + lsize);
                (*plen) += (4 + lsize); // 4 bytes for collection item size
            }
            i++;
        }
    }
    return(iRes);
}

long EWKBtoGeomBlobSize(unsigned char *pGeom, long lSize, int iDim)
{
    unsigned char *sCurBuf = pGeom;
/*#if DBGLEVEL > 0
    WriteLogFile("EWKBtoGeomBlob:\r\n", true);
    wchar_t wbuf[256];
    wcsncpy(wbuf, sBufBase, 255);
    wbuf[255] = 0;
    WriteLogFile(wbuf, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL*/

    int iCalcLen = 0;
    unsigned char byteorder = GetByte(&sCurBuf, &iCalcLen);

    unsigned long ulType = GetGeomType(&sCurBuf, byteorder, &iCalcLen);
    sCurBuf += 4; // 4-srid
    iCalcLen += 4; // 4-srid

    int iLocDim = ulType / 1000;
    int iType = ulType - 1000*iLocDim;
    iLocDim += 2;

    long lBlobSize = 0;
    switch(iType)
    {
    case 1:
        iCalcLen += GetEWKBtoPointLen(sCurBuf, iDim, &lBlobSize, byteorder);
        break;
    case 2:
        iCalcLen += GetEWKBtoLineStringLen(sCurBuf, iDim, &lBlobSize, byteorder,
            true);
        break;
    case 3:
        iCalcLen += GetEWKBtoPolygonLen(sCurBuf, iDim, &lBlobSize, byteorder);
        break;
    case 4:
        iCalcLen += GetEWKBtoMultiPointLen(sCurBuf, iDim, &lBlobSize, byteorder);
        break;
    case 5:
        iCalcLen += GetEWKBtoMultiLineStringLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    case 6:
        iCalcLen += GetEWKBtoMultiPolygonLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    case 7:
        iCalcLen += GetEWKBtoGeometryCollectionLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    case 8:
        iCalcLen += GetEWKBtoCircularStringLen(sCurBuf, iDim, &lBlobSize,
            byteorder, true);
        break;
    case 9:
        iCalcLen += GetEWKBtoCompoundCurveLen(sCurBuf, iDim, &lBlobSize,
            byteorder, true);
        break;
    case 13:
        iCalcLen += GetEWKBtoCurvePolygonLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    case 11:
    case 14:
        iCalcLen += GetEWKBtoMultiCurveLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    case 15:
        iCalcLen += GetEWKBtoMultiSurfaceLen(sCurBuf, iDim, &lBlobSize,
            byteorder);
        break;
    }

    if((iCalcLen > lSize) || (lBlobSize < 1)) return(0);
    return(lBlobSize);
}

void EWKBtoGeomBlob(unsigned char *pGeom, long lSize, int iDim,
    unsigned char *pGDOBuf)
{
    unsigned char *sCurBuf = pGeom;
/*#if DBGLEVEL > 0
    WriteLogFile("EWKBtoGeomBlob:\r\n", true);
    wchar_t wbuf[256];
    wcsncpy(wbuf, sBufBase, 255);
    wbuf[255] = 0;
    WriteLogFile(wbuf, false);
    WriteLogFile("\r\n", false);
#endif // DBGLEVEL*/

    int iCalcLen = 0;
    unsigned char byteorder = GetByte(&sCurBuf, &iCalcLen);

    unsigned long ulType = GetGeomType(&sCurBuf, byteorder, &iCalcLen);
    sCurBuf += 4; // 4-srid
    iCalcLen += 4; // 4-srid

    int iLocDim = ulType / 1000;
    int iType = ulType - 1000*iLocDim;
    iLocDim += 2;

    long lBlobSize = 0;
    switch(iType)
    {
    case 1:
        EWKBtoPoint(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 2:
        EWKBtoLineString(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder, NULL);
        break;
    case 3:
        EWKBtoPolygon(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 4:
        EWKBtoMultiPoint(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 5:
        EWKBtoMultiLineString(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 6:
        EWKBtoMultiPolygon(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 7:
        EWKBtoGeometryCollection(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 8:
        EWKBtoCircularString(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder, NULL);
        break;
    case 9:
        EWKBtoCompoundCurve(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder, NULL);
        break;
    case 13:
        EWKBtoCurvePolygon(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 11:
    case 14:
        EWKBtoMultiCurve(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    case 15:
        EWKBtoMultiSurface(sCurBuf, pGDOBuf, iDim, &lBlobSize, byteorder);
        break;
    }
    return;
}
