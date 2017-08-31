//Header file generated by Delphi-TypeLib compiler 2.1
//Created by Pavel Krejcir, INTERGRAPH, 2002-2007

#ifndef _PAFM_TLB_H
#define _PAFM_TLB_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <basetyps.h>
#include <wtypes.h>

#include <ocidl.h>
//#include "GDO_TLB.hpp"

//
// GUID constant declarations
//

const IID DIID_EqualityExtension = {0x31A3EE61,0x4730,0x11D0,{0x8F,0x97,0x08,0x00,0x36,0xAD,0xCA,0x02}};
const IID DIID__GFeatureModel = {0x8E1761A2,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GFeatureClasses = {0x8E176181,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GFeatureClass = {0x8E176178,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAssociationRoleClasses = {0x76B6A650,0x8C3F,0x11D3,{0x84,0x8B,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAssociationRoleClass = {0xDF466828,0x41D3,0x11D3,{0x82,0x13,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID DIID__GOperationClasses = {0x8E1761A5,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GOperationClass = {0xDF466829,0x41D3,0x11D3,{0x82,0x13,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID DIID__GOperation = {0x8E1761A7,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GRuleClasses = {0x7BD1441D,0x9635,0x42BE,{0x86,0x9A,0x34,0xE1,0x82,0xCE,0xF8,0xCD}};
const IID DIID__GRuleClass = {0x236A7778,0xDB7F,0x4348,{0x9F,0x41,0xE3,0x4D,0x0B,0x6A,0x17,0x7E}};
const IID DIID__GRule = {0x80BC1C07,0x567F,0x4FCB,{0xBD,0x02,0x82,0xBF,0xAD,0x49,0xB0,0x43}};
const IID DIID__GAssociationRole = {0x8E1761A3,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GRules = {0x1F4B8EBD,0x19B2,0x4B22,{0x89,0x5D,0x09,0x1F,0x1A,0x9D,0xEF,0xAB}};
const IID DIID__GOperations = {0x267CD4EB,0xCC3C,0x11D4,{0x84,0x9A,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GFeature = {0x8E176180,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAttributes = {0x267CD4EA,0xCC3C,0x11D4,{0x84,0x9A,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAttribute = {0x5A68AC8C,0x74BF,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAttributeClass = {0xDF466826,0x41D3,0x11D3,{0x82,0x13,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID DIID__GPickList = {0x097133F7,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GAssociationRoles = {0x476D6B70,0xCC45,0x11D4,{0x84,0x9A,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID DIID__GAttributeClasses = {0x8E176179,0x74D8,0x11D3,{0x84,0x80,0x00,0x50,0x04,0x0C,0xD1,0x25}};
const IID IID__GFeatureClassEvents = {0x72D777D7,0x35AB,0x4095,{0x81,0xBB,0x88,0x26,0x60,0x4B,0x21,0x4D}};
const IID DIID__GSelection = {0x76A8F47D,0xE8D1,0x482C,{0x97,0xA1,0x40,0x50,0x72,0xF6,0x09,0x00}};
const IID IID__GAttributeClassEvents = {0x7E7E2278,0x320D,0x43D6,{0xA8,0x61,0x73,0x70,0x73,0xBB,0xC7,0xC8}};
const IID IID__GAssociationRoleClassEvents = {0x097133F4,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GInheritance = {0x114F668C,0x6906,0x47C0,{0x8A,0x82,0x96,0x13,0xD1,0x42,0x26,0xA5}};
const IID DIID__GInheritances = {0xDCCEA924,0x53DF,0x4D11,{0x80,0x16,0x79,0xAD,0xE8,0xF0,0x69,0xF1}};
const IID DIID__GEventHandler = {0x097133F8,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GExtensions = {0x9E693A83,0x691A,0x4884,{0xA9,0x6D,0x7D,0x28,0xC1,0xB2,0x52,0x46}};
const CLSID CLASS_GFeatureModel = {0x09713401,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GFeatureClass = {0x09713402,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GFeature = {0x09713404,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GSelection = {0x09713405,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAttributeClass = {0x09713406,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAttribute = {0x09713407,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAssociationRoleClass = {0x09713408,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAssociationRole = {0x09713409,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GOperationClass = {0x0971340A,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GOperation = {0x0971340B,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GRuleClass = {0x0971340C,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GRule = {0x0971340D,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GPickList = {0x0971340E,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GInheritance = {0x09713410,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GEventHandler = {0x09713411,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GFeatureClasses = {0x09713412,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAttributeClasses = {0x09713414,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAttributes = {0x09713415,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAssociationRoleClasses = {0x09713416,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GAssociationRoles = {0x09713417,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GOperationClasses = {0x09713418,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GOperations = {0x09713419,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GRuleClasses = {0x0971341A,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GRules = {0x0971341B,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GExtensions = {0x0971341C,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GInheritances = {0x0971341D,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GPersistMgr = {0x09713393,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GPersist = {0x097133B6,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GPersistData = {0x097133B7,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID_DGMClassObjects = {0x097133C8,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GPersistObjectType = {0x097133C9,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID__GPersistMgrEvents = {0x09713436,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const IID DIID_DGMDefinitionControl = {0x097133C7,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GPersistMgr = {0xD17553B4,0xF083,0x11D4,{0xB6,0x6D,0x00,0x50,0x04,0xAA,0x94,0x48}};
const CLSID CLASS_GPersistData = {0x0971341F,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GPersist = {0x09713420,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GDefinitionControl = {0x09713421,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GClassObjects = {0x09713422,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};
const CLSID CLASS_GPersistObjectType = {0x09713423,0x9D72,0x11D5,{0x92,0x83,0x00,0x00,0xE2,0x2B,0x9E,0xF0}};


// Constants for enum AssociationRoleReturnTypeConstants
#ifndef _AssociationRoleReturnTypeConstants_
#define _AssociationRoleReturnTypeConstants_
typedef enum _AssociationRoleReturnTypeConstants {
    gmarrtGRecordset = 0x00000000,
    gmarrtFeatures = 0x00000001,
    gmarrtKeys = 0x00000002
} AssociationRoleReturnTypeConstants;
#endif

// Constants for enum FeatureStateConstants
#ifndef _FeatureStateConstants_
#define _FeatureStateConstants_
typedef enum _FeatureStateConstants {
    gmfsUninitialized = 0x00000000,
    gmfsInitialized = 0x00000001,
    gmfsEdited = 0x00000002,
    gmfsAddedNew = 0x00000003
} FeatureStateConstants;
#endif

// Constants for enum TraversalConstants
#ifndef _TraversalConstants_
#define _TraversalConstants_
typedef enum _TraversalConstants {
    gmtvDeep = 0x00000001,
    gmtvShallow = 0x00000002
} TraversalConstants;
#endif

// Constants for enum AssociationStatusConstants
#ifndef _AssociationStatusConstants_
#define _AssociationStatusConstants_
typedef enum _AssociationStatusConstants {
    gmasInvalidAssociation = 0x00000000,
    gmasAlreadyAssociated = 0x00000001,
    gmasCanAssociate = 0x00000002
} AssociationStatusConstants;
#endif

// Constants for enum DataTypeConstants
#ifndef _DataTypeConstants_
#define _DataTypeConstants_
typedef enum _DataTypeConstants {
    gmdtBoolean = 0x00000001,
    gmdtByte = 0x00000002,
    gmdtInteger = 0x00000003,
    gmdtLong = 0x00000004,
    gmdtCurrency = 0x00000005,
    gmdtSingle = 0x00000006,
    gmdtDouble = 0x00000007,
    gmdtDate = 0x00000008,
    gmdtObject = 0x00000009,
    gmdtText = 0x0000000A,
    gmdtLongBinary = 0x0000000B,
    gmdtMemo = 0x0000000C,
    gmdtVariant = 0x0000000D,
    gmdtGuid = 0x0000000F,
    gmdtPoint = 0x00000010,
    gmdtLinear = 0x00000011,
    gmdtAreal = 0x00000012,
    gmdtAnySpatial = 0x00000013,
    gmdtCoverage = 0x00000014,
    gmdtGraphicsText = 0x00000015,
    gmdtArray = 0x00002000,
    gmdtParentObject = 0x00000809
} DataTypeConstants;
#endif

// Constants for enum ClassTypeConstants
#ifndef _ClassTypeConstants_
#define _ClassTypeConstants_
typedef enum _ClassTypeConstants {
    gmctInherited = 0x00000001
} ClassTypeConstants;
#endif
// *********************************************************************//
// Forward declaration of types defined in TypeLibrary
// *********************************************************************//

struct EqualityExtension;
struct _GFeatureModel;
struct _GFeatureClasses;
struct _GFeatureClass;
struct _GAssociationRoleClasses;
struct _GAssociationRoleClass;
struct _GOperationClasses;
struct _GOperationClass;
struct _GOperation;
struct _GRuleClasses;
struct _GRuleClass;
struct _GRule;
struct _GAssociationRole;
struct _GRules;
struct _GOperations;
struct _GFeature;
struct _GAttributes;
struct _GAttribute;
struct _GAttributeClass;
struct _GPickList;
struct _GAssociationRoles;
struct _GAttributeClasses;
struct _GFeatureClassEvents;
struct _GSelection;
struct _GAttributeClassEvents;
struct _GAssociationRoleClassEvents;
struct _GInheritance;
struct _GInheritances;
struct _GEventHandler;
struct _GExtensions;
struct _GPersistMgr;
struct _GPersist;
struct _GPersistData;
struct DGMClassObjects;
struct _GPersistObjectType;
struct _GPersistMgrEvents;
struct DGMDefinitionControl;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library
// *********************************************************************//

typedef _GFeatureModel GFeatureModel;
typedef _GFeatureClass GFeatureClass;
typedef _GFeature GFeature;
typedef _GSelection GSelection;
typedef _GAttributeClass GAttributeClass;
typedef _GAttribute GAttribute;
typedef _GAssociationRoleClass GAssociationRoleClass;
typedef _GAssociationRole GAssociationRole;
typedef _GOperationClass GOperationClass;
typedef _GOperation GOperation;
typedef _GRuleClass GRuleClass;
typedef _GRule GRule;
typedef _GPickList GPickList;
typedef _GInheritance GInheritance;
typedef _GEventHandler GEventHandler;
typedef _GFeatureClasses GFeatureClasses;
typedef _GAttributeClasses GAttributeClasses;
typedef _GAttributes GAttributes;
typedef _GAssociationRoleClasses GAssociationRoleClasses;
typedef _GAssociationRoles GAssociationRoles;
typedef _GOperationClasses GOperationClasses;
typedef _GOperations GOperations;
typedef _GRuleClasses GRuleClasses;
typedef _GRules GRules;
typedef _GExtensions GExtensions;
typedef _GInheritances GInheritances;
typedef _GPersistMgr GPersistMgr;
typedef _GPersistData GPersistData;
typedef _GPersist GPersist;
typedef DGMDefinitionControl GDefinitionControl;
typedef DGMClassObjects GClassObjects;
typedef _GPersistObjectType GPersistObjectType;

struct EqualityExtension : IDispatch
{
    virtual HRESULT __stdcall GetComparisonKey(BSTR *ComparisonKey) = 0;
    virtual HRESULT __stdcall GetBookmarkFromComparisonKey(BSTR ComparisonKey,
      SAFEARRAY * *Bm) = 0;
};

struct EqualityExtensionDisp : IDispatch
{
    LPTSTR GetComparisonKey();
    SAFEARRAY * GetBookmarkFromComparisonKey(LPTSTR ComparisonKey);
};

struct _GFeatureModel : IDispatch
{
    virtual HRESULT __stdcall get_FeatureClasses(_GFeatureClasses* *objFeatureClasses) = 0;
    virtual HRESULT __stdcall get_Database(GDatabase* *objDataConnections) = 0;
};

struct _GFeatureModelDisp : IDispatch
{
    _GFeatureClasses* get_FeatureClasses();
    GDatabase* get_Database();
};

struct _GFeatureClasses : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GFeatureClass* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GFeatureClassesDisp : IDispatch
{
    long get_Count();
    _GFeatureClass* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GFeatureClass : IDispatch
{
    virtual HRESULT __stdcall get_AssociationRoleClasses(_GAssociationRoleClasses* *objClasses) = 0;
    virtual HRESULT __stdcall get_AttributeClasses(_GAttributeClasses* *objClasses) = 0;
    virtual HRESULT __stdcall get_KeyAttributeNames(VARIANT *pvarKeyAttributeNames) = 0;
    virtual HRESULT __stdcall get_Name(BSTR *strVal) = 0;
    virtual HRESULT __stdcall get_OperationClasses(_GOperationClasses* *objClasses) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *objParent) = 0;
    virtual HRESULT __stdcall get_RuleClasses(_GRuleClasses* *objClasses) = 0;
    virtual HRESULT __stdcall GetFeature(GRecordset* objSelection, _GFeature* *objIFeature) = 0;
    virtual HRESULT __stdcall OpenSelection(BSTR bstrFilter, VARIANT vType, VARIANT vOptions,
      VARIANT vSpatialOperator, VARIANT vSpatialGeometryFilter, VARIANT vGeometryFieldName,
      GRecordset* *ppSelection) = 0;
};

struct _GFeatureClassDisp : IDispatch
{
    _GAssociationRoleClasses* get_AssociationRoleClasses();
    _GAttributeClasses* get_AttributeClasses();
    VARIANT get_KeyAttributeNames();
    LPTSTR get_Name();
    _GOperationClasses* get_OperationClasses();
    IDispatch * get_Parent();
    _GRuleClasses* get_RuleClasses();
    _GFeature* GetFeature(GRecordset* objSelection);
    GRecordset* OpenSelection(LPTSTR bstrFilter, VARIANT vType, VARIANT vOptions,
      VARIANT vSpatialOperator, VARIANT vSpatialGeometryFilter, VARIANT vGeometryFieldName);
};

struct _GAssociationRoleClasses : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT vIndex, _GAssociationRoleClass* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GAssociationRoleClassesDisp : IDispatch
{
    long get_Count();
    _GAssociationRoleClass* get_Item(VARIANT vIndex);
    IUnknown * _NewEnum();
};

struct _GAssociationRoleClass : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *strVal) = 0;
    virtual HRESULT __stdcall get_Type(BSTR *strType) = 0;
    virtual HRESULT __stdcall get_OperationClasses(_GOperationClasses* *ppOperationClasses) = 0;
    virtual HRESULT __stdcall get_RuleClasses(_GRuleClasses* *ppRuleClasses) = 0;
    virtual HRESULT __stdcall get_AssociatedFeatureClass(_GFeatureClass* *ppFeatureClass) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall GetAssociationRole(IDispatch * objDataBound, _GAssociationRole* *objAssociationRole) = 0;
};

struct _GAssociationRoleClassDisp : IDispatch
{
    LPTSTR get_Name();
    LPTSTR get_Type();
    _GOperationClasses* get_OperationClasses();
    _GRuleClasses* get_RuleClasses();
    _GFeatureClass* get_AssociatedFeatureClass();
    IDispatch * get_Parent();
    _GAssociationRole* GetAssociationRole(IDispatch * objDataBound);
};

struct _GOperationClasses : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GOperationClass* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GOperationClassesDisp : IDispatch
{
    long get_Count();
    _GOperationClass* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GOperationClass : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *strName) = 0;
    virtual HRESULT __stdcall get_Type(BSTR *bstrType) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall GetOperation(IDispatch * objDataBound, _GOperation* *objOperation) = 0;
};

struct _GOperationClassDisp : IDispatch
{
    LPTSTR get_Name();
    LPTSTR get_Type();
    IDispatch * get_Parent();
    _GOperation* GetOperation(IDispatch * objDataBound);
};

struct _GOperation : IDispatch
{
    virtual HRESULT __stdcall get_OperationClass(_GOperationClass* *objOperationClass) = 0;
    virtual HRESULT __stdcall get_Advisor(IDispatch * *objAdvisor) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall Invoke1() = 0;
};

struct _GOperationDisp : IDispatch
{
    _GOperationClass* get_OperationClass();
    IDispatch * get_Advisor();
    IDispatch * get_Parent();
    void Invoke1();
};

struct _GRuleClasses : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT vIndex, _GRuleClass* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GRuleClassesDisp : IDispatch
{
    long get_Count();
    _GRuleClass* get_Item(VARIANT vIndex);
    IUnknown * _NewEnum();
};

struct _GRuleClass : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *pbstrName) = 0;
    virtual HRESULT __stdcall get_Type(BSTR *bstrType) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall GetRule(IDispatch * objParentInstance, _GRule* *ppRule) = 0;
};

struct _GRuleClassDisp : IDispatch
{
    LPTSTR get_Name();
    LPTSTR get_Type();
    IDispatch * get_Parent();
    _GRule* GetRule(IDispatch * objParentInstance);
};

struct _GRule : IDispatch
{
    virtual HRESULT __stdcall get_RuleClass(_GRuleClass* *objRule) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall Validate(VARIANT *varReasons, VARIANT_BOOL *blnIsValid) = 0;
};

struct _GRuleDisp : IDispatch
{
    _GRuleClass* get_RuleClass();
    IDispatch * get_Parent();
    BOOL Validate(VARIANT *varReasons);
};

struct _GAssociationRole : IDispatch
{
    virtual HRESULT __stdcall get_AssociatedFeaturesCount(long *lCount) = 0;
    virtual HRESULT __stdcall get_AssociationRoleClass(_GAssociationRoleClass* *objAssociationRoleClass) = 0;
    virtual HRESULT __stdcall get_Rules(_GRules* *ppRules) = 0;
    virtual HRESULT __stdcall get_Operations(_GOperations* *ppOperations) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall GetAssociatedFeatures(long lngTypeAssociatedFeatures,
      VARIANT *pvAssociatedFeatures) = 0;
    virtual HRESULT __stdcall GetConnectivityFeatures(long lngReturnType, VARIANT *varConnectivityFeatures) = 0;
    virtual HRESULT __stdcall GetAssociationStatus(_GFeature* objFeature, long *eAssociationInfo) = 0;
    virtual HRESULT __stdcall Associate(_GFeature* objInstance) = 0;
    virtual HRESULT __stdcall Disassociate(_GFeature* objInstance) = 0;
    virtual HRESULT __stdcall Validate(long enumDeepOrShallow, VARIANT *varReasons,
      VARIANT_BOOL *blnValid) = 0;
};

struct _GAssociationRoleDisp : IDispatch
{
    long get_AssociatedFeaturesCount();
    _GAssociationRoleClass* get_AssociationRoleClass();
    _GRules* get_Rules();
    _GOperations* get_Operations();
    IDispatch * get_Parent();
    VARIANT GetAssociatedFeatures(long lngTypeAssociatedFeatures);
    VARIANT GetConnectivityFeatures(long lngReturnType);
    long GetAssociationStatus(_GFeature* objFeature);
    void Associate(_GFeature* objInstance);
    void Disassociate(_GFeature* objInstance);
    BOOL Validate(long enumDeepOrShallow, VARIANT *varReasons);
};

struct _GRules : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT vIndex, _GRule* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GRulesDisp : IDispatch
{
    long get_Count();
    _GRule* get_Item(VARIANT vIndex);
    IUnknown * _NewEnum();
};

struct _GOperations : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GOperation* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GOperationsDisp : IDispatch
{
    long get_Count();
    _GOperation* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GFeature : IDispatch
{
    virtual HRESULT __stdcall get_FeatureClass(_GFeatureClass* *objVal) = 0;
    virtual HRESULT __stdcall get_KeyAttributeValues(VARIANT *varVal) = 0;
    virtual HRESULT __stdcall set_KeyAttributeValues(VARIANT varVal) = 0;
    virtual HRESULT __stdcall get_Bookmark(VARIANT *varVal) = 0;
    virtual HRESULT __stdcall set_Bookmark(VARIANT varVal) = 0;
    virtual HRESULT __stdcall get_State(long *lngState) = 0;
    virtual HRESULT __stdcall get_Attributes(_GAttributes* *objAttributes) = 0;
    virtual HRESULT __stdcall get_Rules(_GRules* *objRules) = 0;
    virtual HRESULT __stdcall get_Operations(_GOperations* *objOperations) = 0;
    virtual HRESULT __stdcall get_AssociationRoles(_GAssociationRoles* *objRoles) = 0;
    virtual HRESULT __stdcall AddNew() = 0;
    virtual HRESULT __stdcall Edit() = 0;
    virtual HRESULT __stdcall Delete() = 0;
    virtual HRESULT __stdcall Update() = 0;
    virtual HRESULT __stdcall CancelUpdate() = 0;
    virtual HRESULT __stdcall Validate(long enumDeepOrShallow, VARIANT *varReasons,
      VARIANT_BOOL *bValid) = 0;
};

struct _GFeatureDisp : IDispatch
{
    _GFeatureClass* get_FeatureClass();
    VARIANT get_KeyAttributeValues();
    void set_KeyAttributeValues(VARIANT Value);
    VARIANT get_Bookmark();
    void set_Bookmark(VARIANT Value);
    long get_State();
    _GAttributes* get_Attributes();
    _GRules* get_Rules();
    _GOperations* get_Operations();
    _GAssociationRoles* get_AssociationRoles();
    void AddNew();
    void Edit();
    void Delete();
    void Update();
    void CancelUpdate();
    BOOL Validate(long enumDeepOrShallow, VARIANT *varReasons);
};

struct _GAttributes : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GAttribute* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GAttributesDisp : IDispatch
{
    long get_Count();
    _GAttribute* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GAttribute : IDispatch
{
    virtual HRESULT __stdcall get_Value(VARIANT *varValue) = 0;
    virtual HRESULT __stdcall set_Value(VARIANT varValue) = 0;
    virtual HRESULT __stdcall get_AttributeClass(_GAttributeClass* *objClass) = 0;
    virtual HRESULT __stdcall get_Operations(_GOperations* *objOperations) = 0;
    virtual HRESULT __stdcall get_Rules(_GRules* *objRules) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
    virtual HRESULT __stdcall Validate(long enumDeepOrShallow, VARIANT *varReasons,
      VARIANT_BOOL *blnValid) = 0;
};

struct _GAttributeDisp : IDispatch
{
    VARIANT get_Value();
    void set_Value(VARIANT Value);
    _GAttributeClass* get_AttributeClass();
    _GOperations* get_Operations();
    _GRules* get_Rules();
    IDispatch * get_Parent();
    BOOL Validate(long enumDeepOrShallow, VARIANT *varReasons);
};

struct _GAttributeClass : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *strName) = 0;
    virtual HRESULT __stdcall get_OperationClasses(_GOperationClasses* *objClasses) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *objParent) = 0;
    virtual HRESULT __stdcall get_PickList(_GPickList* *objPickList) = 0;
    virtual HRESULT __stdcall get_RuleClasses(_GRuleClasses* *objClasses) = 0;
    virtual HRESULT __stdcall GetAttribute(IDispatch * objIFeature, _GAttribute* *objFeatureAttribute) = 0;
};

struct _GAttributeClassDisp : IDispatch
{
    LPTSTR get_Name();
    _GOperationClasses* get_OperationClasses();
    IDispatch * get_Parent();
    _GPickList* get_PickList();
    _GRuleClasses* get_RuleClasses();
    _GAttribute* GetAttribute(IDispatch * objIFeature);
};

struct _GPickList : IDispatch
{
    virtual HRESULT __stdcall get_List(GRecordset* *objGRecordset) = 0;
    virtual HRESULT __stdcall get_Name(BSTR *pbstrName) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
};

struct _GPickListDisp : IDispatch
{
    GRecordset* get_List();
    LPTSTR get_Name();
    IDispatch * get_Parent();
};

struct _GAssociationRoles : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GAssociationRole* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GAssociationRolesDisp : IDispatch
{
    long get_Count();
    _GAssociationRole* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GAttributeClasses : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT varIndex, _GAttributeClass* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GAttributeClassesDisp : IDispatch
{
    long get_Count();
    _GAttributeClass* get_Item(VARIANT varIndex);
    IUnknown * _NewEnum();
};

struct _GFeatureClassEvents : IUnknown
{
    virtual HRESULT __stdcall BeforeAddNew(VARIANT_BOOL *pvbCancel) = 0;
    virtual HRESULT __stdcall AfterAddNew(VARIANT_BOOL vbCanceled, _GFeature* objFeature) = 0;
    virtual HRESULT __stdcall BeforeEdit(_GFeature* objFeature, VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterEdit(VARIANT_BOOL vbCanceled, _GFeature* objFeature) = 0;
    virtual HRESULT __stdcall BeforeCancelUpdate(_GFeature* objFeature, VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterCancelUpdate(VARIANT_BOOL vbCanceled, _GFeature* objFeature) = 0;
    virtual HRESULT __stdcall BeforeUpdate(_GFeature* objFeature, VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterUpdate(VARIANT_BOOL vbCanceled, _GFeature* objFeature) = 0;
    virtual HRESULT __stdcall BeforeDelete(_GFeature* objFeature, VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterDelete(VARIANT_BOOL vbCanceled, VARIANT vKeys) = 0;
};

struct _GSelection : IDispatch
{
    virtual HRESULT __stdcall get_FeatureClass(_GFeatureClass* *objFeatureClass) = 0;
    virtual HRESULT __stdcall get_AttributeFilter(BSTR *strAttributeFilter) = 0;
    virtual HRESULT __stdcall get_GeometryAttributeClassName(VARIANT *vGeometryAttributeClassName) = 0;
    virtual HRESULT __stdcall get_SpatialFilterGeometry(VARIANT *vSpatialFilterGeometry) = 0;
    virtual HRESULT __stdcall get_SpatialFilterOperator(VARIANT *vSpatialFilterOperator) = 0;
};

struct _GSelectionDisp : IDispatch
{
    _GFeatureClass* get_FeatureClass();
    LPTSTR get_AttributeFilter();
    VARIANT get_GeometryAttributeClassName();
    VARIANT get_SpatialFilterGeometry();
    VARIANT get_SpatialFilterOperator();
};

struct _GAttributeClassEvents : IUnknown
{
    virtual HRESULT __stdcall BeforeSetValue(_GAttribute* objAttribute, VARIANT vNewValue,
      VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterSetValue(VARIANT_BOOL vbCanceled, _GAttribute* objAttribute) = 0;
};

struct _GAssociationRoleClassEvents : IUnknown
{
    virtual HRESULT __stdcall BeforeAssociate(_GAssociationRole* objAssociationRole,
      _GFeature* objFeature, VARIANT_BOOL *vbCanceled) = 0;
    virtual HRESULT __stdcall AfterAssociate(VARIANT_BOOL vbCanceled, _GAssociationRole* objAssociationRole,
      _GFeature* objFeature) = 0;
    virtual HRESULT __stdcall BeforeDisassociate(_GAssociationRole* objAssociationRole,
      _GFeature* objFeature, VARIANT_BOOL *vbCancel) = 0;
    virtual HRESULT __stdcall AfterDisassociate(VARIANT_BOOL vbCanceled, _GAssociationRole* objAssociationRole,
      _GFeature* objFeature) = 0;
};

struct _GInheritance : IDispatch
{
    virtual HRESULT __stdcall get_BaseClasses(_GInheritances* *colInheritances) = 0;
    virtual HRESULT __stdcall get_SubClasses(_GInheritances* *colInheritances) = 0;
    virtual HRESULT __stdcall get_Abstract(VARIANT_BOOL *blnIsAbstract) = 0;
    virtual HRESULT __stdcall IsKindOf(BSTR strClass, VARIANT_BOOL *blnIsKindOf) = 0;
};

struct _GInheritanceDisp : IDispatch
{
    _GInheritances* get_BaseClasses();
    _GInheritances* get_SubClasses();
    BOOL get_Abstract();
    BOOL IsKindOf(LPTSTR strClass);
};

struct _GInheritances : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *lngCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT vIndex, _GInheritance* *objItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppunk) = 0;
};

struct _GInheritancesDisp : IDispatch
{
    long get_Count();
    _GInheritance* get_Item(VARIANT vIndex);
    IUnknown * _NewEnum();
};

struct _GEventHandler : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *strName) = 0;
    virtual HRESULT __stdcall get_EventObject(IDispatch * *objEventObject) = 0;
    virtual HRESULT __stdcall set_EventObject(IDispatch * objEventObject) = 0;
    virtual HRESULT __stdcall get_Parent(IDispatch * *ppParent) = 0;
};

struct _GEventHandlerDisp : IDispatch
{
    LPTSTR get_Name();
    IDispatch * get_EventObject();
    void set_EventObject(IDispatch * Value);
    IDispatch * get_Parent();
};

struct _GExtensions : IDispatch
{
    virtual HRESULT __stdcall GetExtension(BSTR strExtensionName, IDispatch * *objExtension) = 0;
};

struct _GExtensionsDisp : IDispatch
{
    IDispatch * GetExtension(LPTSTR strExtensionName);
};

GFeatureModel* CreateGFeatureModel();

GFeatureClass* CreateGFeatureClass();

GFeature* CreateGFeature();

GSelection* CreateGSelection();

GAttributeClass* CreateGAttributeClass();

GAttribute* CreateGAttribute();

GAssociationRoleClass* CreateGAssociationRoleClass();

GAssociationRole* CreateGAssociationRole();

GOperationClass* CreateGOperationClass();

GOperation* CreateGOperation();

GRuleClass* CreateGRuleClass();

GRule* CreateGRule();

GPickList* CreateGPickList();

GInheritance* CreateGInheritance();

GEventHandler* CreateGEventHandler();

GFeatureClasses* CreateGFeatureClasses();

GAttributeClasses* CreateGAttributeClasses();

GAttributes* CreateGAttributes();

GAssociationRoleClasses* CreateGAssociationRoleClasses();

GAssociationRoles* CreateGAssociationRoles();

GOperationClasses* CreateGOperationClasses();

GOperations* CreateGOperations();

GRuleClasses* CreateGRuleClasses();

GRules* CreateGRules();

GExtensions* CreateGExtensions();

GInheritances* CreateGInheritances();

struct _GPersistMgr : IDispatch
{
    virtual HRESULT __stdcall get_Storage(IDispatch * *ppStorage) = 0;
    virtual HRESULT __stdcall set_Storage(IDispatch * ppStorage) = 0;
    virtual HRESULT __stdcall get_RootObject(_GPersist* *pRootObject) = 0;
    virtual HRESULT __stdcall RegisterObjectClass(BSTR bstrProgID, BSTR bstrStorageName,
      BSTR bstrSurrogateProgID) = 0;
    virtual HRESULT __stdcall UnregisterObjectClass(BSTR bstrProgID) = 0;
    virtual HRESULT __stdcall GetObjectClasses(BSTR sIID, VARIANT *pvNames) = 0;
    virtual HRESULT __stdcall CreatePersistInstance(BSTR bstrProgID, _GPersist* pParent,
      IDispatch * *ppObject) = 0;
    virtual HRESULT __stdcall Publish() = 0;
    virtual HRESULT __stdcall CommitChanges(VARIANT_BOOL vbRemoveOrphans, VARIANT_BOOL *pvbDBHasBeenModified) = 0;
    virtual HRESULT __stdcall get_Updatable(VARIANT_BOOL *pvbUpdatable) = 0;
};

struct _GPersistMgrDisp : IDispatch
{
    IDispatch * get_Storage();
    void set_Storage(IDispatch * Value);
    _GPersist* get_RootObject();
    void RegisterObjectClass(LPTSTR bstrProgID, LPTSTR bstrStorageName, LPTSTR bstrSurrogateProgID);
    void UnregisterObjectClass(LPTSTR bstrProgID);
    VARIANT GetObjectClasses(LPTSTR sIID);
    IDispatch * CreatePersistInstance(LPTSTR bstrProgID, _GPersist* pParent);
    void Publish();
    BOOL CommitChanges(BOOL vbRemoveOrphans);
    BOOL get_Updatable();
};

struct _GPersist : IDispatch
{
    virtual HRESULT __stdcall get_PersistData(_GPersistData* *ppPersistData) = 0;
    virtual HRESULT __stdcall set_PersistData(_GPersistData* ppPersistData) = 0;
    virtual HRESULT __stdcall get_DefinitionControlProgID(BSTR *strProgID) = 0;
    virtual HRESULT __stdcall get_ObjectClassDescription(BSTR *pbstrObjectClassDescription) = 0;
    virtual HRESULT __stdcall get_ObjectClassName(BSTR *pbstrObjectClassName) = 0;
    virtual HRESULT __stdcall get_PropertyDefinitions(VARIANT *pvaResults) = 0;
    virtual HRESULT __stdcall IsValidParentObject(IDispatch * objParentObject,
      VARIANT_BOOL *pvbValid) = 0;
    virtual HRESULT __stdcall Validate(long shTraversalConstant, VARIANT *pvErrors,
      VARIANT_BOOL *pvbIsValid) = 0;
};

struct _GPersistDisp : IDispatch
{
    _GPersistData* get_PersistData();
    void set_PersistData(_GPersistData* Value);
    LPTSTR get_DefinitionControlProgID();
    LPTSTR get_ObjectClassDescription();
    LPTSTR get_ObjectClassName();
    VARIANT get_PropertyDefinitions();
    BOOL IsValidParentObject(IDispatch * objParentObject);
    BOOL Validate(long shTraversalConstant, VARIANT *pvErrors);
};

struct _GPersistData : IDispatch
{
    virtual HRESULT __stdcall get_ParentObject(_GPersist* *ppParent) = 0;
    virtual HRESULT __stdcall get_PersistMgr(_GPersistMgr* *ppMgr) = 0;
    virtual HRESULT __stdcall get_BaseObject(_GPersist* *ppBaseObject) = 0;
    virtual HRESULT __stdcall GetBaseClasses(long lClassType, DGMClassObjects* *ppBaseClasses) = 0;
    virtual HRESULT __stdcall GetSubClasses(long lClassType, DGMClassObjects* *ppSubClasses) = 0;
    virtual HRESULT __stdcall get_Properties(VARIANT *pvProperties) = 0;
    virtual HRESULT __stdcall Clone(_GPersist* pParent, _GPersist* *ppClone) = 0;
    virtual HRESULT __stdcall Copy(_GPersist* pParent, _GPersistMgr* pPersistMgr,
      _GPersist* *ppCopy) = 0;
    virtual HRESULT __stdcall GetValue(BSTR sName, VARIANT *pvValue) = 0;
    virtual HRESULT __stdcall SetValue(BSTR sName, VARIANT vValue) = 0;
    virtual HRESULT __stdcall GetClonedValue(BSTR sName, VARIANT *pvValue) = 0;
    virtual HRESULT __stdcall Refresh() = 0;
    virtual HRESULT __stdcall IsKindOf(_GPersistData* pGPersistData, VARIANT_BOOL *pvbIsKindOf) = 0;
};

struct _GPersistDataDisp : IDispatch
{
    _GPersist* get_ParentObject();
    _GPersistMgr* get_PersistMgr();
    _GPersist* get_BaseObject();
    DGMClassObjects* GetBaseClasses(long lClassType);
    DGMClassObjects* GetSubClasses(long lClassType);
    VARIANT get_Properties();
    _GPersist* Clone(_GPersist* pParent);
    _GPersist* Copy(_GPersist* pParent, _GPersistMgr* pPersistMgr);
    VARIANT GetValue(LPTSTR sName);
    void SetValue(LPTSTR sName, VARIANT vValue);
    VARIANT GetClonedValue(LPTSTR sName);
    void Refresh();
    BOOL IsKindOf(_GPersistData* pGPersistData);
};

struct DGMClassObjects : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *plCount) = 0;
    virtual HRESULT __stdcall get_Item(VARIANT vIndex, _GPersist* *ppPersist) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *Enum) = 0;
    virtual HRESULT __stdcall Append(_GPersist* pPersist) = 0;
    virtual HRESULT __stdcall Remove(VARIANT vIndex, _GPersist* *ppPersist) = 0;
};

struct DGMClassObjectsDisp : IDispatch
{
    long get_Count();
    _GPersist* get_Item(VARIANT vIndex);
    IUnknown * _NewEnum();
    void Append(_GPersist* pPersist);
    _GPersist* Remove(VARIANT vIndex);
};

struct _GPersistObjectType : IDispatch
{
    virtual HRESULT __stdcall get_ObjectTypeName(BSTR *pbstrObjectTypeName) = 0;
    virtual HRESULT __stdcall get_NameOfPropertyName(BSTR *pbstrNameOfPropertyName) = 0;
    virtual HRESULT __stdcall get_PropertyDefinitions(VARIANT *pvPropertyDefinitions) = 0;
};

struct _GPersistObjectTypeDisp : IDispatch
{
    LPTSTR get_ObjectTypeName();
    LPTSTR get_NameOfPropertyName();
    VARIANT get_PropertyDefinitions();
};

struct _GPersistMgrEvents : IUnknown
{
    virtual HRESULT __stdcall AfterInsert(_GPersist* pGPersist) = 0;
    virtual HRESULT __stdcall AfterUpdate(_GPersist* pGPersist) = 0;
    virtual HRESULT __stdcall BeforeDelete(_GPersist* pGPersist) = 0;
};

struct _GPersistMgrEventsDisp : IDispatch
{
    void AfterInsert(_GPersist* pGPersist);
    void AfterUpdate(_GPersist* pGPersist);
    void BeforeDelete(_GPersist* pGPersist);
};

struct DGMDefinitionControl : IDispatch
{
    virtual HRESULT __stdcall SetObjects(VARIANT vObjects) = 0;
    virtual HRESULT __stdcall RestoreDefaults(BSTR bstrRootPreference, IDispatch * objPreferenceSet) = 0;
    virtual HRESULT __stdcall StoreDefaults(BSTR bstrRootPreference, IDispatch * objPreferenceSet) = 0;
    virtual HRESULT __stdcall Apply() = 0;
};

struct DGMDefinitionControlDisp : IDispatch
{
    void SetObjects(VARIANT vObjects);
    void RestoreDefaults(LPTSTR bstrRootPreference, IDispatch * objPreferenceSet);
    void StoreDefaults(LPTSTR bstrRootPreference, IDispatch * objPreferenceSet);
    void Apply();
};

GPersistMgr* CreateGPersistMgr();

GPersistData* CreateGPersistData();

GPersist* CreateGPersist();

GDefinitionControl* CreateGDefinitionControl();

GClassObjects* CreateGClassObjects();

GPersistObjectType* CreateGPersistObjectType();

// Constants for module AssociationRoleTypesConstants
#ifndef _AssociationRoleTypesConstants_
#define _AssociationRoleTypesConstants_
const BSTR gmartStandard = L"Standard";
const BSTR gmartOwner = L"Owner";
const BSTR gmartOwned = L"Owned";
#endif

// Constants for module ExtensionNameConstants
#ifndef _ExtensionNameConstants_
#define _ExtensionNameConstants_
const BSTR gmextRecordset = L"Recordset";
const BSTR gmextField = L"Field";
const BSTR gmextSelection = L"Selection";
const BSTR gmextTableDef = L"TableDef";
const BSTR gmextFeatureClass = L"FeatureClass";
const BSTR gmextAttributeClass = L"AttributeClass";
const BSTR gmextFeatureModel = L"FeatureModel";
#endif

// Constants for module PersistDataConstants
#ifndef _PersistDataConstants_
#define _PersistDataConstants_
const BSTR gmpdVersion = L"Version";
#endif


#ifdef __cplusplus
}
#endif
#endif
