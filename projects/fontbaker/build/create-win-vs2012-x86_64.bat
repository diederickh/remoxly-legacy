set d=%CD%
set bd=win-vs2012-x86_64

if not exist "%d%\%bd%" (
      mkdir %d%\%bd%
)

cd %d%\%bd%
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 11 Win64" ..\
cd %d%

