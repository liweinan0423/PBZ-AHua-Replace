# v1.0.0 — 新娘阿花独立替换 Mod

首个独立版本：把主角「魂」的外观替换为 Boss「新娘阿花」，保留原主角的
操作、动画、碰撞、相机与武器系统。

## 下载

- **AHuaReplace_20261105.zip**
  <https://github.com/liweinan0423/PBZ-AHua-Replace/releases/download/v1.0.0/AHuaReplace_20261105.zip>

## 支持版本

- Steam build **26367565**（2026-11-04 更新）
- `PBZ.exe` SHA-256：
  `4415933ce33476f14c837abb61a1b60af8b78cd81783896501913a7aac69e423`

游戏更新后 DLL 会输出 `UNSUPPORTED_BUILD` 并停止工作，不会修改游戏内存。

## 安装

1. 完全退出游戏。
2. 解压 zip。
3. 运行：

   ```powershell
   powershell -ExecutionPolicy Bypass -File install.ps1
   ```

   非默认 Steam 目录：

   ```powershell
   powershell -ExecutionPolicy Bypass -File install.ps1 -GameDir "D:\Steam\steamapps\common\PBZ"
   ```

4. 启动游戏，加载任意存档；进入场景后约 1–3 秒自动应用。

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

```powershell
powershell -ExecutionPolicy Bypass -File uninstall.ps1
```

会删除 `version.dll` 并恢复安装前备份的 `version.dll.backup_<时间戳>`。

## 说明

- 不修改游戏 pak，不写存档。
- 安装时自动备份原有 `version.dll`。
- 只替换本地显示层；不影响操作、判定、动画蓝图。
- 与其它 `version.dll` 代理 Mod 不能同时使用。

## 已知限制

- 按 `PBZ.exe` 版本锁定。
- 手部绑定姿态与 Hun 约有 38–56° 差异，极端姿势腕部可能轻微变形。
- 攻击、技能、过场、传送后的表现建议继续实测。

## 来源

重定向子系统来自
[Jersyi/cringe-blade-zero](https://github.com/Jersyi/cringe-blade-zero)
（固定提交 `d77f0d9a1b316c4accb5a6d2e8790909b0673029`）。
详见仓库 `NOTICE.md`。

## SHA-256

```text
706b0a5c6f2d6b49018aa1b618d560ec007aab31032bfcd15e12a9b6c8647852  version.dll
9ceec69dbf67744ac244947832cbd931706e45cf02afa01488fc222d8873e902  AHuaReplace_20261105.zip
```
