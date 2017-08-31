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

#include "PsgDBUFtrClsDlg.hpp"
#include <tchar.h>
#include "PsgDBUtils.rh"

INT_PTR CALLBACK DBUFtrClsDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    CDBUFtrClsDlg *cw = NULL;
    if(uMsg == WM_INITDIALOG) cw = (CDBUFtrClsDlg*)lParam;
    else cw = (CDBUFtrClsDlg*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

    switch(uMsg)
    {
    case WM_INITDIALOG:
        return(cw->WMInitDialog(hwndDlg, (HWND)wParam, lParam));
    case WM_COMMAND:
        return(cw->WMCommand(hwndDlg, HIWORD(wParam), LOWORD(wParam),
            (HWND)lParam));
    case WM_NOTIFY:
        return(cw->WMNotify(hwndDlg, (int)wParam, (LPNMHDR)lParam));
    default:
        return(FALSE);
    }
}

CDBUFtrClsDlg::CDBUFtrClsDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_lConnHandle = 0;
    m_hwndMain = NULL;
    m_hwndDlg = NULL;
    m_pFldEdtDlg = NULL;
    m_pTables = NULL;
    m_pCurTbl = NULL;
    m_bSetup = false;
}

CDBUFtrClsDlg::~CDBUFtrClsDlg()
{
    if(m_pFldEdtDlg) delete m_pFldEdtDlg;
    delete m_pTables;
}

bool CDBUFtrClsDlg::ShowModal(HWND hWnd, long lConnHandle)
{
    m_hwndMain = hWnd;
    m_lConnHandle = lConnHandle;
    m_pTables = DbuGetTables(lConnHandle);
    return(DialogBoxParam(m_hInstance, _T("FTRCLASSESDLG"), hWnd,
        DBUFtrClsDlgProc, (LPARAM)this));
}

void CDBUFtrClsDlg::SetupListView(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    SendMessage(lv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_CHECKBOXES,
        LVS_EX_CHECKBOXES);
    LVCOLUMN lvc = {LVCF_TEXT, LVCFMT_LEFT, 0, (LPTSTR)_T("Tables"),
        0, 0, 0, 0};
    SendMessage(lv, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

    int n = m_pTables->GetCount();
    int idx;
    IDbuTable *ptbl;
    LVITEM lvi = {LVIF_TEXT | LVIF_PARAM | LVIF_STATE, 0, 0, 0, 0, NULL, 0, 0,
        0, 0};
    for(int i = 0; i < n; i++)
    {
        ptbl = m_pTables->GetItem(i);
        lvi.iItem = i;
        lvi.pszText = ptbl->GetNamePtr();
        lvi.lParam = (LPARAM)ptbl;
        lvi.stateMask = LVIS_STATEIMAGEMASK;
        lvi.state = INDEXTOSTATEIMAGEMASK((ptbl->GetVisible()) + 1);
        idx = SendMessage(lv, LVM_INSERTITEM, 0, (LPARAM)&lvi);
        SendMessage(lv, LVM_SETITEMSTATE, idx, (LPARAM)&lvi);
    }

    RECT R;
    GetClientRect(lv, &R);
    SendMessage(lv, LVM_SETCOLUMNWIDTH, 0, LVSCW_AUTOSIZE);
    n = SendMessage(lv, LVM_GETCOLUMNWIDTH, 0, 0);
    if(n < R.right) SendMessage(lv, LVM_SETCOLUMNWIDTH, 0, R.right);
    return;
}


// Main dialog Windows Messages

INT_PTR CDBUFtrClsDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus,
    LPARAM lInitParam)
{
    m_hwndDlg = hwndDlg;

    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lInitParam);

    m_bSetup = true;
    SendDlgItemMessage(hwndDlg, CTR_FCD_FTRDESCEDT, EM_LIMITTEXT, 256, 0);

    SetupListView(hwndDlg);
    m_bSetup = false;
    return(1);
}

