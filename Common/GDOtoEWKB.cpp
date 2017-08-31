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

#include "GDOtoEWKB.hpp"
#include "PGSTypes.hpp"
#include <tchar.h>
#include <math.h>
#include "LogFiles.hpp"
#include <stdio.h>
#include <limits>

//const double pi = 3.1415926535897932384626433832795;

int WriteByte(unsigned char **psBuf, unsigned char bnum)
{
    *(*psBuf)++ = bnum;
    return(1);
}

int WriteLong(unsigned char **psBuf, unsigned long lnum)
{
    unsigned char *pc = (unsigned char*)&lnum;
    *(*psBuf)++ = pc[0];
    *(*psBuf)++ = pc[1];
    *(*psBuf)++ = pc[2];
    *(*psBuf)++ = pc[3];
    return(4);
}

int WriteGeomType(unsigned char **psBuf, unsigned long lType,
    unsigned char bLevel)
{
    unsigned char *pc = (unsigned char*)&lType;
    *(*psBuf)++ = pc[0];
    *(*psBuf)++ = pc[1];
    *(*psBuf)++ = pc[2];
    *(*psBuf)++ = bLevel;
    return(4);
}

int WriteDouble(unsigned char **psBuf, double dnum)
{
    unsigned char *pc = (unsigned char*)&dnum;
    *(*psBuf)++ = pc[0];
    *(*psBuf)++ = pc[1];
    *(*psBuf)++ = pc[2];
    *(*psBuf)++ = pc[3];
    *(*psBuf)++ = pc[4];
    *(*psBuf)++ = pc[5];
    *(*psBuf)++ = pc[6];
    *(*psBuf)++ = pc[7];
    return(8);
}

int WriteCoordTriple(unsigned char **ppBuf, unsigned char **psBuf, int iDim)
{
    double x = *(double*)(*ppBuf);
    (*ppBuf) += 8;
    double y = *(double*)(*ppBuf);
    (*ppBuf) += 8;
    double z = *(double*)(*ppBuf);
    (*ppBuf) += 8;

    if(fabs(z + 9999.0) < 0.5) z = std::numeric_limits<double>::quiet_NaN();

    int iRes = WriteDouble(psBuf, x);
    iRes += WriteDouble(psBuf, y);
    if(iDim > 2) iRes += WriteDouble(psBuf, z);
    if(iDim > 3) iRes += WriteDouble(psBuf, 0.0);

    return(iRes);
}

int GetCoordSize(int iDim)
{
    //int iRes = 32;
    //if(iDim > 2) iRes += 16;
    //if(iDim > 3) iRes += 16;
    //return(iRes);
    return(8*iDim);
}

void GetTriple(unsigned char **ppBuf, double *px)
{
    px[0] = *(double*)(*ppBuf);
    (*ppBuf) += 8;
    px[1] = *(double*)(*ppBuf);
    (*ppBuf) += 8;
    px[2] = *(double*)(*ppBuf);
    (*ppBuf) += 8;
    return;
}

int WriteTriple(double *px, unsigned char **psBuf, int iDim)
{
    int iRes = WriteDouble(psBuf, px[0]);
    iRes += WriteDouble(psBuf, px[1]);
    if(iDim > 2) iRes += WriteDouble(psBuf, px[2]);
    if(iDim > 3) iRes += WriteDouble(psBuf, 0.0);

    return(iRes);
}

int GetStrokeArcCount(unsigned char *pBuf, bool bWriteFirstPoint)
{
    double xStart[3], xEnd[3], xNorm[3], dRad;
    GetTriple(&pBuf, xStart);
    GetTriple(&pBuf, xEnd);
    GetTriple(&pBuf, xNorm);
    dRad = *(double*)pBuf;

    //double x0[3]
    double n0[3], n1[3], n2[3];
    //x0[0] = (xStart[0] + xEnd[0])/2.0;
    //x0[1] = (xStart[1] + xEnd[1])/2.0;
    //x0[2] = (xStart[2] + xEnd[2])/2.0;
    n0[0] = xEnd[0] - xStart[0];
    n0[1] = xEnd[1] - xStart[1];
    n0[2] = xEnd[2] - xStart[2];
    double norm0 = n0[0]*n0[0] + n0[1]*n0[1] + n0[2]*n0[2];
    if(norm0 < gdPrecLim) return(0);

    double dd = dRad*dRad - norm0/4.0;
    if(dd < -gdPrecLim) return(0);

    double xr = 0.0;
    if(dd > gdPrecLim) xr = sqrt(dd);
    else dd = 0.0;

    double alpha, r;
    norm0 = sqrt(norm0);

    if(dRad > 0)
    {
        r = dRad;
        xr = -xr;
        alpha = (M_PI - asin(norm0/2.0/dRad));
    }
    else
    {
        r = -dRad;
        alpha = asin(norm0/2.0/r);
    }
    // xr now holds th y-coordinate of the center in rotated system
    // alpha is half of the sweep angle

    int nstrokes = (int)floor(0.5 + 32.0*alpha/M_PI) + 1;

    n0[0] /= norm0;
    n0[1] /= norm0;
    n0[2] /= norm0;

    double alph = xNorm[0]*n0[0] + xNorm[1]*n0[1] + xNorm[2]*n0[2];
    n1[0] = xNorm[0] - alph*n0[0];
    n1[1] = xNorm[1] - alph*n0[1];
    n1[2] = xNorm[2] - alph*n0[2];

    double norm1 = n1[0]*n1[0] + n1[1]*n1[1] + n1[2]*n1[2];
    if(norm1 < gdPrecLim) return(0);

    norm1 = sqrt(norm1);
    n1[0] /= norm1;
    n1[1] /= norm1;
    n1[2] /= norm1;

    n2[0] = n1[1]*n0[2] - n1[2]*n0[1];
    n2[1] = n1[2]*n0[0] - n1[0]*n0[2];
    n2[2] = n1[0]*n0[1] - n1[1]*n0[0];

    double norm2 = n2[0]*n2[0] + n2[1]*n2[1] + n2[2]*n2[2];
    if(norm2 < gdPrecLim) return(0);

    if(bWriteFirstPoint) nstrokes++;

    return(nstrokes);
}

int GetStrokeArcLen(double *pxStart, double *pxEnd, double *pxNorm, double dRad,
    int iDim, bool bWriteFirstPoint, bool bWriteCount)
{
    //double x0[3]
    double n0[3], n1[3], n2[3];
    //x0[0] = (pxStart[0] + pxEnd[0])/2.0;
    //x0[1] = (pxStart[1] + pxEnd[1])/2.0;
    //x0[2] = (pxStart[2] + pxEnd[2])/2.0;
    n0[0] = pxEnd[0] - pxStart[0];
    n0[1] = pxEnd[1] - pxStart[1];
    n0[2] = pxEnd[2] - pxStart[2];
    double norm0 = n0[0]*n0[0] + n0[1]*n0[1] + n0[2]*n0[2];
    if(norm0 < gdPrecLim) return(0);

    double dd = dRad*dRad - norm0/4.0;
    if(dd < -gdPrecLim) return(0);

    double xr = 0.0;
    if(dd > gdPrecLim) xr = sqrt(dd);
    else dd = 0.0;

    double alpha, r;
    norm0 = sqrt(norm0);

    if(dRad > 0)
    {
        r = dRad;
        xr = -xr;
        alpha = (M_PI - asin(norm0/2.0/dRad));
    }
    else
    {
        r = -dRad;
        alpha = asin(norm0/2.0/r);
    }
    // xr now holds th y-coordinate of the center in rotated system
    // alpha is half of the sweep angle

    int nstrokes = (int)floor(0.5 + 32.0*alpha/M_PI) + 1;

    n0[0] /= norm0;
    n0[1] /= norm0;
    n0[2] /= norm0;

    double alph = pxNorm[0]*n0[0] + pxNorm[1]*n0[1] + pxNorm[2]*n0[2];
    n1[0] = pxNorm[0] - alph*n0[0];
    n1[1] = pxNorm[1] - alph*n0[1];
    n1[2] = pxNorm[2] - alph*n0[2];

    double norm1 = n1[0]*n1[0] + n1[1]*n1[1] + n1[2]*n1[2];
    if(norm1 < gdPrecLim) return(0);

    norm1 = sqrt(norm1);
    n1[0] /= norm1;
    n1[1] /= norm1;
    n1[2] /= norm1;

    n2[0] = n1[1]*n0[2] - n1[2]*n0[1];
    n2[1] = n1[2]*n0[0] - n1[0]*n0[2];
    n2[2] = n1[0]*n0[1] - n1[1]*n0[0];

    double norm2 = n2[0]*n2[0] + n2[1]*n2[1] + n2[2]*n2[2];
    if(norm2 < gdPrecLim) return(0);

    if(bWriteFirstPoint) nstrokes++;
    int iRes = nstrokes*GetCoordSize(iDim);
    if(bWriteCount) iRes += 4;

    return(iRes);
}

int StrokeArc(double *pxStart, double *pxEnd, double *pxNorm, double dRad,
    unsigned char **psBuf, int iDim, bool bWriteFirstPoint, bool bWriteCount)
{
    double x0[3], n0[3], n1[3], n2[3];
    x0[0] = (pxStart[0] + pxEnd[0])/2.0;
    x0[1] = (pxStart[1] + pxEnd[1])/2.0;
    x0[2] = (pxStart[2] + pxEnd[2])/2.0;
    n0[0] = pxEnd[0] - pxStart[0];
    n0[1] = pxEnd[1] - pxStart[1];
    n0[2] = pxEnd[2] - pxStart[2];
    double norm0 = n0[0]*n0[0] + n0[1]*n0[1] + n0[2]*n0[2];
    if(norm0 < gdPrecLim) return(0);

    double dd = dRad*dRad - norm0/4.0;
    if(dd < -gdPrecLim) return(0);

    double xr = 0.0;
    if(dd > gdPrecLim) xr = sqrt(dd);
    else dd = 0.0;

    double alpha, r;
    norm0 = sqrt(norm0);

    if(dRad > 0)
    {
        r = dRad;
        xr = -xr;
        alpha = (M_PI - asin(norm0/2.0/dRad));
    }
    else
    {
        r = -dRad;
        alpha = asin(norm0/2.0/r);
    }
    // xr now holds th y-coordinate of the center in rotated system
    // alpha is half of the sweep angle

    int nstrokes = (int)floor(0.5 + 32.0*alpha/M_PI) + 1;

    n0[0] /= norm0;
    n0[1] /= norm0;
    n0[2] /= norm0;

    double alph = pxNorm[0]*n0[0] + pxNorm[1]*n0[1] + pxNorm[2]*n0[2];
    n1[0] = pxNorm[0] - alph*n0[0];
    n1[1] = pxNorm[1] - alph*n0[1];
    n1[2] = pxNorm[2] - alph*n0[2];

    double norm1 = n1[0]*n1[0] + n1[1]*n1[1] + n1[2]*n1[2];
    if(norm1 < gdPrecLim) return(0);

    norm1 = sqrt(norm1);
    n1[0] /= norm1;
    n1[1] /= norm1;
    n1[2] /= norm1;

    n2[0] = n1[1]*n0[2] - n1[2]*n0[1];
    n2[1] = n1[2]*n0[0] - n1[0]*n0[2];
    n2[2] = n1[0]*n0[1] - n1[1]*n0[0];

    double norm2 = n2[0]*n2[0] + n2[1]*n2[1] + n2[2]*n2[2];
    if(norm2 < gdPrecLim) return(0);

    norm2 = sqrt(norm2);
    n2[0] /= norm2;
    n2[1] /= norm2;
    n2[2] /= norm2;

    int iRes = 0;
    if(bWriteCount) iRes += WriteLong(psBuf, (nstrokes + 1));
    double ai, x, y, pxMiddle[3];
    if(bWriteFirstPoint) iRes += WriteTriple(pxStart, psBuf, iDim);
    for(int i = 0; i < nstrokes - 1; i++)
    {
        ai = 2*(i + 1)*alpha/nstrokes - alpha;
        x = r*sin(ai);
        y = xr - r*cos(ai);
        pxMiddle[0] = x*n0[0] + y*n2[0] + x0[0];
        pxMiddle[1] = x*n0[1] + y*n2[1] + x0[1];
        pxMiddle[2] = x*n0[2] + y*n2[2] + x0[2];
        iRes += WriteTriple(pxMiddle, psBuf, iDim);
    }
    iRes += WriteTriple(pxEnd, psBuf, iDim);

    return(iRes);
}

