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

#include "PsgDBUSpecFldEdt.hpp"
#include <tchar.h>
#include "PsgDBUtils.rh"

INT_PTR CALLBACK DBUSpecFldDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    CDBUSpecFldDlg *cw = NULL;
    if(uMsg == WM_INITDIALOG) cw = (CDBUSpecFldDlg*)lParam;
    else cw = (CDBUSpecFldDlg*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

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

CDBUSpecFldDlg::CDBUSpecFldDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_lConnHandle = 0;
    m_hwndMain = NULL;
    m_hwndDlg = NULL;
    m_pTables = NULL;
    m_pCurTbl = NULL;
    m_bSetup = false;
}

CDBUSpecFldDlg::~CDBUSpecFldDlg()
{
    delete m_pTables;
}

bool CDBUSpecFldDlg::ShowModal(HWND hWnd, long lConnHandle)
{
    m_hwndMain = hWnd;
    m_lConnHandle = lConnHandle;
    m_pTables = DbuGetTables(lConnHandle);
    return(DialogBoxParam(m_hInstance, _T("SPECCOLLEDTDLG"), hWnd,
        DBUSpecFldDlgProc, (LPARAM)this));
}

void CDBUSpecFldDlg::SetupListBox(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_SCED_FTRLB);

    int n = m_pTables->GetCount();
    int idx;
    IDbuTable *ptbl;
    for(int i = 0; i < n; i++)
    {
        ptbl = m_pTables->GetItem(i);
        if(CanTableHaveColumn(ptbl))
        {
            idx = SendMessage(lv, LB_ADDSTRING, 0, (LPARAM)ptbl->GetNamePtr());
            SendMessage(lv, LB_SETITEMDATA, idx, (LPARAM)ptbl);
        }
    }
    return;
}

INT_PTR CDBUSpecFldDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus,
    LPARAM lInitParam)
{
    m_hwndDlg = hwndDlg;

    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lInitParam);

    m_bSetup = true;
    SendDlgItemMessage(hwndDlg, CTR_FCD_FTRDESCEDT, EM_LIMITTEXT, 256, 0);

    SetupListBox(hwndDlg);
    ShowWindow(GetDlgItem(hwndDlg, CTR_SCED_MANAGEFLDBTN), SW_HIDE);
    m_bSetup = false;
    return(1);
}

INT_PTR CDBUSpecFldDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    switch(wID)
    {
    case IDCANCEL:
        return(CancelBtnClick(hwndDlg));
    case CTR_SCED_FTRLB:
        return(FtrsLBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_SCED_GEOMCB:
        return(GeomCBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_SCED_MANAGEFLDBTN:
        return(ManageFldsBtnClick(hwndDlg));
    default:
        return(0);
    }
}

INT_PTR CDBUSpecFldDlg::CancelBtnClick(HWND hwndDlg)
{
    EndDialog(hwndDlg, 0);
    return(1);
}

INT_PTR CDBUSpecFldDlg::FtrsLBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(wNotifyCode == LBN_SELCHANGE)
    {
        HWND cb = GetDlgItem(hwndDlg, CTR_SCED_GEOMCB);
        SendMessage(cb, CB_RESETCONTENT, 0, 0);

        int idx = SendMessage(hwndCtl, LB_GETCURSEL, 0, 0);
        if(idx != LB_ERR)
        {
            m_pCurTbl = (IDbuTable*)SendMessage(hwndCtl, LB_GETITEMDATA, idx, 0);
            GetTableColumns(cb, m_pCurTbl);
            GeomCBCmd(hwndDlg, CBN_SELCHANGE, cb);
            EnableWindow(cb, TRUE);
        }
        else
        {
            m_pCurTbl = NULL;
            EnableWindow(cb, FALSE);
        }
    }
    return(0);
}

INT_PTR CDBUSpecFldDlg::GeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(wNotifyCode == CBN_SELCHANGE)
    {
        HWND lab = GetDlgItem(hwndDlg, CTR_SCED_GEOMTYPELAB);
        HWND btn = GetDlgItem(hwndDlg, CTR_SCED_MANAGEFLDBTN);
        int idx = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
        if(idx != CB_ERR)
        {
            TCHAR sBuf[64];
            IDbuField *pFld = (IDbuField*)
                SendMessage(hwndCtl, CB_GETITEMDATA, idx, 0);
            switch(pFld->GetSubType())
            {
            case 3:
                LoadString(m_hInstance, IDS_ANYSPATIAL, sBuf, 64);
                break;
            case 4:
                LoadString(m_hInstance, IDS_RASTER, sBuf, 64);
                break;
            case 5:
                LoadString(m_hInstance, IDS_GRAPHTEXT, sBuf, 64);
                break;
            case 10:
                LoadString(m_hInstance, IDS_POINT, sBuf, 64);
                break;
            default:
                sBuf[0] = 0;
            }
            SendMessage(lab, WM_SETTEXT, 0, (LPARAM)sBuf);
            SetButtonText(btn, pFld);
            ShowWindow(btn, SW_SHOW);
        }
        else
        {
            SendMessage(lab, WM_SETTEXT, 0, (LPARAM)_T(""));
            ShowWindow(btn, SW_HIDE);
        }
    }
    return(0);
}

