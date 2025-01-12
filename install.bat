
@ECHO OFF

CD /D %~dp0

@REM Install folder
SET relative_install_path=%windir%\..\SOARE
FOR %%i IN ("%relative_install_path%") DO SET "install_path=%%~fi"

@REM SOARE
ECHO.
ECHO "  _____  _____  ___  ______ _____  "
ECHO " /  ___||  _  |/ _ \ | ___ \  ___| "
ECHO " \ `--. | | | / /_\ \| |_/ / |__   "
ECHO "  `--. \| | | |  _  ||    /|  __|  "
ECHO " /\__/ /\ \_/ / | | || |\ \| |___  "
ECHO " \____/  \___/\_| |_/\_| \_\____/  "
ECHO.
ECHO SOARE Installer
ECHO Antoine LANDRIEUX (WTFPL) install.bat
ECHO https://github.com/AntoineLandrieux/SOARE/
ECHO.

@REM Show LICENSE file
IF EXIST "LICENSE" (
    TYPE LICENSE
    ECHO.
)

@REM Ask for install
:askinstall
SET askinst=""
SET /P askinst="Install SOARE? (yes/no): "
IF "%askinst%" EQU "yes" GOTO :main
IF "%askinst%" EQU "y" GOTO :main
IF "%askinst%" EQU "no" GOTO :cancel
IF "%askinst%" EQU "n" GOTO :cancel
GOTO :askinstall

:main
@REM If an installation is already present
IF EXIST %install_path% (
    ECHO Warning: Existing installation
:input
    SET askrem=""
    SET /P askrem="Delete the old installation? (yes/no): "
    IF "%askrem%" EQU "yes" GOTO :delete_install
    IF "%askrem%" EQU "y" GOTO :delete_install
    IF "%askrem%" EQU "no" GOTO :cancel
    IF "%askrem%" EQU "n" GOTO :cancel
    GOTO :input
)

@REM Enable colored output
:register
ECHO Updating REGISTER...
REG add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f
IF NOT errorlevel 0 ECHO Error when update REG (try to run as an Administrator)

@REM Install...
:install
ECHO Creating file...
MKDIR %install_path%

FOR %%i IN ("%CD%\lib" "%CD%\bin\soare.exe" "%CD%\include" "%CD%\doc" "%CD%\resources" "%CD%\LICENSE" "%CD%\script" "%CD%\README.md") DO (
    IF NOT EXIST "%%i" (
        ECHO "%%i" is missing...
        GOTO :end
    )
)

COPY "%CD%\bin\soare.exe" "%install_path%\soare.exe"
COPY "%CD%\LICENSE" "%install_path%\LICENSE"
COPY "%CD%\README.md" "%install_path%\README.md"

ECHO R | XCOPY "%CD%\include\" "%install_path%\include" /S /E
ECHO R | XCOPY "%CD%\lib\" "%install_path%\lib" /S /E
ECHO R | XCOPY "%CD%\doc\" "%install_path%\doc" /S /E
ECHO R | XCOPY "%CD%\script\" "%install_path%\script\" /S /E
ECHO R | XCOPY "%CD%\resources\" "%install_path%\resources" /S /E

SETX SOARE "%install_path%"
ECHO Success
GOTO :end

@REM Remove install
:delete_install
ECHO Remove installation..
RMDIR /S /Q %install_path%
GOTO :main

@REM Cancel
:cancel
ECHO Operation Canceled

@REM Quit
:end
ECHO.
PAUSE