bool GetArcMidPoint(double *pxStart, double *pxMiddle, double *pxEnd,
    double *pxNorm, double dRad)
{
    double x0[3], n0[3], n1[3], n2[3];
    x0[0] = (pxStart[0] + pxEnd[0])/2.0;
    x0[1] = (pxStart[1] + pxEnd[1])/2.0;
    x0[2] = (pxStart[2] + pxEnd[2])/2.0;
    n0[0] = pxEnd[0] - pxStart[0];
    n0[1] = pxEnd[1] - pxStart[1];
    n0[2] = pxEnd[2] - pxStart[2];
    double norm0 = n0[0]*n0[0] + n0[1]*n0[1] + n0[2]*n0[2];
    if(norm0 < gdPrecLim) return(false);

    double dd = dRad*dRad - norm0/4.0;
    if(dd < -gdPrecLim) return(false);

    double xs = 0.0;
    if(dd > gdPrecLim) xs = sqrt(dd);

    if(dRad > 0) xs = -dRad - xs;
    else xs += dRad;
    // xs now holds th y-coordinate of mid point in rotated system

    norm0 = sqrt(norm0);
    n0[0] /= norm0;
    n0[1] /= norm0;
    n0[2] /= norm0;

    double alph = pxNorm[0]*n0[0] + pxNorm[1]*n0[1] + pxNorm[2]*n0[2];
    n1[0] = pxNorm[0] - alph*n0[0];
    n1[1] = pxNorm[1] - alph*n0[1];
    n1[2] = pxNorm[2] - alph*n0[2];

    double norm1 = n1[0]*n1[0] + n1[1]*n1[1] + n1[2]*n1[2];
    if(norm1 < gdPrecLim) return(false);

    norm1 = sqrt(norm1);
    n1[0] /= norm1;
    n1[1] /= norm1;
    n1[2] /= norm1;

    n2[0] = n1[1]*n0[2] - n1[2]*n0[1];
    n2[1] = n1[2]*n0[0] - n1[0]*n0[2];
    n2[2] = n1[0]*n0[1] - n1[1]*n0[0];

    double norm2 = n2[0]*n2[0] + n2[1]*n2[1] + n2[2]*n2[2];
    if(norm2 < gdPrecLim) return(false);

    norm2 = sqrt(norm2);
    n2[0] /= norm2;
    n2[1] /= norm2;
    n2[2] /= norm2;

    pxMiddle[0] = xs*n2[0] + x0[0];
    pxMiddle[1] = xs*n2[1] + x0[1];
    pxMiddle[2] = xs*n2[2] + x0[2];

    return(true);
}

unsigned char GetCollectionTypeMask(unsigned char *pBuf, long lLen,
    int iPsgType)
// if there is a line or polyline adds 1
// if there is a circular arc, adds 2
{
    unsigned char cRes = 0;
    unsigned long lSize, len;
    GUID guidBuf;

    for(int i = 0; i < lLen; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        memcpy(&guidBuf, pBuf, 16);
        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            cRes |= 1;
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            cRes |= 1;
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            cRes |= 2;
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            len = *(unsigned long*)(&pBuf[16]);
            cRes |= GetCollectionTypeMask(&pBuf[20], len, iPsgType);
        }
        pBuf += lSize;
    }
    switch(iPsgType)
    {
    case 2:
    case 3:
    case 5:
    case 6:
        cRes = 1;
        break;
    case 8:
        cRes = 2;
        break;
    }
    return(cRes);
}

