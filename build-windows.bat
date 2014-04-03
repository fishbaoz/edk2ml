@echo off
rem ----------------project settings-------------------
set PROJECT_NAME=DuetPkg
rem # NOOPT   : good for source level, but slower execution
rem # DEBUG   : fast execution, but register variables makes source level debug harder
rem # RELEASE : fast execution, no source level debug and no serial out
set BUILDTARGET=DEBUG

rem # 0x80000000  Errors only 
rem # 0x80000004  Errors and some module load messages
rem # 0x8000004F  Lots of messages
rem # 0x800000CF  Lots of messages, with dependency logging
set DEBUG_PRINT_LEVEL=0x8000004F

rem # compiler selection
set TOOL_CHAIN_TAG=VS2012
set TARGETARCH=X64

rem -------------------memory map----------------------
rem
rem   000000-001000                     real mode vectors, used by BiosVideoThunkDxe
rem   007000-007BFF                     stack (early real mode execution)
rem   007C00-007DFF                     bootsect.asm
rem   010000-0101FF                     temp disk read buffer used by Start64.asm
rem   020000-021FFF IMAGE_BUFFER_LOW    first FAT cluster of EFILDR16 (16 sectors per cluster)
rem   021800-021FFE IMAGE_BUFFER_LOW    stack (st16_64.asm)
rem   020000-02FFFF IMAGE_BUFFER_LOW    EFILDR16 (first 64KB)
rem   050000-05FFFF X64_STACK_TOP       initial x64 stack
rem   070000-07FFFF FAKE_NVRAM_BASE     UEFI NVRAM emulation memory
rem   080000-085FFF X64_PAGE_TABLE_BASE temp page tables for 64-bit enable
rem   100000-xxxxxx IMAGE_BUFFER_HIGH   EFILDR16 (complete)

rem --------these two values are hard-coded in the coreboot payload code--------
set /a IMAGE_BUFFER_HIGH   = 0x100000
set /a START64CODESIZE     = 0x2000
rem ----------------------------------------------------------------------------
set /a IMAGE_BUFFER_LOW    = 0x20000
set /a X64_STACK_TOP       = 0x60000
set /a FAKE_NVRAM_BASE     = 0x70000
set /a FAKE_NVRAM_SIZE     = 0x10000
set /a X64_PAGE_TABLE_BASE = 0x80000
set /a X64_PAGE_TABLE_SIZE = 0x6000
set /a EFI64CODESIZE       = 0x3000
set /a EFI64CODESTART      = %IMAGE_BUFFER_HIGH% + %START64CODESIZE%

rem # optional: display logo during UEFI startup
set LOGO_FILE=DuetPkg/Coreboot_full_web.bmp

rem --------------end project settings-----------------

rem Make sure current dir is that of this batch file.
cd  /d %~dp0

rem # Locate the UEFI workspace directory.
set WORKSPACE=
set WORKSPACE_SEARCH0=%cd%
set WORKSPACE_SEARCH1=%cd%\edk2
if exist %WORKSPACE_SEARCH1%\MdePkg\ set WORKSPACE=%WORKSPACE_SEARCH1%
if exist %WORKSPACE_SEARCH0%\MdePkg\ set WORKSPACE=%WORKSPACE_SEARCH0%
if "%WORKSPACE%" == "" (
    echo --- unable to locate uefi workspace directory
    pause
    exit /b 1
    )

set BASE_TOOLS_PATH=%WORKSPACE%\BaseTools
set BUILD_DIR=%WORKSPACE%\build
set IMAGE_DIR=%BUILD_DIR%\images
set BASETOOLS_DIR=%BASE_TOOLS_PATH%\Bin\Win32
set OUTPUT_DIR=%BUILD_DIR%\%PROJECT_NAME%\%BUILDTARGET%_%TOOL_CHAIN_TAG%
set OUTPUT_DIR_PROC=%OUTPUT_DIR%\%TARGETARCH%
set BOOTSECTOR_BIN_DIR=%OUTPUT_DIR_PROC%\DuetPkg\BootSector\BootSector\Output
set PROJECT_DIR=%WORKSPACE%\%PROJECT_NAME%
set PLATFORMFILE=%PROJECT_DIR%\DuetPkg%TARGETARCH%.dsc

