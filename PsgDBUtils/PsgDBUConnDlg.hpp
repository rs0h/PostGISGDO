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

#ifndef _PSGDBUCONNDLG_HPP_
#define _PSGDBUCONNDLG_HPP_

#include <windows.h>

typedef struct CDBUConnProps
{
    TCHAR sServer[64];
    TCHAR sPort[16];
    TCHAR sDBName[64];
    TCHAR sUsrName[64];
    TCHAR sPasswd[64];
    bool bSave;
} *PDBUConnProps;

class CDBUConnDlg
{
private:
    HWND m_hwndMain, m_hwndDlg;
    PDBUConnProps m_pCP;
    CDBUConnProps m_CP;

    INT_PTR OKBtnClick(HWND hwndDlg);
    INT_PTR CancelBtnClick(HWND hwndDlg);
    INT_PTR EditCmd(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);

    bool CanClose(HWND hwndDlg);
public:
    HINSTANCE m_hInstance;

    CDBUConnDlg(HINSTANCE hInstance);
    ~CDBUConnDlg();
    bool ShowModal(HWND hWnd, PDBUConnProps pCP);

    // main dialog
    INT_PTR WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    INT_PTR WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
};

#endif
