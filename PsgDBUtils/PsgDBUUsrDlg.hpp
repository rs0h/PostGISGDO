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

#ifndef _PSGDBUUSRDLG_
#define _PSGDBUUSRDLG_

#include <windows.h>

class CDBUUsrDlg
{
private:
    HWND m_hwndMain, m_hwndDlg;
    long m_lConnHandle;

    INT_PTR UserCBChanged(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl);
    INT_PTR ExecBtnClick(HWND hwndDlg);
    INT_PTR CloseBtnClick(HWND hwndDlg);
public:
    HINSTANCE m_hInstance;

    CDBUUsrDlg(HINSTANCE hInstance);
    ~CDBUUsrDlg();
    bool ShowModal(HWND hWnd, long lConnHandle);

    // main dialog
    INT_PTR WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam);
    INT_PTR WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl);
};

#endif

