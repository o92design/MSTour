# Jira API Diagnostics
# Tests authentication and permissions

$ErrorActionPreference = "Continue"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Jira API Diagnostics" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Load .env
Write-Host "Step 1: Loading .env file..." -ForegroundColor Yellow
$envFile = Join-Path $PSScriptRoot "..\.env"
if (-not (Test-Path $envFile)) {
    Write-Host "✗ .env file not found at $envFile" -ForegroundColor Red
    exit 1
}

$env = @{}
Get-Content $envFile | ForEach-Object {
    if ($_ -and -not $_.StartsWith('#')) {
        $k, $v = $_ -split '=', 2
        $env[$k.Trim()] = $v.Trim()
    }
}

Write-Host "✓ .env loaded" -ForegroundColor Green
Write-Host "  Email: $($env['ATLASSIAN_EMAIL'])" -ForegroundColor Gray
Write-Host "  Base URL: $($env['ATLASSIAN_BASE_URL'])" -ForegroundColor Gray
Write-Host "  Token: $($env['ATLASSIAN_API_TOKEN'].Substring(0,10))..." -ForegroundColor Gray
Write-Host ""

# Create auth header
Write-Host "Step 2: Creating authentication..." -ForegroundColor Yellow
$creds = "$($env['ATLASSIAN_EMAIL']):$($env['ATLASSIAN_API_TOKEN'])"
$base64 = [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($creds))
Write-Host "✓ Auth header created" -ForegroundColor Green
Write-Host ""

$headers = @{
    "Authorization" = "Basic $base64"
    "Accept" = "application/json"
    "Content-Type" = "application/json"
}

# Test 1: Get current user (myself)
Write-Host "Step 3: Testing authentication..." -ForegroundColor Yellow
try {
    $myself = Invoke-RestMethod -Uri "$($env['ATLASSIAN_BASE_URL'])/rest/api/3/myself" -Headers $headers -Method Get
    Write-Host "✓ Authentication successful!" -ForegroundColor Green
    Write-Host "  Display Name: $($myself.displayName)" -ForegroundColor Gray
    Write-Host "  Email: $($myself.emailAddress)" -ForegroundColor Gray
    Write-Host "  Account ID: $($myself.accountId)" -ForegroundColor Gray
} catch {
    Write-Host "✗ Authentication failed!" -ForegroundColor Red
    Write-Host "  Error: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}
Write-Host ""

# Test 4: List all issues in project
Write-Host "Step 4: Listing recent issues..." -ForegroundColor Yellow
try {
    $search = Invoke-RestMethod -Uri "$($env['ATLASSIAN_BASE_URL'])/rest/api/3/search?jql=project=MST ORDER BY created DESC&maxResults=5" -Headers $headers -Method Get
    Write-Host "✓ Can search issues!" -ForegroundColor Green
    Write-Host "  Total issues in MST: $($search.total)" -ForegroundColor Gray
    Write-Host "  Recent issues:" -ForegroundColor Gray
    foreach ($issue in $search.issues) {
        Write-Host "    - $($issue.key): $($issue.fields.summary)" -ForegroundColor Gray
    }
} catch {
    Write-Host "✗ Cannot search issues!" -ForegroundColor Red
    Write-Host "  Error: $($_.Exception.Message)" -ForegroundColor Red
}
Write-Host ""

# Test 5: Check permissions
Write-Host "Step 5: Checking permissions..." -ForegroundColor Yellow
try {
    $perms = Invoke-RestMethod -Uri "$($env['ATLASSIAN_BASE_URL'])/rest/api/3/mypermissions?projectKey=MST" -Headers $headers -Method Get
    Write-Host "✓ Retrieved permissions!" -ForegroundColor Green
    
    $createPermission = $perms.permissions.CREATE_ISSUES
    if ($createPermission -and $createPermission.havePermission) {
        Write-Host "  ✓ CREATE_ISSUES: Allowed" -ForegroundColor Green
    } else {
        Write-Host "  ✗ CREATE_ISSUES: Denied" -ForegroundColor Red
        Write-Host "    This is why ticket creation failed!" -ForegroundColor Yellow
    }
    
    $editPermission = $perms.permissions.EDIT_ISSUES
    if ($editPermission -and $editPermission.havePermission) {
        Write-Host "  ✓ EDIT_ISSUES: Allowed" -ForegroundColor Green
    } else {
        Write-Host "  ✗ EDIT_ISSUES: Denied" -ForegroundColor Red
    }
} catch {
    Write-Host "✗ Cannot check permissions!" -ForegroundColor Red
    Write-Host "  Error: $($_.Exception.Message)" -ForegroundColor Red
}
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Diagnostics Complete" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan