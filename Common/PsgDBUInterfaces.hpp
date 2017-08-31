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

#ifndef _PGSDBUINTERFACES_HPP_
#define _PGSDBUINTERFACES_HPP_

#include <tchar.h>
#include "GDOUtils.hpp"
#include "PGSCoordSystems.hpp"
#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif

struct IDbuField
{
    virtual LPTSTR _stdcall GetNamePtr() = 0;
    virtual bool _stdcall GetVisible() = 0;
    virtual void _stdcall SetVisible(bool bVisible) = 0;
    virtual int _stdcall GetType() = 0;
    virtual void _stdcall SetType(int iType) = 0;
    virtual long _stdcall GetSubType() = 0;
    virtual void _stdcall SetSubType(long lSubType) = 0;
    virtual unsigned long _stdcall GetSrid() = 0;
    virtual void _stdcall SetSrid(unsigned long ulSrid) = 0;
    virtual bool _stdcall GetIsKey() = 0;
    virtual void _stdcall SetIsKey(bool bIsKey) = 0;
    virtual LPTSTR _stdcall GetDescription() = 0;
    virtual void _stdcall SetDescription(LPTSTR sDesc) = 0;
    virtual bool _stdcall GetHasSpecField() = 0;
    virtual bool _stdcall GetIsHypertext() = 0;
    virtual void _stdcall SetIsHypertext(bool bIsHypertext) = 0;
};

struct IDbuFields
{
    virtual LPTSTR _stdcall GetNextGeomName(int *piStart) = 0;
    virtual int _stdcall GetCount() = 0;
    virtual IDbuField* _stdcall GetItem(int idx) = 0;
    virtual IDbuField* _stdcall FindByName(LPTSTR tsName) = 0;
};

struct IDbuTable
{
    virtual LPTSTR _stdcall GetNamePtr() = 0;
    virtual bool _stdcall GetVisible() = 0;
    virtual void _stdcall SetVisible(bool bVisible) = 0;
    virtual void _stdcall SaveTableMetadata() = 0;
    virtual LPTSTR _stdcall GetPrimaryGeometry() = 0;
    virtual void _stdcall SetPrimaryGeometry(LPTSTR sGeomName) = 0;
    virtual LPTSTR _stdcall GetDescription() = 0;
    virtual void _stdcall SetDescription(LPTSTR sDesc) = 0;
    virtual IDbuFields* _stdcall GetFields() = 0;
    virtual bool _stdcall GetIsView() = 0;
    virtual void _stdcall SetChanged(bool bChanged) = 0;
    virtual bool _stdcall ChangeSpecField(IDbuField *pField) = 0;
};

struct IDbuTables
{
    virtual int _stdcall GetCount() = 0;
    virtual IDbuTable* _stdcall GetItem(int idx) = 0;
};

// returns non zero connection handle, returns zero in case of an error
extern "C" long _stdcall DbuOpenConnection(LPWSTR sServer, LPWSTR sDatabase,
    LPWSTR sUser, LPWSTR sPasswd, int iPort);

// returns true if succesful
extern "C" bool _stdcall DbuCloseConnection(long lHandle);

// recturns connection status as declared in GDOUtils.hpp
extern "C" CConnStatus _stdcall DbuGetConnectionStatus(long lHandle);

// creates metadata tables, if bFull is true, it creates full INGR metadata
// otherwise it only creates simple metadata
extern "C" bool _stdcall DbuCreateMetadata(long lHandle, bool bFull);

// drops all the metadata tables and gdo schema
extern "C" bool _stdcall DbuDropMetadata(long lHandle);

// intended for PsgDBUtils internal use
extern "C" IDbuTables* _stdcall DbuGetTables(long lHandle);

// updates metadata for all non-metadata tables, makes the tables either
// visible or invisible to GeoMedia
extern "C" bool _stdcall DbuUpdateAllTables(long lHandle, bool bVisible);

// also intended for internal use. Fills either a list box or a combo box
// with the names of all available users, except superusers and the
// current user. hwnd can be either handle to a listbox or a combobox
// iMsg is either LB_ADDSTRING or CB_ADDSTRING, according to the control type
extern "C" bool _stdcall DbuFillCtrlWithUserNames(long lHandle, HWND hwnd, UINT iMsg);

// also intended for internal use. Fills either a list box or a combo box
// with the srids currently used by all geometry columns. hwnd can be either
// handle to a listbox or a combobox iMsg is either LB_ADDSTRING or
// CB_ADDSTRING, according to the control type
extern "C" bool _stdcall DbuFillCtrlWithSrids(long lHandle, HWND hwnd, UINT iMsg);

// grants provileges to the GDO schema to the user named sUser. iMode can be:
//    0 - revoke all rights
//    1 - grant read access
//    2 - grant modify access
//    3 - grant admin rights
extern "C" bool _stdcall DbuGrantGdoPrivileges(long lHandle, int iMode, LPWSTR sUser);

// attemts to run an arbitrary sql script loaded from file sFileName
extern "C" bool _stdcall DbuRunScript(long lHandle, LPWSTR sFileName);

// clears the modification log table
extern "C" bool _stdcall DbuClearModificationLog(long lHandle);

#endif