rem ---------------------------------------------------
rem # arguments for the build command
set /a THREADS=NUMBER_OF_PROCESSORS*2
set _BUILD_OPTIONS_=^
-p %PLATFORMFILE% ^
-b %BUILDTARGET% ^
-t %TOOL_CHAIN_TAG% ^
-a %TARGETARCH% ^
-D DEBUG_PRINT_LEVEL=%DEBUG_PRINT_LEVEL% ^
-D BUILD_NEW_SHELL ^
-D LOGO_FILE=%LOGO_FILE% ^
-D X64_STACK_TOP=%X64_STACK_TOP% ^
-D FAKE_NVRAM_BASE=%FAKE_NVRAM_BASE% ^
-D FAKE_NVRAM_SIZE=%FAKE_NVRAM_SIZE% ^
-n %THREADS%

rem # create the B command (build with output sent to a text file)
echo @call rombuild.bat ^> buildlog-windows.txt 2^>^&1 > b.bat
echo @if errorlevel 1 echo ===== BUILD FAILED =====>> b.bat

if "%TOOL_CHAIN_TAG%" == "VS2003" goto LOCATE_VS2003
if "%TOOL_CHAIN_TAG%" == "VS2005" goto LOCATE_VS2005
if "%TOOL_CHAIN_TAG%" == "VS2008" goto LOCATE_VS2008
if "%TOOL_CHAIN_TAG%" == "VS2010" goto LOCATE_VS2010
if "%TOOL_CHAIN_TAG%" == "VS2012" goto LOCATE_VS2012
goto VS_NOT_FOUND

:LOCATE_VS2003
if "%VS71COMNTOOLS%" == "" goto VS_NOT_FOUND
for %%? in ("%VS71COMNTOOLS%..\..") do set VS2003_PATH=%%~f?
set VS_PATH=%VS2003_PATH%
goto VS_FOUND

:LOCATE_VS2005
if "%VS80COMNTOOLS%" == "" goto VS_NOT_FOUND
for %%? in ("%VS80COMNTOOLS%..\..") do set VS2005_PATH=%%~f?
set VS_PATH=%VS2005_PATH%
goto VS_FOUND

:LOCATE_VS2008
if "%VS90COMNTOOLS%" == "" goto VS_NOT_FOUND
for %%? in ("%VS90COMNTOOLS%..\..") do set VS2008_PATH=%%~f?
set VS_PATH=%VS2008_PATH%
goto VS_FOUND

:LOCATE_VS2010
if "%VS100COMNTOOLS%" == "" goto VS_NOT_FOUND
for %%? in ("%VS100COMNTOOLS%..\..") do set VS2010_PATH=%%~f?
set VS_PATH=%VS2010_PATH%
goto VS_FOUND

:LOCATE_VS2012
if "%VS110COMNTOOLS%" == "" goto VS_NOT_FOUND
for %%? in ("%VS110COMNTOOLS%..\..") do set VS2012_PATH=%%~f?
set VS_PATH=%VS2012_PATH%
goto VS_FOUND

:VS_NOT_FOUND
if "%VS_PATH%" == "" (
   echo.
   echo =========================================
   echo ===  MS %TOOL_CHAIN_TAG% build tools not found  ===
   echo =========================================
   echo.
   pause
   exit
   )
echo.

:VS_FOUND
REM # do a test compile to sanity check the build tool setup
setlocal
cd /d %temp%
echo int main (void) {return 0;} > testapp.c
set path=%VS_PATH%\VC\bin;%VS_PATH%\Common7\IDE
cl.exe -c -nologo testapp.c >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
   echo.
   echo --------------------------------------------------------
   echo - UEFI compile tools found, but a test compile failed. -
   echo - Check your tool setup.                               -
   echo --------------------------------------------------------
   echo.
   pause
   exit
   )
del testapp.* > nul
endlocal

rem # create the CLEAN command
echo @setlocal                                                    > clean.bat
echo @rd  %BUILD_DIR%                           /s/q ^>nul 2^>nul >> clean.bat
echo @rd  %WORKSPACE%\Conf\.cache               /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\Conf\tools_def.txt        /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\Conf\build_rule.txt       /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\Conf\target.txt           /s/q ^>nul 2^>nul >> clean.bat
echo @del *.pyc                                 /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\BaseTools\*.o             /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\BaseTools\*.d             /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\BaseTools\*.a             /s/q ^>nul 2^>nul >> clean.bat
echo @del %WORKSPACE%\BaseTools\Source\C\bin\*. /s/q ^>nul 2^>nul >> clean.bat
echo @endlocal                                                    >> clean.bat

