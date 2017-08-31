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

#ifndef _EWKBTOGDO_HPP_
#define _EWKBTOGDO_HPP_

#include <windows.h>

long EWKBtoGeomBlobSize(unsigned char *pGeom, long lSize, int iDim);
void EWKBtoGeomBlob(unsigned char *pGeom, long lSize, int iDim,
    unsigned char *pGDOBuf);

#endif
