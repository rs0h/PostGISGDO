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

#include "VarUtils.hpp"
#include <stdio.h>
#include <tchar.h>
#include <math.h>

int VarGetSize(VARIANT V)
{
    if(V.vt & VT_ARRAY)
    {
        long lbnd, ubnd;
        SafeArrayGetLBound(V.parray, 1, &lbnd);
        SafeArrayGetUBound(V.parray, 1, &ubnd);
        return(ubnd - lbnd + 1);
    }

    switch(V.vt)
    {
    case VT_I4: return(4);
    case VT_UI1: return(1);
    case VT_I2: return(2);
    case VT_BOOL: return(1);
    case VT_I1: return(1);
    case VT_UI2: return(2);
    case VT_UI4:
    case VT_INT:
    case VT_UINT:
    case VT_R4:
        return(4);
    case VT_I8:
    case VT_UI8:
    case VT_R8:
    case VT_DATE:
        return(8);
    case VT_DECIMAL: return(10);
    case VT_BSTR:
        return(2*(wcslen(V.bstrVal) + 1));
    case VT_LPSTR:
        return(strlen(V.pcVal) + 1);
    case VT_LPWSTR:
        return(2*(wcslen(V.bstrVal) + 1));
    default: return(0);
    }
}

int VarToInt(VARIANT V)
{
    double dRes;
    int iRes;
    switch(V.vt)
    {
    case VT_I4: return((int)V.lVal);
    case VT_UI1: return((int)V.bVal);
    case VT_I2: return(V.iVal);
    case VT_BOOL: return((int)V.boolVal);
    case VT_I1: return((int)V.cVal);
    case VT_UI2: return((int)V.uiVal);
    case VT_UI4: return((int)V.ulVal);
    case VT_INT: return((int)V.intVal);
    case VT_UINT: return((int)V.uintVal);
    case VT_I8: return((int)V.llVal);
    case VT_UI8: return((int)V.ullVal);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &dRes);
        return((int)dRes);
    case VT_BSTR:
        swscanf(V.bstrVal, L"%d", &iRes);
        return(iRes);
    case VT_BYREF | VT_I4: return((int)*V.plVal);
    default: return(-1);
    }
}

long VarToLong(VARIANT V)
{
    double dRes;
    long lRes;
    switch(V.vt)
    {
    case VT_I4: return(V.lVal);
    case VT_UI1: return(V.bVal);
    case VT_I2: return(V.iVal);
    case VT_BOOL: return(V.boolVal);
    case VT_I1: return(V.cVal);
    case VT_UI2: return(V.uiVal);
    case VT_UI4: return(V.ulVal);
    case VT_INT: return(V.intVal);
    case VT_UINT: return(V.uintVal);
    case VT_I8: return(V.llVal);
    case VT_UI8: return(V.ullVal);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &dRes);
        return((long)dRes);
    case VT_BSTR:
        swscanf(V.bstrVal, L"%d", &lRes);
        return(lRes);
    default: return(-1);
    }
}

bool VarToBool(VARIANT V)
{
    double dRes;
    bool bRes;
    switch(V.vt)
    {
    case VT_I4: return(V.lVal);
    case VT_UI1: return(V.bVal);
    case VT_I2: return(V.iVal);
    case VT_BOOL: return(V.boolVal);
    case VT_I1: return(V.cVal);
    case VT_UI2: return(V.uiVal);
    case VT_UI4: return(V.ulVal);
    case VT_INT: return(V.intVal);
    case VT_UINT: return(V.uintVal);
    case VT_I8: return(V.llVal);
    case VT_UI8: return(V.ullVal);
    case VT_BSTR:
        bRes = wcslen(V.bstrVal) > 0;
        if(bRes)
        {
            bRes = (V.bstrVal[0] == 't');
            if(!bRes) bRes = (V.bstrVal[0] == 'T');
            if(!bRes) bRes = (V.bstrVal[0] == '1');
        }
        return(bRes);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &dRes);
        return((bool)dRes);
    default: return(FALSE);
    }
}

double VarToDouble(VARIANT V)
{
    double dRes;
    switch(V.vt)
    {
    case VT_I4: return(V.lVal);
    case VT_UI1: return(V.bVal);
    case VT_I2: return(V.iVal);
    case VT_BOOL: return(V.boolVal);
    case VT_I1: return(V.cVal);
    case VT_UI2: return(V.uiVal);
    case VT_UI4: return(V.ulVal);
    case VT_INT: return(V.intVal);
    case VT_UINT: return(V.uintVal);
    case VT_I8: return(V.llVal);
    case VT_UI8: return(V.ullVal);
    case VT_R4: return(V.fltVal);
    case VT_R8: return(V.dblVal);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &dRes);
        return(dRes);
    default: return(0.0);
    }
}

