# v1.0.1 — 简化安装流程

## 变化

- 安装改为两步：**把 `AHuaReplace` 解压到游戏根目录 → 双击 `Install.bat`**。
- `install.ps1` 会自动向上查找游戏根目录（`platform\steam\PBZ.exe` 所在层）。
- 新增 `Install.bat` / `Uninstall.bat`：Windows 默认双击 `.ps1` 会用记事本
  打开，所以用 `.bat` 作为双击入口。
- 一键安装脚本改为下载最新 Release 的 `AHuaReplace.zip`。

## 下载

**AHuaReplace.zip**
<https://github.com/liweinan0423/PBZ-AHua-Replace/releases/latest/download/AHuaReplace.zip>

## 安装（两步）

1. **完全退出游戏**（包括后台进程 / 托盘）。
2. 把整个 **AHuaReplace 文件夹**解压到游戏根目录，例如：

   ```
   C:\Program Files (x86)\Steam\steamapps\common\PBZ\
   ```

   解压后应同时看到：

   ```
   ...\PBZ\AHuaReplace\
   ...\PBZ\platform\
   ```

3. 双击：

   ```
   AHuaReplace\Install.bat
   ```

4. 启动游戏，加载任意存档；进入场景后约 1–3 秒自动应用「新娘阿花」。

> Windows 默认双击 `.ps1` 会用记事本打开；请双击 `Install.bat`，
> 它等价于运行 `install.ps1`。

非默认 Steam 目录也可以直接指定：

```powershell
powershell -ExecutionPolicy Bypass -File install.ps1 -GameDir "D:\Steam\steamapps\common\PBZ"
```

## 验证

日志：

```
<游戏目录>\platform\steam\AHuaReplace.log
```

成功标志：

```
KNOWN_BUILD sha256 verified
GAME_THREAD_HOOK installed; AHua auto-apply armed
NPC_RETARGET applied id=AHua ... display=AHua_Body
AHUA_ACTIVE
```

## 卸载

完全退出游戏，双击 `AHuaReplace\Uninstall.bat`。

会删除 `version.dll` 并恢复安装前备份的 `version.dll.backup_<时间戳>`。

## 支持版本

- Steam build **26367565**（2026-11-04 更新）
- `PBZ.exe` SHA-256：
  `4415933ce33476f14c837abb61a1b60af8b78cd81783896501913a7aac69e423`

游戏更新后 DLL 会输出 `UNSUPPORTED_BUILD` 并停止工作，不会修改游戏内存。

## 来源

重定向子系统来自
[Jersyi/cringe-blade-zero](https://github.com/Jersyi/cringe-blade-zero)
（固定提交 `d77f0d9a1b316c4accb5a6d2e8790909b0673029`）。
详见仓库 `NOTICE.md`。

## SHA-256

```text
706b0a5c6f2d6b49018aa1b618d560ec007aab31032bfcd15e12a9b6c8647852  version.dll
d723c6ba473cbaa589ef9ca727b6685ec9d9fad281d42eeca76a51b8040fd66e  AHuaReplace.zip
```
