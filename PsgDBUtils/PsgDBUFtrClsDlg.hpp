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

#ifndef _PSGDBUFTRCLSDLG_HPP_
#define _PSGDBUFTRCLSDLG_HPP_

#include "PsgDBUFldEdtDlg.hpp"
#include "../Common/PsgDBUInterfaces.hpp"
#include <commctrl.h>

class CDBUFtrClsDlg
{
private:
    HWND m_hwndMain, m_hwndDlg;
    CDBUFldEdtDlg *m_pFldEdtDlg;
    IDbuTables *m_pTables;
    IDbuTable *m_pCurTbl;
    long m_lConnHandle;
    bool m_bSetup;

    INT_PTR OKBtnClick(HWND hwndDlg);
    INT_PTR CancelBtnClick(HWND hwndDlg);
    INT_PTR DescEditCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR PrimGeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR EditFldsBtnClick(HWND hwndDlg);
    INT_PTR SelAllBtnClick(HWND hwndDlg);
    INT_PTR SelNoneBtnClick(HWND hwndDlg);

    bool CanClose(HWND hwndDlg);
    void SetupListView(HWND hwndDlg);
    INT_PTR FtrListNotify(HWND hwndDlg, LPNMHDR pnmh);
    INT_PTR FtrListItemActivate(HWND hwndDlg, LPNMITEMACTIVATE lpnmia);
    INT_PTR FtrListItemChanged(HWND hwndDlg, LPNMLISTVIEW pnmv);
    void EnableControls(HWND hwndDlg, bool bEnable);
    void SetupControls(HWND hwndDlg, IDbuTable *pTbl);
    //void GetSridList(HWND hwndDlg);
    void UpdateGeomFields(HWND hwndDlg);
public:
    HINSTANCE m_hInstance;

    CDBUFtrClsDlg(HINSTANCE hInstance);
    ~CDBUFtrClsDlg();
    bool ShowModal(HWND hWnd, long lConnHandle);

    // main dialog
    INT_PTR WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    INT_PTR WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
    INT_PTR WMNotify(HWND hwndDlg, int idCtrl, LPNMHDR pnmh);
};

#endif
