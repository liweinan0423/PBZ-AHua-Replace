# 从源码构建

本目录是「新娘阿花独立替换 Mod」的裁剪源码。

## 依赖

- Windows 10/11 x64
- Visual Studio 2022
  - 工作负载：**使用 C++ 的桌面开发**
  - 单个组件：**适用于 x64 的 MASM**（`ml64.exe`）
- CMake **3.24+**
- Git（用于拉取上游源码）

## 构建

```powershell
cd source
powershell -ExecutionPolicy Bypass -File build.ps1
```

脚本会：

1. 把上游 `Jersyi/cringe-blade-zero` 克隆到 `source/vendor/cringe-blade-zero`；
2. 切换到固定提交 `d77f0d9a1b316c4accb5a6d2e8790909b0673029`；
3. 用 CMake + VS2022 编译裁剪目标；
4. 输出：

```
source/build/bin/version.dll
```

## 裁剪目标包含什么

| 文件 | 作用 |
|---|---|
| `dllmain.cpp` | `version.dll` 代理入口、版本校验、游戏线程钩子、自动应用 AHua |
| `stubs.cpp` | 上游其它模块（抓取、审计、Toast、热键、Boss 保护）的最小空实现 |
| `version.def` | 转发系统 `version.dll` 的 17 个导出 |
| `CMakeLists.txt` | 只编译 `ue_runtime` + `game_thread` + `settings_store` + `npc_retarget` |

不包含上游的 UI、菜单、Boss Rush、抓取、武器预设、Boss AI 等模块。

## 说明

- 上游源码通过 `source/build.ps1` 按固定提交拉取，**不提交到本仓库**。
- 上游仓库目前未声明许可证；本仓库只提供裁剪构建脚本与文档，不主张对
  `version.dll` 的额外授权。详见仓库根目录 `NOTICE.md`。
- 游戏更新后需要重新编译，并更新 `dllmain.cpp` 中的 `KnownBuild`
  （时间戳、镜像大小、入口点、`PBZ.exe` 大小与 SHA-256）。
