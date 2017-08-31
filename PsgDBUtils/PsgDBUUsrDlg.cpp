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

#include "PsgDBUUsrDlg.hpp"
#include "PsgDBUtils.rh"
#include "../Common/VarUtils.hpp"
#include <tchar.h>
#include "../Common/PsgDBUInterfaces.hpp"

INT_PTR CALLBACK DBUUsrDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    CDBUUsrDlg *cw = NULL;
    if(uMsg == WM_INITDIALOG) cw = (CDBUUsrDlg*)lParam;
    else cw = (CDBUUsrDlg*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

    switch(uMsg)
    {
    case WM_INITDIALOG:
        return(cw->WMInitDialog(hwndDlg, (HWND)wParam, lParam));
    case WM_COMMAND:
        return(cw->WMCommand(hwndDlg, HIWORD(wParam), LOWORD(wParam),
            (HWND)lParam));
    default:
        return(FALSE);
    }
}

CDBUUsrDlg::CDBUUsrDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_lConnHandle = 0;
    m_hwndMain = NULL;
    m_hwndDlg = NULL;
}

CDBUUsrDlg::~CDBUUsrDlg()
{
}

bool CDBUUsrDlg::ShowModal(HWND hWnd, long lConnHandle)
{
    m_hwndMain = hWnd;
    m_lConnHandle = lConnHandle;
    return(DialogBoxParam(m_hInstance, _T("USERSDLG"), hWnd,
        DBUUsrDlgProc, (LPARAM)this));
}

// Main dialog Windows Messages

INT_PTR CDBUUsrDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus,
    LPARAM lInitParam)
{
    m_hwndDlg = hwndDlg;

    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lInitParam);

    TCHAR sBuf[64];
    HWND cb = GetDlgItem(hwndDlg, CTR_URD_RIGHTSCB);
    for(int i = IDS_RIGHTNONE; i <= IDS_RIGHTADMIN; i++)
    {
        LoadString(m_hInstance, i, sBuf, 64);
        SendMessage(cb, CB_ADDSTRING, 0, (LPARAM)sBuf);
    }
    SendMessage(cb, CB_SETCURSEL, 2, 0);

    cb = GetDlgItem(hwndDlg, CTR_URD_USERSCB);
    DbuFillCtrlWithUserNames(m_lConnHandle, cb, CB_ADDSTRING);
    return(1);
}

INT_PTR CDBUUsrDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    switch(wID)
    {
    case CTR_URD_USERSCB:
        return(UserCBChanged(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_URD_EXECBTN:
        return(ExecBtnClick(hwndDlg));
    case IDCANCEL:
        return(CloseBtnClick(hwndDlg));
    default:
        return(0);
    }
}

INT_PTR CDBUUsrDlg::UserCBChanged(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    int iPos = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
    EnableWindow(GetDlgItem(hwndDlg, CTR_URD_EXECBTN), iPos > -1);
    return(1);
}

INT_PTR CDBUUsrDlg::ExecBtnClick(HWND hwndDlg)
{
    TCHAR tsName[64];
    wchar_t wsName[64];

    SendDlgItemMessage(hwndDlg, CTR_URD_USERSCB, WM_GETTEXT, 64, (LPARAM)tsName);
    TCharToWCharBuf(tsName, wsName, 64);
    int iMode = SendDlgItemMessage(hwndDlg, CTR_URD_RIGHTSCB, CB_GETCURSEL, 0, 0);

    DbuGrantGdoPrivileges(m_lConnHandle, iMode, wsName);

    TCHAR sCaption[32];
    TCHAR sBuf[128];
    LoadString(m_hInstance, IDS_INFOBASE, sCaption, 32);
    LoadString(m_hInstance, IDI_USRGRANTED, sBuf, 128);
    MessageBox(hwndDlg, sBuf, sCaption, MB_OK | MB_ICONINFORMATION);
    return(1);
}

INT_PTR CDBUUsrDlg::CloseBtnClick(HWND hwndDlg)
{
    EndDialog(hwndDlg, 0);
    return(1);
}
