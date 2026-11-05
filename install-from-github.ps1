param(
    [string]$GameDir
)

$ErrorActionPreference = 'Stop'

$url = 'https://github.com/liweinan0423/PBZ-AHua-Replace/releases/latest/download/AHuaReplace.zip'
$temp = Join-Path $env:TEMP ('AHuaReplace_' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $temp -Force | Out-Null

try {
    Write-Host "下载 $url ..."
    $zip = Join-Path $temp 'AHuaReplace.zip'
    Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing

    Write-Host "解压 ..."
    Expand-Archive -LiteralPath $zip -DestinationPath $temp -Force

    $installer = Get-ChildItem -LiteralPath $temp -Recurse -Filter 'install.ps1' |
        Select-Object -First 1
    if (-not $installer) { throw "安装包中缺少 install.ps1" }

    if ($GameDir) {
        & $installer.FullName -GameDir $GameDir
    } else {
        & $installer.FullName
    }
    Write-Host ""
    Write-Host "安装完成。启动游戏并加载存档即可。"
} finally {
    Remove-Item -LiteralPath $temp -Recurse -Force -ErrorAction SilentlyContinue
}