INT_PTR CDBUFtrClsDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    switch(wID)
    {
    case IDOK:
        return(OKBtnClick(hwndDlg));
    case IDCANCEL:
        return(CancelBtnClick(hwndDlg));
    case CTR_FCD_PRIMGEOMCB:
        return(PrimGeomCBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FCD_FTRDESCEDT:
        return(DescEditCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FCD_EDTFLDSBTN:
        return(EditFldsBtnClick(hwndDlg));
    case CTR_FCD_SELALLBTN:
        return(SelAllBtnClick(hwndDlg));
    case CTR_FCD_SELNONEBTN:
        return(SelNoneBtnClick(hwndDlg));
    default:
        return(0);
    }
}

INT_PTR CDBUFtrClsDlg::OKBtnClick(HWND hwndDlg)
{
    if(CanClose(hwndDlg))
    {
        EndDialog(hwndDlg, 1);
    }
    return(1);
}

INT_PTR CDBUFtrClsDlg::CancelBtnClick(HWND hwndDlg)
{
    EndDialog(hwndDlg, 0);
    return(1);
}

INT_PTR CDBUFtrClsDlg::EditFldsBtnClick(HWND hwndDlg)
{
    if(!m_pFldEdtDlg) m_pFldEdtDlg = new CDBUFldEdtDlg(m_hInstance);
    m_pFldEdtDlg->ShowModal(hwndDlg, m_pCurTbl, m_lConnHandle);
    UpdateGeomFields(hwndDlg);
    return(0);
}

INT_PTR CDBUFtrClsDlg::SelAllBtnClick(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    LVITEM lvi = {LVIF_PARAM | LVIF_STATE, 0, 0, 0, LVIS_STATEIMAGEMASK, NULL,
        0, 0, 0, 0};
    IDbuTable *pTbl;
    int iCnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
    for(int i = 0; i < iCnt; i++)
    {
        lvi.iItem = i;
        SendMessage(lv, LVM_GETITEM, 0, (LPARAM)&lvi);
        if((lvi.state >> 12) < 2)
        {
            pTbl = (IDbuTable*)lvi.lParam;
            pTbl->SetVisible(true);
            lvi.state = INDEXTOSTATEIMAGEMASK(2);
            SendMessage(lv, LVM_SETITEM, 0, (LPARAM)&lvi);
        }
    }
    if(SendMessage(lv, LVM_GETSELECTEDCOUNT, 0, 0) > 0)
    {
        EnableControls(hwndDlg, true);
    }
    return(0);
}

INT_PTR CDBUFtrClsDlg::SelNoneBtnClick(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    LVITEM lvi = {LVIF_PARAM | LVIF_STATE, 0, 0, 0, LVIS_STATEIMAGEMASK, NULL,
        0, 0, 0, 0};
    IDbuTable *pTbl;
    int iCnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
    for(int i = 0; i < iCnt; i++)
    {
        lvi.iItem = i;
        SendMessage(lv, LVM_GETITEM, 0, (LPARAM)&lvi);
        if((lvi.state >> 12) == 2)
        {
            pTbl = (IDbuTable*)lvi.lParam;
            pTbl->SetVisible(false);
            lvi.state = INDEXTOSTATEIMAGEMASK(1);
            SendMessage(lv, LVM_SETITEM, 0, (LPARAM)&lvi);
        }
    }
    EnableControls(hwndDlg, false);
    return(0);
}

bool CDBUFtrClsDlg::CanClose(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    LVITEM lvi = {LVIF_PARAM, 0, 0, 0, 0, NULL, 0, 0, 0, 0};
    IDbuTable *pTbl;
    int iCnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
    for(int i = 0; i < iCnt; i++)
    {
        lvi.iItem = i;
        SendMessage(lv, LVM_GETITEM, 0, (LPARAM)&lvi);
        pTbl = (IDbuTable*)lvi.lParam;
        pTbl->SaveTableMetadata();
    }
    return(true);
}

INT_PTR CDBUFtrClsDlg::PrimGeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(wNotifyCode == CBN_SELCHANGE)
    {
        int idx = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
        int ilen = SendMessage(hwndCtl, CB_GETLBTEXTLEN, idx, 0);
        LPTSTR sName = (LPTSTR)malloc((ilen + 1)*sizeof(TCHAR));
        SendMessage(hwndCtl, CB_GETLBTEXT, idx, (LPARAM)sName);
        m_pCurTbl->SetPrimaryGeometry(sName);
        free(sName);
        EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
    }
    return(0);
}

INT_PTR CDBUFtrClsDlg::DescEditCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(m_bSetup) return(0);

    if(wNotifyCode == EN_CHANGE)
    {
        TCHAR sDesc[256];
        SendMessage(hwndCtl, WM_GETTEXT, 256, (LPARAM)sDesc);
        m_pCurTbl->SetDescription(sDesc);
        EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
    }
    return(0);
}

INT_PTR CDBUFtrClsDlg::WMNotify(HWND hwndDlg, int idCtrl, LPNMHDR pnmh)
{
    switch(pnmh->idFrom)
    {
    case CTR_FCD_FTRLIST:
        return(FtrListNotify(hwndDlg, pnmh));
    }
    return(0);
}

INT_PTR CDBUFtrClsDlg::FtrListNotify(HWND hwndDlg, LPNMHDR pnmh)
{
    switch(pnmh->code)
    {
    case LVN_ITEMACTIVATE:
        return(FtrListItemActivate(hwndDlg, (LPNMITEMACTIVATE)pnmh));
    case LVN_ITEMCHANGED:
        return(FtrListItemChanged(hwndDlg, (LPNMLISTVIEW)pnmh));
    }
    return(0);
}

INT_PTR CDBUFtrClsDlg::FtrListItemActivate(HWND hwndDlg, LPNMITEMACTIVATE lpnmia)
{
    // show edit fields dialog
    if(!m_pFldEdtDlg) m_pFldEdtDlg = new CDBUFldEdtDlg(m_hInstance);
    m_pFldEdtDlg->ShowModal(hwndDlg, m_pCurTbl, m_lConnHandle);
    UpdateGeomFields(hwndDlg);
    return(0);
}

