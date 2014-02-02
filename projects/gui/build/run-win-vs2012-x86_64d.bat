set d=%CD%
set bd=win-vs2012-x86_64d

call build-%bd%.bat    

set id=%d%\..\..\..\install\%bd%\bin\
cd %id%

gui_debug.exe
cd %d%

