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

#ifndef _PSGDBUDLLMAIN_HPP_
#define _PSGDBUDLLMAIN_HPP_

#include <windows.h>
#include "../Common/GDOUtils.hpp"
#include "../Common/PsgDBUInterfaces.hpp"
#include "PsgDBUTables.hpp"

typedef class CConnList
{
private:
    int m_iListSize;
    int m_iListLen;
    PConnStruct *m_ppData;
    PDbuTables *m_ppTables;
    HINSTANCE m_hInstance;
    bool CheckGAliasTable(PConnStruct pCStr);
    bool InsertSimpleMetadata(PConnStruct pCStr);
    bool InsertINGRMetadata(PConnStruct pCStr);
    void PopulateCoordSystemTable(PConnStruct pCStr, LPSTR sTblName);
    bool CheckRefTable(PConnStruct pCStr, LPCSTR sStmt, LPCSTR sTypeName,
        LPCSTR sTblName, LPCSTR sTblDef, LPSTR sNewNameBuf);
    void PopulateSQLOperatorTable(PConnStruct pCStr, LPSTR sTblName);
    bool CheckGraphicColumnsTable(PConnStruct pCStr);
    bool CheckGFeaturesTable(PConnStruct pCStr, LPCSTR sStmt, LPCSTR sTypeName,
        LPCSTR sTblName, LPSTR sNewNameBuf);
public:
    CConnList(HINSTANCE hInstance);
    ~CConnList();
    long AddConnection(HINSTANCE hInstance, LPWSTR sServer, LPWSTR sDatabase,
        LPWSTR sUser, LPWSTR sPassword, int iPort);
    bool CloseConnection(long lHandle);
    CConnStatus GetConnStatus(long lHandle);
    bool CreateSimpleMetadata(long lHandle);
    bool CreateINGRMetadata(long lHandle);
    bool DropMetadata(long lHandle);
    IDbuTables* GetTables(long lHandle);
    bool UpdateAllTables(long lHandle, bool bVisible);
    bool FillCtrlWithUserNames(long lHandle, HWND hwnd, UINT iMsg);
    bool FillCtrlWithSrids(long lHandle, HWND hwnd, UINT iMsg);
    bool GrantGdoPrivileges(long lHandle, int iMode, LPWSTR sUser);
    bool RunScript(long lHandle, LPWSTR sFileName);
    bool ClearModificationLog(long lHandle);
} *PConnList;

#endif
