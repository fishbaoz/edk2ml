@echo off
if "%mingw32%" == "" (
    echo --- unable to locate mingw 32-bit compiler
    pause
    exit /b 1
    )

setlocal
cd  /d %~dp0
%mingw32%\bin\gcc -O2 -Wall -Wextra -o..\..\..\Bin\Win32\BuildFatImage.exe BuildFatImage.c
pause
endlocal
