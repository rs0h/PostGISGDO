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

#ifndef _LOGFILES_HPP_
#define _LOGFILES_HPP_

#include <wtypes.h>

#ifdef DBGLEVEL
void CreateLogFile();
void WriteLogFile(LPCSTR ABuf, bool bDate);
void WriteLogFile(LPCWSTR AWBuf, bool bDate);
void WriteLogFile(VARIANT V);
void WriteVariantToLogFile(LPCWSTR sMsg, VARIANT V);
void WriteCoordToLogFile(double *px);
void WriteMallocReport(int iPosition, int iExpectedSize, int iTrueSize);
void FreeLogFileName();
#endif // DBGLEVEL

#endif // _LOGFILES_HPP_
