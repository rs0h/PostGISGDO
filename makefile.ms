# Copyright 2011 Intergraph Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

VPATH=.\build
libpath="C:\Program Files\Microsoft SDKs\Windows\v7.0A\Lib"
libpath2="C:\Program Files\Microsoft Visual Studio 10.0\VC\Lib"

inclpath="C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include"
inclpath2="C:\Program Files\Microsoft Visual Studio 10.0\VC\include"

BUILDDIR=build^\

CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /D_WIN32_WINNT=0x501 \
# CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /DDBGLEVEL=1 /D_WIN32_WINNT=0x501 \
/D_USE_MATH_DEFINES \
/I"C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include" \
/I"C:\Program Files\Microsoft Visual Studio 10.0\VC\include" \
/I"C:\pgsql\include"

# CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_DEPRECATE \
# /D_CRT_NON_CONFORMING_SWPRINTFS \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\include"

RCFLAGS=-D_WIN32_IE=0x0400

# ole32 defines CoInitialize, CoUninitialize
# oleaut32 defines CoCreateInstance
# uuid defines GUID_NULL

PostGISobj = PostGIS.obj PostGIS.res PostGISClsFactory.obj \
    LogFiles.obj VarUtils.obj CComTypes.obj PostGISGDO.obj GDOUtils.obj \
    PGSIndex.obj PGSTableDef.obj PGSRecordset.obj PGtoGDOmaps.obj PGSGeom.obj \
    PGSTypes.obj PGSCols.obj PGSCoordSystems.obj EWKBtoGDO.obj GDOtoEWKB.obj
PostGISmod = $(BUILDDIR)PostGIS.obj $(BUILDDIR)PostGIS.res \
    $(BUILDDIR)PostGISClsFactory.obj $(BUILDDIR)LogFiles.obj $(BUILDDIR)VarUtils.obj \
    $(BUILDDIR)CComTypes.obj $(BUILDDIR)PostGISGDO.obj \
    $(BUILDDIR)GDOUtils.obj $(BUILDDIR)PGSIndex.obj \
    $(BUILDDIR)PGSTableDef.obj $(BUILDDIR)PGSRecordset.obj \
    $(BUILDDIR)PGtoGDOmaps.obj $(BUILDDIR)PGSGeom.obj $(BUILDDIR)PGSTypes.obj \
    $(BUILDDIR)PGSCols.obj $(BUILDDIR)PGSCoordSystems.obj \
    $(BUILDDIR)EWKBtoGDO.obj $(BUILDDIR)GDOtoEWKB.obj

MakePostGIS : $(PostGISobj)
	CL /LD PostGISGDO\PostGISms.def $(PostGISmod) /link /libpath:$(libpath) \
	/libpath:$(libpath2) /libpath:"C:\pgsql\lib" /out:$(BUILDDIR)PostGISGDO.dll \
	/defaultlib:ole32.lib /defaultlib:oleaut32.lib /defaultlib:user32.lib \
	/defaultlib:uuid.lib /defaultlib:rpcrt4.lib /defaultlib:odbc32.lib \
	/defaultlib:libpq.lib /implib:$(BUILDDIR)PostGISGDO.lib

# Common sources
VarUtils.obj : Common\VarUtils.cpp Common\VarUtils.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)VarUtils.obj Common\VarUtils.cpp
LogFiles.obj : Common\LogFiles.cpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)LogFiles.obj Common\LogFiles.cpp
CComTypes.obj : Common\CComTypes.cpp Common\CComTypes.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\CComTypes.obj Common\CComTypes.cpp
PGSCoordSystems.obj : Common\PGSCoordSystems.cpp Common\PGSCoordSystems.hpp \
    Common\PGtoGDOmaps.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\PGSCoordSystems.obj Common\PGSCoordSystems.cpp
GDOUtils.obj : Common\GDOUtils.cpp Common\GDOUtils.hpp \
    Common\PGSCoordSystems.hpp Common\LogFiles.hpp Common\PGtoGDOmaps.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\GDOUtils.obj Common\GDOUtils.cpp
