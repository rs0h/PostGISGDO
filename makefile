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

VPATH=C:/MinGW32/Lib;./build
BUILDDIR=build/

# for PostGIS debug version define
GFLAGS=-DUNICODE -D_UNICODE -DDBGLEVEL=3 -DWINVER=0x0501 -D_WIN32_IE=0x0400
# for PostGIS define
# GFLAGS=-DUNICODE -D_UNICODE -DWINVER=0x0501 -D_WIN32_IE=0x0400
CFLAGS=-c -O2 -Ilibpq/Include $(GFLAGS)
LPQFLAGS=-c -O2 -Ilibpq/Include -Ilibpq/Include/win32 -DHAVE_LONG_INT_64 \
    -DHAVE_STRUCT_ADDRINFO -DHAVE_STRUCT_SOCKADDR_STORAGE \
    -DHAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY -DFRONTEND -DHAVE_GETTIMEOFDAY \
    -DENABLE_THREAD_SAFETY=1
RCFLAGS=-D_WIN32_IE=0x0400

# ole32 defines CoInitialize, CoUninitialize
# oleaut32 defines CoCreateInstance
# uuid defines GUID_NULL

libpqobj=fe-connect.o pqexpbuffer.o win32.o snprintf.o ip.o fe-secure.o \
    fe-misc.o noblock.o fe-protocol2.o fe-protocol3.o fe-auth.o fe-exec.o \
    strlcpy.o dirmod.o pthread-win32.o encnames.o pgstrcasecmp.o wchar.o \
    md5.o win32error.o pgsleep.o getaddrinfo.o inet_aton.o thread.o
libpqmod = $(addprefix $(BUILDDIR), $(libpqobj))

libpq : $(libpqobj)
	ar cr $(BUILDDIR)libpq.a $(libpqmod)

PostGISobj = PostGIS.o PostGISExp.o PostGIS_res.o PostGISClsFactory.o \
    LogFiles.o CComTypes.o PostGISGDO.o GDOUtils.o PGSIndex.o PGSTableDef.o \
    PGSRecordset.o PGtoGDOmaps.o PGSGeom.o PGSTypes.o PGSCols.o \
    PGSCoordSystems.o EWKBtoGDO.o GDOtoEWKB.o VarUtils.o
PostGISmod = $(addprefix $(BUILDDIR), $(PostGISobj)) \
	-lole32 -loleaut32 -luuid -lrpcrt4 -lpq -lshfolder -lwsock32 -lws2_32 \
	-lsecur32

PostGIS : $(PostGISobj)
	g++ -pie -s -L./build -o $(BUILDDIR)PostGISGDO.dll $(PostGISmod) \
	-static-libgcc -static-libstdc++ -mthreads -mdll

# Template command for compiling .cpp to .o
%.o : Common/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# Common sources
VarUtils.o : Common/VarUtils.cpp Common/VarUtils.hpp
LogFiles.o : Common/LogFiles.cpp Common/LogFiles.hpp
CComTypes.o : Common/CComTypes.cpp Common/CComTypes.hpp
PGSCoordSystems.o : Common/PGSCoordSystems.cpp Common/PGSCoordSystems.hpp \
    Common/VarUtils.hpp Common/GDOUtils.hpp
GDOUtils.o : Common/GDOUtils.cpp Common/GDOUtils.hpp Common/PGSCoordSystems.hpp \
    Common/LogFiles.hpp Common/VarUtils.hpp Common/pg_const.h \
    Common/EWKBtoGDO.hpp
PCSS_TLB.o : Common/PCSS_TLB.cpp Common/PCSS_TLB.hpp
PGtoGDOmaps.o : Common/PGtoGDOmaps.cpp Common/PGtoGDOmaps.hpp
EWKBtoGDO.o : Common/EWKBtoGDO.cpp Common/EWKBtoGDO.hpp \
    Common/PGSTypes.hpp Common/LogFiles.hpp
GDOtoEWKB.o : Common/GDOtoEWKB.cpp Common/GDOtoEWKB.hpp \
    Common/PGSTypes.hpp Common/LogFiles.hpp
PGSTypes.o : Common/PGSTypes.cpp Common/PGSTypes.hpp

# Template command for compiling .c to .o
%.o : libpq/%.c
	gcc $(LPQFLAGS) $< -o $(BUILDDIR)$@

