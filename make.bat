@echo off
@echo === Cmd Liberaid compile ===
ping -n 2 localhost >nul
color 0a

:: Check for Visual Studio
if exist "%VS120COMNTOOLS%" (
set VSPATH="%VS120COMNTOOLS%"
goto set_env
)
if exist "%VS110COMNTOOLS%" (
set VSPATH="%VS110COMNTOOLS%"
goto set_env
)
if exist "%VS100COMNTOOLS%" (
set VSPATH="%VS100COMNTOOLS%"
goto set_env
)
if exist "%VS90COMNTOOLS%" (
set VSPATH="%VS90COMNTOOLS%"
goto set_env
)
if exist "%VS80COMNTOOLS%" (
set VSPATH="%VS80COMNTOOLS%"
goto set_env
)

echo You need Microsoft Visual Studio 8, 9, 10, 11 or 12 installed
pause
exit

:: Setup the environment
:set_env
if not exist %VSPATH%vsvars32.bat (
color 0a
cls
echo.
echo === An error occured! ===
echo.
pause >nul
exit
) else call %VSPATH%vsvars32.bat

:: Compile
:compile
echo.
@echo === Building ===
cl /EHsc src/*.cpp
@echo === Deleting *.obj files ===
del *.obj
@echo === Finished ===
echo.

:: Ending/Or not :D
@echo Press any Key to Compile again...
@pause >nul
color 0c
@echo === Again :D ===
@ping -n 2 localhost >nul
color 0a
cls
goto compile