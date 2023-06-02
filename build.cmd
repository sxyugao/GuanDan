@echo off
echo Cleaning up files...
rd /s /q GuanDan
echo Copying files...
mkdir GuanDan
cd /D GuanDan
mkdir Client
mkdir Server
copy ..\Release\Client\release\Client.exe Client\Client.exe
copy ..\Release\Server\release\Server.exe Server\Server.exe
echo Building dependencies...
windeployqt6 Client
windeployqt6 Server