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

#include "PsgDBUMainDlg.hpp"

#include <tchar.h>
#include "PsgDButils.rh"
#include "../Common/VarUtils.hpp"
#include "../Common/pg_const.h"

wchar_t g_ModuleName[MAX_PATH];

/*void HandlePQError(HINSTANCE hInstance, HWND hwndParent, PGconn *pConn)
{
    LPSTR sErr = PQerrorMessage(pConn);
    TCHAR sBuf[64];
    LoadString(hInstance, IDS_ERRORBASE, sBuf, 64);
    LPTSTR sMsg = CharToTChar(sErr);
    MessageBox(hwndParent, sMsg, sBuf, MB_ICONERROR | MB_OK);
    free(sMsg);
    return;
}*/

BOOL CALLBACK DBMainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CDBMainDlg *mw = NULL;
    if(uMsg == WM_INITDIALOG) mw = (CDBMainDlg*)lParam;
    else  mw = (CDBMainDlg*)GetWindowLongPtr(hwndDlg, GWL_USERDATA);

    switch(uMsg)
    {
    case WM_INITDIALOG:
        return(mw->WMInitDialog(hwndDlg, (HWND)wParam, lParam));
    case WM_COMMAND:
        return(mw->WMCommand(hwndDlg, HIWORD(wParam), LOWORD(wParam),
            (HWND)lParam));
    case WM_CLOSE:
        return(mw->WMClose(hwndDlg));
    case WM_DESTROY:
        PostQuitMessage(0);
        return(0);
    default:
        return(0);
    }
}


CDBMainDlg::CDBMainDlg(HINSTANCE hInstance)
{
    m_hInstance = hInstance;

    m_ConnProps.sServer[0] = 0;
    m_ConnProps.sPort[0] = 0;
    m_ConnProps.sDBName[0] = 0;
    m_ConnProps.sUsrName[0] = 0;
    m_ConnProps.sPasswd[0] = 0;
    m_ConnProps.bSave = false;

    m_lConnHandle = 0;

    m_pConnDlg = NULL;
    m_pFtrClsDlg = NULL;
    m_pUsrDlg = NULL;
    m_pSpecFldEdt = NULL;

    GetIniFileName();
    GetPrivateProfileString(_T("Connection"), _T("Server"), _T(""),
        m_ConnProps.sServer, 64, m_tsIniFileName);
    GetPrivateProfileString(_T("Connection"), _T("Port"), _T(""),
        m_ConnProps.sPort, 16, m_tsIniFileName);
    GetPrivateProfileString(_T("Connection"), _T("DBName"), _T(""),
        m_ConnProps.sDBName, 64, m_tsIniFileName);
    GetPrivateProfileString(_T("Connection"), _T("UsrName"), _T(""),
        m_ConnProps.sUsrName, 64, m_tsIniFileName);
    GetPrivateProfileString(_T("Connection"), _T("Passwd"), _T(""),
        m_ConnProps.sPasswd, 64, m_tsIniFileName);
    if(m_ConnProps.sServer[0]) m_ConnProps.bSave = true;
}

CDBMainDlg::~CDBMainDlg()
{
    if(m_lConnHandle > 0) DbuCloseConnection(m_lConnHandle);

    if(m_ConnProps.bSave)
    {
        WritePrivateProfileString(_T("Connection"), _T("Server"),
            m_ConnProps.sServer, m_tsIniFileName);
        WritePrivateProfileString(_T("Connection"), _T("Port"),
            m_ConnProps.sPort, m_tsIniFileName);
        WritePrivateProfileString(_T("Connection"), _T("DBName"),
            m_ConnProps.sDBName, m_tsIniFileName);
        WritePrivateProfileString(_T("Connection"), _T("UsrName"),
            m_ConnProps.sUsrName, m_tsIniFileName);
        WritePrivateProfileString(_T("Connection"), _T("Passwd"),
            m_ConnProps.sPasswd, m_tsIniFileName);
    }
    else
    {
        WritePrivateProfileString(_T("Connection"), NULL, NULL, m_tsIniFileName);
    }
    free(m_tsIniFileName);

    if(m_pSpecFldEdt) delete m_pSpecFldEdt;
    if(m_pUsrDlg) delete m_pUsrDlg;
    if(m_pFtrClsDlg) delete m_pFtrClsDlg;
    if(m_pConnDlg) delete m_pConnDlg;
}

void CDBMainDlg::GetIniFileName()
{
    int slen = 0;
    LPWSTR dotptr = wcsrchr(g_ModuleName, '.');
    if(dotptr) slen = dotptr - g_ModuleName + 4;
    else slen = wcslen(g_ModuleName) + 4;
    m_tsIniFileName = (LPTSTR)malloc((slen + 1)*sizeof(TCHAR));
#ifdef UNICODE
    wcsncpy(m_tsIniFileName, g_ModuleName, slen - 4);
    m_tsIniFileName[slen - 4] = 0;
    wcscat(m_tsIniFileName, L".ini");
#else
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, g_ModuleName, slen - 4,
        m_tsIniFileName, slen + 1, NULL, NULL);
    m_tsIniFileName[slen - 4] = 0;
    strcat(m_tsIniFileName, ".ini");
