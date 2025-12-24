# 基于 ESPHome 的 Haier 空调 (KFR-26GW) 红外控制器

[[Readme English]](./README_EN.md)

## 简介

本项目基于 [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) 提供的海尔空调红外编码协议，通过 ESPHome 自定义组件的方式，实现对 **Haier KFR-26GW 系列空调**的红外控制。

通过组合 `number`、`switch` 与 `select` 三类组件，对空调的各项功能进行精细化映射与控制，当前已支持的功能包括：

- 空调电源开 / 关
- 运行模式选择（自动、制热、制冷、除湿、送风）
- 目标温度设置（16 ℃ – 30 ℃）
- 出风口摆动模式选择
- 出风速度选择
- 定时开 / 关空调
- 空调状态显示开关
- 童锁模式开关
- 辅热模式开关
- 静音模式开关
- 睡眠模式开关
- 强劲模式开关
- 健康模式开关
- 自清洁模式开关

> [!NOTE] 
> 由于 ESPHome 的 [climate 组件](https://esphome.io/components/climate/) 在功能模型上无法与 [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) 提供的海尔红外协议实现一一对应，因此本项目 **未基于 climate 组件实现**。
>
> 因此，在接入 Home Assistant 后，无法使用标准的 `climate` UI 卡片，而需要分别通过
>  `number`、`switch` 与 `select` 实体进行控制。

> [!IMPORTANT] 
> 目前空调的**核心功能**（如电源控制、运行模式选择、温度设置等）工作正常。
>
> 但在测试过程中发现，**部分功能**（例如空调状态显示控制、出风口摆动模式设置等）与预期行为存在不一致的情况。
>
> 该问题初步判断与 [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) 中对应红外协议的实现细节有关，仍有待进一步分析与验证。

------

## 如何使用

### ESPHome 配置

由于本项目尚未合入 ESPHome 官方仓库，需要通过 [external_components](https://esphome.io/components/external_components/) 的方式引入自定义组件 `haier_ac_kfr26gw`。

在 ESPHome 的 YAML 配置文件中添加如下内容即可：

```yaml
external_components:
  - source: github://CCBP/ESPHome-HaierAC_KFR26GW-IRController@main
    components: [ haier_ac_kfr26gw ]

haier_ac_kfr26gw:
  id: haier_ac
  name: "Haier Air Conditioner"
  pin: GPIO4
```

其中：

- `id`：组件内部 ID，用于引用
- `name`：在 ESPHome / Home Assistant 中显示的名称
- `pin`：连接红外发射 LED 的 GPIO 引脚编号

> [!WARNING] 
> 请确保红外 LED 已正确连接，并配合限流电阻或三极管驱动，否则可能导致发射距离不足或 GPIO 过载。

------

#### 示例配置说明

仓库中提供了示例配置文件 `example.yaml`。
在使用该示例前，需要在 **同级目录下创建 `secrets.yaml` 文件**，并根据实际环境填写以下内容：

```yaml
ota_password: 123456
wifi_ssid: my_wifi
wifi_password: 12345678
```

完成配置后，使用 ESPHome 命令行工具进行编译与烧录：

```shell
esphome run example.yaml
```

该组件可运行于 **ESP8266 或 ESP32** 平台（需具备红外发送能力）。

------

### Home Assistant 集成

由于本项目未基于 `climate` 组件实现，在 Home Assistant 仪表盘中**无法使用官方空调卡片**，所有控制均以 `number`、`switch` 与 `select` 实体的形式呈现。

为获得更友好、直观的控制体验，仓库中提供了一个示例 Lovelace UI 配置文件 `ui-lovelace.yaml`，用于构建专用空调控制卡片。

该 UI 样式参考并改编自以下社区方案：

- [【自制分享】清爽简洁 + 自适应主题的空调卡片](https://bbs.hassbian.com/thread-26328-1-1.html)

#### UI 依赖组件

在使用 `ui-lovelace.yaml` 前，请确保 Home Assistant 已安装以下前端插件：

- Decluttering Card
- Mushroom Card
- Button Card
- Mini Graph Card
- Card Mod

#### 界面效果示例

![haier_ac_kfr26gw](./assets/haier_ac_kfr26gw_card.jpg)

------

## 已知问题与待办事项（TODO）

目前项目已实现基本可用功能，但仍有不少问题与可优化空间，有待后续完善：

### 功能与 BUG 修复

- 系统性测试 IRremoteESP8266 提供的全部海尔协议字段
- 分析并修复以下功能异常问题：
  - 空调状态显示控制
  - 出风口摆动模式设置等

### ESPHome 组件优化

- 精简与合并重复代码（如 `select` / `switch` 回调逻辑高度相似的问题）
- 优化定时开 / 关空调逻辑：
  - 考虑结合 `select` 与 `datetime` 组件
  - 改善 Home Assistant 端的配置体验
- 集成 ESPHome Web Server 的 `sorting_weight` 属性，改善 Web UI 中实体排序混乱的问题
- 除核心功能（电源 / 温度 / 模式）外，其余功能默认禁用，通过 YAML 配置显式开启

### Home Assistant Lovelace UI 优化

- 将 `ui-lovelace.yaml` 中的模板拆分为独立文件，降低维护成本
- 引入 `select` 模板，简化：
  - 出风口摆动模式选择
  - 出风速度选择相关卡片代码
- 增加定时开 / 关空调的独立卡片：
  - 仅在目标时间到达后由 ESPHome 执行动作
- 补充 `ui-lovelace.yaml` 的详细使用说明
- 针对不同运行模式优化配色方案

## 参考

- [ChatGPT](https://chatgpt.com/)
- [DeepSeek](https://chat.deepseek.com/)
- [【自制分享】[8.4更新]清爽简洁+自适应主题的空调卡片](https://bbs.hassbian.com/thread-26328-1-1.html)
- [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)