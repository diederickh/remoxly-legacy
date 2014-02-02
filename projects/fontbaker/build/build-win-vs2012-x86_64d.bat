set d=%CD%
set bd=win-vs2012-x86_64d

call create-%bd%.bat

cd %d%\%bd%
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 11 Win64" ..\
cmake --build . --target install --config Release
cd %d%

