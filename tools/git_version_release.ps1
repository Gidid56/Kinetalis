# Create a versioned Kinetalis release from the VERSION field in CMakeLists.txt.
# Usage: .\tools\release.ps1 [-PublishRelease]
# Run from a clean main branch after committing the intended release contents.
param(
    [switch]$PublishRelease
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Invoke-Git {
    & git @args
    if ($LASTEXITCODE -ne 0) {
        throw "Git command failed: git $($args -join ' ')"
    }
}

$root = (& git rev-parse --show-toplevel)
if ($LASTEXITCODE -ne 0 -or -not $root) {
    throw 'Run this script from within the Kinetalis Git repository.'
}

Push-Location $root.Trim()
try {
    $cmakePath = Join-Path (Get-Location).Path 'CMakeLists.txt'
    $cmakeText = Get-Content -LiteralPath $cmakePath -Raw
    $match = [regex]::Match(
        $cmakeText,
        '(?is)\bproject\s*\(\s*Kinetalis\s+VERSION\s+(\d+\.\d+\.\d+)\b'
    )
    if (-not $match.Success) {
        throw 'Could not read the Kinetalis VERSION from the top-level CMakeLists.txt.'
    }

    $version = $match.Groups[1].Value
    $tag = "v$version"

    $branch = (& git branch --show-current).Trim()
    if ($LASTEXITCODE -ne 0 -or $branch -ne 'main') {
        throw "Expected branch 'main', found '$branch'."
    }

    $changes = & git status --porcelain
    if ($LASTEXITCODE -ne 0) { throw 'Could not check Git working tree.' }
    if ($changes) {
        throw 'Working tree is not clean. Commit or discard all changes (including this script) first.'
    }

    $existingTag = & git tag --list $tag
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect existing tags.' }
    if ($existingTag) { throw "Tag '$tag' already exists locally. Increase the version or inspect the existing release." }

    if ($PublishRelease) {
        if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
            throw 'GitHub CLI (gh) is required for -PublishRelease. Install and authenticate it, or omit this option.'
        }
        & gh auth status --hostname github.com *> $null
        if ($LASTEXITCODE -ne 0) {
            throw 'GitHub CLI is not authenticated. Run: gh auth login'
        }
    }

    Write-Host "Preparing Kinetalis $tag from branch main..."
    Invoke-Git fetch origin main

    $remoteTag = & git ls-remote --tags origin "refs/tags/$tag"
    if ($LASTEXITCODE -ne 0) { throw 'Could not inspect tags on GitHub.' }
    if ($remoteTag) { throw "Tag '$tag' already exists on GitHub. Do not overwrite a published version." }

    $remoteHead = (& git rev-parse --verify refs/remotes/origin/main)
    if ($LASTEXITCODE -ne 0 -or -not $remoteHead) {
        throw 'Remote branch origin/main was not found.'
    }
    & git merge-base --is-ancestor $remoteHead.Trim() HEAD
    if ($LASTEXITCODE -ne 0) {
        throw 'Local main has diverged from origin/main. Synchronize the branches before releasing.'
    }

    Invoke-Git push origin main
    Invoke-Git tag -a $tag -m "Kinetalis $tag"
    Invoke-Git push origin $tag

    if ($PublishRelease) {
        & gh release create $tag --verify-tag --title "Kinetalis $tag" --generate-notes
        if ($LASTEXITCODE -ne 0) {
            throw "Git tag '$tag' was pushed, but GitHub release creation failed. Create the release from the existing tag."
        }
        Write-Host "GitHub release $tag published."
    }
    else {
        Write-Host "Tag $tag pushed. Create the GitHub release from this tag at:"
        Write-Host 'https://github.com/Gidid56/Kinetalis/releases/new'
        Write-Host 'Or run the script with -PublishRelease next time (requires GitHub CLI).'
    }
}
finally {
    Pop-Location
}
