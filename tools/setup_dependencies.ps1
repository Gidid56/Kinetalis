param(
    [switch]$LatestImGui
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$repository_root = Split-Path -Parent $PSScriptRoot
$external = Join-Path $repository_root "external"

New-Item -ItemType Directory -Force -Path $external | Out-Null

function Invoke-Git
{
    param(
        [Parameter(Mandatory)]
        [string]$Repository,

        [Parameter(Mandatory)]
        [string[]]$Arguments,

        [Parameter(Mandatory)]
        [string]$FailureMessage
    )

    & git -C $Repository @Arguments

    if ($LASTEXITCODE -ne 0)
    {
        throw $FailureMessage
    }
}

$magnum_integration_revision = if ($LatestImGui)
{
    @{ Type = "PullRequest"; Value = "pull/119/head" }
}
else
{
    @{ Type = "Branch"; Value = "master" }
}

$imgui_revision = if ($LatestImGui)
{
    @{ Type = "Branch"; Value = "master" }
}
else
{
    @{ Type = "Tag"; Value = "v1.88" }
}

$implot_revision = if ($LatestImGui)
{
    @{ Type = "Branch"; Value = "master" }
}
else
{
    @{ Type = "Tag"; Value = "v0.16" }
}

$dependencies = @(
    @{ Name = "corrade"; Url = "https://github.com/mosra/corrade.git"; Revision = @{ Type = "Branch"; Value = "master" } },
    @{ Name = "magnum"; Url = "https://github.com/mosra/magnum.git"; Revision = @{ Type = "Branch"; Value = "master" } },
    @{ Name = "magnum-integration"; Url = "https://github.com/mosra/magnum-integration.git"; Revision = $magnum_integration_revision },
    @{ Name = "SDL"; Url = "https://github.com/libsdl-org/SDL.git"; Revision = @{ Type = "Branch"; Value = "SDL2" } },
    @{ Name = "imgui"; Url = "https://github.com/ocornut/imgui.git"; Revision = $imgui_revision },
    @{ Name = "implot"; Url = "https://github.com/epezent/implot.git"; Revision = $implot_revision }
)

foreach ($dependency in $dependencies)
{
    $destination = Join-Path $external $dependency.Name
    $git_directory = Join-Path $destination ".git"

    if (-not (Test-Path $destination))
    {
        Write-Host "Cloning $($dependency.Name)..."
        & git clone $dependency.Url $destination

        if ($LASTEXITCODE -ne 0)
        {
            throw "Failed to clone $($dependency.Name)."
        }
    }
    elseif (-not (Test-Path $git_directory))
    {
        throw "Dependency path exists but is not a Git checkout: $destination"
    }

    $revision_type = $dependency.Revision.Type
    $revision_value = $dependency.Revision.Value

    switch ($revision_type)
    {
        "Branch"
        {
            Write-Host "Updating $($dependency.Name) from branch $revision_value..."
            Invoke-Git $destination @("fetch", "origin", $revision_value) "Failed to fetch branch $revision_value for $($dependency.Name)."

            & git -C $destination switch $revision_value

            if ($LASTEXITCODE -ne 0)
            {
                Invoke-Git $destination @("switch", "--track", "-c", $revision_value, "origin/$revision_value") "Failed to create branch $revision_value for $($dependency.Name)."
            }

            Invoke-Git $destination @("pull", "--ff-only", "origin", $revision_value) "Failed to update $($dependency.Name) from branch $revision_value."
        }

        "Tag"
        {
            Write-Host "Checking out $($dependency.Name) tag $revision_value..."
            Invoke-Git $destination @("fetch", "--tags", "origin") "Failed to fetch tags for $($dependency.Name)."
            Invoke-Git $destination @("switch", "--detach", $revision_value) "Failed to check out tag $revision_value for $($dependency.Name)."
        }

        "PullRequest"
        {
            Write-Host "Checking out $($dependency.Name) pull request $revision_value..."
            Invoke-Git $destination @("fetch", "origin", $revision_value) "Failed to fetch pull request $revision_value for $($dependency.Name)."
            Invoke-Git $destination @("switch", "--detach", "FETCH_HEAD") "Failed to check out pull request $revision_value for $($dependency.Name)."
        }

        default
        {
            throw "Unknown revision type '$revision_type' for $($dependency.Name)."
        }
    }
}

if ($LatestImGui)
{
    Write-Host "Viewer dependencies are ready in experimental latest-ImGui mode."
}
else
{
    Write-Host "Viewer dependencies are ready with Dear ImGui v1.88 and ImPlot v0.16."
}