long GetCollectionItemCount(unsigned char *pBuf, long lLen,
    unsigned char bTypeMask)
{
    long lRes = 0;
    unsigned long lSize, len;
    GUID guidBuf;
    int i;

    if(bTypeMask == 1)
    {
        if(lLen > 0) lRes = 1;

        i = 0;
        while(i < lLen)
        {
            lSize = *(unsigned long*)pBuf;
            pBuf += 4;
            memcpy(&guidBuf, pBuf, 16);
            if(IsEqualGUID(guidBuf, GID_LineGeometry))
            {
                lRes++;
            }
            else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
            {
                len = *(unsigned long*)(&pBuf[16]);
                lRes += (len - 1);
            }
            else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
            {
                lRes += GetStrokeArcCount(&pBuf[16], false);
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
            {
                len = *(unsigned long*)(&pBuf[16]);
                lRes += (GetCollectionItemCount(&pBuf[20], len, bTypeMask) - 1);
            }
            pBuf += lSize;
            i++;
        }
    }
    else if(bTypeMask > 1)
    {
        if(lLen > 0) lRes = 1;

        i = 0;
        while(i < lLen)
        {
            lSize = *(unsigned long*)pBuf;
            pBuf += 4;
            memcpy(&guidBuf, pBuf, 16);
            if(IsEqualGUID(guidBuf, GID_LineGeometry))
            {
                lRes += 2;
            }
            else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
            {
                len = *(unsigned long*)(&pBuf[16]);
                lRes += 2*(len - 1);
            }
            else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
            {
                lRes += 2;
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
            {
                len = *(unsigned long*)(&pBuf[16]);
                lRes += (GetCollectionItemCount(&pBuf[20], len, bTypeMask) - 1);
            }
            pBuf += lSize;
            i++;
        }
    }
    return(lRes);
}

int GetCollectionNextLineLen(unsigned char **ppBuf, int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        (*ppBuf) += 48;
        (*plsize) += 48;
        iRes += 8*iDim;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*ppBuf) += 24*len;
        (*plsize) += (4 + 24*len);
        iRes = 8*iDim*(len - 1);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;
        iRes += GetStrokeArcLen(xStart, xEnd, xNorm, dRad, iDim, false, false);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        for(int i = 0; i < (int)len; i++)
        {
            iRes += GetCollectionNextLineLen(ppBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int WriteCollectionNextLine(unsigned char **ppBuf, unsigned char **psBuf,
    int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        (*ppBuf) += 24;
        (*plsize) += 24;
        iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*ppBuf) += 24;
        (*plsize) += 28;
        for(int i = 1; i < (int)len; i++)
        {
            iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
            (*plsize) += 24;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;
        iRes += StrokeArc(xStart, xEnd, xNorm, dRad, psBuf, iDim, false, false);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        for(int i = 0; i < (int)len; i++)
        {
            iRes += WriteCollectionNextLine(ppBuf, psBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int GetCollectionFirstLineLen(unsigned char **ppBuf, int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        (*ppBuf) += 48;
        (*plsize) += 48;
        iRes += 16*iDim;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*ppBuf) += 24*len;
        (*plsize) += (4 + 24*len);
        iRes = 8*iDim*len;
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;
        iRes += GetStrokeArcLen(xStart, xEnd, xNorm, dRad, iDim, true, false);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        iRes += GetCollectionFirstLineLen(ppBuf, iDim, plsize);
        for(int i = 1; i < (int)len; i++)
        {
            iRes += GetCollectionNextLineLen(ppBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int WriteCollectionFirstLine(unsigned char **ppBuf, unsigned char **psBuf,
    int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
        iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
        (*plsize) += 48;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        for(int i = 0; i < (int)len; i++)
        {
            iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
        }
        (*plsize) += (4 + 24*len);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;
        iRes += StrokeArc(xStart, xEnd, xNorm, dRad, psBuf, iDim, true, false);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        iRes += WriteCollectionFirstLine(ppBuf, psBuf, iDim, plsize);
        for(int i = 1; i < (int)len; i++)
        {
            iRes += WriteCollectionNextLine(ppBuf, psBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int GetCollectionNextArcLen(unsigned char **ppBuf, int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        (*ppBuf) += 48;
        (*plsize) += 48;
        iRes += 2*GetCoordSize(iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*ppBuf) += 24*len;
        (*plsize) += (4 + 24*len);
        iRes += 2*(len - 1)*GetCoordSize(iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;

        if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

        iRes += 16*iDim;
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        for(int i = 0; i < (int)len; i++)
        {
            iRes += GetCollectionNextArcLen(ppBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int WriteCollectionNextArc(unsigned char **ppBuf, unsigned char **psBuf,
    int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
        xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
        xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

        iRes += WriteTriple(xMiddle, psBuf, iDim);
        iRes += WriteTriple(xEnd, psBuf, iDim);
        (*plsize) += 48;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        GetTriple(ppBuf, xEnd);
        for(int i = 1; i < (int)len; i++)
        {
            xStart[0] = xEnd[0];
            xStart[1] = xEnd[1];
            xStart[2] = xEnd[2];
            GetTriple(ppBuf, xEnd);
            xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
            xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
            xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

            iRes += WriteTriple(xMiddle, psBuf, iDim);
            iRes += WriteTriple(xEnd, psBuf, iDim);
        }
        (*plsize) += (4 + 24*len);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;

        if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

        iRes += WriteTriple(xMiddle, psBuf, iDim);
        iRes += WriteTriple(xEnd, psBuf, iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        for(int i = 0; i < (int)len; i++)
        {
            iRes += WriteCollectionNextArc(ppBuf, psBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int GetCollectionFirstArcLen(unsigned char **ppBuf, int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        (*ppBuf) += 48;
        (*plsize) += 48;
        iRes += 3*GetCoordSize(iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*ppBuf) += 24*len;
        (*plsize) += (4 + 24*len);
        iRes += (1 + 2*(len - 1))*GetCoordSize(iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;

        if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

        iRes += 24*iDim;
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        iRes += GetCollectionFirstArcLen(ppBuf, iDim, plsize);
        for(int i = 1; i < (int)len; i++)
        {
            iRes += GetCollectionNextArcLen(ppBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int WriteCollectionFirstArc(unsigned char **ppBuf, unsigned char **psBuf,
    int iDim, long *plsize)
{
    int iRes = 0;
    unsigned long len;
    GUID guidBuf;
    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;

    //lSize = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;
    memcpy(&guidBuf, *ppBuf, 16);
    (*ppBuf) += 16;
    (*plsize) += 20;
    if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
        xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
        xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

        iRes += WriteTriple(xStart, psBuf, iDim);
        iRes += WriteTriple(xMiddle, psBuf, iDim);
        iRes += WriteTriple(xEnd, psBuf, iDim);
        (*plsize) += 48;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        GetTriple(ppBuf, xEnd);
        iRes += WriteTriple(xEnd, psBuf, iDim);
        for(int i = 1; i < (int)len; i++)
        {
            xStart[0] = xEnd[0];
            xStart[1] = xEnd[1];
            xStart[2] = xEnd[2];
            GetTriple(ppBuf, xEnd);
            xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
            xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
            xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

            iRes += WriteTriple(xMiddle, psBuf, iDim);
            iRes += WriteTriple(xEnd, psBuf, iDim);
        }
        (*plsize) += (4 + 24*len);
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        GetTriple(ppBuf, xStart);
        GetTriple(ppBuf, xEnd);
        GetTriple(ppBuf, xNorm);
        dRad = *(double*)(*ppBuf);
        (*ppBuf) += 8;
        (*plsize) += 80;

        if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

        iRes += WriteTriple(xStart, psBuf, iDim);
        iRes += WriteTriple(xMiddle, psBuf, iDim);
        iRes += WriteTriple(xEnd, psBuf, iDim);
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        len = *(unsigned long*)(*ppBuf);
        (*ppBuf) += 4;
        (*plsize) += 4;
        iRes += WriteCollectionFirstArc(ppBuf, psBuf, iDim, plsize);
        for(int i = 1; i < (int)len; i++)
        {
            iRes += WriteCollectionNextArc(ppBuf, psBuf, iDim, plsize);
        }
    }

    return(iRes);
}

// for return values see explanation of GetBoundaryType
unsigned char GetLineCollectionType(unsigned char *pBuf)
{
    unsigned char cRes = 0;

    unsigned long lCollLen = *(unsigned long*)pBuf;
    pBuf += 4;

    unsigned long lSize;
    GUID guidBuf;

    for(int i = 0; i < (int)lCollLen; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        memcpy(&guidBuf, pBuf, 16);
        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            cRes |= 2;
            pBuf += lSize;
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            cRes |= 2;
            pBuf += lSize;
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            cRes |= 4;
            pBuf += lSize;
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            pBuf += 16;
            cRes |= GetLineCollectionType(pBuf);
            pBuf += (lSize - 16);
        }
    }
    return(cRes);
}

// for return values see explanation of GetBoundaryType
unsigned char GetPolygonType(unsigned char *pBuf)
{
    unsigned char cRes = 0;
    GUID guidBuf;
    memcpy(&guidBuf, pBuf, 16);
    if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
    {
        cRes = 1;
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
    {
        pBuf += 16;
        cRes = GetLineCollectionType(pBuf);
    }

    return(cRes);
}

// The return value can be combination of:
// 0 - unknown type
// 1 - the collection contains PolygonGeometry
// 2 - the collection contains CompositePolygonGeometry, however
//     they will only contain linear segments
// 4 - the collection contains CompositePolygonGeometry, and
//     at least one of them contains arc(s)
unsigned char GetBoundaryType(unsigned char *pBuf, int iPsgType)
{
    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;

    unsigned char cRes = GetPolygonType(pBuf);
    pBuf += lSize;

    pBuf += 4;
    GUID guidBuf;
    memcpy(&guidBuf, pBuf, 16);
    if(IsEqualGUID(guidBuf, GID_GeometryCollection))
    {
        pBuf += 16;
        unsigned long len = *(unsigned long*)pBuf;
        pBuf += 4;

        for(int i = 0; i < (int)len; i++)
        {
            lSize = *(unsigned long*)pBuf;
            pBuf += 4;
            cRes |= GetPolygonType(pBuf);
            pBuf += lSize;
        }
    }
    switch(iPsgType)
    {
    case 3:
    case 6:
        cRes = 2;
        break;
    }
    return(cRes);
}

// this function is only used in GetBoundaryToEWKBLen, where we don't
// need to know the GDO blob size
int GetPolygonPointsLen(unsigned char **ppBuf, int iDim)
{
    unsigned long lCount = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;

    //int iRes = WriteLong(psBuf, lCount);
    int iRes = 4;

    //for(int i = 0; i < lCount; i++)
    //{
    //    iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
    //}

    iRes += lCount*GetCoordSize(iDim);
    (*ppBuf) += 24*lCount;

    return(iRes);
}

// this function is only used in GetBoundaryToEWKBLen, where we don't
// need to know the GDO blob size
int GetCollPointsLen(unsigned char uiTypeMask, unsigned long lCount,
    unsigned char **ppBuf, int iDim)
{
    //unsigned long lItemCount = GetCollectionItemCount(*ppBuf, lCount,
    //    uiTypeMask, NULL, NULL);
    //int iRes = WriteLong(psBuf, lItemCount);
    int iRes = 4;
    long lsize = 0;

    if(uiTypeMask == 1)
    {
        iRes += GetCollectionFirstLineLen(ppBuf, iDim, &lsize);
        for(int i = 1; i < (int)lCount; i++)
            iRes += GetCollectionNextLineLen(ppBuf, iDim, &lsize);
    }
    else if(uiTypeMask > 1)
    {
        iRes += GetCollectionFirstArcLen(ppBuf, iDim, &lsize);
        for(int i = 1; i < (int)lCount; i++)
            iRes += GetCollectionNextArcLen(ppBuf, iDim, &lsize);
    }

    return(iRes);
}

// this function is only used in BoundaryToEWKB, where we don't
// need to know the GDO blob size
int WritePolygonPoints(unsigned char **ppBuf, unsigned char **psBuf, int iDim)
{
    unsigned long lCount = *(unsigned long*)(*ppBuf);
    (*ppBuf) += 4;

    int iRes = WriteLong(psBuf, lCount);

    for(int i = 0; i < (int)lCount; i++)
    {
        iRes += WriteCoordTriple(ppBuf, psBuf, iDim);
    }

    return(iRes);
}

// this function is only used in BoundaryToEWKB, where we don't
// need to know the GDO blob size
int WriteCollPoints(unsigned char uiTypeMask, unsigned long lCount,
    unsigned char **ppBuf, unsigned char **psBuf, int iDim,
    unsigned char bLevel)
{
    unsigned long lItemCount = GetCollectionItemCount(*ppBuf, lCount,
        uiTypeMask);
    int iRes = WriteLong(psBuf, lItemCount);
    long lsize = 0;

    if(uiTypeMask == 1)
    {
        iRes += WriteCollectionFirstLine(ppBuf, psBuf, iDim, &lsize);
        for(int i = 1; i < (int)lCount; i++)
            iRes += WriteCollectionNextLine(ppBuf, psBuf, iDim, &lsize);
    }
    else if(uiTypeMask > 1)
    {
        iRes += WriteCollectionFirstArc(ppBuf, psBuf, iDim, &lsize);
        for(int i = 1; i < (int)lCount; i++)
            iRes += WriteCollectionNextArc(ppBuf, psBuf, iDim, &lsize);
    }

    return(iRes);
}

// in case of linear collection, the pbLineType is the line segment flag:
// 1 - contains linear segments
// 2 - contains arc segments
// the plCollCount argument will hold the real number of collection items
int GetCollectionMonotype(unsigned char *pBuf, unsigned char *pbLineType,
    long *plCollCount)
{
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;

    GUID guidBuf;
    memcpy(&guidBuf, pBuf, 16);

    unsigned char bLineType;

    int iRes = 0;
    if(IsEqualGUID(guidBuf, GID_PointGeometry))
    {
        iRes = 1;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
    {
        iRes = 1;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
    {
        iRes = 10;
    }
    else if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        *pbLineType |= 1;
        iRes = 2;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        *pbLineType |= 1;
        iRes = 2;
        (*plCollCount) += 1;

    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        *pbLineType |= 2;
        iRes = 2;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        bLineType = GetLineCollectionType(&pBuf[16]);
        if((bLineType & 2) > 0) *pbLineType |= 1;
        if((bLineType & 4) > 0) *pbLineType |= 2;
        iRes = 2;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
    {
        *pbLineType |= 1;
        iRes = 3;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
    {
        bLineType = GetLineCollectionType(&pBuf[16]);
        if((bLineType & 2) > 0) *pbLineType |= 1;
        if((bLineType & 4) > 0) *pbLineType |= 2;
        iRes = 3;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
    {
        bLineType = GetBoundaryType(&pBuf[16], 0);
        if((bLineType & 2) > 0) *pbLineType |= 1;
        if((bLineType & 4) > 0) *pbLineType |= 2;
        iRes = 3;
        (*plCollCount) += 1;
    }
    else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
    {
        iRes = GetCollectionMonotype(&pBuf[16], pbLineType, plCollCount);
    }

    if(iRes < 1) return(0);

    bool bSame = true;
    int i = 1;
    while(i < (int)lCnt)
    {
        pBuf += lSize;
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        memcpy(&guidBuf, pBuf, 16);

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            bSame = (iRes == 1);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            bSame = (iRes == 1);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
        {
            bSame = (iRes == 10);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            *pbLineType |= 1;
            bSame = (iRes == 2);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            *pbLineType |= 1;
            bSame = (iRes == 2);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            *pbLineType |= 2;
            bSame = (iRes == 2);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            bLineType = GetLineCollectionType(&pBuf[16]);
            if((bLineType & 2) > 0) *pbLineType |= 1;
            if((bLineType & 4) > 0) *pbLineType |= 2;
            bSame = (iRes == 2);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            *pbLineType |= 1;
            bSame = (iRes == 3);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            bLineType = GetLineCollectionType(&pBuf[16]);
            if((bLineType & 2) > 0) *pbLineType |= 1;
            if((bLineType & 4) > 0) *pbLineType |= 2;
            bSame = (iRes == 3);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bLineType = GetBoundaryType(&pBuf[16], 0);
            if((bLineType & 2) > 0) *pbLineType |= 1;
            if((bLineType & 4) > 0) *pbLineType |= 2;
            bSame = (iRes == 3);
            (*plCollCount) += 1;
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            bSame = (iRes == GetCollectionMonotype(&pBuf[16], pbLineType,
                plCollCount));
        }

        i++;
    }

    if(!bSame) return(0);

    return(iRes);
}



//======================================================================
// The iPsgType holds the target geometry type as returned by
// PGSGeom.GetGeometryType function. The type can be used to adjust
// the actual type to fit the target type. However, if ulSrid is 0, then
// we know that the geometry is already a part of a collection, so we
// can ignore the iPsgType. However, in case of linear and polygon geometries,
// we still need the iPsgType information to stroke the arcs

int GetPointToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 24;
    int iRes = 0;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;

            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    //iRes += WriteByte(&sBuf, 1);
    iRes += 1;

    //iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    iRes += 4;
    //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    if(ulSrid > 0) iRes += 4;
    //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    iRes += GetCoordSize(iDim);
    return(iRes);
}

int PointToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize, int iDim,
    unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 24;
    int iRes = 0;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;
            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    iRes += WriteByte(&sBuf, 1);
    iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);

    return(iRes);
}

int GetOrientedPointToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;
    int iRes = 0;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;

            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    //iRes += WriteByte(&sBuf, 1);
    iRes += 1;

    //iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    iRes += 4;
    //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    if(ulSrid > 0) iRes += 4;
    //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    iRes += GetCoordSize(iDim);

    return(iRes);
}

int OrientedPointToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;
    int iRes = 0;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;
            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    iRes += WriteByte(&sBuf, 1);
    iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);

    return(iRes);
}

int GetTextPointToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 64;
    int iRes = 0;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;

            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    //iRes += WriteByte(&sBuf, 1);
    iRes += 1;

    //iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    iRes += 4;
    //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    if(ulSrid > 0) iRes += 4;
    //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    iRes += GetCoordSize(iDim);

    pBuf += 60;
    unsigned long *plTextLen = (unsigned long*)pBuf;
    (*lsize) += (*plTextLen);

    return(iRes);
}

int TextPointToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 64;
    int iRes = 0;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;
            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    iRes += WriteByte(&sBuf, 1);
    iRes += WriteGeomType(&sBuf, 1, bNewLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);

    pBuf += 36;
    unsigned long *plTextLen = (unsigned long*)pBuf;
    (*lsize) += (*plTextLen);

    return(iRes);
}

int GetLineToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;

    //int iRes = WriteByte(&sBuf, 1);
    int iRes = 1;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;

            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            break;
        }
    }

    if(iPsgType == 8) // arc ring
    {
        //double xStart[3], xEnd[3], xMiddle[3];
        //GetTriple(&pBuf, xStart);
        //GetTriple(&pBuf, xEnd);
        (*pBuf) += 48;
        //xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
        //xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
        //xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

        //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        iRes += 4;
        //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, 3);
        iRes += 4;
        //iRes += WriteTriple(xStart, &sBuf, iDim);
        //iRes += WriteTriple(xMiddle, &sBuf, iDim);
        //iRes += WriteTriple(xEnd, &sBuf, iDim);
        iRes += 3*GetCoordSize(iDim);
    }
    else
    {
        //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
        iRes += 4;
        //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, 2);
        iRes += 4;
        //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
        //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
        iRes += 3*GetCoordSize(iDim);
    }
    return(iRes);
}

int LineToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize, int iDim,
    unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;

    int iRes = WriteByte(&sBuf, 1);

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            iRes += WriteByte(&sBuf, 1);
            break;
        }
    }

    if(iPsgType == 8) // arc ring
    {
        double xStart[3], xEnd[3], xMiddle[3];
        GetTriple(&pBuf, xStart);
        GetTriple(&pBuf, xEnd);
        xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
        xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
        xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

        iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, 3);
        iRes += WriteTriple(xStart, &sBuf, iDim);
        iRes += WriteTriple(xMiddle, &sBuf, iDim);
        iRes += WriteTriple(xEnd, &sBuf, iDim);
    }
    else
    {
        iRes += WriteGeomType(&sBuf, 2, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, 2);
        iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
        iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    }

    return(iRes);
}

// We are not sure how the rectangle geometry looks like, so let's skip it
/*int GetRectangleToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;

    //int iRes = WriteByte(&sBuf, 1);
    int iRes = 1;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;

            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            break;
        }
    }

    //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
    iRes += 4;
    //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    if(ulSrid > 0) iRes += 4;
    //iRes += WriteLong(&sBuf, 2);
    iRes += 4;
    //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    iRes += 3*GetCoordSize(iDim);
    return(iRes);
}

int RectangleToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 48;

    int iRes = WriteByte(&sBuf, 1);

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            iRes += WriteByte(&sBuf, 1);
            break;
        }
    }

    iRes += WriteGeomType(&sBuf, 3, bNewLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteLong(&sBuf, 1);
    iRes += WriteLong(&sBuf, 5);

    double xStart[3], xEnd[3], x1[3], x2[3];
    GetTriple(&pBuf, xStart);
    GetTriple(&pBuf, xEnd);

    x1[0] = xEnd[0];
    x1[1] = xStart[1];
    x1[2] = (xStart[2] + xEnd[2])/2.0;
    x2[0] = xStart[0];
    x2[1] = xEnd[1];
    x2[2] = x1[2];

    iRes += WriteTriple(xStart, &sBuf, iDim);
    iRes += WriteTriple(x1, &sBuf, iDim);
    iRes += WriteTriple(xEnd, &sBuf, iDim);
    iRes += WriteTriple(x2, &sBuf, iDim);
    iRes += WriteTriple(xStart, &sBuf, iDim);

    return(iRes);
}*/

int GetPolylineToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    unsigned long lSize = *(unsigned long*)pBuf;
    (*lsize) += (4 + lSize*24);
    int iRes = 0;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;

            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    if(iPsgType == 8) // arc ring
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;

        iRes += 8 + (1 + 2*(lSize - 1))*GetCoordSize(iDim);
    }
    else
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;

        iRes += 8 + lSize*GetCoordSize(iDim);
    }
    if(ulSrid > 0) iRes += 4;
    return(iRes);
}

int PolylineToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;
    (*lsize) += (4 + lSize*24);
    int iRes = 0;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    if(iPsgType == 8) // arc ring
    {
        double xStart[3], xEnd[3], xMiddle[3];
        GetTriple(&pBuf, xEnd);

        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, 1 + 2*(lSize - 1));
        iRes += WriteTriple(xEnd, &sBuf, iDim);
        for(int i = 1; i < (int)lSize; i++)
        {
            xStart[0] = xEnd[0];
            xStart[1] = xEnd[1];
            xStart[2] = xEnd[2];

            GetTriple(&pBuf, xEnd);
            xMiddle[0] = (xStart[0] + xEnd[0])/2.0;
            xMiddle[1] = (xStart[1] + xEnd[1])/2.0;
            xMiddle[2] = (xStart[2] + xEnd[2])/2.0;

            iRes += WriteTriple(xMiddle, &sBuf, iDim);
            iRes += WriteTriple(xEnd, &sBuf, iDim);
        }
    }
    else
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 2, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lSize);
        for(int i = 0; i < (int)lSize; i++)
            iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
    }
    return(iRes);
}

int GetArcToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 80;

    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;
    GetTriple(&pBuf, xStart);
    GetTriple(&pBuf, xEnd);
    GetTriple(&pBuf, xNorm);
    dRad = *(double*)pBuf;
    pBuf += 8;
    if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

    int iRes = 0;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    switch(iPsgType)
    {
    case 2:
    case 3:
    case 5:
    case 6:
        if(iPsgType != 3)
        {
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
            iRes += 4;
            //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
            if(ulSrid > 0) iRes += 4;
        }
        iRes += GetStrokeArcLen(xStart, xEnd, xNorm, dRad, iDim, true, true);
        break;
    default:
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        iRes += 4;
        //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, 3); // count of arc points
        iRes += 4;

        //iRes += WriteTriple(xStart, &sBuf, iDim);
        //iRes += WriteTriple(xMiddle, &sBuf, iDim);
        //iRes += WriteTriple(xEnd, &sBuf, iDim);
        iRes += 3*GetCoordSize(iDim);
    }

    return(iRes);
}

int ArcToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize, int iDim,
    unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    (*lsize) += 80;

    double xStart[3], xMiddle[3], xEnd[3], xNorm[3], dRad;
    GetTriple(&pBuf, xStart);
    GetTriple(&pBuf, xEnd);
    GetTriple(&pBuf, xNorm);
    dRad = *(double*)pBuf;
    pBuf += 8;
    if(!GetArcMidPoint(xStart, xMiddle, xEnd, xNorm, dRad)) return(0);

    int iRes = 0;
    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    switch(iPsgType)
    {
    case 2:
    case 3:
    case 5:
    case 6:
        if(iPsgType != 3)
        {
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, 2, bNewLevel);
            if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        }
        iRes += StrokeArc(xStart, xEnd, xNorm, dRad, &sBuf, iDim, true, true);
        break;
    default:
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, 3); // count of arc points

        iRes += WriteTriple(xStart, &sBuf, iDim);
        iRes += WriteTriple(xMiddle, &sBuf, iDim);
        iRes += WriteTriple(xEnd, &sBuf, iDim);
    }

    return(iRes);
}

int GetCompositePolylineToEWKBLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, unsigned char *pbTypeMask, long *plItemCount,
    int iPsgType)
{
    int iRes = 0;
    //unsigned char bNewLevel = bLevel;

    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;
    *pbTypeMask = GetCollectionTypeMask(pBuf, lCnt, iPsgType);
        // 1 - line or polyline, 2 - arc, 3 - mix

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;
            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    if(*pbTypeMask == 1) // treat it as LINESTRING
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 2, bLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;

        *plItemCount = GetCollectionItemCount(pBuf, lCnt, 1);
        //iRes += WriteLong(&sBuf, lItemCount);
        iRes += 4;

        iRes += GetCollectionFirstLineLen(&pBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += GetCollectionNextLineLen(&pBuf, iDim, plsize);
        }
    }
    else if(*pbTypeMask > 1) // treat it as CIRCULARSTRING
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;

        *plItemCount = GetCollectionItemCount(pBuf, lCnt, 2);
        //iRes += WriteLong(&sBuf, lItemCount);
        iRes += 4;

        iRes += GetCollectionFirstArcLen(&pBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += GetCollectionNextArcLen(&pBuf, iDim, plsize);
        }
    }

    return(iRes);
}

