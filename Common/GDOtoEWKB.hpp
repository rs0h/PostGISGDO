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

#ifndef _GDOTOEWKB_HPP_
#define _GDOTOEWKB_HPP_

#include <windows.h>

#define EWKB_INVALIDGEOMETRYTYPE 1
#define EWKB_TEXTNOTSUPPORTED 2
#define EWKB_RASTERNOTSUPPORTED 3

long GetGeometryCollectionToEWKBLen(unsigned char *pBuf, long *plsize, int iDim,
    unsigned long ulSrid, int iType, long lItemCount, unsigned char bLineType,
    int iPsgType);
long GeometryCollectionToEWKB(unsigned char *pBuf, unsigned char *sBuf,
    long *plsize, int iDim, unsigned char bLevel, unsigned long ulSrid,
    int iType, long lItemCount, unsigned char bLineType, int iPsgType);
long GeomBlobtoEWKBLen(unsigned char *pGeom, int iDim, unsigned long ulSrid,
    int iPsgType, long *plParams, long *plErrMsgId);
void GeomBlobtoEWKB(unsigned char *pGeom, int iDim, unsigned long ulSrid,
    int iPsgType, unsigned char *pBuf, long *plParams, long *plErrMsgId);

#endif
