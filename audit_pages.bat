@echo off
REM General-purpose Confluence DRY Audit Tool
REM Compares two Confluence pages and identifies redundant content
REM
REM Usage: audit_pages.bat "Page Title 1" "Page Title 2"
REM Example: audit_pages.bat "GDD Master" "Core Gameplay Loop"

setlocal enabledelayedexpansion

REM Check if both arguments provided
if "%~1"=="" (
    echo.
    echo ====================================================================
    echo Confluence DRY Audit Tool
    echo ====================================================================
    echo.
    echo Usage: audit_pages.bat "Page Title 1" "Page Title 2"
    echo.
    echo Examples:
    echo   audit_pages.bat "GDD Master" "Core Gameplay Loop"
    echo   audit_pages.bat "Ship Management" "Fleet Management"
    echo.
    echo This tool will:
    echo   1. Fetch both Confluence pages
    echo   2. Analyze for redundant content
    echo   3. Generate comparison report in audit_output\
    echo.
    echo Output files:
    echo   - page1_name.md
    echo   - page2_name.md
    echo   - audit_comparison_[timestamp].txt
    echo.
    echo ====================================================================
    exit /b 1
)

if "%~2"=="" (
    echo ERROR: Two page titles required
    echo Usage: audit_pages.bat "Page Title 1" "Page Title 2"
    exit /b 1
)

set PAGE1=%~1
set PAGE2=%~2

echo.
echo ====================================================================
echo Confluence DRY Audit Tool
echo ====================================================================
echo.
echo Page 1: %PAGE1%
echo Page 2: %PAGE2%
echo.

REM Create output directory
if not exist audit_output mkdir audit_output

REM Generate timestamp for unique output file
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
set TIMESTAMP=%mydate%_%mytime%
set TIMESTAMP=%TIMESTAMP: =0%

REM Set output files
set SAFE_PAGE1=%PAGE1: =_%
set SAFE_PAGE1=%SAFE_PAGE1::=%
set SAFE_PAGE2=%PAGE2: =_%
set SAFE_PAGE2=%SAFE_PAGE2::=%

set OUTPUT_PAGE1=audit_output\%SAFE_PAGE1%.md
set OUTPUT_PAGE2=audit_output\%SAFE_PAGE2%.md
set OUTPUT_REPORT=audit_output\audit_comparison_%TIMESTAMP%.txt

echo [Step 1/3] Fetching "%PAGE1%"...
py tools\confluence_fetch.py get "%PAGE1%" > "%OUTPUT_PAGE1%" 2>&1
if errorlevel 1 (
    echo ERROR: Failed to fetch "%PAGE1%"
    type "%OUTPUT_PAGE1%"
    exit /b 1
)
echo   Saved to: %OUTPUT_PAGE1%

echo.
echo [Step 2/3] Fetching "%PAGE2%"...
py tools\confluence_fetch.py get "%PAGE2%" > "%OUTPUT_PAGE2%" 2>&1
if errorlevel 1 (
    echo ERROR: Failed to fetch "%PAGE2%"
    type "%OUTPUT_PAGE2%"
    exit /b 1
)
echo   Saved to: %OUTPUT_PAGE2%

echo.
echo [Step 3/3] Running DRY audit...
py tools\confluence_audit.py "%PAGE1%" "%PAGE2%" > "%OUTPUT_REPORT%" 2>&1
if errorlevel 1 (
    echo WARNING: Audit completed with warnings
) else (
    echo   Audit complete!
)
echo   Report saved to: %OUTPUT_REPORT%

echo.
echo ====================================================================
echo Audit Complete!
echo ====================================================================
echo.
echo Output files:
echo   1. %OUTPUT_PAGE1%
echo   2. %OUTPUT_PAGE2%
echo   3. %OUTPUT_REPORT%
echo.
echo Review the report to see:
echo   - Content overlap percentage
echo   - Duplicate sections
echo   - Recommendations for consolidation
echo.

REM Try to open the report (if available)
if exist "%OUTPUT_REPORT%" (
    echo Opening report...
    start "" "%OUTPUT_REPORT%"
)

exit /b 0
