@echo off
IF %1.==. goto exit
IF %1==d goto debug
IF %1==r goto release
goto exit

:debug
rmdir "bin\debug" /s /q
mkdir "bin\debug\audio"
mkdir "bin\debug\images"
xcopy "src\audio" "bin\debug\audio" /s /e /i
xcopy "src\images" "bin\debug\images" /s /e /i
xcopy "src\other" "bin\debug" /s /e /i
echo Building Debug ...
g++ -c src\*.cpp -std=c++11 -Wall -m64 -I include
g++ *.o -o bin\debug\Run! -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
start bin\debug
goto exit

:release
rmdir "bin\release" /s /q
mkdir "bin\release\audio"
mkdir "bin\release\images"
xcopy "src\audio" "bin\release\audio" /s /e /i
xcopy "src\images" "bin\release\images" /s /e /i
xcopy "src\other" "bin\release" /s /e /i
echo Building Release ...
g++ -c src\*.cpp -std=c++11 -O3 -Wall -m64 -I include
g++ *.o -o bin\release\Run! -s -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
start bin\release

:exit
del *.o
echo ... Build Attempt Completed
pause
cls