INT_PTR CDBUFtrClsDlg::FtrListItemChanged(HWND hwndDlg, LPNMLISTVIEW pnmv)
{
    if(m_bSetup) return(0);

    IDbuTable *pTbl;

    if((pnmv->uNewState & LVIS_STATEIMAGEMASK)^(pnmv->uOldState & LVIS_STATEIMAGEMASK))
    {
        pTbl = (IDbuTable*)pnmv->lParam;
        if(pnmv->uNewState & INDEXTOSTATEIMAGEMASK(2)) // checked
        {
            pTbl->SetVisible(true);
        }
        else
        {
            pTbl->SetVisible(false);
        }
        EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);

        if(SendMessage(pnmv->hdr.hwndFrom, LVM_GETITEMSTATE, pnmv->iItem,
            LVIS_SELECTED) & LVIS_SELECTED)
                EnableControls(hwndDlg, pTbl->GetVisible());
    }

    if((pnmv->uNewState & LVIS_SELECTED)^(pnmv->uOldState & LVIS_SELECTED))
    {
        if(pnmv->uNewState & LVIS_SELECTED)
        {
            m_pCurTbl = (IDbuTable*)pnmv->lParam;
            EnableControls(hwndDlg, m_pCurTbl->GetVisible());
            SetupControls(hwndDlg, m_pCurTbl);
        }
        else
        {
            m_pCurTbl = NULL;
            SetupControls(hwndDlg, NULL);
            EnableControls(hwndDlg, false);
        }
    }

    return(0);
}

void CDBUFtrClsDlg::EnableControls(HWND hwndDlg, bool bEnable)
{
    EnableWindow(GetDlgItem(hwndDlg, CTR_FCD_PRIMGEOMCB), bEnable);
    EnableWindow(GetDlgItem(hwndDlg, CTR_FCD_FTRDESCEDT), bEnable);
    EnableWindow(GetDlgItem(hwndDlg, CTR_FCD_EDTFLDSBTN), bEnable);
    return;
}

void CDBUFtrClsDlg::SetupControls(HWND hwndDlg, IDbuTable *pTbl)
{
    if(pTbl)
    {
        IDbuFields *pFlds = pTbl->GetFields();
        HWND cb = GetDlgItem(hwndDlg, CTR_FCD_PRIMGEOMCB);
        bool bHasStrings = false;
        int i = 0;
        LPTSTR sGeomName = pFlds->GetNextGeomName(&i);
        if(sGeomName) bHasStrings = true;
        while(sGeomName)
        {
            SendMessage(cb, CB_ADDSTRING, 0, (LPARAM)sGeomName);
            sGeomName = pFlds->GetNextGeomName(&i);
        }
        if(bHasStrings)
        {
            i = SendMessage(cb, CB_FINDSTRINGEXACT, 0,
                (LPARAM)m_pCurTbl->GetPrimaryGeometry());
            SendMessage(cb, CB_SETCURSEL, i, 0);
        }
        SendDlgItemMessage(hwndDlg, CTR_FCD_FTRDESCEDT, WM_SETTEXT,
            0, (LPARAM)pTbl->GetDescription());
    }
    else
    {
        SendDlgItemMessage(hwndDlg, CTR_FCD_PRIMGEOMCB, CB_RESETCONTENT, 0, 0);
        SendDlgItemMessage(hwndDlg, CTR_FCD_PRIMGEOMCB, CB_SETCURSEL,
            (WPARAM)-1, 0);
        SendDlgItemMessage(hwndDlg, CTR_FCD_FTRDESCEDT, WM_SETTEXT,
            0, (LPARAM)_T(""));
    }
    return;
}

void CDBUFtrClsDlg::UpdateGeomFields(HWND hwndDlg)
{
    HWND cb = GetDlgItem(hwndDlg, CTR_FCD_PRIMGEOMCB);
    SendMessage(cb, CB_RESETCONTENT, 0, 0);

    bool bHasStrings = false;
    int i = 0;
    IDbuFields *pFlds = m_pCurTbl->GetFields();
    LPTSTR sGeomName = pFlds->GetNextGeomName(&i);
    if(sGeomName) bHasStrings = true;
    while(sGeomName)
    {
        SendMessage(cb, CB_ADDSTRING, 0, (LPARAM)sGeomName);
        sGeomName = pFlds->GetNextGeomName(&i);
    }
    if(bHasStrings)
    {
        i = SendMessage(cb, CB_FINDSTRINGEXACT, 0,
            (LPARAM)m_pCurTbl->GetPrimaryGeometry());
        SendMessage(cb, CB_SETCURSEL, i, 0);
        if(i < 0) m_pCurTbl->SetPrimaryGeometry(NULL);
    }
    else m_pCurTbl->SetPrimaryGeometry(NULL);
    m_pCurTbl->SetChanged(true);
    EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);
    return;
}