# libpq source
fe-connect.o : libpq/fe-connect.c libpq/libpq-fe.h
pqexpbuffer.o : libpq/pqexpbuffer.c libpq/pqexpbuffer.h
win32.o : libpq/win32.c libpq/win32.h
snprintf.o : libpq/snprintf.c
ip.o : libpq/ip.c libpq/ip.h
fe-secure.o : libpq/fe-secure.c
fe-misc.o : libpq/fe-misc.c
noblock.o : libpq/noblock.c
fe-protocol2.o : libpq/fe-protocol2.c
fe-protocol3.o : libpq/fe-protocol3.c
fe-auth.o : libpq/fe-auth.c libpq/fe-auth.h
fe-exec.o : libpq/fe-exec.c libpq/libpq-fe.h
strlcpy.o : libpq/strlcpy.c
dirmod.o : libpq/dirmod.c
pthread-win32.o : libpq/pthread-win32.c libpq/pthread-win32.h
encnames.o : libpq/encnames.c
pgstrcasecmp.o : libpq/pgstrcasecmp.c
wchar.o : libpq/wchar.c
md5.o : libpq/md5.c libpq/md5.h
win32error.o : libpq/win32error.c
pgsleep.o : libpq/pgsleep.c
getaddrinfo.o : libpq/getaddrinfo.c
inet_aton.o : libpq/inet_aton.c
thread.o : libpq/thread.c

# Template command for compiling .cpp to .o
%.o : PostGISGDO/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# PostGIS sources
PostGIS.o : PostGISGDO/PostGIS.cpp PostGISGDO/PostGISClsFactory.hpp \
    PostGISGDO/PostGISGDO.hpp Common/Registrar.hpp Common/LogFiles.hpp \
    PostGISGDO/PostGIS.rh Common/VarUtils.hpp
PostGIS_res.o : PostGISGDO/PostGIS.rc PostGISGDO/PostGIS.rh \
    PostGISGDO/PostGIS.rgs Common/csres.bin
	windres $(RCFLAGS) $< -o $(BUILDDIR)$@
PostGISClsFactory.o : PostGISGDO/PostGISClsFactory.cpp Common/LogFiles.hpp \
    PostGISGDO/PostGISClsFactory.hpp PostGISGDO/PostGISGDO.hpp
PostGISGDO.o : PostGISGDO/PostGISGDO.cpp PostGISGDO/PostGISGDO.hpp \
    Common/CComTypes.hpp Common/GDOUtils.hpp Common/PGSTypes.hpp \
    PostGISGDO/PGSGeom.hpp PostGISGDO/PGSRecordset.hpp \
    PostGISGDO/PGSTableDef.hpp Common/PGtoGDOmaps.hpp
PGSRecordset.o : PostGISGDO/PGSRecordset.cpp PostGISGDO/PGSRecordset.hpp \
    Common/CComTypes.hpp Common/GDOUtils.hpp Common/PGSTypes.hpp \
    Common/PGtoGDOmaps.hpp Common/LogFiles.hpp PostGISGDO/PGSCols.hpp \
    PostGISGDO/PGSGeom.hpp PostGISGDO/PGSTableDef.hpp \
    PostGISGDO/PostGISGDO.hpp Common/GDOtoEWKB.hpp \
    Common/PGSCoordSystems.hpp PostGISGDO/PostGIS.rh
PGSTableDef.o : PostGISGDO/PGSTableDef.cpp PostGISGDO/PGSTableDef.hpp \
    Common/CComTypes.hpp PostGISGDO/PGSCols.hpp Common/PGSTypes.hpp \
    Common/LogFiles.hpp Common/PGtoGDOmaps.hpp PostGISGDO/PGSIndex.hpp \
    Common/VarUtils.hpp Common/GDOUtils.hpp
PGSIndex.o : PostGISGDO/PGSIndex.cpp PostGISGDO/PGSIndex.hpp \
    Common/CComTypes.hpp PostGISGDO/PGSCols.hpp Common/PGSTypes.hpp \
    Common/PGtoGDOmaps.hpp Common/LogFiles.hpp PostGISGDO/PostGIS.rh \
    PostGISGDO/PGSGeom.hpp Common/GDOUtils.hpp Common/VarUtils.hpp
PGSGeom.o : PostGISGDO/PGSGeom.cpp PostGISGDO/PGSGeom.hpp \
    Common/VarUtils.hpp Common/LogFiles.hpp
PGSCols.o : PostGISGDO/PGSCols.cpp PostGISGDO/PGSCols.hpp \
    PostGISGDO/PostGIS.rh Common/LogFiles.hpp Common/CComTypes.hpp \
    Common/PGSTypes.hpp PostGISGDO/PGSGeom.hpp Common/GDOUtils.hpp
PostGISExp.o : PostGISGDO/PostGIS.cpp PostGISGDO/PostGIS.def
	dlltool -d PostGISGDO/PostGIS.def -D PostGISGDO.dll -k -e $(BUILDDIR)PostGISExp.o

GMPsgCtlobj = GMPsgCtl.o GMPsgCtlExp.o GMPsgCtl_res.o GMPsgCtlClsFactory.o \
    LogFiles.o CComTypes.o GMPsgCtlIntfc.o