PCSS_TLB.obj : Common\PCSS_TLB.cpp Common\PCSS_TLB.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\PCSS_TLB.obj Common\PCSS_TLB.cpp
PGtoGDOmaps.obj : Common\PGtoGDOmaps.cpp Common\PGtoGDOmaps.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\PGtoGDOmaps.obj Common\PGtoGDOmaps.cpp
EWKBtoGDO.obj : Common\EWKBtoGDO.cpp Common\EWKBtoGDO.hpp \
    Common\PGSTypes.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\EWKBtoGDO.obj Common\EWKBtoGDO.cpp
GDOtoEWKB.obj : Common\GDOtoEWKB.cpp Common\GDOtoEWKB.hpp \
    Common\PGSTypes.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\GDOtoEWKB.obj Common\GDOtoEWKB.cpp
PGSTypes.obj : Common\PGSTypes.cpp Common\PGSTypes.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)\PGSTypes.obj Common\PGSTypes.cpp

# PostGIS sources
PostGIS.obj : PostGISGDO\PostGIS.cpp PostGISGDO\PostGISClsFactory.hpp \
    PostGISGDO\PostGISGDO.hpp Common\Registrar.hpp Common\LogFiles.hpp \
    PostGISGDO\PostGIS.rh Common\VarUtils.hpp
    CL $(CFLAGS) /Fobuild\PostGIS.obj PostGISGDO\PostGIS.cpp
PostGIS.res : PostGISGDO\PostGIS.rc PostGISGDO\PostGIS.rh \
    PostGISGDO\PostGIS.rgs Common\csres.bin
	rc /i $(inclpath) /i $(inclpath2) /d_WIN32_IE=0x0400 /d_MSCOMP_ /fo \
	$(BUILDDIR)$@ PostGISGDO\PostGIS.rc
PostGISClsFactory.obj : PostGISGDO\PostGISClsFactory.cpp \
    PostGISGDO\PostGISClsFactory.hpp PostGISGDO\PostGISGDO.hpp
    CL $(CFLAGS) /Fobuild\PostGISClsFactory.obj PostGISGDO\PostGISClsFactory.cpp
PostGISGDO.obj : PostGISGDO\PostGISGDO.cpp PostGISGDO\PostGISGDO.hpp \
    Common\CComTypes.hpp Common\GDOUtils.hpp Common\PGSTypes.hpp \
    PostGISGDO\PGSGeom.hpp PostGISGDO\PGSRecordset.hpp \
    PostGISGDO\PGSTableDef.hpp Common\PGtoGDOmaps.hpp
    CL $(CFLAGS) /Fobuild\PostGISGDO.obj PostGISGDO\PostGISGDO.cpp
PGSRecordset.obj : PostGISGDO\PGSRecordset.cpp PostGISGDO\PGSRecordset.hpp \
    Common\CComTypes.hpp Common\GDOUtils.hpp Common\PGSTypes.hpp \
    Common\PGtoGDOmaps.hpp Common\LogFiles.hpp PostGISGDO\PGSCols.hpp \
    PostGISGDO\PGSGeom.hpp PostGISGDO\PGSTableDef.hpp \
    PostGISGDO\PostGISGDO.hpp Common\GDOtoEWKB.hpp Common\EWKBtoGDO.hpp \
    Common\PGSCoordSystems.hpp PostGISGDO\PostGIS.rh
    CL $(CFLAGS) /Fobuild\PGSRecordset.obj PostGISGDO\PGSRecordset.cpp
PGSTableDef.obj : PostGISGDO\PGSTableDef.cpp PostGISGDO\PGSTableDef.hpp \
    Common\CComTypes.hpp PostGISGDO\PGSCols.hpp Common\PGSTypes.hpp \
    Common\LogFiles.hpp Common\PGtoGDOmaps.hpp PostGISGDO\PGSIndex.hpp
    CL $(CFLAGS) /Fobuild\PGSTableDef.obj PostGISGDO\PGSTableDef.cpp
PGSIndex.obj : PostGISGDO\PGSIndex.cpp PostGISGDO\PGSIndex.hpp \
    Common\CComTypes.hpp PostGISGDO\PGSCols.hpp Common\PGSTypes.hpp \
    Common\PGtoGDOmaps.hpp Common\LogFiles.hpp PostGISGDO\PostGIS.rh \
    PostGISGDO\PGSGeom.hpp Common\GDOUtils.hpp
    CL $(CFLAGS) /Fobuild\PGSIndex.obj PostGISGDO\PGSIndex.cpp
