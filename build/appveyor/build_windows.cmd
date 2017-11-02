echo on

echo Setting up environment...
set PATH=C:\Qt\5.9.1\mingw53_32\bin\;%PATH%
set PATH=C:\Qt\Tools\QtCreator\bin\;%PATH%
set PATH=C:\Qt\QtIFW-3.0.1\bin\;%PATH%
set PATH=C:\Qt\Tools\mingw530_32\bin\;%PATH%
set PATH=C:\MinGW\bin\;%PATH%

echo Setting program version...
perl.exe -p -i.bak -e "s/0\.0\.0/%APPVEYOR_BUILD_VERSION%/g" src\version.h dist\windows\config\config.xml dist\windows\packages\com.lexcelon.batlabtoolkitgui\meta\package.xml

echo Setting program date...
perl.exe -p -i.bak -e "s/2000-01-01/%APPVEYOR_REPO_COMMIT_TIMESTAMP%/g" dist\windows\packages\com.lexcelon.batlabtoolkitgui\meta\package.xml

echo Building program...
mkdir temp
cd temp
qmake ..\src
mingw32-make

echo Copying program and libraries to package directories...
echo f | xcopy /f /y release\Batlab.exe ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui\data\Batlab.exe
windeployqt.exe ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui\data\Batlab.exe --dir ..\dist\windows\packages\com.lexcelon.batlabtoolkitgui.lib\data

echo Building installer...
binarycreator.exe -p ..\dist\windows\packages\ -c ..\dist\windows\config\config.xml -n Batlab-Toolkit-GUI-Installer-%APPVEYOR_BUILD_VERSION%.exe

echo Creating update repository...
mkdir windows
repogen.exe -p ..\dist\windows\packages\ windows\

