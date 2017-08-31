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

#ifndef _PGSINDEX_HPP_
#define _PGSINDEX_HPP_

#include "../Common/CComTypes.hpp"
#include "../Common/PGSTypes.hpp"
#include "PGSCols.hpp"
#include "PGSGeom.hpp"

//LPSTR CheckReservedName(BSTR bsName, long lEnc);

typedef struct CFldAttrs
{
    long lAttr;
    short int iType;
    long lSize;
    long lSubType;
    long lColOrder;
    LPSTR sDefValDef;
    LPWSTR wsDefVal;
    bool bRequired;
    bool bUpdatable;
    int iGeomDim;
    int iPsgType;
    unsigned long ulSrid;
    wchar_t sCSGuid[40];
    Oid lTblOid;
    int iTblPos;
    Oid lTypeOid;
    LPSTR sOrigSchema;
    LPSTR sOrigTable;
    LPWSTR wsTblName;
    // next three fields applicable for views
    Oid lBaseTblOid;
    int iBaseTblPos;
} *PFldAttrs;

void InitFldAttrs(PFldAttrs pfa);
void ClearFldAttrs(PFldAttrs pfa);

class FldLitConvExt : public CCOMDispatch
{
public:
    FldLitConvExt(IUnknown *pUnkOuter, ITypeLib *ALib);
    FldLitConvExt(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~FldLitConvExt();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall ConvertToLiteral(VARIANT DataValue,
        BSTR *LiteralString);
    virtual HRESULT __stdcall ConvertFromLiteral(BSTR LiteralString,
        VARIANT *DataValue);
};

// GTDField is common for GTableDef and for GIndex

class GTDField : public CCOMDispatch
{
private:
    CCollItemStatus m_iStatus;
    int m_iFldType; // 0 unknown, 1 table def, 2 index
    LPSTR m_sOrigName; // original field name in DB encoding
    LPWSTR m_wsName; // displayed field name with possible "" decoration
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    CFldAttrs m_cAttrs;
    bool m_bHasDef;
    bool m_bIsKey;
    FldLitConvExt *m_pLitConv;
    int m_iSpecType; // 1 - has ingr field, 2 - has native field
    LPSTR m_sTmpDefVal;
public:
    GTDField(IUnknown *pUnkOuter, ITypeLib *pLib);
    GTDField(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex);
    ~GTDField();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Value(VARIANT *pValue);
    virtual HRESULT __stdcall set_Value(VARIANT pValue);
    virtual HRESULT __stdcall get_AllowZeroLength(VARIANT_BOOL *pAllow);
    virtual HRESULT __stdcall set_AllowZeroLength(VARIANT_BOOL pAllow);
    virtual HRESULT __stdcall get_Attributes(long *pAttr);
    virtual HRESULT __stdcall set_Attributes(long pAttr);
    virtual HRESULT __stdcall get_CollatingOrder(long *pOrder);
    virtual HRESULT __stdcall get_CoordSystemGUID(VARIANT *CSGuid);
    virtual HRESULT __stdcall set_CoordSystemGUID(VARIANT CSGuid);
    virtual HRESULT __stdcall get_DataUpdatable(VARIANT_BOOL *pUpdatable);
    virtual HRESULT __stdcall set_DefaultValue(BSTR pValue);
    virtual HRESULT __stdcall get_DefaultValue(BSTR *pValue);
    virtual HRESULT __stdcall get_Name(BSTR *pName);
    virtual HRESULT __stdcall set_Name(BSTR pName);
    virtual HRESULT __stdcall get_Required(VARIANT_BOOL *pRequired);
    virtual HRESULT __stdcall set_Required(VARIANT_BOOL pRequired);
    virtual HRESULT __stdcall get_Size(long *pSize);
    virtual HRESULT __stdcall set_Size(long pSize);
    virtual HRESULT __stdcall get_SourceDatabase(BSTR *pDatabase);
    virtual HRESULT __stdcall get_SourceField(BSTR *pField);
    virtual HRESULT __stdcall get_SourceTable(BSTR *pTable);
    virtual HRESULT __stdcall get_SubType(long *pGeometryType);
    virtual HRESULT __stdcall set_SubType(long pGeometryType);
    virtual HRESULT __stdcall get_Type(short *pType);
    virtual HRESULT __stdcall set_Type(short pType);
    virtual HRESULT __stdcall AppendChunk(VARIANT Val);
    virtual HRESULT __stdcall FieldSize(long *pSize);
    virtual HRESULT __stdcall GetChunk(long Offset, long Bytes, VARIANT *pChunk);
    virtual HRESULT __stdcall GetExtension(BSTR Name, IDispatch * *ppGExtension);