int VarStrLen(VARIANT V)
{
    switch(V.vt)
    {
    case VT_BSTR:
        return(wcslen(V.bstrVal));
    default:
        return(32);
    }
}

BOOL VarToChar(VARIANT V, char *buf, int bufsize, bool bQuote, bool bNulls)
{
    buf[0] = 0;
    char tmpbuf[64];
    double d;
    int i;
    switch(V.vt)
    {
    case VT_EMPTY:
    case VT_NULL:
        if(bNulls) strcpy(buf, "null");
        return(TRUE);
    case VT_I4:
        sprintf(buf, "%d", V.lVal);
        return(TRUE);
    case VT_UI1:
        if(bQuote) sprintf(buf, "'\\%d'", V.bVal);
        else sprintf(buf, "%d", V.bVal);
        return(TRUE);
    case VT_I2:
        sprintf(buf, "%d", V.iVal);
        return(TRUE);
    case VT_BOOL:
        sprintf(buf, "%d", V.boolVal);
        return(TRUE);
    case VT_I1:
        sprintf(buf, "%d", V.cVal);
        return(TRUE);
    case VT_UI2:
        sprintf(buf, "%u", V.uiVal);
        return(TRUE);
    case VT_UI4:
        sprintf(buf, "%u", V.ulVal);
        return(TRUE);
    case VT_INT:
        sprintf(buf, "%d", V.intVal);
        return(TRUE);
    case VT_UINT:
        sprintf(buf, "%u", V.uintVal);
        return(TRUE);
    case VT_R4:
        if(bQuote) sprintf(buf, "%.4f", V.dblVal);
        else
        {
            sprintf(tmpbuf, "%.4f", V.fltVal);
            GetNumberFormatA(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        }
        return(TRUE);
    case VT_R8:
        if(bQuote) sprintf(buf, "%.4f", V.dblVal);
        else
        {
            sprintf(tmpbuf, "%.4f", V.dblVal);
            GetNumberFormatA(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        }
        return(TRUE);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &d);
        if(bQuote) sprintf(buf, "%.4f", d);
        else
        {
            sprintf(tmpbuf, "%.4f", d);
            GetNumberFormatA(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        }
        return(TRUE);
    case VT_BSTR:
        if(bQuote)
        {
            i = wcslen(V.bstrVal);
            if(i > bufsize - 3) i = bufsize - 3;
            strcpy(buf, "'");
            WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, V.bstrVal, -1,
                &buf[1], i, NULL, NULL);
            buf[i + 1] = 0;
            strcat(buf, "'");
            buf[i + 2] = 0;
        }
        else
        {
            i = wcslen(V.bstrVal);
            if(i > bufsize) i = bufsize;
            WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, V.bstrVal, -1,
                buf, i, NULL, NULL);
            buf[i - 1] = 0;
        }
        return(TRUE);
    case VT_LPSTR:
        strcpy(buf, V.pcVal);
        return(TRUE);
    case VT_LPWSTR:
        i = wcslen(V.bstrVal);
        if(i > bufsize) i = bufsize;
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, V.bstrVal, -1,
            buf, i, NULL, NULL);
        buf[i - 1] = 0;
        return(TRUE);
    default:
        buf[0] = 0;
        return(FALSE);
    }
}

