# Move top-level .c files into src/ and .h files into include/
$root = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $root

# Ensure target directories exist
$dirs = @("src","include")
foreach ($d in $dirs) {
    if (-not (Test-Path $d)) { New-Item -ItemType Directory -Path $d | Out-Null }
}

# Move .c files from root to src (exclude files already in subfolders)
Get-ChildItem -Path $root -Filter *.c -File | ForEach-Object {
    if ($_.DirectoryName -ieq $root) {
        Move-Item -Path $_.FullName -Destination (Join-Path $root "src") -Force
    }
}

# Move .h files from root to include
Get-ChildItem -Path $root -Filter *.h -File | ForEach-Object {
    if ($_.DirectoryName -ieq $root) {
        Move-Item -Path $_.FullName -Destination (Join-Path $root "include") -Force
    }
}

Write-Host "Moved .c files to src/ and .h files to include/"