PGSGeom.obj : PostGISGDO\PGSGeom.cpp PostGISGDO\PGSGeom.hpp \
    Common\PGtoGDOmaps.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fobuild\PGSGeom.obj PostGISGDO\PGSGeom.cpp
PGSCols.obj : PostGISGDO\PGSCols.cpp PostGISGDO\PGSCols.hpp PostGISGDO\PostGIS.rh \
    Common\LogFiles.hpp Common\CComTypes.hpp Common\PGSTypes.hpp \
    PostGISGDO\PGSGeom.hpp
    CL $(CFLAGS) /Fobuild\PGSCols.obj PostGISGDO\PGSCols.cpp

GMPsgCtlobj = GMPsgCtl.obj GMPsgCtl.res GMPsgCtlClsFactory.obj \
    LogFiles.obj CComTypes.obj GMPsgCtlIntfc.obj
GMPsgCtlmod = $(BUILDDIR)GMPsgCtl.obj $(BUILDDIR)GMPsgCtl.res \
    $(BUILDDIR)GMPsgCtlClsFactory.obj $(BUILDDIR)LogFiles.obj \
    $(BUILDDIR)CComTypes.obj $(BUILDDIR)GMPsgCtlIntfc.obj

MakeGMPsgCtl : $(GMPsgCtlobj)
	CL /LD GMPsgCtl\GMPsgCtlms.def $(GMPsgCtlmod) /link /libpath:$(libpath) \
	/libpath:$(libpath2) /out:$(BUILDDIR)GMPsgCtl.ocx \
	/defaultlib:ole32.lib /defaultlib:oleaut32.lib /defaultlib:user32.lib \
	/defaultlib:gdi32.lib \
	/implib:$(BUILDDIR)GMPsgCtl.lib

# GMPsgCtl sources
GMPsgCtl.obj : GMPsgCtl\GMPsgCtl.cpp GMPsgCtl\GMPsgCtlClsFactory.hpp \
    GMPsgCtl\GMPsgCtlIntfc.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fobuild\GMPsgCtl.obj GMPsgCtl\GMPsgCtl.cpp
GMPsgCtl.res : GMPsgCtl\GMPsgCtl.rc GMPsgCtl\GMPsgCtl.rh GMPsgCtl\GMPsgCtl.rgs \
    GMPsgCtl\GMPsgCtl.tlb
	rc /i $(inclpath) /i $(inclpath2) /d_WIN32_IE=0x0400 /d_MSCOMP_ /fo \
	$(BUILDDIR)$@ GMPsgCtl\GMPsgCtl.rc
GMPsgCtlClsFactory.obj : GMPsgCtl\GMPsgCtlClsFactory.cpp \
    GMPsgCtl\GMPsgCtlClsFactory.hpp GMPsgCtl\GMPsgCtlIntfc.hpp \
    Common\LogFiles.hpp Common\CComTypes.hpp
    CL $(CFLAGS) /Fobuild\GMPsgCtlClsFactory.obj GMPsgCtl\GMPsgCtlClsFactory.cpp
GMPsgCtlIntfc.obj : GMPsgCtl\GMPsgCtlIntfc.cpp GMPsgCtl\GMPsgCtlIntfc.hpp \
    Common\LogFiles.hpp Common\CComTypes.hpp
    CL $(CFLAGS) /Fobuild\GMPsgCtlIntfc.obj GMPsgCtl\GMPsgCtlIntfc.cpp

PsgDBUtilsobj = PsgDBUtils.obj PsgDBUMainDlg.obj PsgDBUtils.res \
    PsgDBUConnDlg.obj PsgDBUFtrClsDlg.obj PsgDBUFldEdtDlg.obj \
    PsgDBUUsrDlg.obj VarUtils.obj PsgDBUSpecFldEdt.obj
PsgDBUtilsmod = $(BUILDDIR)PsgDBUtils.obj $(BUILDDIR)PsgDBUMainDlg.obj \
    $(BUILDDIR)PsgDBUtils.res $(BUILDDIR)PsgDBUConnDlg.obj \
    $(BUILDDIR)PsgDBUFtrClsDlg.obj $(BUILDDIR)PsgDBUFldEdtDlg.obj \
    $(BUILDDIR)PsgDBUUsrDlg.obj $(BUILDDIR)VarUtils.obj \
    $(BUILDDIR)PsgDBUSpecFldEdt.obj

