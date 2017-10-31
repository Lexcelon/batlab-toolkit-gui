echo on

SET project_dir=%cd%

echo Setting up environment...
set PATH=%QT%\bin\;%PATH%
set PATH=C:\Qt\Tools\QtCreator\bin\;%PATH%
set PATH=C:\Qt\QtIFW-3.0.1\bin\;%PATH%
set PATH=C:\Qt\Tools\mingw530_32\bin\;%PATH%
set PATH=C:\MinGW\bin\;%PATH%

echo Setting program version...


echo Building program...
mkdir temp
cd temp
qmake ..\src
mingw32-make %configuration%

echo Building installer...
binarycreator.exe -p ..\dist\windows\packages\ -c ..\dist\windows\config\config.xml -n Batlab-Toolkit-GUI-Installer.exe

