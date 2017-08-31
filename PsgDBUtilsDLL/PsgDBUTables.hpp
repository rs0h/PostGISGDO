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

#ifndef _PGSDBUTABLES_HPP_
#define _PGSDBUTABLES_HPP_

#include <tchar.h>
#include "../Common/GDOUtils.hpp"
#include "../Common/PGSCoordSystems.hpp"
#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif

#include "../Common/PsgDBUInterfaces.hpp"

class CDbuField : public IDbuField
{
private:
    PConnStruct m_pConnStruct;
    LPSTR m_sOrigSchema;
    LPSTR m_sOrigTable;
    LPTSTR m_tsTblName;
    Oid m_lTblOid;
    LPSTR m_sOrigName;
    LPTSTR m_tsName;
    int m_iColPos;
    int m_iType;
    long m_lSize;
    long m_lAttr;
    long m_lSubType;
    bool m_bRequired;
    int m_iGeomDim;
    int m_iPsgType;
    unsigned long m_ulSrid;
    bool m_bHasDefault;
    bool m_bVisible;
    bool m_bHasChanged;
    bool m_bIsKey; // primary geometry for geometry fields
    bool m_bHypertext;
    TCHAR m_tsDescription[256];
    LPSTR m_sDefault;
    int m_iDateType;
    bool m_bHasSpecField;

    void CheckDefault(LPSTR sSchema, LPSTR sTblName, LPSTR sFldName);
    void CheckAttrMeta(LPTSTR tsTblName, LPTSTR tsFldName);
    void CheckGeomMeta(LPTSTR tsTblName, LPTSTR tsFldName);
public:
    CDbuField(PConnStruct pConnStruct, LPSTR sName, Oid lPgType, int iSize,
        long lMod, bool bNullable, bool bHasDefault, LPSTR sOrigSchema,
        LPSTR sOrigTable, LPTSTR tsTblName, Oid lTblOid, int iColPos);
    ~CDbuField();
    virtual LPTSTR _stdcall GetNamePtr();
    virtual bool _stdcall GetVisible();
    virtual void _stdcall SetVisible(bool bVisible);
    virtual int _stdcall GetType();
    virtual void _stdcall SetType(int iType);
    virtual long _stdcall GetSubType();
    virtual void _stdcall SetSubType(long lSubType);
    virtual unsigned long _stdcall GetSrid();
    virtual void _stdcall SetSrid(unsigned long ulSrid);
    virtual bool _stdcall GetIsKey();
    virtual void _stdcall SetIsKey(bool bIsKey);
    virtual LPTSTR _stdcall GetDescription();
    virtual void _stdcall SetDescription(LPTSTR sDesc);
    virtual bool _stdcall GetHasSpecField();
    virtual bool _stdcall GetIsHypertext();
    virtual void _stdcall SetIsHypertext(bool bIsHypertext);
    LPSTR GetOrigNamePtr();
    bool UpdateAttrs(LPTSTR *psPrimGeomName, bool bPrimGeomSet);
    int GetDateType();
    void SetHasSpecField(bool bHas);
    int GetColPos();
};

class CDbuFields : public IDbuFields
{
private:
    int m_iDataLen;
    int m_iDataSize;
    CDbuField **m_pFields;
    PConnStruct m_pConnStruct;
    void AddField(CDbuField *pFld);
    void DelField(int iIndx);
    void RemSpecFields();
    CDbuField* FindByColPos(int iColPos);
public:
    CDbuFields(PConnStruct pConnStruct);
    ~CDbuFields();
    virtual LPTSTR _stdcall GetNextGeomName(int *piStart);
    virtual int _stdcall GetCount();
    virtual IDbuField* _stdcall GetItem(int idx);
    virtual IDbuField* _stdcall FindByName(LPTSTR tsName);
    void BuildFromConnection(Oid lOid, LPSTR sOrigSchema,
        LPSTR sOrigTable, LPTSTR tsTblName, LPTSTR *ptsPrimGeom);
    void BuildPKsFromConnection(LPSTR sOid);
    int FindPosByName(LPTSTR tsName);
};

class CDbuTable : public IDbuTable
{
private:
    PConnStruct m_pConnStruct;
    Oid m_lOid;
    bool m_bLoaded;
    bool m_bHasIndex;
    CDbuFields *m_pFields;
    LPSTR m_sOrigSchema;
    LPSTR m_sOrigName;
    LPTSTR m_tsName;
    LPTSTR m_tsPrimGeom;
    TCHAR m_tsDescription[256];
    bool m_bVisible;
    bool m_bHasChanged;
    bool m_bHasMetadata;
    bool m_bIsView;

    void InsertAttrMetadata(LPTSTR tsTblName, LPSTR sSchema, LPSTR sTable,
        CDbuField *pfld);
    void InsertGeomMetadata(LPTSTR tsTblName, LPSTR sSchema, LPSTR sTable,
        CDbuField *pfld);
    void InsertFeatureMetadata();
    void LoadFields();
    bool DropINGRField(CDbuField *pField);
    bool CreateINGRField(CDbuField *pField);
    bool DropNativeField(CDbuField *pField);
    bool CreateNativeField(CDbuField *pField);
    void PopulateNatField(CDbuField *pField, long lSubType);
public:
    CDbuTable(PConnStruct pConnStruct, LPSTR sSchema, LPSTR sTable,
        bool bHasIndex);
    ~CDbuTable();
    virtual LPTSTR _stdcall GetNamePtr();
    virtual bool _stdcall GetVisible();
    virtual void _stdcall SetVisible(bool bVisible);
    virtual void _stdcall SaveTableMetadata();
    virtual LPTSTR _stdcall GetPrimaryGeometry();
    virtual void _stdcall SetPrimaryGeometry(LPTSTR sGeomName);
    virtual LPTSTR _stdcall GetDescription();
    virtual void _stdcall SetDescription(LPTSTR sDesc);
    virtual IDbuFields* _stdcall GetFields();
    virtual bool _stdcall GetIsView();
    virtual void _stdcall SetChanged(bool bChanged);
    virtual bool _stdcall ChangeSpecField(IDbuField *pField);
    void RefreshFromConn(LPCSTR stmtOid, LPCSTR stmtFtr);
    void SetIsView(bool bIsView);
};

typedef class CDbuTables : public IDbuTables
{
private:
    int m_iDataSize;
    int m_iDataLen;
    CDbuTable **m_pTables;
    PConnStruct m_pConnStruct;
    void AddTable(CDbuTable *pTbl);
    bool IsNotMetaTable(LPSTR sName, bool bTrans);
public:
    CDbuTables(PConnStruct pConnStruct);
    ~CDbuTables();
    virtual int _stdcall GetCount();
    virtual IDbuTable* _stdcall GetItem(int idx);
    void BuildFromConn();
    void UpdateAllTables(bool bVisible);
} *PDbuTables;

#endif
