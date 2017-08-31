if "%1" == "a" nmake /fmakefile.ms clean
nmake /fmakefile.ms MakeMakeCSRes > MakeCSRes.log
nmake /fmakefile.ms MakePostGIS > PostGIS.log
nmake /fmakefile.ms MakePsgDBUtilsDLL > PsgDBUtilsDLL.log
nmake /fmakefile.ms MakePsgDBUtils > PsgDBUtils.log
if "%1" == "c" MIDL GMPsgCtl\GMPsgCtl.IDL /I "C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include" /o GMPsgCtlTlb.log /out GMPsgCtl
if "%1" == "c" nmake /fmakefile.ms MakeGMPsgCtl > GMPsgCtl.log
