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

#include <stdio.h>
#include <windows.h>
#include "LogFiles.hpp"
#include <winbase.h>
#include <tchar.h>

wchar_t g_ModuleName[MAX_PATH];

#ifdef DBGLEVEL
LPWSTR g_LogFileName;
CRITICAL_SECTION g_cs;
FILE *gfh;

void CreateLogFile()
{
    int slen = 0;
    LPWSTR dotptr = wcsrchr(g_ModuleName, '.');
    if(dotptr) slen = dotptr - g_ModuleName + 4;
    else slen = wcslen(g_ModuleName) + 4;
    g_LogFileName = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
    wcsncpy(g_LogFileName, g_ModuleName, slen - 4);
    g_LogFileName[slen - 4] = 0;
    wcscat(g_LogFileName, L".log");

    gfh = _wfopen(g_LogFileName, L"wb");

    InitializeCriticalSection(&g_cs);
    //WriteLogFile("Application starts logging\r\n", true);
    return;
}

void WriteLogFile(LPCSTR ABuf, bool bDate)
{
    EnterCriticalSection(&g_cs);

    int slen;

    if(bDate)
    {
        char tmbuf[64]; // the data/time lenght should be 29 chars max
        SYSTEMTIME st;
        GetLocalTime(&st);
        sprintf(tmbuf, "%d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d - ", st.wYear,
            st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        slen = strlen(tmbuf);
        fwrite(tmbuf, 1, slen, gfh);
    }

    slen = strlen(ABuf);

    fwrite(ABuf, 1, slen, gfh);
    fflush(gfh);
    LeaveCriticalSection(&g_cs);
    return;
}
void WriteLogFile(LPCWSTR AWBuf, bool bDate)
{
    EnterCriticalSection(&g_cs);

    int slen;

    if(bDate)
    {
        char tmbuf[64]; // the data/time lenght should be 29 chars max
        SYSTEMTIME st;
        GetLocalTime(&st);
        sprintf(tmbuf, "%d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d - ", st.wYear,
            st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        slen = strlen(tmbuf);
        fwrite(tmbuf, 1, slen, gfh);
    }

    slen = wcslen(AWBuf) + 1;
    char *buf = (char*)malloc(slen*sizeof(char));
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, AWBuf, -1, buf,
        slen, NULL, NULL);

    fwrite(buf, 1, slen - 1, gfh);
    fflush(gfh);
    free(buf);
    LeaveCriticalSection(&g_cs);
    return;
}

void WriteLogFile(VARIANT V)
{
    wchar_t tmpbuf[64];
    int slen = 64;
    if(V.vt == VT_BSTR) slen = wcslen(V.bstrVal) + 3;
    else if(V.vt == (VT_BYREF | VT_BSTR)) slen = wcslen(*V.pbstrVal) + 3;
    LPWSTR buf = (LPWSTR)malloc(slen*sizeof(wchar_t));

    switch(V.vt)
    {
    case VT_EMPTY:
        wcscpy(buf, L"empty");
        break;
    case VT_NULL:
        wcscpy(buf, L"null");
        break;
    case VT_ERROR:
        if(V.scode == DISP_E_PARAMNOTFOUND) wcscpy(buf, L"missing");
        else wcscpy(buf, L"error");
        break;
    case VT_I4:
        swprintf(buf, L"%d", V.lVal);
        break;
    case VT_UI1:
        swprintf(buf, L"%d", V.bVal);
        break;
    case VT_I2:
        swprintf(buf, L"%d", V.iVal);
        break;
    case VT_BOOL:
        swprintf(buf, L"%d", V.boolVal);
        break;
    case VT_I1:
        swprintf(buf, L"%d", V.cVal);
        break;
    case VT_UI2:
        swprintf(buf, L"%d", V.uiVal);
        break;
    case VT_UI4:
        swprintf(buf, L"%d", V.ulVal);
        break;
    case VT_INT:
        swprintf(buf, L"%d", V.intVal);
        break;
    case VT_UINT:
        swprintf(buf, L"%d", V.uintVal);
        break;
    case VT_R4:
        swprintf(tmpbuf, L"%.4f", V.fltVal);
        GetNumberFormatW(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, slen);
        break;
    case VT_R8:
        swprintf(tmpbuf, L"%.4f", V.dblVal);
        GetNumberFormatW(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, slen);
        break;
    case VT_BSTR:
        swprintf(buf, L"\"%s\"", V.bstrVal);
        break;
    case VT_BYREF | VT_BSTR:
        swprintf(buf, L"\"%s\"", *(V.pbstrVal));
        break;
    case VT_BYREF | VT_I4:
        swprintf(buf, L"%d", *V.plVal);
        break;
    default:
        //buf[0] = 0;
        swprintf(buf, L"Unknown type: %d", V.vt);
    }
    WriteLogFile(buf, false);
    free(buf);
    return;
}

void WriteVariantToLogFile(LPCWSTR sMsg, VARIANT V)
{
    WriteLogFile(sMsg, false);
    WriteLogFile(V);
    WriteLogFile("\r\n", false);
    return;
}

void WriteCoordToLogFile(double *px)
{
    char buf[128];
    sprintf(buf, "%f; %f; %f\r\n", px[0], px[1], px[2]);
    WriteLogFile(buf, false);
    return;
}

void WriteMallocReport(int iPosition, int iExpectedSize, int iTrueSize)
{
    if(iTrueSize > iExpectedSize)
    {
        char buf[128];
        sprintf(buf, "MALLOC %d: exp: %d, true: %d\r\n", iPosition,
            iExpectedSize, iTrueSize);
        WriteLogFile(buf, true);
    }
    return;
}

void FreeLogFileName()
{
    fclose(gfh);
    DeleteCriticalSection(&g_cs);
    free(g_LogFileName);
    return;
}
#endif // DBGLEVEL