    LPSTR GetOrigNamePtr();
    LPSTR GetOrigSchemaPtr();
    LPSTR GetOrigTablePtr();
    LPWSTR GetNamePtr();
    LPWSTR GetTblNamePtr();

    void SetNewProps(PConnStruct pConnStruct, VARIANT Name, VARIANT Type,
        VARIANT Size, int iFldType, LPSTR sOrigSchema, LPSTR sOrigTable,
        LPWSTR wsTblName);
    void CheckDefault();
    void UpdateAttrs();
    void SetTDProps(PConnStruct pConnStruct, LPCSTR sName, int iColPos,
        Oid lTypeOid, int iSize, long lMod, bool bRequired, bool bHasDef,
        Oid lTblOid, LPSTR sOrigSchema, LPSTR sOrigTable, LPWSTR wsTblName,
        PGresult *pViewRes, LPSTR sDefault, bool bExtInfo, bool bIsKey,
        long lGeomType, LPSTR sCSGuid);
    void SetIdxProps(PConnStruct pConnStruct, LPCSTR sName, bool bDesc,
        int iType);
    void GetSQLTypeStr(LPSTR sBuf);
    void SetStatus(CCollItemStatus iStatus);
    int GetType();
    void GetGeomTypeStr(LPSTR sBuf);
    bool GetDesc();
    bool GetRequired();
    long GetGeomType();
    int GetPsgType();
    unsigned long GetSrid();
    bool IsNativeGeometry();
    bool IsEmulGeometry();
    void CopyAttributes(PFldAttrs pAttrs);
    void ChangeCSGuid(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    void SetTblOid(Oid lTblOid);
    int GetTblPos();
    void SetTblPos(int iPos);
    bool IsAutoNum();
    bool IsSmallField();
    bool IsKeyField();
    void SetKeyField(bool bKey);
    void UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
        LPWSTR wsNewTblName);
    void SetHasSpecField(int iSpecType);
    LPSTR GetFilterFieldName();
    bool AddIngrField();
    void DropIngrField();
    bool AddNativeField();
    void DropNativeField();
};

class GTDFields : public _IGDynaCollection
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    int m_iFldType; // 0 unknown, 1 table def, 2 index
    CCollItemStatus m_iStatus;
    LPWSTR m_wsTblName;
    LPWSTR m_wsIdxName;
    void RemSpecFields();
public:
    GTDFields(IUnknown *pUnkOuter, ITypeLib *pLib);
    GTDFields(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex);
    ~GTDFields();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall Append(IDispatch * Object);
    virtual HRESULT __stdcall Delete(BSTR Name);
    virtual HRESULT __stdcall get_Item(VARIANT index, GTDField* *ppGField);

    void ClearAll();
    GTDField* AddField(LPCSTR sName, bool bDesc, int iType);
    int FindByName(LPCSTR sName);
    GTDField* GetByName(LPCSTR sName);
    int FindByWName(LPWSTR wsName);
    GTDField* GetByWName(LPWSTR wsName);
    GTDField* GetByTblPos(int iPos);
    int BuildFromConnection(PConnStruct pConnStruct, Oid lTblOid,
        LPSTR sOrigSchema, LPSTR sOrigName, LPWSTR wsTblName, bool bView);
    void BuildFromIndex(PConnStruct pConnStruct, LPWSTR wsTblName,
        LPWSTR wsIdxName);
    void UpdateName(LPWSTR wsIdxName);
    void SetNewAttrs(PConnStruct pConnStruct, LPWSTR wsTblName);
    int BuildFromGAlias(PConnStruct pConnStruct, LPSTR sOrigSchema,
        LPSTR sOrigTable, LPWSTR wsTblName);
    void SetStatus(CCollItemStatus iStatus);
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    void SetTblOids(Oid lTblOid);
    void Sort();
    GTDField* GetAutoNumFld();
    void ResetKeyFields();
    int SetKeyFields(GIndex *pIdx);
    bool HasKeyField();
    void UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigName, LPWSTR wsNewTblName);
};