BOOL VarToWideChar(VARIANT V, wchar_t *buf, int bufsize, bool bQuote, bool bNulls)
{
    buf[0] = 0;
    int i;
    double d;
    switch(V.vt)
    {
    case VT_EMPTY:
    case VT_NULL:
        if(bNulls) wcscpy(buf, L"null");
        return(TRUE);
    case VT_I4:
        swprintf(buf, L"%d", V.lVal);
        return(TRUE);
    case VT_UI1:
        swprintf(buf, L"%d", V.bVal);
        return(TRUE);
    case VT_I2:
        swprintf(buf, L"%d", V.iVal);
        return(TRUE);
    case VT_BOOL:
        swprintf(buf, L"%d", V.boolVal);
        return(TRUE);
    case VT_I1:
        swprintf(buf, L"%d", V.cVal);
        return(TRUE);
    case VT_UI2:
        swprintf(buf, L"%d", V.uiVal);
        return(TRUE);
    case VT_UI4:
        swprintf(buf, L"%d", V.ulVal);
        return(TRUE);
    case VT_INT:
        swprintf(buf, L"%d", V.intVal);
        return(TRUE);
    case VT_UINT:
        swprintf(buf, L"%d", V.uintVal);
        return(TRUE);
    case VT_R4:
        //swprintf(tmpbuf, L"%.4f", V.fltVal);
        //GetNumberFormatW(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        if(fabs(V.fltVal - (int)(V.fltVal + 0.5)) < 0.0001)
            swprintf(buf, L"%d", (int)V.fltVal);
        else swprintf(buf, L"%.4f", V.fltVal);
        return(TRUE);
    case VT_R8:
        //swprintf(tmpbuf, L"%.4f", V.dblVal);
        //GetNumberFormatW(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        if(fabs(V.dblVal - (int)(V.dblVal + 0.5)) < 0.0001)
            swprintf(buf, L"%d", (int)V.dblVal);
        else swprintf(buf, L"%.4f", V.dblVal);
        return(TRUE);
    case VT_DECIMAL:
        VarR8FromDec(&V.decVal, &d);
        //sprintf(tmpbuf, "%.4f", d);
        //GetNumberFormatW(LOCALE_USER_DEFAULT, 0, tmpbuf, NULL, buf, bufsize);
        if(fabs(d - (int)(d + 0.5)) < 0.0001)
            swprintf(buf, L"%d", (int)d);
        else swprintf(buf, L"%.4f", d);
        return(TRUE);
    case VT_BSTR:
        //i = SysStringLen(V.bstrVal);
        if(bQuote)
        {
            i = wcslen(V.bstrVal);
            if(i > bufsize - 3) i = bufsize - 3;
            wcscpy(buf, L"'");
            wcsncpy(&buf[1], V.bstrVal, i);
            buf[i + 1] = 0;
            wcscat(buf, L"'");
            buf[i + 2] = 0;
            return(TRUE);
        }
        else
        {
            i = wcslen(V.bstrVal);
            if(i < bufsize)
            {
                wcscpy(buf, V.bstrVal);
                return(TRUE);
            }
            else return(FALSE);
        }
    case VT_LPSTR:
        i = strlen(V.pcVal);
        if(2*i < bufsize)
        {
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, V.pcVal, -1, buf, bufsize);
            return(TRUE);
        }
        else return(FALSE);
    case VT_LPWSTR:
        i = wcslen(V.bstrVal);
        if(i < bufsize)
        {
            wcscpy(buf, V.bstrVal);
            return(TRUE);
        }
        else return(FALSE);
    default:
        return(FALSE);
    }
}

int VarToTCharLen(VARIANT V)
{
    switch(V.vt)
    {
    case VT_R4:
        return(64);
    case VT_R8:
        return(64);
    case VT_DECIMAL:
        return(64);
    case VT_BSTR:
        return(wcslen(V.bstrVal) + 1);
    case VT_BSTR | VT_BYREF:
        return(wcslen(*V.pbstrVal) + 1);
    default:
        return(32);
    }
}

LPTSTR VarToTChar(VARIANT V)
{
    int ilen = VarToTCharLen(V);
    LPTSTR sRes = (LPTSTR)malloc(ilen*sizeof(TCHAR));
#ifdef UNICODE
    VarToWideChar(V, sRes, ilen, false, false);
#else
    VarToChar(V, sRes, ilen);
#endif
    return(sRes);
}

BSTR VarToBSTR(VARIANT V)
{
    int ilen = VarToTCharLen(V);
    BSTR sRes = SysAllocStringLen(NULL, ilen);
    VarToWideChar(V, sRes, ilen, false, false);
    return(sRes);
}

LPWSTR VarToWChar(VARIANT V)
{
    int ilen = VarToTCharLen(V);
    LPWSTR sRes = (LPWSTR)malloc((ilen + 1)*sizeof(wchar_t));
    VarToWideChar(V, sRes, ilen, false, false);
    return(sRes);
}

void VarToTCharBuf(VARIANT vValue, LPTSTR tsBuf, int iBufSize)
{
    tsBuf[0] = 0;
#ifdef UNICODE
    VarToWideChar(vValue, tsBuf, iBufSize, false, false);
#else
    VarToChar(vValue, tsBuf, iBufSize);
#endif
    return;
}


