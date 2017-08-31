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

#ifndef _PSGDBUMAINDLG_HPP_
#define _PSGDBUMAINDLG_HPP_

#include <windows.h>
#include "PsgDBUConnDlg.hpp"
#include "PsgDBUFtrClsDlg.hpp"
#include "PsgDBUUsrDlg.hpp"
#include "PsgDBUSpecFldEdt.hpp"

class CDBMainDlg
{
private:
    HINSTANCE m_hInstance;

    CDBUConnProps m_ConnProps;
    //CConnStruct m_cConnStruct;
    long m_lConnHandle;
    CDBUConnDlg *m_pConnDlg;
    CDBUFtrClsDlg *m_pFtrClsDlg;
    CDBUUsrDlg *m_pUsrDlg;
    CDBUSpecFldDlg *m_pSpecFldEdt;

    LPTSTR m_tsIniFileName;

    BOOL ExitCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL OpenDBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    bool CloseDatabase(HWND hwndDlg);
    void DoOpenDatabase(HWND hwndDlg);
    BOOL GrantMetadataPrivileges(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL EditFeatureClassMetadata(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL DeleteFeatureClassMetadata(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL DropMetadataTables(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL CreateSimpleMetadataTables(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL CreateIngrMetadataTables(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL ClearModificationLog(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL CreateModificationLogTriggers(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL HelpContent(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    BOOL HelpAbout(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);

    void GetIniFileName();
    bool IsPrivilegeProblem();
    void UpdateButtons(HWND hwndDlg);
    void ShowConnDetails(HWND hwndDlg, bool bShow);
public:
    CDBMainDlg(HINSTANCE hInstance);
    ~CDBMainDlg();

    HWND DisplayWindow();

    BOOL WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    BOOL WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
    BOOL WMClose(HWND hwndDlg);
};

#endif