rem # create the ROMBUILD command
echo @echo off                                                                                                       > rombuild.bat
echo rem (generated file, edit build-windows.bat instead)                                                           >> rombuild.bat
echo setlocal                                                                                                       >> rombuild.bat
echo echo using compile tools from %UEFI_COMPILE_TOOLS%                                                             >> rombuild.bat
echo rem # delete the rom image so that none is present after a build fail                                          >> rombuild.bat
echo del %OUTPUT_DIR%\FV\*.Fv 2^>nul                                                                                >> rombuild.bat
echo cd %~dp0                                                                                                       >> rombuild.bat
echo call %BASE_TOOLS_PATH%\toolsetup.bat ^> nul                                                                    >> rombuild.bat
echo @echo off                                                                                                      >> rombuild.bat
echo build.exe %_BUILD_OPTIONS_%                                                                                    >> rombuild.bat
echo if errorlevel 1 GOTO buildfail                                                                                 >> rombuild.bat
echo IF not exist %OUTPUT_DIR%\FV\DUETEFIMAINFV.Fv GOTO buildfail                                                   >> rombuild.bat
echo cd %%OUTPUT_DIR%%\FV                                                                                           >> rombuild.bat
echo echo Compressing %%OUTPUT_DIR%%\FV\DUETEFIMAINFV.Fv...                                                         >> rombuild.bat
echo %%BASETOOLS_DIR%%\LzmaCompress -e -o FWVOLUME.z DUETEFIMAINFV.Fv                                               >> rombuild.bat
echo IF not %%errorlevel%% == 0 GOTO buildfail                                                                      >> rombuild.bat
echo echo Compressing %%OUTPUT_DIR_PROC%%\DxeCore.efi ...                                                           >> rombuild.bat
echo %%BASETOOLS_DIR%%\LzmaCompress -e -o DxeMain.z %%OUTPUT_DIR_PROC%%\DxeCore.efi                                 >> rombuild.bat
echo IF not %%errorlevel%% == 0 GOTO buildfail                                                                      >> rombuild.bat
echo echo Compressing %%OUTPUT_DIR_PROC%%\DxeIpl.efi ...                                                            >> rombuild.bat
echo %%BASETOOLS_DIR%%\LzmaCompress -e -o DxeIpl.z %%OUTPUT_DIR_PROC%%\DxeIpl.efi                                   >> rombuild.bat
echo IF not %%errorlevel%% == 0 GOTO buildfail                                                                      >> rombuild.bat
echo echo Generate Loader Image Efildr64...                                                                         >> rombuild.bat
echo %%BASETOOLS_DIR%%\GenFw.exe -r --rebase 0x10000 %%OUTPUT_DIR_PROC%%\EfiLoader.efi                              >> rombuild.bat
echo %%BASETOOLS_DIR%%\EfiLdrImage.exe -o Efildr64 %%OUTPUT_DIR_PROC%%\EfiLoader.efi DxeIpl.z DxeMain.z FWVOLUME.z  >> rombuild.bat
echo IF not %%errorlevel%% == 0 GOTO buildfail                                                                      >> rombuild.bat
echo @copy /b %BOOTSECTOR_BIN_DIR%\St16_64.com+%BOOTSECTOR_BIN_DIR%\Efi64.com2+Efildr64 EFILDR16                    >> rombuild.bat
echo md %%IMAGE_DIR%% 2^> nul                                                                                       >> rombuild.bat
echo copy EFILDR16 %%IMAGE_DIR%% /y                                                                                 >> rombuild.bat
echo echo Creating FAT disk image...                                                                                >> rombuild.bat
echo cd %BOOTSECTOR_BIN_DIR%                                                                                        >> rombuild.bat
echo BuildFatImage mbr=Mbr.com bs=Bs16.com ldr=%%IMAGE_DIR%%\EFILDR16 out=%%IMAGE_DIR%%\FAT16.img                   >> rombuild.bat
echo if errorlevel 1 GOTO buildfail                                                                                 >> rombuild.bat
echo cd %WORKSPACE%                                                                                                 >> rombuild.bat
echo ECHO ===== BUILD SUCCESS =====                                                                                 >> rombuild.bat
echo     exit /b 0                                                                                                  >> rombuild.bat
echo :buildfail                                                                                                     >> rombuild.bat
echo     ECHO ===== BUILD FAILED =====                                                                              >> rombuild.bat
echo endlocal                                                                                                       >> rombuild.bat
echo     exit /b 1                                                                                                  >> rombuild.bat

@echo off

if "%1" == "batch" rombuild.bat
if "%1" == "" (
   start "Duet build %cd%" %comspec% /k^
   "echo compile tools   %UEFI_COMPILE_TOOLS%&&echo BASE_TOOLS_PATH %BASE_TOOLS_PATH%&&echo WORKSPACE       %WORKSPACE%&&echo PLATFORMFILE    %PLATFORMFILE%&&echo VS_PATH         %VS_PATH%&&echo.&&echo === build   ROMBUILD&&echo === clean   CLEAN"
   ) else (
   echo === unrecognized option: %1
   pause
)
