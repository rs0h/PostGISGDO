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

#include "PsgDBUFldEdtDlg.hpp"
#include <tchar.h>
#include "PsgDBUtils.rh"
#include "../Common/PGtoGDOmaps.hpp"


INT_PTR CALLBACK DBUFldEdtDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,
    LPARAM lParam)
{
    CDBUFldEdtDlg *cw = NULL;
    if(uMsg == WM_INITDIALOG) cw = (CDBUFldEdtDlg*)lParam;
    else cw = (CDBUFldEdtDlg*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

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

CDBUFldEdtDlg::CDBUFldEdtDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_lConnHandle = 0;
    m_hwndMain = NULL;
    m_hwndDlg = NULL;
    m_pTbl = NULL;
    m_pCurFld = NULL;
}

CDBUFldEdtDlg::~CDBUFldEdtDlg()
{
}

bool CDBUFldEdtDlg::ShowModal(HWND hWnd, IDbuTable *pTbl, long lConnHandle)
{
    m_hwndMain = hWnd;
    m_pTbl = pTbl;
    m_lConnHandle = lConnHandle;
    return(DialogBoxParam(m_hInstance, _T("FIELDSDLG"), hWnd,
        DBUFldEdtDlgProc, (LPARAM)this));
}

void CDBUFldEdtDlg::SetupListView(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FLD_FLDLIST);
    SendMessage(lv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_CHECKBOXES,
        LVS_EX_CHECKBOXES);
    LVCOLUMN lvc = {LVCF_TEXT, LVCFMT_LEFT, 0, (LPTSTR)_T("Tables"),
        0, 0, 0, 0};
    SendMessage(lv, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

    IDbuFields *pFlds = m_pTbl->GetFields();
    int n = pFlds->GetCount();
    int idx;
    IDbuField *pfld;
    LVITEM lvi = {LVIF_TEXT | LVIF_PARAM | LVIF_STATE, 0, 0, 0, 0, NULL, 0, 0,
        0, 0};
    for(int i = 0; i < n; i++)
    {
        pfld = pFlds->GetItem(i);
        lvi.iItem = i;
        lvi.pszText = pfld->GetNamePtr();
        lvi.lParam = (LPARAM)pfld;
        lvi.stateMask = LVIS_STATEIMAGEMASK;
        lvi.state = INDEXTOSTATEIMAGEMASK((pfld->GetVisible()) + 1);
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

INT_PTR CDBUFldEdtDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus,
    LPARAM lInitParam)
{
    m_hwndDlg = hwndDlg;

    SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lInitParam);

    SetupListView(hwndDlg);

    SendDlgItemMessage(hwndDlg, CTR_FLD_FLDDESCEDT, EM_LIMITTEXT, 256, 0);

    HWND cb = GetDlgItem(hwndDlg, CTR_FLD_SRIDCB);
    SendMessage(cb, CB_LIMITTEXT, 10, 0);
    DbuFillCtrlWithSrids(m_lConnHandle, cb, CB_ADDSTRING);

    ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_HYPERCHB), FALSE);
    ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), FALSE);
    ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), FALSE);
    ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), FALSE);
    ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), FALSE);

    return(1);
}

