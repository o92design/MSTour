# Windows Shortcut Creator Tool

A PowerShell utility for creating Windows shortcuts (.lnk files) for batch files that can be pinned to the taskbar.

**Important**: This tool creates shortcuts that target `cmd.exe` (not the .bat file directly) to ensure Windows taskbar pinning works properly.

## Location
`tools/create_shortcut.ps1`

## Usage

### Basic Usage
```powershell
.\tools\create_shortcut.ps1 -BatFile "startup.bat" -ShortcutName "MSTour Startup"
```

### With Optional Parameters
```powershell
.\tools\create_shortcut.ps1 `
    -BatFile "audit_pages.bat" `
    -ShortcutName "Audit Pages" `
    -Description "Run page audit tool" `
    -Icon "C:\Windows\System32\shell32.dll,21"
```

## Parameters

| Parameter | Required | Description | Default |
|-----------|----------|-------------|---------|
| `-BatFile` | Yes | Path to the .bat file (relative to project root) | - |
| `-ShortcutName` | Yes | Name for the shortcut (without .lnk extension) | - |
| `-Description` | No | Description text for the shortcut | "" |
| `-Icon` | No | Icon location in format "path,index" | "C:\Windows\System32\shell32.dll,283" |

## How It Works

The tool creates shortcuts that target `cmd.exe` with your batch file as an argument:
- **Target**: `C:\Windows\System32\cmd.exe`
- **Arguments**: `/c "path\to\your\script.bat"`

This approach is required because Windows won't allow pinning shortcuts that point directly to .bat files. By targeting cmd.exe, Windows allows the pin, and your batch file still runs correctly.

## Icon Examples

Common useful icons from `C:\Windows\System32\shell32.dll`:
- `,21` - Gear/settings
- `,283` - Developer/code
- `,16` - Application window
- `,46` - Folder with tools

## Pinning to Taskbar

After creating a shortcut:
1. Navigate to the project directory
2. Right-click the `.lnk` file
3. Select "Pin to taskbar" ✓ This will work!

## Example: MSTour Startup

The project includes a pre-created shortcut:
- **File**: `MSTour Startup.lnk`
- **Runs**: `startup.bat` (opens PowerShell 7 + VSCode)
- **Purpose**: Quick launch of development environment
- **To use**: Right-click and select "Pin to taskbar"

## Troubleshooting

**Q: "Pin to taskbar" option is missing?**  
A: Make sure the shortcut targets `cmd.exe` (not the .bat directly). Re-run the script to recreate the shortcut with the correct target.

**Q: Can I pin the .bat file directly?**  
A: No, Windows doesn't allow pinning .bat files. You must use a .lnk shortcut as created by this tool.
