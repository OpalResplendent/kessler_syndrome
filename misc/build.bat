@echo off

set SDLIncludes= -IC:\DEV\SDL\SDL2_ttf-2.0.12\include -IC:\DEV\SDL\SDL2-2.0.3\include -IC:\DEV\SDL\SDL2_mixer-2.0.0\include
set SDLLinks= C:\DEV\SDL\SDL2-2.0.3\lib\x64\SDL2.lib C:\DEV\SDL\SDL2-2.0.3\lib\x64\SDL2main.lib C:\DEV\SDL\SDL2_ttf-2.0.12\lib\x64\SDL2_ttf.lib C:\DEV\SDL\SDL2_mixer-2.0.0\lib\x64\SDL2_mixer.lib  /SUBSYSTEM:WINDOWS

set OtherIncludes=-IC:\DEV\Tools\iaca-win64

set BuildStyle=-Od -MTd -DIACA_MARKS_OFF

set WarningSurpression= -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -wd4313 -wd4244
set CommonCompilerFlags=%BuildStyle% -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -EHsc -Zo -Oi -WX -W4 -FC -Z7 %WarningSurpression% %OtherIncludes%
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib dsound.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM del *.pdb > NUL 2> NUL
REM echo WAITING FOR PDB > lock.tmp

REM del lock.tmp

cl %CommonCompilerFlags% ..\code\%ProjectName%.cpp -Fm%ProjectName%.map /link %CommonLinkerFlags%

popd
