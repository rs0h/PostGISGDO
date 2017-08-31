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

#include "PsgDBUConnDlg.hpp"
#include "PsgDBUtils.rh"
#include <tchar.h>

INT_PTR CALLBACK DBUConnDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    CDBUConnDlg *cw = NULL;
    if(uMsg == WM_INITDIALOG) cw = (CDBUConnDlg*)lParam;
    else cw = (CDBUConnDlg*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

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

CDBUConnDlg::CDBUConnDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_hwndMain = NULL;
    m_hwndDlg = NULL;
}

CDBUConnDlg::~CDBUConnDlg()
{
}

bool CDBUConnDlg::ShowModal(HWND hWnd, PDBUConnProps pCP)
{
    m_hwndMain = hWnd;
    m_pCP = pCP;
    _tcscpy(m_CP.sServer, m_pCP->sServer);
    _tcscpy(m_CP.sPort, m_pCP->sPort);
    _tcscpy(m_CP.sDBName, m_pCP->sDBName);
    _tcscpy(m_CP.sUsrName, m_pCP->sUsrName);
    _tcscpy(m_CP.sPasswd, m_pCP->sPasswd);
    m_CP.bSave = m_pCP->bSave;
    return(DialogBoxParam(m_hInstance, _T("CONNDLG"), hWnd,
        DBUConnDlgProc, (LPARAM)this));
}


// Main dialog Windows Messages

INT_PTR CDBUConnDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus,
    LPARAM lInitParam)
{
    m_hwndDlg = hwndDlg;

    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lInitParam);

    SendDlgItemMessage(hwndDlg, CTL_CON_SERVEREDT, EM_LIMITTEXT, 63, 0);
    SendDlgItemMessage(hwndDlg, CTL_CON_PORTEDT, EM_LIMITTEXT, 15, 0);
    SendDlgItemMessage(hwndDlg, CTL_CON_DBNAMEEDT, EM_LIMITTEXT, 63, 0);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERNAMEEDT, EM_LIMITTEXT, 63, 0);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERPASSWDEDT, EM_LIMITTEXT, 63, 0);

    SendDlgItemMessage(hwndDlg, CTL_CON_SERVEREDT, WM_SETTEXT, 0,
        (LPARAM)m_CP.sServer);
    SendDlgItemMessage(hwndDlg, CTL_CON_PORTEDT, WM_SETTEXT, 0,
        (LPARAM)m_CP.sPort);
    SendDlgItemMessage(hwndDlg, CTL_CON_DBNAMEEDT, WM_SETTEXT, 0,
        (LPARAM)m_CP.sDBName);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERNAMEEDT, WM_SETTEXT, 0,
        (LPARAM)m_CP.sUsrName);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERPASSWDEDT, WM_SETTEXT, 0,
        (LPARAM)m_CP.sPasswd);

    WPARAM wCheck = BST_UNCHECKED;
    if(m_CP.bSave) wCheck = BST_CHECKED;
    SendDlgItemMessage(hwndDlg, CTL_CON_REMEMBER, BM_SETCHECK, wCheck, 0);

    return(1);
}

INT_PTR CDBUConnDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    switch(wID)
    {
    case IDOK:
        return(OKBtnClick(hwndDlg));
    case IDCANCEL:
        return(CancelBtnClick(hwndDlg));
    case CTL_CON_SERVEREDT:
    case CTL_CON_PORTEDT:
    case CTL_CON_DBNAMEEDT:
    case CTL_CON_USERNAMEEDT:
    case CTL_CON_USERPASSWDEDT:
        return(EditCmd(hwndDlg, wNotifyCode, wID, hwndCtl));
    default:
        return(0);
    }
}

INT_PTR CDBUConnDlg::OKBtnClick(HWND hwndDlg)
{
    if(CanClose(hwndDlg))
    {
        _tcscpy(m_pCP->sServer, m_CP.sServer);
        _tcscpy(m_pCP->sPort, m_CP.sPort);
        _tcscpy(m_pCP->sDBName, m_CP.sDBName);
        _tcscpy(m_pCP->sUsrName, m_CP.sUsrName);
        _tcscpy(m_pCP->sPasswd, m_CP.sPasswd);
        m_pCP->bSave = m_CP.bSave;
        EndDialog(hwndDlg, 1);
    }
    return(1);
}

INT_PTR CDBUConnDlg::CancelBtnClick(HWND hwndDlg)
{
    EndDialog(hwndDlg, 0);
    return(1);
}

bool CDBUConnDlg::CanClose(HWND hwndDlg)
{
    SendDlgItemMessage(hwndDlg, CTL_CON_SERVEREDT, WM_GETTEXT, 64,
        (LPARAM)m_CP.sServer);
    SendDlgItemMessage(hwndDlg, CTL_CON_PORTEDT, WM_GETTEXT, 16,
        (LPARAM)m_CP.sPort);
    SendDlgItemMessage(hwndDlg, CTL_CON_DBNAMEEDT, WM_GETTEXT, 64,
        (LPARAM)m_CP.sDBName);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERNAMEEDT, WM_GETTEXT, 64,
        (LPARAM)m_CP.sUsrName);
    SendDlgItemMessage(hwndDlg, CTL_CON_USERPASSWDEDT, WM_GETTEXT, 64,
        (LPARAM)m_CP.sPasswd);
    m_CP.bSave = (SendDlgItemMessage(hwndDlg, CTL_CON_REMEMBER, BM_GETCHECK,
        0, 0) == BST_CHECKED);
    return(m_CP.sServer[0] && m_CP.sDBName[0] && m_CP.sUsrName[0] &&
        m_CP.sPasswd[0]);
}

INT_PTR CDBUConnDlg::EditCmd(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    if(wNotifyCode == EN_CHANGE)
    {
        switch(wID)
        {
        case CTL_CON_SERVEREDT:
            SendMessage(hwndCtl, WM_GETTEXT, 64, (LPARAM)m_CP.sServer);
            break;
        case CTL_CON_PORTEDT:
            SendMessage(hwndCtl, WM_GETTEXT, 16, (LPARAM)m_CP.sPort);
            break;
        case CTL_CON_DBNAMEEDT:
            SendMessage(hwndCtl, WM_GETTEXT, 64, (LPARAM)m_CP.sDBName);
            break;
        case CTL_CON_USERNAMEEDT:
            SendMessage(hwndCtl, WM_GETTEXT, 64, (LPARAM)m_CP.sUsrName);
            break;
        case CTL_CON_USERPASSWDEDT:
            SendMessage(hwndCtl, WM_GETTEXT, 64, (LPARAM)m_CP.sPasswd);
            break;
        }
        EnableWindow(GetDlgItem(hwndDlg, IDOK), m_CP.sServer[0] &&
            m_CP.sDBName[0] && m_CP.sUsrName[0] && m_CP.sPasswd[0]);
    }
    return(0);
}
