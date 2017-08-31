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
#include <io.h>
#ifdef GCC
#include <strings.h>
#endif
#include <malloc.h>
#include "../Common/PCSS_TLB.hpp"
#include "../Common/VarUtils.hpp"

/*=====================================
This utility will read a folder with csf files and create a binary table
to be linked into resource. The usage will be:

MakeCSRes <sourc_folder> <destination_filename>
=====================================*/

typedef HRESULT (*PInitializeCache)(LPCSTR ProgID);
typedef HRESULT (*PDestroyCacheExitingApp)(void);

HINSTANCE DoInitCache(PDestroyCacheExitingApp *pdcfunc)
{
    HINSTANCE jutdll = LoadLibrary(L"JUtil.dll");
    if(!jutdll) return(0);

    PInitializeCache icfunc =
        (PInitializeCache)GetProcAddress(jutdll, "InitializeCache");
    HRESULT hr = icfunc("GeoMedia Professional_06.01");

    /* --- InitializeCache always seems to return 0, regardless the status
    if(hr != S_OK)
    {
        FreeLibrary(jutdll);
        return(NULL);
    }*/

    *pdcfunc =
        (PDestroyCacheExitingApp)GetProcAddress(jutdll, "DestroyCacheExitingApp");
    return(jutdll);
}


// We define two constants which may vary in the future versions
const int gCodeSize = 8; // the size (in wide characters) of the EPSG code
    // number, thus the particular record has 16 bytes
const int gColumnCount = 119; // the number of columns in GCoordSystems table
    // we will ignore the first two columns - CSGUID (we will write the EPSG
    // code instead, and CSDUIDTYPE - we will always generate the GUID at
    // run time, so CSDUIDTYPE will be gdbgTransient (1)

const int pFieldSizes[gColumnCount] = {40, 4, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 2, 2, 8, 8, 2, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8,
    8, 2, 2, 2, 2, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 8, 8, 1, 8,
    1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 2, 2, 8, 0};

// we expect that the length of the APSG code (only thi digital part) can
// be maximally 8 characters. We will make the EPSG record in the binary file
// as fixed, 16 bytes.
void GetEPSGCode(LPSTR sInName, LPWSTR sOutCode)
{
    char sBuf[64];
    sprintf(sBuf, "%-12s", sInName);
    int i = 0;
    int j = 0;
    while((i < 64) && ((sBuf[i] < '0') || (sBuf[i] > '9'))) i++;
    while((i < 64) && (sBuf[i] >= '0') && (sBuf[i] <= '9'))
    {
        sOutCode[j++] = sBuf[i++];
    }
    while(j < 8) sOutCode[j++] = 0;
    return;
}

void WriteMetaInformation(FILE *fp, short iCodeSize, short iColumnCount)
{
    fwrite(&iCodeSize, 1, 2, fp);
    fwrite(&iColumnCount, 1, 2, fp);
    return;
}

void WriteVariant(FILE *fp, VARIANT V, int iSize, bool debug)
{
    short slen;
    unsigned char c = 0xFF;
    short s = 0xFFFF;
    long l = 0xFFFFFFFF;
    double d;

    switch(iSize)
    {
    case 0:
        slen = 0;
        if(V.vt == VT_BSTR)
        {
            slen = wcslen(V.bstrVal) + 1;
            fwrite(&slen, 1, 2, fp);
            fwrite(V.bstrVal, 1, 2*slen, fp);
        }
        else fwrite(&slen, 1, 2, fp);
        break;
    case 1:
        if(V.vt > VT_NULL) c = (unsigned char)VarToInt(V);
if(debug) printf("byte val: %d\n", c);
        fwrite(&c, 1, 1, fp);
        break;
    case 2:
        if(V.vt > VT_NULL) s = (short)VarToInt(V);
        fwrite(&s, 1, 2, fp);
        break;
    case 4:
        if(V.vt > VT_NULL) l = (long)VarToLong(V);
        fwrite(&l, 1, 4, fp);
        break;
    case 8:
        //if(V.vt == VT_R8) fwrite(&V.dblVal, 1, 8, fp);
        //else
        if(V.vt > VT_NULL)
        {
            d = (double)VarToDouble(V);
            fwrite(&d, 1, 8, fp);
//if(debug) printf("double val: %d, %f\n", V.vt, d);
        }
        else
        {
            fwrite(&l, 1, 4, fp);
            fwrite(&l, 1, 4, fp);
        }
        break;
    }

    /*if(V.vt == VT_BSTR)
    {
        short iLen = wcslen(V.bstrVal);
        fwrite(&iLen, 1, 2, fp);
        fwrite(V.bstrVal, 1, 2*iLen, fp);
    }
    else
    {
        char c = 0;
        switch(V.vt)
        {
        case VT_UI1:
            fwrite(&V.bVal, 1, iSize, fp);
            break;
        case VT_I2:
            fwrite(&V.iVal, 1, iSize, fp);
            break;
        case VT_I4:
            fwrite(&V.lVal, 1, iSize, fp);
            break;
        case VT_UI2:
            fwrite(&V.uiVal, 1, iSize, fp);
            break;
        case VT_UI4:
            fwrite(&V.ulVal, 1, iSize, fp);
            break;
        case VT_R4:
            fwrite(&V.fltVal, 1, iSize, fp);
            break;
        case VT_R8:
            fwrite(&V.dblVal, 1, iSize, fp);
            break;
        default:
            // write iSize-times zero
            fwrite(&c, 1, iSize, fp);
        }
    }*/
    return;
}

