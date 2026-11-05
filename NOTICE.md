# NOTICE / 来源说明

## 上游项目

本仓库的 `version.dll` 是从以下开源项目的 **NPC 重定向子系统** 裁剪构建的衍生二进制：

- 项目：**Jersyi/cringe-blade-zero**
- 地址：<https://github.com/Jersyi/cringe-blade-zero>
- 固定提交：`d77f0d9a1b316c4accb5a6d2e8790909b0673029`

裁剪后只保留：

- `ue_runtime`（UE 5.7 运行时读取）
- `game_thread`（游戏线程 tick 钩子）
- `npc_retarget`（NPC/Boss 外观重定向）
- 必要的生成数据（AHua 档案、骨骼映射、武器挂点）

不包含上游的 UI、菜单、Boss Rush、抓取、武器预设、Boss AI 等模块。

## 许可证状态

上游仓库 **当前没有声明许可证（no license）**。

因此：

- 本仓库**不主张**对 `version.dll` 的额外授权；
- 二进制按“原样（as-is）”提供，仅供个人学习与单机使用；
- 如果你要再分发或商用，请先联系上游作者取得授权。

本仓库中的安装脚本、文档与裁剪源码由 **liweinan0423** 编写，
可自由用于安装与构建本 Mod。

## 商标与版权

《影之刃零 / Phantom Blade Zero》及其角色、素材、商标归 **S-GAME / 官方** 所有。
本 Mod 为非官方粉丝作品，与官方无任何关联，也未获得官方背书。

## 第三方组件

`version.dll` 是系统 `version.dll` 的代理，会转发所有原始导出函数到
`C:\Windows\System32\version.dll`。它不替换系统文件，只放在游戏目录中。
