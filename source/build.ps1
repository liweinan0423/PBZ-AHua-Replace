param(
    [string]$Upstream = 'https://github.com/Jersyi/cringe-blade-zero.git',
    [string]$Commit = 'd77f0d9a1b316c4accb5a6d2e8790909b0673029'
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    throw "找不到 CMake。请安装 CMake 3.24+ 并加入 PATH：https://cmake.org/download/"
}

$vendor = Join-Path $root 'vendor\cringe-blade-zero'
if (-not (Test-Path -LiteralPath $vendor)) {
    Write-Host "克隆上游源码 $Upstream ..."
    git clone $Upstream $vendor
    if ($LASTEXITCODE -ne 0) { throw "git clone 失败" }
}

Write-Host "切换到固定提交 $Commit ..."
git -C $vendor checkout --force $Commit
if ($LASTEXITCODE -ne 0) { throw "git checkout $Commit 失败" }

Write-Host "配置 CMake ..."
& $cmake.Source -S $root -B (Join-Path $root 'build') -G 'Visual Studio 17 2022' -A x64 -DREF="$vendor\src"
if ($LASTEXITCODE -ne 0) { throw "CMake 配置失败" }

Write-Host "编译 Release ..."
& $cmake.Source --build (Join-Path $root 'build') --config Release --parallel
if ($LASTEXITCODE -ne 0) { throw "编译失败" }

$dll = Join-Path $root 'build\bin\version.dll'
if (-not (Test-Path -LiteralPath $dll)) { throw "没有生成 $dll" }
Write-Host ""
Write-Host "构建完成：$dll"
Write-Host "把该文件复制到 <游戏目录>\platform\steam\version.dll 即可。"
