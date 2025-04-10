@echo off
echo Compiling Langton's Ant...
g++ -o langton_ant.exe langton_ant.cpp -lgdi32
if %errorlevel% neq 0 (
    echo Compilation failed! Please make sure you have MinGW-w64 installed and added to your PATH.
    echo See README.md for installation instructions.
    pause
    exit /b %errorlevel%
)
echo Compilation successful! Running the program...
.\langton_ant.exe 