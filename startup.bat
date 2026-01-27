@echo off
REM MSTour Development Environment Startup
REM Opens PowerShell 7 for Copilot CLI and VSCode for editing

REM Get the directory where this batch file is located
set "PROJECT_DIR=%~dp0"
REM Remove trailing backslash
set "PROJECT_DIR=%PROJECT_DIR:~0,-1%"

echo Starting MSTour development environment...
echo Project directory: %PROJECT_DIR%

REM Start PowerShell 7 window at MSTour directory
start pwsh -NoExit -WorkingDirectory "%PROJECT_DIR%"

REM Launch VSCode with MSTour directory
code "%PROJECT_DIR%"

echo Done! PowerShell 7 and VSCode are starting...
