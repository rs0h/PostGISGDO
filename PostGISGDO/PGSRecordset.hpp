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

#ifndef _PGSRECORDSET_HPP_
#define _PGSRECORDSET_HPP_

#include "../Common/CComTypes.hpp"
#include "../Common/GDOUtils.hpp"
#include "../Common/PGSTypes.hpp"
#include "../Common/PAFM_TLB.hpp"
#include "PGSCols.hpp"
#include "PGSGeom.hpp"
#include "PGSTableDef.hpp"


// General notes on the GRecordset implementation:
// PostgreSQL provides cursors, which can be declared for update. This would
// theoretically allow to implement the most wide range of read/write
// recordsets. However, the cursors have some drawbacks. For example,
// a cursor declared for update cannot be backward moving and must have
// an explicitelly defined transaction. For our purpose, these are very strong
// limitations, so we will use cursors only for loading data, not for updates.
//
// For updates, we will use the ctid system column. We will inject the ctid
// column into the select query and try to prepare the recordset statement
// (PQprepare), even if it does not involve parameters. This will later allow
// us to get the binary results.
//
// If the statement is prepared OK, then it means that we can use the ctid
// column to update the records. If the statement fails, it means most likely
// that the query contains joins, aggregation, grouping etc. In this case
// we will consider the query as read only, even if it might be read/write.
// In this case, we will prepare the original statement without the ctid column.
//
// Then we create a normal, forward only cursor. The data will be read from
// that cursor into a memory buffer. The reading will not be immediate, we
// will rather wait for the calling application to access all the records.
// Once the end of the cursor is reached, it will be closed.
// (Actually the current implementation reads all the data at once, it is the
// fastest way in most cases.)
//
// To create the standard cursor, we need to start a transaction, and this
// transaction must not end until the whole data is read into the memory.
// For that reason, each single execution on the database must be enclosed
// by the SAVEPOINT / RELEASE SAVEPOINT pairs. We will also need a custom
// transaction counter, let's call it iSysTrans to see whether the transaction
// can be ended or not. The save points might cause a bit delay, however
// they will only be applied on edits, not on fetch, so this should not
// be critical for the performance.
//
// All inserts, edits and deletions will be immediately committed, the deleted
// records will be marked and held in the buffer.

typedef struct CBufRow
{
    int iState; // 0 normal, 1 inserted, 2 deleted
    int iCtidLen;
    char sCtid[16];
    VARIANT *pvData;
} *PBufRow;

class FldExtendedPropertySet : public CCOMDispatch
{
private:
    long m_lSinglePass;
    long m_lCache;
    GTDField *m_pField;
public:
    FldExtendedPropertySet(IUnknown *pUnkOuter, ITypeLib *ALib);
    FldExtendedPropertySet(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~FldExtendedPropertySet();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetValue(BSTR PropertyName, VARIANT *Value);
    virtual HRESULT __stdcall SetValue(BSTR PropertyName, VARIANT Value,
        long *Status);
    void SetTDField(GTDField *pField);
};

class GField : public CCOMDispatch
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;
    int m_iPos;
    LPSTR m_sOrigName; // original field name in DB encoding
    LPWSTR m_wsName; // displayed field name
    FldExtendedPropertySet *m_pExt;
    FldLitConvExt *m_pLitConv;
    CFldAttrs m_cAttrs;

