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

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "PsgDBUMainDlg.hpp"
#include "PsgDBUtils.rh"

extern wchar_t g_ModuleName[MAX_PATH];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    HANDLE hfMtx = CreateMutex(NULL, TRUE, _T("PsgGdoMtx"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        TCHAR sCap[32];
        TCHAR sMsg[128];
        LoadString(hInstance, IDS_ERRORBASE, sCap, 32);
        LoadString(hInstance, IDE_DBU_IS_RUNNING, sMsg, 128);
        MessageBox(0, sMsg, sCap, MB_OK | MB_ICONSTOP);
        ExitProcess(0);
        return(0);
    }

    GetModuleFileNameW(hInstance, g_ModuleName, MAX_PATH);

    INITCOMMONCONTROLSEX ictr;
    ictr.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ictr.dwICC = ICC_STANDARD_CLASSES | ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES |
        ICC_TAB_CLASSES | ICC_PAGESCROLLER_CLASS;
    InitCommonControlsEx(&ictr);

    HACCEL accel = LoadAccelerators(hInstance, _T("MAINACC"));
    CDBMainDlg *mw = new CDBMainDlg(hInstance);
    HWND wnd = mw->DisplayWindow();

    MSG msg;
    BOOL res, finish = FALSE;

    while(!finish)
    {
        res = GetMessage(&msg, 0, 0, 0);
        if (!TranslateAccelerator(wnd, accel, &msg) &&
            !IsDialogMessage(wnd, &msg))
        {
            switch (res)
            {
            case -1:
                MessageBox(wnd, _T("Error?"), _T("Debug"), MB_OK);
            case 0:
                finish = true;
            default:
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    delete mw;

    ReleaseMutex(hfMtx);

    ExitProcess(0);
    return(0);
}
