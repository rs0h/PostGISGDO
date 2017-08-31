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

#ifndef _PSGDBUFLDEDTDLG_HPP_
#define _PSGDBUFLDEDTDLG_HPP_

#include "../Common/PsgDBUInterfaces.hpp"
#include <commctrl.h>

class CDBUFldEdtDlg
{
private:
    HWND m_hwndMain, m_hwndDlg;
    IDbuTable *m_pTbl;
    long m_lConnHandle;

    INT_PTR OKBtnClick(HWND hwndDlg);
    INT_PTR FldTypeCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR FldDescEditCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR FldGeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR FldSridCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR SelAllBtnClick(HWND hwndDlg);
    INT_PTR SelNoneBtnClick(HWND hwndDlg);
    INT_PTR KeyChBClick(HWND hwndDlg, HWND hwndCtl);
    INT_PTR HyperChBClick(HWND hwndDlg, HWND hwndCtl);

    bool CanClose(HWND hwndDlg);
    void SetupListView(HWND hwndDlg);
    INT_PTR FldListNotify(HWND hwndDlg, LPNMHDR pnmh);
    INT_PTR FldListItemChanged(HWND hwndDlg, LPNMLISTVIEW pnmv);
    void EnableControls(HWND hwndDlg, bool bEnable);
    void SetupControls(HWND hwndDlg, IDbuField *pFld);
public:
    HINSTANCE m_hInstance;
    IDbuField *m_pCurFld;

    CDBUFldEdtDlg(HINSTANCE hInstance);
    ~CDBUFldEdtDlg();
    bool ShowModal(HWND hWnd, IDbuTable *pTbl, long lConnHandle);

    // main dialog
    INT_PTR WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    INT_PTR WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
    INT_PTR WMNotify(HWND hwndDlg, int idCtrl, LPNMHDR pnmh);
};

#endif