bool WriteCoordSystem(FILE *fp, IGMCoordSystem *pCS, LPWSTR sCode,
    SAFEARRAY *pRow)
{
    HRESULT hres = pCS->ChangeGUID();
    if(hres != S_OK)
    {
        wprintf(L"Cannot change CS GUID: %s\n", sCode);
        return(false);
    }

    hres = pCS->FormatGCoordSystemTableRow(pRow);
    if(hres != S_OK)
    {
        wprintf(L"Cannot format the CS table row: %s\n", sCode);
        return(false);
    }

    fwrite(sCode, 1, 16, fp);

    VARIANT *pVar = NULL;
    SafeArrayAccessData(pRow, (void**)&pVar);
    for(int i = 2; i < gColumnCount; i++)
    {
        WriteVariant(fp, pVar[i], pFieldSizes[i], i == 25);
    }

    SafeArrayUnaccessData(pRow);
    return(true);
}

int main(int argc, char *argv[], char *envp[])
{
    if(argc != 3)
    {
        printf("Usage: MakeCSRes <sourc_folder> <destination_filename>\n");
        return(0);
    }

    struct _finddata_t c_file;
    long hFile;

    int ilen = strlen(argv[1]);
    char *buf = (char*)malloc((ilen + 6)*sizeof(char));
    if(argv[1][ilen - 1] != '\\') sprintf(buf, "%s\\*.csf", argv[1]);
    else sprintf(buf, "%s*.csf", argv[1]);

    if((hFile = _findfirst(buf, &c_file)) == -1L)
    {
        printf("No *.csf files found...\n");
        free(buf);
        return(0);
    }

    if(argv[1][ilen - 1] != '\\') sprintf(buf, "%s\\", argv[1]);
    else strcpy(buf, argv[1]);

    CoInitialize(NULL);

    PDestroyCacheExitingApp dcfunc;
    HINSTANCE jutildll = DoInitCache(&dcfunc);
    // Starting GeoMedia 2013 the is no JCache anymore
    //if(!jutildll)
    //{
    //    printf("Error initializing GeoMedia cache\r\n");
    //    CoUninitialize();
    //    return(1);
    //}

    int iCnt = 1;
    int iProc = 0;

    CoordSystem *pCS = CreateCoordSystem();
    IGMCoordSystem3 *ipCS = NULL;
    HRESULT hres = pCS->QueryInterface(IID_IGMCoordSystem3, (void**)&ipCS);
    pCS->Release();

    if(hres != S_OK)
    {
        _findclose(hFile);
        CoUninitialize();
        printf("Unexpected error - cannot get the IGMCoordSystem interface\n");
        return(0);
    }

    FILE *fp = fopen(argv[2], "wb");
    WriteMetaInformation(fp, gCodeSize, gColumnCount);

    wchar_t wsOutPath[_MAX_FNAME];
    wchar_t wsFileName[64];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buf, -1, wsOutPath, _MAX_FNAME);

    BSTR bsFileName = SysAllocStringLen(NULL, _MAX_FNAME);
    //printf(" %-12s\n", c_file.name);

    SAFEARRAYBOUND sa = {gColumnCount, 0};
    SAFEARRAY *pRow = SafeArrayCreate(VT_VARIANT, 1, &sa);
    if(!pRow)
    {
        CoUninitialize();
        printf("Error - cannot allocate the safearray\n");
        return(0);
    }

    VARIANT *pVar = NULL;
    SafeArrayAccessData(pRow, (void**)&pVar);
    for(int i = 0; i < gColumnCount; i++) VariantInit(&pVar[i]);
    SafeArrayUnaccessData(pRow);

    wchar_t wsCodeBuf[gCodeSize];

    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, c_file.name, -1, wsFileName, 64);
    swprintf(bsFileName, L"%s%s", wsOutPath, wsFileName);
    hres = ipCS->LoadFromFile(bsFileName);
    if(hres == S_OK)
    {
        // write record to output file
        GetEPSGCode(c_file.name, wsCodeBuf);
        if(WriteCoordSystem(fp, ipCS, wsCodeBuf, pRow)) iProc++;
    }
    else wprintf(L"Cannot write file: %s\n", bsFileName);

    while(_findnext(hFile, &c_file) == 0)
    {
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, c_file.name, -1, wsFileName,
            64);
        swprintf(bsFileName, L"%s%s", wsOutPath, wsFileName);
        hres = ipCS->LoadFromFile(bsFileName);
        if(hres == S_OK)
        {
            // write record to output file
            GetEPSGCode(c_file.name, wsCodeBuf);
            if(WriteCoordSystem(fp, ipCS, wsCodeBuf, pRow)) iProc++;
        }
        else wprintf(L"Cannot write file: %s\n", bsFileName);

        iCnt++;
    }

    SafeArrayDestroy(pRow);
    SysFreeString(bsFileName);
    fclose(fp);
    ipCS->Release();

    _findclose(hFile);

    if(jutildll)
    {
        dcfunc();
        FreeLibrary(jutildll);
    }
    CoUninitialize();

    free(buf);

    printf("%d files processed, %d coordsystems written\n", iCnt, iProc);

    return(0);
}