MakePsgDBUtils : $(PsgDBUtilsobj)
	CL $(PsgDBUtilsmod) /link /libpath:$(libpath) /libpath:$(libpath2) \
	/out:$(BUILDDIR)PsgDBUtils.exe \
	/defaultlib:ole32.lib /defaultlib:oleaut32.lib /defaultlib:uuid.lib \
	/defaultlib:gdi32.lib /defaultlib:comctl32.lib /defaultlib:rpcrt4.lib \
	/defaultlib:Comdlg32.lib /defaultlib:user32.lib /defaultlib:odbc32.lib \
	/defaultlib:$(BUILDDIR)PsgDBUtilsDLL.lib

# PsgDBUtils sources
PsgDBUtils.obj : PsgDBUtils\PsgDBUtils.cpp PsgDBUtils\PsgDBUMainDlg.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUtils.obj PsgDBUtils\PsgDBUtils.cpp
PsgDBUtils.res : PsgDBUtils\PsgDBUtils.rc PsgDBUtils\PsgDBUtils.rh \
    PsgDBUtils\PsgDBUtils.exe.manifest.32 Common\csres.bin
	rc /i $(inclpath) /i $(inclpath2) /d_WIN32_IE=0x0400 /d_MSCOMP_ /fo \
	$(BUILDDIR)$@ PsgDBUtils\PsgDBUtils.rc
PsgDBUMainDlg.obj : PsgDBUtils\PsgDBUMainDlg.cpp PsgDBUtils\PsgDBUMainDlg.hpp \
    PsgDBUtils\PsgDBUConnDlg.hpp Common\GDOUtils.hpp Common\LogFiles.hpp \
    PsgDBUtils\PsgDBUFtrClsDlg.hpp PsgDBUtils\PsgDBUUsrDlg.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUMainDlg.obj PsgDBUtils\PsgDBUMainDlg.cpp
PsgDBUConnDlg.obj : PsgDBUtils\PsgDBUConnDlg.cpp PsgDBUtils\PsgDBUConnDlg.hpp \
    Common\LogFiles.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUConnDlg.obj PsgDBUtils\PsgDBUConnDlg.cpp
PsgDBUFtrClsDlg.obj : PsgDBUtils\PsgDBUFtrClsDlg.cpp Common\LogFiles.hpp \
    PsgDBUtils\PsgDBUFtrClsDlg.hpp PsgDBUtils\PsgDBUFldEdtDlg.hpp \
    PsgDBUtils\PsgDBUtils.rh
    CL $(CFLAGS) /Fobuild\PsgDBUFtrClsDlg.obj PsgDBUtils\PsgDBUFtrClsDlg.cpp
PsgDBUFldEdtDlg.obj : PsgDBUtils\PsgDBUFldEdtDlg.cpp Common\LogFiles.hpp \
    PsgDBUtils\PsgDBUFldEdtDlg.hpp PsgDBUtils\PsgDBUtils.rh
    CL $(CFLAGS) /Fobuild\PsgDBUFldEdtDlg.obj PsgDBUtils\PsgDBUFldEdtDlg.cpp
PsgDBUUsrDlg.obj : PsgDBUtils\PsgDBUUsrDlg.cpp Common\VarUtils.hpp \
    PsgDBUtils\PsgDBUUsrDlg.hpp PsgDBUtils\PsgDBUtils.rh
    CL $(CFLAGS) /Fobuild\PsgDBUUsrDlg.obj PsgDBUtils\PsgDBUUsrDlg.cpp
PsgDBUSpecFldEdt.obj : PsgDBUtils\PsgDBUSpecFldEdt.cpp Common\VarUtils.hpp \
    PsgDBUtils\PsgDBUSpecFldEdt.hpp PsgDBUtils\PsgDBUtils.rh \
    Common\PsgDBUInterfaces.hpp

MakeCSResobj = MakeCSRes.obj PCSS_TLB.obj PGtoGDOmaps.obj VarUtils.obj LogFiles.obj
MakeCSResmod = $(BUILDDIR)MakeCSRes.obj $(BUILDDIR)PCSS_TLB.obj \
    $(BUILDDIR)PGtoGDOmaps.obj $(BUILDDIR)VarUtils.obj $(BUILDDIR)LogFiles.obj