INT_PTR CDBUFldEdtDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID,
    HWND hwndCtl)
{
    switch(wID)
    {
    case IDOK:
        return(OKBtnClick(hwndDlg));
    case CTR_FLD_FLDTYPECB:
        return(FldTypeCBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FLD_FLDDESCEDT:
        return(FldDescEditCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FLD_GEOMTYPECB:
        return(FldGeomCBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FLD_SRIDCB:
        return(FldSridCBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTR_FLD_SELALLBTN:
        return(SelAllBtnClick(hwndDlg));
    case CTR_FLD_SELNONEBTN:
        return(SelNoneBtnClick(hwndDlg));
    case CTR_FLD_KEYCHB:
        return(KeyChBClick(hwndDlg, hwndCtl));
    case CTR_FLD_HYPERCHB:
        return(HyperChBClick(hwndDlg, hwndCtl));
    default:
        return(0);
    }
}

INT_PTR CDBUFldEdtDlg::OKBtnClick(HWND hwndDlg)
{
    if(CanClose(hwndDlg))
    {
        EndDialog(hwndDlg, 1);
    }
    return(1);
}

INT_PTR CDBUFldEdtDlg::SelAllBtnClick(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    LVITEM lvi = {LVIF_PARAM | LVIF_STATE, 0, 0, 0, LVIS_STATEIMAGEMASK, NULL,
        0, 0, 0, 0};
    IDbuField *pFld;
    int iCnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
    for(int i = 0; i < iCnt; i++)
    {
        lvi.iItem = i;
        SendMessage(lv, LVM_GETITEM, 0, (LPARAM)&lvi);
        if((lvi.state >> 12) < 2)
        {
            pFld = (IDbuField*)lvi.lParam;
            pFld->SetVisible(true);
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

INT_PTR CDBUFldEdtDlg::SelNoneBtnClick(HWND hwndDlg)
{
    HWND lv = GetDlgItem(hwndDlg, CTR_FCD_FTRLIST);
    LVITEM lvi = {LVIF_PARAM | LVIF_STATE, 0, 0, 0, LVIS_STATEIMAGEMASK, NULL,
        0, 0, 0, 0};
    IDbuField *pFld;
    int iCnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
    for(int i = 0; i < iCnt; i++)
    {
        lvi.iItem = i;
        SendMessage(lv, LVM_GETITEM, 0, (LPARAM)&lvi);
        if((lvi.state >> 12) == 2)
        {
            pFld = (IDbuField*)lvi.lParam;
            pFld->SetVisible(false);
            lvi.state = INDEXTOSTATEIMAGEMASK(1);
            SendMessage(lv, LVM_SETITEM, 0, (LPARAM)&lvi);
        }
    }
    EnableControls(hwndDlg, false);
    return(0);
}

bool CDBUFldEdtDlg::CanClose(HWND hwndDlg)
{
    return(true);
}

INT_PTR CDBUFldEdtDlg::FldTypeCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(wNotifyCode == CBN_SELCHANGE)
    {
        //int idx = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
        HWND wnd;
        TCHAR sBuf[64];
        int iSrid = 0;
        m_pCurFld->SetIsKey(false);
        wnd = GetDlgItem(hwndDlg, CTR_FLD_KEYCHB);
        SendMessage(wnd, BM_SETCHECK, (WPARAM)FALSE, 0);
        if(m_pCurFld->GetType() < 32)
        {
            m_pCurFld->SetType(33);
            m_pCurFld->SetSubType(gdbGraphicsText);
            wnd = GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB);
            EnableWindow(wnd, true);
            SendMessage(wnd, CB_RESETCONTENT, 0, 0);
            LoadString(m_hInstance, IDS_GRAPHTEXT, sBuf, 64);
            SendMessage(wnd, CB_ADDSTRING, 0, (LPARAM)sBuf);
            LoadString(m_hInstance, IDS_RASTER, sBuf, 64);
            SendMessage(wnd, CB_ADDSTRING, 0, (LPARAM)sBuf);
            SendMessage(wnd, CB_SETCURSEL, (WPARAM)0, 0);

            wnd = GetDlgItem(hwndDlg, CTR_FLD_SRIDCB);
            EnableWindow(wnd, true);
            SendMessage(wnd, CB_SETCURSEL, 0, 0);
            SendMessage(wnd, CB_GETLBTEXT, 0, (LPARAM)sBuf);
            _stscanf(sBuf, _T("%d"), &iSrid);
            m_pCurFld->SetSrid(iSrid);

            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), TRUE);
        }
        else
        {
            m_pCurFld->SetType(gdbLongBinary);
            m_pCurFld->SetSubType(0);
            m_pCurFld->SetSrid(0);
            wnd = GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB);
            EnableWindow(wnd, false);
            SendMessage(wnd, CB_RESETCONTENT, 0, 0);
            SendMessage(wnd, CB_SETCURSEL, (WPARAM)-1, 0);
            EnableWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), false);

            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), FALSE);
        }
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::FldDescEditCmd(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    if(wNotifyCode == EN_CHANGE)
    {
        TCHAR sBuf[256];
        SendMessage(hwndCtl, WM_GETTEXT, 256, (LPARAM)sBuf);
        m_pCurFld->SetDescription(sBuf);
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::FldGeomCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(wNotifyCode == CBN_SELCHANGE)
    {
        if(m_pCurFld->GetSubType() == gdbCoverage)
            m_pCurFld->SetSubType(gdbGraphicsText);
        else m_pCurFld->SetSubType(gdbCoverage);
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::FldSridCBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    TCHAR sBuf[32];
    int iSrid = 0;
    if(wNotifyCode == CBN_SELCHANGE)
    {
        int idx = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
        SendMessage(hwndCtl, CB_GETLBTEXT, idx, (LPARAM)sBuf);
        _stscanf(sBuf, _T("%d"), &iSrid);
        m_pCurFld->SetSrid(iSrid);
    }
    else if(wNotifyCode == CBN_EDITCHANGE)
    {
        SendMessage(hwndCtl, WM_GETTEXT, 32, (LPARAM)sBuf);
        _stscanf(sBuf, _T("%d"), &iSrid);
        m_pCurFld->SetSrid(iSrid);
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::WMNotify(HWND hwndDlg, int idCtrl, LPNMHDR pnmh)
{
    switch(pnmh->idFrom)
    {
    case CTR_FLD_FLDLIST:
        return(FldListNotify(hwndDlg, pnmh));
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::FldListNotify(HWND hwndDlg, LPNMHDR pnmh)
{
    switch(pnmh->code)
    {
    case LVN_ITEMCHANGED:
        return(FldListItemChanged(hwndDlg, (LPNMLISTVIEW)pnmh));
    }
    return(0);
}

INT_PTR CDBUFldEdtDlg::FldListItemChanged(HWND hwndDlg, LPNMLISTVIEW pnmv)
{
    IDbuField *pFld;

    if((pnmv->uNewState & LVIS_STATEIMAGEMASK)^(pnmv->uOldState & LVIS_STATEIMAGEMASK))
    {
        pFld = (IDbuField*)pnmv->lParam;
        if(pnmv->uNewState & INDEXTOSTATEIMAGEMASK(2)) // checked
        {
            pFld->SetVisible(true);
        }
        else
        {
            pFld->SetVisible(false);
        }
        EnableWindow(GetDlgItem(hwndDlg, IDOK), TRUE);

        if(SendMessage(pnmv->hdr.hwndFrom, LVM_GETITEMSTATE, pnmv->iItem,
            LVIS_SELECTED) & LVIS_SELECTED)
                EnableControls(hwndDlg, pFld->GetVisible());
    }

    if((pnmv->uNewState & LVIS_SELECTED)^(pnmv->uOldState & LVIS_SELECTED))
    {
        if(pnmv->uNewState & LVIS_SELECTED)
        {
            m_pCurFld = (IDbuField*)pnmv->lParam;
            EnableControls(hwndDlg, m_pCurFld->GetVisible());
            SetupControls(hwndDlg, m_pCurFld);
        }
        else
        {
            m_pCurFld = NULL;
            SetupControls(hwndDlg, NULL);
            EnableControls(hwndDlg, false);
        }
    }

    return(0);
}

void CDBUFldEdtDlg::EnableControls(HWND hwndDlg, bool bEnable)
{
    EnableWindow(GetDlgItem(hwndDlg, CTR_FLD_FLDDESCEDT), bEnable);
    HWND wnd = GetDlgItem(hwndDlg, CTR_FLD_KEYCHB);
    EnableWindow(wnd, bEnable && m_pTbl->GetIsView());
    return;
}

void CDBUFldEdtDlg::SetupControls(HWND hwndDlg, IDbuField *pFld)
{
    if(pFld)
    {
        HWND cb1 = GetDlgItem(hwndDlg, CTR_FLD_FLDTYPECB);
        HWND cb2 = GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB);
        HWND cb3 = GetDlgItem(hwndDlg, CTR_FLD_SRIDCB);
        switch(pFld->GetType())
        {
        case gdbLongBinary:
        case gdbGraphic:
            EnableWindow(cb1, true);
            EnableWindow(cb2, pFld->GetType() == gdbGraphic);
            EnableWindow(cb3, pFld->GetType() == gdbGraphic);
            break;
        default:
            EnableWindow(cb1, false);
            EnableWindow(cb2, false);
            EnableWindow(cb3, false);
        }

        SendMessage(cb1, CB_RESETCONTENT, 0, 0);
        SendMessage(cb2, CB_RESETCONTENT, 0, 0);

        TCHAR sBuf[64];
        int isIdx = 0;
        switch(pFld->GetType())
        {
        case gdbByte:
            isIdx = IDS_BYTE;
            break;
        case gdbInteger:
            isIdx = IDS_INTEGER;
            break;
        case gdbLong:
            isIdx = IDS_LONG;
            break;
        case gdbSingle:
            isIdx = IDS_SINGLE;
            break;
        case gdbCurrency:
            isIdx = IDS_CURRENCY;
            break;
        case gdbDouble:
            isIdx = IDS_DOUBLE;
            break;
        case gdbBoolean:
            isIdx = IDS_BOOLEAN;
            break;
        case gdbGuid:
            isIdx = IDS_GUID;
            break;
        case gdbDate:
            isIdx = IDS_DATE;
            break;
        case gdbText:
            isIdx = IDS_TEXT;
            break;
        case gdbMemo:
            isIdx = IDS_MEMO;
            break;
        case gdbLongBinary:
            LoadString(m_hInstance, IDS_GRAPHICS, sBuf, 64);
            SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)sBuf);
            isIdx = IDS_BINARY;
            break;
        case gdbSpatial:
            isIdx = IDS_SPATIAL;
            break;
        case gdbGraphic:
            LoadString(m_hInstance, IDS_BINARY, sBuf, 64);
            SendMessage(cb1, CB_ADDSTRING, 0, (LPARAM)sBuf);
            isIdx = IDS_GRAPHICS;
            break;
        }

        if(isIdx > 0)
        {
            LoadString(m_hInstance, isIdx, sBuf, 64);
            SendMessage(cb1, CB_INSERTSTRING, 0, (LPARAM)sBuf);
            SendMessage(cb1, CB_SETCURSEL, (WPARAM)0, 0);
        }
        else SendMessage(cb1, CB_SETCURSEL, (WPARAM)-1, 0);

        SendDlgItemMessage(hwndDlg, CTR_FLD_FLDDESCEDT, WM_SETTEXT,
            0, (LPARAM)pFld->GetDescription());

        switch(pFld->GetType())
        {
        case gdbSpatial:
        case gdbGraphic:
            LoadString(m_hInstance, ISD_ISPRIMARY, sBuf, 64);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_HYPERCHB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), TRUE);
            break;
        case gdbLongBinary:
            sBuf[0] = 0;
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_HYPERCHB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), TRUE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), TRUE);
            break;
        default:
            LoadString(m_hInstance, ISD_ISKEY, sBuf, 64);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_HYPERCHB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPELAB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDLAB), FALSE);
            ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), FALSE);
        }
        cb1 = GetDlgItem(hwndDlg, CTR_FLD_KEYCHB);
        SendMessage(cb1, WM_SETTEXT, 0, (LPARAM)sBuf);
        SendMessage(cb1, BM_SETCHECK, (WPARAM)pFld->GetIsKey(), 0);
        ShowWindow(cb1, pFld->GetType() != gdbLongBinary);

        cb1 = GetDlgItem(hwndDlg, CTR_FLD_HYPERCHB);
        if((pFld->GetType() == gdbText) ||
           (pFld->GetType() == gdbMemo))
        {
            ShowWindow(cb1, TRUE);
            WPARAM fCheck = BST_UNCHECKED;
            if(pFld->GetIsHypertext()) fCheck = BST_CHECKED;
            SendMessage(cb1, BM_SETCHECK, fCheck, 0);
        }
        else ShowWindow(cb1, FALSE);

        isIdx = 0;
        switch(pFld->GetSubType())
        {
        case gdbLinear:
            isIdx = IDS_LINE;
            break;
        case gdbAreal:
            isIdx = IDS_AREA;
            break;
        case gdbAnySpatial:
            isIdx = IDS_ANYSPATIAL;
            break;
        case gdbCoverage:
            LoadString(m_hInstance, IDS_GRAPHTEXT, sBuf, 64);
            SendMessage(cb2, CB_ADDSTRING, 0, (LPARAM)sBuf);
            isIdx = IDS_RASTER;
            break;
        case gdbGraphicsText:
            LoadString(m_hInstance, IDS_RASTER, sBuf, 64);
            SendMessage(cb2, CB_ADDSTRING, 0, (LPARAM)sBuf);
            isIdx = IDS_GRAPHTEXT;
            break;
        case gdbPoint:
            isIdx = IDS_POINT;
            break;
        }

        if(isIdx > 0)
        {
            LoadString(m_hInstance, isIdx, sBuf, 64);
            SendMessage(cb2, CB_INSERTSTRING, 0, (LPARAM)sBuf);
            SendMessage(cb2, CB_SETCURSEL, (WPARAM)0, 0);
        }
        else SendMessage(cb2, CB_SETCURSEL, (WPARAM)-1, 0);

        if(isIdx > 0)
        {
            _stprintf(sBuf, _T("%d"), m_pCurFld->GetSrid());
            SendMessage(cb3, WM_SETTEXT, 0, (LPARAM)sBuf);
        }
        else SendMessage(cb3, WM_SETTEXT, 0, (LPARAM)_T(""));
    }
    else
    {
        SendDlgItemMessage(hwndDlg, CTR_FLD_FLDTYPECB, CB_SETCURSEL,
            (WPARAM)-1, 0);
        SendDlgItemMessage(hwndDlg, CTR_FLD_FLDDESCEDT, WM_SETTEXT,
            0, (LPARAM)_T(""));
        //SendDlgItemMessage(hwndDlg, CTR_FLD_GEOMTYPECB, CB_SETCURSEL,
        //    (WPARAM)-1, 0);
        //SendDlgItemMessage(hwndDlg, CTR_FLD_SRIDCB, WM_SETTEXT, 0, (LPARAM)_T(""));
        ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_GEOMTYPECB), FALSE);
        ShowWindow(GetDlgItem(hwndDlg, CTR_FLD_SRIDCB), FALSE);
    }
    return;
}

INT_PTR CDBUFldEdtDlg::KeyChBClick(HWND hwndDlg, HWND hwndCtl)
{
    m_pCurFld->SetIsKey(SendMessage(hwndCtl, BM_GETCHECK, 0, 0) == BST_CHECKED);
    return(0);
}

INT_PTR CDBUFldEdtDlg::HyperChBClick(HWND hwndDlg, HWND hwndCtl)
{
    m_pCurFld->SetIsHypertext(SendMessage(hwndCtl, BM_GETCHECK, 0, 0) == BST_CHECKED);
    return(0);
}
