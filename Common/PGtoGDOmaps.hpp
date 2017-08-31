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

#ifndef _PGTOGDOMAPS_HPP_
#define _PGTOGDOMAPS_HPP_

const short gdbEntirelyContains = 0x0001;
const short gdbEntirelyContainedBy = 0x0002;
const short gdbTouches = 0x0003;
const short gdbNotTouches = 0x0004;
const short gdbIndexIntersect = 0x0005;
const short gdbContains = 0x0006;
const short gdbPoint = 0x000A;
const short gdbLinear = 0x0001;
const short gdbAreal = 0x0002;
const short gdbAnySpatial = 0x0003;
const short gdbCoverage = 0x0004;
const short gdbGraphicsText = 0x0005;
const short gdbOpenDynaset = 0x0002;
const short gdbOpenSnapshot = 0x0004;
const short gdbEditNone = 0x0000;
const short gdbEditInProgress = 0x0001;
const short gdbEditAdd = 0x0002;
const short gdbDenyWrite = 0x0001;
const short gdbDenyRead = 0x0002;
const short gdbReadOnly = 0x0004;
const short gdbAppendOnly = 0x0008;
const short gdbForwardOnly = 0x0100;
const short gdbSeeChanges = 0x0200;
const short gdbFixedField = 0x0001;
const short gdbVariableField = 0x0002;
const short gdbAutoIncrField = 0x0010;
const short gdbUpdatableField = 0x0020;
const short gdbDescending = 0x0001;
const short gdbBoolean = 0x0001;
const short gdbByte = 0x0002;
const short gdbInteger = 0x0003;
const short gdbLong = 0x0004;
const short gdbCurrency = 0x0005;
const short gdbSingle = 0x0006;
const short gdbDouble = 0x0007;
const short gdbDate = 0x0008;
const short gdbText = 0x000A;
const short gdbLongBinary = 0x000B;
const short gdbMemo = 0x000C;
const short gdbGuid = 0x000F;
const short gdbSpatial = 0x0020;
const short gdbGraphic = 0x0021;

//int AdoFldTypeToGdo(int iIdx, int iSize);
//long GdoFldTypeToAdo(int iIdx);
int OidToGdoType(long lOid, int iSize, long lMod, long *presSize);
long GdoTypeToOid(int iType, long lGeomOid);
int GdoGetFieldStrLen(int iGdoType, long lSize);
int GetGdoTextType();

#endif
