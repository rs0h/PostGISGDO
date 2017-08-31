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
#include <olectl.h>
#include "../Common/Registrar.hpp"
#include "GMPsgCtl.rh"
#include "GMPsgCtlClsFactory.hpp"
#include "../Common/LogFiles.hpp"
#include "GMPsgCtlIntfc.hpp"

extern wchar_t g_ModuleName[MAX_PATH];

CClsFacList *g_List;
HINSTANCE g_hinstDLL;

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    // Perform actions based on the reason for calling.
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            g_ModuleName[0] = 0;
            g_hinstDLL = hinstDLL;
            g_List = NULL;
            break;
        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;
        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;
        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            if(g_List)
            {
                delete g_List;
#ifdef DBGLEVEL
                FreeLogFileName();
#endif // DBGLEVEL
            }
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

STDAPI DllCanUnloadNow()
{
    return(g_List->IsEmpty() ? S_OK : S_FALSE);
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    if(!g_List)
    {
        if(!g_ModuleName[0])
            GetModuleFileName(g_hinstDLL, g_ModuleName, MAX_PATH);
#ifdef DBGLEVEL
        CreateLogFile();
#endif // DBGLEVEL
        g_List = new CClsFacList(g_hinstDLL);
    }
#if DBGLEVEL > 0
    WriteLogFile("DllGetClassObject\r\n", true);
#if DBGLEVEL > 2
    char buf[64];
    FormatGuid(buf, "  rclsid: ", "\r\n", rclsid);
    WriteLogFile(buf, false);
    FormatGuid(buf, "  riid: ", "\r\n", riid);
    WriteLogFile(buf, false);
#endif // DBGLEVEL
#endif // DBGLEVEL
    *ppv = NULL;
    if(IsEqualIID(riid, IID_IClassFactory))
    {
        *ppv = g_List->GetClassFactory(rclsid);
    }
    else if(IsEqualCLSID(rclsid, CLASS_GMPostGISConnectionProps) &&
        (IsEqualIID(riid, DIID___GMPostGISConnectionProps) || IsEqualIID(riid, IID_IUnknown)))
    {
        // For some reason, GeoMedia requests class id of the control,
        // however, it wants the IClassFactory interface in fact
        *ppv = g_List->GetClassFactory(rclsid);
        //IClassFactory *pcf = (IClassFactory*)g_List->GetClassFactory(rclsid);
        //pcf->CreateInstance(NULL, riid, ppv);
        //*ppv = new _GMPostGISConnectionProps(NULL, NULL, NULL);
    }
    /*else if(IsEqualCLSID(rclsid, CLASS_GMPostGISNativeQueryProps) && (IsEqualIID(riid, DIID_GMPostGISNativeQueryProps) || IsEqualIID(riid, IID_IUnknown)))
    {
        *ppv = new GMPostGISNativeQueryProps(NULL, NULL);
    }*/
    if(*ppv)
    {
        ((IUnknown*)*ppv)->AddRef();
        ((IUnknown*)*ppv)->AddRef(); // ??? - seems to be necessary si that
            // the class factory does not get released
        return(S_OK);
    }
    else return(CLASS_E_CLASSNOTAVAILABLE);
}

STDAPI DllRegisterServer()
{
    HRESULT hres = SELFREG_E_CLASS;
    IRegistrar *reg = NULL;
    if((CoCreateInstance(CLASS_Registrar, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IRegistrar,
        (void**)&reg) == S_OK) && reg)
    {
        if(!g_ModuleName[0])
            GetModuleFileName(g_hinstDLL, g_ModuleName, MAX_PATH);
        reg->AddReplacement(L"MODULE", g_ModuleName);
        hres = reg->ResourceRegister(g_ModuleName, REG_MAIN, L"REGISTRY");
        reg->Release();
    }
    return(hres);
}

STDAPI DllUnregisterServer()
{
    HRESULT hres = SELFREG_E_CLASS;
    IRegistrar *reg = NULL;
    if((CoCreateInstance(CLASS_Registrar, NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IRegistrar,
        (void**)&reg) == S_OK) && reg)
    {
        if(!g_ModuleName[0])
            GetModuleFileName(g_hinstDLL, g_ModuleName, MAX_PATH);
        reg->AddReplacement(L"MODULE", g_ModuleName);
        hres = reg->ResourceUnregister(g_ModuleName, REG_MAIN, L"REGISTRY");
        reg->Release();
    }
    return(hres);
}
