:: BuildPspImage rtmfile PspDirectoryCfg OutputBiosFile KeyMode

@echo off
::Input parameter check
IF /I "%4" == "" GOTO HELP
::System variable Check

if /I "%4" == "TK" (
  IF "%OPENSSL_PATH%" == "" (
    SET OPENSSL_PATH=C:\OpenSSL-Win32
  )
  ::CHECK_PATH
  IF NOT EXIST %OPENSSL_PATH%\bin\openssl.exe (
    @echo !!!OPENSSL_PATH system variable is NOT set correctly!!
    goto ERROR
  )
)

IF "%PSPKIT_PATH%" == "" (
  SET PSPKIT_PATH=%cd%
)
::CHECK_PATH
IF NOT EXIST %PSPKIT_PATH%\BuildPspImage.bat (
  @echo !!!PSPKIT_PATH system variable is NOT set correctly!!
  goto ERROR
)

IF "%PSP_FW_PATH%" == "" (
  SET PSP_FW_PATH=%cd%
)


if /I "%4" == "PK" (
  IF "%SPCLIENT_PATH%" == "" (
    set SPCLIENT_PATH=C:\SPClient
  )
  IF NOT EXIST %SPCLIENT_PATH%\SPClient.jar (
    @echo !!!SPCLIENT_PATH enviroment variable haven't been set correctly
    goto ERROR
  )
)


:START
::BIOS images
@set RTM_FILE=%1
@set PspDirectoryCfg=%2
@set FINAL_BIOS=%3

::Related PATH
@SET PSP_DIR_TOOL_PATH=%PSPKIT_PATH%\PspDirectoryTool
@SET TEMP_PATH=%PSPKIT_PATH%\Temp

::Create Temporary folder, and copy all files to it
rd    %TEMP_PATH% /S /Q
mkdir %TEMP_PATH%
copy  %PSP_DIR_TOOL_PATH%\*.*  %TEMP_PATH% /Y
copy  %PSP_FW_PATH%\*.*        %TEMP_PATH% /Y
copy  %PSPKIT_PATH%\*.*       %TEMP_PATH% /Y


if /I "%4" == "TK" (
  xcopy %OPENSSL_PATH%\*.*       %TEMP_PATH% /S /Y
)

copy  %RTM_FILE%               %TEMP_PATH% /Y
::TOOLS
@set OPENSSL=bin\openssl.exe
@set BUILDPSPDIRECTORY=BuildPspDirectory.exe
::BINARYS
@set RTM_PRIVATE_KEY=TestRtmPrivateKey.pem
@set PSP_DIR_FILE=PspDir.bin
@set RTM_PSPDIR_COMBINE_FILE=BiosRtmPspCombine.bin
@set RTM_FILE_SIGNATURE=RTMSignature.bin

pushd %TEMP_PATH%
@echo.
@echo [Generate PSP Directory]
@echo %BUILDPSPDIRECTORY% bd %PspDirectoryCfg% %PSP_DIR_FILE%
%BUILDPSPDIRECTORY% bd %PspDirectoryCfg% %PSP_DIR_FILE%
if %ERRORLEVEL% NEQ 0 goto ERROR

::Build without embed RTMSignature
if /I "%4" == "NOSIGN" goto BLDBIOS

@echo.
@echo [Combine RTM - PSP Directory and sign this using publc key]
copy /b %RTM_FILE%+%PSP_DIR_FILE% %RTM_PSPDIR_COMBINE_FILE%
::Sign using Test Key
if /I "%4" == "TK" (
  @echo Make RTM Signature using test Key
  %OPENSSL% dgst -binary -sign %RTM_PRIVATE_KEY% -sha256 -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-2 -out %RTM_FILE_SIGNATURE% %RTM_PSPDIR_COMBINE_FILE%
  reversestr.exe %RTM_FILE_SIGNATURE%
  goto BLDBIOS
)
::Sign using production Key
if /I "%4" == "PK" (
  @echo Make RTM Signature using production Key
  java -version
  @echo SPCLIENT_PATH:%SPCLIENT_PATH%
  pushd %SPCLIENT_PATH%
  del %RTM_FILE_SIGNATURE% /F /Q
  @echo   call SPClient.bat -in %cd%\%RTM_PSPDIR_COMBINE_FILE% -out %RTM_FILE_SIGNATURE%
  call SPClient.bat -in %cd%\%RTM_PSPDIR_COMBINE_FILE% -out %RTM_FILE_SIGNATURE%
  :: Copy back to Working path, and delete file in SPClient path
  copy /Y /B %RTM_FILE_SIGNATURE% %TEMP_PATH%
  del %RTM_FILE_SIGNATURE% /F /Q
  popd
  if %ERRORLEVEL% NEQ 0 goto ERROR
  goto BLDBIOS
)

@echo Invalid KeyMode
goto HELP

:BLDBIOS
@echo.
@echo [Finally build the Final BiosImage]
@echo %BUILDPSPDIRECTORY% bb %PspDirectoryCfg% %FINAL_BIOS%
%BUILDPSPDIRECTORY% bb %PspDirectoryCfg% %FINAL_BIOS%
if %ERRORLEVEL% NEQ 0 goto ERROR
copy %FINAL_BIOS% ..\
popd
::rd %TEMP_PATH% /S /Q
@echo.
@echo PSP contents have been embed to %FINAL_BIOS%

GOTO END

:ERROR
@echo **** Build PSP Image Fail ****
@exit 2
GOTO END

:HELP
@echo Embed PSP contents to Specific BIOS image
@echo.
@echo BuildPspImage.bat rtmfile PspDirectoryCfg OutputBiosFile
@echo   Positional parameters:
@echo     rtmfile           The Root trunk module of BIOS, commonly it is the SEC,PEI combined FV image
@echo     PspDirectoryCfg   Config file which describe PSP directory layout, and input bios informations
@echo     OutputBiosFile    The output bios image after embed the PSP contents
@echo     KeyMode           "TK|PK" Specify which key will be used, test key or production key
goto END
:END   