@echo off                                                                                                       
rem (generated file, edit build-windows.bat instead)                                                           
setlocal                                                                                                       
echo using compile tools from                                                              
rem # delete the rom image so that none is present after a build fail                                          
del D:\uefi\edk2ml\build\DuetPkg\DEBUG_VS2012\FV\*.Fv 2>nul                                                                                
cd D:\uefi\edk2ml\                                                                                                       
call D:\uefi\edk2ml\BaseTools\toolsetup.bat > nul                                                                    
@echo off                                                                                                      
build.exe -p D:\uefi\edk2ml\DuetPkg\DuetPkgX64.dsc -b DEBUG -t VS2012 -a X64 -D DEBUG_PRINT_LEVEL=0x8000004F -D BUILD_NEW_SHELL -D LOGO_FILE=DuetPkg/Coreboot_full_web.bmp -D X64_STACK_TOP=393216 -D FAKE_NVRAM_BASE=458752 -D FAKE_NVRAM_SIZE=65536 -n 6                                                                                    
if errorlevel 1 GOTO buildfail                                                                                 
IF not exist D:\uefi\edk2ml\build\DuetPkg\DEBUG_VS2012\FV\DUETEFIMAINFV.Fv GOTO buildfail                                                   
cd %OUTPUT_DIR%\FV                                                                                           
echo Compressing %OUTPUT_DIR%\FV\DUETEFIMAINFV.Fv...                                                         
%BASETOOLS_DIR%\LzmaCompress -e -o FWVOLUME.z DUETEFIMAINFV.Fv                                               
IF not %errorlevel% == 0 GOTO buildfail                                                                      
echo Compressing %OUTPUT_DIR_PROC%\DxeCore.efi ...                                                           
%BASETOOLS_DIR%\LzmaCompress -e -o DxeMain.z %OUTPUT_DIR_PROC%\DxeCore.efi                                 
IF not %errorlevel% == 0 GOTO buildfail                                                                      
echo Compressing %OUTPUT_DIR_PROC%\DxeIpl.efi ...                                                            
%BASETOOLS_DIR%\LzmaCompress -e -o DxeIpl.z %OUTPUT_DIR_PROC%\DxeIpl.efi                                   
IF not %errorlevel% == 0 GOTO buildfail                                                                      
echo Generate Loader Image Efildr64...                                                                         
%BASETOOLS_DIR%\GenFw.exe -r --rebase 0x10000 %OUTPUT_DIR_PROC%\EfiLoader.efi                              
%BASETOOLS_DIR%\EfiLdrImage.exe -o Efildr64 %OUTPUT_DIR_PROC%\EfiLoader.efi DxeIpl.z DxeMain.z FWVOLUME.z  
IF not %errorlevel% == 0 GOTO buildfail                                                                      
@copy /b D:\uefi\edk2ml\build\DuetPkg\DEBUG_VS2012\X64\DuetPkg\BootSector\BootSector\Output\St16_64.com+D:\uefi\edk2ml\build\DuetPkg\DEBUG_VS2012\X64\DuetPkg\BootSector\BootSector\Output\Efi64.com2+Efildr64 EFILDR16                    
md %IMAGE_DIR% 2> nul                                                                                       
copy EFILDR16 %IMAGE_DIR% /y                                                                                 
echo Creating FAT disk image...                                                                                
cd D:\uefi\edk2ml\build\DuetPkg\DEBUG_VS2012\X64\DuetPkg\BootSector\BootSector\Output                                                                                        
BuildFatImage mbr=Mbr.com bs=Bs16.com ldr=%IMAGE_DIR%\EFILDR16 out=%IMAGE_DIR%\FAT16.img                   
if errorlevel 1 GOTO buildfail                                                                                 
cd D:\uefi\edk2ml                                                                                                 
ECHO ===== BUILD SUCCESS =====                                                                                 
    exit /b 0                                                                                                  
:buildfail                                                                                                     
    ECHO ===== BUILD FAILED =====                                                                              
endlocal                                                                                                       
    exit /b 1                                                                                                  