class GIndex : public CCOMDispatch
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    CCollItemStatus m_iStatus;
    GTDFields *m_pFields;
    LPSTR m_sOrigName; // original index name in DB encoding
    LPWSTR m_wsName; // displayed index name with possible "" decoration
    LPWSTR m_wsTblName;
    bool m_bPrimary;
    bool m_bUnique;
    bool m_bNulls;
    Oid m_lOid;
    LPSTR m_sOrigSchema;
    LPSTR m_sOrigTable;
public:
    GIndex(IUnknown *pUnkOuter, ITypeLib *ALib);
    GIndex(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GIndex();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_GFields(GTDFields* *pVar);
    virtual HRESULT __stdcall get_IgnoreNulls(VARIANT_BOOL *pb);
    virtual HRESULT __stdcall set_IgnoreNulls(VARIANT_BOOL pb);
    virtual HRESULT __stdcall get_Name(BSTR *Name);
    virtual HRESULT __stdcall set_Name(BSTR Name);
    virtual HRESULT __stdcall get_Primary(VARIANT_BOOL *pPrimary);
    virtual HRESULT __stdcall set_Primary(VARIANT_BOOL pPrimary);
    virtual HRESULT __stdcall set_Required(VARIANT_BOOL pRequired);
    virtual HRESULT __stdcall get_Required(VARIANT_BOOL *pRequired);
    virtual HRESULT __stdcall get_Unique(VARIANT_BOOL *pUnique);
    virtual HRESULT __stdcall set_Unique(VARIANT_BOOL pUnique);
    virtual HRESULT __stdcall CreateField(VARIANT Name, VARIANT Type,
        VARIANT Size, GTDField* *ppField);

    void SetNewAttrs(PConnStruct pConnStruct, VARIANT Name, LPWSTR wsTblName,
        LPSTR sOrigSchema, LPSTR sOrigTable);
    void UpdateName();
    void SetTDAttrs(PConnStruct pConnStruct, Oid lIdxOid, bool bPrimary,
        bool bUnique, bool bNulls, LPWSTR wsTblName);
    void SetNameAttrs(PConnStruct pConnStruct, LPCSTR sName, bool bPrimary,
        bool bUnique, bool bNulls, LPWSTR wsTblName);
    void AddFieldName(LPCSTR sName, bool bDesc, GTDFields *pFields);
    void AddFieldPos(int iPos, bool bDesc, GTDFields *pFields);
    LPWSTR GetNamePtr();
    LPSTR GetOrigNamePtr();
    bool GetPrimary();
    bool GetUnique();
    void SetPrimary(bool bPrimary);
    GTDFields* GetFieldsPtr();
    void SetStatus(CCollItemStatus iStatus);
    void UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
        LPWSTR wsNewTblName);
};

class GIndexes : public _IGDynaCollection
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    LPWSTR m_wsTblName;
    CCollItemStatus m_iStatus;
public:
    GIndexes(IUnknown *pUnkOuter, ITypeLib *ALib);
    GIndexes(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GIndexes();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall Append(IDispatch * Object);
    virtual HRESULT __stdcall Delete(BSTR Name);
    virtual HRESULT __stdcall get_Item(VARIANT index, GIndex* *ppGIndex);

    void ClearAll();
    int FindByWName(LPWSTR wsName);
    GIndex* GetByWName(LPWSTR wsName);
    int BuildFromConnection(PConnStruct pConnStruct, Oid lTblOid,
        LPWSTR wsTblName, GTDFields *pFields);
    void SetNewAttrs(PConnStruct pConnStruct, LPWSTR wsTblName);
    GIndex* GetPrimaryKey();
    GIndex* GetUniqueIdx();
    int BuildFromGAlias(PConnStruct pConnStruct, LPWSTR wsTblName,
        GTDFields *pFields);
    void SetStatus(CCollItemStatus iStatus);
    int CreatePseudoIndex(PConnStruct pConnStruct, LPSTR sOrigTblName,
        LPWSTR wsTblName, GTDFields *pFields);
    void UpdateTableName(LPSTR sOrigSchema, LPSTR sOrigTable,
        LPWSTR wsNewTblName);
};

LPWSTR GetValidFieldName(BSTR bsName, long lEnc, LPSTR *psName);
int GetTableNameLen(LPSTR sSchema, LPSTR sTable);
void CatTableName(LPSTR sCmd, LPSTR sSchema, LPSTR sTable, bool bDecorate);
void CatFieldName(LPSTR sCmd, LPSTR sField, bool bDecorate);

#endif