#endif
    return;
}

HWND CDBMainDlg::DisplayWindow()
{
    return(CreateDialogParam(m_hInstance, _T("MAINFORM"), 0, DBMainDlgProc,
        (LPARAM)this));
}

BOOL CDBMainDlg::WMInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lInitParam)
{
    SetWindowLongPtr(hwndDlg, GWL_USERDATA, (LONG_PTR)lInitParam);

    HICON hico = LoadIcon(m_hInstance, _T("MAINICON"));
    SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hico);
    SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hico);

    return(0);
}

BOOL CDBMainDlg::WMCommand(HWND hwndDlg, WORD wNotifyCode, WORD wID, HWND hwndCtl)
{
    switch(wID)
    {
    case IDM_DBOPEN:
        return(OpenDBCmd(hwndDlg, wNotifyCode, hwndCtl));
    case IDM_DBCLOSE:
        return(CloseDatabase(hwndDlg));
    case IDM_FILEEXIT:
        return(ExitCmd(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_GMDP:
        return(GrantMetadataPrivileges(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_EFCM:
        return(EditFeatureClassMetadata(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_DFCM:
        return(DeleteFeatureClassMetadata(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_ACS:
        return(DropMetadataTables(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_CSMT:
        return(CreateSimpleMetadataTables(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_CIMT:
        return(CreateIngrMetadataTables(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_CML:
        return(ClearModificationLog(hwndDlg, wNotifyCode, hwndCtl));
    case CTL_MND_CMLT:
        return(CreateModificationLogTriggers(hwndDlg, wNotifyCode, hwndCtl));
    case IDM_HELPCONT:
        return(HelpContent(hwndDlg, wNotifyCode, hwndCtl));
    case IDM_HELPABOUT:
        return(HelpAbout(hwndDlg, wNotifyCode, hwndCtl));
    default:
        return(0);
    }
}

BOOL CDBMainDlg::WMClose(HWND hwndDlg)
{
    CloseDatabase(hwndDlg);
    DestroyWindow(hwndDlg);
    return(0);
}

BOOL CDBMainDlg::ExitCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    SendMessage(hwndDlg, WM_CLOSE, 0, 0);
    return(0);
}

void CDBMainDlg::UpdateButtons(HWND hwndDlg)
{
    CConnStatus stat = DbuGetConnectionStatus(m_lConnHandle);

    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_GMDP), stat > icsNoMetadata);
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_EFCM), stat > icsINGRMetaIncomplete);
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_DFCM), stat > icsNotEnoughPrivileges);
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_ACS),
        stat > icsSimpleMetaIncomplete); //icsINGRMetaIncomplete);
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CSMT),
        (stat > icsNoPostGis) && (stat < icsSimpleMetadata));
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CIMT),
        (stat > icsNoPostGis) && (stat < icsINGRMetadata));
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CML), stat > icsSimpleMetaIncomplete);
    EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CMLT), stat > icsNoPostGis);

    TCHAR sBuf[128];
    LoadString(m_hInstance, IDS_DBCLOSED + stat, sBuf, 128);
    SendDlgItemMessage(hwndDlg, CTL_MND_CONNSTATLAB, WM_SETTEXT, 0, (LPARAM)sBuf);
    return;
}

void CDBMainDlg::ShowConnDetails(HWND hwndDlg, bool bShow)
{
    if(bShow)
    {
        SendDlgItemMessage(hwndDlg, CTL_MND_SRVLAB, WM_SETTEXT, 0,
            (LPARAM)m_ConnProps.sServer);
        SendDlgItemMessage(hwndDlg, CTL_MND_DBLAB, WM_SETTEXT, 0,
            (LPARAM)m_ConnProps.sDBName);
        SendDlgItemMessage(hwndDlg, CTL_MND_USRLAB, WM_SETTEXT, 0,
            (LPARAM)m_ConnProps.sUsrName);
    }
    else
    {
        SendDlgItemMessage(hwndDlg, CTL_MND_SRVLAB, WM_SETTEXT, 0,
            (LPARAM)_T(""));
        SendDlgItemMessage(hwndDlg, CTL_MND_DBLAB, WM_SETTEXT, 0,
            (LPARAM)_T(""));
        SendDlgItemMessage(hwndDlg, CTL_MND_USRLAB, WM_SETTEXT, 0,
            (LPARAM)_T(""));
    }
    return;
}

