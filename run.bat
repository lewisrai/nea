@echo off
IF %1.==. goto exit
IF %1==d goto debug
IF %1==r goto release

:debug
start bin\debug
goto exit

:release
start bin\release

:exit
cls
