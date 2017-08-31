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
libpath="C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib"
libpath2="C:\Program Files\Microsoft Visual Studio 8\VC\lib"

inclpath="C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include"
inclpath2="C:\Program Files\Microsoft Visual Studio 8\VC\Include"

BUILDDIR=buildms^\

CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /D_WIN32_WINNT=0x501 \
/D_CRT_SECURE_NO_DEPRECATE /D_CRT_NON_CONFORMING_SWPRINTFS \
/I"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" \
/I"C:\Program Files\Microsoft Visual Studio 8\VC\include"

# CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /DDBGLEVEL=3 /D_WIN32_WINNT=0x501 \
# /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NON_CONFORMING_SWPRINTFS \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\include"

# CFLAGS=/c /O2 /EHsc /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_DEPRECATE \
# /D_CRT_NON_CONFORMING_SWPRINTFS \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" \
# /I"C:\Program Files\Microsoft Visual Studio 8\VC\include"

RCFLAGS=-D_WIN32_IE=0x0400

# ole32 defines CoInitialize, CoUninitialize
# oleaut32 defines CoCreateInstance
# uuid defines GUID_NULL

# Common sources
LogFiles.obj : Common\LogFiles.cpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)LogFiles.obj Common\LogFiles.cpp
CComTypes.obj : Common\CComTypes.cpp Common\CComTypes.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)CComTypes.obj Common\CComTypes.cpp

GMPsgCtlobj = GMPsgCtl.obj GMPsgCtl.res GMPsgCtlClsFactory.obj \
    LogFiles.obj CComTypes.obj GMPsgCtlIntfc.obj
GMPsgCtlmod = $(BUILDDIR)GMPsgCtl.obj $(BUILDDIR)GMPsgCtl.res \
    $(BUILDDIR)GMPsgCtlClsFactory.obj $(BUILDDIR)LogFiles.obj \
    $(BUILDDIR)CComTypes.obj $(BUILDDIR)GMPsgCtlIntfc.obj

MakeGMPsgCtl : $(GMPsgCtlobj)
	CL /LD GMPsgCtl\GMPsgCtl.def $(GMPsgCtlmod) /link /libpath:$(libpath) \
	/libpath:$(libpath2) /out:$(BUILDDIR)GMPsgCtl.ocx \
	/defaultlib:ole32.lib /defaultlib:oleaut32.lib /defaultlib:user32.lib \
	/defaultlib:gdi32.lib \
	/implib:$(BUILDDIR)GMPsgCtl.lib

# GMPsgCtl sources
GMPsgCtl.obj : GMPsgCtl\GMPsgCtl.cpp GMPsgCtl\GMPsgCtlClsFactory.hpp \
    GMPsgCtl\GMPsgCtlIntfc.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)GMPsgCtl.obj GMPsgCtl\GMPsgCtl.cpp
GMPsgCtl.res : GMPsgCtl\GMPsgCtl.rc GMPsgCtl\GMPsgCtl.rh GMPsgCtl\GMPsgCtl.rgs \
    GMPsgCtl\GMPsgCtl.tlb
	rc /i $(inclpath) /i $(inclpath2) /d_WIN32_IE=0x0400 /d_MSCOMP_ /fo \
	$(BUILDDIR)$@ GMPsgCtl\GMPsgCtl.rc
GMPsgCtlClsFactory.obj : GMPsgCtl\GMPsgCtlClsFactory.cpp \
    GMPsgCtl\GMPsgCtlClsFactory.hpp GMPsgCtl\GMPsgCtlIntfc.hpp Common\LogFiles.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)GMPsgCtlClsFactory.obj GMPsgCtl\GMPsgCtlClsFactory.cpp
GMPsgCtlIntfc.obj : GMPsgCtl\GMPsgCtlIntfc.cpp GMPsgCtl\GMPsgCtlIntfc.hpp \
    Common\LogFiles.hpp Common\CComTypes.hpp
    CL $(CFLAGS) /Fo$(BUILDDIR)GMPsgCtlIntfc.obj GMPsgCtl\GMPsgCtlIntfc.cpp

#clean
clean:
	del buildms\*.o
