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

#ifndef _PCSS_TLB_H
#define _PCSS_TLB_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <basetyps.h>
#include <wtypes.h>

#include <ocidl.h>

//
// GUID constant declarations
//

const IID IID_IGMProjAlgorithmStrings = {0x887BC700,0xD9A4,0x11CE,{0x96,0x56,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_ProjAlgorithmStrings = {0x887BC702,0xD9A4,0x11CE,{0x96,0x56,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMGeodeticDatumStrings = {0x9A107E90,0xE0B9,0x11CE,{0x96,0x57,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_GeodeticDatumStrings = {0x9A107E91,0xE0B9,0x11CE,{0x96,0x57,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMEllipsoidStrings = {0x9A107E92,0xE0B9,0x11CE,{0x96,0x57,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_EllipsoidStrings = {0x9A107E94,0xE0B9,0x11CE,{0x96,0x57,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMProjSpace = {0x803E6BF0,0xB312,0x11CE,{0x96,0x4D,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_ProjSpace = {0x803E6BF2,0xB312,0x11CE,{0x96,0x4D,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMProjSpace2 = {0xB7D9B9DD,0xADD9,0x41D9,{0x8F,0x4A,0x22,0x2E,0x6C,0xF3,0x2E,0xA2}};
const IID IID_IGMProjSpace3 = {0xC93D1A5F,0xA5A0,0x426E,{0x86,0xD0,0xEF,0xF0,0x68,0x57,0x71,0x8B}};
const IID IID_IGMProjSpace4 = {0x2FEE116A,0x949E,0x4405,{0x9D,0x4C,0x0E,0x27,0x7F,0xF1,0x8C,0x76}};
const IID IID_IGMGeogSpace = {0x76416DE0,0xC82D,0x11CE,{0x96,0x53,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_GeogSpace = {0x76416DE1,0xC82D,0x11CE,{0x96,0x53,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMGeogSpace2 = {0x5921AFE9,0x73FA,0x4EDA,{0x9E,0x4C,0xBC,0xCE,0xC9,0x27,0x1D,0x35}};
const IID DIID_DGMStringCollection = {0x6D3D6F80,0xF7B9,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMUnitFormatSpec = {0xFF65CAE0,0x4E8A,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_UnitFormatSpec = {0x2457B700,0x4E96,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMUnitFormatSpec2 = {0x477E9884,0x2C6C,0x4BE1,{0xB9,0x54,0x97,0xE5,0xF6,0x0E,0xE5,0x36}};
const IID IID_IGMPaperSpace = {0xD74800A0,0x3762,0x11D0,{0xAF,0x9C,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID DIID_PaperSpace = {0xD74800A1,0x3762,0x11D0,{0xAF,0x9C,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID IID_IGMRefSpaceMgr = {0xC85D2720,0x49A1,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_RefSpaceMgr = {0x06E855C0,0x49A2,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMRefSpaceMgr2 = {0x878BDEB7,0x107D,0x475C,{0x9B,0x50,0x0B,0x50,0x5F,0x01,0xFE,0x17}};
const IID IID_IGMCoordSystem = {0xA94CD060,0x33DF,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_DGMCoordSystem = {0x15626FD0,0x33E0,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMCoordSystem2 = {0x4A9551F7,0xC75B,0x40F7,{0xB5,0x57,0x02,0xD6,0x00,0x97,0x34,0x5C}};
const IID IID_IGMCoordSystem3 = {0x02DA93B1,0xDF92,0x4621,{0xBC,0xA3,0xD5,0x34,0xD3,0x30,0x6C,0x8D}};
const IID DIID_DGMDatumTransformation = {0xE3E00BE0,0xCB04,0x11D0,{0xAF,0xC5,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID DIID_DGMBursaWolf = {0x75184600,0xF3C4,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_DGMComplexPolynomial = {0xBD405000,0xF3C9,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_DGMMultipleRegression = {0xC60B4CB0,0xF3CA,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_DGMSecondDegreePolynomial = {0x46935D20,0xF3CB,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_DGMStandardMolodensky = {0x2A76DE30,0xF3C5,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMLink = {0x7080A269,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_Link = {0x7080A26A,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMLinks = {0x7080A264,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_Links = {0x7080A265,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMTransLinks = {0x7080A267,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_TransLinks = {0x7080A268,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMAltCoordSystemPath = {0x7080A262,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_AltCoordSystemPath = {0x7080A263,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMAltCoordSystemPaths = {0x7080A260,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID DIID_AltCoordSystemPaths = {0x7080A261,0x80F3,0x11CF,{0xA1,0x83,0x08,0x00,0x36,0x01,0xD5,0x02}};
const IID IID_IGMCoordSystemsMgr = {0xE4B5E810,0x3DE4,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_DGMCoordSystemsMgr = {0xC8F007A0,0x3DE6,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID IID_IGMUnitIDs = {0x116F4080,0xE3D7,0x11CF,{0xAF,0x86,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID DIID_UnitIDs = {0x116F4081,0xE3D7,0x11CF,{0xAF,0x86,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID IID_IGMUnitIDs2 = {0x10336D08,0x1C3F,0x4415,{0xB6,0xA9,0xD5,0x23,0xC9,0x0C,0x24,0x74}};
const IID IID_IGMUnitsOfMeasureTables = {0xD009AF70,0xDCC2,0x11CE,{0xAF,0x56,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID DIID_DGMUnitsOfMeasure = {0xD009AF71,0xDCC2,0x11CE,{0xAF,0x56,0x08,0x00,0x36,0x01,0xE5,0x29}};
const IID IID_IGMUnitsOfMeasureTables2 = {0x3EB69073,0x512E,0x413D,{0x8B,0x70,0x04,0xE7,0x99,0xF1,0x75,0x4B}};
const IID DIID_DGMLeastSquares = {0xE054829E,0xF325,0x11D0,{0xBC,0x0C,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMSingleParameterConstraints = {0x0FDDA0F8,0xF967,0x11D0,{0xBC,0x0C,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMSingleParameterConstraint = {0xCCA27FFF,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMObservations = {0xCCA27FFD,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMObservation = {0xCCA28001,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMLinearModelResidual = {0x36F830DF,0x0913,0x11D1,{0xBC,0x10,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID DIID_DGMNonlinearModelResidual = {0x36F830E1,0x0913,0x11D1,{0xBC,0x10,0x08,0x00,0x36,0xEA,0x30,0x03}};
const IID IID_UnitAndFormatSpecEvents = {0xB666885E,0x05DD,0x433A,{0xAB,0x83,0x35,0xF2,0x7C,0xE5,0x35,0x90}};
const CLSID CLASS_UnitAndFormatSpec = {0x28607600,0x106D,0x11CE,{0x89,0x43,0x08,0x00,0x36,0x01,0xE5,0x29}};
const CLSID CLASS_CoordSystem = {0xF9796220,0x1876,0x11CE,{0x9C,0xB5,0x08,0x00,0x36,0x01,0xC1,0xAF}};
const IID DIID_DGMDatumTransformation2 = {0xCFF5EB8A,0x74BE,0x11D5,{0x8B,0xCD,0x00,0x00,0xE2,0x30,0xAB,0x17}};
const IID DIID_DGMDatumTransformation3 = {0x35920EA7,0x11FB,0x43EC,{0x83,0x79,0xB6,0xA3,0x12,0x60,0x5D,0xEA}};
const CLSID CLASS_DatumTransformation = {0x0F02BC00,0xCBCA,0x11D0,{0xAF,0xC5,0x08,0x00,0x36,0x01,0xE5,0x29}};
const CLSID CLASS_CoordSystemsMgr = {0x535F6210,0x2996,0x11CE,{0x9B,0xC7,0x08,0x00,0x36,0x22,0xFF,0x01}};
const CLSID CLASS_UnitsOfMeasure = {0xBB814580,0xC2DD,0x11CE,{0xAF,0x4F,0x08,0x00,0x36,0x01,0xE5,0x29}};
const CLSID CLASS_BursaWolf = {0x329CF2C0,0xF6E2,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const CLSID CLASS_ComplexPolynomial = {0xAAE227B0,0xF6E2,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const CLSID CLASS_MultipleRegression = {0xD5ED1CC0,0xF6E2,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const CLSID CLASS_SecondDegreePolynomial = {0xFA3ADF20,0xF6E2,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const CLSID CLASS_StandardMolodensky = {0x10CF9570,0xF6E3,0x11D0,{0xA2,0x6D,0x08,0x00,0x36,0x01,0xD5,0x02}};
const CLSID CLASS_LeastSquares = {0x75692B94,0xF328,0x11D0,{0xBC,0x0C,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_SingleParameterConstraints = {0x0FDDA0F9,0xF967,0x11D0,{0xBC,0x0C,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_SingleParameterConstraint = {0xCCA27FFE,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_Observations = {0xCCA27FFC,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_Observation = {0xCCA28000,0x08FC,0x11D1,{0xBC,0x0F,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_LinearModelResidual = {0x36F830DE,0x0913,0x11D1,{0xBC,0x10,0x08,0x00,0x36,0xEA,0x30,0x03}};
const CLSID CLASS_NonlinearModelResidual = {0x36F830E0,0x0913,0x11D1,{0xBC,0x10,0x08,0x00,0x36,0xEA,0x30,0x03}};


// Constants for enum CSProjectionAlgorithmConstants
#ifndef _CSProjectionAlgorithmConstants_
#define _CSProjectionAlgorithmConstants_
typedef enum _CSProjectionAlgorithmConstants {
    cspaUndefined = 0x00000000,
    cspaMercator = 0x00000001,
    cspaLambert = 0x00000002,
    cspaNorthPolar = 0x00000003,
    cspaSouthPolar = 0x00000004,
    cspaLinearElastic = 0x00000005,
    cspaSimpleCylindrical = 0x00000005,
    cspaTransverseMercator = 0x00000006,
    cspaUniveralTransverseMercator = 0x00000008,
    cspaUniversalTransverseMercator = 0x00000008,
    cspaIndonesianPoly = 0x00000009,
    cspaSPCS27 = 0x0000000A,
    cspaPolyconic = 0x0000000B,
    cspaAlbers = 0x0000000C,
    cspaObliqueMercator = 0x0000000D,
    cspaNewZealand = 0x0000000E,
    cspaVanDerGrinten = 0x0000000F,
    cspaAzimuthalEquidistant = 0x00000013,
    cspaOrthographic = 0x00000014,
    cspaSPCS83 = 0x00000015,
    cspaStereographic = 0x00000016,
    cspaGnomonic = 0x00000017,
    cspaRobinson = 0x00000018,
    cspaCassini = 0x00000019,
    cspaSinusoidal = 0x0000001A,
    cspaLambertAzimuthalEqArea = 0x0000001B,
    cspaMillerCyl = 0x0000001C,
    cspaBonne = 0x0000001D,
    cspaBritNGrid = 0x0000001E,
    cspaMollweide = 0x0000001F,
    cspaLocalSpaceRect = 0x00000020,
    cspaUniversalPolarStereo = 0x00000021,
    cspaRectifiedSkewOrthomorphic = 0x00000023,
    cspaIMWModPolycon = 0x00000024,
    cspaObliqueLambert = 0x00000025,
    cspaChamberlinTrimetric = 0x00000026,
    cspaCylEquirect = 0x00000027,
    cspaGeneralPespective = 0x00000028,
    cspaGeneralPerspective = 0x00000028,
    cspaConic = 0x00000029,
    cspaStereo3Step = 0x0000002A,
    cspaGaussKruger = 0x0000002B,
    cspaGaussKruger3Deg = 0x0000002B,
    cspaBIPOCC = 0x0000002C,
    cspaLaborde = 0x0000002D,
    cspaEckertIV = 0x0000002E,
    cspaSpaceObliqueMercator = 0x0000002F,
    cspaSpaceObliqueMercatorPS = 0x00000030,
    cspaKrovak = 0x00000031,
    cspaSwitzerlandLV03 = 0x00000032,
    cspaSwitzerlandLV95 = 0x00000033,
    cspaGaussConformalSA = 0x00000034,
    cspaGaussKrugerEC = 0x00000035,
    cspaGaussKruger6Deg = 0x00000035,
    cspaTokyoStatePlaneCS = 0x00000037,
    cspaJapanPlaneRectangularTokyo = 0x00000037,
    cspaNewBrunswickStereoATS77 = 0x00000038,
    cspaNewBrunswickStereoNAD83 = 0x00000039,
    cspaJapanPlaneRectangularJGD2000 = 0x0000003A,
    cspaNewZealandTransverseMercator = 0x0000003B,
    cspaKKJ = 0x0000003C,
    cspaGaussBoaga = 0x0000003D,
    cspaSWEREF99 = 0x0000003E,
    cspaHRHDKS = 0x0000003F,
    cspaSITM = 0x00000040,
    cspaMKTM = 0x00000041,
    cspaRT90 = 0x00000042,
    cspaBudapestStereographic = 0x00000043,
    cspaEOV = 0x00000044,
    cspaHungarianObliqueCylHER = 0x00000045,
    cspaHungarianObliqueCylHKR = 0x00000046,
    cspaHungarianObliqueCylHDR = 0x00000047,
    cspaARC = 0x00000048,
    cspaRDAmersfoort = 0x00000049
} CSProjectionAlgorithmConstants;
#endif

// Constants for enum CSGeographicHemisphereSelectionConstants
#ifndef _CSGeographicHemisphereSelectionConstants_
#define _CSGeographicHemisphereSelectionConstants_
typedef enum _CSGeographicHemisphereSelectionConstants {
    csghsNorthern = 0x00000000,
    csghsSouthern = 0x00000001
} CSGeographicHemisphereSelectionConstants;
#endif

// Constants for enum CSAzimuthDefinitionModeConstants
#ifndef _CSAzimuthDefinitionModeConstants_
#define _CSAzimuthDefinitionModeConstants_
typedef enum _CSAzimuthDefinitionModeConstants {
    csadmDirect = 0x00000000,
    csadmByTwoPoints = 0x00000001
} CSAzimuthDefinitionModeConstants;
#endif

// Constants for enum CSGeodeticDatumConstants
#ifndef _CSGeodeticDatumConstants_
#define _CSGeodeticDatumConstants_
typedef enum _CSGeodeticDatumConstants {
    csgdNamed = 0xFFFFFFFE,
    csgdUndefined = 0xFFFFFFFF,
    csgdUserDefined = 0x00000000,
    csgdAFG = 0x00000001,
    csgdAustralian1966 = 0x00000002,
    csgdAustralian1984 = 0x00000003,
    csgdEuropean1950 = 0x00000004,
    csgdEuropean1979 = 0x00000005,
    csgdGeodDatum1949 = 0x00000006,
    csgdIndian = 0x00000007,
    csgdIreland1965 = 0x00000008,
    csgdNAD27 = 0x00000009,
    csgdNAD83 = 0x0000000A,
    csgdOrdnanceSurveyGreatBritain = 0x0000000B,
    csgdPulkovo1942 = 0x0000000C,
    csgdQornoq = 0x0000000D,
    csgdTokyo = 0x0000000E,
    csgdWGS66 = 0x0000000F,
    csgdWGS72 = 0x00000010,
    csgdWGS84 = 0x00000011,
    csgdSAD1956 = 0x00000012,
    csgdSAD1969 = 0x00000013,
    csgdIndonesian1974 = 0x00000014,
    csgdWGS60 = 0x00000015,
    csgdTananObservatory1925 = 0x00000016,
    csgdKertau1948 = 0x00000017,
    csgdPotsdam = 0x00000018,
    csgdUSStandard = 0x00000019,
    csgdMGICS = 0x0000001A,
    csgdGeocDatumOfAustralia94 = 0x0000001B,
    csgdAgadez = 0x0000001C,
    csgdAmersfoort = 0x0000001D,
    csgdAncienneTriangFrancaise = 0x0000001E,
    csgdAratu = 0x0000001F,
    csgdAvgTerrestrialSystem1977 = 0x00000020,
    csgdBarbados = 0x00000021,
    csgdBatavia = 0x00000022,
    csgdBeduaram = 0x00000023,
    csgdBeijing1954 = 0x00000024,
    csgdBern1898 = 0x00000025,
    csgdBern1938 = 0x00000026,
    csgdCamacupa = 0x00000027,
    csgdConakry1905 = 0x00000028,
    csgdCotedIvoire = 0x00000029,
    csgdDatum73 = 0x0000002A,
    csgdDealulPiscului1933 = 0x0000002B,
    csgdDealulPiscului1970 = 0x0000002C,
    csgdDeirezZor = 0x0000002D,
    csgdDeutscheHauptdreiecksnetz = 0x0000002E,
    csgdDouala = 0x0000002F,
    csgdEuropean1987 = 0x00000030,
    csgdETRS89 = 0x00000031,
    csgdFahud = 0x00000032,
    csgdGaroua = 0x00000033,
    csgdGreek = 0x00000034,
    csgdGreekGRS1987 = 0x00000035,
    csgdGuyaneFrancaise = 0x00000036,
    csgdHitoXVIII1963 = 0x00000037,
    csgdHungarianDatum1972 = 0x00000038,
    csgdJamaica1875 = 0x00000039,
    csgdJamaica1969 = 0x0000003A,
    csgdKalianpur = 0x0000003B,
    csgdKalianpur1880 = 0x0000003B,
    csgdKKJ = 0x0000003C,
    csgdKuwaitOilCompany = 0x0000003D,
    csgdKuwaitUtility = 0x0000003E,
    csgdLaCanoa = 0x0000003F,
    csgdLake = 0x00000040,
    csgdLisbon = 0x00000041,
    csgdLomaQuintana = 0x00000042,
    csgdLome = 0x00000043,
    csgdMakassar = 0x00000044,
    csgdMalongo1987 = 0x00000045,
    csgdManoca = 0x00000046,
    csgdMhast = 0x00000047,
    csgdMilitarGeogInstitut = 0x00000048,
    csgdMonteMario = 0x00000049,
    csgdNationalGeodeticNetwork = 0x0000004A,
    csgdNGO1948 = 0x0000004B,
    csgdNorddeGuerre = 0x0000004C,
    csgdNouvelleTriangFrancaise = 0x0000004D,
    csgdNSWC9Z2 = 0x0000004E,
    csgdOSSN1980 = 0x0000004F,
    csgdOSGB1970SN = 0x00000050,
    csgdPadang1884 = 0x00000051,
    csgdPalestine1923 = 0x00000052,
    csgdPulkovo1995 = 0x00000053,
    csgdQatar1948 = 0x00000054,
    csgdReseauNationalBelge1950 = 0x00000055,
    csgdReseauNationalBelge1972 = 0x00000056,
    csgdSegora = 0x00000057,
    csgdSerindung = 0x00000058,
    csgdStockholm1938 = 0x00000059,
    csgdSwedishRT38 = 0x00000059,
    csgdSudan = 0x0000005A,
    csgdTM65 = 0x0000005B,
    csgdTM75 = 0x0000005C,
    csgdTrinidad1903 = 0x0000005D,
    csgdTrucialCoast1948 = 0x0000005E,
    csgdVoirol1875EPSG = 0x0000005F,
    csgdWGS72TransitBE = 0x00000060,
    csgdYoff = 0x00000061,
    csgdAdindan = 0x00000064,
    csgdAinelAbd1970 = 0x00000066,
    csgdAmericanSamoaDatum1962 = 0x00000067,
    csgdAnna1Astro1965 = 0x00000068,
    csgdAntiguaIslandAstro1943 = 0x00000069,
    csgdArc1950 = 0x0000006A,
    csgdArc1950NIMA = 0x0000006A,
    csgdArc1950NGA = 0x0000006A,
    csgdArc1960 = 0x0000006B,
    csgdAscensionIsland1958 = 0x0000006C,
    csgdAstroBeaconE1945 = 0x0000006D,
    csgdAstroDOS71s4 = 0x0000006E,
    csgdAstroTernIsland1961 = 0x0000006F,
    csgdAstronomicalStation1952 = 0x00000070,
    csgdAyabelleLighthouse = 0x00000073,
    csgdBellevueIGN = 0x00000074,
    csgdBermuda1957 = 0x00000075,
    csgdBissau = 0x00000076,
    csgdBogotaObservatory = 0x00000077,
    csgdBukitRimpah = 0x00000078,
    csgdCampAreaAstro = 0x00000079,
    csgdCampoInchauspe = 0x0000007A,
    csgdCantonAstro1966 = 0x0000007B,
    csgdCape = 0x0000007C,
    csgdCapeNIMA = 0x0000007C,
    csgdCapeNGA = 0x0000007C,
    csgdCapeCanaveral = 0x0000007D,
    csgdCarthage = 0x0000007E,
    csgdCarthageNIMA = 0x0000007E,
    csgdCarthageNGA = 0x0000007E,
    csgdChathamIslandAstro1971 = 0x0000007F,
    csgdChuaAstro = 0x00000080,
    csgdCorregoAlegre = 0x00000081,
    csgdDabola = 0x00000082,
    csgdDeceptionIslandAntarctica = 0x00000083,
    csgdDjakartaBatavia = 0x00000084,
    csgdDOS1968 = 0x00000085,
    csgdEasterIsland1967 = 0x00000086,
    csgdFortThomas1955 = 0x00000089,
    csgdGan1970 = 0x0000008A,
    csgdGraciosaBaseSW1948 = 0x0000008C,
    csgdGuam1963 = 0x0000008D,
    csgdGunungSegara = 0x0000008E,
    csgdGUX1Astro = 0x0000008F,
    csgdHeratNorth = 0x00000090,
    csgdHjorsey1955 = 0x00000091,
    csgdHongKong1963 = 0x00000092,
    csgdHuTzuShan = 0x00000093,
    csgdIndian1954 = 0x00000095,
    csgdIndian1960 = 0x00000096,
    csgdIndian1975 = 0x00000097,
    csgdISTS061Astro1968 = 0x0000009A,
    csgdISTS073Astro1969 = 0x0000009B,
    csgdJohnstonIsland1961 = 0x0000009C,
    csgdKandawala = 0x0000009D,
    csgdKerguelenIsland1949 = 0x0000009E,
    csgdKusaieAstro1951 = 0x000000A0,
    csgdLC5Astro1961 = 0x000000A1,
    csgdLeigon = 0x000000A2,
    csgdLiberia1964 = 0x000000A3,
    csgdLuzon = 0x000000A4,
    csgdMPoraloko = 0x000000A5,
    csgdMPoralokoNIMA = 0x000000A5,
    csgdMPoralokoNGA = 0x000000A5,
    csgdMahe1971 = 0x000000A6,
    csgdMassawa = 0x000000A7,
    csgdMerchich = 0x000000A8,
    csgdMerchichNIMA = 0x000000A8,
    csgdMerchichNGA = 0x000000A8,
    csgdMidwayAstro1961 = 0x000000A9,
    csgdMinna = 0x000000AA,
    csgdMontserratIslandAstro1958 = 0x000000AB,
    csgdNahrwan = 0x000000AC,
    csgdNaparimaBWI = 0x000000AD,
    csgdNorthSaharaGeodeticSystem1959 = 0x000000B0,
    csgdNorthSahara1959NIMA = 0x000000B0,
    csgdNorthSahara1959NGA = 0x000000B0,
    csgdObservatorioMeteorologico1939 = 0x000000B1,
    csgdOldEgyptian1907 = 0x000000B2,
    csgdOldHawaiian = 0x000000B3,
    csgdOman = 0x000000B4,
    csgdPicodelasNieves = 0x000000B6,
    csgdPitcairnAstro1967 = 0x000000B7,
    csgdPoint58 = 0x000000B8,
    csgdPointeNoire1948 = 0x000000B9,
    csgdPointeNoire1948NIMA = 0x000000B9,
    csgdPointeNoire1948NGA = 0x000000B9,
    csgdPortoSanto1936 = 0x000000BA,
    csgdProvisionalSouthChilean1963 = 0x000000BC,
    csgdPuertoRico = 0x000000BD,
    csgdQatarNational = 0x000000BF,
    csgdReunion = 0x000000C1,
    csgdRome1940 = 0x000000C2,
    csgdS42Pulkovo1942 = 0x000000C3,
    csgdSJTSK = 0x000000C4,
    csgdSantoDOS1965 = 0x000000C5,
    csgdSaoBraz = 0x000000C6,
    csgdSapperHill1943 = 0x000000C7,
    csgdSchwarzeck = 0x000000C8,
    csgdSelvagemGrande1938 = 0x000000C9,
    csgdSouthAsia = 0x000000CB,
    csgdTimbalai1948 = 0x000000CD,
    csgdTristanAstro1968 = 0x000000CF,
    csgdVitiLevu1916 = 0x000000D0,
    csgdVoirol1960 = 0x000000D1,
    csgdVoirol1960NIMA = 0x000000D1,
    csgdVoirol1960NGA = 0x000000D1,
    csgdWakeIslandAstro1952 = 0x000000D2,
    csgdWakeEniwetok1960 = 0x000000D3,
    csgdYacare = 0x000000D6,
    csgdZanderij = 0x000000D7,
    csgdIndianIndiaAndNepal = 0x0000012D,
    csgdIndianPakistan = 0x0000012E,
    csgdCoordSys1937Estonia = 0x0000012F,
    csgdSierraLeone1960 = 0x00000130,
    csgdHermannskogel = 0x00000131,
    csgdRiketskoordinatsystem1990 = 0x00000132,
    csgdSwedishRT90 = 0x00000132,
    csgdSamboja = 0x00000133,
    csgdLithuania1994ETRS89 = 0x00000134,
    csgdNAD83HARN = 0x00000135,
    csgdArc1950EPSG = 0x00000136,
    csgdCapeEPSG = 0x00000137,
    csgdCarthageEPSG = 0x00000138,
    csgdMPoralokoEPSG = 0x00000139,
    csgdMerchichEPSG = 0x0000013A,
    csgdNorthSahara1959EPSG = 0x0000013B,
    csgdPointeNoire1948EPSG = 0x0000013C,
    csgdVoirol1960EPSG = 0x0000013D,
    csgdVoirol1874NIMA = 0x0000013E,
    csgdVoirol1874NGA = 0x0000013E,
    csgdReseauGeodFrancais1993 = 0x0000013F,
    csgdHartebeesthoek94 = 0x00000140,
    csgdCH1903 = 0x00000141,
    csgdCH1903Plus = 0x00000142,
    csgdCHTRF95 = 0x00000143,
    csgdJapanGeodDatum2000 = 0x00000144,
    csgdNZGeodDatum2000 = 0x00000145,
    csgdKoreanDatum1995 = 0x00000146,
    csgdHongKong1980 = 0x00000147,
    csgdTWD67 = 0x00000148,
    csgdTWD97 = 0x00000149,
    csgdKalianpur1937 = 0x0000014A,
    csgdKalianpur1962 = 0x0000014B,
    csgdKalianpur1975 = 0x0000014C,
    csgdHungarianStereo1863 = 0x0000014D,
    csgdHungarianFasching1909 = 0x0000014E,
    csgdIRENET95 = 0x0000014F,
    csgdEUREFFIN = 0x00000150,
    csgdITRF2000 = 0x00000151,
    csgdTimbalaiSabah = 0x00000152,
    csgdTimbalaiSarawak = 0x00000153,
    csgdDatumBaseSE = 0x00000154,
    csgdGeocDatumMalaysia2000 = 0x00000155,
    csgdMAGNASIRGAS = 0x00000156,
    csgdSIRGAS2000 = 0x00000157,
    csgdHDKS = 0x00000158,
    csgdD48 = 0x00000159,
    csgdMK = 0x0000015A,
    csgdSWEREF99 = 0x0000015B
} CSGeodeticDatumConstants;
#endif

// Constants for enum CSEllipsoidConstants
#ifndef _CSEllipsoidConstants_
#define _CSEllipsoidConstants_
typedef enum _CSEllipsoidConstants {
    cseUndefined = 0x00000000,
    cseGRS80 = 0x00000001,
    cseWGS72 = 0x00000002,
    cseAussyNatSAmer69 = 0x00000003,
    cseAustralianNational = 0x00000003,
    cseKrassovsky = 0x00000004,
    cseInternational = 0x00000005,
    cseWGS66 = 0x00000006,
    cseClark1880 = 0x00000007,
    cseClark1880NIMA = 0x00000007,
    cseClarke1880NGA = 0x00000007,
    cseClark1866 = 0x00000008,
    cseClarke1866 = 0x00000008,
    cseHelmert1906 = 0x00000009,
    cseBessel1841 = 0x0000000A,
    cseDanish = 0x0000000B,
    cseStruve = 0x0000000C,
    cseHough = 0x0000000D,
    cseAiry1830 = 0x0000000E,
    cseEverest1830 = 0x0000000F,
    cseEverest1830Adj1937 = 0x0000000F,
    cseModAiry = 0x00000010,
    cseModEverest = 0x00000011,
    cseEverestSingaporeAndWMalaysia1948 = 0x00000011,
    cseFischer1960Mercury = 0x00000012,
    cseModFischer1960SA = 0x00000013,
    cseFischer1968 = 0x00000014,
    cseUserDefined = 0x00000015,
    cseWGS84 = 0x00000016,
    cseIndoNational1974 = 0x00000017,
    cseWGS60 = 0x00000018,
    cseUnitSphere = 0x00000019,
    cseEverestBruneiAndEMalaysia = 0x0000001A,
    cseEverestBruneiAndEMalaysia1967 = 0x0000001A,
    cseEverestIndia1956 = 0x0000001B,
    cseEverest1830Def1956And1962 = 0x0000001B,
    cseEverest1830Def1956 = 0x0000001B,
    cseEverestWMalaysia1969 = 0x0000001C,
    cseBessel1841Namibia = 0x0000001D,
    cseEverestPakistan = 0x0000001E,
    cseClarke1880IGN = 0x0000001F,
    cseClarke1880Arc = 0x00000020,
    cseClarke1880Benoit = 0x00000021,
    cseClarke1880SGA1922 = 0x00000022,
    cseAvgTerrestrialSystem1977 = 0x00000023,
    cseModifiedBessel = 0x00000024,
    cseClarke1858EPSG = 0x00000025,
    cseGEM10C = 0x00000026,
    cseGRS1967 = 0x00000027,
    cseNWL10D = 0x00000028,
    cseNWL9D = 0x00000029,
    cseOSU86F = 0x0000002A,
    cseOSU91A = 0x0000002B,
    csePlessis1817 = 0x0000002C,
    cseStruve1860 = 0x0000002D,
    cseWarOffice = 0x0000002E,
    cseClarke1880EPSG = 0x0000002F,
    cseEverest1830Def1975 = 0x00000030,
    cseEverest1830IndianFeet1865 = 0x00000031,
    cseSouthAmerican1969 = 0x00000032,
    cseGRS1967Rounded = 0x00000033,
    cseEverest1830Def1962 = 0x00000034
} CSEllipsoidConstants;
#endif

// Constants for enum CSVerticalDatumConstants
#ifndef _CSVerticalDatumConstants_
#define _CSVerticalDatumConstants_
typedef enum _CSVerticalDatumConstants {
    csvdUserDefined = 0x00000000,
    csvdNGVD29 = 0x00000001,
    csvdNAVD88 = 0x00000002,
    csvdAHD71 = 0x00000003,
    csvdIGLD55 = 0x00000004,
    csvdONTGL = 0x00000005,
    csvdTsingtao = 0x00000006,
    csvdUELN55 = 0x00000007,
    csvdUELN73 = 0x00000008,
    csvdUELN7381 = 0x00000009,
    csvdCVR28 = 0x0000000A,
    csvdUELN9598 = 0x0000000B,
    csvdIGLD85 = 0x0000000C,
    csvdNAD83 = 0x0000000D,
    csvdUnspecified = 0x0000000E,
    csvdGeoid = 0x00000064,
    csvdEllipsoid = 0x00000065,
    csvdEGM = 0x00000066,
    csvdNGSGravimetric = 0x00000067,
    csvdNZVDGeoid = 0x00000068,
    csvdBrazilImbituba = 0x00000069,
    csvdDHHN92 = 0x0000006A,
    csvdRH00 = 0x0000006B,
    csvdRH70 = 0x0000006C,
    csvdRH2000 = 0x0000006D
} CSVerticalDatumConstants;
#endif

// Constants for enum CSGeographicOrderConstants
#ifndef _CSGeographicOrderConstants_
#define _CSGeographicOrderConstants_
typedef enum _CSGeographicOrderConstants {
    csgoLongitudeLatitude = 0x00000000,
    csgoLatitudeLongitude = 0x00000001
} CSGeographicOrderConstants;
#endif

// Constants for enum CSGeographicHemisphereOptionConstants
#ifndef _CSGeographicHemisphereOptionConstants_
#define _CSGeographicHemisphereOptionConstants_
typedef enum _CSGeographicHemisphereOptionConstants {
    csghoPositiveEastNorth = 0x00000000,
    csghoPositiveWestSouth = 0x00000001,
    csghoPositiveEastSouth = 0x00000002,
    csghoPositiveWestNorth = 0x00000003,
    csghoPrefixHemisphereDesignator = 0x00000004,
    csghoAppendHemisphereDesignator = 0x00000005
} CSGeographicHemisphereOptionConstants;
#endif

// Constants for enum CSLongitudeNormalizationConstants
#ifndef _CSLongitudeNormalizationConstants_
#define _CSLongitudeNormalizationConstants_
typedef enum _CSLongitudeNormalizationConstants {
    cslnZeroTo360LeftInclude = 0x00000000,
    cslnZeroTo360BothInclude = 0x00000001,
    cslnMinus180ToPlus180LeftInclude = 0x00000002,
    cslnMinus180ToPlus180BothInclude = 0x00000003,
    cslnMinus360To0RightInclude = 0x00000004,
    cslnMinus360To0BothInclude = 0x00000005,
    cslnNoLongitudeNormalization = 0x00000006
} CSLongitudeNormalizationConstants;
#endif

// Constants for enum CSProjectionQuadrantConstants
#ifndef _CSProjectionQuadrantConstants_
#define _CSProjectionQuadrantConstants_
typedef enum _CSProjectionQuadrantConstants {
    cspqXYPositiveEastNorth = 0x00000000,
    cspqXYPositiveEastSouth = 0x00000001,
    cspqXYPositiveSouthWest = 0x00000002,
    cspqXYPositiveSouthEast = 0x00000003,
    cspqXYPositiveWestNorth = 0x00000004,
    cspqXYPositiveWestSouth = 0x00000005,
    cspqXYPositiveNorthEast = 0x00000006,
    cspqXYPositiveNorthWest = 0x00000007
} CSProjectionQuadrantConstants;
#endif

// Constants for enum CSHeightDisplayConstants
#ifndef _CSHeightDisplayConstants_
#define _CSHeightDisplayConstants_
typedef enum _CSHeightDisplayConstants {
    cshDisplayHeightsYes = 0x00000000,
    cshDisplayHeightsNo = 0x00000001
} CSHeightDisplayConstants;
#endif

// Constants for enum CSHeightDirectionConstants
#ifndef _CSHeightDirectionConstants_
#define _CSHeightDirectionConstants_
typedef enum _CSHeightDirectionConstants {
    cshDirectionPositiveUp = 0x00000000,
    cshDirectionPositiveDown = 0x00000001
} CSHeightDirectionConstants;
#endif

// Constants for enum CSNegativeSuppressionLongitudeConstants
#ifndef _CSNegativeSuppressionLongitudeConstants_
#define _CSNegativeSuppressionLongitudeConstants_
typedef enum _CSNegativeSuppressionLongitudeConstants {
    csnsPreserveLongitudeMinus = 0x00000000,
    csnsDropLongitudeMinus = 0x00000001
} CSNegativeSuppressionLongitudeConstants;
#endif

// Constants for enum CSNegativeSuppressionHeightConstants
#ifndef _CSNegativeSuppressionHeightConstants_
#define _CSNegativeSuppressionHeightConstants_
typedef enum _CSNegativeSuppressionHeightConstants {
    csnsPreserveHeightMinus = 0x00000000,
    csnsDropHeightMinus = 0x00000001
} CSNegativeSuppressionHeightConstants;
#endif

// Constants for enum CSValueStringConstants
#ifndef _CSValueStringConstants_
#define _CSValueStringConstants_
typedef enum _CSValueStringConstants {
    csvsProjectionXCoordinate = 0x00000000,
    csvsProjectionYCoordinate = 0x00000001,
    csvsLongitude = 0x00000002,
    csvsLatitude = 0x00000003,
    csvsGeocentricXCoordinate = 0x00000004,
    csvsGeocentricYCoordinate = 0x00000005,
    csvsGeocentricZCoordinate = 0x00000006,
    csvsPaperXCoordinate = 0x00000007,
    csvsPaperYCoordinate = 0x00000008,
    csvsHeight = 0x00000009,
    csvsPaperHeight = 0x0000000A,
    csvsZHeight = 0x0000000B,
    csvsDistance = 0x0000000C,
    csvsPaperDistance = 0x0000000D,
    csvsAngle = 0x0000000E,
    csvsAzimuth = 0x0000000F,
    csvsArea = 0x00000010,
    csvsPaperArea = 0x00000011,
    csvsVolume = 0x00000012,
    csvsPaperVolume = 0x00000013,
    csvsTime = 0x00000014,
    csvsBearing = 0x00000015
} CSValueStringConstants;
#endif

// Constants for enum CSPointStringConstants
#ifndef _CSPointStringConstants_
#define _CSPointStringConstants_
typedef enum _CSPointStringConstants {
    cspsGeographic = 0x00000000,
    cspsProjectionPoint = 0x00000001,
    cspsGeocentric = 0x00000002,
    cspsPaper = 0x00000003,
    cspsLocalSpaceRectangular = 0x00000004
} CSPointStringConstants;
#endif

// Constants for enum CSDescriptionConstants
#ifndef _CSDescriptionConstants_
#define _CSDescriptionConstants_
typedef enum _CSDescriptionConstants {
    csdOnInput = 0x00000000,
    csdOnOutput = 0x00000001
} CSDescriptionConstants;
#endif

// Constants for enum CSAzimuthStartConstants
#ifndef _CSAzimuthStartConstants_
#define _CSAzimuthStartConstants_
typedef enum _CSAzimuthStartConstants {
    csasNorth = 0x00000000,
    csasSouth = 0x00000001,
    csasEast = 0x00000002,
    csasWest = 0x00000003
} CSAzimuthStartConstants;
#endif

// Constants for enum CSAzimuthDirectionConstants
#ifndef _CSAzimuthDirectionConstants_
#define _CSAzimuthDirectionConstants_
typedef enum _CSAzimuthDirectionConstants {
    csadClockwise = 0x00000000,
    csadCounterClockwise = 0x00000001
} CSAzimuthDirectionConstants;
#endif

// Constants for enum CSSReferenceSpaceConstants
#ifndef _CSSReferenceSpaceConstants_
#define _CSSReferenceSpaceConstants_
typedef enum _CSSReferenceSpaceConstants {
    gmcssGeographic = 0x00000000,
    gmcssProjection = 0x00000001,
    gmcssPaper = 0x00000002
} CSSReferenceSpaceConstants;
#endif

// Constants for enum CSPointConstants
#ifndef _CSPointConstants_
#define _CSPointConstants_
typedef enum _CSPointConstants {
    cspENU = 0x00000000,
    cspENO = 0x00000001,
    cspENG = 0x00000002,
    cspLLU = 0x00000003,
    cspLLO = 0x00000004,
    cspLLG = 0x00000005,
    cspPAPU = 0x00000006,
    cspPAPO = 0x00000007,
    cspPAPG = 0x00000008,
    cspXYZU = 0x00000009,
    cspXYZO = 0x0000000A,
    cspXYZG = 0x0000000B,
    cspUOR = 0x0000000C
} CSPointConstants;
#endif

// Constants for enum CSBaseStorageConstants
#ifndef _CSBaseStorageConstants_
#define _CSBaseStorageConstants_
typedef enum _CSBaseStorageConstants {
    csbsProjected = 0x00000000,
    csbsGeographic = 0x00000001,
    csbsGeocentric = 0x00000002
} CSBaseStorageConstants;
#endif

// Constants for enum CSHeightStorageConstants
#ifndef _CSHeightStorageConstants_
#define _CSHeightStorageConstants_
typedef enum _CSHeightStorageConstants {
    cshStorageOrthometric = 0x00000000,
    cshStorageGeometric = 0x00000001,
    cshStorageUnspecified = 0x00000002
} CSHeightStorageConstants;
#endif

// Constants for enum CSDatumTransformationModelConstants
#ifndef _CSDatumTransformationModelConstants_
#define _CSDatumTransformationModelConstants_
typedef enum _CSDatumTransformationModelConstants {
    csdtUndefined = 0x00000000,
    csdtMultipleRegression = 0x00000002,
    csdtNADCONforNAD27toNAD83 = 0x00000003,
    csdtNADCONforNAD83toHARN = 0x00000009,
    csdtCanadianNational = 0x00000008,
    csdtComplexPolynomial = 0x00000005,
    csdtBursaWolf = 0x00000001,
    csdtStandardMolodensky = 0x00000004,
    csdt2ndDegConformalPolynomial = 0x00000006,
    csdt2ndDegGeneralPolynomial = 0x00000007,
    csdtUserSupplied = 0x0000000A,
    csdtVERTCON = 0x0000000B,
    csdtNGAEGM = 0x0000000C,
    csdtNGSGEOID = 0x0000000D,
    csdtNGSUSGG = 0x0000000E
} CSDatumTransformationModelConstants;
#endif

// Constants for enum CSDatumTransformationDirectionConstants
#ifndef _CSDatumTransformationDirectionConstants_
#define _CSDatumTransformationDirectionConstants_
typedef enum _CSDatumTransformationDirectionConstants {
    csdtdDatum1toDatum2 = 0x00000000,
    csdtdDatum2toDatum1 = 0x00000001
} CSDatumTransformationDirectionConstants;
#endif

// Constants for enum CSComplexPolynomialModelConstants
#ifndef _CSComplexPolynomialModelConstants_
#define _CSComplexPolynomialModelConstants_
typedef enum _CSComplexPolynomialModelConstants {
    cscpmDenmarkJyllandToUTMzone32 = 0x00000004,
    cscpmDenmarkSjaellandToUTMzone32 = 0x00000005,
    cscpmDenmarkBornholmToUTMzone33 = 0x00000006,
    cscpmNZealandAmuriCircToNZMG = 0x00000007,
    cscpmNZealandBayOfPlentyCircToNZMG = 0x00000008,
    cscpmNZealandBluffCircToNZMG = 0x00000009,
    cscpmNZealandBullerCircToNZMG = 0x0000000A,
    cscpmNZealandCollingwoodCircToNZMG = 0x0000000B,
    cscpmNZealandGawlerCircToNZMG = 0x0000000C,
    cscpmNZealandGreyCircToNZMG = 0x0000000D,
    cscpmNZealandHawkesBayCircToNZMG = 0x0000000E,
    cscpmNZealandHokitikaCircToNZMG = 0x0000000F,
    cscpmNZealandJacksonsBayCircToNZMG = 0x00000010,
    cscpmNZealandKarameaCircToNZMG = 0x00000011,
    cscpmNZealandLindisPeakCircToNZMG = 0x00000012,
    cscpmNZealandMarlboroughCircToNZMG = 0x00000013,
    cscpmNZealandMtEdenCircToNZMG = 0x00000014,
    cscpmNZealandMtNicholasCircToNZMG = 0x00000015,
    cscpmNZealandMtPleasantCircToNZMG = 0x00000016,
    cscpmNZealandMtYorkCircToNZMG = 0x00000017,
    cscpmNZealandNelsonCircToNZMG = 0x00000018,
    cscpmNZealandNorthIslandToNZMG = 0x00000019,
    cscpmNZealandNorthTaieriCircToNZMG = 0x0000001A,
    cscpmNZealandObservationPtCircToNZMG = 0x0000001B,
    cscpmNZealandOkaritoCircToNZMG = 0x0000001C,
    cscpmNZealandPovertyBayCircToNZMG = 0x0000001D,
    cscpmNZealandSouthIslandToNZMG = 0x0000001E,
    cscpmNZealandTaranakiCircToNZMG = 0x0000001F,
    cscpmNZealandTimaruCircToNZMG = 0x00000020,
    cscpmNZealandTuhirangiCircToNZMG = 0x00000021,
    cscpmNZealandWairarapaCircToNZMG = 0x00000022,
    cscpmNZealandWanganuiCircToNZMG = 0x00000023,
    cscpmNZealandWellingtonCircToNZMG = 0x00000024,
    cscpmSwedenRT38ToUTMzone32 = 0x00000001,
    cscpmSwedishRT38ToUTMzone32 = 0x00000001,
    cscpmSwedenRT38ToUTMzone33 = 0x00000002,
    cscpmSwedishRT38ToUTMzone33 = 0x00000002,
    cscpmSwedenRT38ToUTMzone34 = 0x00000003,
    cscpmSwedishRT38ToUTMzone34 = 0x00000003,
    cscpmUndefined = 0x00000000
} CSComplexPolynomialModelConstants;
#endif

// Constants for enum CSMultipleRegressionModelConstants
#ifndef _CSMultipleRegressionModelConstants_
#define _CSMultipleRegressionModelConstants_
typedef enum _CSMultipleRegressionModelConstants {
    csmrmAdindanDatumEthiopiaSudan = 0x00000009,
    csmrmAdindanDatumMali = 0x00000036,
    csmrmAdindanDatumSenegal = 0x00000037,
    csmrmAfgooyeDatumSomalia = 0x00000038,
    csmrmAinElAbd1970Bahrain = 0x00000039,
    csmrmAnna1Astro1965CocosIslands = 0x0000003A,
    csmrmArcDatum1950SouthCentralAfrica = 0x0000000A,
    csmrmArcDatum1960KenyaTanzania = 0x0000000B,
    csmrmAscensionIsland1958 = 0x0000003B,
    csmrmAstroB4SorolAtollTernIsland = 0x0000003D,
    csmrmAstroBeaconEIwoJimaIsland = 0x0000003C,
    csmrmAstroDos71_4StHelenaIsland = 0x0000003E,
    csmrmAstronomicStation1952MarcusIsland = 0x0000003F,
    csmrmAustralianGeodeticDatum1966 = 0x00000006,
    csmrmAustralianGeodeticDatum1984 = 0x00000007,
    csmrmBellevueDatumEfateErromangaIs = 0x0000000C,
    csmrmBermudaDatum1957 = 0x0000000D,
    csmrmBogotaObservatoryDatumColombia = 0x0000000E,
    csmrmCampoInchauspeDatumArgentina = 0x0000000F,
    csmrmCantonAstroDatum1966PhoenixIs = 0x00000010,
    csmrmCapeCanaveralDatumFloridaBahamas = 0x00000012,
    csmrmCapeDatumSouthAfrica = 0x00000011,
    csmrmCarthageDatumTunisia = 0x00000013,
    csmrmChathamDatum1971ChathamIs = 0x00000014,
    csmrmChuaAstroDatumParaguay = 0x00000015,
    csmrmCorregoAlegreDatumBrazil = 0x00000016,
    csmrmDjakartaDatumSumatra = 0x00000017,
    csmrmDOS1968GizoIsland = 0x00000040,
    csmrmEasterIsland1967 = 0x00000041,
    csmrmEuropean1950Cyprus = 0x00000018,
    csmrmEuropean1950Egypt = 0x00000019,
    csmrmEuropean1950Iran = 0x0000001A,
    csmrmEuropean1950UKIreland = 0x00000027,
    csmrmEuropean1950UnitedKingdomOnly = 0x00000028,
    csmrmEuropeanDatum1950Sardinia = 0x00000042,
    csmrmEuropeanDatum1950Sicily = 0x00000043,
    csmrmEuropeanDatum1950Western = 0x00000008,
    csmrmGUX1AstroGuadalcanal = 0x00000045,
    csmrmGandajikaBaseRepublicOfMaldives = 0x00000044,
    csmrmGeodeticDatum1949NewZealand = 0x0000001B,
    csmrmGuamDatum1963 = 0x0000001C,
    csmrmHjorseyDatum1955Iceland = 0x0000001D,
    csmrmHongKong1963 = 0x00000046,
    csmrmISTS073Astro1969DiegoGarcia = 0x00000048,
    csmrmIndianDatumBangladeshIndiaNepal = 0x00000047,
    csmrmIndianDatumThailandVietnam = 0x0000001E,
    csmrmIrelandDatum1965 = 0x0000001F,
    csmrmJohnstonIsland1961 = 0x00000049,
    csmrmKandawalaDatumSriLanka = 0x00000020,
    csmrmKerguelenIsland = 0x0000004A,
    csmrmKertauDatum1948WestMalaysiaSing = 0x00000021,
    csmrmLC5AstroCaymanBrac = 0x0000004B,
    csmrmLiberiaDatum1964 = 0x00000022,
    csmrmLuzonMindanaoIsland = 0x0000004C,
    csmrmLuzonDatumPhilippines = 0x00000023,
    csmrmMahe1971MaheIsland = 0x0000004D,
    csmrmMarcoAstroSalvageIslands = 0x0000004E,
    csmrmMassawaEritreaEthiopia = 0x0000004F,
    csmrmMerchichDatumMorocco = 0x00000024,
    csmrmMidwayAstro1961MidwayIsland = 0x00000050,
    csmrmMinnaDatumNigeria = 0x00000025,
    csmrmNahrwanSaudiaArabia = 0x00000051,
    csmrmNahrwanDatumMasirahUnitedArabEm = 0x00000026,
    csmrmNaparimaBWITrinidadAndTobago = 0x00000052,
    csmrmNorthAmerican1927Greenland = 0x00000053,
    csmrmNorthAmericanDatum1927Alaska = 0x00000002,
    csmrmNorthAmericanDatum1927Canada = 0x00000003,
    csmrmNorthAmericanDatum1927Caribbean = 0x00000029,
    csmrmNorthAmericanDatum1927MexCAm = 0x0000002A,
    csmrmNorthAmericanDatum1927lower48 = 0x00000001,
    csmrmObservatorio1966CorvoFloresIslands = 0x00000054,
    csmrmOldEgyptianDatum = 0x0000002B,
    csmrmOldHawaiianDatum = 0x0000002C,
    csmrmOmanDatum = 0x0000002D,
    csmrmOrdnanceSurveyOfGreatBritain1936 = 0x0000002E,
    csmrmPicoDeLasNievesCanaryIslands = 0x00000055,
    csmrmPitcairnAstro1967PitcairnIsland = 0x00000056,
    csmrmProvisionalSouthAmericanDatum = 0x00000004,
    csmrmProvisionalSouthChilean1963 = 0x00000057,
    csmrmPuertoRicoDatumWithVirginIs = 0x00000030,
    csmrmQatarNationalDatum = 0x00000031,
    csmrmQornoqSouthGreenland = 0x00000058,
    csmrmReunionMascareneIslands = 0x00000059,
    csmrmRome1940SardiniaIsland = 0x0000005A,
    csmrmSantoDOSEspiritoSantoIsland = 0x0000005B,
    csmrmSaoBrazSaoMiguelSantaMariaIs = 0x0000005C,
    csmrmSapperHill1943EastFalklandIsland = 0x0000005D,
    csmrmSchwarzeckNamibia = 0x0000005E,
    csmrmSouthAmericanDatum1969 = 0x00000005,
    csmrmSouthAsiaSingapore = 0x0000005F,
    csmrmSoutheastBasePortoSantoMadeiraIs = 0x00000060,
    csmrmSouthwestBaseDatumAzores = 0x00000032,
    csmrmTimbalaiDatum1948Brunei = 0x00000033,
    csmrmTokyoDatum = 0x0000002F,
    csmrmTristanAstro1968TristanDaCunha = 0x00000061,
    csmrmVitiLevu1916VitiLevuFiji = 0x00000062,
    csmrmWakeEniwetokDatum1960KwajaleinIs = 0x00000034,
    csmrmZanderijDatumSuriname = 0x00000035,
    csmrmUndefined = 0x00000000
} CSMultipleRegressionModelConstants;
#endif

// Constants for enum CSTransformLinkConstants
#ifndef _CSTransformLinkConstants_
#define _CSTransformLinkConstants_
typedef enum _CSTransformLinkConstants {
    cstlCoordinateSystem = 0x00000000,
    cstlReferenceSpaceMgr = 0x00000001,
    cstlDatumTransformation = 0x00000002,
    cstlLeastSquares = 0x00000003
} CSTransformLinkConstants;
#endif

// Constants for enum CSTransDirectionConstants
#ifndef _CSTransDirectionConstants_
#define _CSTransDirectionConstants_
typedef enum _CSTransDirectionConstants {
    cstdForward = 0x00000000,
    cstdInverse = 0x00000001,
    cstdIgnored = 0x00000002
} CSTransDirectionConstants;
#endif

// Constants for enum UnitTypeConstants
#ifndef _UnitTypeConstants_
#define _UnitTypeConstants_
typedef enum _UnitTypeConstants {
    igUnitDistance = 0x00000001,
    igUnitAngle = 0x00000002,
    igUnitMass = 0x00000003,
    igUnitTime = 0x00000004,
    igUnitTemperature = 0x00000005,
    igUnitElectricCharge = 0x00000006,
    igUnitLuminousIntensity = 0x00000007,
    igUnitAmountOfSubstance = 0x00000008,
    igUnitSolidAngle = 0x00000009,
    igUnitAngularAcceleration = 0x0000000A,
    igUnitAngularMomentum = 0x0000000B,
    igUnitAngularVelocity = 0x0000000C,
    igUnitArea = 0x0000000D,
    igUnitBodyForce = 0x0000000E,
    igUnitCoefficientOfThermalExpansion = 0x0000000F,
    igUnitDensity = 0x00000010,
    igUnitElectricalCapacitance = 0x00000011,
    igUnitElectricalConductance = 0x00000012,
    igUnitElectricalFieldStrength = 0x00000013,
    igUnitElectricalInductance = 0x00000014,
    igUnitElectricalPotential = 0x00000015,
    igUnitElectricalResistance = 0x00000016,
    igUnitEnergy = 0x00000017,
    igUnitEntropy = 0x00000018,
    igUnitFilmCoefficient = 0x00000019,
    igUnitForce = 0x0000001A,
    igUnitForcePerArea = 0x0000001B,
    igUnitForcePerDistance = 0x0000001C,
    igUnitFrequency = 0x0000001D,
    igUnitHeatCapacity = 0x0000001E,
    igUnitHeatFluxPerArea = 0x0000001F,
    igUnitHeatFluxPerDistance = 0x00000020,
    igUnitHeatSource = 0x00000021,
    igUnitIlluminance = 0x00000022,
    igUnitLinearAcceleration = 0x00000023,
    igUnitLinearPerAngular = 0x00000024,
    igUnitLinearVelocity = 0x00000025,
    igUnitLuminousFlux = 0x00000026,
    igUnitMagneticFieldStrength = 0x00000027,
    igUnitMagneticFlux = 0x00000028,
    igUnitMagneticFluxDensity = 0x00000029,
    igUnitMassFlowRate = 0x0000002A,
    igUnitMassMomentOfInertia = 0x0000002B,
    igUnitMassPerArea = 0x0000002C,
    igUnitMassPerLength = 0x0000002D,
    igUnitMomentum = 0x0000002E,
    igUnitPerDistance = 0x0000002F,
    igUnitPower = 0x00000030,
    igUnitQuantityOfElectricity = 0x00000031,
    igUnitRadiantIntensity = 0x00000032,
    igUnitRotationalStiffness = 0x00000033,
    igUnitSecondMomentOfArea = 0x00000034,
    igUnitThermalConductivity = 0x00000035,
    igUnitDynamicViscosity = 0x00000036,
    igUnitKinematicViscosity = 0x00000037,
    igUnitVolume = 0x00000038,
    igUnitVolumeFlowRate = 0x00000039,
    igUnitScalar = 0x0000003A
} UnitTypeConstants;
#endif

// Constants for enum LeastSquaresModelConstants
#ifndef _LeastSquaresModelConstants_
#define _LeastSquaresModelConstants_
typedef enum _LeastSquaresModelConstants {
    cslsmUndefined = 0x00000000,
    cslsmAffine = 0x00000001,
    cslsmGeometricallyInterpretedAffine = 0x00000002,
    cslsmHelmert = 0x00000003,
    cslsmLinearProjective = 0x00000004,
    cslsmProjective = 0x00000005,
    cslsmLinear2Dim2ndDegPolynomial = 0x00000006,
    cslsmLinear2DimNDegPolynomial = 0x00000007,
    cslsmLinear2DimNDegConformalPolynomial = 0x00000008,
    cslsm2DimNDegPolynomial = 0x00000009,
    cslsm2DimNDegConformalPolynomial = 0x0000000A,
    cslsmLinear3DimNDegPolynomial = 0x0000000B,
    cslsm3DimNDegPolynomial = 0x0000000C,
    cslsm7ParameterConformal = 0x0000000D,
    cslsmBursaWolf = 0x0000000E
} LeastSquaresModelConstants;
#endif

// Constants for enum LeastSquaresWeightsUsageConstants
#ifndef _LeastSquaresWeightsUsageConstants_
#define _LeastSquaresWeightsUsageConstants_
typedef enum _LeastSquaresWeightsUsageConstants {
    cslswNone = 0x00000000,
    cslswSystem1Only = 0x00000001,
    cslswSystem2Only = 0x00000002,
    cslswSystem1AndSystem2 = 0x00000003
} LeastSquaresWeightsUsageConstants;
#endif

// Constants for enum LeastSquaresHomogeneousMatrixConstants
#ifndef _LeastSquaresHomogeneousMatrixConstants_
#define _LeastSquaresHomogeneousMatrixConstants_
typedef enum _LeastSquaresHomogeneousMatrixConstants {
    cslshmRightHanded = 0x00000000,
    cslshmLeftHanded = 0x00000001
} LeastSquaresHomogeneousMatrixConstants;
#endif

// Constants for enum LeastSquaresObservationConstants
#ifndef _LeastSquaresObservationConstants_
#define _LeastSquaresObservationConstants_
typedef enum _LeastSquaresObservationConstants {
    cslsoCheckPoint = 0x00000000,
    cslsoControlPoint = 0x00000001
} LeastSquaresObservationConstants;
#endif

typedef struct _CSSpoint {
    double X;
    double Y;
    double Z;
} CSSpoint, *PCSSpoint;

// Constants for enum CSUnitConstants
#ifndef _CSUnitConstants_
#define _CSUnitConstants_
typedef enum _CSUnitConstants {
    csuProjectionCoordinate = 0x00000000,
    csuGeographicCoordinate = 0x00000001,
    csuGeocentricCoordinate = 0x00000002,
    csuPaperCoordinate = 0x00000003,
    csuHeight = 0x00000004,
    csuPaperHeight = 0x00000005,
    csuZHeight = 0x00000006,
    csuDistance = 0x00000007,
    csuPaperDistance = 0x00000008,
    csuAngle = 0x00000009,
    csuAzimuth = 0x0000000A,
    csuArea = 0x0000000B,
    csuPaperArea = 0x0000000C,
    csuVolume = 0x0000000D,
    csuPaperVolume = 0x0000000E,
    csuTime = 0x0000000F,
    csuBearing = 0x00000010
} CSUnitConstants;
#endif

typedef struct _CSStransDesc {
    long InputPointType;
    long InputSpaceID;
    long OutputPointType;
    long OutputSpaceID;
} CSStransDesc, *PCSStransDesc;

// Constants for enum UnitConstants
#ifndef _UnitConstants_
#define _UnitConstants_
typedef enum _UnitConstants {
    igDistanceMeter = 0x0000003B,
    igDistanceNanometer = 0x0000003C,
    igDistanceMillimeter = 0x0000003D,
    igDistanceCentimeter = 0x0000003E,
    igDistanceKilometer = 0x0000003F,
    igDistanceInch = 0x00000040,
    igDistanceFoot = 0x00000041,
    igDistanceYard = 0x00000042,
    igDistanceMile = 0x00000043,
    igDistanceTenth = 0x00000044,
    igDistanceHundredth = 0x00000045,
    igDistanceThousandth = 0x00000046,
    igDistanceRod = 0x00000047,
    igDistancePole = 0x00000048,
    igDistanceLink = 0x00000049,
    igDistanceChain = 0x0000004A,
    igDistancePoint = 0x0000004B,
    igDistanceFurlong = 0x0000004C,
    igAngleRadian = 0x0000004D,
    igAngleDegree = 0x0000004E,
    igAngleMinute = 0x0000004F,
    igAngleSecond = 0x00000050,
    igAngleGradian = 0x00000051,
    igAngleRevolution = 0x00000052,
    igMassKilogram = 0x00000053,
    igMassGram = 0x00000054,
    igMassMilligram = 0x00000055,
    igMassMegagram = 0x00000056,
    igMassGrain = 0x00000057,
    igMassSlug = 0x00000058,
    igMassPoundMass = 0x00000059,
    igMassSlinch = 0x0000005A,
    igMassOunce = 0x0000005B,
    igMassMetricTon = 0x0000005C,
    igMassLongTon = 0x0000005D,
    igMassShortTon = 0x0000005E,
    igTimeSecond = 0x0000005F,
    igTimeMinute = 0x00000060,
    igTimeHour = 0x00000061,
    igTimeDay = 0x00000062,
    igTimeWeek = 0x00000063,
    igTimeYear = 0x00000064,
    igTimeMillisecond = 0x00000065,
    igTimeNanosecond = 0x00000066,
    igTemperatureKelvin = 0x00000067,
    igTemperatureFahrenheit = 0x00000068,
    igTemperatureCelcius = 0x00000069,
    igTemperatureRankine = 0x0000006A,
    igElectricChargeAmpere = 0x0000006B,
    igLuminousIntensityCandela = 0x0000006C,
    igAmountOfSubstanceMole = 0x0000006D,
    igSolidAngleSteradian = 0x0000006E,
    igAngularAccelerationRadianPerSqrSecond = 0x0000006F,
    igAngularAccelerationDegreePerSqrSecond = 0x00000070,
    igAngularAccelerationCyclePerSqrSecond = 0x00000071,
    igAngularMomentumKilogramSqrMeterPerSecond = 0x00000072,
    igAngularVelocityRadianPerSecond = 0x00000073,
    igAngularVelocityRadianPerMinute = 0x00000074,
    igAngularVelocityRadianPerHour = 0x00000075,
    igAngularVelocityCyclePerSecond = 0x00000076,
    igAngularVelocityCyclePerMinute = 0x00000077,
    igAngularVelocityCyclePerHour = 0x00000078,
    igAngularVelocityDegreePerSecond = 0x00000079,
    igAngularVelocityDegreePerMinute = 0x0000007A,
    igAngularVelocityDegreePerHour = 0x0000007B,
    igAreaSquareMeter = 0x0000007C,
    igAreaSquareMillimeter = 0x0000007D,
    igAreaSquareCentimeter = 0x0000007E,
    igAreaSquareKilometer = 0x0000007F,
    igAreaSquareInch = 0x00000080,
    igAreaSquareFoot = 0x00000081,
    igAreaSquareYard = 0x00000082,
    igAreaSquareMile = 0x00000083,
    igAreaAcre = 0x00000084,
    igBodyForceNewtonPerCubicMeter = 0x00000085,
    igBodyForceNewtonPerCubicCentimeter = 0x00000086,
    igBodyForceNewtonPerCubicMillimeter = 0x00000087,
    igBodyForceKilonewtonPerCubicMeter = 0x00000088,
    igBodyForcePoundForcePerCubicInch = 0x00000089,
    igBodyForcePoundForcePerCubicFoot = 0x0000008A,
    igCoefficientOfThermalExpansionPerKelvin = 0x0000008B,
    igCoefficientOfThermalExpansionPerCelcius = 0x0000008C,
    igCoefficientOfThermalExpansionPerFahrenheit = 0x0000008D,
    igCoefficientOfThermalExpansionPerRankine = 0x0000008E,
    igDensityKilogramPerCubicMeter = 0x0000008F,
    igDensityKilogramPerCubicMillimeter = 0x00000090,
    igDensityKilogramPerCubicCentimeter = 0x00000091,
    igDensityPoundMassPerCubicFoot = 0x00000092,
    igDensityPoundMassPerCubicInch = 0x00000093,
    igDensitySlugPerCubicFoot = 0x00000094,
    igDensitySlinchPerCubicInch = 0x00000095,
    igElectricalCapacitanceFarad = 0x00000096,
    igElectricalConductanceSiemens = 0x00000097,
    igElectricalFieldStrengthVoltPerMeter = 0x00000098,
    igElectricalPotentialVolt = 0x00000099,
    igElectricalResistanceOhm = 0x0000009A,
    igEnergyJoule = 0x0000009B,
    igEnergyMicrojoule = 0x0000009C,
    igEnergyKilojoule = 0x0000009D,
    igEnergyWattSecond = 0x0000009E,
    igEnergyWattHour = 0x0000009F,
    igEnergyKilowattHour = 0x000000A0,
    igEnergyInchPoundForce = 0x000000A1,
    igEnergyFootPoundForce = 0x000000A2,
    igEnergyBtu = 0x000000A3,
    igEnergyErg = 0x000000A4,
    igEnergyElectronVolt = 0x000000A5,
    igEntropyJoulePerKelvin = 0x000000A6,
    igFilmCoefficientWattPerSqrMeterKelvin = 0x000000A7,
    igFilmCoefficientBtuPerHourSqrFootFahrenheit = 0x000000A8,
    igFilmCoefficientFootPoundPerSqrFootFahrenheit = 0x000000A9,
    igFilmCoefficientInchPoundPerSqrInchFahrenheit = 0x000000AA,
    igForceNewton = 0x000000AB,
    igForceNanonewton = 0x000000AC,
    igForceMillinewton = 0x000000AD,
    igForceKilonewton = 0x000000AE,
    igForcePoundForce = 0x000000AF,
    igForceDyne = 0x000000B0,
    igForceKip = 0x000000B1,
    igForcePerAreaPascal = 0x000000B2,
    igForcePerAreaMillipascal = 0x000000B3,
    igForcePerAreaKilopascal = 0x000000B4,
    igForcePerAreaMegapascal = 0x000000B5,
    igForcePerAreaPoundForcePerSqrInch = 0x000000B6,
    igForcePerAreaKipPerSqrInch = 0x000000B7,
    igForcePerAreaPoundForcePerSqrFoot = 0x000000B8,
    igForcePerAreaKipPerSqrFoot = 0x000000B9,
    igForcePerAreaBar = 0x000000BA,
    igForcePerAreaMillibar = 0x000000BB,
    igForcePerAreaAtmosphere = 0x000000BC,
    igForcePerDistanceNewtonPerMeter = 0x000000BD,
    igForcePerDistanceNewtonPerMillimeter = 0x000000BE,
    igForcePerDistanceNewtonPerCentimeter = 0x000000BF,
    igForcePerDistanceKilonewtonPerMeter = 0x000000C0,
    igForcePerDistancePoundForcePerInch = 0x000000C1,
    igForcePerDistancePoundForcePerFoot = 0x000000C2,
    igForcePerDistanceKipPerInch = 0x000000C3,
    igForcePerDistanceKipPerFoot = 0x000000C4,
    igForcePerDistanceDynePerMeter = 0x000000C5,
    igForcePerDistanceDynePerMillimeter = 0x000000C6,
    igFrequencyPerSecond = 0x000000C7,
    igFrequencyPerMinute = 0x000000C8,
    igFrequencyPerHour = 0x000000C9,
    igHeatCapacityJoulePerKilogramKelvin = 0x000000CA,
    igHeatCapacityBtuPerPoundMassFahrenheit = 0x000000CB,
    igHeatFluxPerAreaWattPerSqrMeter = 0x000000CC,
    igHeatFluxPerAreaKilowattPerSqrMeter = 0x000000CD,
    igHeatFluxPerAreaInchPoundForcePerSecondSqrInch = 0x000000CE,
    igHeatFluxPerAreaFootPoundForcePerSecondSqrFoot = 0x000000CF,
    igHeatFluxPerAreaBtuPerHourSqrFoot = 0x000000D0,
    igHeatFluxPerAreaHorsepowerPerSqrFoot = 0x000000D1,
    igHeatFluxPerDistanceWattPerMeter = 0x000000D2,
    igHeatFluxPerDistanceKilowattPerMeter = 0x000000D3,
    igHeatFluxPerDistanceMicrowattPerMeter = 0x000000D4,
    igHeatFluxPerDistanceMicrowattPerMillimeter = 0x000000D5,
    igHeatFluxPerDistanceInchPoundForcePerSecondInch = 0x000000D6,
    igHeatFluxPerDistanceBtuPerHourFoot = 0x000000D7,
    igHeatFluxPerDistanceHorsepowerPerFoot = 0x000000D8,
    igHeatSourceWattPerCubicMeter = 0x000000D9,
    igHeatSourceKilowattPerCubicMeter = 0x000000DA,
    igHeatSourceInchPoundForcePerSecondCubicInch = 0x000000DB,
    igHeatSourceFootPoundForcePerSecondCubicFoot = 0x000000DC,
    igHeatSourceBtuPerHourCubicFoot = 0x000000DD,
    igHeatSourceHorsepowerPerCubicFoot = 0x000000DE,
    igIlluminanceLux = 0x000000DF,
    igLinearAccelerationMeterPerSqrSecond = 0x000000E0,
    igLinearAccelerationKilometerPerSqrSecond = 0x000000E1,
    igLinearAccelerationMillimeterPerSqrSecond = 0x000000E2,
    igLinearAccelerationCentimeterPerSqrSecond = 0x000000E3,
    igLinearAccelerationInchPerSqrSecond = 0x000000E4,
    igLinearAccelerationFootPerSqrSecond = 0x000000E5,
    igLinearAccelerationMilePerSqrSecond = 0x000000E6,
    igLinearAccelerationMilePerSqrHour = 0x000000E7,
    igLinearAccelerationKilometerPerSqrHour = 0x000000E8,
    igLinearPerAngularMeterPerRadian = 0x000000E9,
    igLinearPerAngularMeterPerRevolution = 0x000000EA,
    igLinearPerAngularMillimeterPerRevolution = 0x000000EB,
    igLinearPerAngularInchPerRevolution = 0x000000EC,
    igLinearPerAngularFootPerRevolution = 0x000000ED,
    igLinearVelocityMeterPerSecond = 0x000000EE,
    igLinearVelocityMillimeterPerSecond = 0x000000EF,
    igLinearVelocityCentimeterPerSecond = 0x000000F0,
    igLinearVelocityKilometerPerSecond = 0x000000F1,
    igLinearVelocityKilometerPerHour = 0x000000F2,
    igLinearVelocityInchPerSecond = 0x000000F3,
    igLinearVelocityFootPerSecond = 0x000000F4,
    igLinearVelocityMilePerSecond = 0x000000F5,
    igLinearVelocityMilePerHour = 0x000000F6,
    igLuminousFluxLumen = 0x000000F7,
    igMagneticFieldStrengthAmperePerMeter = 0x000000F8,
    igMagneticFluxWeber = 0x000000F9,
    igMagneticFluxDensityTesla = 0x000000FA,
    igMassFlowRateKilogramPerSecond = 0x000000FB,
    igMassFlowRateGramPerSecond = 0x000000FC,
    igMassFlowRateMegagramPerSecond = 0x000000FD,
    igMassFlowRatePoundMassPerSecond = 0x000000FE,
    igMassFlowRateSlugPerSecond = 0x000000FF,
    igMassFlowRateSlinchPerSecond = 0x00000100,
    igMassMomentOfInertiaKilogramSqrMeter = 0x00000101,
    igMassMomentOfInertiaKilogramSqrMillimeter = 0x00000102,
    igMassMomentOfInertiaGramSqrMillimeter = 0x00000103,
    igMassMomentOfInertiaGramSqrCentimeter = 0x00000104,
    igMassMomentOfInertiaSlugSqrFoot = 0x00000105,
    igMassMomentOfInertiaSlugSqrInch = 0x00000106,
    igMassMomentOfInertiaSlinchSqrInch = 0x00000107,
    igMassMomentOfInertiaPoundMassSqrInch = 0x00000108,
    igMassMomentOfInertiaPoundMassSqrFoot = 0x00000109,
    igMassPerAreaKilogramPerSqrMeter = 0x0000010A,
    igMassPerAreaKilogramPerSqrMillimeter = 0x0000010B,
    igMassPerAreaGramPerSqrMillimeter = 0x0000010C,
    igMassPerAreaGramPerSqrCentimeter = 0x0000010D,
    igMassPerAreaSlugPerSqrInch = 0x0000010E,
    igMassPerAreaSlugPerSqrFoot = 0x0000010F,
    igMassPerAreaSlinchPerSqrInch = 0x00000110,
    igMassPerAreaPoundMassPerSqrInch = 0x00000111,
    igMassPerAreaPoundMassPerSqrFoot = 0x00000112,
    igMassPerLengthKilogramPerMeter = 0x00000113,
    igMassPerLengthKilogramPerMillimeter = 0x00000114,
    igMassPerLengthGramPerMillimeter = 0x00000115,
    igMassPerLengthGramPerCentimeter = 0x00000116,
    igMassPerLengthSlugPerInch = 0x00000117,
    igMassPerLengthSlugPerFoot = 0x00000118,
    igMassPerLengthSlinchPerInch = 0x00000119,
    igMassPerLengthPoundMassPerInch = 0x0000011A,
    igMassPerLengthPoundMassPerFoot = 0x0000011B,
    igMomentumKilogramMeterPerSecond = 0x0000011C,
    igPerDistancePerMeter = 0x0000011D,
    igPerDistancePerNanometer = 0x0000011E,
    igPerDistancePerMillimeter = 0x0000011F,
    igPerDistancePerCentimeter = 0x00000120,
    igPerDistancePerKilometer = 0x00000121,
    igPerDistancePerInch = 0x00000122,
    igPerDistancePerFoot = 0x00000123,
    igPerDistancePerYard = 0x00000124,
    igPerDistancePerMile = 0x00000125,
    igPerDistancePerTenth = 0x00000126,
    igPerDistancePerHundredth = 0x00000127,
    igPerDistancePerMil = 0x00000128,
    igPowerWatt = 0x00000129,
    igPowerKilowatt = 0x0000012A,
    igPowerMicrowatt = 0x0000012B,
    igPowerInchPoundForcePerSecond = 0x0000012C,
    igPowerFootPoundForcePerSecond = 0x0000012D,
    igPowerBtuPerHour = 0x0000012E,
    igPowerHorsepower = 0x0000012F,
    igQuantityOfElectricityCoulomb = 0x00000130,
    igRadiantIntensityWattPerSteradian = 0x00000131,
    igRotationalStiffnessNewtonMeterPerRadian = 0x00000132,
    igRotationalStiffnessNewtonMeterPerDegree = 0x00000133,
    igRotationalStiffnessNewtonMillimeterPerRadian = 0x00000134,
    igRotationalStiffnessNewtonMillimeterPerDegree = 0x00000135,
    igRotationalStiffnessKilonewtonMeterPerRadian = 0x00000136,
    igRotationalStiffnessKilonewtonMeterPerDegree = 0x00000137,
    igRotationalStiffnessPoundForceInchPerRadian = 0x00000138,
    igRotationalStiffnessPoundForceInchPerDegree = 0x00000139,
    igRotationalStiffnessPoundForceFootPerRadian = 0x0000013A,
    igRotationalStiffnessPoundForceFootPerDegree = 0x0000013B,
    igSecondMomentOfAreaMeterToFourth = 0x0000013C,
    igSecondMomentOfAreaMillimeterToFourth = 0x0000013D,
    igSecondMomentOfAreaCentimeterToFourth = 0x0000013E,
    igSecondMomentOfAreaInchToFourth = 0x0000013F,
    igSecondMomentOfAreaFootToFourth = 0x00000140,
    igThermalConductivityWattPerMeterCelcius = 0x00000141,
    igThermalConductivityKilowattPerMeterCelcius = 0x00000142,
    igThermalConductivityBtuPerHourFahrenheit = 0x00000143,
    igThermalConductivityInchPoundForcePerSecondInchFahrenheit = 0x00000144,
    igDynamicViscosityPascalSecond = 0x00000145,
    igDynamicViscosityKilopascalSecond = 0x00000146,
    igDynamicViscosityMegapascalSecond = 0x00000147,
    igDynamicViscosityPoundForceSecondPerSqrInch = 0x00000148,
    igKinematicViscositySqrMeterPerSecond = 0x00000149,
    igKinematicViscositySqrMillimeterPerSecond = 0x0000014A,
    igKinematicViscositySqrCentimeterPerSecond = 0x0000014B,
    igKinematicViscositySqrInchPerSecond = 0x0000014C,
    igKinematicViscositySqrFootPerSecond = 0x0000014D,
    igVolumeCubicMeter = 0x0000014E,
    igVolumeCubicMillimeter = 0x0000014F,
    igVolumeLiter = 0x00000150,
    igVolumeMilliliter = 0x00000151,
    igVolumeCubicInch = 0x00000152,
    igVolumeCubicFoot = 0x00000153,
    igVolumeCubicYard = 0x00000154,
    igVolumeGallon = 0x00000155,
    igVolumeQuart = 0x00000156,
    igVolumePint = 0x00000157,
    igVolumeFluidOunce = 0x00000158,
    igScalarNull = 0x0000015A,
    igVolumeFlowRateCubicMeterPerSecond = 0x0000015B,
    igVolumeFlowRateCubicMillimeterPerSecond = 0x0000015C,
    igVolumeFlowRateLiterPerSecond = 0x0000015D,
    igVolumeFlowRateMilliliterPerSecond = 0x0000015E,
    igVolumeFlowRateCubicInchPerSecond = 0x0000015F,
    igVolumeFlowRateCubicFootPerSecond = 0x00000160,
    igVolumeFlowRateCubicYardPerSecond = 0x00000161,
    igVolumeFlowRateGallonPerSecond = 0x00000162,
    igVolumeFlowRateGallonPerMinute = 0x00000163,
    igElectricalInductanceHenry = 0x00000164,
    igAngleDegreeSymbol = 0x00000165,
    igAngleMinuteSymbol = 0x00000166,
    igAngleSecondSymbol = 0x00000167,
    igDistanceFootSymbol = 0x00000168,
    igDistanceInchSymbol = 0x00000169,
    igAreaHectare = 0x0000016A,
    igForcePerAreaNewtonPerSqrMillimeter = 0x0000016B,
    igDynamicViscosityPoise = 0x0000016C,
    igDynamicViscosityCentipoise = 0x0000016D,
    igVolumeCubicCentimeter = 0x0000016E,
    igAngleDegreesMinutesSeconds = 0x00010000,
    igAngleHPFormat = 0x00010001,
    igDistanceSurveyInchTwoCharacters = 0x00010002,
    igDistanceSurveyKilofootTwoCharacters = 0x00010003,
    igDistanceSurveyFootTwoCharacters = 0x00010004,
    igDistanceNauticalMile = 0x00010005,
    igDistanceKilofoot = 0x00010006,
    igDistanceSurveyInch = 0x00010007,
    igDistanceSurveyFoot = 0x00010008,
    igDistanceSurveyKilofoot = 0x00010009,
    igAreaDeciare = 0x0001000A,
    igAreaAres = 0x0001000B,
    igAreaCentare = 0x0001000C,
    igAreaSquareRod = 0x0001000D,
    igAreaPerch = 0x0001000E,
    igAreaSquareChain = 0x0001000F,
    igAreaSquareLink = 0x00010010,
    igAngleDegreeMinutes = 0x00010011
} UnitConstants;
#endif

typedef struct _CSSedge {
    CSStransDesc TransDesc;
    double Cost;
} CSSedge, *PCSSedge;

// Constants for enum MatrixConstants
#ifndef _MatrixConstants_
#define _MatrixConstants_
typedef enum _MatrixConstants {
    gmmIdentity = 0x00000001,
    gmmScale = 0x00000002,
    gmmTranslation = 0x00000003,
    gmmScalePlusTranslation = 0x00000004,
    gmmTwoByTwo = 0x00000005,
    gmmTwoByTwoPlusTranslation = 0x00000006,
    gmmGeneral = 0x00000007,
    gmmGeneralMinusProjAndOverallScale = 0x00000008
} MatrixConstants;
#endif
// *********************************************************************//
// Forward declaration of types defined in TypeLibrary
// *********************************************************************//

struct IGMProjAlgorithmStrings;
struct ProjAlgorithmStrings;
struct IGMGeodeticDatumStrings;
struct GeodeticDatumStrings;
struct IGMEllipsoidStrings;
struct EllipsoidStrings;
struct IGMProjSpace;
struct ProjSpace;
struct IGMProjSpace2;
struct IGMProjSpace3;
struct IGMProjSpace4;
struct IGMGeogSpace;
struct GeogSpace;
struct IGMGeogSpace2;
struct DGMStringCollection;
struct IGMUnitFormatSpec;
struct UnitFormatSpec;
struct IGMUnitFormatSpec2;
struct IGMPaperSpace;
struct PaperSpace;
struct IGMRefSpaceMgr;
struct RefSpaceMgr;
struct IGMRefSpaceMgr2;
struct IGMCoordSystem;
struct DGMCoordSystem;
struct IGMCoordSystem2;
struct IGMCoordSystem3;
struct DGMDatumTransformation;
struct DGMBursaWolf;
struct DGMComplexPolynomial;
struct DGMMultipleRegression;
struct DGMSecondDegreePolynomial;
struct DGMStandardMolodensky;
struct IGMLink;
struct Link;
struct IGMLinks;
struct Links;
struct IGMTransLinks;
struct TransLinks;
struct IGMAltCoordSystemPath;
struct AltCoordSystemPath;
struct IGMAltCoordSystemPaths;
struct AltCoordSystemPaths;
struct IGMCoordSystemsMgr;
struct DGMCoordSystemsMgr;
struct IGMUnitIDs;
struct UnitIDs;
struct IGMUnitIDs2;
struct IGMUnitsOfMeasureTables;
struct DGMUnitsOfMeasure;
struct IGMUnitsOfMeasureTables2;
struct DGMLeastSquares;
struct DGMSingleParameterConstraints;
struct DGMSingleParameterConstraint;
struct DGMObservations;
struct DGMObservation;
struct DGMLinearModelResidual;
struct DGMNonlinearModelResidual;
struct UnitAndFormatSpecEvents;
struct DGMDatumTransformation2;
struct DGMDatumTransformation3;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library
// *********************************************************************//

typedef UnitFormatSpec UnitAndFormatSpec;
typedef DGMCoordSystem CoordSystem;
typedef DGMDatumTransformation3 DatumTransformation;
typedef DGMCoordSystemsMgr CoordSystemsMgr;
typedef DGMUnitsOfMeasure UnitsOfMeasure;
typedef DGMBursaWolf BursaWolf;
typedef DGMComplexPolynomial ComplexPolynomial;
typedef DGMMultipleRegression MultipleRegression;
typedef DGMSecondDegreePolynomial SecondDegreePolynomial;
typedef DGMStandardMolodensky StandardMolodensky;
typedef DGMLeastSquares LeastSquares;
typedef DGMSingleParameterConstraints SingleParameterConstraints;
typedef DGMSingleParameterConstraint SingleParameterConstraint;
typedef DGMObservations Observations;
typedef DGMObservation Observation;
typedef DGMLinearModelResidual LinearModelResidual;
typedef DGMNonlinearModelResidual NonlinearModelResidual;
typedef CSSpoint *LPCSSpoint;
typedef long *LPCSUnitConstants;
typedef long *LPCSGeographicOrderConstants;
typedef long *LPCSGeographicHemisphereOptionConstants;
typedef long *LPCSLongitudeNormalizationConstants;
typedef long *LPCSProjectionQuadrantConstants;
typedef long *LPCSAzimuthStartConstants;
typedef long *LPCSAzimuthDirectionConstants;
typedef long *LPCSPointStringConstants;
typedef long *LPCSDescriptionConstants;
typedef long *LPCSHeightDisplayConstants;
typedef long *LPCSHeightDirectionConstants;
typedef long *LPCSValueStringConstants;
typedef long *LPCSNegativeSuppressionLongitudeConstants;
typedef long *LPCSNegativeSuppressionHeightConstants;
typedef long *LPCSPointConstants;
typedef long *LPCSTransDirectionConstants;
typedef CSStransDesc *LPCSStransDesc;
typedef long *LPCSProjectionAlgorithmConstants;
typedef long *LPCSGeographicHemisphereSelectionConstants;
typedef long *LPCSAzimuthDefinitionModeConstants;
typedef long *LPCSGeodeticDatumConstants;
typedef long *LPCSEllipsoidConstants;
typedef long *LPCSVerticalDatumConstants;
typedef long *LPUnitTypeConstants;
typedef long *LPUnitConstants;
typedef long *LPCSBaseStorageConstants;
typedef long *LPCSHeightStorageConstants;
typedef CSSedge *LPCSSedge;
typedef long *LPMatrixConstants;
typedef long *LPCSDatumTransformationModelConstants;
typedef long *LPCSDatumTransformationDirectionConstants;
typedef long *LPCSComplexPolynomialModelConstants;
typedef long *LPCSMultipleRegressionModelConstants;
typedef IGMProjAlgorithmStrings* *LPGMProjAlgorithmStrings;
typedef IGMProjSpace* *LPGMProjSpace;
typedef IGMProjSpace2* *LPGMProjSpace2;
typedef IGMProjSpace3* *LPGMProjSpace3;
typedef IGMProjSpace4* *LPGMProjSpace4;
typedef IGMGeogSpace2* *LPGMGeogSpace2;
typedef IGMUnitFormatSpec* *LPGMUnitFormatSpec;
typedef IGMUnitFormatSpec2* *LPGMUnitFormatSpec2;
typedef IGMPaperSpace* *LPGMPaperSpace;
typedef IGMRefSpaceMgr* *LPGMRefSpaceMgr;
typedef IGMRefSpaceMgr2* *LPGMRefSpaceMgr2;
typedef IGMCoordSystem* *LPGMCoordSystem;
typedef IGMCoordSystem2* *LPGMCoordSystem2;
typedef IGMCoordSystem3* *LPGMCoordSystem3;
typedef IGMLink* *LPGMLink;
typedef IGMLinks* *LPGMLinks;
typedef IGMTransLinks* *LPGMTransLinks;
typedef IGMAltCoordSystemPath* *LPGMAltCoordSystemPath;
typedef IGMAltCoordSystemPaths* *LPGMAltCoordSystemPaths;
typedef IGMCoordSystemsMgr* *LPGMCoordSystemsMgr;
typedef IGMUnitIDs* *LPGMUnitIDs;
typedef IGMUnitIDs2* *LPGMUnitIDs2;
typedef IGMUnitsOfMeasureTables* *LPGMUnitsOfMeasureTables;
typedef IGMUnitsOfMeasureTables2* *LPGMUnitsOfMeasureTables2;
typedef long *LPLeastSquaresModelConstants;
typedef long *LPLeastSquaresWeightsUsageConstants;
typedef long *LPLeastSquaresHomogeneousMatrixConstants;
typedef long *LPLeastSquaresObservationConstants;

struct IGMProjAlgorithmStrings : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *lpCount) = 0;
    virtual HRESULT __stdcall Item(long index, BSTR *lpItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *lplpEnum) = 0;
};

struct ProjAlgorithmStrings : IDispatch
{
    long get_Count();
    LPTSTR Item(long index);
    IUnknown * _NewEnum();
};

struct IGMGeodeticDatumStrings : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall Item(long index, BSTR *pItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *pEnum) = 0;
};

struct GeodeticDatumStrings : IDispatch
{
    long get_Count();
    LPTSTR Item(long index);
    IUnknown * _NewEnum();
};

struct IGMEllipsoidStrings : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall Item(long index, BSTR *pItem) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *pEnum) = 0;
};

struct EllipsoidStrings : IDispatch
{
    long get_Count();
    LPTSTR Item(long index);
    IUnknown * _NewEnum();
};

struct IGMProjSpace : IUnknown
{
    virtual HRESULT __stdcall get_ProjAlgorithmStr(BSTR *pProjAlgorithmStr) = 0;
    virtual HRESULT __stdcall set_ProjAlgorithmStr(BSTR pProjAlgorithmStr) = 0;
    virtual HRESULT __stdcall get_ProjAlgorithmVal(long *pProjAlgorithmVal) = 0;
    virtual HRESULT __stdcall set_ProjAlgorithmVal(long pProjAlgorithmVal) = 0;
    virtual HRESULT __stdcall get_LonOfOrigStr(BSTR *pLonOfOrigStr) = 0;
    virtual HRESULT __stdcall set_LonOfOrigStr(BSTR pLonOfOrigStr) = 0;
    virtual HRESULT __stdcall get_LonOfOrig(double *pLonOfOrig) = 0;
    virtual HRESULT __stdcall set_LonOfOrig(double pLonOfOrig) = 0;
    virtual HRESULT __stdcall get_LatOfOrigStr(BSTR *pLatOfOrigStr) = 0;
    virtual HRESULT __stdcall set_LatOfOrigStr(BSTR pLatOfOrigStr) = 0;
    virtual HRESULT __stdcall get_LatOfOrig(double *pLatOfOrig) = 0;
    virtual HRESULT __stdcall set_LatOfOrig(double pLatOfOrig) = 0;
    virtual HRESULT __stdcall get_FalseXStr(BSTR *pFalseXStr) = 0;
    virtual HRESULT __stdcall set_FalseXStr(BSTR pFalseXStr) = 0;
    virtual HRESULT __stdcall get_FalseX(double *pFalseX) = 0;
    virtual HRESULT __stdcall set_FalseX(double pFalseX) = 0;
    virtual HRESULT __stdcall get_FalseYStr(BSTR *pFalseYStr) = 0;
    virtual HRESULT __stdcall set_FalseYStr(BSTR pFalseYStr) = 0;
    virtual HRESULT __stdcall get_FalseY(double *pFalseY) = 0;
    virtual HRESULT __stdcall set_FalseY(double pFalseY) = 0;
    virtual HRESULT __stdcall get_ScaleReductFactStr(BSTR *pScaleReductFactStr) = 0;
    virtual HRESULT __stdcall set_ScaleReductFactStr(BSTR pScaleReductFactStr) = 0;
    virtual HRESULT __stdcall get_ScaleReductFactVal(double *pScaleReductFactVal) = 0;
    virtual HRESULT __stdcall set_ScaleReductFactVal(double pScaleReductFactVal) = 0;
    virtual HRESULT __stdcall get_ZoneStr(BSTR *pZoneNumberStr) = 0;
    virtual HRESULT __stdcall set_ZoneStr(BSTR pZoneNumberStr) = 0;
    virtual HRESULT __stdcall get_ZoneVal(long *pZoneVal) = 0;
    virtual HRESULT __stdcall set_ZoneVal(long pZoneVal) = 0;
    virtual HRESULT __stdcall get_HemisphereStr(BSTR *pHemisphereStr) = 0;
    virtual HRESULT __stdcall set_HemisphereStr(BSTR pHemisphereStr) = 0;
    virtual HRESULT __stdcall get_HemisphereVal(long *pHemisphereVal) = 0;
    virtual HRESULT __stdcall set_HemisphereVal(long pHemisphereVal) = 0;
    virtual HRESULT __stdcall get_StandPar1Str(BSTR *pStandPar1Str) = 0;
    virtual HRESULT __stdcall set_StandPar1Str(BSTR pStandPar1Str) = 0;
    virtual HRESULT __stdcall get_StandPar1(double *pStandPar1) = 0;
    virtual HRESULT __stdcall set_StandPar1(double pStandPar1) = 0;
    virtual HRESULT __stdcall get_StandPar2Str(BSTR *pStandPar2Str) = 0;
    virtual HRESULT __stdcall set_StandPar2Str(BSTR pStandPar2Str) = 0;
    virtual HRESULT __stdcall get_StandPar2(double *pStandPar2) = 0;
    virtual HRESULT __stdcall set_StandPar2(double pStandPar2) = 0;
    virtual HRESULT __stdcall get_LatOfTrueScaleStr(BSTR *pLatOfTrueScaleStr) = 0;
    virtual HRESULT __stdcall set_LatOfTrueScaleStr(BSTR pLatOfTrueScaleStr) = 0;
    virtual HRESULT __stdcall get_LatOfTrueScale(double *pLatOfTrueScale) = 0;
    virtual HRESULT __stdcall set_LatOfTrueScale(double pLatOfTrueScale) = 0;
    virtual HRESULT __stdcall get_AzimuthAngStr(BSTR *pAzimuthAngStr) = 0;
    virtual HRESULT __stdcall set_AzimuthAngStr(BSTR pAzimuthAngStr) = 0;
    virtual HRESULT __stdcall get_AzimuthAng(double *pAzimuthAng) = 0;
    virtual HRESULT __stdcall set_AzimuthAng(double pAzimuthAng) = 0;
    virtual HRESULT __stdcall get_ExtendProjRotStr(BSTR *pExtendProjRotStr) = 0;
    virtual HRESULT __stdcall set_ExtendProjRotStr(BSTR pExtendProjRotStr) = 0;
    virtual HRESULT __stdcall get_ExtendProjRot(double *pExtendProjRot) = 0;
    virtual HRESULT __stdcall set_ExtendProjRot(double pExtendProjRot) = 0;
    virtual HRESULT __stdcall get_ProjAlgorithmStrings(ProjAlgorithmStrings* *pProjAlgorithmStrings) = 0;
};

struct ProjSpace : IDispatch
{
    LPTSTR get_ProjAlgorithmStr();
    void set_ProjAlgorithmStr(LPTSTR Value);
    long get_ProjAlgorithmVal();
    void set_ProjAlgorithmVal(long Value);
    LPTSTR get_LonOfOrigStr();
    void set_LonOfOrigStr(LPTSTR Value);
    double get_LonOfOrig();
    void set_LonOfOrig(double Value);
    LPTSTR get_LatOfOrigStr();
    void set_LatOfOrigStr(LPTSTR Value);
    double get_LatOfOrig();
    void set_LatOfOrig(double Value);
    LPTSTR get_FalseXStr();
    void set_FalseXStr(LPTSTR Value);
    double get_FalseX();
    void set_FalseX(double Value);
    LPTSTR get_FalseYStr();
    void set_FalseYStr(LPTSTR Value);
    double get_FalseY();
    void set_FalseY(double Value);
    LPTSTR get_ScaleReductFactStr();
    void set_ScaleReductFactStr(LPTSTR Value);
    double get_ScaleReductFactVal();
    void set_ScaleReductFactVal(double Value);
    LPTSTR get_ZoneStr();
    void set_ZoneStr(LPTSTR Value);
    long get_ZoneVal();
    void set_ZoneVal(long Value);
    LPTSTR get_HemisphereStr();
    void set_HemisphereStr(LPTSTR Value);
    long get_HemisphereVal();
    void set_HemisphereVal(long Value);
    LPTSTR get_StandPar1Str();
    void set_StandPar1Str(LPTSTR Value);
    double get_StandPar1();
    void set_StandPar1(double Value);
    LPTSTR get_StandPar2Str();
    void set_StandPar2Str(LPTSTR Value);
    double get_StandPar2();
    void set_StandPar2(double Value);
    LPTSTR get_LatOfTrueScaleStr();
    void set_LatOfTrueScaleStr(LPTSTR Value);
    double get_LatOfTrueScale();
    void set_LatOfTrueScale(double Value);
    LPTSTR get_AzimuthAngStr();
    void set_AzimuthAngStr(LPTSTR Value);
    double get_AzimuthAng();
    void set_AzimuthAng(double Value);
    LPTSTR get_ExtendProjRotStr();
    void set_ExtendProjRotStr(LPTSTR Value);
    double get_ExtendProjRot();
    void set_ExtendProjRot(double Value);
    ProjAlgorithmStrings* get_ProjAlgorithmStrings();
    long get_AzimuthDefinitionMode();
    void set_AzimuthDefinitionMode(long Value);
    double get_LatPoint1();
    void set_LatPoint1(double Value);
    double get_LatPoint2();
    void set_LatPoint2(double Value);
    double get_LonPoint1();
    void set_LonPoint1(double Value);
    double get_LonPoint2();
    void set_LonPoint2(double Value);
    double get_GeomHeightOrigin();
    void set_GeomHeightOrigin(double Value);
    double get_GeomHeightPoint1();
    void set_GeomHeightPoint1(double Value);
    double get_GeomHeightPoint2();
    void set_GeomHeightPoint2(double Value);
    double get_GeogRadiusStdCircle();
    void set_GeogRadiusStdCircle(double Value);
};

struct IGMProjSpace2 : IGMProjSpace
{
    virtual HRESULT __stdcall get_AzimuthDefinitionMode(long *pAzimuthDefinitionMode) = 0;
    virtual HRESULT __stdcall set_AzimuthDefinitionMode(long pAzimuthDefinitionMode) = 0;
    virtual HRESULT __stdcall get_LatPoint1(double *pLatPoint1) = 0;
    virtual HRESULT __stdcall set_LatPoint1(double pLatPoint1) = 0;
    virtual HRESULT __stdcall get_LatPoint2(double *pLatPoint2) = 0;
    virtual HRESULT __stdcall set_LatPoint2(double pLatPoint2) = 0;
    virtual HRESULT __stdcall get_LonPoint1(double *pLonPoint1) = 0;
    virtual HRESULT __stdcall set_LonPoint1(double pLonPoint1) = 0;
    virtual HRESULT __stdcall get_LonPoint2(double *pLonPoint2) = 0;
    virtual HRESULT __stdcall set_LonPoint2(double pLonPoint2) = 0;
};

struct IGMProjSpace3 : IGMProjSpace2
{
    virtual HRESULT __stdcall get_GeomHeightOrigin(double *pGeomHeightOrigin) = 0;
    virtual HRESULT __stdcall set_GeomHeightOrigin(double pGeomHeightOrigin) = 0;
    virtual HRESULT __stdcall get_GeomHeightPoint1(double *pGeomHeightPoint1) = 0;
    virtual HRESULT __stdcall set_GeomHeightPoint1(double pGeomHeightPoint1) = 0;
    virtual HRESULT __stdcall get_GeomHeightPoint2(double *pGeomHeightPoint2) = 0;
    virtual HRESULT __stdcall set_GeomHeightPoint2(double pGeomHeightPoint2) = 0;
};

struct IGMProjSpace4 : IGMProjSpace3
{
    virtual HRESULT __stdcall get_GeogRadiusStdCircle(double *pGeogRadiusStdCircle) = 0;
    virtual HRESULT __stdcall set_GeogRadiusStdCircle(double pGeogRadiusStdCircle) = 0;
};

struct IGMGeogSpace : IUnknown
{
    virtual HRESULT __stdcall get_GeodeticDatumStr(BSTR *pGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall set_GeodeticDatumStr(BSTR pGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall get_GeodeticDatumVal(long *pGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall set_GeodeticDatumVal(long pGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall get_EllipsoidStr(BSTR *pEllipsoidStr) = 0;
    virtual HRESULT __stdcall set_EllipsoidStr(BSTR pEllipsoidStr) = 0;
    virtual HRESULT __stdcall get_EllipsoidVal(long *pEllipsoidVal) = 0;
    virtual HRESULT __stdcall set_EllipsoidVal(long pEllipsoidVal) = 0;
    virtual HRESULT __stdcall get_EquatorialRadStr(BSTR *pEquatorialRadStr) = 0;
    virtual HRESULT __stdcall set_EquatorialRadStr(BSTR pEquatorialRadStr) = 0;
    virtual HRESULT __stdcall get_EquatorialRad(double *pEquatorialRad) = 0;
    virtual HRESULT __stdcall set_EquatorialRad(double pEquatorialRad) = 0;
    virtual HRESULT __stdcall get_PolarRadStr(BSTR *pPolarRadStr) = 0;
    virtual HRESULT __stdcall set_PolarRadStr(BSTR pPolarRadStr) = 0;
    virtual HRESULT __stdcall get_PolarRad(double *pPolarRad) = 0;
    virtual HRESULT __stdcall set_PolarRad(double pPolarRad) = 0;
    virtual HRESULT __stdcall get_EccentricityStr(BSTR *pEccentricityStr) = 0;
    virtual HRESULT __stdcall set_EccentricityStr(BSTR pEccentricityStr) = 0;
    virtual HRESULT __stdcall get_EccentricityVal(double *pEccentricityVal) = 0;
    virtual HRESULT __stdcall set_EccentricityVal(double pEccentricityVal) = 0;
    virtual HRESULT __stdcall get_FlatteningStr(BSTR *pFlatteningStr) = 0;
    virtual HRESULT __stdcall set_FlatteningStr(BSTR pFlatteningStr) = 0;
    virtual HRESULT __stdcall get_FlatteningVal(double *pFlatteningVal) = 0;
    virtual HRESULT __stdcall set_FlatteningVal(double pFlatteningVal) = 0;
    virtual HRESULT __stdcall get_FlatteningInverseStr(BSTR *pFlatteningInverseStr) = 0;
    virtual HRESULT __stdcall set_FlatteningInverseStr(BSTR pFlatteningInverseStr) = 0;
    virtual HRESULT __stdcall get_FlatteningInverseVal(double *pFlatteningInverseVal) = 0;
    virtual HRESULT __stdcall set_FlatteningInverseVal(double pFlatteningInverseVal) = 0;
    virtual HRESULT __stdcall get_GeodeticDatumStrings(GeodeticDatumStrings* *pGeodeticDatumStrings) = 0;
    virtual HRESULT __stdcall get_EllipsoidStrings(EllipsoidStrings* *pEllipsoidStrings) = 0;
};

struct GeogSpace : IDispatch
{
    LPTSTR get_GeodeticDatumStr();
    void set_GeodeticDatumStr(LPTSTR Value);
    long get_GeodeticDatumVal();
    void set_GeodeticDatumVal(long Value);
    LPTSTR get_EllipsoidStr();
    void set_EllipsoidStr(LPTSTR Value);
    long get_EllipsoidVal();
    void set_EllipsoidVal(long Value);
    LPTSTR get_EquatorialRadStr();
    void set_EquatorialRadStr(LPTSTR Value);
    double get_EquatorialRad();
    void set_EquatorialRad(double Value);
    LPTSTR get_PolarRadStr();
    void set_PolarRadStr(LPTSTR Value);
    double get_PolarRad();
    void set_PolarRad(double Value);
    LPTSTR get_EccentricityStr();
    void set_EccentricityStr(LPTSTR Value);
    double get_EccentricityVal();
    void set_EccentricityVal(double Value);
    LPTSTR get_FlatteningStr();
    void set_FlatteningStr(LPTSTR Value);
    double get_FlatteningVal();
    void set_FlatteningVal(double Value);
    LPTSTR get_FlatteningInverseStr();
    void set_FlatteningInverseStr(LPTSTR Value);
    double get_FlatteningInverseVal();
    void set_FlatteningInverseVal(double Value);
    GeodeticDatumStrings* get_GeodeticDatumStrings();
    EllipsoidStrings* get_EllipsoidStrings();
    LPTSTR get_VerticalDatumStr();
    void set_VerticalDatumStr(LPTSTR Value);
    long get_VerticalDatumVal();
    void set_VerticalDatumVal(long Value);
    DGMStringCollection* get_VerticalDatumStrings();
};

struct IGMGeogSpace2 : IGMGeogSpace
{
    virtual HRESULT __stdcall get_VerticalDatumStr(BSTR *pVerticalDatumStr) = 0;
    virtual HRESULT __stdcall set_VerticalDatumStr(BSTR pVerticalDatumStr) = 0;
    virtual HRESULT __stdcall get_VerticalDatumVal(long *pVerticalDatumVal) = 0;
    virtual HRESULT __stdcall set_VerticalDatumVal(long pVerticalDatumVal) = 0;
    virtual HRESULT __stdcall get_VerticalDatumStrings(DGMStringCollection* *pVerticalDatumStrings) = 0;
};

struct DGMStringCollection : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall Item(long index, BSTR *Item) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *Enum) = 0;
};

struct DGMStringCollectionDisp : IDispatch
{
    long get_Count();
    LPTSTR Item(long index);
    IUnknown * _NewEnum();
};

struct IGMUnitFormatSpec : IUnknown
{
    virtual HRESULT __stdcall set_ProjUnit(BSTR ProjUnit) = 0;
    virtual HRESULT __stdcall get_ProjUnit(BSTR *ProjUnit) = 0;
    virtual HRESULT __stdcall set_GeogUnit(BSTR GeogUnit) = 0;
    virtual HRESULT __stdcall get_GeogUnit(BSTR *GeogUnit) = 0;
    virtual HRESULT __stdcall set_GeocUnit(BSTR GeocUnit) = 0;
    virtual HRESULT __stdcall get_GeocUnit(BSTR *GeocUnit) = 0;
    virtual HRESULT __stdcall set_PaperUnit(BSTR PaperUnit) = 0;
    virtual HRESULT __stdcall get_PaperUnit(BSTR *PaperUnit) = 0;
    virtual HRESULT __stdcall set_HeightUnit(BSTR HeightUnit) = 0;
    virtual HRESULT __stdcall get_HeightUnit(BSTR *HeightUnit) = 0;
    virtual HRESULT __stdcall set_PaperHeightUnit(BSTR PaperHeightUnit) = 0;
    virtual HRESULT __stdcall get_PaperHeightUnit(BSTR *PaperHeightUnit) = 0;
    virtual HRESULT __stdcall set_ZHeightUnit(BSTR ZHeightUnit) = 0;
    virtual HRESULT __stdcall get_ZHeightUnit(BSTR *ZHeightUnit) = 0;
    virtual HRESULT __stdcall set_DistUnit(BSTR DistUnit) = 0;
    virtual HRESULT __stdcall get_DistUnit(BSTR *DistUnit) = 0;
    virtual HRESULT __stdcall set_PaperDistUnit(BSTR PaperDistUnit) = 0;
    virtual HRESULT __stdcall get_PaperDistUnit(BSTR *PaperDistUnit) = 0;
    virtual HRESULT __stdcall set_AngUnit(BSTR AngUnit) = 0;
    virtual HRESULT __stdcall get_AngUnit(BSTR *AngUnit) = 0;
    virtual HRESULT __stdcall set_AzimuthUnit(BSTR AzimuthUnit) = 0;
    virtual HRESULT __stdcall get_AzimuthUnit(BSTR *AzimuthUnit) = 0;
    virtual HRESULT __stdcall set_AreaUnit(BSTR AreaUnit) = 0;
    virtual HRESULT __stdcall get_AreaUnit(BSTR *AreaUnit) = 0;
    virtual HRESULT __stdcall set_PaperAreaUnit(BSTR PaperAreaUnit) = 0;
    virtual HRESULT __stdcall get_PaperAreaUnit(BSTR *PaperAreaUnit) = 0;
    virtual HRESULT __stdcall set_VolumeUnit(BSTR VolumeUnit) = 0;
    virtual HRESULT __stdcall get_VolumeUnit(BSTR *VolumeUnit) = 0;
    virtual HRESULT __stdcall set_TimeUnit(BSTR TimeUnit) = 0;
    virtual HRESULT __stdcall get_TimeUnit(BSTR *TimeUnit) = 0;
    virtual HRESULT __stdcall set_ProjPrec(long ProjPrec) = 0;
    virtual HRESULT __stdcall get_ProjPrec(long *ProjPrec) = 0;
    virtual HRESULT __stdcall set_GeogPrec(long GeogPrec) = 0;
    virtual HRESULT __stdcall get_GeogPrec(long *GeogPrec) = 0;
    virtual HRESULT __stdcall set_GeocPrec(long GeocPrec) = 0;
    virtual HRESULT __stdcall get_GeocPrec(long *GeocPrec) = 0;
    virtual HRESULT __stdcall set_PaperPrec(long PaperPrec) = 0;
    virtual HRESULT __stdcall get_PaperPrec(long *PaperPrec) = 0;
    virtual HRESULT __stdcall set_HeightPrec(long HeightPrec) = 0;
    virtual HRESULT __stdcall get_HeightPrec(long *HeightPrec) = 0;
    virtual HRESULT __stdcall set_PaperHeightPrec(long PaperHeightPrec) = 0;
    virtual HRESULT __stdcall get_PaperHeightPrec(long *PaperHeightPrec) = 0;
    virtual HRESULT __stdcall set_ZHeightPrec(long ZHeightPrec) = 0;
    virtual HRESULT __stdcall get_ZHeightPrec(long *ZHeightPrec) = 0;
    virtual HRESULT __stdcall set_DistPrec(long DistPrec) = 0;
    virtual HRESULT __stdcall get_DistPrec(long *DistPrec) = 0;
    virtual HRESULT __stdcall set_PaperDistPrec(long PaperDistPrec) = 0;
    virtual HRESULT __stdcall get_PaperDistPrec(long *PaperDistPrec) = 0;
    virtual HRESULT __stdcall set_AngPrec(long AngPrec) = 0;
    virtual HRESULT __stdcall get_AngPrec(long *AngPrec) = 0;
    virtual HRESULT __stdcall set_AzimuthPrec(long AzimuthPrec) = 0;
    virtual HRESULT __stdcall get_AzimuthPrec(long *AzimuthPrec) = 0;
    virtual HRESULT __stdcall set_AreaPrec(long AreaPrec) = 0;
    virtual HRESULT __stdcall get_AreaPrec(long *AreaPrec) = 0;
    virtual HRESULT __stdcall set_PaperAreaPrec(long PaperAreaPrec) = 0;
    virtual HRESULT __stdcall get_PaperAreaPrec(long *PaperAreaPrec) = 0;
    virtual HRESULT __stdcall set_VolumePrec(long VolumePrec) = 0;
    virtual HRESULT __stdcall get_VolumePrec(long *VolumePrec) = 0;
    virtual HRESULT __stdcall set_TimePrec(long TimePrec) = 0;
    virtual HRESULT __stdcall get_TimePrec(long *TimePrec) = 0;
    virtual HRESULT __stdcall set_GeogCoordOrd(long GeogCoordOrd) = 0;
    virtual HRESULT __stdcall get_GeogCoordOrd(long *GeogCoordOrd) = 0;
    virtual HRESULT __stdcall set_GeogHemiOpt(long GeogHemiOpt) = 0;
    virtual HRESULT __stdcall get_GeogHemiOpt(long *GeogHemiOpt) = 0;
    virtual HRESULT __stdcall set_LonNormOpt(long LonNormOpt) = 0;
    virtual HRESULT __stdcall get_LonNormOpt(long *LonNormOpt) = 0;
    virtual HRESULT __stdcall set_ProjQuadrantOpt(long ProjQuadrantOpt) = 0;
    virtual HRESULT __stdcall get_ProjQuadrantOpt(long *ProjQuadrantOpt) = 0;
    virtual HRESULT __stdcall set_HeightDisplayOpt(long HeightDisplayOpt) = 0;
    virtual HRESULT __stdcall get_HeightDisplayOpt(long *HeightDisplayOpt) = 0;
    virtual HRESULT __stdcall set_HeightDirectionOpt(long HeightDirectionOpt) = 0;
    virtual HRESULT __stdcall get_HeightDirectionOpt(long *HeightDirectionOpt) = 0;
    virtual HRESULT __stdcall set_MinusSuppresLonOpt(long MinusSuppresLonOpt) = 0;
    virtual HRESULT __stdcall get_MinusSuppresLonOpt(long *MinusSuppresLonOpt) = 0;
    virtual HRESULT __stdcall set_MinusSuppresHeightOpt(long MinusSuppresHeightOpt) = 0;
    virtual HRESULT __stdcall get_MinusSuppresHeightOpt(long *MinusSuppresHeightOpt) = 0;
    virtual HRESULT __stdcall ParseValueString(long ValueStringType, BSTR ValueString,
      double *Value) = 0;
    virtual HRESULT __stdcall ParsePointString(long PointStringType, BSTR PointString,
      double *X, double *Y, double *Z) = 0;
    virtual HRESULT __stdcall FormatValueString(long ValueStringType, double Value,
      BSTR *ValueString) = 0;
    virtual HRESULT __stdcall FormatPointString(long PointStringType, double X,
      double Y, double Z, BSTR *PointString) = 0;
    virtual HRESULT __stdcall GetPointDescription(long PointStringType, long DescriptionType,
      BSTR *DescriptionString) = 0;
    virtual HRESULT __stdcall set_AzimuthStart(long AzimuthStart) = 0;
    virtual HRESULT __stdcall get_AzimuthStart(long *AzimuthStart) = 0;
    virtual HRESULT __stdcall set_AzimuthDirection(long AzimuthDirection) = 0;
    virtual HRESULT __stdcall get_AzimuthDirection(long *AzimuthDirection) = 0;
    virtual HRESULT __stdcall set_BearingUnit(BSTR BearingUnit) = 0;
    virtual HRESULT __stdcall get_BearingUnit(BSTR *BearingUnit) = 0;
    virtual HRESULT __stdcall set_BearingPrec(long BearingPrec) = 0;
    virtual HRESULT __stdcall get_BearingPrec(long *BearingPrec) = 0;
};

struct UnitFormatSpec : IDispatch
{
    void set_ProjUnit(LPTSTR Value);
    LPTSTR get_ProjUnit();
    void set_GeogUnit(LPTSTR Value);
    LPTSTR get_GeogUnit();
    void set_GeocUnit(LPTSTR Value);
    LPTSTR get_GeocUnit();
    void set_PaperUnit(LPTSTR Value);
    LPTSTR get_PaperUnit();
    void set_HeightUnit(LPTSTR Value);
    LPTSTR get_HeightUnit();
    void set_PaperHeightUnit(LPTSTR Value);
    LPTSTR get_PaperHeightUnit();
    void set_ZHeightUnit(LPTSTR Value);
    LPTSTR get_ZHeightUnit();
    void set_DistUnit(LPTSTR Value);
    LPTSTR get_DistUnit();
    void set_PaperDistUnit(LPTSTR Value);
    LPTSTR get_PaperDistUnit();
    void set_AngUnit(LPTSTR Value);
    LPTSTR get_AngUnit();
    void set_AzimuthUnit(LPTSTR Value);
    LPTSTR get_AzimuthUnit();
    void set_AreaUnit(LPTSTR Value);
    LPTSTR get_AreaUnit();
    void set_PaperAreaUnit(LPTSTR Value);
    LPTSTR get_PaperAreaUnit();
    void set_VolumeUnit(LPTSTR Value);
    LPTSTR get_VolumeUnit();
    void set_TimeUnit(LPTSTR Value);
    LPTSTR get_TimeUnit();
    void set_ProjPrec(long Value);
    long get_ProjPrec();
    void set_GeogPrec(long Value);
    long get_GeogPrec();
    void set_GeocPrec(long Value);
    long get_GeocPrec();
    void set_PaperPrec(long Value);
    long get_PaperPrec();
    void set_HeightPrec(long Value);
    long get_HeightPrec();
    void set_PaperHeightPrec(long Value);
    long get_PaperHeightPrec();
    void set_ZHeightPrec(long Value);
    long get_ZHeightPrec();
    void set_DistPrec(long Value);
    long get_DistPrec();
    void set_PaperDistPrec(long Value);
    long get_PaperDistPrec();
    void set_AngPrec(long Value);
    long get_AngPrec();
    void set_AzimuthPrec(long Value);
    long get_AzimuthPrec();
    void set_AreaPrec(long Value);
    long get_AreaPrec();
    void set_PaperAreaPrec(long Value);
    long get_PaperAreaPrec();
    void set_VolumePrec(long Value);
    long get_VolumePrec();
    void set_TimePrec(long Value);
    long get_TimePrec();
    void set_GeogCoordOrd(long Value);
    long get_GeogCoordOrd();
    void set_GeogHemiOpt(long Value);
    long get_GeogHemiOpt();
    void set_LonNormOpt(long Value);
    long get_LonNormOpt();
    void set_ProjQuadrantOpt(long Value);
    long get_ProjQuadrantOpt();
    void set_HeightDisplayOpt(long Value);
    long get_HeightDisplayOpt();
    void set_HeightDirectionOpt(long Value);
    long get_HeightDirectionOpt();
    void set_MinusSuppresLonOpt(long Value);
    long get_MinusSuppresLonOpt();
    void set_MinusSuppresHeightOpt(long Value);
    long get_MinusSuppresHeightOpt();
    void ParseValueString(long ValueStringType, LPTSTR ValueString, double *Value);
    void ParsePointString(long PointStringType, LPTSTR PointString, double *X,
      double *Y, double *Z);
    void FormatValueString(long ValueStringType, double Value, BSTR *ValueString);
    void FormatPointString(long PointStringType, double X, double Y, double Z,
      BSTR *PointString);
    void GetPointDescription(long PointStringType, long DescriptionType, BSTR *DescriptionString);
    void set_AzimuthStart(long Value);
    long get_AzimuthStart();
    void set_AzimuthDirection(long Value);
    long get_AzimuthDirection();
    void set_BearingUnit(LPTSTR Value);
    LPTSTR get_BearingUnit();
    void set_BearingPrec(long Value);
    long get_BearingPrec();
    void set_MeasurementReferenceSpace(long Value);
    long get_MeasurementReferenceSpace();
    void CopyTo(IDispatch * UnitFormatSpecDest);
    void FormatUnitFormatSpecTableRow(SAFEARRAY * *RowFormat);
    void LoadFromUnitFormatSpecTableRow(SAFEARRAY * RowFormat);
};

struct IGMUnitFormatSpec2 : IGMUnitFormatSpec
{
    virtual HRESULT __stdcall set_MeasurementReferenceSpace(long MeasurementRefSpace) = 0;
    virtual HRESULT __stdcall get_MeasurementReferenceSpace(long *MeasurementRefSpace) = 0;
    virtual HRESULT __stdcall CopyTo(IDispatch * UnitFormatSpecDest) = 0;
    virtual HRESULT __stdcall FormatUnitFormatSpecTableRow(SAFEARRAY * *RowFormat) = 0;
    virtual HRESULT __stdcall LoadFromUnitFormatSpecTableRow(SAFEARRAY * RowFormat) = 0;
};

struct IGMPaperSpace : IUnknown
{
    virtual HRESULT __stdcall set_NominalMapScaleDenominator(double NominalMapScaleDenominator) = 0;
    virtual HRESULT __stdcall get_NominalMapScaleDenominator(double *NominalMapScaleDenominator) = 0;
    virtual HRESULT __stdcall set_ProjectionToPaperMatrix(SAFEARRAY * ProjectionToPaperMatrix) = 0;
    virtual HRESULT __stdcall get_ProjectionToPaperMatrix(SAFEARRAY * *ProjectionToPaperMatrix) = 0;
};

struct PaperSpace : IDispatch
{
    void set_NominalMapScaleDenominator(double Value);
    double get_NominalMapScaleDenominator();
    void set_ProjectionToPaperMatrix(SAFEARRAY * Value);
    SAFEARRAY * get_ProjectionToPaperMatrix();
};

struct IGMRefSpaceMgr : IUnknown
{
    virtual HRESULT __stdcall get_UnitFormatSpec(UnitFormatSpec* *UnitFormatSpec) = 0;
    virtual HRESULT __stdcall get_ProjSpace(ProjSpace* *ProjSpace) = 0;
    virtual HRESULT __stdcall get_GeogSpace(GeogSpace* *GeogSpace) = 0;
    virtual HRESULT __stdcall get_PaperSpace(PaperSpace* *PaperSpace) = 0;
    virtual HRESULT __stdcall TransformPoints(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points) = 0;
    virtual HRESULT __stdcall TransformPoint(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z) = 0;
    virtual HRESULT __stdcall LoadUsingPrimaryFromMGEfile(BSTR MGEfileName) = 0;
    virtual HRESULT __stdcall GetMeridianConvergence(double Longitude, double Latitude,
      double Height, double *MeridianConvergence) = 0;
    virtual HRESULT __stdcall IsTransformationValid(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid) = 0;
};

struct RefSpaceMgr : IDispatch
{
    UnitFormatSpec* get_UnitFormatSpec();
    ProjSpace* get_ProjSpace();
    GeogSpace* get_GeogSpace();
    PaperSpace* get_PaperSpace();
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    void LoadUsingPrimaryFromMGEfile(LPTSTR MGEfileName);
    void GetMeridianConvergence(double Longitude, double Latitude, double Height,
      double *MeridianConvergence);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void GetScaleFactors(double Longitude, double Latitude, double Height, double *K,
      double *H, double *S);
};

struct IGMRefSpaceMgr2 : IGMRefSpaceMgr
{
    virtual HRESULT __stdcall GetScaleFactors(double Longitude, double Latitude,
      double Height, double *K, double *H, double *S) = 0;
};

struct IGMCoordSystem : IUnknown
{
    virtual HRESULT __stdcall get_RefSpaceMgr(RefSpaceMgr* *RefSpaceMgr) = 0;
    virtual HRESULT __stdcall set_BaseStorageType(long BaseStorageType) = 0;
    virtual HRESULT __stdcall get_BaseStorageType(long *BaseStorageType) = 0;
    virtual HRESULT __stdcall set_HeightStorageType(long HeightStorageType) = 0;
    virtual HRESULT __stdcall get_HeightStorageType(long *HeightStorageType) = 0;
    virtual HRESULT __stdcall set_LonNormStorageOpt(long LonNormStorageOpt) = 0;
    virtual HRESULT __stdcall get_LonNormStorageOpt(long *LonNormStorageOpt) = 0;
    virtual HRESULT __stdcall set_ApproxVertDataCenterAsHeightVal(double ApproxVertDataCenterAsHeightVal) = 0;
    virtual HRESULT __stdcall get_ApproxVertDataCenterAsHeightVal(double *ApproxVertDataCenterAsHeightVal) = 0;
    virtual HRESULT __stdcall get_StorageToProjectionScale(double *StorageToProjectionScale) = 0;
    virtual HRESULT __stdcall get_StorageToPaperScale(double *StorageToPaperScale) = 0;
    virtual HRESULT __stdcall set_StorageToCompMatrix(SAFEARRAY * StorageToCompMatrix) = 0;
    virtual HRESULT __stdcall get_StorageToCompMatrix(SAFEARRAY * *StorageToCompMatrix) = 0;
    virtual HRESULT __stdcall set_GUID(VARIANT GUID) = 0;
    virtual HRESULT __stdcall get_GUID(VARIANT *GUID) = 0;
    virtual HRESULT __stdcall set_GUIDType(long GUIDType) = 0;
    virtual HRESULT __stdcall get_GUIDType(long *GUIDType) = 0;
    virtual HRESULT __stdcall SetHorizResByGroundDistVal(double MagnitudeVal, BSTR UnitStr) = 0;
    virtual HRESULT __stdcall GetHorizResByGroundDistVal(double *MagnitudeVal,
      BSTR *UnitStr) = 0;
    virtual HRESULT __stdcall SetHorizResByAngAlongGroundVal(double MagnitudeVal,
      BSTR UnitStr) = 0;
    virtual HRESULT __stdcall GetHorizResByAngAlongGroundVal(double *MagnitudeVal,
      BSTR *UnitStr) = 0;
    virtual HRESULT __stdcall SetHorizResByPaperDistVal(double MagnitudeVal, BSTR UnitStr) = 0;
    virtual HRESULT __stdcall GetHorizResByPaperDistVal(double *MagnitudeVal, BSTR *UnitStr) = 0;
    virtual HRESULT __stdcall SetVertResByHeightVal(double MagnitudeVal, BSTR UnitStr) = 0;
    virtual HRESULT __stdcall GetVertResByHeightVal(double *MagnitudeVal, BSTR *UnitStr) = 0;
    virtual HRESULT __stdcall SetVertResByPaperHeightVal(double MagnitudeVal, BSTR UnitStr) = 0;
    virtual HRESULT __stdcall GetVertResByPaperHeightVal(double *MagnitudeVal,
      BSTR *UnitStr) = 0;
    virtual HRESULT __stdcall SetApproxHorizDataCenterAsGeogPntVal(double Longitude,
      double Latitude) = 0;
    virtual HRESULT __stdcall GetApproxHorizDataCenterAsGeogPntVal(double *Longitude,
      double *Latitude) = 0;
    virtual HRESULT __stdcall SetApproxHorizDataCenterAsProjPntVal(double ProjectionX,
      double ProjectionY) = 0;
    virtual HRESULT __stdcall GetApproxHorizDataCenterAsProjPntVal(double *ProjectionX,
      double *ProjectionY) = 0;
    virtual HRESULT __stdcall SetApproxHorizDataCenterAsGeocPntVal(double GeocentricX,
      double GeocentricY, double GeocentricZ) = 0;
    virtual HRESULT __stdcall GetApproxHorizDataCenterAsGeocPntVal(double *GeocentricX,
      double *GeocentricY, double *GeocentricZ) = 0;
    virtual HRESULT __stdcall SetApproxHorizDataCenterAsPaperPntVal(double PaperX,
      double PaperY) = 0;
    virtual HRESULT __stdcall GetApproxHorizDataCenterAsPaperPntVal(double *PaperX,
      double *PaperY) = 0;
    virtual HRESULT __stdcall TransformPoints(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points) = 0;
    virtual HRESULT __stdcall TransformPoint(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z) = 0;
    virtual HRESULT __stdcall LoadUsingPrimaryFromMGEfile(BSTR MGEfileName) = 0;
    virtual HRESULT __stdcall LoadFromGCoordSystemTableRowFormat(SAFEARRAY * RowFormat) = 0;
    virtual HRESULT __stdcall FormatGCoordSystemTableRow(SAFEARRAY * RowFormat) = 0;
    virtual HRESULT __stdcall IsEssentiallyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsEssentiallyIdentical) = 0;
    virtual HRESULT __stdcall Clone(IDispatch * *CoordSystemClone) = 0;
    virtual HRESULT __stdcall CopyTo(IDispatch * CoordSystemDest) = 0;
    virtual HRESULT __stdcall GetStorageMeridianConvergence(double Longitude, double Latitude,
      double Height, double *StorageMeridianConvergence) = 0;
    virtual HRESULT __stdcall IsTransformationValid(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid) = 0;
    virtual HRESULT __stdcall ChangeGUID() = 0;
    virtual HRESULT __stdcall SaveToFile(BSTR FileName) = 0;
    virtual HRESULT __stdcall LoadFromFile(BSTR FileName) = 0;
};

struct DGMCoordSystem : IDispatch
{
    RefSpaceMgr* get_RefSpaceMgr();
    void set_BaseStorageType(long Value);
    long get_BaseStorageType();
    void set_HeightStorageType(long Value);
    long get_HeightStorageType();
    void set_LonNormStorageOpt(long Value);
    long get_LonNormStorageOpt();
    void set_ApproxVertDataCenterAsHeightVal(double Value);
    double get_ApproxVertDataCenterAsHeightVal();
    double get_StorageToProjectionScale();
    double get_StorageToPaperScale();
    void set_StorageToCompMatrix(SAFEARRAY * Value);
    SAFEARRAY * get_StorageToCompMatrix();
    void set_GUID(VARIANT Value);
    VARIANT get_GUID();
    void set_GUIDType(long Value);
    long get_GUIDType();
    void SetHorizResByGroundDistVal(double MagnitudeVal, LPTSTR UnitStr);
    void GetHorizResByGroundDistVal(double *MagnitudeVal, BSTR *UnitStr);
    void SetHorizResByAngAlongGroundVal(double MagnitudeVal, LPTSTR UnitStr);
    void GetHorizResByAngAlongGroundVal(double *MagnitudeVal, BSTR *UnitStr);
    void SetHorizResByPaperDistVal(double MagnitudeVal, LPTSTR UnitStr);
    void GetHorizResByPaperDistVal(double *MagnitudeVal, BSTR *UnitStr);
    void SetVertResByHeightVal(double MagnitudeVal, LPTSTR UnitStr);
    void GetVertResByHeightVal(double *MagnitudeVal, BSTR *UnitStr);
    void SetVertResByPaperHeightVal(double MagnitudeVal, LPTSTR UnitStr);
    void GetVertResByPaperHeightVal(double *MagnitudeVal, BSTR *UnitStr);
    void SetApproxHorizDataCenterAsGeogPntVal(double Longitude, double Latitude);
    void GetApproxHorizDataCenterAsGeogPntVal(double *Longitude, double *Latitude);
    void SetApproxHorizDataCenterAsProjPntVal(double ProjectionX, double ProjectionY);
    void GetApproxHorizDataCenterAsProjPntVal(double *ProjectionX, double *ProjectionY);
    void SetApproxHorizDataCenterAsGeocPntVal(double GeocentricX, double GeocentricY,
      double GeocentricZ);
    void GetApproxHorizDataCenterAsGeocPntVal(double *GeocentricX, double *GeocentricY,
      double *GeocentricZ);
    void SetApproxHorizDataCenterAsPaperPntVal(double PaperX, double PaperY);
    void GetApproxHorizDataCenterAsPaperPntVal(double *PaperX, double *PaperY);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    void LoadUsingPrimaryFromMGEfile(LPTSTR MGEfileName);
    void LoadFromGCoordSystemTableRowFormat(SAFEARRAY * RowFormat);
    void FormatGCoordSystemTableRow(SAFEARRAY * RowFormat);
    void IsEssentiallyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsEssentiallyIdentical);
    void Clone(IDispatch * *CoordSystemClone);
    void CopyTo(IDispatch * CoordSystemDest);
    void GetStorageMeridianConvergence(double Longitude, double Latitude, double Height,
      double *StorageMeridianConvergence);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void ChangeGUID();
    void SaveToFile(LPTSTR FileName);
    void LoadFromFile(LPTSTR FileName);
    void set_Name(LPTSTR Value);
    LPTSTR get_Name();
    void set_Description(LPTSTR Value);
    LPTSTR get_Description();
    void IsExactlyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsExactlyIdentical);
    void LoadFromMGEFileSecondaryCoordSystem(LPTSTR FileName);
};

struct IGMCoordSystem2 : IGMCoordSystem
{
    virtual HRESULT __stdcall set_Name(BSTR Name) = 0;
    virtual HRESULT __stdcall get_Name(BSTR *Name) = 0;
    virtual HRESULT __stdcall set_Description(BSTR Description) = 0;
    virtual HRESULT __stdcall get_Description(BSTR *Description) = 0;
    virtual HRESULT __stdcall IsExactlyIdentical(IDispatch * CoordSystem, VARIANT_BOOL *IsExactlyIdentical) = 0;
};

struct IGMCoordSystem3 : IGMCoordSystem2
{
    virtual HRESULT __stdcall LoadFromMGEFileSecondaryCoordSystem(BSTR FileName) = 0;
};

struct DGMDatumTransformation : IDispatch
{
    virtual HRESULT __stdcall get_ModelTypeVal(long *ModelTypeVal) = 0;
    virtual HRESULT __stdcall set_ModelTypeVal(long ModelTypeVal) = 0;
    virtual HRESULT __stdcall get_ModelTypeStr(BSTR *ModelTypeStr) = 0;
    virtual HRESULT __stdcall set_ModelTypeStr(BSTR ModelTypeStr) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceGeodeticDatumVal(long *ForwardInputSpaceGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceGeodeticDatumVal(long ForwardInputSpaceGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceGeodeticDatumStr(BSTR *ForwardInputSpaceGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceGeodeticDatumStr(BSTR ForwardInputSpaceGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall get_ForwardOutputSpaceGeodeticDatumVal(long *ForwardOutputSpaceGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall set_ForwardOutputSpaceGeodeticDatumVal(long ForwardOutputSpaceGeodeticDatumVal) = 0;
    virtual HRESULT __stdcall get_ForwardOutputSpaceGeodeticDatumStr(BSTR *ForwardOutputSpaceGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall set_ForwardOutputSpaceGeodeticDatumStr(BSTR ForwardOutputSpaceGeodeticDatumStr) = 0;
    virtual HRESULT __stdcall get_Direction(long *Direction) = 0;
    virtual HRESULT __stdcall set_Direction(long Direction) = 0;
    virtual HRESULT __stdcall get_Name(BSTR *Name) = 0;
    virtual HRESULT __stdcall set_Name(BSTR Name) = 0;
    virtual HRESULT __stdcall get_Description(BSTR *Description) = 0;
    virtual HRESULT __stdcall set_Description(BSTR Description) = 0;
    virtual HRESULT __stdcall get_BursaWolf(DGMBursaWolf* *BursaWolf) = 0;
    virtual HRESULT __stdcall get_ComplexPolynomial(DGMComplexPolynomial* *ComplexPolynomial) = 0;
    virtual HRESULT __stdcall get_MultipleRegression(DGMMultipleRegression* *MultipleRegression) = 0;
    virtual HRESULT __stdcall get_SecondDegreePolynomial(DGMSecondDegreePolynomial* *SecondDegreePolynomial) = 0;
    virtual HRESULT __stdcall get_StandardMolodensky(DGMStandardMolodensky* *StandardMolodensky) = 0;
    virtual HRESULT __stdcall get_DatumTransModelTypeStrings(DGMStringCollection* *DatumTransModelTypeStrings) = 0;
    virtual HRESULT __stdcall get_GeodeticDatumStrings(DGMStringCollection* *GeodeticDatumStrings) = 0;
    virtual HRESULT __stdcall CopyTo(DGMDatumTransformation* DatumTransformationDest) = 0;
    virtual HRESULT __stdcall IsTransformationValid(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid) = 0;
    virtual HRESULT __stdcall TransformPoints(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points) = 0;
    virtual HRESULT __stdcall TransformPoint(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z) = 0;
};

struct DGMDatumTransformationDisp : IDispatch
{
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    long get_ForwardInputSpaceGeodeticDatumVal();
    void set_ForwardInputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardInputSpaceGeodeticDatumStr();
    void set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_ForwardOutputSpaceGeodeticDatumVal();
    void set_ForwardOutputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardOutputSpaceGeodeticDatumStr();
    void set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_Direction();
    void set_Direction(long Value);
    LPTSTR get_Name();
    void set_Name(LPTSTR Value);
    LPTSTR get_Description();
    void set_Description(LPTSTR Value);
    DGMBursaWolf* get_BursaWolf();
    DGMComplexPolynomial* get_ComplexPolynomial();
    DGMMultipleRegression* get_MultipleRegression();
    DGMSecondDegreePolynomial* get_SecondDegreePolynomial();
    DGMStandardMolodensky* get_StandardMolodensky();
    DGMStringCollection* get_DatumTransModelTypeStrings();
    DGMStringCollection* get_GeodeticDatumStrings();
    void CopyTo(DGMDatumTransformation* DatumTransformationDest);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
};

struct DGMBursaWolf : IDispatch
{
    virtual HRESULT __stdcall get_DeltaX(double *DeltaX) = 0;
    virtual HRESULT __stdcall set_DeltaX(double DeltaX) = 0;
    virtual HRESULT __stdcall get_DeltaY(double *DeltaY) = 0;
    virtual HRESULT __stdcall set_DeltaY(double DeltaY) = 0;
    virtual HRESULT __stdcall get_DeltaZ(double *DeltaZ) = 0;
    virtual HRESULT __stdcall set_DeltaZ(double DeltaZ) = 0;
    virtual HRESULT __stdcall get_OmegaX(double *OmegaX) = 0;
    virtual HRESULT __stdcall set_OmegaX(double OmegaX) = 0;
    virtual HRESULT __stdcall get_OmegaY(double *OmegaY) = 0;
    virtual HRESULT __stdcall set_OmegaY(double OmegaY) = 0;
    virtual HRESULT __stdcall get_OmegaZ(double *OmegaZ) = 0;
    virtual HRESULT __stdcall set_OmegaZ(double OmegaZ) = 0;
    virtual HRESULT __stdcall get_DeltaScale(double *DeltaScale) = 0;
    virtual HRESULT __stdcall set_DeltaScale(double DeltaScale) = 0;
};

struct DGMBursaWolfDisp : IDispatch
{
    double get_DeltaX();
    void set_DeltaX(double Value);
    double get_DeltaY();
    void set_DeltaY(double Value);
    double get_DeltaZ();
    void set_DeltaZ(double Value);
    double get_OmegaX();
    void set_OmegaX(double Value);
    double get_OmegaY();
    void set_OmegaY(double Value);
    double get_OmegaZ();
    void set_OmegaZ(double Value);
    double get_DeltaScale();
    void set_DeltaScale(double Value);
};

struct DGMComplexPolynomial : IDispatch
{
    virtual HRESULT __stdcall get_ModelTypeVal(long *ModelTypeVal) = 0;
    virtual HRESULT __stdcall set_ModelTypeVal(long ModelTypeVal) = 0;
    virtual HRESULT __stdcall get_ModelTypeStr(BSTR *ModelTypeStr) = 0;
    virtual HRESULT __stdcall set_ModelTypeStr(BSTR ModelTypeStr) = 0;
    virtual HRESULT __stdcall get_ComplexPolyModelTypeStrings(DGMStringCollection* *ComplexPolyModelTypeStrings) = 0;
};

struct DGMComplexPolynomialDisp : IDispatch
{
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    DGMStringCollection* get_ComplexPolyModelTypeStrings();
};

struct DGMMultipleRegression : IDispatch
{
    virtual HRESULT __stdcall get_ModelTypeVal(long *ModelTypeVal) = 0;
    virtual HRESULT __stdcall set_ModelTypeVal(long ModelTypeVal) = 0;
    virtual HRESULT __stdcall get_ModelTypeStr(BSTR *ModelTypeStr) = 0;
    virtual HRESULT __stdcall set_ModelTypeStr(BSTR ModelTypeStr) = 0;
    virtual HRESULT __stdcall get_MultRegressionModelTypeStrings(DGMStringCollection* *MultRegressionModelTypeStrings) = 0;
};

struct DGMMultipleRegressionDisp : IDispatch
{
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    DGMStringCollection* get_MultRegressionModelTypeStrings();
};

struct DGMSecondDegreePolynomial : IDispatch
{
    virtual HRESULT __stdcall get_ModelForwardInputUnitID(long *ModelForwardInputUnitID) = 0;
    virtual HRESULT __stdcall set_ModelForwardInputUnitID(long ModelForwardInputUnitID) = 0;
    virtual HRESULT __stdcall get_ModelForwardOutputUnitID(long *ModelForwardOutputUnitID) = 0;
    virtual HRESULT __stdcall set_ModelForwardOutputUnitID(long ModelForwardOutputUnitID) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceXofOrig(double *ForwardInputSpaceXofOrig) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceXofOrig(double ForwardInputSpaceXofOrig) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceYofOrig(double *ForwardInputSpaceYofOrig) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceYofOrig(double ForwardInputSpaceYofOrig) = 0;
    virtual HRESULT __stdcall get_XNormalizationFactor(double *XNormalizationFactor) = 0;
    virtual HRESULT __stdcall set_XNormalizationFactor(double XNormalizationFactor) = 0;
    virtual HRESULT __stdcall get_YNormalizationFactor(double *YNormalizationFactor) = 0;
    virtual HRESULT __stdcall set_YNormalizationFactor(double YNormalizationFactor) = 0;
    virtual HRESULT __stdcall get_Coefficients(SAFEARRAY * *Coefficients) = 0;
    virtual HRESULT __stdcall set_Coefficients(SAFEARRAY * Coefficients) = 0;
};

struct DGMSecondDegreePolynomialDisp : IDispatch
{
    long get_ModelForwardInputUnitID();
    void set_ModelForwardInputUnitID(long Value);
    long get_ModelForwardOutputUnitID();
    void set_ModelForwardOutputUnitID(long Value);
    double get_ForwardInputSpaceXofOrig();
    void set_ForwardInputSpaceXofOrig(double Value);
    double get_ForwardInputSpaceYofOrig();
    void set_ForwardInputSpaceYofOrig(double Value);
    double get_XNormalizationFactor();
    void set_XNormalizationFactor(double Value);
    double get_YNormalizationFactor();
    void set_YNormalizationFactor(double Value);
    SAFEARRAY * get_Coefficients();
    void set_Coefficients(SAFEARRAY * Value);
};

struct DGMStandardMolodensky : IDispatch
{
    virtual HRESULT __stdcall get_ForwardInputSpaceEllipsoidVal(long *ForwardInputSpaceEllipsoidVal) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceEllipsoidVal(long ForwardInputSpaceEllipsoidVal) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceEllipsoidStr(BSTR *ForwardInputSpaceEllipsoidStr) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceEllipsoidStr(BSTR ForwardInputSpaceEllipsoidStr) = 0;
    virtual HRESULT __stdcall get_EllipsoidStrings(DGMStringCollection* *EllipsoidStrings) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceEquatorialRad(double *ForwardInputSpaceEquatorialRad) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceEquatorialRad(double ForwardInputSpaceEquatorialRad) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceEccentricity(double *ForwardInputSpaceEccentricity) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceEccentricity(double ForwardInputSpaceEccentricity) = 0;
    virtual HRESULT __stdcall get_DeltaEquatorialRad(double *DeltaEquatorialRad) = 0;
    virtual HRESULT __stdcall set_DeltaEquatorialRad(double DeltaEquatorialRad) = 0;
    virtual HRESULT __stdcall get_DeltaFlattening(double *DeltaFlattening) = 0;
    virtual HRESULT __stdcall set_DeltaFlattening(double DeltaFlattening) = 0;
    virtual HRESULT __stdcall get_DeltaX(double *DeltaX) = 0;
    virtual HRESULT __stdcall set_DeltaX(double DeltaX) = 0;
    virtual HRESULT __stdcall get_DeltaY(double *DeltaY) = 0;
    virtual HRESULT __stdcall set_DeltaY(double DeltaY) = 0;
    virtual HRESULT __stdcall get_DeltaZ(double *DeltaZ) = 0;
    virtual HRESULT __stdcall set_DeltaZ(double DeltaZ) = 0;
};

struct DGMStandardMolodenskyDisp : IDispatch
{
    long get_ForwardInputSpaceEllipsoidVal();
    void set_ForwardInputSpaceEllipsoidVal(long Value);
    LPTSTR get_ForwardInputSpaceEllipsoidStr();
    void set_ForwardInputSpaceEllipsoidStr(LPTSTR Value);
    DGMStringCollection* get_EllipsoidStrings();
    double get_ForwardInputSpaceEquatorialRad();
    void set_ForwardInputSpaceEquatorialRad(double Value);
    double get_ForwardInputSpaceEccentricity();
    void set_ForwardInputSpaceEccentricity(double Value);
    double get_DeltaEquatorialRad();
    void set_DeltaEquatorialRad(double Value);
    double get_DeltaFlattening();
    void set_DeltaFlattening(double Value);
    double get_DeltaX();
    void set_DeltaX(double Value);
    double get_DeltaY();
    void set_DeltaY(double Value);
    double get_DeltaZ();
    void set_DeltaZ(double Value);
};

struct IGMLink : IUnknown
{
    virtual HRESULT __stdcall get_LinkObject(IDispatch * *lplpLinkObject) = 0;
    virtual HRESULT __stdcall get_Type(long *lpType) = 0;
};

struct Link : IDispatch
{
    IDispatch * get_LinkObject();
    long get_Type();
};

struct IGMLinks : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *lpCount) = 0;
    virtual HRESULT __stdcall Add(BSTR Name, long Type) = 0;
    virtual HRESULT __stdcall Item(BSTR Name, Link* *lplpLink) = 0;
    virtual HRESULT __stdcall Delete(BSTR Name) = 0;
    virtual HRESULT __stdcall DeleteAll() = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *lplpEnum) = 0;
};

struct Links : IDispatch
{
    long get_Count();
    void Add(LPTSTR Name, long Type);
    Link* Item(LPTSTR Name);
    void Delete(LPTSTR Name);
    void DeleteAll();
    IUnknown * _NewEnum();
};

struct IGMTransLinks : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *lpCount) = 0;
    virtual HRESULT __stdcall Append(Link* lpLink, VARIANT_BOOL Strengthen, long DirectionToStrengthen) = 0;
    virtual HRESULT __stdcall Item(long index, Link* *lplpLink) = 0;
    virtual HRESULT __stdcall RemoveAll() = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *lplpEnum) = 0;
};

struct TransLinks : IDispatch
{
    long get_Count();
    void Append(Link* lpLink, BOOL Strengthen, long DirectionToStrengthen);
    Link* Item(long index);
    void RemoveAll();
    IUnknown * _NewEnum();
};

struct IGMAltCoordSystemPath : IUnknown
{
    virtual HRESULT __stdcall get_Links(Links* *Links) = 0;
    virtual HRESULT __stdcall get_TransLinks(TransLinks* *TransLinks) = 0;
    virtual HRESULT __stdcall set_Persist(VARIANT_BOOL Persist) = 0;
    virtual HRESULT __stdcall get_Persist(VARIANT_BOOL *Persist) = 0;
    virtual HRESULT __stdcall IsTransformationValid(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid) = 0;
    virtual HRESULT __stdcall TransformPoints(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points) = 0;
    virtual HRESULT __stdcall TransformPoint(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z) = 0;
    virtual HRESULT __stdcall RecomputeInternalPaths() = 0;
};

struct AltCoordSystemPath : IDispatch
{
    Links* get_Links();
    TransLinks* get_TransLinks();
    void set_Persist(BOOL Value);
    BOOL get_Persist();
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    void RecomputeInternalPaths();
};

struct IGMAltCoordSystemPaths : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *lpCount) = 0;
    virtual HRESULT __stdcall Add(BSTR Name) = 0;
    virtual HRESULT __stdcall Item(BSTR Name, AltCoordSystemPath* *lplpAltCoordSystemPath) = 0;
    virtual HRESULT __stdcall Delete(BSTR Name) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *lplpEnum) = 0;
};

struct AltCoordSystemPaths : IDispatch
{
    long get_Count();
    void Add(LPTSTR Name);
    AltCoordSystemPath* Item(LPTSTR Name);
    void Delete(LPTSTR Name);
    IUnknown * _NewEnum();
};

struct IGMCoordSystemsMgr : IUnknown
{
    virtual HRESULT __stdcall get_CoordSystem(DGMCoordSystem* *ppCoordSystem) = 0;
    virtual HRESULT __stdcall get_AltCoordSystemPaths(AltCoordSystemPaths* *ppAltCoordSystemPaths) = 0;
};

struct DGMCoordSystemsMgr : IDispatch
{
    DGMCoordSystem* get_CoordSystem();
    AltCoordSystemPaths* get_AltCoordSystemPaths();
};

struct IGMUnitIDs : IUnknown
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall set__UnitType(long Value) = 0;
    virtual HRESULT __stdcall Item(long index, long *Item) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *Enum) = 0;
};

struct UnitIDs : IDispatch
{
    long get_Count();
    void set__UnitType(long Value);
    long Item(long index);
    IUnknown * _NewEnum();
    void set__Filter(VARIANT Value);
};

struct IGMUnitIDs2 : IGMUnitIDs
{
    virtual HRESULT __stdcall set__Filter(VARIANT Value) = 0;
};

struct IGMUnitsOfMeasureTables : IUnknown
{
    virtual HRESULT __stdcall AddUserDefinedUnit(long UnitType, BSTR UnitName,
      double NumCompBaseUnits, double UnitOffset, BSTR AppName, BSTR AppVersionStr,
      long *UnitID) = 0;
    virtual HRESULT __stdcall ConvertUnitToUnit(long UnitType, double ValueIn,
      long UnitIDFrom, long UnitIDTo, double *ValueOut) = 0;
    virtual HRESULT __stdcall DeleteUserDefinedUnit(long UnitType, long UnitID,
      BSTR AppName, BSTR AppVersionStr) = 0;
    virtual HRESULT __stdcall GetCompBaseUnit(long UnitType, long *UnitID) = 0;
    virtual HRESULT __stdcall GetUnitIDs(long UnitType, UnitIDs* *UnitIDs) = 0;
    virtual HRESULT __stdcall GetUnitID(long UnitType, BSTR UnitName, VARIANT CaseFlag,
      long *UnitID) = 0;
    virtual HRESULT __stdcall GetUnitName(long UnitID, BSTR *UnitName) = 0;
    virtual HRESULT __stdcall LoadUserDefinedUnits(BSTR AppName, BSTR AppVersionStr) = 0;
};

struct DGMUnitsOfMeasure : IDispatch
{
    long AddUserDefinedUnit(long UnitType, LPTSTR UnitName, double NumCompBaseUnits,
      double UnitOffset, LPTSTR AppName, LPTSTR AppVersionStr);
    double ConvertUnitToUnit(long UnitType, double ValueIn, long UnitIDFrom, long UnitIDTo);
    void DeleteUserDefinedUnit(long UnitType, long UnitID, LPTSTR AppName, LPTSTR AppVersionStr);
    long GetCompBaseUnit(long UnitType);
    UnitIDs* GetUnitIDs(long UnitType, VARIANT Filter);
    long GetUnitID(long UnitType, LPTSTR UnitName, VARIANT CaseFlag);
    LPTSTR GetUnitName(long UnitID);
    void LoadUserDefinedUnits(LPTSTR AppName, LPTSTR AppVersionStr);
};

struct IGMUnitsOfMeasureTables2 : IUnknown
{
    virtual HRESULT __stdcall AddUserDefinedUnit(long UnitType, BSTR UnitName,
      double NumCompBaseUnits, double UnitOffset, BSTR AppName, BSTR AppVersionStr,
      long *UnitID) = 0;
    virtual HRESULT __stdcall ConvertUnitToUnit(long UnitType, double ValueIn,
      long UnitIDFrom, long UnitIDTo, double *ValueOut) = 0;
    virtual HRESULT __stdcall DeleteUserDefinedUnit(long UnitType, long UnitID,
      BSTR AppName, BSTR AppVersionStr) = 0;
    virtual HRESULT __stdcall GetCompBaseUnit(long UnitType, long *UnitID) = 0;
    virtual HRESULT __stdcall GetUnitIDs(long UnitType, VARIANT Filter, UnitIDs* *UnitIDs) = 0;
    virtual HRESULT __stdcall GetUnitID(long UnitType, BSTR UnitName, VARIANT CaseFlag,
      long *UnitID) = 0;
    virtual HRESULT __stdcall GetUnitName(long UnitID, BSTR *UnitName) = 0;
    virtual HRESULT __stdcall LoadUserDefinedUnits(BSTR AppName, BSTR AppVersionStr) = 0;
};

struct DGMLeastSquares : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *Name) = 0;
    virtual HRESULT __stdcall set_Name(BSTR Name) = 0;
    virtual HRESULT __stdcall get_Description(BSTR *Description) = 0;
    virtual HRESULT __stdcall set_Description(BSTR Description) = 0;
    virtual HRESULT __stdcall get_ModelTypeStr(BSTR *ModelTypeStr) = 0;
    virtual HRESULT __stdcall set_ModelTypeStr(BSTR ModelTypeStr) = 0;
    virtual HRESULT __stdcall get_ModelTypeVal(long *ModelTypeVal) = 0;
    virtual HRESULT __stdcall set_ModelTypeVal(long ModelTypeVal) = 0;
    virtual HRESULT __stdcall get_Degree(long *Degree) = 0;
    virtual HRESULT __stdcall set_Degree(long Degree) = 0;
    virtual HRESULT __stdcall get_NumberControlPointsNeeded(long *NumberControlPointsNeeded) = 0;
    virtual HRESULT __stdcall get_DegreesOfFreedom(long *DegreesOfFreedom) = 0;
    virtual HRESULT __stdcall get_Is3d(VARIANT_BOOL *Is3d) = 0;
    virtual HRESULT __stdcall get_IsLinear(VARIANT_BOOL *IsLinear) = 0;
    virtual HRESULT __stdcall get_WeightsUsage(long *WeightsUsage) = 0;
    virtual HRESULT __stdcall set_WeightsUsage(long WeightsUsage) = 0;
    virtual HRESULT __stdcall get_NumberTransformationParameters(long *NumberTransformationParameters) = 0;
    virtual HRESULT __stdcall get_TransformationParameters(SAFEARRAY * *TransformationParameters) = 0;
    virtual HRESULT __stdcall get_ParameterCovarianceMatrix(SAFEARRAY * *ParameterCovarianceMatrix) = 0;
    virtual HRESULT __stdcall get_ObservationMatrixSize(long *ObservationMatrixSize) = 0;
    virtual HRESULT __stdcall get_AdjObservationCovarianceMatrix(SAFEARRAY * *AdjObservationCovarianceMatrix) = 0;
    virtual HRESULT __stdcall get_ResidualCovarianceMatrix(SAFEARRAY * *ResidualCovarianceMatrix) = 0;
    virtual HRESULT __stdcall get_HomogeneousParamMatrixType(long *HomogeneousParamMatrixType) = 0;
    virtual HRESULT __stdcall set_HomogeneousParamMatrixType(long HomogeneousParamMatrixType) = 0;
    virtual HRESULT __stdcall get_HomogeneousParameterMatrix(SAFEARRAY * *HomogeneousParameterMatrix) = 0;
    virtual HRESULT __stdcall get_VarianceFactor(double *VarianceFactor) = 0;
    virtual HRESULT __stdcall get_RootMeanSquareOfResiduals(double *RootMeanSquareOfResiduals) = 0;
    virtual HRESULT __stdcall get_ApplyResidualSmoothing(VARIANT_BOOL *ApplyResidualSmoothing) = 0;
    virtual HRESULT __stdcall set_ApplyResidualSmoothing(VARIANT_BOOL ApplyResidualSmoothing) = 0;
    virtual HRESULT __stdcall get_ResidualSmoothingExponent(double *ResidualSmoothingExponent) = 0;
    virtual HRESULT __stdcall set_ResidualSmoothingExponent(double ResidualSmoothingExponent) = 0;
    virtual HRESULT __stdcall get_InputPointType(long *InputPointType) = 0;
    virtual HRESULT __stdcall set_InputPointType(long InputPointType) = 0;
    virtual HRESULT __stdcall get_InputSpaceID(long *InputSpaceID) = 0;
    virtual HRESULT __stdcall set_InputSpaceID(long InputSpaceID) = 0;
    virtual HRESULT __stdcall get_OutputPointType(long *OutputPointType) = 0;
    virtual HRESULT __stdcall set_OutputPointType(long OutputPointType) = 0;
    virtual HRESULT __stdcall get_OutputSpaceID(long *OutputSpaceID) = 0;
    virtual HRESULT __stdcall set_OutputSpaceID(long OutputSpaceID) = 0;
    virtual HRESULT __stdcall get_ConstraintsUsage(VARIANT_BOOL *ConstraintsUsage) = 0;
    virtual HRESULT __stdcall set_ConstraintsUsage(VARIANT_BOOL ConstraintsUsage) = 0;
    virtual HRESULT __stdcall get_SingleParameterConstraints(DGMSingleParameterConstraints* *SingleParameterConstraints) = 0;
    virtual HRESULT __stdcall get_Observations(DGMObservations* *Observations) = 0;
    virtual HRESULT __stdcall get_LeastSquaresModelTypeStrings(DGMStringCollection* *LeastSquaresModelTypeStrings) = 0;
    virtual HRESULT __stdcall CopyTo(DGMLeastSquares* LeastSquaresDest) = 0;
    virtual HRESULT __stdcall IsTransformationValid(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, VARIANT_BOOL *IsValid) = 0;
    virtual HRESULT __stdcall TransformPoints(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, long Count, SAFEARRAY * *Points) = 0;
    virtual HRESULT __stdcall TransformPoint(long InputPointType, long InputSpaceID,
      long OutputPointType, long OutputSpaceID, double *X, double *Y, double *Z) = 0;
    virtual HRESULT __stdcall CalculateTransformation() = 0;
};

struct DGMLeastSquaresDisp : IDispatch
{
    LPTSTR get_Name();
    void set_Name(LPTSTR Value);
    LPTSTR get_Description();
    void set_Description(LPTSTR Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    long get_Degree();
    void set_Degree(long Value);
    long get_NumberControlPointsNeeded();
    long get_DegreesOfFreedom();
    BOOL get_Is3d();
    BOOL get_IsLinear();
    long get_WeightsUsage();
    void set_WeightsUsage(long Value);
    long get_NumberTransformationParameters();
    SAFEARRAY * get_TransformationParameters();
    SAFEARRAY * get_ParameterCovarianceMatrix();
    long get_ObservationMatrixSize();
    SAFEARRAY * get_AdjObservationCovarianceMatrix();
    SAFEARRAY * get_ResidualCovarianceMatrix();
    long get_HomogeneousParamMatrixType();
    void set_HomogeneousParamMatrixType(long Value);
    SAFEARRAY * get_HomogeneousParameterMatrix();
    double get_VarianceFactor();
    double get_RootMeanSquareOfResiduals();
    BOOL get_ApplyResidualSmoothing();
    void set_ApplyResidualSmoothing(BOOL Value);
    double get_ResidualSmoothingExponent();
    void set_ResidualSmoothingExponent(double Value);
    long get_InputPointType();
    void set_InputPointType(long Value);
    long get_InputSpaceID();
    void set_InputSpaceID(long Value);
    long get_OutputPointType();
    void set_OutputPointType(long Value);
    long get_OutputSpaceID();
    void set_OutputSpaceID(long Value);
    BOOL get_ConstraintsUsage();
    void set_ConstraintsUsage(BOOL Value);
    DGMSingleParameterConstraints* get_SingleParameterConstraints();
    DGMObservations* get_Observations();
    DGMStringCollection* get_LeastSquaresModelTypeStrings();
    void CopyTo(DGMLeastSquares* LeastSquaresDest);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    void CalculateTransformation();
};

struct DGMSingleParameterConstraints : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall Add(DGMSingleParameterConstraint* *Constraint) = 0;
    virtual HRESULT __stdcall Delete(long index) = 0;
    virtual HRESULT __stdcall DeleteAll() = 0;
    virtual HRESULT __stdcall Item(long index, DGMSingleParameterConstraint* *Constraint) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppEnum) = 0;
};

struct DGMSingleParameterConstraintsDisp : IDispatch
{
    long get_Count();
    DGMSingleParameterConstraint* Add();
    void Delete(long index);
    void DeleteAll();
    DGMSingleParameterConstraint* Item(long index);
    IUnknown * _NewEnum();
};

struct DGMSingleParameterConstraint : IDispatch
{
    virtual HRESULT __stdcall get_TransformationParameterIndex(long *TransformationParameterIndex) = 0;
    virtual HRESULT __stdcall set_TransformationParameterIndex(long TransformationParameterIndex) = 0;
    virtual HRESULT __stdcall get_Value(double *Value) = 0;
    virtual HRESULT __stdcall set_Value(double Value) = 0;
};

struct DGMSingleParameterConstraintDisp : IDispatch
{
    long get_TransformationParameterIndex();
    void set_TransformationParameterIndex(long Value);
    double get_Value();
    void set_Value(double Value);
};

struct DGMObservations : IDispatch
{
    virtual HRESULT __stdcall get_Count(long *Count) = 0;
    virtual HRESULT __stdcall get_ControlPointCount(long *ControlPointCount) = 0;
    virtual HRESULT __stdcall get_CheckPointCount(long *CheckPointCount) = 0;
    virtual HRESULT __stdcall Add(DGMObservation* *Observation) = 0;
    virtual HRESULT __stdcall Delete(VARIANT index) = 0;
    virtual HRESULT __stdcall DeleteAll() = 0;
    virtual HRESULT __stdcall Item(VARIANT index, DGMObservation* *Observation) = 0;
    virtual HRESULT __stdcall _NewEnum(IUnknown * *ppEnum) = 0;
};

struct DGMObservationsDisp : IDispatch
{
    long get_Count();
    long get_ControlPointCount();
    long get_CheckPointCount();
    DGMObservation* Add();
    void Delete(VARIANT index);
    void DeleteAll();
    DGMObservation* Item(VARIANT index);
    IUnknown * _NewEnum();
};

struct DGMObservation : IDispatch
{
    virtual HRESULT __stdcall get_Name(BSTR *Name) = 0;
    virtual HRESULT __stdcall set_Name(BSTR Name) = 0;
    virtual HRESULT __stdcall get_Description(BSTR *Description) = 0;
    virtual HRESULT __stdcall set_Description(BSTR Description) = 0;
    virtual HRESULT __stdcall get_System1X(double *System1X) = 0;
    virtual HRESULT __stdcall set_System1X(double System1X) = 0;
    virtual HRESULT __stdcall get_System1Y(double *System1Y) = 0;
    virtual HRESULT __stdcall set_System1Y(double System1Y) = 0;
    virtual HRESULT __stdcall get_System1Z(double *System1Z) = 0;
    virtual HRESULT __stdcall set_System1Z(double System1Z) = 0;
    virtual HRESULT __stdcall get_System2X(double *System2X) = 0;
    virtual HRESULT __stdcall set_System2X(double System2X) = 0;
    virtual HRESULT __stdcall get_System2Y(double *System2Y) = 0;
    virtual HRESULT __stdcall set_System2Y(double System2Y) = 0;
    virtual HRESULT __stdcall get_System2Z(double *System2Z) = 0;
    virtual HRESULT __stdcall set_System2Z(double System2Z) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem1X(double *AdjustedSystem1X) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem1Y(double *AdjustedSystem1Y) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem1Z(double *AdjustedSystem1Z) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem2X(double *AdjustedSystem2X) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem2Y(double *AdjustedSystem2Y) = 0;
    virtual HRESULT __stdcall get_AdjustedSystem2Z(double *AdjustedSystem2Z) = 0;
    virtual HRESULT __stdcall get_System1StdDeviationX(double *System1StdDeviationX) = 0;
    virtual HRESULT __stdcall set_System1StdDeviationX(double System1StdDeviationX) = 0;
    virtual HRESULT __stdcall get_System1StdDeviationY(double *System1StdDeviationY) = 0;
    virtual HRESULT __stdcall set_System1StdDeviationY(double System1StdDeviationY) = 0;
    virtual HRESULT __stdcall get_System1StdDeviationZ(double *System1StdDeviationZ) = 0;
    virtual HRESULT __stdcall set_System1StdDeviationZ(double System1StdDeviationZ) = 0;
    virtual HRESULT __stdcall get_System2StdDeviationX(double *System2StdDeviationX) = 0;
    virtual HRESULT __stdcall set_System2StdDeviationX(double System2StdDeviationX) = 0;
    virtual HRESULT __stdcall get_System2StdDeviationY(double *System2StdDeviationY) = 0;
    virtual HRESULT __stdcall set_System2StdDeviationY(double System2StdDeviationY) = 0;
    virtual HRESULT __stdcall get_System2StdDeviationZ(double *System2StdDeviationZ) = 0;
    virtual HRESULT __stdcall set_System2StdDeviationZ(double System2StdDeviationZ) = 0;
    virtual HRESULT __stdcall get_Type(long *Type) = 0;
    virtual HRESULT __stdcall set_Type(long Type) = 0;
    virtual HRESULT __stdcall get_LinearModelResidual(DGMLinearModelResidual* *LinearModelResidual) = 0;
    virtual HRESULT __stdcall get_NonlinearModelResidual(DGMNonlinearModelResidual* *NonlinearModelResidual) = 0;
};

struct DGMObservationDisp : IDispatch
{
    LPTSTR get_Name();
    void set_Name(LPTSTR Value);
    LPTSTR get_Description();
    void set_Description(LPTSTR Value);
    double get_System1X();
    void set_System1X(double Value);
    double get_System1Y();
    void set_System1Y(double Value);
    double get_System1Z();
    void set_System1Z(double Value);
    double get_System2X();
    void set_System2X(double Value);
    double get_System2Y();
    void set_System2Y(double Value);
    double get_System2Z();
    void set_System2Z(double Value);
    double get_AdjustedSystem1X();
    double get_AdjustedSystem1Y();
    double get_AdjustedSystem1Z();
    double get_AdjustedSystem2X();
    double get_AdjustedSystem2Y();
    double get_AdjustedSystem2Z();
    double get_System1StdDeviationX();
    void set_System1StdDeviationX(double Value);
    double get_System1StdDeviationY();
    void set_System1StdDeviationY(double Value);
    double get_System1StdDeviationZ();
    void set_System1StdDeviationZ(double Value);
    double get_System2StdDeviationX();
    void set_System2StdDeviationX(double Value);
    double get_System2StdDeviationY();
    void set_System2StdDeviationY(double Value);
    double get_System2StdDeviationZ();
    void set_System2StdDeviationZ(double Value);
    long get_Type();
    void set_Type(long Value);
    DGMLinearModelResidual* get_LinearModelResidual();
    DGMNonlinearModelResidual* get_NonlinearModelResidual();
};

struct DGMLinearModelResidual : IDispatch
{
    virtual HRESULT __stdcall get_X(double *X) = 0;
    virtual HRESULT __stdcall get_Y(double *Y) = 0;
    virtual HRESULT __stdcall get_Z(double *Z) = 0;
    virtual HRESULT __stdcall get_Norm(double *Norm) = 0;
    virtual HRESULT __stdcall get_StandardizedX(double *StandardizedX) = 0;
    virtual HRESULT __stdcall get_StandardizedY(double *StandardizedY) = 0;
    virtual HRESULT __stdcall get_StandardizedZ(double *StandardizedZ) = 0;
    virtual HRESULT __stdcall get_StandardizedNorm(double *StandardizedNorm) = 0;
};

struct DGMLinearModelResidualDisp : IDispatch
{
    double get_X();
    double get_Y();
    double get_Z();
    double get_Norm();
    double get_StandardizedX();
    double get_StandardizedY();
    double get_StandardizedZ();
    double get_StandardizedNorm();
};

struct DGMNonlinearModelResidual : IDispatch
{
    virtual HRESULT __stdcall get_System1X(double *System1X) = 0;
    virtual HRESULT __stdcall get_System1Y(double *System1Y) = 0;
    virtual HRESULT __stdcall get_System1Z(double *System1Z) = 0;
    virtual HRESULT __stdcall get_System1Norm(double *System1Norm) = 0;
    virtual HRESULT __stdcall get_System2X(double *System2X) = 0;
    virtual HRESULT __stdcall get_System2Y(double *System2Y) = 0;
    virtual HRESULT __stdcall get_System2Z(double *System2Z) = 0;
    virtual HRESULT __stdcall get_System2Norm(double *System2Norm) = 0;
    virtual HRESULT __stdcall get_System1StandardizedX(double *System1StandardizedX) = 0;
    virtual HRESULT __stdcall get_System1StandardizedY(double *System1StandardizedY) = 0;
    virtual HRESULT __stdcall get_System1StandardizedZ(double *System1StandardizedZ) = 0;
    virtual HRESULT __stdcall get_System1StandardizedNorm(double *System1StandardizedNorm) = 0;
    virtual HRESULT __stdcall get_System2StandardizedX(double *System2StandardizedX) = 0;
    virtual HRESULT __stdcall get_System2StandardizedY(double *System2StandardizedY) = 0;
    virtual HRESULT __stdcall get_System2StandardizedZ(double *System2StandardizedZ) = 0;
    virtual HRESULT __stdcall get_System2StandardizedNorm(double *System2StandardizedNorm) = 0;
    virtual HRESULT __stdcall get_Sys2MinusTransformedSys1X(double *Sys2MinusTransformedSys1X) = 0;
    virtual HRESULT __stdcall get_Sys2MinusTransformedSys1Y(double *Sys2MinusTransformedSys1Y) = 0;
    virtual HRESULT __stdcall get_Sys2MinusTransformedSys1Z(double *Sys2MinusTransformedSys1Z) = 0;
    virtual HRESULT __stdcall get_Sys2MinusTransformedSys1Norm(double *Sys2MinusTransformedSys1Norm) = 0;
};

struct DGMNonlinearModelResidualDisp : IDispatch
{
    double get_System1X();
    double get_System1Y();
    double get_System1Z();
    double get_System1Norm();
    double get_System2X();
    double get_System2Y();
    double get_System2Z();
    double get_System2Norm();
    double get_System1StandardizedX();
    double get_System1StandardizedY();
    double get_System1StandardizedZ();
    double get_System1StandardizedNorm();
    double get_System2StandardizedX();
    double get_System2StandardizedY();
    double get_System2StandardizedZ();
    double get_System2StandardizedNorm();
    double get_Sys2MinusTransformedSys1X();
    double get_Sys2MinusTransformedSys1Y();
    double get_Sys2MinusTransformedSys1Z();
    double get_Sys2MinusTransformedSys1Norm();
};

struct UnitAndFormatSpecEvents : IDispatch
{
    virtual HRESULT __stdcall Change() = 0;
};

UnitAndFormatSpec* CreateUnitAndFormatSpec();

CoordSystem* CreateCoordSystem();

struct DGMDatumTransformation2 : DGMDatumTransformation
{
    virtual HRESULT __stdcall get_UserSuppliedConfigurationParameters(BSTR *UserSuppliedConfigurationParameters) = 0;
    virtual HRESULT __stdcall set_UserSuppliedConfigurationParameters(BSTR UserSuppliedConfigurationParameters) = 0;
};

struct DGMDatumTransformation2Disp : IDispatch
{
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    long get_ForwardInputSpaceGeodeticDatumVal();
    void set_ForwardInputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardInputSpaceGeodeticDatumStr();
    void set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_ForwardOutputSpaceGeodeticDatumVal();
    void set_ForwardOutputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardOutputSpaceGeodeticDatumStr();
    void set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_Direction();
    void set_Direction(long Value);
    LPTSTR get_Name();
    void set_Name(LPTSTR Value);
    LPTSTR get_Description();
    void set_Description(LPTSTR Value);
    DGMBursaWolf* get_BursaWolf();
    DGMComplexPolynomial* get_ComplexPolynomial();
    DGMMultipleRegression* get_MultipleRegression();
    DGMSecondDegreePolynomial* get_SecondDegreePolynomial();
    DGMStandardMolodensky* get_StandardMolodensky();
    DGMStringCollection* get_DatumTransModelTypeStrings();
    DGMStringCollection* get_GeodeticDatumStrings();
    void CopyTo(DGMDatumTransformation* DatumTransformationDest);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    LPTSTR get_UserSuppliedConfigurationParameters();
    void set_UserSuppliedConfigurationParameters(LPTSTR Value);
};

struct DGMDatumTransformation3 : DGMDatumTransformation2
{
    virtual HRESULT __stdcall get_ForwardInputSpaceVerticalDatumVal(long *ForwardInputSpaceVerticalDatumVal) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceVerticalDatumVal(long ForwardInputSpaceVerticalDatumVal) = 0;
    virtual HRESULT __stdcall get_ForwardInputSpaceVerticalDatumStr(BSTR *ForwardInputSpaceVerticalDatumStr) = 0;
    virtual HRESULT __stdcall set_ForwardInputSpaceVerticalDatumStr(BSTR ForwardInputSpaceVerticalDatumStr) = 0;
    virtual HRESULT __stdcall get_ForwardOutputSpaceVerticalDatumVal(long *ForwardOutputSpaceVerticalDatumVal) = 0;
    virtual HRESULT __stdcall set_ForwardOutputSpaceVerticalDatumVal(long ForwardOutputSpaceVerticalDatumVal) = 0;
    virtual HRESULT __stdcall get_ForwardOutputSpaceVerticalDatumStr(BSTR *ForwardOutputSpaceVerticalDatumStr) = 0;
    virtual HRESULT __stdcall set_ForwardOutputSpaceVerticalDatumStr(BSTR ForwardOutputSpaceVerticalDatumStr) = 0;
    virtual HRESULT __stdcall get_VerticalDatumStrings(DGMStringCollection* *VerticalDatumStrings) = 0;
};

struct DGMDatumTransformation3Disp : IDispatch
{
    long get_ModelTypeVal();
    void set_ModelTypeVal(long Value);
    LPTSTR get_ModelTypeStr();
    void set_ModelTypeStr(LPTSTR Value);
    long get_ForwardInputSpaceGeodeticDatumVal();
    void set_ForwardInputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardInputSpaceGeodeticDatumStr();
    void set_ForwardInputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_ForwardOutputSpaceGeodeticDatumVal();
    void set_ForwardOutputSpaceGeodeticDatumVal(long Value);
    LPTSTR get_ForwardOutputSpaceGeodeticDatumStr();
    void set_ForwardOutputSpaceGeodeticDatumStr(LPTSTR Value);
    long get_Direction();
    void set_Direction(long Value);
    LPTSTR get_Name();
    void set_Name(LPTSTR Value);
    LPTSTR get_Description();
    void set_Description(LPTSTR Value);
    DGMBursaWolf* get_BursaWolf();
    DGMComplexPolynomial* get_ComplexPolynomial();
    DGMMultipleRegression* get_MultipleRegression();
    DGMSecondDegreePolynomial* get_SecondDegreePolynomial();
    DGMStandardMolodensky* get_StandardMolodensky();
    DGMStringCollection* get_DatumTransModelTypeStrings();
    DGMStringCollection* get_GeodeticDatumStrings();
    void CopyTo(DGMDatumTransformation* DatumTransformationDest);
    void IsTransformationValid(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, VARIANT_BOOL *IsValid);
    void TransformPoints(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, long Count, SAFEARRAY * *Points);
    void TransformPoint(long InputPointType, long InputSpaceID, long OutputPointType,
      long OutputSpaceID, double *X, double *Y, double *Z);
    LPTSTR get_UserSuppliedConfigurationParameters();
    void set_UserSuppliedConfigurationParameters(LPTSTR Value);
    long get_ForwardInputSpaceVerticalDatumVal();
    void set_ForwardInputSpaceVerticalDatumVal(long Value);
    LPTSTR get_ForwardInputSpaceVerticalDatumStr();
    void set_ForwardInputSpaceVerticalDatumStr(LPTSTR Value);
    long get_ForwardOutputSpaceVerticalDatumVal();
    void set_ForwardOutputSpaceVerticalDatumVal(long Value);
    LPTSTR get_ForwardOutputSpaceVerticalDatumStr();
    void set_ForwardOutputSpaceVerticalDatumStr(LPTSTR Value);
    DGMStringCollection* get_VerticalDatumStrings();
};

DatumTransformation* CreateDatumTransformation();

CoordSystemsMgr* CreateCoordSystemsMgr();

UnitsOfMeasure* CreateUnitsOfMeasure();

BursaWolf* CreateBursaWolf();

ComplexPolynomial* CreateComplexPolynomial();

MultipleRegression* CreateMultipleRegression();

SecondDegreePolynomial* CreateSecondDegreePolynomial();

StandardMolodensky* CreateStandardMolodensky();

LeastSquares* CreateLeastSquares();

SingleParameterConstraints* CreateSingleParameterConstraints();

SingleParameterConstraint* CreateSingleParameterConstraint();

Observations* CreateObservations();

Observation* CreateObservation();

LinearModelResidual* CreateLinearModelResidual();

NonlinearModelResidual* CreateNonlinearModelResidual();

#ifdef __cplusplus
}
#endif
#endif
