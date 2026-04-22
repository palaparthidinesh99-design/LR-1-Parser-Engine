@echo off
echo Compiling the parser...
g++ -std=c++11 src\main.cpp src\grammar.cpp src\parser.cpp -o parser.exe
if %ERRORLEVEL% equ 0 (
    echo Compilation successful. Running the parser...
    echo ----------------------------------------------------
    parser.exe
) else (
    echo Compilation failed.
)
