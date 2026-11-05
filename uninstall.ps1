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
$running = Get-Process -ErrorAction SilentlyContinue |
    Where-Object { $_.ProcessName -like 'PBZ*' -and $_.WorkingSet64 -gt 2GB }
if ($running) { throw "游戏正在运行。请先完全退出游戏，然后重新双击 Uninstall.bat。" }

$ours = $null
if (Test-Path -LiteralPath (Join-Path $PSScriptRoot 'version.dll')) {
    $ours = (Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'version.dll') -Algorithm SHA256).Hash
}

$target = Join-Path $exeDir 'version.dll'
if (Test-Path -LiteralPath $target) {
    $hash = (Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash
    if ($ours -and $hash -ne $ours) {
        Write-Host "警告：$target 不是本 Mod 的文件，未删除。"
    } else {
        Remove-Item -LiteralPath $target -Force
        Write-Host "已删除 version.dll"
        $backup = Get-ChildItem -LiteralPath $exeDir -Filter 'version.dll.backup_*' -ErrorAction SilentlyContinue |
            Sort-Object LastWriteTime -Descending | Select-Object -First 1
        if ($backup) {
            Copy-Item -LiteralPath $backup.FullName -Destination $target -Force
            Write-Host "已恢复备份：$($backup.Name)"
        }
    }
}

foreach ($name in @('AHuaReplace.log', 'AHuaReplace.log.previous', 'AHuaReplace.ini')) {
    Remove-Item -LiteralPath (Join-Path $exeDir $name) -Force -ErrorAction SilentlyContinue
}
Write-Host "已卸载「新娘阿花」独立替换 Mod。游戏本体 pak 未被修改。"