GMPsgCtlmod = $(addprefix $(BUILDDIR), $(GMPsgCtlobj)) \
	-lole32 -loleaut32 -luuid -lgdi32

GMPsgCtl : $(GMPsgCtlobj)
	g++ -pie -s -L./build -o $(BUILDDIR)GMPsgCtl.ocx $(GMPsgCtlmod) \
	-static-libgcc -static-libstdc++ -mdll

# Template command for compiling .cpp to .o
%.o : GMPsgCtl/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# GMPsgCtl sources
GMPsgCtl.o : GMPsgCtl/GMPsgCtl.cpp GMPsgCtl/GMPsgCtlClsFactory.hpp \
    GMPsgCtl/GMPsgCtlIntfc.hpp Common/LogFiles.hpp
GMPsgCtl_res.o : GMPsgCtl/GMPsgCtl.rc GMPsgCtl/GMPsgCtl.rh GMPsgCtl/GMPsgCtl.rgs \
    GMPsgCtl/GMPsgCtl.tlb
	windres $(RCFLAGS) $< -o $(BUILDDIR)$@
GMPsgCtlClsFactory.o : GMPsgCtl/GMPsgCtlClsFactory.cpp \
    GMPsgCtl/GMPsgCtlClsFactory.hpp GMPsgCtl/GMPsgCtlIntfc.hpp \
    Common/LogFiles.hpp Common/CComTypes.hpp
GMPsgCtlIntfc.o : GMPsgCtl/GMPsgCtlIntfc.cpp GMPsgCtl/GMPsgCtlIntfc.hpp \
    Common/LogFiles.hpp Common/CComTypes.hpp
GMPsgCtlExp.o : GMPsgCtl/GMPsgCtl.cpp GMPsgCtl/GMPsgCtl.def
	dlltool -d GMPsgCtl/GMPsgCtl.def -D GMPsgCtl.ocx -k -e $(BUILDDIR)GMPsgCtlExp.o

# GFLAGS=-DUNICODE -D_UNICODE -DWINVER=0x0501 -D_WIN32_IE=0x0400
# CFLAGS=-c -o2 $(GFLAGS)

PsgDBUtilsobj = PsgDBUtils.o PsgDBUMainDlg.o PsgDBUtils_res.o \
    PsgDBUConnDlg.o PsgDBUFtrClsDlg.o PsgDBUFldEdtDlg.o PsgDBUUsrDlg.o \
    VarUtils.o PsgDBUSpecFldEdt.o
PsgDBUtilsmod = $(addprefix $(BUILDDIR), $(PsgDBUtilsobj)) \
	-lgdi32 -lcomctl32 -lrpcrt4 -lpq -lshfolder -lwsock32 -lws2_32 -lsecur32 \
	-loleaut32 -lPsgDBUtils1

PsgDBUtils : $(PsgDBUtilsobj) libPsgDBUtils.a
	g++ -s -L./build -o $(BUILDDIR)PsgDBUtils.exe $(PsgDBUtilsmod) \
	-static-libgcc -static-libstdc++ -mwindows

# Template command for compiling .cpp to .o
%.o : PsgDBUtils/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# PsgDBUtils sources
PsgDBUtils.o : PsgDBUtils/PsgDBUtils.cpp PsgDBUtils/PsgDBUMainDlg.hpp
PsgDBUtils_res.o : PsgDBUtils/PsgDBUtils.rc PsgDBUtils/PsgDBUtils.rh \
    PsgDBUtils/PsgDBUtils.exe.manifest.32 Common/csres.bin \
    PsgDBUtils/DBUtils.ico
	windres $(RCFLAGS) $< -o $(BUILDDIR)$@
PsgDBUMainDlg.o : PsgDBUtils/PsgDBUMainDlg.cpp PsgDBUtils/PsgDBUMainDlg.hpp \
    PsgDBUtils/PsgDBUConnDlg.hpp PsgDBUtils/PsgDBUUsrDlg.hpp \
    PsgDBUtils/PsgDBUFtrClsDlg.hpp Common/VarUtils.hpp \
    PsgDBUtils/PsgDBUSpecFldEdt.hpp
PsgDBUConnDlg.o : PsgDBUtils/PsgDBUConnDlg.cpp PsgDBUtils/PsgDBUConnDlg.hpp \
    PsgDBUtils/PsgDBUtils.rh
PsgDBUFtrClsDlg.o : PsgDBUtils/PsgDBUFtrClsDlg.cpp \
    PsgDBUtils/PsgDBUFtrClsDlg.hpp PsgDBUtils/PsgDBUFldEdtDlg.hpp \
    PsgDBUtils/PsgDBUtils.rh Common/PsgDBUInterfaces.hpp