int CompositePolylineToEWKB(unsigned char *pBuf, unsigned char *sBuf,
    long *plsize, int iDim, unsigned char bLevel, unsigned long ulSrid,
    unsigned char bTypeMask, long lItemCount, int iPsgType)
{
    // allowed PsgTypes: 0, 2, 5, 7, 8, 9, 11, 14
    // stroking required for 2, 5

    int iRes = 0;
    unsigned char bNewLevel = bLevel;

    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 9:
        case 11:
        case 14:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;
            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    if(bTypeMask == 1) // treat it as LINESTRING
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 2, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lItemCount);

        iRes += WriteCollectionFirstLine(&pBuf, &sBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += WriteCollectionNextLine(&pBuf, &sBuf, iDim, plsize);
        }
    }
    else if(bTypeMask > 1) // treat it as CIRCULARSTRING
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lItemCount);

        iRes += WriteCollectionFirstArc(&pBuf, &sBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += WriteCollectionNextArc(&pBuf, &sBuf, iDim, plsize);
        }
    }
    //else if(bTypeMask == 3)
    // treat it as COMPOUNDCURVE
    // BUT - COMPOUNDCURVE, as well as CURVEPOLYGON has probably some
    // bugs in PostGIS implementation, so we will treat any mix of linear
    // and circular segments as ONE CIRCULAR STRING. The linear parts
    // can be expressed as three points in line
    return(iRes);
}

