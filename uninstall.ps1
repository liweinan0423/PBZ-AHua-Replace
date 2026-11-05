param(
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\PBZ'
)

$ErrorActionPreference = 'Stop'
$exeDir = Join-Path $GameDir 'platform\steam'
$target = Join-Path $exeDir 'version.dll'

$running = Get-Process -ErrorAction SilentlyContinue |
    Where-Object { $_.ProcessName -like 'PBZ*' -and $_.WorkingSet64 -gt 2GB }
if ($running) { throw "游戏正在运行，请先退出游戏再卸载。" }

$ours = $null
if (Test-Path -LiteralPath (Join-Path $PSScriptRoot 'version.dll')) {
    $ours = (Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'version.dll') -Algorithm SHA256).Hash
}

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