bool VarArgCompare(VARIANT v1, VARIANT v2)
{
    if((v1.vt < 2) && (v2.vt < 2)) return(true);
    if(v1.vt != v2.vt) return(false);

    bool bRes = false;
    switch(v1.vt)
    {
    case VT_UI1:
        bRes = (v1.bVal == v2.bVal);
        break;
    case VT_I2:
        bRes = (v1.iVal == v2.iVal);
        break;
    case VT_I4:
        bRes = (v1.lVal == v2.lVal);
        break;
    case VT_R4:
        bRes = (v1.fltVal == v2.fltVal);
        break;
    case VT_R8:
        bRes = (v1.dblVal == v2.dblVal);
        break;
    case VT_BOOL:
        bRes = (v1.boolVal == v2.boolVal);
        break;
    case VT_BSTR:
        bRes = (wcscmp(v1.bstrVal, v2.bstrVal) == 0);
        break;
    }
    return(bRes);
}


void BStrToCharBuf(BSTR bsStr, LPSTR sBuf, int iBufSize)
{
    int n = wcslen(bsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, bsStr, -1, sBuf, iBufSize,
        NULL, NULL);
    sBuf[n] = 0;
    return;
}

void WCharToCharBuf(LPWSTR wsStr, LPSTR sBuf, int iBufSize)
{
    int n = wcslen(wsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wsStr, -1, sBuf, iBufSize,
        NULL, NULL);
    sBuf[n] = 0;
    return;
}

void TCharToCharBuf(LPTSTR tsStr, LPSTR sBuf, int iBufSize)
{
#ifdef UNICODE
    int n = wcslen(tsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, tsStr, -1, sBuf, iBufSize,
        NULL, NULL);
#else
    int n = strlen(tsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    strncpy(sBuf, tsBuf, n);
#endif
    sBuf[n] = 0;
    return;
}

void TCharToWCharBuf(LPTSTR tsStr, LPWSTR wsBuf, int iBufSize)
{
#ifdef UNICODE
    int n = wcslen(tsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    wcsncpy(wsBuf, tsStr, n);
#else
    int n = strlen(tsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tsStr, -1, wsBuf, iBufSize);
#endif
    wsBuf[n] = 0;
    return;
}

void CharToTCharBuf(LPSTR sStr, LPTSTR tsBuf, int iBufSize)
{
    int n = strlen(sStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
#ifdef UNICODE
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sStr, -1, tsBuf, iBufSize);
#else
    strncpy(tsBuf, sBuf, n);
#endif
    tsBuf[n] = 0;
    return;
}

void WCharToTCharBuf(LPWSTR wsStr, LPTSTR tsBuf, int iBufSize)
{
    int n = wcslen(wsStr);
    if(n > iBufSize - 1) n = iBufSize - 1;
#ifdef UNICODE
    wcsncpy(tsBuf, wsStr, n);
#else
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wsStr, -1, tsBuf, iBufSize,
        NULL, NULL);
#endif
    tsBuf[n] = 0;
    return;
}

LPTSTR CharToTChar(LPSTR sStr)
{
    int n = strlen(sStr) + 1;
    LPTSTR tsRes = (LPTSTR)malloc(n*sizeof(TCHAR));
    CharToTCharBuf(sStr, tsRes, n);
    return(tsRes);
}

BSTR CharToBStr(LPSTR sStr)
{
    int slen = strlen(sStr);
    BSTR sRes = SysAllocStringLen(NULL, slen);
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sStr, -1, sRes, slen + 1);
    sRes[slen] = 0;
    return(sRes);
}

BSTR TCharToBStr(LPTSTR tsStr)
{
#ifdef UNICODE
    return(SysAllocString(tsStr));
#else
    int ilen = strlen(tsStr);
    BSTR bsRes = SysAllocStringLen(NULL, ilen);
    MultiByteToWideChar(CP_ACP, WC_PRECOMPOSED, tsStr, -1, bsStr, ilen + 1);
    bsRes[ilen] = 0;
    return(bsRes);
#endif
}

LPSTR BStrToChar(BSTR bsStr)
{
    int iLen = wcslen(bsStr) + 1;
    LPSTR sRes = (LPSTR)malloc(iLen*sizeof(char));
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, bsStr, -1, sRes, iLen,
        NULL, NULL);
    sRes[iLen - 1] = 0;
    return(sRes);
}

LPWSTR CharToWChar(LPSTR sStr)
{
    int slen = strlen(sStr);
    LPWSTR sRes = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sStr, -1, sRes, slen + 1);
    sRes[slen] = 0;
    return(sRes);
}

LPWSTR TCharToWChar(LPTSTR tsStr)
{
    int slen = _tcslen(tsStr);
    LPWSTR sRes = (LPWSTR)malloc((slen + 1)*sizeof(wchar_t));
#ifdef UNICODE
    wcscpy(sRes, tsStr);
#else
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tsStr, -1, sRes, slen + 1);
#endif
    sRes[slen] = 0;
    return(sRes);
}
