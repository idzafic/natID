param (
    [string]$Target,
    [string]$LinkName
)

if (-not (Test-Path $Target)) {
    Write-Host "Error: Target does not exist."
    exit 1
}

New-Item -ItemType SymbolicLink -Path $LinkName -Target $Target -Force

if ($?) {
    Write-Host "Symbolic link created:"
    Write-Host "$LinkName -> $Target"
} else {
    Write-Host "Failed to create symbolic link."
}