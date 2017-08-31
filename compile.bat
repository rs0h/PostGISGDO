if "%1" == "a" mingw32-make clean
mingw32-make libpq 2> libpq.log
mingw32-make MakeCSRes 2> MakeCSRes.log
mingw32-make PostGIS 2> PostGIS.log
mingw32-make PsgDBUtilsDLL 2> PsgDBUtilsDLL.log
mingw32-make PsgDBUtils 2> PsgDBUtils.log
if "%1" == "c" MIDL GMPsgCtl\GMPsgCtl.IDL /I "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" /o GMPsgCtlTlb.log /out GMPsgCtl
if "%1" == "c" mingw32-make GMPsgCtl 2> GMPsgCtl.log
