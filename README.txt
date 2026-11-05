新娘阿花 独立替换 Mod
========================

把《影之刃零 / Phantom Blade Zero》的主角「魂」外观替换为 Boss「新娘阿花」。
不修改游戏 pak，不写入存档；只放一个 version.dll 代理。

支持的游戏版本
----------------
Steam build 26367565（2026-11-04 更新）
PBZ.exe SHA-256:
4415933ce33476f14c837abb61a1b60af8b78cd81783896501913a7aac69e423

游戏更新后 DLL 会拒绝工作（不会写游戏内存），需要等新版本。

安装（两步）
------------
1. 完全退出游戏。
2. 把整个 AHuaReplace 文件夹解压到游戏根目录，例如：
   C:\Program Files (x86)\Steam\steamapps\common\PBZ\
   解压后应同时看到：
     ...\PBZ\AHuaReplace\
     ...\PBZ\platform\
3. 双击 AHuaReplace\Install.bat。
4. 启动游戏，加载任意存档；进入场景后约 1-3 秒自动应用。

说明：
Windows 默认双击 .ps1 会用记事本打开，所以请双击 Install.bat。
Install.bat 等价于运行 install.ps1，并会自动查找游戏目录。

日志
----
<游戏目录>\platform\steam\AHuaReplace.log
出现 AHUA_ACTIVE 表示已应用。
出现 UNSUPPORTED_BUILD 表示游戏版本不匹配。

原理
----
AHua_Body 的 161 根骨骼与主角实际身体 Hun_Body_Simple（264 骨）100% 同名、
父子关系完全一致。DLL 在游戏线程上把 AHua 网格、3 个 Chaos 布料部件挂到
主角的显示组件，并按骨名共享主角的实时姿势；同时保留主角的动画、碰撞、
相机和 16 条原生武器挂点。

卸载
----
1. 完全退出游戏。
2. 双击 Uninstall.bat。
   它会删除 version.dll、恢复安装前备份的 version.dll，并清理日志。

已知限制
--------
- 依赖 version.dll 代理，仅适用于上面列出的 PBZ.exe 版本。
- 攻击、技能、过场、传送后的表现仍建议继续实测。
- AHua 与 Hun 的手部绑定姿态约有 38-56° 差异，极端姿势下腕部可能轻微变形。
- 这是外观替换，不改变玩家操作、判定或存档数据。

文件
----
version.dll      独立运行时替换组件
Install.bat      双击安装
Uninstall.bat    双击卸载
install.ps1      安装脚本（由 Install.bat 调用）
uninstall.ps1    卸载脚本（由 Uninstall.bat 调用）
README.txt       本说明
preview.png      预览图
