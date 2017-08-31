if "%1" == "a" mingw32-make -fmakefile.mingw64 clean
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 libpq 2> libpq.log
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 MakeCSRes 2> MakeCSRes.log
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 PostGIS 2> PostGIS.log
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 PsgDBUtilsDLL 2> PsgDBUtilsDLL.log
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 PsgDBUtils 2> PsgDBUtils.log
MIDL PsgDBUtilsCOM\PsgDBUtilsCOM.IDL /I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include" /o PsgDBUtilsCOMTlb.log /out PsgDBUtilsCOM
mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 PsgDBUtilsCOM 2> PsgDBUtilsCOM.log
if "%1" == "c" MIDL GMPsgCtl\GMPsgCtl.IDL /I "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include" /o GMPsgCtlTlb.log /out GMPsgCtl
if "%1" == "c" mingw32-make -fmakefile.mingw64 PREFIX=i686-w64-mingw32- TOOLPFX=i686-w64-mingw32- BUILDDIR=Build SUFFIX=32 GMPsgCtl 2> GMPsgCtl.log