    LPSTR m_sNewIdCmd, m_sGenIdCmd;
    VARIANT *m_pvBuffer;
    VARIANT m_vNewVal;
    int m_iSpecType; // 0 - unknown, 1 - GAliasTable, 2 - GCoordSystem
    int m_iSpecField; // 1 - has INGR field, 2 - has Native field;
    GField *m_pSpecField;
    bool m_bIsMetatable;
public:
    GField(IUnknown *pUnkOuter, ITypeLib *ALib);
    GField(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GField();
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

    LPWSTR GetNamePtr();
    LPSTR GetOrigNamePtr();

    Oid GetTableOid();
    void SetAttrs(PConnStruct pConnStruct, PGresult *pRes, int iPos, Oid lType,
        bool bUpdatable, int iSpecType, bool bMetatable);
    int UpdateField(GTableDefs *pGTbls);
    void SetNewVal();
    void SetBuffer(VARIANT *pBuffer);
    void CancelNewVal();
    bool Modified();
    bool HasSeq();
    VARIANT GetModVal();
    void UpdateVal(VARIANT *pvVal);
    bool GetRequired();
    Oid GetTypeOid();
    LPSTR GetSchemaNamePtr();
    LPSTR GetTableNamePtr();

    void FillGeomInfo(PGeomInfo pgInfo);
    void ChangeCSGuid(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    void SetSpecField(GField *pSpecFld, int iSpecFld);
    void SetSpecFldAttrs(unsigned long ulSrid, long iGDOGeomType,
        LPSTR sSchema, LPSTR sTable);
    bool IsThisField(LPSTR sSchema, LPSTR sTable, LPSTR sName);
};

class GFields : public _IGDynaCollection
{
private:
    PConnStruct m_pConnStruct;
    GTableDefs *m_pTblDefs;
    int m_iCtidCol;
    bool m_bIsMetatable;
    int m_iSpecFields;
    int *m_piListIndex;
public:
    GFields(IUnknown *pUnkOuter, ITypeLib *ALib);
    GFields(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GFields();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Count(short *Count);
    virtual HRESULT __stdcall _NewEnum(IUnknown * *Enum);
    virtual HRESULT __stdcall Append(IDispatch * Object);
    virtual HRESULT __stdcall Delete(BSTR Name);
    virtual HRESULT __stdcall get_Item(VARIANT index, GField* *ppGField);
    void ClearAll();
    void BuildFromConn(PConnStruct pConnStruct, PGresult *pRes,
        GTableDefs *pGTbls, int iCtidCol, Oid *plTypes, int iSpecType,
        bool bIsMetatable);
    void UpdateFields(int *piGeomDims);
    //GField* FindKeyField();
    //GField* FindByName(LPSTR sVal);
    GField* FindByWName(BSTR bsVal);
    void SetBuffer(VARIANT *pBuffer);
    void SetNewRow();
    void GetNewRow(VARIANT *pBuffer);
    void CancelNewRow();
    void UpdateRow(VARIANT *pBuffer);
    GTableDefs* GetTableDefsPtr();
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    int HasField(LPSTR sSchema, LPSTR sTable, LPSTR sName);
#if DBGLEVEL > 0
    GRecordset* GetParentRecordset();
#endif
};

class GRecordsets;

class GFeatureClassExt : public CCOMDispatch
{
private:
    LPWSTR m_wsTblName;
public:
    GFeatureClassExt(IUnknown *pUnkOuter, ITypeLib *ALib);
    GFeatureClassExt(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GFeatureClassExt();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_AssociationRoleClasses(_GAssociationRoleClasses* *objClasses);
    virtual HRESULT __stdcall get_AttributeClasses(_GAttributeClasses* *objClasses);
    virtual HRESULT __stdcall get_KeyAttributeNames(VARIANT *pvarKeyAttributeNames);
    virtual HRESULT __stdcall get_Name(BSTR *strVal);
    virtual HRESULT __stdcall get_OperationClasses(_GOperationClasses* *objClasses);
    virtual HRESULT __stdcall get_Parent(IDispatch * *objParent);
    virtual HRESULT __stdcall get_RuleClasses(_GRuleClasses* *objClasses);
    virtual HRESULT __stdcall GetFeature(GRecordset* objSelection, _GFeature* *objIFeature);
    virtual HRESULT __stdcall OpenSelection(BSTR bstrFilter, VARIANT vType, VARIANT vOptions,
        VARIANT vSpatialOperator, VARIANT vSpatialGeometryFilter, VARIANT vGeometryFieldName,
        GRecordset* *ppSelection);
    void SetTableName(LPWSTR wsTblName);
};

class GRecordset : public CCOMDispatch
{
private:
    PConnStruct m_pConnStruct;
    GErrors *m_pErrors;

    int m_iSpecType; // 0 - unknown, 1 - GAliasTable, 2 - GCoordSystem
    wchar_t m_sName[256];
    VARIANT m_vType;
    VARIANT m_voptions;
    VARIANT m_vSpatialOp;
    VARIANT m_vSpatialGeometryFilter;
    VARIANT m_vGeometryFieldName;

    bool m_bUpdatable;
    bool m_bIsMetatable;

    CBufRow m_vBuffer;
    long m_lBufSize;
    long m_lRecordCount;
    long m_lCurPos;
    long m_lModPos;
    PGresult *m_pPGres;
    bool m_bBOF, m_bEOF;

    GFields *m_pFields;
    short m_iEditMode;
    int m_iOpenStatus; // 0 - closed, 1 - opened, 2 - all data read
    char m_sSelect[64];
    Oid *m_plTypes;
    Oid m_lTblOid;
    int m_iCtidCol;
    int m_iRowSize;

    GTableDef *m_pTblDef; // NULL, if not updatable, Ctid relation otherwise
    GRecordsets *m_pParent;
    GFeatureClassExt *m_pFCExt;

    int *m_piGeomDims;

    char m_sStmtName[32];
    int m_iParams;
    LPSTR *m_psParams;
    int *m_piParLenght;
    int *m_piParType;
    long m_lInserted;

    bool IsCurDeleted();
    void SyncBuf();
    void SyncFields();
    //long FindByCtid(int iCtidLen, LPSTR pCtid);
public:
    GRecordset(GRecordsets *pParent, ITypeLib *ALib);
    GRecordset(GRecordsets *pParent, ITypeLib *ALib, int iIndex);
    ~GRecordset();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_GFields(GFields* *ppGFields);
    virtual HRESULT __stdcall get_BOF(VARIANT_BOOL *pb);
    virtual HRESULT __stdcall get_Bookmark(SAFEARRAY **ppsach);
    virtual HRESULT __stdcall set_Bookmark(SAFEARRAY **ppsach);
    virtual HRESULT __stdcall get_Bookmarkable(VARIANT_BOOL *pb);
    virtual HRESULT __stdcall get_EditMode(short *pi);
    virtual HRESULT __stdcall get_EOF(VARIANT_BOOL *pb);
    virtual HRESULT __stdcall get_LastModified(SAFEARRAY * *LastModified);
    virtual HRESULT __stdcall get_Name(BSTR *Name);
    virtual HRESULT __stdcall get_RecordCount(long *RecordCount);
    virtual HRESULT __stdcall get_Type(short *Type);
    virtual HRESULT __stdcall get_Updatable(VARIANT_BOOL *Updatable);
    virtual HRESULT __stdcall AddNew();
    virtual HRESULT __stdcall CancelUpdate();
    virtual HRESULT __stdcall Close();
    virtual HRESULT __stdcall Delete();
    virtual HRESULT __stdcall Edit();
    virtual HRESULT __stdcall GetExtension(BSTR Name, IDispatch * *ppGExtension);
    virtual HRESULT __stdcall GetRows(VARIANT Rows, VARIANT *pVar);
    virtual HRESULT __stdcall Move(long Rows, VARIANT StartBookmark);
    virtual HRESULT __stdcall MoveFirst();
    virtual HRESULT __stdcall MoveLast();
    virtual HRESULT __stdcall MoveNext();
    virtual HRESULT __stdcall MovePrevious();
    virtual HRESULT __stdcall Update();

    LPWSTR GetNamePtr();
    bool SetAttributes(BSTR Name, VARIANT Type, VARIANT options,
        VARIANT SpatialOp, VARIANT SpatialGeometryFilter,
        VARIANT GeometryFieldName, GTableDefs *pGTbls, PConnStruct pConnStruct);
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    //void RecordDeleted(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid);
    bool GetIsMetatable();
    //void UpdateWithDBChanges(int iModType, VARIANT *pvModTable,
    //    VARIANT *pvModData, PGresult *res, LPSTR sSchema, LPSTR sTable,
    //    bool bHasCtid, int iCtidLen, LPSTR pCtid);
    int GetFieldsCount();
    long GetBufSize();
    bool GetRow(long lPos, PBufRow pBuf);
};

class GRecordsets : public _IGCollection
{
private:
    PConnStruct m_pConnStruct;
    GTableDefs *m_pGTbls;
    //void ProcessModLogRec(PBufRow pRow);
public:
    GRecordsets(IUnknown *pUnkOuter, ITypeLib *ALib);
    GRecordsets(IUnknown *pUnkOuter, ITypeLib *ALib, int iIndex);
    ~GRecordsets();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall get_Item(VARIANT index, GRecordset* *ppGRecordset);
    virtual HRESULT __stdcall Append(GRecordset* pObject);
    virtual HRESULT __stdcall Remove(VARIANT vIndex);
    void ClearAll();
    GRecordset* FindRecordset(BSTR Name, VARIANT Type, VARIANT options,
        VARIANT SpatialOp, VARIANT SpatialGeometryFilter,
        VARIANT GeometryFieldName, GTableDefs *pGTbls, PConnStruct pConnStruct);
    void BroadcastCSChange(LPWSTR wsOldCSGuid, LPWSTR wsNewCSGuid);
    //void BroadcastDelRecord(LPWSTR wsTblName, int iCtidLen, LPSTR sCtid);
    void DeletePtr(GRecordset *pRS);
    //void BroadcastDataChanges(GRecordset* pRS);
};

#endif
