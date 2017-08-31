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

#include "PGtoGDOmaps.hpp"
#include <stdio.h>
#include "LogFiles.hpp"
#include "pg_const.h"

int OidToGdoType(long lOid, int iSize, long lMod, long *presSize)
{
#if DBGLEVEL > 2
    char buf[32];
#endif
    if(presSize) *presSize = iSize;
    switch(lOid)
    {
    case BOOLOID:
        return(gdbBoolean);
    case CHAROID:
        return(gdbByte);
    case BYTEAOID:
        return(gdbLongBinary);
    case NAMEOID:
        if(presSize) *presSize = 64;
        return(gdbText);
    case INT2OID:
        return(gdbInteger);
    case INT4OID:
    case OIDOID:
        return(gdbLong);
    case TEXTOID:
        if(presSize) *presSize = 0;
        return(gdbMemo);
    case FLOAT4OID:
        return(gdbSingle);
    case FLOAT8OID:
        return(gdbDouble);
    case BPCHAROID:
    case VARCHAROID:
        if(lMod > 4)
        {
            if(presSize) *presSize = lMod - VARHDRSZ;
            return(gdbText);
        }
        return(gdbMemo);
    case INT8OID:
        if(presSize) *presSize = 32;
        return(gdbText);
    case NUMERICOID:
        // if lMod == 1 the type is NUMERIC
        // otherwise the precission is ((lMod - VARHDRSZ) >> 16) & 0xFFFF
        // and the scale is (lMod - VARHDRSZ) & 0xFFFF
        // the type is NUMERIC(precission, scale)
        return(gdbDouble);
    case TIMESTAMPOID:
    case TIMESTAMPTZOID:
    case TIMETZOID:
    case TIMEOID:
    case DATEOID:
        return(gdbDate);
    case UUIDOID:
        return(gdbGuid);
    case CASHOID:
        return(gdbCurrency);
    default:
#if DBGLEVEL > 2
        WriteLogFile("OidToGdoType\r\n  Oid: ", true);
        sprintf(buf, "%d", lOid);
        WriteLogFile(buf, false);
        WriteLogFile(";  Size: ", false);
        sprintf(buf, "%d\r\n", iSize);
        WriteLogFile(buf, false);
#endif // DBGLEVEL
        return(gdbLongBinary);
    }
}

long GdoTypeToOid(int iType, long lGeomOid)
{
    switch(iType)
    {
    case gdbBoolean:
        return(BOOLOID);
    case gdbByte:
        return(CHAROID);
    case gdbInteger:
        return(INT2OID);
    case gdbLong:
        return(INT4OID);
    case gdbCurrency:
        return(CASHOID);
    case gdbSingle:
        return(FLOAT4OID);
    case gdbDouble:
        return(FLOAT8OID);
    case gdbDate:
        return(TIMESTAMPOID);
    case gdbText:
        return(VARCHAROID);
    case gdbLongBinary:
        return(BYTEAOID);
    case gdbMemo:
        return(TEXTOID);
    case gdbGuid:
        return(UUIDOID);
    case gdbSpatial:
        return(lGeomOid);
    case gdbGraphic:
        return(BYTEAOID);
    default:
        return(BYTEAOID);
    }
}

int GdoGetFieldStrLen(int iGdoType, long lSize)
{
    switch(iGdoType)
    {
    case gdbText:
    case gdbMemo:
        return(lSize);
    case gdbGuid:
        return(40);
    default:
        return(16);
    }
}

int GetGdoTextType()
{
    return(gdbText);
}
