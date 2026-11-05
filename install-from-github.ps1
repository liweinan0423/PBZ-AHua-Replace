param(
    [string]$GameDir = 'C:\Program Files (x86)\Steam\steamapps\common\PBZ',
    [string]$Tag = 'v1.0.0'
)

$ErrorActionPreference = 'Stop'

$url = "https://github.com/liweinan0423/PBZ-AHua-Replace/releases/download/$Tag/AHuaReplace_20261105.zip"
$temp = Join-Path $env:TEMP ('AHuaReplace_' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $temp -Force | Out-Null

try {
    Write-Host "下载 $url ..."
    $zip = Join-Path $temp 'AHuaReplace.zip'
    Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing

    Write-Host "解压 ..."
    Expand-Archive -LiteralPath $zip -DestinationPath $temp -Force

    $installer = Join-Path $temp 'install.ps1'
    if (-not (Test-Path -LiteralPath $installer)) { throw "安装包中缺少 install.ps1" }

    & $installer -GameDir $GameDir
    Write-Host ""
    Write-Host "安装完成。启动游戏并加载存档即可。"
} finally {
    Remove-Item -LiteralPath $temp -Recurse -Force -ErrorAction SilentlyContinue
}