PsgDBUFldEdtDlg.o : PsgDBUtils/PsgDBUFldEdtDlg.cpp \
    PsgDBUtils/PsgDBUFldEdtDlg.hpp PsgDBUtils/PsgDBUtils.rh \
    Common/PsgDBUInterfaces.hpp
PsgDBUUsrDlg.o : PsgDBUtils/PsgDBUUsrDlg.cpp Common/VarUtils.hpp \
    PsgDBUtils/PsgDBUUsrDlg.hpp PsgDBUtils/PsgDBUtils.rh \
    Common/PsgDBUInterfaces.hpp
PsgDBUSpecFldEdt.o : PsgDBUtils/PsgDBUSpecFldEdt.cpp Common/VarUtils.hpp \
    PsgDBUtils/PsgDBUSpecFldEdt.hpp PsgDBUtils/PsgDBUtils.rh \
    Common/PsgDBUInterfaces.hpp
libPsgDBUtils.a : PsgDBUtilsDLL/PsgDBUtilsDLL.cpp PsgDBUtilsDLL/PsgDBUtilsDLL.def
	dlltool -d PsgDBUtilsDLL/PsgDBUtilsDLL.def -D PsgDBUtils.dll -k -A \
	-l $(BUILDDIR)libPsgDBUtils1.a $(BUILDDIR)PsgDBUtilsDLL.o

MakeCSResobj = MakeCSRes.o PCSS_TLB.o VarUtils.o
MakeCSResmod = $(addprefix $(BUILDDIR), $(MakeCSResobj)) \
	-lole32 -loleaut32 -luuid

MakeCSRes : $(MakeCSResobj)
	g++ -s -L./build -o $(BUILDDIR)MakeCSRes.exe $(MakeCSResmod) \
	-static-libgcc -static-libstdc++

# Template command for compiling .cpp to .o
%.o : MakeCSRes/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# MakeCSRes sources
MakeCSRes.o : MakeCSRes/MakeCSRes.cpp Common/PGtoGDOmaps.hpp \
    Common/PCSS_TLB.hpp


PsgDBUtilsDLLobj = PsgDBUtilsDLL.o PsgDBUtilsDLL_res.o PsgDBUDLLMain.o \
    PsgDBUDLLExp.o LogFiles.o GDOUtils.o VarUtils.o PGSCoordSystems.o \
    PGSTypes.o EWKBtoGDO.o GDOtoEWKB.o PsgDBUTables.o PGtoGDOmaps.o
PsgDBUtilsDLLmod = $(addprefix $(BUILDDIR), $(PsgDBUtilsDLLobj)) \
	-lrpcrt4 -lpq -lshfolder -lwsock32 -lws2_32 -lsecur32 -loleaut32 \

PsgDBUtilsDLL : $(PsgDBUtilsDLLobj)
	g++ -pie -s -L./build -o $(BUILDDIR)PsgDBUtils.dll $(PsgDBUtilsDLLmod) \
	-static-libgcc -static-libstdc++ -mthreads -mdll

# Template command for compiling .cpp to .o
%.o : PsgDBUtilsDLL/%.cpp
	gcc $(CFLAGS) $< -o $(BUILDDIR)$@

# PsgDBUtilsDLL sources
PsgDBUtilsDLL.o : PsgDBUtilsDLL/PsgDBUtilsDLL.cpp PsgDBUtilsDLL/PsgDBUDLLMain.hpp
PsgDBUtilsDLL_res.o : PsgDBUtilsDLL/PsgDBUtilsDLL.rc \
    PsgDBUtilsDLL/PsgDBUtilsDLL.rh Common/csres.bin
	windres $(RCFLAGS) $< -o $(BUILDDIR)$@
PsgDBUDLLMain.o : PsgDBUtilsDLL/PsgDBUDLLMain.cpp PsgDBUtilsDLL/PsgDBUDLLMain.hpp \
    Common/GDOUtils.hpp Common/VarUtils.hpp PsgDBUtilsDLL/PsgDBUTables.hpp
PsgDBUDLLExp.o : PsgDBUtilsDLL/PsgDBUtilsDLL.cpp PsgDBUtilsDLL/PsgDBUtilsDLL.def
	dlltool -d PsgDBUtilsDLL/PsgDBUtilsDLL.def -D PsgDBUtils.dll -k -e $(BUILDDIR)PsgDBUDLLExp.o
PsgDBUTables.o : PsgDBUtilsDLL/PsgDBUTables.cpp PsgDBUtilsDLL/PsgDBUTables.hpp \
    Common/PGtoGDOmaps.hpp Common/PGSCoordSystems.hpp Common/GDOUtils.hpp \
    Common/VarUtils.hpp Common/LogFiles.hpp Common/PsgDBUInterfaces.hpp \
    Common/GDOtoEWKB.hpp

#clean
clean:
	del build\*.o
