set d=%CD%
set bd=win-vs2012-x86_64

call create-%bd%.bat

cd %d%\%bd%
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 11 Win64" ..\
cmake --build . --target install --config Release
cd %d%

