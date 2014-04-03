@setlocal                                                    
@rd  D:\uefi\edk2ml\build                           /s/q >nul 2>nul 
@rd  D:\uefi\edk2ml\Conf\.cache               /s/q >nul 2>nul 
@del D:\uefi\edk2ml\Conf\tools_def.txt        /s/q >nul 2>nul 
@del D:\uefi\edk2ml\Conf\build_rule.txt       /s/q >nul 2>nul 
@del D:\uefi\edk2ml\Conf\target.txt           /s/q >nul 2>nul 
@del *.pyc                                 /s/q >nul 2>nul 
@del D:\uefi\edk2ml\BaseTools\*.o             /s/q >nul 2>nul 
@del D:\uefi\edk2ml\BaseTools\*.d             /s/q >nul 2>nul 
@del D:\uefi\edk2ml\BaseTools\*.a             /s/q >nul 2>nul 
@del D:\uefi\edk2ml\BaseTools\Source\C\bin\*. /s/q >nul 2>nul 
@endlocal                                                    