INT_PTR CDBUSpecFldDlg::ManageFldsBtnClick(HWND hwndDlg)
{
    UpdateGeomFields(hwndDlg);
    return(0);
}

void CDBUSpecFldDlg::UpdateGeomFields(HWND hwndDlg)
{
    HWND cb = GetDlgItem(hwndDlg, CTR_SCED_GEOMCB);
    int idx = SendMessage(cb, CB_GETCURSEL, 0, 0);
    if(idx != CB_ERR)
    {
        IDbuField *pFld = (IDbuField*)SendMessage(cb, CB_GETITEMDATA, idx, 0);
        bool bRes = m_pCurTbl->ChangeSpecField(pFld);
        TCHAR sMsg[64];
        TCHAR sCap[32];
        UINT iFlag = MB_OK;
        if(bRes)
        {
            SetButtonText(GetDlgItem(hwndDlg, CTR_SCED_MANAGEFLDBTN), pFld);
            LoadString(m_hInstance, IDS_INFOBASE, sCap, 32);
            LoadString(m_hInstance, IDI_DONE, sMsg, 64);
            iFlag |= MB_ICONINFORMATION;
        }
        else
        {
            LoadString(m_hInstance, IDS_ERRORBASE, sCap, 32);
            LoadString(m_hInstance, IDE_FAIL, sMsg, 64);
            iFlag |= MB_ICONERROR;
        }
        MessageBox(hwndDlg, sMsg, sCap, iFlag);
    }
    return;
}

bool CDBUSpecFldDlg::CanTableHaveColumn(IDbuTable *pTbl)
{
    bool bRes = false;

    IDbuFields *pFlds = pTbl->GetFields();
    IDbuField *pFld;
    int iType;
    long lSubType;

    int iFlds = pFlds->GetCount();
    int i = 0;

    while(!bRes && (i < iFlds))
    {
        pFld = pFlds->GetItem(i++);
        iType = pFld->GetType();
        if(iType > 31)
        {
            lSubType = pFld->GetSubType();
            bRes = (lSubType > 2);
        }
    }

    return(bRes);
}

void CDBUSpecFldDlg::GetTableColumns(HWND hwndCB, IDbuTable *pTbl)
{
    IDbuFields *pFlds = pTbl->GetFields();
    IDbuField *pFld;
    int iType;
    long lSubType;
    int idx;

    int iFlds = pFlds->GetCount();
    for(int i = 0; i < iFlds; i++)
    {
        pFld = pFlds->GetItem(i);
        iType = pFld->GetType();
        if(iType > 31)
        {
            lSubType = pFld->GetSubType();
            if(lSubType > 2)
            {
                idx = SendMessage(hwndCB, CB_ADDSTRING, 0,
                    (LPARAM)pFld->GetNamePtr());
                SendMessage(hwndCB, CB_SETITEMDATA, idx, (LPARAM)pFld);
            }
        }
    }
    SendMessage(hwndCB, CB_SETCURSEL, 0, 0);
    return;
}

void CDBUSpecFldDlg::SetButtonText(HWND hwndBtn, IDbuField *pFld)
{
    long lSubType = pFld->GetSubType();
    bool bHasField = pFld->GetHasSpecField();
    TCHAR sBuf[64];

    if((lSubType == 3) || (lSubType == 10))
    {
        if(bHasField) LoadString(m_hInstance, IDS_DROPINGRFLD, sBuf, 64);
        else LoadString(m_hInstance, IDS_CREATEINGRFLD, sBuf, 64);
    }
    else
    {
        if(bHasField) LoadString(m_hInstance, IDS_DROPNATIVEFLD, sBuf, 64);
        else LoadString(m_hInstance, IDS_CREATENATIVEFLD, sBuf, 64);
    }
    SendMessage(hwndBtn, WM_SETTEXT, 0, (LPARAM)sBuf);
    return;
}