// Hack for WMS Get Feature Info... command
bool IsPolygonClosed(unsigned long lNumPoints, unsigned char *pBuf)
{
    if(lNumPoints < 4) return(false);

    long lLastPtPos = 24*(lNumPoints - 1);
    bool bRes = true;
    int i = 0;
    while(bRes && (i < 24))
    {
        bRes = (pBuf[i] == pBuf[lLastPtPos + i]);
        i++;
    }
    return(bRes);
}

int GetPolygonToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    unsigned long lSize = *(unsigned long*)pBuf;
    bool bClosed = IsPolygonClosed(lSize, &pBuf[4]);
    if(!bClosed) lSize++;

    (*lsize) += (4 + lSize*24);
    int iRes = 13 + lSize*GetCoordSize(iDim);

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    if(ulSrid > 0) iRes += 4;
    return(iRes);
}

int PolygonToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;

    bool bClosed = IsPolygonClosed(lSize, pBuf);
    unsigned char *pCoordStart = pBuf;

    if(bClosed) (*lsize) += (4 + lSize*24);
    else (*lsize) += (4 + (lSize + 1)*24);

    int iRes = 0;
    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    iRes += WriteByte(&sBuf, 1);
    iRes += WriteGeomType(&sBuf, 3, bNewLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteLong(&sBuf, 1);

    if(bClosed) iRes += WriteLong(&sBuf, lSize);
    else iRes += WriteLong(&sBuf, lSize + 1);

    for(int i = 0; i < (int)lSize; i++)
        iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);

    if(!bClosed) iRes += WriteCoordTriple(&pCoordStart, &sBuf, iDim);
    return(iRes);
}

int GetCompositePolygonToEWKBLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, unsigned char *pbTypeMask, long *plItemCount,
    int iPsgType)
{
    int iRes = 0;
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;
    *pbTypeMask = GetCollectionTypeMask(pBuf, lCnt, iPsgType);
        // 1 - line or polyline, 2 - arc, 3 - mix

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    if(*pbTypeMask == 1) // treat it as POLYGON
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 3, bLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, 1);
        iRes += 4;

        *plItemCount = GetCollectionItemCount(pBuf, lCnt, 1);
        //iRes += WriteLong(&sBuf, lItemCount);
        iRes += 4;

        iRes += GetCollectionFirstLineLen(&pBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += GetCollectionNextLineLen(&pBuf, iDim, plsize);
        }
    }
    else if(*pbTypeMask > 1) // treat it as CURVEDPOLYGON
    {
        switch(iPsgType)
        {
        case 0:
        case 7:
        case 15:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, 13, bNewLevel);
            iRes += 4;
            if(ulSrid > 0)
            {
                //iRes += WriteLong(&sBuf, ulSrid);
                iRes += 4;
                //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }

        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        //iRes += 4;

        *plItemCount = GetCollectionItemCount(pBuf, lCnt, 2);
        //iRes += WriteLong(&sBuf, lItemCount);
        iRes += 4;

        iRes += GetCollectionFirstArcLen(&pBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += GetCollectionNextArcLen(&pBuf, iDim, plsize);
        }
    }
    return(iRes);
}

int CompositePolygonToEWKB(unsigned char *pBuf, unsigned char *sBuf,
    long *plsize, int iDim, unsigned char bLevel, unsigned long ulSrid,
    unsigned char bTypeMask, long lItemCount, int iPsgType)
{
    int iRes = 0;
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    if(bTypeMask == 1) // treat it as POLYGON
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 3, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, 1);
        iRes += WriteLong(&sBuf, lItemCount);

        iRes += WriteCollectionFirstLine(&pBuf, &sBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += WriteCollectionNextLine(&pBuf, &sBuf, iDim, plsize);
        }
    }
    else if(bTypeMask > 1) // treat it as CURVEDPOLYGON
    {
        switch(iPsgType)
        {
        case 0:
        case 7:
        case 15:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, 13, bNewLevel);
            if(ulSrid > 0)
            {
                iRes += WriteLong(&sBuf, ulSrid);
                if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            iRes += WriteLong(&sBuf, 1);
            break;
        }

        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 8, bNewLevel);
        //iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lItemCount);

        iRes += WriteCollectionFirstArc(&pBuf, &sBuf, iDim, plsize);
        for(int i = 1; i < (int)lCnt; i++)
        {
            iRes += WriteCollectionNextArc(&pBuf, &sBuf, iDim, plsize);
        }
    }
    return(iRes);
}

int GetBoundaryToEWKBLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, unsigned char *pcBoundType, int iPsgType)
{
    int iRes = 0;
    *pcBoundType = GetBoundaryType(pBuf, iPsgType);

    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += (4 + lSize);
    unsigned char *ptmpBuf = pBuf;
    ptmpBuf += lSize;
    lSize = *(unsigned long*)ptmpBuf;
    ptmpBuf += 4;
    (*plsize) += (4 + lSize);

    ptmpBuf += 16;
    unsigned long lCollCnt = *(unsigned long*)ptmpBuf;

    unsigned long lCount;
    unsigned char uiTypeMask;
    GUID guidBuf;

    //unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            ulSrid = 0;

            //iRes += WriteLong(&sBuf, 1);
            iRes += 4;
            break;
        }
    }

    if(*pcBoundType < 4) // POLYGON type - contains no arc
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 3, bLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, lCollCnt + 1);
        iRes += 4;

        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += GetPolygonPointsLen(&pBuf, iDim);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
            // we know that it contains linear segments only
        {
            lCount = *(unsigned long*)pBuf;
            pBuf += 4;
            iRes += GetCollPointsLen(1, lCount, &pBuf, iDim);
        }

        pBuf += 4; // size of holes
        pBuf += 16; // geometry IID, should be GeometryCollection
        pBuf += 4; // collection items count - should be already in lCollCnt

        for(int i = 0; i < (int)lCollCnt; i++)
        {
            pBuf += 4; // colelction item size
            memcpy(&guidBuf, pBuf, 16);
            pBuf += 16; // item IID, should be PolygonGeometry

            if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
            {
                iRes += GetPolygonPointsLen(&pBuf, iDim);
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
                // we know that it contains linear segments only
            {
                lCount = *(unsigned long*)pBuf;
                pBuf += 4;
                iRes += GetCollPointsLen(1, lCount, &pBuf, iDim);
            }
        }
    }
    else // CURVEPOLYGON type - contains arcs
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 13, bLevel);
        iRes += 4;
        if(ulSrid > 0)
        {
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
        }
        //iRes += WriteLong(&sBuf, lCollCnt + 1);
        iRes += 4;

        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
            iRes += 4;
            //iRes += WriteLong(&sBuf, ulSrid);
            //iRes += 4;

            iRes += GetPolygonPointsLen(&pBuf, iDim);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            pBuf += 4;
            uiTypeMask = GetCollectionTypeMask(pBuf, lCount, iPsgType);

            if(uiTypeMask == 1)
            {
                //iRes += WriteByte(&sBuf, 1);
                iRes += 1;
                //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                iRes += 4;
                //iRes += WriteLong(&sBuf, ulSrid);
                //iRes += 4;

                iRes += GetCollPointsLen(1, lCount, &pBuf, iDim);
            }
            else if(uiTypeMask > 1)
            {
                //iRes += WriteByte(&sBuf, 1);
                iRes += 1;
                //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
                iRes += 4;
                //iRes += WriteLong(&sBuf, ulSrid);
                //iRes += 4;

                iRes += GetCollPointsLen(2, lCount, &pBuf, iDim);
            }
        }

        lSize = *(unsigned long*)pBuf;
        pBuf += 4; // size of holes
        pBuf += 16; // geometry IID, should be GeometryCollection
        pBuf += 4; // collection items count - should be already in lCollCnt

        for(int i = 0; i < (int)lCollCnt; i++)
        {
            pBuf += 4; // colelction item size
            memcpy(&guidBuf, pBuf, 16);
            pBuf += 16;

            if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
            {
                //iRes += WriteByte(&sBuf, 1);
                iRes += 1;
                //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                iRes += 4;
                //iRes += WriteLong(&sBuf, ulSrid);
                //iRes += 4;

                iRes += GetPolygonPointsLen(&pBuf, iDim);
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
            {
                lCount = *(unsigned long*)pBuf;
                pBuf += 4;
                uiTypeMask = GetCollectionTypeMask(pBuf, lCount, iPsgType);

                if(uiTypeMask == 1)
                {
                    //iRes += WriteByte(&sBuf, 1);
                    iRes += 1;
                    //iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                    iRes += 4;
                    //iRes += WriteLong(&sBuf, ulSrid);
                    //iRes += 4;

                    iRes += GetCollPointsLen(1, lCount, &pBuf, iDim);
                }
                else if(uiTypeMask > 1)
                {
                    //iRes += WriteByte(&sBuf, 1);
                    iRes += 1;
                    //iRes += WriteGeomType(&sBuf, 8, bNewLevel);
                    iRes += 4;
                    //iRes += WriteLong(&sBuf, ulSrid);
                    //iRes += 4;

                    iRes += GetCollPointsLen(uiTypeMask, lCount, &pBuf, iDim);
                }
            }
        }
    }
    return(iRes);
}

int BoundaryToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid,
    unsigned char cBoundType, int iPsgType)
{
    int iRes = 0;
    unsigned long lSize = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += (4 + lSize);
    unsigned char *ptmpBuf = pBuf;
    ptmpBuf += lSize;
    lSize = *(unsigned long*)ptmpBuf;
    ptmpBuf += 4;
    (*plsize) += (4 + lSize);

    ptmpBuf += 16;
    unsigned long lCollCnt = *(unsigned long*)ptmpBuf;

    unsigned long lCount;
    unsigned char uiTypeMask;
    GUID guidBuf;

    unsigned char bNewLevel = bLevel;

    if(ulSrid > 0)
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 13:
        case 15:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += WriteLong(&sBuf, ulSrid);
            if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            ulSrid = 0;

            iRes += WriteLong(&sBuf, 1);
            break;
        }
    }

    if(cBoundType < 4) // POLYGON type - contains no arc
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 3, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lCollCnt + 1);

        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += WritePolygonPoints(&pBuf, &sBuf, iDim);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
            // we know that it contains linear segments only
        {
            lCount = *(unsigned long*)pBuf;
            pBuf += 4;
            iRes += WriteCollPoints(1, lCount, &pBuf, &sBuf, iDim, 0);
        }

        pBuf += 4; // size of holes
        pBuf += 16; // geometry IID, should be GeometryCollection
        pBuf += 4; // collection items count - should be already in lCollCnt

        for(int i = 0; i < (int)lCollCnt; i++)
        {
            pBuf += 4; // colelction item size
            memcpy(&guidBuf, pBuf, 16);
            pBuf += 16; // item IID, should be PolygonGeometry

            if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
            {
                iRes += WritePolygonPoints(&pBuf, &sBuf, iDim);
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
                // we know that it contains linear segments only
            {
                lCount = *(unsigned long*)pBuf;
                pBuf += 4;
                iRes += WriteCollPoints(1, lCount, &pBuf, &sBuf, iDim, 0);
            }
        }
    }
    else // CURVEPOLYGON type - contains arcs
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 13, bNewLevel);
        if(ulSrid > 0)
        {
            iRes += WriteLong(&sBuf, ulSrid);
            if(bNewLevel >= 0x20) bNewLevel -= 0x20;
        }
        iRes += WriteLong(&sBuf, lCollCnt + 1);

        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, 2, bNewLevel);
            //iRes += WriteLong(&sBuf, ulSrid);

            iRes += WritePolygonPoints(&pBuf, &sBuf, iDim);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            pBuf += 4;
            uiTypeMask = GetCollectionTypeMask(pBuf, lCount, iPsgType);

            if(uiTypeMask == 1)
            {
                iRes += WriteByte(&sBuf, 1);
                iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                //iRes += WriteLong(&sBuf, ulSrid);

                iRes += WriteCollPoints(1, lCount, &pBuf, &sBuf, iDim, 0);
            }
            else if(uiTypeMask > 1)
            {
                iRes += WriteByte(&sBuf, 1);
                iRes += WriteGeomType(&sBuf, 8, bNewLevel);
                //iRes += WriteLong(&sBuf, ulSrid);

                iRes += WriteCollPoints(uiTypeMask, lCount, &pBuf, &sBuf, iDim, 0);
            }
        }

        lSize = *(unsigned long*)pBuf;
        pBuf += 4; // size of holes
        pBuf += 16; // geometry IID, should be GeometryCollection
        pBuf += 4; // collection items count - should be already in lCollCnt

        for(int i = 0; i < (int)lCollCnt; i++)
        {
            pBuf += 4; // colelction item size
            memcpy(&guidBuf, pBuf, 16);
            pBuf += 16;

            if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
            {
                iRes += WriteByte(&sBuf, 1);
                iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                //iRes += WriteLong(&sBuf, ulSrid);

                iRes += WritePolygonPoints(&pBuf, &sBuf, iDim);
            }
            else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
            {
                lCount = *(unsigned long*)pBuf;
                pBuf += 4;
                uiTypeMask = GetCollectionTypeMask(pBuf, lCount, iPsgType);

                if(uiTypeMask == 1)
                {
                    iRes += WriteByte(&sBuf, 1);
                    iRes += WriteGeomType(&sBuf, 2, bNewLevel);
                    //iRes += WriteLong(&sBuf, ulSrid);

                    iRes += WriteCollPoints(1, lCount, &pBuf, &sBuf, iDim, 0);
                }
                else if(uiTypeMask > 1)
                {
                    iRes += WriteByte(&sBuf, 1);
                    iRes += WriteGeomType(&sBuf, 8, bNewLevel);
                    //iRes += WriteLong(&sBuf, ulSrid);

                    iRes += WriteCollPoints(uiTypeMask, lCount, &pBuf, &sBuf, iDim, 0);
                }
            }
        }
    }
    return(iRes);
}

int GetMultiPointLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    int iRes = 0;
    int len = 0;
    long lSize, lTmpSize;
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    GUID guidBuf;
    //unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lCnt > 1))
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            iRes += 4;
            //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            ulSrid = 0;
            //iRes += WriteLong(&sBuf, lCnt);
            iRes += 4;
            break;
        }
    }

    if(lCnt > 1)
    {
        switch(iPsgType)
        {
        case 0:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, 4, bNewLevel);
            iRes += 4;
            if(ulSrid > 0)
            {
                //iRes += WriteLong(&sBuf, ulSrid);
                iRes += 4;
                //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            //iRes += WriteLong(&sBuf, lCnt);
            iRes += 4;
            break;
        }
    }
    else
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 1, bNewLevel);
        iRes += 4;
        //iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
        iRes += GetCoordSize(iDim);
        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            len = GetPointToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            len = GetOrientedPointToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }

        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int WriteMultiPoint(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    int iRes = 0;
    int len = 0;
    long lSize, lTmpSize;
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    GUID guidBuf;
    unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lCnt > 0))
    {
        switch(iPsgType)
        {
        case 4:
        case 7:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bLevel);
            if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            iRes += WriteLong(&sBuf, ulSrid);
            ulSrid = 0;
            iRes += WriteLong(&sBuf, lCnt);
            break;
        }
    }

    if(lCnt > 1)
    {
        switch(iPsgType)
        {
        case 0:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, 4, bNewLevel);
            if(ulSrid > 0)
            {
                iRes += WriteLong(&sBuf, ulSrid);
                if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            iRes += WriteLong(&sBuf, lCnt);
            break;
        }
    }
    else
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 1, bNewLevel);
        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);
        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            len = PointToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            len = OrientedPointToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel,
                0, iPsgType);
        }

        sBuf += len;
        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int GetMultiLineLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, long lItemCount, unsigned char bLineType,
    int iPsgType)
{
    int iRes = 0;
    long lSize, lTmpSize;
    unsigned long len = 0, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    // force line type for some output geometries
    switch(iPsgType)
    {
    case 2:
    case 5:
        bLineType = 1;
        break;
    }

    GUID guidBuf;
    //int iType;
    long lCount;
    unsigned char bType;
    //unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lItemCount > 1))
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 11:
        case 14:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += 4;
            //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            ulSrid = 0;
            //iRes += WriteLong(&sBuf, lItemCount);
            iRes += 4;
            break;
        }
    }

    if(lItemCount > 1)
    {
        switch(iPsgType)
        {
        case 0:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //if((bLineType & 2) > 0) iPsgType = 14; // multicurve
            if((bLineType & 2) > 0) iPsgType = 11; // multicurve
            else iPsgType = 5; // multilinestring
            //iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += 4;

            if(ulSrid > 0)
            {
                //iRes += WriteLong(&sBuf, ulSrid);
                iRes += 4;
                //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            //iRes += WriteLong(&sBuf, lItemCount);
            iRes += 4;
            break;
        }
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            iRes += GetLineToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            iRes += GetPolylineToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            iRes += GetArcToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolylineToEWKBLen(pBuf, &lTmpSize, iDim,
                ulSrid, &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            // we know that if the collection contains embedded collection,
            // still the item count must be 1
            lCount = 1;
            iRes += GetMultiLineLen(pBuf, &lTmpSize, iDim, ulSrid, lCount,
                bLineType, iPsgType);
        }

        (*plsize) += lSize;

        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            len = GetLineToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            len = GetPolylineToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            len = GetArcToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            len = GetCompositePolylineToEWKBLen(pBuf, &lTmpSize, iDim, 0,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            len = GetMultiLineLen(pBuf, &lTmpSize, iDim, 0, lCount, bType,
                iPsgType);
        }

        //sBuf += len;
        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int WriteMultiLine(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, long lItemCount,
    unsigned char bLineType, int iPsgType)
{
    int iRes = 0;
    long lSize, lTmpSize;
    unsigned long len = 0, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    // force line type for some output geometries
    switch(iPsgType)
    {
    case 2:
    case 5:
        bLineType = 1;
        break;
    }

    GUID guidBuf;
    //int iType;
    long lCount;
    unsigned char bType;
    unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lItemCount > 1))
    {
        switch(iPsgType)
        {
        case 5:
        case 7:
        case 11:
        case 14:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            iRes += WriteLong(&sBuf, ulSrid);
            ulSrid = 0;
            iRes += WriteLong(&sBuf, lItemCount);
            break;
        }
    }

    if(lItemCount > 1)
    {
        switch(iPsgType)
        {
        case 0:
            iRes += WriteByte(&sBuf, 1);
            //if((bLineType & 2) > 0) iPsgType = 14; // multicurve
            if((bLineType & 2) > 0) iPsgType = 11; // multicurve
            else iPsgType = 5; // multilinestring
            iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);

            if(ulSrid > 0)
            {
                iRes += WriteLong(&sBuf, ulSrid);
                if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            iRes += WriteLong(&sBuf, lItemCount);
            break;
        }
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            iRes += LineToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            iRes += PolylineToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel,
                ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            iRes += ArcToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bLineType);
            iRes += CompositePolylineToEWKB(pBuf, sBuf, &lTmpSize, iDim,
                bNewLevel, ulSrid, bLineType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            // we know that if the collection contains embedded collection,
            // still the item count must be 1
            lCount = 1;
            iRes += WriteMultiLine(pBuf, sBuf, &lTmpSize, iDim, bNewLevel,
                ulSrid, lCount, bLineType, iPsgType);
        }

        (*plsize) += lSize;

        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            len = LineToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            len = PolylineToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            len = ArcToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bLineType);
            len = CompositePolylineToEWKB(pBuf, sBuf, &lTmpSize, iDim,
                bNewLevel, 0, bLineType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            len = WriteMultiLine(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                lCount, bType, iPsgType);
        }

        sBuf += len;
        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int GetMultiPolygonLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, long lItemCount, unsigned char bLineType,
    int iPsgType)
{
    int iRes = 0;
    long lSize, lTmpSize;
    unsigned long len = 0, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    // force line type for some output geometries
    switch(iPsgType)
    {
    case 3:
    case 6:
        bLineType = 1;
        break;
    }

    GUID guidBuf;
    //int iType;
    long lCount;
    unsigned char bType;

    //unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lItemCount > 1))
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 15:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;
            //iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += 4;
            //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            //iRes += WriteLong(&sBuf, ulSrid);
            iRes += 4;
            ulSrid = 0;
            //iRes += WriteLong(&sBuf, lItemCount);
            iRes += 4;
            break;
        }
    }

    if(lItemCount > 1)
    {
        switch(iPsgType)
        {
        case 0:
            //iRes += WriteByte(&sBuf, 1);
            iRes += 1;

            if((bLineType & 2) > 0) iPsgType = 15; // multisurface
            else iPsgType = 6; // multipolygon
            //iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            iRes += 4;

            if(ulSrid > 0)
            {
                //iRes += WriteLong(&sBuf, ulSrid);
                iRes += 4;
                //if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            //iRes += WriteLong(&sBuf, lItemCount);
            iRes += 4;
            break;
        }
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += GetPolygonToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolygonToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = 0;
            iRes += GetBoundaryToEWKBLen(pBuf, &lTmpSize, iDim, ulSrid,
                &bType, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            iRes += GetMultiPolygonLen(pBuf, &lTmpSize, iDim, ulSrid,
                lCount, bType, iPsgType);
        }

        (*plsize) += lTmpSize;

        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            len = GetPolygonToEWKBLen(pBuf, &lTmpSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            len = GetCompositePolygonToEWKBLen(pBuf, &lTmpSize, iDim, 0,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = 0;
            len = GetBoundaryToEWKBLen(pBuf, &lTmpSize, iDim, 0, &bType,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            len = GetMultiPolygonLen(pBuf, &lTmpSize, iDim, 0, lCount, bType,
                iPsgType);
        }

        //sBuf += len;
        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int WriteMultiPolygon(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, long lItemCount,
    unsigned char bLineType, int iPsgType)
{
    int iRes = 0;
    long lSize, lTmpSize;
    unsigned long len = 0, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    // force line type for some output geometries
    switch(iPsgType)
    {
    case 3:
    case 6:
        bLineType = 1;
        break;
    }

    GUID guidBuf;
    //int iType;
    long lCount;
    unsigned char bType;

    unsigned char bNewLevel = bLevel;

    if((ulSrid > 0) && (lItemCount > 1))
    {
        switch(iPsgType)
        {
        case 6:
        case 7:
        case 15:
            iRes += WriteByte(&sBuf, 1);
            iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);
            if(bNewLevel >= 0x20) bNewLevel -= 0x20;
            iRes += WriteLong(&sBuf, ulSrid);
            ulSrid = 0;
            iRes += WriteLong(&sBuf, lItemCount);
            break;
        }
    }

    if(lItemCount > 1)
    {
        switch(iPsgType)
        {
        case 0:
            iRes += WriteByte(&sBuf, 1);
            if((bLineType & 2) > 0) iPsgType = 15; // multisurface
            else iPsgType = 6; // multipolygon
            iRes += WriteGeomType(&sBuf, iPsgType, bNewLevel);

            if(ulSrid > 0)
            {
                iRes += WriteLong(&sBuf, ulSrid);
                if(bNewLevel >= 0x20) bNewLevel -= 0x20;
                ulSrid = 0;
            }
            iRes += WriteLong(&sBuf, lItemCount);
            break;
        }
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += PolygonToEWKB(pBuf, sBuf, &lTmpSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bLineType);
            iRes += CompositePolygonToEWKB(pBuf, sBuf, &lTmpSize, iDim, bLevel,
                ulSrid, bLineType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = GetBoundaryType(pBuf, iPsgType);
            iRes += BoundaryToEWKB(pBuf, sBuf, &lTmpSize, iDim, bLevel, ulSrid,
                bType, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            iRes += WriteMultiPolygon(pBuf, sBuf, &lTmpSize, iDim, bLevel,
                ulSrid, lCount, bType, iPsgType);
        }

        (*plsize) += lTmpSize;

        return(iRes);
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;

        lTmpSize = 0;

        if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            len = PolygonToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bLineType);
            len = CompositePolygonToEWKB(pBuf, sBuf, &lTmpSize, iDim,
                bNewLevel, 0, bLineType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = GetBoundaryType(pBuf, iPsgType);
            len = BoundaryToEWKB(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                bType, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            //iType =
            GetCollectionMonotype(pBuf, &bType, &lCount);
            len = WriteMultiPolygon(pBuf, sBuf, &lTmpSize, iDim, bNewLevel, 0,
                lCount, bType, iPsgType);
        }

        sBuf += len;
        iRes += len;
        pBuf += (lSize - 16);
        (*plsize) += lSize;
    }
    return(iRes);
}

int GetMultiTextLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid)
{
    unsigned long lSize, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;
    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        *pBuf += (4 + lSize);
        (*plsize) += (4 + lSize);
    }
    return(0);
}

int WriteMultiText(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid)
{
    unsigned long lSize, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;
    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        *pBuf += (4 + lSize);
        (*plsize) += (4 + lSize);
    }
    return(0);
}

int GetCollectionLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, long lItemCount, int iPsgType)
{
    int iRes = 0;
    long lSize;
    unsigned long lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    GUID guidBuf;
    int iType;
    long lCount;
    unsigned char bType;

    if(lItemCount > 1)
    {
        //iRes += WriteByte(&sBuf, 1);
        iRes += 1;
        //iRes += WriteGeomType(&sBuf, 7, bLevel); // geometry collection
        iRes += 4;

        //if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        if(ulSrid > 0) iRes += 4;
        //iRes += WriteLong(&sBuf, lItemCount);
        iRes += 4;
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;
        (*plsize) += 16;

        lSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            iRes += GetPointToEWKBLen(pBuf, &lSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            iRes += GetOrientedPointToEWKBLen(pBuf, &lSize, iDim, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
        {
            iRes += GetTextPointToEWKBLen(pBuf, &lSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            iRes += GetLineToEWKBLen(pBuf, &lSize, iDim, ulSrid, 7);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            iRes += GetPolylineToEWKBLen(pBuf, &lSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            iRes += GetArcToEWKBLen(pBuf, &lSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolylineToEWKBLen(pBuf, &lSize, iDim, ulSrid,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += GetPolygonToEWKBLen(pBuf, &lSize, iDim, ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolygonToEWKBLen(pBuf, &lSize, iDim, ulSrid,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = 0;
            iRes += GetBoundaryToEWKBLen(pBuf, &lSize, iDim, ulSrid, &bType,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            iRes += GetGeometryCollectionToEWKBLen(pBuf, &lSize, iDim, ulSrid,
                iType, lCount, bType, iPsgType);
        }

        (*plsize) += lSize;

        return(iRes);
    }

    //unsigned char bNewLevel = bLevel;
    //if(ulSrid > 0)
    //{
    //    if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
    //    else bNewLevel = 0;
    //}

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;
        (*plsize) += 16;

        lSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            iRes += GetPointToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            iRes += GetOrientedPointToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
        {
            iRes += GetTextPointToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            iRes += GetLineToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            iRes += GetPolylineToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            iRes += GetArcToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolylineToEWKBLen(pBuf, &lSize, iDim, 0,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += GetPolygonToEWKBLen(pBuf, &lSize, iDim, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            iRes += GetCompositePolygonToEWKBLen(pBuf, &lSize, iDim, 0,
                &bType, &lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = 0;
            iRes += GetBoundaryToEWKBLen(pBuf, &lSize, iDim, 0, &bType,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            iRes += GetGeometryCollectionToEWKBLen(pBuf, &lSize, iDim, 0,
                iType, lCount, bType, iPsgType);
        }

        pBuf += lSize;
        (*plsize) += lSize;
    }
    return(iRes);
}

int WriteCollection(unsigned char *pBuf, unsigned char *sBuf, long *plsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, long lItemCount,
    int iPsgType)
{
    int iRes = 0;
    long lSize;
    unsigned long len = 0, lCnt = *(unsigned long*)pBuf;
    pBuf += 4;
    (*plsize) += 4;

    GUID guidBuf;
    int iType;
    long lCount;
    unsigned char bType;

    if(lItemCount > 1)
    {
        iRes += WriteByte(&sBuf, 1);
        iRes += WriteGeomType(&sBuf, 7, bLevel); // geometry collection

        if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
        iRes += WriteLong(&sBuf, lItemCount);
    }
    else
    {
        lSize = *(unsigned long*)pBuf;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;
        (*plsize) += 16;

        lSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            iRes += PointToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            iRes += OrientedPointToEWKB(pBuf, sBuf, &lSize, iDim, bLevel,
                ulSrid, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
        {
            iRes += TextPointToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            iRes += LineToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            iRes += PolylineToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            iRes += ArcToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bType);
            iRes += CompositePolylineToEWKB(pBuf, sBuf, &lSize, iDim, bLevel,
                ulSrid, bType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            iRes += PolygonToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bType);
            iRes += CompositePolygonToEWKB(pBuf, sBuf, &lSize, iDim, bLevel,
                ulSrid, bType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = GetBoundaryType(pBuf, iPsgType);
            iRes += BoundaryToEWKB(pBuf, sBuf, &lSize, iDim, bLevel, ulSrid,
                bType, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            iRes += GeometryCollectionToEWKB(pBuf, sBuf, &lSize, iDim, bLevel,
                ulSrid, iType, lCount, bType, iPsgType);
        }

        (*plsize) += lSize;

        return(iRes);
    }

    unsigned char bNewLevel = bLevel;
    if(ulSrid > 0)
    {
        if(bLevel >= 0x20) bNewLevel = bLevel - 0x20;
        else bNewLevel = 0;
    }

    for(int i = 0; i < (int)lCnt; i++)
    {
        lSize = *(unsigned long*)pBuf;
        pBuf += 4;
        (*plsize) += 4;
        memcpy(&guidBuf, pBuf, 16);
        pBuf += 16;
        (*plsize) += 16;

        lSize = 0;

        if(IsEqualGUID(guidBuf, GID_PointGeometry))
        {
            len = PointToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
        {
            len = OrientedPointToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
        {
            len = TextPointToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_LineGeometry))
        {
            len = LineToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
        {
            len = PolylineToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
        {
            len = ArcToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bType);
            len = CompositePolylineToEWKB(pBuf, sBuf, &lSize, iDim,
                bNewLevel, 0, bType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
        {
            len = PolygonToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0,
                iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            lCount = *(unsigned long*)pBuf;
            lCount = GetCollectionItemCount(&pBuf[4], lCount, bType);
            len = CompositePolygonToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel,
                0, bType, lCount, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
        {
            bType = GetBoundaryType(pBuf, iPsgType);
            len = BoundaryToEWKB(pBuf, sBuf, &lSize, iDim, bNewLevel, 0,
                bType, iPsgType);
        }
        else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
        {
            lCount = 0;
            bType = 0;
            iType = GetCollectionMonotype(pBuf, &bType, &lCount);
            len = GeometryCollectionToEWKB(pBuf, sBuf, &lSize, iDim,
                bNewLevel, 0, iType, lCount, bType, iPsgType);
        }

        sBuf += len;
        iRes += len;
        pBuf += lSize;
        (*plsize) += lSize;
    }
    return(iRes);
}

long GetGeometryCollectionToEWKBLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, int iType, long lItemCount, unsigned char bLineType,
    int iPsgType)
{
    switch(iType)
    {
    case 1:
        return(GetMultiPointLen(pBuf, plsize, iDim, ulSrid, iPsgType));
    case 2:
        return(GetMultiLineLen(pBuf, plsize, iDim, ulSrid, lItemCount,
            bLineType, iPsgType));
    case 3:
        return(GetMultiPolygonLen(pBuf, plsize, iDim, ulSrid, lItemCount,
            bLineType, iPsgType));
    case 10:
        return(GetMultiTextLen(pBuf, plsize, iDim, ulSrid));
    default:
        return(GetCollectionLen(pBuf, plsize, iDim, ulSrid, lItemCount,
            iPsgType));
    }
    return(0);
}

long GeometryCollectionToEWKB(unsigned char *pBuf, unsigned char *sBuf,
    long *plsize, int iDim, unsigned char bLevel, unsigned long ulSrid,
    int iType, long lItemCount, unsigned char bLineType, int iPsgType)
{
    switch(iType)
    {
    case 1:
        return(WriteMultiPoint(pBuf, sBuf, plsize, iDim, bLevel, ulSrid,
            iPsgType));
    case 2:
        return(WriteMultiLine(pBuf, sBuf, plsize, iDim, bLevel, ulSrid,
            lItemCount, bLineType, iPsgType));
    case 3:
        return(WriteMultiPolygon(pBuf, sBuf, plsize, iDim, bLevel, ulSrid,
            lItemCount, bLineType, iPsgType));
    case 10:
        return(WriteMultiText(pBuf, sBuf, plsize, iDim, bLevel, ulSrid));
    default:
        return(WriteCollection(pBuf, sBuf, plsize, iDim, bLevel, ulSrid,
            lItemCount, iPsgType));
    }
    return(0);
}

int GetRasterToEWKBLen(unsigned char *pBuf, long *lsize, int iDim,
    unsigned long ulSrid, int iPsgType)
{
    // iPsgType should always be 3

    pBuf += 224;
    unsigned long lSize = *(unsigned long*)pBuf;

    (*lsize) += (226 + 2*lSize);

    int iRes = 13 + 5*GetCoordSize(iDim);
    if(ulSrid > 0) iRes += 4;
    return(iRes);
}

int RasterToEWKB(unsigned char *pBuf, unsigned char *sBuf, long *lsize,
    int iDim, unsigned char bLevel, unsigned long ulSrid, int iPsgType)
{
    // iPsgType should always be 3

    pBuf += 128;
    unsigned char *pCoordStart = pBuf;

    (*lsize) += 226;

    int iRes = WriteByte(&sBuf, 1);
    iRes += WriteGeomType(&sBuf, 3, bLevel);
    if(ulSrid > 0) iRes += WriteLong(&sBuf, ulSrid);
    iRes += WriteLong(&sBuf, 1);

    iRes += WriteLong(&sBuf, 5); // number of polygon points

    for(int i = 0; i < 4; i++) iRes += WriteCoordTriple(&pBuf, &sBuf, iDim);

    iRes += WriteCoordTriple(&pCoordStart, &sBuf, iDim);

    unsigned long lSize = *(unsigned long*)pBuf;

    (*lsize) += (2*lSize);

    return(iRes);
}

bool IsValidGeometryType(unsigned char *pGeomBuf, int iPsgType, int *piGdoType,
    long *plErrMsgId, int *piType, long *plItemCount, unsigned char *pbLineType)
{
    bool bValid = false;
    GUID guidBuf;
    memcpy(&guidBuf, pGeomBuf, 16);
    pGeomBuf += 16;

    if(IsEqualGUID(guidBuf, GID_PointGeometry))
    {
        *piGdoType = 1;
        switch(iPsgType)
        {
        case 0:
        case 1:
        case 4:
        case 7:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_OrientedPointGeometry))
    {
        *piGdoType = 2;
        switch(iPsgType)
        {
        case 0:
        case 1:
        case 4:
        case 7:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_TextPointGeometry))
    {
        *piGdoType = 3;
        *plErrMsgId = EWKB_TEXTNOTSUPPORTED;
        switch(iPsgType)
        {
        case 0:
        case 1:
        case 4:
        case 7:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_LineGeometry))
    {
        *piGdoType = 4;
        switch(iPsgType)
        {
        case 0:
        case 2:
        case 5:
        case 7:
        case 8:
        case 9:
        case 11:
        case 14:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_PolylineGeometry))
    {
        *piGdoType = 5;
        switch(iPsgType)
        {
        case 0:
        case 2:
        case 5:
        case 7:
        case 8:
        case 9:
        case 11:
        case 14:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_ArcGeometry))
    {
        // we allow arc geometry for any type of linear geometry target
        // if the target type cannot handle arcs, we will stroke it
        *piGdoType = 6;
        switch(iPsgType)
        {
        case 0:
        case 2:
        case 5:
        case 7:
        case 8:
        case 9:
        case 11:
        case 14:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolylineGeometry))
    {
        *piGdoType = 7;
        switch(iPsgType)
        {
        case 0:
        case 2:
        case 5:
        case 7:
        case 8:
        case 9:
        case 11:
        case 14:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_PolygonGeometry))
    {
        *piGdoType = 8;
        switch(iPsgType)
        {
        case 0:
        case 3:
        case 6:
        case 7:
        case 13:
        case 15:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_CompositePolygonGeometry))
    {
        *piGdoType = 9;
        switch(iPsgType)
        {
        case 0:
        case 3:
        case 6:
        case 7:
        case 13:
        case 15:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_BoundaryGeometry))
    {
        *piGdoType = 10;
        switch(iPsgType)
        {
        case 0:
        case 3:
        case 6:
        case 7:
        case 13:
        case 15:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_GeometryCollection))
    {
        *piGdoType = 11;
        *piType = GetCollectionMonotype(pGeomBuf, pbLineType, plItemCount);

        switch(iPsgType)
        {
        case 0:
        case 7:
            bValid = true;
            break;
        case 1:
            bValid = (*piType == 1) && (*plItemCount == 1);
            break;
        case 4:
            bValid = (*piType == 1);
            break;
        case 2:
        case 8:
        case 9:
            bValid = (*piType == 2) && (*plItemCount == 1);
            break;
        case 5:
        case 11:
        case 14:
            bValid = (*piType == 2);
            break;
        case 3:
            bValid = (*piType == 3) && (*plItemCount == 1);
            break;
        case 6:
        case 13:
        case 15:
            bValid = (*piType == 3);
            break;
        }
        if(!bValid) *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
    }
    else if(IsEqualGUID(guidBuf, GID_RectangleGeometry))
    {
        *piGdoType = 12;
        switch(iPsgType)
        {
        case 0:
        case 3:
        case 6:
        case 7:
        case 13:
        case 15:
            bValid = true;
            break;
        default:
            *plErrMsgId = EWKB_INVALIDGEOMETRYTYPE;
        }
    }
    else if(IsEqualGUID(guidBuf, GID_RasterGeometry))
    {
        *piGdoType = 13;
        bValid = (iPsgType == 3);
        *plErrMsgId = EWKB_RASTERNOTSUPPORTED;
    }
    return(bValid);
}

long GeomBlobtoEWKBLen(unsigned char *pGeom, int iDim, unsigned long ulSrid,
    int iPsgType, long *plParams, long *plErrMsgId)
{
    /*unsigned char bLevel = 0;
    switch(iDim)
    {
    case 2:
        bLevel = 0x20;
        break;
    case 3:
        bLevel = 0xA0;
        break;
    case 4:
        bLevel = 0xE0;
        break;
    }*/

    int iType = 0;
    long lItemCount = 0;
    unsigned char bLineType = 0;
    int iGeomType = 0;

    if(!IsValidGeometryType(pGeom, iPsgType, &iGeomType, plErrMsgId, &iType,
        &lItemCount, &bLineType)) return(0);

    pGeom += 16;
    long lRes = 1;
    long lSize = 0;

    switch(iGeomType)
    {
    case 1:
        lRes += GetPointToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 2:
        lRes += GetOrientedPointToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 3:
        lRes += GetTextPointToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 4:
        lRes += GetLineToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 5:
        lRes += GetPolylineToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 6:
        lRes += GetArcToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 7:
        lRes += GetCompositePolylineToEWKBLen(pGeom, &lSize, iDim, ulSrid,
            &bLineType, &lItemCount, iPsgType);
        break;
    case 8:
        lRes += GetPolygonToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    case 9:
        lRes += GetCompositePolygonToEWKBLen(pGeom, &lSize, iDim, ulSrid,
            &bLineType, &lItemCount, iPsgType);
        break;
    case 10:
        lRes += GetBoundaryToEWKBLen(pGeom, &lSize, iDim, ulSrid, &bLineType,
            iPsgType);
        break;
    case 11:
        lRes += GetGeometryCollectionToEWKBLen(pGeom, &lSize, iDim, ulSrid,
            iType, lItemCount, bLineType, iPsgType);
        break;
    //case 12:
    //    lRes += GetRectangleToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
    //    break;
    case 13:
        lRes += GetRasterToEWKBLen(pGeom, &lSize, iDim, ulSrid, iPsgType);
        break;
    }

    plParams[0] = iType;
    plParams[1] = lItemCount;
    plParams[2] = bLineType;
    plParams[3] = iGeomType;
    return(lRes);
}

void GeomBlobtoEWKB(unsigned char *pGeom, int iDim, unsigned long ulSrid,
    int iPsgType, unsigned char *pBuf, long *plParams, long *plErrMsgId)
{
    unsigned char bLevel = 0;
    switch(iDim)
    {
    case 2:
        bLevel = 0x20;
        break;
    case 3:
        bLevel = 0xA0;
        break;
    case 4:
        bLevel = 0xE0;
        break;
    }

    int iType = plParams[0];
    long lItemCount = plParams[1];
    unsigned char bLineType = (unsigned char)plParams[2];
    int iGeomType = plParams[3];

    pGeom += 16;
    long lSize = 0;
    switch(iGeomType)
    {
    case 1:
        PointToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 2:
        OrientedPointToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 3:
        TextPointToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 4:
        LineToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 5:
        PolylineToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 6:
        ArcToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 7:
        CompositePolylineToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid,
            bLineType, lItemCount, iPsgType);
        break;
    case 8:
        PolygonToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    case 9:
        CompositePolygonToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid,
            bLineType, lItemCount, iPsgType);
        break;
    case 10:
        BoundaryToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, bLineType,
            iPsgType);
        break;
    case 11:
        GeometryCollectionToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid,
            iType, lItemCount, bLineType, iPsgType);
        break;
    //case 12:
    //    RectangleToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
    //    break;
    case 13:
        RasterToEWKB(pGeom, pBuf, &lSize, iDim, bLevel, ulSrid, iPsgType);
        break;
    }

/*#if DBGLEVEL > 0
    WriteLogFile("GeomBlobtoEWKB:\r\n");
    wchar_t wbuf[512];
    wcsncpy(wbuf, bsBuf, 511);
    wbuf[511] = 0;
    WriteLogFile(wbuf);
    WriteLogFile("\r\n");
#endif // DBGLEVEL*/

    return;
}
