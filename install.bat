
@ECHO OFF

@REM Install folder
SET relative_install_path=%windir%\..\SOARE\
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
IF "%askinst%"=="yes" GOTO main
IF "%askinst%"=="y" GOTO main
IF "%askinst%"=="no" GOTO cancel
IF "%askinst%"=="n" GOTO cancel
GOTO askinstall

:main

@REM If an installation is already present
IF EXIST %install_path% (
    ECHO Warning: Existing installation
:input
    SET askrem=""
    SET /P askrem="Delete the old installation? (yes/no): "
    IF "%askrem%"=="yes" GOTO delete_install
    IF "%askrem%"=="y" GOTO delete_install
    IF "%askrem%"=="no" GOTO cancel
    IF "%askrem%"=="n" GOTO cancel
    GOTO input
)

@REM Install...
:install

ECHO Creating file...
MKDIR %install_path%

IF EXIST ".\bin\soare.exe" (
    COPY .\bin\soare.exe %install_path%\soare.exe
    SETX SOARE "%install_path%"
    ECHO Success
) ELSE (
    ECHO .\bin\soare.exe not found...
    ECHO Try building an executable...
    make
    IF NOT errorlevel==0 (
        ECHO make not found or compile error...
        GOTO end
    )
    GOTO install
)

GOTO end

@REM Remove install
:delete_install
ECHO Remove installation..
RMDIR /S /Q %install_path%
GOTO main

@REM Cancel
:cancel
ECHO Operation Canceled

@REM Quit
:end
ECHO.
PAUSE