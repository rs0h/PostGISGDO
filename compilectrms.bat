if "%1" == "a" nmake /fmakefilectr.ms clean
MIDL GMPsgCtl\GMPsgCtl.IDL /I "C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include" /o GMPsgCtlTlb.log /out GMPsgCtl
nmake /fmakefilectr.ms MakeGMPsgCtl > GMPsgCtl.log
