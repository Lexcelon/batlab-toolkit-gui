echo on

SET project_dir=%cd%

echo Setting up environment...
set PATH=%QT%\bin\;%PATH%
set PATH=C:\Qt\Tools\QtCreator\bin\;%PATH%
set PATH=C:\Qt\QtIFW-3.0.1\bin\;%PATH%
set PATH=C:\Qt\Tools\mingw530_32\bin\;%PATH%

echo Building...
qmake src
make

