param(
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\PBZ'
)

$ErrorActionPreference = 'Stop'
$exeDir = Join-Path $GameDir 'platform\steam'
$exe = Join-Path $exeDir 'PBZ.exe'
if (-not (Test-Path -LiteralPath $exe)) { throw "找不到 PBZ.exe：$exe" }

$source = Join-Path $PSScriptRoot 'version.dll'
if (-not (Test-Path -LiteralPath $source)) { throw "找不到 version.dll：$source" }

$target = Join-Path $exeDir 'version.dll'
if (Test-Path -LiteralPath $target) {
    $existing = (Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash
    $ours = (Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash
    if ($existing -ne $ours) {
        $backup = Join-Path $exeDir ('version.dll.backup_' + (Get-Date -Format 'yyyyMMdd_HHmmss'))
        Copy-Item -LiteralPath $target -Destination $backup -Force
        Write-Host "已备份原有 version.dll -> $backup"
    }
}

$running = Get-Process -ErrorAction SilentlyContinue |
    Where-Object { $_.ProcessName -like 'PBZ*' -and $_.WorkingSet64 -gt 2GB }
if ($running) { throw "游戏正在运行，请先退出游戏再安装。" }

Copy-Item -LiteralPath $source -Destination $target -Force
Write-Host "已安装「新娘阿花」独立替换 Mod。"
Write-Host "启动游戏并加载存档后会自动应用；日志：$exeDir\AHuaReplace.log"