MakeMakeCSRes : $(MakeCSResobj)
	CL $(MakeCSResmod) /link /libpath:$(libpath) /libpath:$(libpath2) \
	/out:$(BUILDDIR)MakeCSRes.exe \
	/defaultlib:ole32.lib /defaultlib:oleaut32.lib /defaultlib:uuid.lib

# MakeCSRes sources
MakeCSRes.obj : MakeCSRes\MakeCSRes.cpp Common\PGtoGDOmaps.hpp \
    Common\PCSS_TLB.hpp
    CL $(CFLAGS) /Fobuild\MakeCSRes.obj MakeCSRes\MakeCSRes.cpp

PsgDBUtilsDLLobj = PsgDBUtilsDLL.obj PsgDBUtilsDLL.res PsgDBUDLLMain.obj \
    LogFiles.obj GDOUtils.obj VarUtils.obj PGSCoordSystems.obj \
    PGSTypes.obj EWKBtoGDO.obj GDOtoEWKB.obj PsgDBUTables.obj PGtoGDOmaps.obj
PsgDBUtilsDLLmod = $(BUILDDIR)PsgDBUtilsDLL.obj $(BUILDDIR)PsgDBUtilsDLL.res \
    $(BUILDDIR)PsgDBUDLLMain.obj $(BUILDDIR)LogFiles.obj $(BUILDDIR)GDOUtils.obj \
    $(BUILDDIR)VarUtils.obj $(BUILDDIR)PGSCoordSystems.obj \
    $(BUILDDIR)PGSTypes.obj $(BUILDDIR)EWKBtoGDO.obj $(BUILDDIR)GDOtoEWKB.obj \
    $(BUILDDIR)PsgDBUTables.obj $(BUILDDIR)PGtoGDOmaps.obj

MakePsgDBUtilsDLL : $(PsgDBUtilsDLLobj)
	CL /LD PsgDBUtilsDLL\PsgDBUtilsDLLms.def $(PsgDBUtilsDLLmod) /link \
	/libpath:$(libpath) /libpath:$(libpath2) /libpath:"C:\pgsql\lib" \
	/out:$(BUILDDIR)PsgDBUtils.dll /defaultlib:ole32.lib /defaultlib:oleaut32.lib \
	/defaultlib:uuid.lib /defaultlib:rpcrt4.lib /defaultlib:user32.lib \
	/defaultlib:odbc32.lib /defaultlib:libpq.lib \
	/implib:$(BUILDDIR)PsgDBUtilsDLL.lib

# PsgDBUtilsDLL sources
PsgDBUtilsDLL.obj : PsgDBUtilsDLL\PsgDBUtilsDLL.cpp PsgDBUtilsDLL\PsgDBUDLLMain.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUtilsDLL.obj PsgDBUtilsDLL\PsgDBUtilsDLL.cpp
PsgDBUtilsDLL.res : PsgDBUtilsDLL\PsgDBUtilsDLL.rc \
    PsgDBUtilsDLL\PsgDBUtilsDLL.rh Common\csres.bin
	rc /i $(inclpath) /i $(inclpath2) /d_WIN32_IE=0x0400 /d_MSCOMP_ /fo \
	$(BUILDDIR)$@ PsgDBUtilsDLL\PsgDBUtilsDLL.rc
PsgDBUDLLMain.obj : PsgDBUtilsDLL\PsgDBUDLLMain.cpp PsgDBUtilsDLL\PsgDBUDLLMain.hpp \
    Common\GDOUtils.hpp Common\VarUtils.hpp PsgDBUtilsDLL\PsgDBUTables.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUDLLMain.obj PsgDBUtilsDLL\PsgDBUDLLMain.cpp
PsgDBUTables.obj : PsgDBUtilsDLL\PsgDBUTables.cpp PsgDBUtilsDLL\PsgDBUTables.hpp \
    Common\PGtoGDOmaps.hpp Common\PGSCoordSystems.hpp Common\GDOUtils.hpp \
    Common\VarUtils.hpp Common\LogFiles.hpp Common\PsgDBUInterfaces.hpp \
    Common\GDOtoEWKB.hpp
    CL $(CFLAGS) /Fobuild\PsgDBUTables.obj PsgDBUtilsDLL\PsgDBUTables.cpp

#clean
clean:
	del build\*.obj
