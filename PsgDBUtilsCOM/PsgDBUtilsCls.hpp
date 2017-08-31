#ifndef _PSGDBUCLS_HPP_
#define _PSGDBUCLS_HPP_

#ifdef __GNUC__
#include "../libpq/libpq-fe.h"
#else
#include <libpq-fe.h>
#endif

#include <basetyps.h>
#include <wtypes.h>
#include <ocidl.h>

#include "../PsgDBUtilsDLL/PsgDBUTables.hpp"
#include "../Common/CComTypes.hpp"
#include "../PsgDBUtilsDLL/PsgDBUDLLMain.hpp"

const CLSID CLASS_DbuConnection = {0x0132C3DC,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};

const IID DIID_DbuConnection = {0x0132C3DD,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};
const IID DIID_DbuField = {0x0132C3DE,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};
const IID DIID_DbuFields = {0x0132C3DF,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};
const IID DIID_DbuTable = {0x0132C3E0,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};
const IID DIID_DbuTables = {0x0132C3E1,0xDEC6,0x11E4,{0x84,0x2E,0x08,0x00,0x27,0x2B,0x53,0xA1}};

class CDbuFieldDisp : public CCOMDispatch
{
private:
    CDbuField *m_pField;
public:
    CDbuFieldDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex, CDbuField *pField);
    ~CDbuFieldDisp();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetName(BSTR *pbsName);
    virtual HRESULT __stdcall GetVisible(bool *pbVisible);
    virtual HRESULT __stdcall SetVisible(bool bVisible);
    virtual HRESULT __stdcall GetType(int *piType);
    virtual HRESULT __stdcall SetType(int iType);
    virtual HRESULT __stdcall GetSubType(long *plSubType);
    virtual HRESULT __stdcall SetSubType(long lSubType);
    virtual HRESULT __stdcall GetSrid(unsigned long *pulSrid);
    virtual HRESULT __stdcall SetSrid(unsigned long ulSrid);
    virtual HRESULT __stdcall GetIsKey(bool *pbIsKey);
    virtual HRESULT __stdcall SetIsKey(bool bIsKey);
    virtual HRESULT __stdcall GetDescription(BSTR *pbsDesc);
    virtual HRESULT __stdcall SetDescription(BSTR bsDesc);
    virtual HRESULT __stdcall GetHasSpecField(bool *pbHasSpecFld);
    virtual HRESULT __stdcall GetIsHypertext(bool *pbIsHypertext);
    virtual HRESULT __stdcall SetIsHypertext(bool bIsHypertext);

    CDbuField* GetBaseField();
};

struct CDbuFieldsDisp : public CCOMDispatch
{
private:
    CDbuFields *m_pFields;
    int m_iCount;
    CDbuFieldDisp **m_pFieldsDisp;
public:
    CDbuFieldsDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex, CDbuFields *pFields);
    ~CDbuFieldsDisp();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetNextGeomName(int *piStart, BSTR *pbsNextGeomName);
    virtual HRESULT __stdcall GetCount(int *piCount);
    virtual HRESULT __stdcall GetItem(int idx, CDbuFieldDisp **ppItem);
    virtual HRESULT __stdcall FindByName(BSTR bsName, CDbuFieldDisp **ppItem);
};

class CDbuTableDisp : public CCOMDispatch
{
private:
    CDbuTable *m_pTable;
    CDbuFieldsDisp *m_pFields;
public:
    CDbuTableDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex, CDbuTable *pTable);
    ~CDbuTableDisp();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetName(BSTR *pbsName);
    virtual HRESULT __stdcall GetVisible(bool *pbVisible);
    virtual HRESULT __stdcall SetVisible(bool bVisible);
    virtual HRESULT __stdcall SaveTableMetadata();
    virtual HRESULT __stdcall GetPrimaryGeometry(BSTR *pbsGeomName);
    virtual HRESULT __stdcall SetPrimaryGeometry(BSTR bsGeomName);
    virtual HRESULT __stdcall GetDescription(BSTR *pbsDesc);
    virtual HRESULT __stdcall SetDescription(BSTR bsDesc);
    virtual HRESULT __stdcall GetFields(CDbuFieldsDisp **ppFields);
    virtual HRESULT __stdcall GetIsView(bool *pbIsView);
    virtual HRESULT __stdcall SetChanged(bool bChanged);
    virtual HRESULT __stdcall ChangeSpecField(CDbuFieldDisp *pField, bool *pbChanged);
};

class CDbuTablesDisp : public CCOMDispatch
{
private:
    CDbuTables *m_pTables;
    CDbuTableDisp **m_pTablesDisp;
    int m_iCount;
public:
    CDbuTablesDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex, CDbuTables *pTables);
    ~CDbuTablesDisp();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall GetCount(int *piCount);
    virtual HRESULT __stdcall GetItem(int idx, CDbuTableDisp **ppTable);
};

class CDbuConnectionDisp : public CCOMDispatch
{
private:
    long m_lHandle;
    CDbuTablesDisp *m_pTables;
    PConnList m_pConnList;
    HINSTANCE m_hInstance;
public:
    CDbuConnectionDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex,
        HINSTANCE hInstance, PConnList pConnList);
    ~CDbuConnectionDisp();
    virtual HRESULT __stdcall QueryInterface(REFIID iid, void **ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();
    virtual HRESULT __stdcall OpenConnection(BSTR bsServer, BSTR bsDatabase,
        BSTR bsUser, BSTR bsPasswd, VARIANT iPort);
    virtual HRESULT __stdcall CloseConnection();
    virtual HRESULT __stdcall GetConnectionStatus(CConnStatus *pConnStatus);
    virtual HRESULT __stdcall CreateMetadata(bool bFull);
    virtual HRESULT __stdcall DropMetadata();
    virtual HRESULT __stdcall GetTables(CDbuTablesDisp **ppTables);
    virtual HRESULT __stdcall UpdateAllTables(bool bVisible);
    virtual HRESULT __stdcall FillCtrlWithUserNames(HWND hwnd, UINT iMsg);
    virtual HRESULT __stdcall FillCtrlWithSrids(HWND hwnd, UINT iMsg);
    virtual HRESULT __stdcall GrantGdoPrivileges(int iMode, BSTR bsUser);
    virtual HRESULT __stdcall RunScript(BSTR bsFileName);
    virtual HRESULT __stdcall ClearModificationLog();
};

#endif
