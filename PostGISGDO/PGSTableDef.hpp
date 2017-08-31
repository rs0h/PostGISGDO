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

#ifndef _PGSTABLEDEF_HPP_
#define _PGSTABLEDEF_HPP_

#include "../Common/CComTypes.hpp"
#include "../Common/PGSTypes.hpp"
#include "PGSCols.hpp"
#include "PGSIndex.hpp"

typedef class GTableDef : public CCOMDispatch
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    LPSTR m_sOrigSchema; // original schema name in the DBencoding
    LPSTR m_sOrigName; // original table name in the DB encoding
    LPWSTR m_wsName; // displayed table name with possible "" decoration
    GTDFields *m_pFields;
    GIndexes *m_pIndices;
    CCollItemStatus m_iStatus;
    LPWSTR m_wsPrimGeom;
    Oid m_lOid;
    bool m_bBuild;
    bool m_bHasUniqueIndex;
    int m_iKeyCnt;
    bool m_bView;
    bool m_bUpdatable;
public:
    GTableDef(IUnknown *pUnkOuter, ITypeLib *pLib);
    GTableDef(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex);
    ~GTableDef();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_GFields(GTDFields* *Fields);
    virtual HRESULT __stdcall get_GIndexes(GIndexes* *Indexes);
    virtual HRESULT __stdcall get_DateCreated(VARIANT *pDate);
    virtual HRESULT __stdcall get_LastUpdated(VARIANT *pLastUpdated);
    virtual HRESULT __stdcall get_Name(BSTR *pName);
    virtual HRESULT __stdcall set_Name(BSTR pName);
    virtual HRESULT __stdcall get_RecordCount(long *pCount);
    virtual HRESULT __stdcall get_Updatable(VARIANT_BOOL *pUpdatable);
    virtual HRESULT __stdcall CreateField(VARIANT Name, VARIANT Type, VARIANT Size,
      GTDField* *ppField);
    virtual HRESULT __stdcall CreateIndex(VARIANT Name, GIndex* *ppIndex);
    virtual HRESULT __stdcall GetExtension(BSTR Name, IDispatch * *ppGExtension);

    void SetNewAttrs(PConnStruct pConnStruct, VARIANT Name);
    void SetTDAttrs(PConnStruct pConnStruct, LPCSTR sSchema, LPCSTR sName,
        bool bView);
    void BuildKeys();
    int BuildFromConnection();
    LPWSTR GetNamePtr();
    LPSTR GetOrigSchemaPtr();
    LPSTR GetOrigNamePtr();
    GIndexes* GetIndexesPtr();
    GTDFields* GetFieldsPtr();
    void SetStatus(CCollItemStatus iStatus);
    GTDField* GetPrimGeomFldPtr();
    void BuildGAlias();
    unsigned long GetSrid(LPSTR sGeomName);
    void SetBuild(bool bBuild);
    Oid GetTableOid();
    void RefreshOid();
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    bool GetHasUniqueIndex();
    int GetKeyCount();
    GTDField* GetFieldPtrByTblPos(int iPos);
    bool GetUpdatable();
} *PTableDef;

class GTableDefs : public _IGDynaCollection
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
public:
    GTableDefs(IUnknown *pUnkOuter, ITypeLib *ALib);
    GTableDefs(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GTableDefs();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall Append(IDispatch * Object);
    virtual HRESULT __stdcall Delete(BSTR Name);
    virtual HRESULT __stdcall get_Item(VARIANT index, GTableDef* *ppGTableDef);
    void ClearAll();
    int BuildFromConnection(PConnStruct pConnStruct);
    //int FindByName(LPSTR sName);
    int FindByWName(BSTR bsName);
    //unsigned long GetGeomSrid(LPSTR sName, LPSTR sGeomName);
    GTableDef* FindByOid(Oid lTblOid);
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
};

#endif
