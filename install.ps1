param(
    [string]$GameDir,
    [switch]$CheckOnly
)

$ErrorActionPreference = 'Stop'

function Find-GameRoot([string]$Start) {
    if (-not $Start) { return $null }
    $dir = Get-Item -LiteralPath $Start -ErrorAction SilentlyContinue
    while ($dir) {
        if (Test-Path -LiteralPath (Join-Path $dir.FullName 'platform\steam\PBZ.exe')) {
            return $dir.FullName
        }
        $dir = $dir.Parent
    }
    return $null
}

$root = $null
if ($GameDir) {
    $root = (Resolve-Path -LiteralPath $GameDir).Path
} else {
    # 脚本在 <游戏根>\AHuaReplace\ 或直接放在 <游戏根>\ 时都能找到。
    $root = Find-GameRoot $PSScriptRoot
    if (-not $root) { $root = 'C:\Program Files (x86)\Steam\steamapps\common\PBZ' }
}

$exe = Join-Path $root 'platform\steam\PBZ.exe'
if (-not (Test-Path -LiteralPath $exe)) {
    throw "找不到游戏目录。请把整个 AHuaReplace 文件夹解压到游戏根目录（能看到 platform 文件夹的那一层）。"
}

if ($CheckOnly) {
    Write-Host "游戏目录：$root"
    exit 0
}

$exeDir = Join-Path $root 'platform\steam'
$source = Join-Path $PSScriptRoot 'version.dll'
if (-not (Test-Path -LiteralPath $source)) { throw "找不到 version.dll：$source" }

$running = Get-Process -ErrorAction SilentlyContinue |
    Where-Object { $_.ProcessName -like 'PBZ*' -and $_.WorkingSet64 -gt 2GB }
if ($running) { throw "游戏正在运行。请先完全退出游戏，然后重新双击 Install.bat。" }

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

Copy-Item -LiteralPath $source -Destination $target -Force
Write-Host "检测到游戏目录：$root"
Write-Host "安装完成！请启动游戏并加载存档。"
Write-Host "日志：$exeDir\AHuaReplace.log"
