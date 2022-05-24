@echo off
echo Setting up environment for Qt usage...
set PATH=C:\Qt\6.2.4\mingw_64\bin;C:\Qt\Tools\mingw1120_64\bin;%PATH%
echo Cleaning up files...
rd /s /q GuanDan
echo Copying files...
mkdir GuanDan
cd /D GuanDan
mkdir Client
mkdir Server
copy ..\Debug\Client\debug\Client.exe Client\Client.exe
copy ..\Debug\Server\debug\Server.exe Server\Server.exe
echo Building dependencies...
windeployqt Client
windeployqt Server