param(
    [string] $Ffmpeg = 'C:\Program Files\ffmpeg\bin\ffmpeg.exe',
    [int] $FrameRate = 24,
    [switch] $Force
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function ConvertTo-SafeFileName {
    param(
        [Parameter(Mandatory)]
        [string] $Name
    )

    $safeName = $Name

    foreach ($character in [System.IO.Path]::GetInvalidFileNameChars()) {
        $safeName = $safeName.Replace([string] $character, '_')
    }

    return ($safeName -replace '\s+', ' ').Trim()
}

if ($FrameRate -le 0) {
    throw 'Frame rate must be positive.'
}

if (-not (Test-Path -LiteralPath $Ffmpeg -PathType Leaf)) {
    throw "FFmpeg was not found at '$Ffmpeg'."
}

$rootPath = Split-Path -Parent $PSScriptRoot
$resultsPath = Join-Path $rootPath 'results'
$moviesPath = Join-Path $rootPath 'movies'

if (-not (Test-Path -LiteralPath $resultsPath -PathType Container)) {
    throw "Results directory was not found at '$resultsPath'."
}

New-Item -ItemType Directory -Path $moviesPath -Force | Out-Null

$frames = @(
    Get-ChildItem -LiteralPath $resultsPath -Recurse -File -Filter '*.png' |
        ForEach-Object {
            if ($_.Name -match '^(?<prefix>.+?)(?<frame>\d{4})\.png$') {
                [pscustomobject]@{
                    Directory = $_.Directory.FullName
                    Prefix    = $Matches.prefix
                    Frame     = [int] $Matches.frame
                }
            }
        }
)

if ($frames.Count -eq 0) {
    Write-Host "No PNG frame sequences were found below '$resultsPath'."
    exit 0
}

$sequences = @(
    $frames |
        Group-Object -Property Directory, Prefix |
        ForEach-Object {
            $first = $_.Group[0]

            $relativeDirectory = $first.Directory.Substring(
                $resultsPath.Length
            ).TrimStart([char[]]'\/')

            $pathParts = $relativeDirectory -split '[\\/]'

            if ($pathParts.Count -eq 0 -or
                [string]::IsNullOrWhiteSpace($pathParts[0])) {
                Write-Warning "Ignoring sequence directly inside '$resultsPath'."
                return
            }

            $imageName = $first.Prefix.TrimEnd([char[]]' _-')

            if ([string]::IsNullOrWhiteSpace($imageName)) {
                $imageName = 'image'
            }

            [pscustomobject]@{
                Sample    = $pathParts[0]
                ImageName = $imageName
                Directory = $first.Directory
                Prefix    = $first.Prefix
                Frames    = @($_.Group.Frame | Sort-Object -Unique)
            }
        } |
        Sort-Object -Property Sample, ImageName, Directory
)

# Determine which output names require an additional numeric identifier.
$nameCounts = @{}

foreach ($sequence in $sequences) {
    $baseName = ConvertTo-SafeFileName(
        "$($sequence.Sample) - $($sequence.ImageName)"
    )

    $sequence | Add-Member -NotePropertyName BaseName -NotePropertyValue $baseName

    if (-not $nameCounts.ContainsKey($baseName)) {
        $nameCounts[$baseName] = 0
    }

    ++$nameCounts[$baseName]
}

$nameIndices = @{}
$createdCount = 0
$skippedCount = 0

foreach ($sequence in $sequences) {
    $framesInSequence = $sequence.Frames
    $startNumber = $framesInSequence[0]

    # FFmpeg's numbered-image input stops at the first missing frame.
    $expectedFrame = $startNumber

    foreach ($frame in $framesInSequence) {
        if ($frame -ne $expectedFrame) {
            throw "Sequence '$($sequence.Directory)\$($sequence.Prefix)%04d.png' " +
                  "is missing frame $expectedFrame."
        }

        ++$expectedFrame
    }

    $outputName = $sequence.BaseName

    if ($nameCounts[$sequence.BaseName] -gt 1) {
        if (-not $nameIndices.ContainsKey($sequence.BaseName)) {
            $nameIndices[$sequence.BaseName] = 0
        }

        ++$nameIndices[$sequence.BaseName]

        $outputName = '{0} - {1:00}' -f `
            $sequence.BaseName, `
            $nameIndices[$sequence.BaseName]
    }

    $inputPattern = Join-Path `
        $sequence.Directory `
        "$($sequence.Prefix)%04d.png"

    $outputPath = Join-Path $moviesPath "$outputName.mp4"

    if ((Test-Path -LiteralPath $outputPath) -and -not $Force) {
        Write-Host "Skipping existing movie: $outputName.mp4"
        ++$skippedCount
        continue
    }

    Write-Host
    Write-Host "Creating: $outputName.mp4"
    Write-Host "Frames:   $($framesInSequence.Count), starting at $startNumber"
    Write-Host "Source:   $inputPattern"

    $ffmpegArguments = @(
        '-hide_banner'
        '-loglevel', 'warning'
        $(if ($Force) { '-y' } else { '-n' })
        '-framerate', $FrameRate.ToString(
            [System.Globalization.CultureInfo]::InvariantCulture
        )
        '-start_number', $startNumber.ToString(
            [System.Globalization.CultureInfo]::InvariantCulture
        )
        '-i', $inputPattern
        '-vf', 'scale=trunc(iw/2)*2:trunc(ih/2)*2'
        '-c:v', 'libx264'
        '-preset', 'slow'
        '-crf', '18'
        '-profile:v', 'high'
        '-level:v', '4.1'
        '-pix_fmt', 'yuv420p'
        '-movflags', '+faststart'
        '-tag:v', 'avc1'
        '-an'
        $outputPath
    )

    & $Ffmpeg @ffmpegArguments

    if ($LASTEXITCODE -ne 0) {
        throw "FFmpeg failed for '$inputPattern' with exit code $LASTEXITCODE."
    }

    ++$createdCount
}

Write-Host
Write-Host "Created $createdCount movie(s); skipped $skippedCount existing movie(s)."
Write-Host "Output directory: $moviesPath"