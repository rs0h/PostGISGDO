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

#ifndef _VARUTILS_HPP_
#define _VARUTILS_HPP_

#include <basetyps.h>
#include <wtypes.h>

int VarGetSize(VARIANT V);
int VarToInt(VARIANT V);
long VarToLong(VARIANT V);
bool VarToBool(VARIANT V);
double VarToDouble(VARIANT V);
BOOL VarToChar(VARIANT V, char *buf, int bufsize, bool bQuote, bool bNulls);
BOOL VarToWideChar(VARIANT V, wchar_t *buf, int bufsize, bool bQuote, bool bNulls);
LPTSTR VarToTChar(VARIANT V);
BSTR VarToBSTR(VARIANT V);
LPWSTR VarToWChar(VARIANT V);
void VarToTCharBuf(VARIANT vValue, LPTSTR tsBuf, int iBufSize);

bool VarArgCompare(VARIANT v1, VARIANT v2);

void BStrToCharBuf(BSTR bsStr, LPSTR sBuf, int iBufSize);
void WCharToCharBuf(LPWSTR wsStr, LPSTR sBuf, int iBufSize);
void TCharToCharBuf(LPTSTR tsStr, LPSTR sBuf, int iBufSize);
void TCharToWCharBuf(LPTSTR tsStr, LPWSTR wsBuf, int iBufSize);
void CharToTCharBuf(LPSTR sStr, LPTSTR tsBuf, int iBufSize);
void WCharToTCharBuf(LPWSTR wsStr, LPTSTR tsBuf, int iBufSize);
LPTSTR CharToTChar(LPSTR sStr);
BSTR CharToBStr(LPSTR sStr);
BSTR TCharToBStr(LPTSTR tsStr);
LPSTR BStrToChar(BSTR bsStr);
LPWSTR CharToWChar(LPSTR sStr);
LPWSTR TCharToWChar(LPTSTR tsStr);

#endif