bool CDBMainDlg::CloseDatabase(HWND hwndDlg)
{
    if(m_lConnHandle < 1) return(true);
    if(DbuCloseConnection(m_lConnHandle))
    {
        m_lConnHandle = 0;
        return(true);
    }
    return(false);
}

BOOL CDBMainDlg::OpenDBCmd(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    if(!m_pConnDlg) m_pConnDlg = new CDBUConnDlg(m_hInstance);
    if(m_pConnDlg->ShowModal(hwndDlg, &m_ConnProps))
    {
        if(CloseDatabase(hwndDlg)) DoOpenDatabase(hwndDlg);
    }
    return(0);
}

void CDBMainDlg::DoOpenDatabase(HWND hwndDlg)
{
    wchar_t wsHost[64];
    wchar_t wsPort[16];
    wchar_t wsDBName[64];
    wchar_t wsUserName[64];
    wchar_t wsPasswd[64];
    TCharToWCharBuf(m_ConnProps.sServer, wsHost, 64);
    TCharToWCharBuf(m_ConnProps.sPort, wsPort, 16);
    TCharToWCharBuf(m_ConnProps.sDBName, wsDBName, 64);
    TCharToWCharBuf(m_ConnProps.sUsrName, wsUserName, 64);
    TCharToWCharBuf(m_ConnProps.sPasswd, wsPasswd, 64);

    int iPort = 0;
    LPWSTR psPort = NULL;
    if(wsPort[0])
    {
        int ilen = wcslen(wsPort) - 1;
        int i = ilen - 1;
        while((i >= 0) && (wsPort[i] == ' '))
        {
            wsPort[i--] = 0;
            ilen--;
        }
        i = 0;
        while((i < ilen) && (wsPort[i] == ' '))
        {
            i++;
        }
        ilen -= i;
        if(ilen > 0) psPort = &wsPort[i];
        if(swscanf(psPort, L"%d", &iPort) != 1) iPort = 0;
    }

    m_lConnHandle = DbuOpenConnection(wsHost, wsDBName, wsUserName, wsPasswd,
        iPort);

    if(m_lConnHandle > 0)
    {
        // enable buttons
        UpdateButtons(hwndDlg);
        ShowConnDetails(hwndDlg, true);
    }
    else
    {
        //HandlePQError(m_hInstance, hwndDlg, m_cConnStruct.pConn);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_GMDP), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_EFCM), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_DFCM), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_ACS), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CSMT), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CIMT), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CML), FALSE);
        EnableWindow(GetDlgItem(hwndDlg, CTL_MND_CMLT), FALSE);

        TCHAR sBuf[128];
        LoadString(m_hInstance, IDS_DBCLOSED, sBuf, 128);
        SendDlgItemMessage(hwndDlg, CTL_MND_CONNSTATLAB, WM_SETTEXT, 0, (LPARAM)sBuf);
    }
    return;
}

BOOL CDBMainDlg::GrantMetadataPrivileges(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    if(!m_pUsrDlg) m_pUsrDlg = new CDBUUsrDlg(m_hInstance);

    m_pUsrDlg->ShowModal(hwndDlg, m_lConnHandle);
    return(0);
}

BOOL CDBMainDlg::EditFeatureClassMetadata(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    if(!m_pFtrClsDlg) m_pFtrClsDlg = new CDBUFtrClsDlg(m_hInstance);
    m_pFtrClsDlg->ShowModal(hwndDlg, m_lConnHandle);
    return(0);
}

// this method is actually used to run an arbitrary script
// it is mainly intended for debugging purposes
BOOL CDBMainDlg::DeleteFeatureClassMetadata(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    //MessageBox(hwndDlg, L"This functionality is not implemented yet",
    //    L"We are sorry", MB_OK);
    TCHAR sFilt[128];
    LoadString(m_hInstance, IDS_SQLFILTER, sFilt, 128);
    for(int i = 0; i < 128; i++)
    {
        if(sFilt[i] == 1) sFilt[i] = 0;
    }
    TCHAR sFile[MAX_PATH] = _T("");

    OPENFILENAME ofn = {sizeof(OPENFILENAME), hwndDlg, m_hInstance, sFilt,
        NULL, 0, 0, sFile, MAX_PATH, NULL, 0, _T(""), NULL, OFN_ENABLESIZING |
        //OFN_EXPLORER, 0, 0, NULL,
        OFN_EXPLORER | OFN_FILEMUSTEXIST, 0, 0, NULL,
        0, NULL, NULL};

    if(GetOpenFileName(&ofn))
    {
        LPWSTR wsFile = TCharToWChar(sFile);

        TCHAR sHeader[64], sMsg[128];

        if(DbuRunScript(m_lConnHandle, wsFile))
        {
            LoadString(m_hInstance, IDS_INFOBASE, sHeader, 64);
            LoadString(m_hInstance, IDI_CUSTSCRIPTOK, sMsg, 128);
            MessageBox(hwndDlg, sMsg, sHeader, MB_OK | MB_ICONASTERISK);
        }
        else
        {
            LoadString(m_hInstance, IDS_ERRORBASE, sHeader, 64);
            LoadString(m_hInstance, IDE_CUSTSCRIPTFAIL, sMsg, 128);
            MessageBox(hwndDlg, sMsg, sHeader, MB_OK | MB_ICONWARNING);
        }

        free(wsFile);
    }

    return(0);
}

