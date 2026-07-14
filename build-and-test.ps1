# RED/GREEN/REFACTOR 에이전트가 공용으로 호출하는 빌드+테스트 스크립트.
# 빌드 실패든 테스트 실패든, 실패 시 0이 아닌 종료 코드를 반환한다.
param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$solution = Join-Path $repoRoot "bowling-tdd.slnx"

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    Write-Error "vswhere.exe not found at $vswhere"
    exit 1
}

$msbuildPath = & $vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe | Select-Object -First 1
if (-not $msbuildPath) {
    Write-Error "MSBuild.exe not found via vswhere"
    exit 1
}

Write-Host "==> Building $solution ($Configuration|$Platform)"
& $msbuildPath $solution "/p:Configuration=$Configuration" "/p:Platform=$Platform" /nologo /verbosity:minimal
if ($LASTEXITCODE -ne 0) {
    Write-Host "==> BUILD FAILED (exit $LASTEXITCODE)"
    exit $LASTEXITCODE
}

$exePath = Join-Path $repoRoot "$Platform\$Configuration\bowling-tdd.exe"
if (-not (Test-Path $exePath)) {
    Write-Error "Test executable not found at $exePath"
    exit 1
}

Write-Host "==> Running tests: $exePath"
& $exePath
exit $LASTEXITCODE
