# Generalized Windows Shortcut Creator
# Creates .lnk shortcuts for batch files that can be pinned to taskbar
#
# IMPORTANT: Creates shortcuts targeting cmd.exe (not the .bat directly)
# This is required for Windows taskbar pinning to work properly.
#
# Usage:
#   .\create_shortcut.ps1 -BatFile "startup.bat" -ShortcutName "MSTour Startup" -Description "Launch dev environment"
#   .\create_shortcut.ps1 -BatFile ".\tools\build.bat" -ShortcutName "Build MSTour" -Icon "C:\Windows\System32\shell32.dll,21"

param(
    [Parameter(Mandatory=$true, HelpMessage="Path to the .bat file (relative or absolute)")]
    [string]$BatFile,
    
    [Parameter(Mandatory=$true, HelpMessage="Name for the shortcut (without .lnk extension)")]
    [string]$ShortcutName,
    
    [Parameter(Mandatory=$false, HelpMessage="Description for the shortcut")]
    [string]$Description = "",
    
    [Parameter(Mandatory=$false, HelpMessage="Icon location (format: path,index)")]
    [string]$Icon = "C:\Windows\System32\shell32.dll,283"
)

# Resolve paths
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptDir

if (-not [System.IO.Path]::IsPathRooted($BatFile)) {
    $batFilePath = Join-Path $projectRoot $BatFile
} else {
    $batFilePath = $BatFile
}

# Verify bat file exists
if (-not (Test-Path $batFilePath)) {
    Write-Host "Error: Batch file not found: $batFilePath" -ForegroundColor Red
    exit 1
}

$shortcutPath = Join-Path $projectRoot "$ShortcutName.lnk"
$workingDir = Split-Path -Parent $batFilePath

# Create WScript Shell COM object
$WScriptShell = New-Object -ComObject WScript.Shell

# Create the shortcut
# IMPORTANT: Target cmd.exe instead of the .bat file directly for taskbar pinning to work
$shortcut = $WScriptShell.CreateShortcut($shortcutPath)
$shortcut.TargetPath = "C:\Windows\System32\cmd.exe"
$shortcut.Arguments = "/c `"$batFilePath`""
$shortcut.WorkingDirectory = $workingDir
$shortcut.Description = $Description
$shortcut.IconLocation = $Icon
$shortcut.Save()

Write-Host "✓ Shortcut created successfully!" -ForegroundColor Green
Write-Host "  Location: $shortcutPath" -ForegroundColor Cyan
Write-Host "  Target: cmd.exe (runs: $batFilePath)" -ForegroundColor Cyan
Write-Host ""
Write-Host "To pin to taskbar:" -ForegroundColor Yellow
Write-Host "  1. Right-click '$ShortcutName.lnk'" 
Write-Host "  2. Select 'Pin to taskbar'"