BOOL CDBMainDlg::DropMetadataTables(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    DbuDropMetadata(m_lConnHandle);
    UpdateButtons(hwndDlg);
    return(0);
}

BOOL CDBMainDlg::CreateSimpleMetadataTables(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    DbuCreateMetadata(m_lConnHandle, false);
    UpdateButtons(hwndDlg);
    return(0);
}

BOOL CDBMainDlg::CreateIngrMetadataTables(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    DbuCreateMetadata(m_lConnHandle, true);
    UpdateButtons(hwndDlg);
    return(0);
}

BOOL CDBMainDlg::ClearModificationLog(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    DbuClearModificationLog(m_lConnHandle);

    TCHAR sCap[64], sMsg[128];
    LoadString(m_hInstance, IDS_INFOBASE, sCap, 64);
    LoadString(m_hInstance, IDI_MODLOGDELETED, sMsg, 128);
    MessageBox(hwndDlg, sMsg, sCap, MB_OK | MB_ICONINFORMATION);
    return(0);
}

BOOL CDBMainDlg::CreateModificationLogTriggers(HWND hwndDlg, WORD wNotifyCode,
    HWND hwndCtl)
{
    if(!m_pSpecFldEdt) m_pSpecFldEdt = new CDBUSpecFldDlg(m_hInstance);
    m_pSpecFldEdt->ShowModal(hwndDlg, m_lConnHandle);

// Some testing code bellow
//    PGresult *res = PQexec(m_cConnStruct.pConn,
//        "begin;declare testcur cursor for select t1.ctid, t2.ctid, t1.name, t2.name as name2 \
//from featureclass1 t1 inner join luptest t2 on t1.name = t2.name for update");
/*    PGresult *res = PQexec(m_cConnStruct.pConn,
        "begin;declare testcur cursor for select ctid, t1.name \
from featureclass1 t1 where id > 1 order by name for update");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        //HandlePQError(m_hInstance, hwndDlg, m_cConnStruct.pConn);
        WritePQErrorToLog("Declare cursor failed\r\n",
            m_cConnStruct.pConn);
        MessageBox(0, L"Cursor failed", L"Debug", MB_OK);
        PQclear(res);
        res = PQexec(m_cConnStruct.pConn, "end");
        PQclear(res);
    }
    else
    {
        PQclear(res);
        MessageBox(0, L"Cursor OK", L"Debug", MB_OK);

        res = PQdescribePortal(m_cConnStruct.pConn, "testcur");
        if(PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            MessageBox(0, L"Cannot describe the cursor", L"Debug", MB_OK);
            PQclear(res);
            res = PQexec(m_cConnStruct.pConn, "end");
        }
        else
        {
            int idx = PQfnumber(res, "ctid");
            wchar_t wbuf[64];
            swprintf(wbuf, L"Ctid index: %d", idx);
            MessageBox(0, wbuf, L"Debug", MB_OK);
        }
        PQclear(res);

        res = PQexec(m_cConnStruct.pConn, "close testcur;end");
        PQclear(res);
    }*/

    /*PGresult *res = PQprepare(m_cConnStruct.pConn, "teststmt",
        "select ctid, t1.name, t2.name as name2 \
from featureclass1 t1 inner join luptest t2 on t1.name = t2.name",
        0, NULL);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        //HandlePQError(m_hInstance, hwndDlg, m_cConnStruct.pConn);
        WritePQErrorToLog("Statement failed\r\n",
            m_cConnStruct.pConn);
        MessageBox(0, L"Statement failed", L"Debug", MB_OK);
        PQclear(res);
    }
    else
    {
        PQclear(res);
        MessageBox(0, L"Statement OK", L"Debug", MB_OK);
        res = PQexec(m_cConnStruct.pConn, "deallocate teststmt");
        PQclear(res);
    }*/
    return(0);
}

BOOL CDBMainDlg::HelpContent(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    MessageBox(hwndDlg, L"This functionality is not implemented yet",
        L"We are sorry", MB_OK);
    return(0);
}

BOOL CDBMainDlg::HelpAbout(HWND hwndDlg, WORD wNotifyCode, HWND hwndCtl)
{
    MessageBox(hwndDlg, L"This functionality is not implemented yet",
        L"We are sorry", MB_OK);
    return(0);
}
