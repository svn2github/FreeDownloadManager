@echo off
@cls
del %1.h > nul
del %1.xpt > nul
copy /Y %1.idl e:\vc98\gecko-sdk\idl\ > nul
cd e:\vc98\gecko-sdk\idl
del %1.h > nul
del %1.xpt > nul
..\bin\xpidl -m header %1.idl
..\bin\xpidl -m typelib %1.idl
cd "e:\vcw\fdm\fdm\Firefox\XPCOM Component"
copy /Y e:\vc98\gecko-sdk\idl\%1.h .\ > nul
copy /Y e:\vc98\gecko-sdk\idl\%1.xpt .\ > nul