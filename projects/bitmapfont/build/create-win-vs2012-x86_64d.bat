set d=%CD%
set bd=win-vs2012-x86_64d
set id=%d%\..\..\..\install\%bd%\bin\data\

if not exist "%d%\%bd%" (
   mkdir %d%\%bd%
   mkdir %d%\%bd%\Debug
   mkdir %d%\%bd%\Debug\data
   mkdir %d%\%bd%\Release
   mkdir %d%\%bd%\Release\data
   copy %id%\freepixel.fnt %d%\%bd%\Debug\Data\
   copy %id%\freepixel_0.png %d%\%bd%\Debug\Data\
   copy %id%\freepixel.fnt %d%\%bd%\Release\Data\
   copy %id%\freepixel_0.png %d%\%bd%\Release\Data\
)

cd %d%\%bd%
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 11 Win64" ..\
cd %d%

