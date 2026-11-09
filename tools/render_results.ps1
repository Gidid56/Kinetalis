param(
    [string] $Ovitos,
    [string] $State = (Join-Path $PSScriptRoot 'rendering.ovito'),
    [int] $Threads = [Environment]::ProcessorCount,
    [int] $Width = 1000,
    [int] $Height = 1000,
    [switch] $Overwrite
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Find-Ovitos {
    param(
        [string] $RequestedPath
    )

    if (-not [string]::IsNullOrWhiteSpace($RequestedPath)) {
        if (Test-Path -LiteralPath $RequestedPath -PathType Leaf) {
            return (Resolve-Path -LiteralPath $RequestedPath).Path
        }

        throw "OVITO interpreter was not found at '$RequestedPath'."
    }

    $command = Get-Command 'ovitos.exe' -ErrorAction SilentlyContinue

    if ($null -ne $command) {
        return $command.Path
    }

    $candidates = @()

    if (-not [string]::IsNullOrWhiteSpace($env:ProgramFiles)) {
        $candidates += @(
            (Join-Path $env:ProgramFiles 'OVITO Pro\ovitos.exe')
            (Join-Path $env:ProgramFiles 'OVITO\ovitos.exe')
        )
    }

    if (-not [string]::IsNullOrWhiteSpace(${env:ProgramFiles(x86)})) {
        $candidates += @(
            (Join-Path ${env:ProgramFiles(x86)} 'OVITO Pro\ovitos.exe')
            (Join-Path ${env:ProgramFiles(x86)} 'OVITO\ovitos.exe')
        )
    }

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            return $candidate
        }
    }

    throw @"
Could not find ovitos.exe.

Checked:
$($candidates -join "`n")

Supply its location explicitly using:
  -Ovitos 'C:\path\to\ovitos.exe'
"@
}

if ($Threads -le 0) {
    throw 'OVITO thread count must be positive.'
}

if ($Width -le 0 -or $Height -le 0) {
    throw 'Render dimensions must be positive.'
}

$Ovitos = Find-Ovitos -RequestedPath $Ovitos
$State = (Resolve-Path -LiteralPath $State).Path

$rootPath = Split-Path -Parent $PSScriptRoot
$resultsPath = Join-Path $rootPath 'results'
$renderScript = Join-Path $PSScriptRoot 'render_script_ovito.py'

if (-not (Test-Path -LiteralPath $resultsPath -PathType Container)) {
    throw "Results directory was not found at '$resultsPath'."
}

if (-not (Test-Path -LiteralPath $renderScript -PathType Leaf)) {
    throw "OVITO render script was not found at '$renderScript'."
}

Write-Host "Using OVITO interpreter: $Ovitos"
Write-Host "Using session state:      $State"
Write-Host "Overwrite existing:       $Overwrite"

$latticeDirectories = @(
    Get-ChildItem -LiteralPath $resultsPath -Recurse -Directory |
        Where-Object {
            $_.Name -ieq 'lattice' -and
            @(Get-ChildItem -LiteralPath $_.FullName -File -Filter '*.xyz').Count -gt 0
        } |
        Sort-Object -Property FullName
)

if ($latticeDirectories.Count -eq 0) {
    Write-Host "No lattice XYZ sequences were found below '$resultsPath'."
    exit 0
}

$processedCount = 0

foreach ($latticeDirectory in $latticeDirectories) {
    $sampleDirectory = $latticeDirectory.Parent
    $renderDirectory = Join-Path $sampleDirectory.FullName 'rendered'

    New-Item -ItemType Directory -Path $renderDirectory -Force | Out-Null

    $logPath = Join-Path $renderDirectory 'ovito-render.log'

    $ovitoArguments = @(
        '--nthreads', $Threads.ToString([System.Globalization.CultureInfo]::InvariantCulture)
        $renderScript
        '--state', $State
        '--input-directory', $latticeDirectory.FullName
        '--output-directory', $renderDirectory
        '--prefix', 'surface'
        '--width', $Width.ToString([System.Globalization.CultureInfo]::InvariantCulture)
        '--height', $Height.ToString([System.Globalization.CultureInfo]::InvariantCulture)
    )

    if ($Overwrite) {
        $ovitoArguments += '--overwrite'
    }

    Write-Host
    Write-Host "Rendering: $($sampleDirectory.Name)"
    Write-Host "Input:     $($latticeDirectory.FullName)"
    Write-Host "Output:    $renderDirectory\surface0000.png ..."
    Write-Host "Log:       $logPath"

    $previousErrorActionPreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'

    try {
        & $Ovitos @ovitoArguments 2>&1 |
            Tee-Object -FilePath $logPath

        $exitCode = $LASTEXITCODE
    }
    finally {
        $ErrorActionPreference = $previousErrorActionPreference
    }

    if ($exitCode -ne 0) {
        throw "OVITO failed for sample '$($sampleDirectory.Name)' with exit code $exitCode. Log: '$logPath'."
    }

    ++$processedCount
}

Write-Host
Write-Host "Processed $processedCount sample(s)."