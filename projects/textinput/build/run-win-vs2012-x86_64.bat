set d=%CD%
set bd=win-vs2012-x86_64

call build-%bd%.bat    

set id=%d%\..\..\..\install\%bd%\bin\
cd %id%

textinput.exe
cd %d%

