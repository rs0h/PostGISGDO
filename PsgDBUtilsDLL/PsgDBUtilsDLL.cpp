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
#include "../Common/LogFiles.hpp"
#include "PsgDBUDLLMain.hpp"

extern wchar_t g_ModuleName[MAX_PATH];

PConnList g_pList;
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
            g_pList = NULL;
            break;
        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;
        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;
        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            if(g_pList)
            {
                delete g_pList;
#ifdef DBGLEVEL
                FreeLogFileName();
#endif // DBGLEVEL
            }
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

// returns non zero connection handle, returns zero in case of an error
long _stdcall DbuOpenConnection(LPWSTR sServer, LPWSTR sDatabase,
    LPWSTR sUser, LPWSTR sPasswd, int iPort)
{
    if(!g_pList)
    {
        if(!g_ModuleName[0])
            GetModuleFileName(g_hinstDLL, g_ModuleName, MAX_PATH);
#ifdef DBGLEVEL
        CreateLogFile();
#endif // DBGLEVEL
        g_pList = new CConnList(g_hinstDLL);
    }
#if DBGLEVEL > 0
    WriteLogFile("DbuOpenConnection\r\n", true);
#endif // DBGLEVEL
    return(g_pList->AddConnection(g_hinstDLL, sServer, sDatabase, sUser,
        sPasswd, iPort));
}

// returns true if succesful
bool _stdcall DbuCloseConnection(long lHandle)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuCloseConnection\r\n", true);
#endif // DBGLEVEL
    return(g_pList->CloseConnection(lHandle));
}

CConnStatus _stdcall DbuGetConnectionStatus(long lHandle)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuGetConnectionStatus\r\n", true);
#endif // DBGLEVEL
    return(g_pList->GetConnStatus(lHandle));
}

bool _stdcall DbuCreateMetadata(long lHandle, bool bFull)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuCreateMetadata\r\n", true);
#endif // DBGLEVEL
    if(bFull) return(g_pList->CreateINGRMetadata(lHandle));

    return(g_pList->CreateSimpleMetadata(lHandle));
}

bool _stdcall DbuDropMetadata(long lHandle)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuDropMetadata\r\n", true);
#endif // DBGLEVEL
    return(g_pList->DropMetadata(lHandle));
}

IDbuTables* _stdcall DbuGetTables(long lHandle)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuGetTables\r\n", true);
#endif // DBGLEVEL
    return(g_pList->GetTables(lHandle));
}

bool _stdcall DbuUpdateAllTables(long lHandle, bool bVisible)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuUpdateAllTables\r\n", true);
#endif // DBGLEVEL
    return(g_pList->UpdateAllTables(lHandle, bVisible));
}

bool _stdcall DbuFillCtrlWithUserNames(long lHandle, HWND hwnd,
    UINT iMsg)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuFillCtrlWithUserNames\r\n", true);
#endif // DBGLEVEL
    return(g_pList->FillCtrlWithUserNames(lHandle, hwnd, iMsg));
}

bool _stdcall DbuFillCtrlWithSrids(long lHandle, HWND hwnd,
    UINT iMsg)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuFillCtrlWithSrids\r\n", true);
#endif // DBGLEVEL
    return(g_pList->FillCtrlWithSrids(lHandle, hwnd, iMsg));
}

bool _stdcall DbuGrantGdoPrivileges(long lHandle, int iMode,
    LPWSTR sUser)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuGrantGdoPrivileges\r\n", true);
#endif // DBGLEVEL
    return(g_pList->GrantGdoPrivileges(lHandle, iMode, sUser));
}

bool _stdcall DbuRunScript(long lHandle, LPWSTR sFileName)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuRunScript\r\n", true);
#endif // DBGLEVEL
    return(g_pList->RunScript(lHandle, sFileName));
}

bool _stdcall DbuClearModificationLog(long lHandle)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuClearModificationLog\r\n", true);
#endif // DBGLEVEL
    return(g_pList->ClearModificationLog(lHandle));
}
