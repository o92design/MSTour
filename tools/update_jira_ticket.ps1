# Update Jira Ticket with Custom Fields
# Usage: .\update_jira_ticket.ps1 -IssueKey MST-2 -Purpose "..." -Description "..." -DefinitionOfDone "..." -TestPlan "..." -Changes "..."

param(
    [Parameter(Mandatory=$true)]
    [string]$IssueKey,
    
    [string]$Purpose = "",
    [string]$Description = "",
    [string]$DefinitionOfDone = "",
    [string]$TestPlan = "",
    [string]$Changes = ""
)

# Load .env
function Load-Env {
    $envFile = Join-Path $PSScriptRoot "..\..\.env"
    $env = @{}
    Get-Content $envFile | ForEach-Object {
        if ($_ -and -not $_.StartsWith('#')) {
            $k, $v = $_ -split '=', 2
            $env[$k] = $v.Trim()
        }
    }
    return $env
}

# Create ADF paragraph
function New-ADFParagraph {
    param([string]$text)
    return @{
        type = "paragraph"
        content = @(
            @{
                type = "text"
                text = $text
            }
        )
    }
}

# Create ADF document from text (handles line breaks)
function New-ADFDoc {
    param([string]$text)
    $lines = $text -split "`n"
    return @{
        type = "doc"
        version = 1
        content = @($lines | ForEach-Object { New-ADFParagraph -text $_ })
    }
}

Write-Host "🔄 Updating Jira ticket: $IssueKey" -ForegroundColor Cyan

$env = Load-Env
$creds = "$($env['JIRA_EMAIL']):$($env['JIRA_API_TOKEN'])"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($creds)
$base64 = [System.Convert]::ToBase64String($bytes)

$headers = @{
    "Authorization" = "Basic $base64"
    "Content-Type" = "application/json"
}

# Build fields object
$fields = @{}

if ($Description) {
    Write-Host "  ✓ Description" -ForegroundColor White
    $fields["description"] = New-ADFDoc -text $Description
}

if ($Purpose) {
    Write-Host "  ✓ Purpose" -ForegroundColor White
    $fields["customfield_10042"] = New-ADFDoc -text $Purpose
}

if ($DefinitionOfDone) {
    Write-Host "  ✓ Definition of Done" -ForegroundColor White
    $fields["customfield_10041"] = New-ADFDoc -text $DefinitionOfDone
}

if ($TestPlan) {
    Write-Host "  ✓ Test Plan" -ForegroundColor White
    $fields["customfield_10040"] = New-ADFDoc -text $TestPlan
}

if ($Changes) {
    Write-Host "  ✓ Changes" -ForegroundColor White
    $fields["customfield_10039"] = New-ADFDoc -text $Changes
}

$body = @{
    fields = $fields
} | ConvertTo-Json -Depth 20

# Update issue
$url = "$($env['JIRA_BASE_URL'])/rest/api/3/issue/$IssueKey"
try {
    Invoke-RestMethod -Uri $url -Headers $headers -Method Put -Body $body
    Write-Host "`n✅ $IssueKey updated successfully!" -ForegroundColor Green
} catch {
    Write-Host "`n❌ Failed: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Response: $($_.ErrorDetails.Message)" -ForegroundColor Yellow
    exit 1
}
