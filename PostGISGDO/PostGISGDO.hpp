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

#ifndef _PostGISGDO_HPP_
#define _PostGISGDO_HPP_

#include "../Common/CComTypes.hpp"
#include "../Common/GDOUtils.hpp"
#include "../Common/PGSTypes.hpp"
#include "PGSCols.hpp"
#include "PGSTableDef.hpp"
#include "PGSRecordset.hpp"
#include "PGSGeom.hpp"

const int ciTypeLibs = 4; // 0 - GDO.tlb, 1 - ExtendedPropertySet.tlb,
    // 2 - LiterarConversionExtension.tlb, 3 - PAFM.tlb

class GDatabase;

class DBExtendedPropertySet : public CCOMDispatch
{
public:
    DBExtendedPropertySet(GDatabase *pDB, ITypeLib *ALib);
    DBExtendedPropertySet(GDatabase *pDB, ITypeLib *ALib, int iIndex);
    ~DBExtendedPropertySet();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetValue(BSTR PropertyName, VARIANT *Value);
    virtual HRESULT __stdcall SetValue(BSTR PropertyName, VARIANT Value, long *Status);
};

class GDatabase : public CCOMDispatch
{
private:
    CConnStruct m_cConnStruct;
    GRecordsets *m_pRecordsets;
    GTableDefs *m_pTableDefs;
    GErrors *m_pErrors;
    LPWSTR m_sConnect;
    DBExtendedPropertySet *m_pExtPS;
    VARIANT_BOOL m_bModLog;
    VARIANT m_vDefCS;
public:
    GDatabase(HINSTANCE hInstance, IUnknown *pUnkOuter, ITypeLib **ppLibs);
    GDatabase(HINSTANCE hInstance, IUnknown *pUnkOuter, ITypeLib **ppLibs, int iIndex);
    ~GDatabase();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_CollatingOrder(long *pCollatingOrder);
    virtual HRESULT __stdcall get_Connect(BSTR *Connect);
    virtual HRESULT __stdcall get_GAliasTable(BSTR *AliasTableName);
    virtual HRESULT __stdcall get_GErrors(GErrors* *ppGErrors);
    virtual HRESULT __stdcall get_GRecordsets(GRecordsets* *ppGRecordsets);
    virtual HRESULT __stdcall get_GTableDefs(GTableDefs* *ppGTabledefs);
    virtual HRESULT __stdcall get_Name(BSTR *Name);
    virtual HRESULT __stdcall get_SchemaUpdatable(VARIANT_BOOL *Updatable);
    virtual HRESULT __stdcall get_SQLConformance(long *pSQLConformance);
    virtual HRESULT __stdcall get_Transactions(VARIANT_BOOL *Transactions);
    virtual HRESULT __stdcall get_Updatable(VARIANT_BOOL *Updatable);
    virtual HRESULT __stdcall get_Version(BSTR *Version);
    virtual HRESULT __stdcall BeginTrans();
    virtual HRESULT __stdcall ClearModificationLog(VARIANT Entry);
    virtual HRESULT __stdcall Close();
    virtual HRESULT __stdcall CommitTrans();
    virtual HRESULT __stdcall CreateDatabase(BSTR Name, BSTR locale);
    virtual HRESULT __stdcall CreateTableDef(VARIANT Name, GTableDef* *ppGTableDef);
    virtual HRESULT __stdcall GetExtension(BSTR Name, IDispatch * *ppGExtension);
    virtual HRESULT __stdcall OpenDatabase(BSTR Name, VARIANT exclusive, VARIANT readonly,
      VARIANT ModTrack, VARIANT Source);
    virtual HRESULT __stdcall OpenRecordset(BSTR Name, VARIANT Type, VARIANT options,
      VARIANT SpatialOp, VARIANT SpatialGeometryFilter, VARIANT GeometryFieldName,
      GRecordset* *ppGRecordset);
    virtual HRESULT __stdcall Rollback();

    bool CloseDatabase();
    VARIANT_BOOL GetModificationLogging();
    void SetModificationLogging(VARIANT_BOOL bAllow);
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    //void BroadcastDelRecord(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid);
    bool SetDefCS(VARIANT vDefCS);
    void GetDefCS(VARIANT *pvDefCS);
};

#endif
