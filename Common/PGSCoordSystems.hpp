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

#ifndef _PGSCOORDSYSTEMS_HPP_
#define _PGSCOORDSYSTEMS_HPP_

#include <windows.h>
#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif

const int giCSRowSize = 120;

// The GCoordSystems table has 119 entries at the moment. We will load 118
// entries from the resource. The first item of the array is SRID, stored
// in the registry. The second item is CSGUID. This item will be initially
// empty. If a particular CS is found in the database, we will generate
// a new GUID. CSGUID is not stored in the resource. The third entry -
// CSGUIDTYPE - is also not stored. It will be 1 (temporary) for all coord
// systems. The rest of the fields will be populated from the resource.

// But in the end, we will read the row size also from the resource
#define PCSArrayRow VARIANT*

typedef struct CCSArray
{
    int iDataLen, iDataSize;
    PCSArrayRow *pData;
} *PCSArray;

void LoadCSTable(HINSTANCE hInstance, PCSArray pCSTable);
void ClearCSTable(PCSArray pCSTable);
void MarkUsedCSs(PGconn *pConn, PCSArray pSCTable);
void UnmarkUsedCSs(PCSArray pSCTable);
int GetNamedCSGuidCount(PCSArray pSCTable);
// pbNull argument holds information about the number of coordinate
// systems available in the database. If true, then it means that "where 0 = 1"
// clause has been added and thus the caller should not appentd any sql
// clauses comming from the OpenRecordset method
LPSTR GetCSSelectString(PCSArray pSCTable, bool *pbNull, LPSTR *psParams,
    int *piParLens, Oid *plDataTypes, long lEnc, int iRows);
VARIANT *FindCSTableRow(PCSArray pCSTable, long lSrid);
VARIANT *FindCSTableRowByCS(PCSArray pCSTable, VARIANT *PCSRow);
VARIANT *FindCSTableRowByCSGUID(PCSArray pCSTable, BSTR sCSGUID);
unsigned long SRIDfromCSGUID(PCSArray pCSTable, BSTR sCSGUID);

//void UpdateCSGuids(PGconn *pConn, PCSArray pCSTable, LPSTR sCSTable);

#endif
