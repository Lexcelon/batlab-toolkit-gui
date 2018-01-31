echo on

echo Setting up environment...
set PATH=C:\Qt\5.9\mingw53_32\bin\;%PATH%
set PATH=C:\Qt\Tools\QtCreator\bin\;%PATH%
set PATH=C:\Qt\QtIFW-3.0.1\bin\;%PATH%
set PATH=C:\Qt\Tools\mingw530_32\bin\;%PATH%
set PATH=C:\MinGW\bin\;%PATH%


IF "%APPVEYOR_REPO_TAG%"=="true" (
    echo Setting program version...   
    perl.exe -p -i.bak -e "s/0\.0\.0/%APPVEYOR_REPO_TAG_NAME%/g" src\batlabcore\version.h dist\windows\config\config.xml dist\windows\packages\com.lexcelon.batlabtoolkitgui\meta\package.xml
) ELSE (
    echo Skipping program version setting since not a tagged build...
)

echo Setting program date...
perl.exe -p -i.bak -e "s/2000-01-01/%APPVEYOR_REPO_COMMIT_TIMESTAMP%/g" dist\windows\packages\com.lexcelon.batlabtoolkitgui\meta\package.xml

echo Building program...
mkdir temp
cd temp
qmake ..\src
mingw32-make

rem Based on this:
rem https://www.appveyor.com/docs/build-configuration/#script-blocks-in-build-configuration
rem I would not think the below is necessary. But in any case
rem sometimes the mingw32-make command would fail and the build would
rem then succeed. On tagged builds on master this would silently upload
rem blank updates to users. The below fixes it so that the build actually
rem terminates.
IF NOT %errorlevel% EQU 0 (
   exit 1
)

echo Copying program and libraries to package directories...
echo f | xcopy /f /y release\BatlabToolkitGUI.exe ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui\data\BatlabToolkitGUI.exe
windeployqt.exe ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui\data\BatlabToolkitGUI.exe --dir ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui.lib\data

echo Building installer...
binarycreator.exe -p ..\dist\windows\packages\ -c ..\dist\windows\config\config.xml -n Batlab-Toolkit-GUI-Installer-Windows.exe

echo Creating update repository...
mkdir windows
repogen.exe -p ..\dist\windows\packages\ windows\

