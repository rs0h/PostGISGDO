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

#include "PCSS_TLB.hpp"

static DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
static DISPID DispIdPropPut = DISPID_PROPERTYPUT;

// We allow each call to Invoke to return an error, however, it might not be
// desirable to throw every exception, so all calls to error handlers are
// commented out by default. To raise exceptions, uncomment this function
// and its calls for desired Invoke calls
/*static void HandleError(HRESULT hr, EXCEPINFO *exi, unsigned int argerr)
{
    switch(hr)
    {
    case DISP_E_BADPARAMCOUNT:
        throw(L"The number of elements provided to DISPPARAMS is different \
from the number of arguments accepted by the method or property.");
    case DISP_E_BADVARTYPE:
        throw(L"One of the arguments in rgvarg is not a valid variant type.");
    case DISP_E_EXCEPTION: //The application needs to raise an exception.
        //In this case, the structure passed in pExcepInfo should be filled in.
        SetLastError(exi->wCode);
        throw(exi->bstrDescription);
    case DISP_E_MEMBERNOTFOUND:
        throw(L"The requested member does not exist, or the call to Invoke \
tried to set the value of a read-only property.");
    case DISP_E_NONAMEDARGS:
        throw(L"This implementation of IDispatch does not support named \
arguments.");
    case DISP_E_OVERFLOW:
        throw(L"One of the arguments in rgvarg could not be coerced to the \
specified type.");
    case DISP_E_PARAMNOTFOUND:
        SetLastError(argerr);
        throw(L"One of the parameter DISPIDs does not correspond to a \
parameter on the method. In this case, puArgErr should be set to the first \
argument that contains the error.");
    case DISP_E_TYPEMISMATCH:
        SetLastError(argerr);
        throw(L"One or more of the arguments could not be coerced. The \
index within rgvarg of the first parameter with the incorrect type is returned \
in the puArgErr parameter.");
    case DISP_E_UNKNOWNINTERFACE:
        throw(L"The interface identifier passed in riid is not IID_NULL.");
    case DISP_E_UNKNOWNLCID:
        throw(L"The member being invoked interprets string arguments \
according to the LCID, and the LCID is not recognized. If the LCID is not \
needed to interpret arguments, this error should not be returned.");
    case DISP_E_PARAMNOTOPTIONAL:
        throw(L"A required parameter was omitted.");
    default:
        return;
    }
}*/

long ProjAlgorithmStrings::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

LPTSTR ProjAlgorithmStrings::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

IUnknown * ProjAlgorithmStrings::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

long GeodeticDatumStrings::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

LPTSTR GeodeticDatumStrings::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

IUnknown * GeodeticDatumStrings::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

long EllipsoidStrings::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

LPTSTR EllipsoidStrings::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

IUnknown * EllipsoidStrings::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

