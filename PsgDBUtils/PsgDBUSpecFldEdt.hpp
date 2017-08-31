// Copyright 2013 Intergraph Corporation
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

#ifndef _PSGDBUSPECFLDEDT_HPP_
#define _PSGDBUSPECFLDEDT_HPP_

#include "../Common/PsgDBUInterfaces.hpp"

class CDBUSpecFldDlg
{
private:
    HWND m_hwndMain, m_hwndDlg;
    IDbuTables *m_pTables;
    IDbuTable *m_pCurTbl;
    long m_lConnHandle;
    bool m_bSetup;

    INT_PTR CancelBtnClick(HWND hwndDlg);
    INT_PTR FtrsLBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR GeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR ManageFldsBtnClick(HWND hwndDlg);

    void SetupListBox(HWND hwndDlg);
    void UpdateGeomFields(HWND hwndDlg);
    bool CanTableHaveColumn(IDbuTable *pTbl);
    void GetTableColumns(HWND hwndCB, IDbuTable *pTbl);
    void SetButtonText(HWND hwndBtn, IDbuField *pFld);
public:
    HINSTANCE m_hInstance;

    CDBUSpecFldDlg(HINSTANCE hInstance);
    ~CDBUSpecFldDlg();
    bool ShowModal(HWND hWnd, long lConnHandle);

    // main dialog
    INT_PTR WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    INT_PTR WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
};

#endif
