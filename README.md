# Aura RPG — 基于 UE5 GAS 的动作 RPG 战斗系统

UE 5.6 + Gameplay Ability System 实现的第三人称动作 RPG 战斗系统。
69 个 C++ 文件 / 约 2600 行，C++ 与蓝图混合开发。

核心代码位于 `Source/RPG/`；工程使用课程配套资产包（角色 / 场景 / 特效 / 音效），
因此仓库体积较大，如需快速浏览请直接进入 `Source/`。

## 技术要点

- **GAS 全链路**：AttributeSet 属性 → GameplayAbility 技能 → 自定义 AbilityTask →
  目标数据 → 投射物 → 伤害计算 → UI 反馈
- **伤害计算**：自定义 ExecutionCalculation（护甲 / 格挡 / 暴击），
  Meta Attribute + SetByCaller 传参；属性 Modifier 由 MMC 按等级与职业计算
- **技能消耗与冷却**：由 GameplayEffect 驱动；Cooldown Tag + 自定义异步任务
  监听 Tag 增删，驱动 HUD 倒计时
- **数据驱动**：CharacterClassInfo / MMC（Modifier Magnitude Calculation）/
  AbilityInfo 配置在 DataAsset 中，调整数值无需改代码
- **UI 架构**：HUD → WidgetController → Widget 的单向下行数据流；
  初始值 pull、变化 push（委托）；幽灵血条延迟跟随
- **输入**：Enhanced Input + InputConfig DataAsset，用 GameplayTag 绑定技能，
  光标命中驱动点击寻路

## 关键实现

**1. 冷却显示用异步任务，不做每帧轮询**

HUD 的冷却倒计时不是每帧 Tick 去查剩余时间，而是注册一个 `WaitCooldownChange`
异步任务：它监听该技能的 Cooldown Tag 增删事件，Tag 被添加时广播开始、被移除时广播结束，
UI 只在两个时刻收到通知。代价是一次注册，收益是零轮询开销。

**2. 伤害逻辑全部收在 ExecutionCalculation 里**

技能本身只负责「发出攻击」，不关心怎么算伤害。护甲减免、格挡、暴击都在
`ExecCalc_Damage` 中按 capture 到的属性计算，结果写入 Meta Attribute 再由
AttributeSet 派生到 Health。调整战斗数值或新增伤害类型时只改这一个类，
所有技能自动生效。

**3. WidgetController 作为 ASC 与 UI 之间唯一的一层**

Widget 不直接持有 ASC，也不直接读属性，只从 WidgetController 拿数据。
初值在初始化时主动 pull，后续变化由 Controller 把 ASC 的委托转发给 Widget（push）。
UI 与 GAS 因此完全解耦，更换 UI 不需要改动任何战斗代码。

## 操作

| 输入 | 功能 |
|---|---|
| W / A / S / D | 角色移动 |
| 鼠标点击地面 | 点击寻路移动（光标命中驱动） |
| 技能按键 | 由 InputConfig DataAsset 配置，用 GameplayTag 绑定到技能 |

## 项目结构

```
Source/RPG/
├── Public/                        # 头文件
│   ├── AbilitySystem/             # ASC、AttributeSet、AssetManager、GameplayTags
│   │   ├── Abilities/             # 技能基类、火球技能
│   │   ├── AbilityTasks/          # 鼠标目标选择（自定义 AbilityTask）
│   │   ├── AsyncTasks/            # WaitCooldownChange：监听冷却 Tag 变化
│   │   ├── ExecCalc/              # 伤害计算 ExecutionCalculation
│   │   ├── ModMagCalc/            # MMC：按等级计算最大血量 / 蓝量
│   │   └── Data/                  # AbilityInfo 等数据结构
│   ├── Character/  Player/  Input/  Interaction/  Actor/  Game/
│   └── UI/                        # HUD / Widget / WidgetController
└── Private/                       # 实现（结构与 Public 对应）
```