LPTSTR ProjSpace::get_ProjAlgorithmStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_ProjAlgorithmStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long ProjSpace::get_ProjAlgorithmVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void ProjSpace::set_ProjAlgorithmVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_LonOfOrigStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_LonOfOrigStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LonOfOrig()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LonOfOrig(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_LatOfOrigStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_LatOfOrigStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LatOfOrig()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LatOfOrig(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_FalseXStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_FalseXStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_FalseX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_FalseX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_FalseYStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_FalseYStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_FalseY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_FalseY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_ScaleReductFactStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_ScaleReductFactStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_ScaleReductFactVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_ScaleReductFactVal(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_ZoneStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_ZoneStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long ProjSpace::get_ZoneVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void ProjSpace::set_ZoneVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_HemisphereStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_HemisphereStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long ProjSpace::get_HemisphereVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void ProjSpace::set_HemisphereVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_StandPar1Str()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_StandPar1Str(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_StandPar1()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_StandPar1(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_StandPar2Str()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_StandPar2Str(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_StandPar2()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_StandPar2(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_LatOfTrueScaleStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_LatOfTrueScaleStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LatOfTrueScale()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LatOfTrueScale(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_AzimuthAngStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000024, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_AzimuthAngStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000024, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_AzimuthAng()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_AzimuthAng(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR ProjSpace::get_ExtendProjRotStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000026, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void ProjSpace::set_ExtendProjRotStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000026, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_ExtendProjRot()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_ExtendProjRot(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

ProjAlgorithmStrings* ProjSpace::get_ProjAlgorithmStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000028, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((ProjAlgorithmStrings*)pVarResult.pdispVal);
}

long ProjSpace::get_AzimuthDefinitionMode()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000029, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void ProjSpace::set_AzimuthDefinitionMode(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000029, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LatPoint1()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LatPoint1(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LatPoint2()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LatPoint2(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LonPoint1()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LonPoint1(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_LonPoint2()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_LonPoint2(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_GeomHeightOrigin()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_GeomHeightOrigin(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_GeomHeightPoint1()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_GeomHeightPoint1(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_GeomHeightPoint2()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000030, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_GeomHeightPoint2(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000030, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double ProjSpace::get_GeogRadiusStdCircle()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000031, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void ProjSpace::set_GeogRadiusStdCircle(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000031, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_GeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_GeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long GeogSpace::get_GeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void GeogSpace::set_GeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_EllipsoidStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_EllipsoidStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long GeogSpace::get_EllipsoidVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void GeogSpace::set_EllipsoidVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_EquatorialRadStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_EquatorialRadStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double GeogSpace::get_EquatorialRad()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void GeogSpace::set_EquatorialRad(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_PolarRadStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_PolarRadStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double GeogSpace::get_PolarRad()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void GeogSpace::set_PolarRad(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_EccentricityStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_EccentricityStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double GeogSpace::get_EccentricityVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void GeogSpace::set_EccentricityVal(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_FlatteningStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_FlatteningStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double GeogSpace::get_FlatteningVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void GeogSpace::set_FlatteningVal(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR GeogSpace::get_FlatteningInverseStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_FlatteningInverseStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double GeogSpace::get_FlatteningInverseVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void GeogSpace::set_FlatteningInverseVal(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

GeodeticDatumStrings* GeogSpace::get_GeodeticDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((GeodeticDatumStrings*)pVarResult.pdispVal);
}

EllipsoidStrings* GeogSpace::get_EllipsoidStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((EllipsoidStrings*)pVarResult.pdispVal);
}

LPTSTR GeogSpace::get_VerticalDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void GeogSpace::set_VerticalDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long GeogSpace::get_VerticalDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void GeogSpace::set_VerticalDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMStringCollection* GeogSpace::get_VerticalDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

long DGMStringCollectionDisp::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

LPTSTR DGMStringCollectionDisp::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

IUnknown * DGMStringCollectionDisp::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

void UnitFormatSpec::set_ProjUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_ProjUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_GeogUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_GeogUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_GeocUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_GeocUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_PaperUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_PaperUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_HeightUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_HeightUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_PaperHeightUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_PaperHeightUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_ZHeightUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_ZHeightUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_DistUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_DistUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_PaperDistUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_PaperDistUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_AngUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_AngUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_AzimuthUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_AzimuthUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_AreaUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_AreaUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_PaperAreaUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_PaperAreaUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_VolumeUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_VolumeUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_TimeUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_TimeUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_ProjPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_ProjPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_GeogPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_GeogPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_GeocPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_GeocPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_PaperPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_PaperPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_HeightPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_HeightPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_PaperHeightPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_PaperHeightPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_ZHeightPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_ZHeightPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_DistPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_DistPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_PaperDistPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_PaperDistPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_AngPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_AngPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_AzimuthPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_AzimuthPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_AreaPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_AreaPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_PaperAreaPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_PaperAreaPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_VolumePrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_VolumePrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_TimePrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_TimePrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_GeogCoordOrd(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_GeogCoordOrd()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_GeogHemiOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_GeogHemiOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_LonNormOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_LonNormOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_ProjQuadrantOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000024, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_ProjQuadrantOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000024, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_HeightDisplayOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_HeightDisplayOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_HeightDirectionOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000026, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_HeightDirectionOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000026, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_MinusSuppresLonOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_MinusSuppresLonOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_MinusSuppresHeightOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000028, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_MinusSuppresHeightOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000028, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::ParseValueString(long ValueStringType, LPTSTR ValueString,
  double *Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Value;
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(ValueString);
#else
    Buf = SysAllocStringLen(NULL, strlen(ValueString));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ValueString, -1, Buf, strlen(ValueString) + 1);
#endif
    params[1].bstrVal = Buf;
    params[2].vt = VT_I4;
    params[2].lVal = ValueStringType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::ParsePointString(long PointStringType, LPTSTR PointString,
  double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(PointString);
#else
    Buf = SysAllocStringLen(NULL, strlen(PointString));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, PointString, -1, Buf, strlen(PointString) + 1);
#endif
    params[3].bstrVal = Buf;
    params[4].vt = VT_I4;
    params[4].lVal = PointStringType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::FormatValueString(long ValueStringType, double Value, BSTR *ValueString)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = ValueString;
    params[1].vt = VT_R8;
    params[1].dblVal = Value;
    params[2].vt = VT_I4;
    params[2].lVal = ValueStringType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::FormatPointString(long PointStringType, double X, double Y,
  double Z, BSTR *PointString)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = PointString;
    params[1].vt = VT_R8;
    params[1].dblVal = Z;
    params[2].vt = VT_R8;
    params[2].dblVal = Y;
    params[3].vt = VT_R8;
    params[3].dblVal = X;
    params[4].vt = VT_I4;
    params[4].lVal = PointStringType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000067, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::GetPointDescription(long PointStringType, long DescriptionType,
  BSTR *DescriptionString)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = DescriptionString;
    params[1].vt = VT_I4;
    params[1].lVal = DescriptionType;
    params[2].vt = VT_I4;
    params[2].lVal = PointStringType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000068, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::set_AzimuthStart(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000029, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_AzimuthStart()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000029, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_AzimuthDirection(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_AzimuthDirection()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_BearingUnit(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR UnitFormatSpec::get_BearingUnit()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void UnitFormatSpec::set_BearingPrec(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_BearingPrec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::set_MeasurementReferenceSpace(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000002D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitFormatSpec::get_MeasurementReferenceSpace()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000002D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitFormatSpec::CopyTo(IDispatch * UnitFormatSpecDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = UnitFormatSpecDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000069, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::FormatUnitFormatSpecTableRow(SAFEARRAY * *RowFormat)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BYREF | VT_SAFEARRAY;
    params.pparray = RowFormat;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void UnitFormatSpec::LoadFromUnitFormatSpecTableRow(SAFEARRAY * RowFormat)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_PTR | VT_SAFEARRAY;
    params.parray = RowFormat;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void PaperSpace::set_NominalMapScaleDenominator(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double PaperSpace::get_NominalMapScaleDenominator()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void PaperSpace::set_ProjectionToPaperMatrix(SAFEARRAY * Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_PTR | VT_SAFEARRAY;
    params.parray = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

SAFEARRAY * PaperSpace::get_ProjectionToPaperMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

UnitFormatSpec* RefSpaceMgr::get_UnitFormatSpec()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((UnitFormatSpec*)pVarResult.pdispVal);
}

ProjSpace* RefSpaceMgr::get_ProjSpace()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((ProjSpace*)pVarResult.pdispVal);
}

GeogSpace* RefSpaceMgr::get_GeogSpace()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((GeogSpace*)pVarResult.pdispVal);
}

PaperSpace* RefSpaceMgr::get_PaperSpace()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((PaperSpace*)pVarResult.pdispVal);
}

void RefSpaceMgr::TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
  long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void RefSpaceMgr::TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
  long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void RefSpaceMgr::LoadUsingPrimaryFromMGEfile(LPTSTR MGEfileName)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(MGEfileName);
#else
    Buf = SysAllocStringLen(NULL, strlen(MGEfileName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MGEfileName, -1, Buf, strlen(MGEfileName) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void RefSpaceMgr::GetMeridianConvergence(double Longitude, double Latitude, double Height,
  double *MeridianConvergence)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[4];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = MeridianConvergence;
    params[1].vt = VT_R8;
    params[1].dblVal = Height;
    params[2].vt = VT_R8;
    params[2].dblVal = Latitude;
    params[3].vt = VT_R8;
    params[3].dblVal = Longitude;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 4;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000067, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void RefSpaceMgr::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000068, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void RefSpaceMgr::GetScaleFactors(double Longitude, double Latitude, double Height,
  double *K, double *H, double *S)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = S;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = H;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = K;
    params[3].vt = VT_R8;
    params[3].dblVal = Height;
    params[4].vt = VT_R8;
    params[4].dblVal = Latitude;
    params[5].vt = VT_R8;
    params[5].dblVal = Longitude;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000069, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

RefSpaceMgr* DGMCoordSystem::get_RefSpaceMgr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((RefSpaceMgr*)pVarResult.pdispVal);
}

void DGMCoordSystem::set_BaseStorageType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMCoordSystem::get_BaseStorageType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMCoordSystem::set_HeightStorageType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMCoordSystem::get_HeightStorageType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMCoordSystem::set_LonNormStorageOpt(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMCoordSystem::get_LonNormStorageOpt()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMCoordSystem::set_ApproxVertDataCenterAsHeightVal(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMCoordSystem::get_ApproxVertDataCenterAsHeightVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMCoordSystem::get_StorageToProjectionScale()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMCoordSystem::get_StorageToPaperScale()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMCoordSystem::set_StorageToCompMatrix(SAFEARRAY * Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_PTR | VT_SAFEARRAY;
    params.parray = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

SAFEARRAY * DGMCoordSystem::get_StorageToCompMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

void DGMCoordSystem::set_GUID(VARIANT Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = Value.vt;
    if(params.vt == VT_ERROR) params.scode = DISP_E_PARAMNOTFOUND;
    else params.dblVal = Value.dblVal;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

VARIANT DGMCoordSystem::get_GUID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult);
}

void DGMCoordSystem::set_GUIDType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMCoordSystem::get_GUIDType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMCoordSystem::SetHorizResByGroundDistVal(double MagnitudeVal, LPTSTR UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitStr);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitStr, -1, Buf, strlen(UnitStr) + 1);
#endif
    params[0].bstrVal = Buf;
    params[1].vt = VT_R8;
    params[1].dblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000067, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetHorizResByGroundDistVal(double *MagnitudeVal, BSTR *UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = UnitStr;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000068, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetHorizResByAngAlongGroundVal(double MagnitudeVal, LPTSTR UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitStr);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitStr, -1, Buf, strlen(UnitStr) + 1);
#endif
    params[0].bstrVal = Buf;
    params[1].vt = VT_R8;
    params[1].dblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000069, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetHorizResByAngAlongGroundVal(double *MagnitudeVal, BSTR *UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = UnitStr;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetHorizResByPaperDistVal(double MagnitudeVal, LPTSTR UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitStr);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitStr, -1, Buf, strlen(UnitStr) + 1);
#endif
    params[0].bstrVal = Buf;
    params[1].vt = VT_R8;
    params[1].dblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetHorizResByPaperDistVal(double *MagnitudeVal, BSTR *UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = UnitStr;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetVertResByHeightVal(double MagnitudeVal, LPTSTR UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitStr);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitStr, -1, Buf, strlen(UnitStr) + 1);
#endif
    params[0].bstrVal = Buf;
    params[1].vt = VT_R8;
    params[1].dblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetVertResByHeightVal(double *MagnitudeVal, BSTR *UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = UnitStr;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetVertResByPaperHeightVal(double MagnitudeVal, LPTSTR UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitStr);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitStr, -1, Buf, strlen(UnitStr) + 1);
#endif
    params[0].bstrVal = Buf;
    params[1].vt = VT_R8;
    params[1].dblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetVertResByPaperHeightVal(double *MagnitudeVal, BSTR *UnitStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BSTR;
    params[0].pbstrVal = UnitStr;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = MagnitudeVal;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000070, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetApproxHorizDataCenterAsGeogPntVal(double Longitude, double Latitude)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_R8;
    params[0].dblVal = Latitude;
    params[1].vt = VT_R8;
    params[1].dblVal = Longitude;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000071, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetApproxHorizDataCenterAsGeogPntVal(double *Longitude, double *Latitude)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Latitude;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Longitude;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000072, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetApproxHorizDataCenterAsProjPntVal(double ProjectionX, double ProjectionY)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_R8;
    params[0].dblVal = ProjectionY;
    params[1].vt = VT_R8;
    params[1].dblVal = ProjectionX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000073, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetApproxHorizDataCenterAsProjPntVal(double *ProjectionX,
  double *ProjectionY)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = ProjectionY;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = ProjectionX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000074, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetApproxHorizDataCenterAsGeocPntVal(double GeocentricX, double GeocentricY,
  double GeocentricZ)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_R8;
    params[0].dblVal = GeocentricZ;
    params[1].vt = VT_R8;
    params[1].dblVal = GeocentricY;
    params[2].vt = VT_R8;
    params[2].dblVal = GeocentricX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000075, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetApproxHorizDataCenterAsGeocPntVal(double *GeocentricX,
  double *GeocentricY, double *GeocentricZ)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = GeocentricZ;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = GeocentricY;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = GeocentricX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000076, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SetApproxHorizDataCenterAsPaperPntVal(double PaperX, double PaperY)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_R8;
    params[0].dblVal = PaperY;
    params[1].vt = VT_R8;
    params[1].dblVal = PaperX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000077, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetApproxHorizDataCenterAsPaperPntVal(double *PaperX, double *PaperY)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = PaperY;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = PaperX;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000078, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
  long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
  long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::LoadUsingPrimaryFromMGEfile(LPTSTR MGEfileName)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(MGEfileName);
#else
    Buf = SysAllocStringLen(NULL, strlen(MGEfileName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MGEfileName, -1, Buf, strlen(MGEfileName) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::LoadFromGCoordSystemTableRowFormat(SAFEARRAY * RowFormat)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_PTR | VT_SAFEARRAY;
    params.parray = RowFormat;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000079, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::FormatGCoordSystemTableRow(SAFEARRAY * RowFormat)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_SAFEARRAY;
    params.parray = RowFormat;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::IsEssentiallyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsEssentiallyIdentical)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsEssentiallyIdentical;
    params[1].vt = VT_DISPATCH;
    params[1].pdispVal = CoordSystem;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::Clone(IDispatch * *CoordSystemClone)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BYREF | VT_DISPATCH;
    params.ppdispVal = CoordSystemClone;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::CopyTo(IDispatch * CoordSystemDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = CoordSystemDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::GetStorageMeridianConvergence(double Longitude, double Latitude,
  double Height, double *StorageMeridianConvergence)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[4];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = StorageMeridianConvergence;
    params[1].vt = VT_R8;
    params[1].dblVal = Height;
    params[2].vt = VT_R8;
    params[2].dblVal = Latitude;
    params[3].vt = VT_R8;
    params[3].dblVal = Longitude;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 4;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000007F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::ChangeGUID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x00000080, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::SaveToFile(LPTSTR FileName)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(FileName);
#else
    Buf = SysAllocStringLen(NULL, strlen(FileName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, FileName, -1, Buf, strlen(FileName) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000081, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::LoadFromFile(LPTSTR FileName)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(FileName);
#else
    Buf = SysAllocStringLen(NULL, strlen(FileName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, FileName, -1, Buf, strlen(FileName) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000082, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMCoordSystem::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMCoordSystem::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMCoordSystem::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x0000000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMCoordSystem::IsExactlyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsExactlyIdentical)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsExactlyIdentical;
    params[1].vt = VT_DISPATCH;
    params[1].pdispVal = CoordSystem;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000083, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMCoordSystem::LoadFromMGEFileSecondaryCoordSystem(LPTSTR FileName)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(FileName);
#else
    Buf = SysAllocStringLen(NULL, strlen(FileName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, FileName, -1, Buf, strlen(FileName) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000084, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformationDisp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformationDisp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformationDisp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformationDisp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformationDisp::get_ForwardInputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformationDisp::set_ForwardInputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformationDisp::get_ForwardInputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformationDisp::set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformationDisp::get_ForwardOutputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformationDisp::set_ForwardOutputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformationDisp::get_ForwardOutputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformationDisp::set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformationDisp::get_Direction()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformationDisp::set_Direction(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformationDisp::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformationDisp::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformationDisp::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformationDisp::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMBursaWolf* DGMDatumTransformationDisp::get_BursaWolf()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMBursaWolf*)pVarResult.pdispVal);
}

DGMComplexPolynomial* DGMDatumTransformationDisp::get_ComplexPolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMComplexPolynomial*)pVarResult.pdispVal);
}

DGMMultipleRegression* DGMDatumTransformationDisp::get_MultipleRegression()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMMultipleRegression*)pVarResult.pdispVal);
}

DGMSecondDegreePolynomial* DGMDatumTransformationDisp::get_SecondDegreePolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSecondDegreePolynomial*)pVarResult.pdispVal);
}

DGMStandardMolodensky* DGMDatumTransformationDisp::get_StandardMolodensky()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStandardMolodensky*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformationDisp::get_DatumTransModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformationDisp::get_GeodeticDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

void DGMDatumTransformationDisp::CopyTo(DGMDatumTransformation* DatumTransformationDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = (IDispatch*)DatumTransformationDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x60020019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformationDisp::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformationDisp::TransformPoints(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformationDisp::TransformPoint(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_DeltaX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_DeltaX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_DeltaY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_DeltaY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_DeltaZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_DeltaZ(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_OmegaX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_OmegaX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_OmegaY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_OmegaY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_OmegaZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_OmegaZ(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMBursaWolfDisp::get_DeltaScale()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMBursaWolfDisp::set_DeltaScale(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMComplexPolynomialDisp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMComplexPolynomialDisp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMComplexPolynomialDisp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMComplexPolynomialDisp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMStringCollection* DGMComplexPolynomialDisp::get_ComplexPolyModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

long DGMMultipleRegressionDisp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMMultipleRegressionDisp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMMultipleRegressionDisp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMMultipleRegressionDisp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMStringCollection* DGMMultipleRegressionDisp::get_MultRegressionModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

long DGMSecondDegreePolynomialDisp::get_ModelForwardInputUnitID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMSecondDegreePolynomialDisp::set_ModelForwardInputUnitID(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMSecondDegreePolynomialDisp::get_ModelForwardOutputUnitID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMSecondDegreePolynomialDisp::set_ModelForwardOutputUnitID(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMSecondDegreePolynomialDisp::get_ForwardInputSpaceXofOrig()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMSecondDegreePolynomialDisp::set_ForwardInputSpaceXofOrig(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMSecondDegreePolynomialDisp::get_ForwardInputSpaceYofOrig()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMSecondDegreePolynomialDisp::set_ForwardInputSpaceYofOrig(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMSecondDegreePolynomialDisp::get_XNormalizationFactor()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMSecondDegreePolynomialDisp::set_XNormalizationFactor(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMSecondDegreePolynomialDisp::get_YNormalizationFactor()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMSecondDegreePolynomialDisp::set_YNormalizationFactor(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

SAFEARRAY * DGMSecondDegreePolynomialDisp::get_Coefficients()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

void DGMSecondDegreePolynomialDisp::set_Coefficients(SAFEARRAY * Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_PTR | VT_SAFEARRAY;
    params.parray = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMStandardMolodenskyDisp::get_ForwardInputSpaceEllipsoidVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMStandardMolodenskyDisp::set_ForwardInputSpaceEllipsoidVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMStandardMolodenskyDisp::get_ForwardInputSpaceEllipsoidStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMStandardMolodenskyDisp::set_ForwardInputSpaceEllipsoidStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMStringCollection* DGMStandardMolodenskyDisp::get_EllipsoidStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

double DGMStandardMolodenskyDisp::get_ForwardInputSpaceEquatorialRad()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_ForwardInputSpaceEquatorialRad(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_ForwardInputSpaceEccentricity()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_ForwardInputSpaceEccentricity(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_DeltaEquatorialRad()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_DeltaEquatorialRad(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_DeltaFlattening()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_DeltaFlattening(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_DeltaX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_DeltaX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_DeltaY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_DeltaY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMStandardMolodenskyDisp::get_DeltaZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMStandardMolodenskyDisp::set_DeltaZ(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

IDispatch * Link::get_LinkObject()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.pdispVal);
}

long Link::get_Type()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

long Links::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void Links::Add(LPTSTR Name, long Type)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_I4;
    params[0].lVal = Type;
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params[1].bstrVal = Buf;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

Link* Links::Item(LPTSTR Name)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return((Link*)pVarResult.pdispVal);
}

void Links::Delete(LPTSTR Name)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

void Links::DeleteAll()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

IUnknown * Links::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

long TransLinks::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void TransLinks::Append(Link* lpLink, BOOL Strengthen, long DirectionToStrengthen)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = VT_I4;
    params[0].lVal = DirectionToStrengthen;
    params[1].vt = VT_BOOL;
    params[1].boolVal = Strengthen;
    params[2].vt = VT_DISPATCH;
    params[2].pdispVal = (IDispatch*)lpLink;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

Link* TransLinks::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((Link*)pVarResult.pdispVal);
}

void TransLinks::RemoveAll()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

IUnknown * TransLinks::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

Links* AltCoordSystemPath::get_Links()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((Links*)pVarResult.pdispVal);
}

TransLinks* AltCoordSystemPath::get_TransLinks()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((TransLinks*)pVarResult.pdispVal);
}

void AltCoordSystemPath::set_Persist(BOOL Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BOOL;
    params.boolVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

BOOL AltCoordSystemPath::get_Persist()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.boolVal);
}

void AltCoordSystemPath::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void AltCoordSystemPath::TransformPoints(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void AltCoordSystemPath::TransformPoint(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void AltCoordSystemPath::RecomputeInternalPaths()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x00000067, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long AltCoordSystemPaths::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void AltCoordSystemPaths::Add(LPTSTR Name)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

AltCoordSystemPath* AltCoordSystemPaths::Item(LPTSTR Name)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return((AltCoordSystemPath*)pVarResult.pdispVal);
}

void AltCoordSystemPaths::Delete(LPTSTR Name)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Name);
#else
    Buf = SysAllocStringLen(NULL, strlen(Name));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Name, -1, Buf, strlen(Name) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

IUnknown * AltCoordSystemPaths::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

DGMCoordSystem* DGMCoordSystemsMgr::get_CoordSystem()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMCoordSystem*)pVarResult.pdispVal);
}

AltCoordSystemPaths* DGMCoordSystemsMgr::get_AltCoordSystemPaths()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((AltCoordSystemPaths*)pVarResult.pdispVal);
}

long UnitIDs::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x00000001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void UnitIDs::set__UnitType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long UnitIDs::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

IUnknown * UnitIDs::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

void UnitIDs::set__Filter(VARIANT Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = Value.vt;
    if(params.vt == VT_ERROR) params.scode = DISP_E_PARAMNOTFOUND;
    else params.dblVal = Value.dblVal;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x00000003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMUnitsOfMeasure::AddUserDefinedUnit(long UnitType, LPTSTR UnitName, double NumCompBaseUnits,
  double UnitOffset, LPTSTR AppName, LPTSTR AppVersionStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    int i;
    BSTR Buf[3];
    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf[0] = SysAllocString(AppVersionStr);
#else
    Buf[0] = SysAllocStringLen(NULL, strlen(AppVersionStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppVersionStr, -1, Buf[0], strlen(AppVersionStr) + 1);
#endif
    params[0].bstrVal = Buf[0];
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf[1] = SysAllocString(AppName);
#else
    Buf[1] = SysAllocStringLen(NULL, strlen(AppName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppName, -1, Buf[1], strlen(AppName) + 1);
#endif
    params[1].bstrVal = Buf[1];
    params[2].vt = VT_R8;
    params[2].dblVal = UnitOffset;
    params[3].vt = VT_R8;
    params[3].dblVal = NumCompBaseUnits;
    params[4].vt = VT_BSTR;
#ifdef UNICODE
    Buf[2] = SysAllocString(UnitName);
#else
    Buf[2] = SysAllocStringLen(NULL, strlen(UnitName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitName, -1, Buf[2], strlen(UnitName) + 1);
#endif
    params[4].bstrVal = Buf[2];
    params[5].vt = VT_I4;
    params[5].lVal = UnitType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000064, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    for(i = 0; i < 3; i++) SysFreeString(Buf[i]);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

double DGMUnitsOfMeasure::ConvertUnitToUnit(long UnitType, double ValueIn, long UnitIDFrom,
  long UnitIDTo)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params[4];
    DISPPARAMS dispparams;
    params[0].vt = VT_I4;
    params[0].lVal = UnitIDTo;
    params[1].vt = VT_I4;
    params[1].lVal = UnitIDFrom;
    params[2].vt = VT_R8;
    params[2].dblVal = ValueIn;
    params[3].vt = VT_I4;
    params[3].lVal = UnitType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 4;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000065, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMUnitsOfMeasure::DeleteUserDefinedUnit(long UnitType, long UnitID, LPTSTR AppName,
  LPTSTR AppVersionStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    int i;
    BSTR Buf[2];
    VARIANT params[4];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf[0] = SysAllocString(AppVersionStr);
#else
    Buf[0] = SysAllocStringLen(NULL, strlen(AppVersionStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppVersionStr, -1, Buf[0], strlen(AppVersionStr) + 1);
#endif
    params[0].bstrVal = Buf[0];
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf[1] = SysAllocString(AppName);
#else
    Buf[1] = SysAllocStringLen(NULL, strlen(AppName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppName, -1, Buf[1], strlen(AppName) + 1);
#endif
    params[1].bstrVal = Buf[1];
    params[2].vt = VT_I4;
    params[2].lVal = UnitID;
    params[3].vt = VT_I4;
    params[3].lVal = UnitType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 4;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000066, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    for(i = 0; i < 2; i++) SysFreeString(Buf[i]);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMUnitsOfMeasure::GetCompBaseUnit(long UnitType)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = UnitType;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000067, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

UnitIDs* DGMUnitsOfMeasure::GetUnitIDs(long UnitType, VARIANT Filter)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = Filter.vt;
    if(params[0].vt == VT_ERROR) params[0].scode = DISP_E_PARAMNOTFOUND;
    else params[0].dblVal = Filter.dblVal;
    params[1].vt = VT_I4;
    params[1].lVal = UnitType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000068, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((UnitIDs*)pVarResult.pdispVal);
}

long DGMUnitsOfMeasure::GetUnitID(long UnitType, LPTSTR UnitName, VARIANT CaseFlag)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    BSTR Buf;
    VARIANT params[3];
    DISPPARAMS dispparams;
    params[0].vt = CaseFlag.vt;
    if(params[0].vt == VT_ERROR) params[0].scode = DISP_E_PARAMNOTFOUND;
    else params[0].dblVal = CaseFlag.dblVal;
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(UnitName);
#else
    Buf = SysAllocStringLen(NULL, strlen(UnitName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, UnitName, -1, Buf, strlen(UnitName) + 1);
#endif
    params[1].bstrVal = Buf;
    params[2].vt = VT_I4;
    params[2].lVal = UnitType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 3;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000069, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

LPTSTR DGMUnitsOfMeasure::GetUnitName(long UnitID)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = UnitID;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMUnitsOfMeasure::LoadUserDefinedUnits(LPTSTR AppName, LPTSTR AppVersionStr)
{
    EXCEPINFO exi;
    unsigned int argerr;

    int i;
    BSTR Buf[2];
    VARIANT params[2];
    DISPPARAMS dispparams;
    params[0].vt = VT_BSTR;
#ifdef UNICODE
    Buf[0] = SysAllocString(AppVersionStr);
#else
    Buf[0] = SysAllocStringLen(NULL, strlen(AppVersionStr));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppVersionStr, -1, Buf[0], strlen(AppVersionStr) + 1);
#endif
    params[0].bstrVal = Buf[0];
    params[1].vt = VT_BSTR;
#ifdef UNICODE
    Buf[1] = SysAllocString(AppName);
#else
    Buf[1] = SysAllocStringLen(NULL, strlen(AppName));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, AppName, -1, Buf[1], strlen(AppName) + 1);
#endif
    params[1].bstrVal = Buf[1];
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x0000006B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    for(i = 0; i < 2; i++) SysFreeString(Buf[i]);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMLeastSquaresDisp::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMLeastSquaresDisp::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMLeastSquaresDisp::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMLeastSquaresDisp::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMLeastSquaresDisp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMLeastSquaresDisp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_Degree()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_Degree(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_NumberControlPointsNeeded()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

long DGMLeastSquaresDisp::get_DegreesOfFreedom()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

BOOL DGMLeastSquaresDisp::get_Is3d()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.boolVal);
}

BOOL DGMLeastSquaresDisp::get_IsLinear()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.boolVal);
}

long DGMLeastSquaresDisp::get_WeightsUsage()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_WeightsUsage(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_NumberTransformationParameters()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

SAFEARRAY * DGMLeastSquaresDisp::get_TransformationParameters()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

SAFEARRAY * DGMLeastSquaresDisp::get_ParameterCovarianceMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

long DGMLeastSquaresDisp::get_ObservationMatrixSize()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

SAFEARRAY * DGMLeastSquaresDisp::get_AdjObservationCovarianceMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

SAFEARRAY * DGMLeastSquaresDisp::get_ResidualCovarianceMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

long DGMLeastSquaresDisp::get_HomogeneousParamMatrixType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_HomogeneousParamMatrixType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

SAFEARRAY * DGMLeastSquaresDisp::get_HomogeneousParameterMatrix()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.parray);
}

double DGMLeastSquaresDisp::get_VarianceFactor()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLeastSquaresDisp::get_RootMeanSquareOfResiduals()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

BOOL DGMLeastSquaresDisp::get_ApplyResidualSmoothing()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.boolVal);
}

void DGMLeastSquaresDisp::set_ApplyResidualSmoothing(BOOL Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BOOL;
    params.boolVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMLeastSquaresDisp::get_ResidualSmoothingExponent()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMLeastSquaresDisp::set_ResidualSmoothingExponent(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_InputPointType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_InputPointType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_InputSpaceID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_InputSpaceID(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020021, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_OutputPointType()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_OutputPointType(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020023, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMLeastSquaresDisp::get_OutputSpaceID()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMLeastSquaresDisp::set_OutputSpaceID(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

BOOL DGMLeastSquaresDisp::get_ConstraintsUsage()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.boolVal);
}

void DGMLeastSquaresDisp::set_ConstraintsUsage(BOOL Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BOOL;
    params.boolVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020027, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMSingleParameterConstraints* DGMLeastSquaresDisp::get_SingleParameterConstraints()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020029, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSingleParameterConstraints*)pVarResult.pdispVal);
}

DGMObservations* DGMLeastSquaresDisp::get_Observations()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002002A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMObservations*)pVarResult.pdispVal);
}

DGMStringCollection* DGMLeastSquaresDisp::get_LeastSquaresModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002002B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

void DGMLeastSquaresDisp::CopyTo(DGMLeastSquares* LeastSquaresDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = (IDispatch*)LeastSquaresDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002002C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMLeastSquaresDisp::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002002D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMLeastSquaresDisp::TransformPoints(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002002E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMLeastSquaresDisp::TransformPoint(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002002F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMLeastSquaresDisp::CalculateTransformation()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x60020030, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMSingleParameterConstraintsDisp::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

DGMSingleParameterConstraint* DGMSingleParameterConstraintsDisp::Add()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSingleParameterConstraint*)pVarResult.pdispVal);
}

void DGMSingleParameterConstraintsDisp::Delete(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMSingleParameterConstraintsDisp::DeleteAll()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x60020003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMSingleParameterConstraint* DGMSingleParameterConstraintsDisp::Item(long index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = index;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSingleParameterConstraint*)pVarResult.pdispVal);
}

IUnknown * DGMSingleParameterConstraintsDisp::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

long DGMSingleParameterConstraintDisp::get_TransformationParameterIndex()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMSingleParameterConstraintDisp::set_TransformationParameterIndex(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMSingleParameterConstraintDisp::get_Value()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMSingleParameterConstraintDisp::set_Value(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMObservationsDisp::get_Count()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

long DGMObservationsDisp::get_ControlPointCount()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

long DGMObservationsDisp::get_CheckPointCount()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

DGMObservation* DGMObservationsDisp::Add()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMObservation*)pVarResult.pdispVal);
}

void DGMObservationsDisp::Delete(VARIANT index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = index.vt;
    if(params.vt == VT_ERROR) params.scode = DISP_E_PARAMNOTFOUND;
    else params.dblVal = index.dblVal;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMObservationsDisp::DeleteAll()
{
    EXCEPINFO exi;
    unsigned int argerr;

    //HRESULT hr =
    Invoke((DISPID)0x60020005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMObservation* DGMObservationsDisp::Item(VARIANT index)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = index.vt;
    if(params.vt == VT_ERROR) params.scode = DISP_E_PARAMNOTFOUND;
    else params.dblVal = index.dblVal;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x00000000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMObservation*)pVarResult.pdispVal);
}

IUnknown * DGMObservationsDisp::_NewEnum()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0xFFFFFFFC, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.punkVal);
}

LPTSTR DGMObservationDisp::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMObservationDisp::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMObservationDisp::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMObservationDisp::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System1X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1X(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System1Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1Y(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System1Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1Z(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2X(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2Y(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2Z(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_AdjustedSystem1X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_AdjustedSystem1Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_AdjustedSystem1Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_AdjustedSystem2X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_AdjustedSystem2Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_AdjustedSystem2Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMObservationDisp::get_System1StdDeviationX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1StdDeviationX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System1StdDeviationY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1StdDeviationY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System1StdDeviationZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System1StdDeviationZ(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2StdDeviationX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2StdDeviationX(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2StdDeviationY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2StdDeviationY(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002001E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

double DGMObservationDisp::get_System2StdDeviationZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

void DGMObservationDisp::set_System2StdDeviationZ(double Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_R8;
    params.dblVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020020, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMObservationDisp::get_Type()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMObservationDisp::set_Type(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020022, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMLinearModelResidual* DGMObservationDisp::get_LinearModelResidual()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020024, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMLinearModelResidual*)pVarResult.pdispVal);
}

DGMNonlinearModelResidual* DGMObservationDisp::get_NonlinearModelResidual()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020025, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMNonlinearModelResidual*)pVarResult.pdispVal);
}

double DGMLinearModelResidualDisp::get_X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_Norm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_StandardizedX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_StandardizedY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_StandardizedZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMLinearModelResidualDisp::get_StandardizedNorm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020001, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1Norm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020003, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020005, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2Norm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020007, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1StandardizedX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1StandardizedY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020009, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1StandardizedZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System1StandardizedNorm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2StandardizedX()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2StandardizedY()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000D, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2StandardizedZ()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_System2StandardizedNorm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000F, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_Sys2MinusTransformedSys1X()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_Sys2MinusTransformedSys1Y()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020011, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_Sys2MinusTransformedSys1Z()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

double DGMNonlinearModelResidualDisp::get_Sys2MinusTransformedSys1Norm()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.dblVal);
}

UnitAndFormatSpec* CreateUnitAndFormatSpec()
{
    UnitFormatSpec *res;
    CoCreateInstance(CLASS_UnitAndFormatSpec, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_UnitFormatSpec, (void**)&res);
    return(res);
}

CoordSystem* CreateCoordSystem()
{
    DGMCoordSystem *res;
    CoCreateInstance(CLASS_CoordSystem, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMCoordSystem, (void**)&res);
    return(res);
}

long DGMDatumTransformation2Disp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation2Disp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation2Disp::get_ForwardInputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation2Disp::set_ForwardInputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_ForwardInputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation2Disp::get_ForwardOutputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation2Disp::set_ForwardOutputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_ForwardOutputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation2Disp::get_Direction()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation2Disp::set_Direction(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMBursaWolf* DGMDatumTransformation2Disp::get_BursaWolf()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMBursaWolf*)pVarResult.pdispVal);
}

DGMComplexPolynomial* DGMDatumTransformation2Disp::get_ComplexPolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMComplexPolynomial*)pVarResult.pdispVal);
}

DGMMultipleRegression* DGMDatumTransformation2Disp::get_MultipleRegression()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMMultipleRegression*)pVarResult.pdispVal);
}

DGMSecondDegreePolynomial* DGMDatumTransformation2Disp::get_SecondDegreePolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSecondDegreePolynomial*)pVarResult.pdispVal);
}

DGMStandardMolodensky* DGMDatumTransformation2Disp::get_StandardMolodensky()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStandardMolodensky*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformation2Disp::get_DatumTransModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformation2Disp::get_GeodeticDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

void DGMDatumTransformation2Disp::CopyTo(DGMDatumTransformation* DatumTransformationDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = (IDispatch*)DatumTransformationDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x60020019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation2Disp::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation2Disp::TransformPoints(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation2Disp::TransformPoint(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation2Disp::get_UserSuppliedConfigurationParameters()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60030000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation2Disp::set_UserSuppliedConfigurationParameters(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60030000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_ModelTypeVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_ModelTypeVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_ModelTypeStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_ModelTypeStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_ForwardInputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_ForwardInputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_ForwardInputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_ForwardOutputSpaceGeodeticDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_ForwardOutputSpaceGeodeticDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_ForwardOutputSpaceGeodeticDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_Direction()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_Direction(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_Name()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_Name(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x6002000E, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_Description()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_Description(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60020010, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMBursaWolf* DGMDatumTransformation3Disp::get_BursaWolf()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020012, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMBursaWolf*)pVarResult.pdispVal);
}

DGMComplexPolynomial* DGMDatumTransformation3Disp::get_ComplexPolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020013, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMComplexPolynomial*)pVarResult.pdispVal);
}

DGMMultipleRegression* DGMDatumTransformation3Disp::get_MultipleRegression()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020014, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMMultipleRegression*)pVarResult.pdispVal);
}

DGMSecondDegreePolynomial* DGMDatumTransformation3Disp::get_SecondDegreePolynomial()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020015, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMSecondDegreePolynomial*)pVarResult.pdispVal);
}

DGMStandardMolodensky* DGMDatumTransformation3Disp::get_StandardMolodensky()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020016, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStandardMolodensky*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformation3Disp::get_DatumTransModelTypeStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020017, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

DGMStringCollection* DGMDatumTransformation3Disp::get_GeodeticDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60020018, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

void DGMDatumTransformation3Disp::CopyTo(DGMDatumTransformation* DatumTransformationDest)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_DISPATCH;
    params.pdispVal = (IDispatch*)DatumTransformationDest;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x60020019, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation3Disp::IsTransformationValid(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[5];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_BOOL;
    params[0].pboolVal = IsValid;
    params[1].vt = VT_I4;
    params[1].lVal = OutputSpaceID;
    params[2].vt = VT_I4;
    params[2].lVal = OutputPointType;
    params[3].vt = VT_I4;
    params[3].lVal = InputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 5;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001A, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation3Disp::TransformPoints(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[6];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_SAFEARRAY;
    params[0].pparray = Points;
    params[1].vt = VT_I4;
    params[1].lVal = Count;
    params[2].vt = VT_I4;
    params[2].lVal = OutputSpaceID;
    params[3].vt = VT_I4;
    params[3].lVal = OutputPointType;
    params[4].vt = VT_I4;
    params[4].lVal = InputSpaceID;
    params[5].vt = VT_I4;
    params[5].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 6;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001B, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

void DGMDatumTransformation3Disp::TransformPoint(long InputPointType, long InputSpaceID,
  long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params[7];
    DISPPARAMS dispparams;
    params[0].vt = VT_BYREF | VT_R8;
    params[0].pdblVal = Z;
    params[1].vt = VT_BYREF | VT_R8;
    params[1].pdblVal = Y;
    params[2].vt = VT_BYREF | VT_R8;
    params[2].pdblVal = X;
    params[3].vt = VT_I4;
    params[3].lVal = OutputSpaceID;
    params[4].vt = VT_I4;
    params[4].lVal = OutputPointType;
    params[5].vt = VT_I4;
    params[5].lVal = InputSpaceID;
    params[6].vt = VT_I4;
    params[6].lVal = InputPointType;
    dispparams.rgvarg = params;
    dispparams.rgdispidNamedArgs = NULL;
    dispparams.cArgs = 7;
    dispparams.cNamedArgs = 0;
    //HRESULT hr =
    Invoke((DISPID)0x6002001C, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_METHOD, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_UserSuppliedConfigurationParameters()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60030000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_UserSuppliedConfigurationParameters(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60030000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_ForwardInputSpaceVerticalDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60040000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_ForwardInputSpaceVerticalDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60040000, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_ForwardInputSpaceVerticalDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60040002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_ForwardInputSpaceVerticalDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60040002, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

long DGMDatumTransformation3Disp::get_ForwardOutputSpaceVerticalDatumVal()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60040004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return(pVarResult.lVal);
}

void DGMDatumTransformation3Disp::set_ForwardOutputSpaceVerticalDatumVal(long Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_I4;
    params.lVal = Value;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60040004, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return;
}

LPTSTR DGMDatumTransformation3Disp::get_ForwardOutputSpaceVerticalDatumStr()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60040006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
#ifdef UNICODE
    return(pVarResult.bstrVal);
#else
    int ilen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, NULL, 0, NULL, NULL);
    char *ResBuf = (char*)LocalAlloc(LMEM_FIXED, ilen + 1);
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, pVarResult.bstrVal, -1, ResBuf, ilen, NULL, NULL);
    VariantClear(&pVarResult);
    return(ResBuf);
#endif
}

void DGMDatumTransformation3Disp::set_ForwardOutputSpaceVerticalDatumStr(LPTSTR Value)
{
    EXCEPINFO exi;
    unsigned int argerr;

    BSTR Buf;
    VARIANT params;
    DISPPARAMS dispparams;
    params.vt = VT_BSTR;
#ifdef UNICODE
    Buf = SysAllocString(Value);
#else
    Buf = SysAllocStringLen(NULL, strlen(Value));
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Value, -1, Buf, strlen(Value) + 1);
#endif
    params.bstrVal = Buf;
    dispparams.rgvarg = &params;
    dispparams.rgdispidNamedArgs = &DispIdPropPut;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 1;
    //HRESULT hr =
    Invoke((DISPID)0x60040006, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYPUT, &dispparams, NULL, &exi, &argerr);
    SysFreeString(Buf);
    //HandleError(hr, &exi, argerr);
    return;
}

DGMStringCollection* DGMDatumTransformation3Disp::get_VerticalDatumStrings()
{
    EXCEPINFO exi;
    unsigned int argerr;

    VARIANT pVarResult;
    VariantInit(&pVarResult);
    //HRESULT hr =
    Invoke((DISPID)0x60040008, IID_NULL, (LCID)LOCALE_USER_DEFAULT,
        (WORD)DISPATCH_PROPERTYGET, &dispparamsNoArgs, &pVarResult, &exi, &argerr);
    //HandleError(hr, &exi, argerr);
    return((DGMStringCollection*)pVarResult.pdispVal);
}

DatumTransformation* CreateDatumTransformation()
{
    DGMDatumTransformation3 *res;
    CoCreateInstance(CLASS_DatumTransformation, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMDatumTransformation3, (void**)&res);
    return(res);
}

CoordSystemsMgr* CreateCoordSystemsMgr()
{
    DGMCoordSystemsMgr *res;
    CoCreateInstance(CLASS_CoordSystemsMgr, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMCoordSystemsMgr, (void**)&res);
    return(res);
}

UnitsOfMeasure* CreateUnitsOfMeasure()
{
    DGMUnitsOfMeasure *res;
    CoCreateInstance(CLASS_UnitsOfMeasure, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMUnitsOfMeasure, (void**)&res);
    return(res);
}

BursaWolf* CreateBursaWolf()
{
    DGMBursaWolf *res;
    CoCreateInstance(CLASS_BursaWolf, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMBursaWolf, (void**)&res);
    return(res);
}

ComplexPolynomial* CreateComplexPolynomial()
{
    DGMComplexPolynomial *res;
    CoCreateInstance(CLASS_ComplexPolynomial, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMComplexPolynomial, (void**)&res);
    return(res);
}

MultipleRegression* CreateMultipleRegression()
{
    DGMMultipleRegression *res;
    CoCreateInstance(CLASS_MultipleRegression, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMMultipleRegression, (void**)&res);
    return(res);
}

SecondDegreePolynomial* CreateSecondDegreePolynomial()
{
    DGMSecondDegreePolynomial *res;
    CoCreateInstance(CLASS_SecondDegreePolynomial, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMSecondDegreePolynomial, (void**)&res);
    return(res);
}

StandardMolodensky* CreateStandardMolodensky()
{
    DGMStandardMolodensky *res;
    CoCreateInstance(CLASS_StandardMolodensky, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMStandardMolodensky, (void**)&res);
    return(res);
}

LeastSquares* CreateLeastSquares()
{
    DGMLeastSquares *res;
    CoCreateInstance(CLASS_LeastSquares, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMLeastSquares, (void**)&res);
    return(res);
}

SingleParameterConstraints* CreateSingleParameterConstraints()
{
    DGMSingleParameterConstraints *res;
    CoCreateInstance(CLASS_SingleParameterConstraints, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMSingleParameterConstraints, (void**)&res);
    return(res);
}

SingleParameterConstraint* CreateSingleParameterConstraint()
{
    DGMSingleParameterConstraint *res;
    CoCreateInstance(CLASS_SingleParameterConstraint, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMSingleParameterConstraint, (void**)&res);
    return(res);
}

Observations* CreateObservations()
{
    DGMObservations *res;
    CoCreateInstance(CLASS_Observations, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMObservations, (void**)&res);
    return(res);
}

Observation* CreateObservation()
{
    DGMObservation *res;
    CoCreateInstance(CLASS_Observation, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMObservation, (void**)&res);
    return(res);
}

LinearModelResidual* CreateLinearModelResidual()
{
    DGMLinearModelResidual *res;
    CoCreateInstance(CLASS_LinearModelResidual, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMLinearModelResidual, (void**)&res);
    return(res);
}

NonlinearModelResidual* CreateNonlinearModelResidual()
{
    DGMNonlinearModelResidual *res;
    CoCreateInstance(CLASS_NonlinearModelResidual, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, DIID_DGMNonlinearModelResidual, (void**)&res);
    return(res);
}

