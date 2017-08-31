#include "PsgDBUtilsCls.hpp"
#include "../Common/LogFiles.hpp"
#include "../Common/VarUtils.hpp"


// CDbuFieldDisp

CDbuFieldDisp::CDbuFieldDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex,
    CDbuField *pField) : CCOMDispatch(pUnkOuter, pLib, iIndex)
{
    m_pField = pField;

    if(m_pTypeInfo) m_pTypeInfo->Release();
    m_pTypeInfo = NULL;
    pLib->GetTypeInfoOfGuid(DIID_DbuField, &m_pTypeInfo);
}

CDbuFieldDisp::~CDbuFieldDisp()
{
}

HRESULT CDbuFieldDisp::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuFieldDisp::QueryInterface\r\n", true);
#endif // DBGLEVEL
    if(!ppvObject) return(E_INVALIDARG);
    //if(*ppvObject) ((IUnknown*)*ppvObject)->Release();
    *ppvObject = NULL;

    if(IsEqualIID(iid, IID_IUnknown))
    {
        *ppvObject = this;
    }
    else if(IsEqualIID(iid, DIID_DbuField) || IsEqualIID(iid, IID_IDispatch))
        *ppvObject = (void*)this;

    if(!*ppvObject)
    {
#if DBGLEVEL > 1
        char buf[128];
        FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CDbuFieldDisp::AddRef()
{
    ULONG res = m_pUnkOuter->AddRef();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuFieldDisp::AddRef (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

ULONG CDbuFieldDisp::Release()
{
    ULONG res = m_pUnkOuter->Release();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuFieldDisp::Release (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

HRESULT CDbuFieldDisp::GetName(BSTR *pbsName)
{
    *pbsName = SysAllocString(m_pField->GetNamePtr());
    return S_OK;
}

HRESULT CDbuFieldDisp::GetVisible(bool *pbVisible)
{
    *pbVisible = m_pField->GetVisible();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetVisible(bool bVisible)
{
    m_pField->SetVisible(bVisible);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetType(int *piType)
{
    *piType = m_pField->GetType();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetType(int iType)
{
    m_pField->SetType(iType);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetSubType(long *plSubType)
{
    *plSubType = m_pField->GetSubType();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetSubType(long lSubType)
{
    m_pField->SetSubType(lSubType);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetSrid(unsigned long *pulSrid)
{
    *pulSrid = m_pField->GetSrid();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetSrid(unsigned long ulSrid)
{
    m_pField->SetSrid(ulSrid);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetIsKey(bool *pbIsKey)
{
    *pbIsKey = m_pField->GetIsKey();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetIsKey(bool bIsKey)
{
    m_pField->SetIsKey(bIsKey);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetDescription(BSTR *pbsDesc)
{
    *pbsDesc = m_pField->GetDescription();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetDescription(BSTR bsDesc)
{
    m_pField->SetDescription(bsDesc);
    return S_OK;
}

HRESULT CDbuFieldDisp::GetHasSpecField(bool *pbHasSpecFld)
{
    *pbHasSpecFld = m_pField->GetHasSpecField();
    return S_OK;
}

HRESULT CDbuFieldDisp::GetIsHypertext(bool *pbIsHypertext)
{
    *pbIsHypertext = m_pField->GetIsHypertext();
    return S_OK;
}

HRESULT CDbuFieldDisp::SetIsHypertext(bool bIsHypertext)
{
    m_pField->SetIsHypertext(bIsHypertext);
    return S_OK;
}

CDbuField* CDbuFieldDisp::GetBaseField()
{
    return m_pField;
}


// CDbuFieldsDisp

CDbuFieldsDisp::CDbuFieldsDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex,
    CDbuFields *pFields) : CCOMDispatch(pUnkOuter, pLib, iIndex)
{
    m_pFields = pFields;
    m_iCount = pFields->GetCount();
    m_pFieldsDisp = (CDbuFieldDisp**)malloc(m_iCount*sizeof(CDbuFieldDisp*));
    for(int i = 0; i < m_iCount; i++)
    {
        m_pFieldsDisp[i] = NULL;
    }

    if(m_pTypeInfo) m_pTypeInfo->Release();
    m_pTypeInfo = NULL;
    pLib->GetTypeInfoOfGuid(DIID_DbuFields, &m_pTypeInfo);
}

CDbuFieldsDisp::~CDbuFieldsDisp()
{
    free(m_pFieldsDisp);
}

HRESULT CDbuFieldsDisp::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuFieldsDisp::QueryInterface\r\n", true);
#endif // DBGLEVEL
    if(!ppvObject) return(E_INVALIDARG);
    //if(*ppvObject) ((IUnknown*)*ppvObject)->Release();
    *ppvObject = NULL;

    if(IsEqualIID(iid, IID_IUnknown))
    {
        *ppvObject = this;
    }
    else if(IsEqualIID(iid, DIID_DbuFields) || IsEqualIID(iid, IID_IDispatch))
        *ppvObject = (void*)this;

    if(!*ppvObject)
    {
#if DBGLEVEL > 1
        char buf[128];
        FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CDbuFieldsDisp::AddRef()
{
    ULONG res = m_pUnkOuter->AddRef();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuFieldsDisp::AddRef (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

ULONG CDbuFieldsDisp::Release()
{
    ULONG res = m_pUnkOuter->Release();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuFieldsDisp::Release (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

HRESULT CDbuFieldsDisp::GetNextGeomName(int *piStart, BSTR *pbsNextGeomName)
{
    *pbsNextGeomName = SysAllocString(m_pFields->GetNextGeomName(piStart));
    return S_OK;
}

HRESULT CDbuFieldsDisp::GetCount(int *piCount)
{
    *piCount = m_iCount;
    return S_OK;
}

HRESULT CDbuFieldsDisp::GetItem(int idx, CDbuFieldDisp **ppItem)
{
    if((idx < 0) || (idx >= m_iCount)) return S_FALSE;

    if(!m_pFieldsDisp[idx])
    {
        CDbuField *pFld = (CDbuField*)m_pFields->GetItem(idx);
        if(!pFld) return S_FALSE;

        m_pFieldsDisp[idx] = new CDbuFieldDisp(this, m_pTypeLib, 0, pFld);
    }

    *ppItem = m_pFieldsDisp[idx];
    if(*ppItem) (*ppItem)->AddRef();
    return S_OK;
}

HRESULT CDbuFieldsDisp::FindByName(BSTR bsName, CDbuFieldDisp **ppItem)
{
    int idx = m_pFields->FindPosByName(bsName);
    if(idx < 0) return S_FALSE;

    if(!m_pFieldsDisp[idx])
    {
        CDbuField *pFld = (CDbuField*)m_pFields->GetItem(idx);
        if(!pFld) return S_FALSE;

        m_pFieldsDisp[idx] = new CDbuFieldDisp(this, m_pTypeLib, 0, pFld);
    }

    *ppItem = m_pFieldsDisp[idx];
    if(*ppItem) (*ppItem)->AddRef();
    return S_OK;
}


// CDbuTableDisp

CDbuTableDisp::CDbuTableDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex,
    CDbuTable *pTable) : CCOMDispatch(pUnkOuter, pLib, iIndex)
{
    m_pTable = pTable;
    m_pFields = NULL;

    if(m_pTypeInfo) m_pTypeInfo->Release();
    m_pTypeInfo = NULL;
    pLib->GetTypeInfoOfGuid(DIID_DbuTable, &m_pTypeInfo);
}

CDbuTableDisp::~CDbuTableDisp()
{
    if(m_pFields) delete m_pFields;
}

HRESULT CDbuTableDisp::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuTableDisp::QueryInterface\r\n", true);
#endif // DBGLEVEL
    if(!ppvObject) return(E_INVALIDARG);
    //if(*ppvObject) ((IUnknown*)*ppvObject)->Release();
    *ppvObject = NULL;

    if(IsEqualIID(iid, IID_IUnknown))
    {
        *ppvObject = this;
    }
    else if(IsEqualIID(iid, DIID_DbuTable) || IsEqualIID(iid, IID_IDispatch))
        *ppvObject = (void*)this;

    if(!*ppvObject)
    {
#if DBGLEVEL > 1
        char buf[128];
        FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CDbuTableDisp::AddRef()
{
    ULONG res = m_pUnkOuter->AddRef();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuTableDisp::AddRef (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

ULONG CDbuTableDisp::Release()
{
    ULONG res = m_pUnkOuter->Release();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuTableDisp::Release (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

HRESULT CDbuTableDisp::GetName(BSTR *pbsName)
{
    *pbsName = SysAllocString(m_pTable->GetNamePtr());
    return S_OK;
}

HRESULT CDbuTableDisp::GetVisible(bool *pbVisible)
{
    *pbVisible = m_pTable->GetVisible();
    return S_OK;
}

HRESULT CDbuTableDisp::SetVisible(bool bVisible)
{
    m_pTable->SetVisible(bVisible);
    return S_OK;
}

HRESULT CDbuTableDisp::SaveTableMetadata()
{
    m_pTable->SaveTableMetadata();
    return S_OK;
}

HRESULT CDbuTableDisp::GetPrimaryGeometry(BSTR *pbsGeomName)
{
    *pbsGeomName = SysAllocString(m_pTable->GetPrimaryGeometry());
    return S_OK;
}

HRESULT CDbuTableDisp::SetPrimaryGeometry(BSTR bsGeomName)
{
    m_pTable->SetPrimaryGeometry(bsGeomName);
    return S_OK;
}

HRESULT CDbuTableDisp::GetDescription(BSTR *pbsDesc)
{
    *pbsDesc = SysAllocString(m_pTable->GetDescription());
    return S_OK;
}

HRESULT CDbuTableDisp::SetDescription(BSTR bsDesc)
{
    m_pTable->SetDescription(bsDesc);
    return S_OK;
}

HRESULT CDbuTableDisp::GetFields(CDbuFieldsDisp **ppFields)
{
    if(!m_pFields)
    {
        CDbuFields *pFlds = (CDbuFields*)m_pTable->GetFields();
        if(!pFlds) return S_FALSE;

        m_pFields = new CDbuFieldsDisp(m_pFields, m_pTypeLib, 1, pFlds);
    }

    m_pFields->AddRef();
    *ppFields = m_pFields;
    return S_OK;
}

HRESULT CDbuTableDisp::GetIsView(bool *pbIsView)
{
    *pbIsView = m_pTable->GetIsView();
    return S_OK;
}

HRESULT CDbuTableDisp::SetChanged(bool bChanged)
{
    m_pTable->SetChanged(bChanged);
    return S_OK;
}

HRESULT CDbuTableDisp::ChangeSpecField(CDbuFieldDisp *pField, bool *pbChanged)
{
    *pbChanged = m_pTable->ChangeSpecField(pField->GetBaseField());
    return S_OK;
}


// CDbuTablesDisp

CDbuTablesDisp::CDbuTablesDisp(IUnknown *pUnkOuter, ITypeLib *pLib, int iIndex,
    CDbuTables *pTables) : CCOMDispatch(pUnkOuter, pLib, iIndex)
{
    m_pTables = pTables;
    m_iCount = pTables->GetCount();
    m_pTablesDisp = (CDbuTableDisp**)malloc(m_iCount*sizeof(CDbuTableDisp*));
    for(int i = 0; i < m_iCount; i++)
    {
        m_pTablesDisp[i] = NULL;
    }

    if(m_pTypeInfo) m_pTypeInfo->Release();
    m_pTypeInfo = NULL;
    pLib->GetTypeInfoOfGuid(DIID_DbuTables, &m_pTypeInfo);
}

CDbuTablesDisp::~CDbuTablesDisp()
{
    free(m_pTablesDisp);
}

HRESULT CDbuTablesDisp::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuTablesDisp::QueryInterface\r\n", true);
#endif // DBGLEVEL
    if(!ppvObject) return(E_INVALIDARG);
    //if(*ppvObject) ((IUnknown*)*ppvObject)->Release();
    *ppvObject = NULL;

    if(IsEqualIID(iid, IID_IUnknown))
    {
        *ppvObject = this;
    }
    else if(IsEqualIID(iid, DIID_DbuTables) || IsEqualIID(iid, IID_IDispatch))
        *ppvObject = (void*)this;

    if(!*ppvObject)
    {
#if DBGLEVEL > 1
        char buf[128];
        FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CDbuTablesDisp::AddRef()
{
    ULONG res = m_pUnkOuter->AddRef();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuTablesDisp::AddRef (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

ULONG CDbuTablesDisp::Release()
{
    ULONG res = m_pUnkOuter->Release();
#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuTablesDisp::Release (Aggregated) - %d\r\n", res);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(res);
}

HRESULT CDbuTablesDisp::GetCount(int *piCount)
{
    *piCount = m_iCount;
    return S_OK;
}

HRESULT CDbuTablesDisp::GetItem(int idx, CDbuTableDisp **ppTable)
{
    if((idx < 0) || (idx >= m_iCount)) return S_FALSE;

    if(!m_pTablesDisp[idx])
    {
        CDbuTable *pTable = (CDbuTable*)m_pTables->GetItem(idx);
        if(!pTable) return S_FALSE;

        m_pTablesDisp[idx] = new CDbuTableDisp(this, m_pTypeLib, 2, pTable);
    }

    *ppTable = m_pTablesDisp[idx];
    if(*ppTable) (*ppTable)->AddRef();
    return S_OK;
}


// CDbuConnectionDisp

CDbuConnectionDisp::CDbuConnectionDisp(IUnknown *pUnkOuter, ITypeLib *pLib,
    int iIndex, HINSTANCE hInstance, PConnList pConnList) :
    CCOMDispatch(pUnkOuter, pLib, iIndex)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuConnectionDisp::CDbuConnectionDisp\r\n", true);
#endif // DBGLEVEL
    m_lHandle = 0;
    m_pTables = NULL;
    m_hInstance = hInstance;
    m_pConnList = pConnList;

    if(m_pTypeInfo) m_pTypeInfo->Release();
    m_pTypeInfo = NULL;
    pLib->GetTypeInfoOfGuid(DIID_DbuConnection, &m_pTypeInfo);
}

CDbuConnectionDisp::~CDbuConnectionDisp()
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuConnectionDisp::~CDbuConnectionDisp\r\n", true);
#endif // DBGLEVEL
    if(m_pTables) delete m_pTables;
    if(m_lHandle) CloseConnection();
}

HRESULT CDbuConnectionDisp::QueryInterface(REFIID iid, void **ppvObject)
{
#if DBGLEVEL > 2
    WriteLogFile("CDbuConnectionDisp::QueryInterface\r\n", true);
#endif // DBGLEVEL
    if(!ppvObject) return(E_INVALIDARG);
    //if(*ppvObject) ((IUnknown*)*ppvObject)->Release();
    *ppvObject = NULL;

    if(IsEqualIID(iid, IID_IUnknown))
    {
        *ppvObject = this;
    }
    else if(IsEqualIID(iid, DIID_DbuConnection) || IsEqualIID(iid, IID_IDispatch))
        *ppvObject = (void*)this;

    if(!*ppvObject)
    {
#if DBGLEVEL > 1
        char buf[128];
        FormatGuid(buf, "Unknown Interface: ", "\r\n", iid);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(E_NOINTERFACE);
    }

    ((IUnknown*)*ppvObject)->AddRef();
    return(S_OK);
}

ULONG CDbuConnectionDisp::AddRef()
{
    if(m_pUnkOuter)
    {
        ULONG res = m_pUnkOuter->AddRef();
#if DBGLEVEL > 2
        char buf[64];
        sprintf(buf, "CDbuConnectionDisp::AddRef (Aggregated) - %d\r\n", res);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        return(res);
    }

#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuConnectionDisp::AddRef - %d\r\n", m_lRefCount);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    return(++m_lRefCount);
}

ULONG CDbuConnectionDisp::Release()
{
    if(m_pUnkOuter)
    {
        ULONG res = m_pUnkOuter->Release();
#if DBGLEVEL > 2
        char buf[64];
        sprintf(buf, "CDbuConnectionDisp::Release (Aggregated) - %d\r\n", res);
        WriteLogFile(buf, true);
#endif // DBGLEVEL
        if(res < 1) delete(this);
        return(res);
    }

#if DBGLEVEL > 2
    char buf[64];
    sprintf(buf, "CDbuConnectionDisp::Release - %d\r\n", m_lRefCount);
    WriteLogFile(buf, true);
#endif // DBGLEVEL
    if(--m_lRefCount > 0) return(m_lRefCount);
    delete(this);
    return(0);
}

HRESULT CDbuConnectionDisp::OpenConnection(BSTR bsServer, BSTR bsDatabase,
    BSTR bsUser, BSTR bsPasswd, VARIANT iPort)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::OpenConnection\r\n", true);
#endif // DBGLEVEL
    if(m_lHandle) CloseConnection();

    int iLocPort = 0;
    if((iPort.vt != VT_ERROR) && (iPort.scode != DISP_E_PARAMNOTFOUND))
    {
        iLocPort = VarToInt(iPort);
    }
    m_lHandle = m_pConnList->AddConnection(m_hInstance, bsServer, bsDatabase, bsUser,
        bsPasswd, iLocPort);

    if(m_lHandle) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::CloseConnection()
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::CloseConnection\r\n", true);
#endif // DBGLEVEL

    if(!m_lHandle) return S_OK;

    if(m_pConnList->CloseConnection(m_lHandle))
    {
        m_lHandle = 0;
        return S_OK;
    }
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::GetConnectionStatus(CConnStatus *pConnStatus)
{
#if DBGLEVEL > 0
    WriteLogFile("DbuGetConnectionStatus\r\n", true);
#endif // DBGLEVEL
    *pConnStatus = m_pConnList->GetConnStatus(m_lHandle);
    return S_OK;
}

HRESULT CDbuConnectionDisp::CreateMetadata(bool bFull)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::CreateMetadata\r\n", true);
#endif // DBGLEVEL
    bool bOK = false;
    if(bFull) bOK = m_pConnList->CreateINGRMetadata(m_lHandle);
    else bOK = m_pConnList->CreateSimpleMetadata(m_lHandle);

    if(bOK) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::DropMetadata()
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::DropMetadata\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->DropMetadata(m_lHandle)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::GetTables(CDbuTablesDisp **ppTables)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::GetTables\r\n", true);
#endif // DBGLEVEL
    if(!m_pTables)
    {
        CDbuTables *pTbls = (CDbuTables*)m_pConnList->GetTables(m_lHandle);
        if(!pTbls) return S_FALSE;

        m_pTables = new CDbuTablesDisp(this, m_pTypeLib, 3, pTbls);
    }
    m_pTables->AddRef();
    *ppTables = m_pTables;
    return S_OK;
}

HRESULT CDbuConnectionDisp::UpdateAllTables(bool bVisible)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::UpdateAllTables\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->UpdateAllTables(m_lHandle, bVisible)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::FillCtrlWithUserNames(HWND hwnd, UINT iMsg)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::FillCtrlWithUserNames\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->FillCtrlWithUserNames(m_lHandle, hwnd, iMsg)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::FillCtrlWithSrids(HWND hwnd, UINT iMsg)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::FillCtrlWithSrids\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->FillCtrlWithSrids(m_lHandle, hwnd, iMsg)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::GrantGdoPrivileges(int iMode, BSTR bsUser)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::GrantGdoPrivileges\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->GrantGdoPrivileges(m_lHandle, iMode, bsUser)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::RunScript(BSTR bsFileName)
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::RunScript\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->RunScript(m_lHandle, bsFileName)) return S_OK;
    return S_FALSE;
}

HRESULT CDbuConnectionDisp::ClearModificationLog()
{
#if DBGLEVEL > 0
    WriteLogFile("CDbuConnectionDisp::ClearModificationLog\r\n", true);
#endif // DBGLEVEL
    if(m_pConnList->ClearModificationLog(m_lHandle)) return S_OK;
    return S_FALSE;
}
