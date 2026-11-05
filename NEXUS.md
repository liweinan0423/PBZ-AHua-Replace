# Nexus Mods 发布指南

> Nexus Mods 目前**没有公开的文件上传 API**。发布必须在网页端使用作者账号完成
> （登录 / 邮箱验证 / 2FA）。本指南把能提前准备的内容都列好，登录后照填即可。

## 0. 文件清单

本目录已准备好：

| 文件 | 用途 |
|---|---|
| `AHuaReplace_1.0.1.zip` | 主文件（上传到 Nexus） |
| `description.bbcode` | Mod 描述（粘贴到 Description） |
| `changelog.txt` | 更新日志（粘贴到 Changelog） |
| `permissions_credits.txt` | 权限与致谢参考 |
| `preview.png` | 主图 |
| `screenshot_2.png` | 第二张截图 |

## 1. 创建 Mod 页面

1. 登录 <https://www.nexusmods.com>。
2. 打开游戏页：<https://www.nexusmods.com/phantombladezero>。
3. 点击 **Upload a mod** / **Add mod**（不同界面文案可能略有差异）。
4. 选择游戏 **Phantom Blade Zero**。
5. 按下表填写：

| 字段 | 建议值 |
|---|---|
| Mod name | `新娘阿花 独立替换 Mod (AHua Standalone Replacement)` |
| Summary | `Replace protagonist Hun's appearance with boss AHua. Standalone version.dll; extract to game root and run Install.bat.` |
| Category | `Models and Textures` 或 `Appearance`（以页面可选分类为准） |
| Tags | `AHua`, `Bride`, `Player`, `Appearance`, `Model`, `version.dll` |
| Adult content | No |
| Game | Phantom Blade Zero |

6. **Description**：把 `description.bbcode` 的全部内容粘贴进去。
7. 保存/继续。

## 2. 上传主文件

在 Files 页添加文件：

| 字段 | 建议值 |
|---|---|
| File | `AHuaReplace_1.0.1.zip` |
| File name | `AHuaReplace 1.0.1` |
| Version | `1.0.1` |
| Category | `Main files` |
| Description | `Standalone AHua replacement. Extract to game root and run Install.bat.` |
| Requirements | Phantom Blade Zero；Steam build 26367565 |

如果页面要求 **Changelog**，粘贴 `changelog.txt`。

## 3. 图片

上传：

- `preview.png`（设为主图）
- `screenshot_2.png`

## 4. 权限与致谢

在 Permissions / Credits 页填写：

- **Credits**：
  `Jersyi/cringe-blade-zero`（retarget subsystem，固定提交
  `d77f0d9a1b316c4accb5a6d2e8790909b0673029`）；`liweinan0423`（裁剪、自动应用、打包）。
- **Permissions**（建议）：
  - Other users may use this mod for personal use: **YES**
  - Other users may modify this mod for personal use: **YES**
  - Other users may reupload this mod: **NO**
  - Other users may use assets from this mod: **NO**
  - Other users may use this mod in collections: **YES**（请链接回本页面）
  - Commercial use: **NO**
- 在描述中已注明：上游仓库未声明许可证，本 Mod 不主张对 `version.dll` 的额外授权。

## 5. 发布

1. 检查所有字段与文件。
2. 点击 **Publish**。
3. Nexus 可能进行文件扫描/人工审核；DLL 代理可能触发杀软误报，
   描述里已说明它只是转发系统 `version.dll` 的代理。
4. 发布后把 Nexus 链接加到 GitHub README 的下载区。

## 6. 发布后建议

- 在 GitHub Release 描述里加一行 Nexus 链接。
- 在 Nexus 页面固定说明支持的游戏 build（26367565）。
- 游戏更新后先发布新构建，再更新 Nexus 文件版本。

## 备注

- 本 Mod **不修改游戏 pak、不写存档**，只放一个 `version.dll` 代理。
- 与其它 `version.dll` 代理 Mod 不能同时使用。
- 安装时自动备份原有 `version.dll`，卸载可恢复。
- Nexus 页面若把 `.dll` 标记为可疑，可引用本指南与 GitHub 源码构建说明。
