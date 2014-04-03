@call rombuild.bat > buildlog-windows.txt 2>&1 
@if errorlevel 1 echo ===== BUILD FAILED =====
