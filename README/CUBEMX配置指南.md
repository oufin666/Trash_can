# CubeMX 配置指南

## 一、基础配置

### 1.1 时钟配置
1. 打开 **RCC** → 配置高速外部时钟 **HSE** → 选择 **Crystal/Ceramic Resonator**
2. 进入 **Clock Configuration** 标签页
3. 配置系统时钟为 **168MHz**：
   - Input frequency: 25 MHz
   - PLL Source Mux: HSE
   - PLLM: /25
   - PLLN: x336
   - PLLP: /2
   - System Clock Mux: PLLCLK

### 1.2 调试配置
1. 打开 **SYS** → 找到 **Debug** 下拉菜单
2. 从下拉菜单中选择 **Serial Wire** 选项
   - 注意：如果默认显示为 **Disable**，请点击下拉菜单并选择 **Serial Wire**

### 1.3 烧录方式
选择 **Serial Wire** 后，使用以下设备进行烧录：
- **ST-Link V2** 或 **ST-Link V3** 调试器
- **J-Link** 调试器（支持Serial Wire协议）

**烧录连接：**
| 调试器引脚 | STM32引脚 | 说明 |
|------------|-----------|------|
| SWDIO | PA13 | 串行数据输入/输出 |
| SWCLK | PA14 | 串行时钟 |
| GND | GND | 地线 |
| VCC | 3.3V | 电源（可选，通常由目标板供电） |

---

## 二、GPIO 配置（电机驱动、蜂鸣器）

### 2.1 具体配置步骤

#### 步骤1：打开GPIO配置页面
1. 在CubeMX主界面左侧选择 **GPIO**
2. 进入 **Pinout & Configuration** 标签页

#### 步骤2：配置方向控制引脚（GPIO_Output）

**电机1方向控制：**
1. 找到并点击 **PG11** 引脚
2. 在右侧配置面板中：
   - **GPIO output level**: Low
   - **GPIO mode**: Output Push Pull
   - **GPIO Pull-up/Pull-down**: No pull-up and no pull-down
   - **Maximum output speed**: Low
   - **User Label**: M1_AIN1
3. 找到并点击 **PG15** 引脚
4. 同样配置，**User Label** 设为 M1_AIN2

**电机2方向控制：**
1. 找到并点击 **PD2** 引脚 → **User Label**: M2_BIN1
2. 找到并点击 **PD3** 引脚 → **User Label**: M2_BIN2

**电机3方向控制：**
1. 找到并点击 **PC10** 引脚 → **User Label**: M3_AIN1
2. 找到并点击 **PA8** 引脚 → **User Label**: M3_AIN2

**电机4方向控制：**
1. 找到并点击 **PG5** 引脚 → **User Label**: M4_BIN1
2. 找到并点击 **PG6** 引脚 → **User Label**: M4_BIN2

**LED指示灯：**
1. 找到并点击 **PC13** 引脚 → **User Label**: LED

**蜂鸣器：**
1. 找到并点击 **PF10** 引脚 → **User Label**: BUZZ

#### 步骤3：配置PWM引脚（TIM3）
1. 在左侧选择 **Timers** → **TIM3**
2. **Clock Source** 选择 **Internal Clock**
3. **Channel1** 下拉菜单选择 **PWM Generation CH1**
4. **Channel2** 下拉菜单选择 **PWM Generation CH2**
5. **Channel3** 下拉菜单选择 **PWM Generation CH3**
6. **Channel4** 下拉菜单选择 **PWM Generation CH4**
7. 进入 **Parameter Settings** 标签页，配置定时器参数：
   - **Prescaler**: 167
   - **Counter Period**: 999
   - **Auto-reload preload**: Enabled
8. 进入 **NVIC Settings** 标签页，确保所有中断都不勾选（暂时不需要定时器中断）
9. 回到主界面，点击每个PWM引脚，在右侧配置面板中设置：
   - **PC6** → **User Label**: M1_PWM
   - **PC7** → **User Label**: M2_PWM
   - **PC8** → **User Label**: M3_PWM
   - **PC9** → **User Label**: M4_PWM

### 2.2 配置参数表

#### 电机1 (TB6612 A通道)
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PG11 | GPIO_Output | M1_AIN1 | 方向控制1 |
| PG15 | GPIO_Output | M1_AIN2 | 方向控制2 |
| PC6 | TIM3_CH1 | M1_PWM | PWM输出 |

#### 电机2 (TB6612 B通道)
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PD2 | GPIO_Output | M2_BIN1 | 方向控制1 |
| PD3 | GPIO_Output | M2_BIN2 | 方向控制2 |
| PC7 | TIM3_CH2 | M2_PWM | PWM输出 |

#### 电机3 (TB6612 A通道)
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PC10 | GPIO_Output | M3_AIN1 | 方向控制1 |
| PA8 | GPIO_Output | M3_AIN2 | 方向控制2 |
| PC8 | TIM3_CH3 | M3_PWM | PWM输出 |

#### 电机4 (TB6612 B通道)
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PG5 | GPIO_Output | M4_BIN1 | 方向控制1 |
| PG6 | GPIO_Output | M4_BIN2 | 方向控制2 |
| PC9 | TIM3_CH4 | M4_PWM | PWM输出 |

#### LED 指示
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PC13 | GPIO_Output | LED | 状态指示灯 |

#### 蜂鸣器
| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PF10 | GPIO_Output | BUZZ | 蜂鸣器控制 |

---

## 三、PWM 定时器配置 (TIM3)

1. 打开 **TIM3**
2. **Clock Source**: Internal Clock
3. **Channel1**: PWM Generation CH1 (M1_PWM)
4. **Channel2**: PWM Generation CH2 (M2_PWM)
5. **Channel3**: PWM Generation CH3 (M3_PWM)
6. **Channel4**: PWM Generation CH4 (M4_PWM)

### 参数配置
| 参数 | 值 | 说明 |
|------|-----|------|
| Prescaler | 167 | 168MHz/(167+1)=1MHz |
| Counter Period | 999 | 1MHz/1000=1kHz PWM频率 |
| Auto-reload preload | Enable | 使能预装载 |

### 各通道配置
- **Pulse**: 0 (初始占空比0)
- **Mode**: PWM Mode 1
- **CH Polarity**: High

---

## 四、串口配置

### 4.1 USART2 (K230摄像头通信)
| 参数 | 值 |
|------|-----|
| 波特率 | 115200 |
| 数据位 | 8 |
| 停止位 | 1 |
| 校验 | None |

**引脚配置：**
- PD5: USART2_TX
- PD6: USART2_RX

**DMA配置（推荐）：**
1. 进入 **DMA Settings** 标签页
2. 点击 **Add** 按钮添加DMA请求
3. 配置USART2_RX：
   - DMA Request: USART2_RX
   - Stream: DMA1 Stream5
   - Channel: Channel4
   - Direction: Peripheral To Memory
   - Mode: Circular (循环模式)
   - Priority: Low
4. 配置USART2_TX：
   - DMA Request: USART2_TX
   - Stream: DMA1 Stream6
   - Channel: Channel4
   - Direction: Memory To Peripheral
   - Mode: Normal (正常模式)
   - Priority: Low

### 4.2 USART3 (调试/蓝牙)
| 参数 | 值 |
|------|-----|
| 波特率 | 115200 |
| 数据位 | 8 |
| 停止位 | 1 |
| 校验 | None |

**引脚配置：**
- PD8: USART3_TX
- PB11: USART3_RX

---

## 五、FreeRTOS 配置（如使用RTOS）

### 5.1 启用 FreeRTOS
1. **Middleware** → **FREERTOS** → Interface 选择 **CMSIS_V2**

### 5.2 配置参数
| 参数 | 值 | 说明 |
|------|-----|------|
| USE_PREEMPTION | Enabled | 抢占式调度 |
| CPU_CLOCK_HZ | SystemCoreClock | CPU主频（自动设置） |
| TICK_RATE_HZ | 1000 | 系统节拍1ms |
| MAX_PRIORITIES | 56 | 最大优先级数 |
| MINIMAL_STACK_SIZE | 128 Words | 最小任务栈 |
| TOTAL_HEAP_SIZE | 15360 Bytes | 堆内存大小（根据实际硬件调整） |

### 5.3 创建任务（在CubeMX中）

**步骤1：打开任务配置**
- 进入 **Tasks and Queues** 标签页
- 点击 **Add** 按钮添加任务

**步骤2：配置VisionTask**
| 配置项 | 值 |
|--------|-----|
| Task Name | VisionTask |
| Priority | osPriorityHigh |
| Stack Size (Words) | 512 |
| Entry Function | StartVisionTask |
| Code Generation Option | Default |
| Allocation | Dynamic |

**步骤3：配置MotionTask**
| 配置项 | 值 |
|--------|-----|
| Task Name | MotionTask |
| Priority | osPriorityAboveNormal |
| Stack Size (Words) | 512 |
| Entry Function | StartMotionTask |
| Code Generation Option | Default |
| Allocation | Dynamic |

**步骤4：配置StateTask**
| 配置项 | 值 |
|--------|-----|
| Task Name | StateTask |
| Priority | osPriorityNormal |
| Stack Size (Words) | 256 |
| Entry Function | StartStateTask |
| Code Generation Option | Default |
| Allocation | Dynamic |

**步骤5：配置DebugTask**
| 配置项 | 值 |
|--------|-----|
| Task Name | DebugTask |
| Priority | osPriorityBelowNormal |
| Stack Size (Words) | 256 |
| Entry Function | StartDebugTask |
| Code Generation Option | Default |
| Allocation | Dynamic |

### 5.4 创建队列

**步骤1：打开队列配置**
- 在 **Tasks and Queues** 标签页中选择 **Queues** 选项卡
- 点击 **Add** 按钮添加队列

**步骤2：配置VisionQueue**
| 配置项 | 值 |
|--------|-----|
| Queue Name | VisionQueue |
| Queue Size | 5 |
| Item Size | 8（直接输入数字8） |
| Allocation | Dynamic |
| Buffer Name | NULL |
| Control Block Name | NULL |

**步骤3：配置CommandQueue**
| 配置项 | 值 |
|--------|-----|
| Queue Name | CommandQueue |
| Queue Size | 3 |
| Item Size | 8（直接输入数字8） |
| Allocation | Dynamic |
| Buffer Name | NULL |
| Control Block Name | NULL |

### 5.5 创建信号量

**步骤1：打开信号量配置**
- 进入 **Timers and Semaphores** 标签页
- 选择 **Semaphores** 选项卡
- 点击 **Add** 按钮添加信号量

**步骤2：配置VisionReadySem**
| 配置项 | 值 |
|--------|-----|
| Semaphore Name | VisionReadySem |
| Type | Binary |

**步骤3：配置MotionDoneSem**
| 配置项 | 值 |
|--------|-----|
| Semaphore Name | MotionDoneSem |
| Type | Binary |

### 5.6 数据结构体定义

在代码中需要定义以下结构体（放在合适的头文件中）：

```c
/* 视觉数据结构体 */
typedef struct {
    int16_t x;      /*!< 目标X坐标 */
    int16_t y;      /*!< 目标Y坐标 */
    int16_t speed;  /*!< 目标速度 */
    uint8_t valid;  /*!< 数据有效性 */
} VisionData_t;

/* 运动指令结构体 */
typedef struct {
    int16_t vx;     /*!< X方向速度 */
    int16_t vy;     /*!< Y方向速度 */
    int16_t omega;  /*!< 角速度 */
    uint8_t mode;   /*!< 运动模式 */
} MotionCmd_t;
```

---

## 六、编码器配置（使用定时器编码器模式）

### 6.1 编码器1配置（TIM1）

**步骤1：打开TIM1配置**
1. 在CubeMX主界面左侧选择 **Timers** → **TIM1**
2. **Clock Source** 选择 **Internal Clock**
3. **Mode** 选择 **Encoder Mode**

**步骤2：配置编码器模式参数**
1. 进入 **Parameter Settings** 标签页
2. **Encoder Mode**: 选择 **Encoder Mode TI1 and TI2**
3. **Counter Mode**: 选择 **Up**
4. **Prescaler**: 0
5. **Counter Period**: 65535

**步骤3：配置编码器引脚**
1. **Channel1** 下拉菜单选择 **Encoder Mode TI1**
2. **Channel2** 下拉菜单选择 **Encoder Mode TI2**
3. 回到主界面，设置引脚标签：
   - **PE9** → ENCODER_A1
   - **PE11** → ENCODER_B1

### 6.2 编码器2配置（TIM2）

**步骤1：打开TIM2配置**
1. 在CubeMX主界面左侧选择 **Timers** → **TIM2**
2. **Clock Source** 选择 **Internal Clock**
3. **Mode** 选择 **Encoder Mode**

**步骤2：配置编码器模式参数**
1. 进入 **Parameter Settings** 标签页
2. **Encoder Mode**: 选择 **Encoder Mode TI1 and TI2**
3. **Counter Mode**: 选择 **Up**
4. **Prescaler**: 0
5. **Counter Period**: 65535

**步骤3：配置编码器引脚**
1. **Channel1** 下拉菜单选择 **Encoder Mode TI1**
2. **Channel2** 下拉菜单选择 **Encoder Mode TI2**
3. 回到主界面，设置引脚标签：
   - **PA0** → ENCODER_A2
   - **PA1** → ENCODER_B2

### 6.3 编码器3配置（TIM3）

**步骤1：打开TIM3配置**
1. 在CubeMX主界面左侧选择 **Timers** → **TIM3**
2. **Clock Source** 选择 **Internal Clock**
3. **Mode** 选择 **Encoder Mode**

**步骤2：配置编码器模式参数**
1. 进入 **Parameter Settings** 标签页
2. **Encoder Mode**: 选择 **Encoder Mode TI1 and TI2**
3. **Counter Mode**: 选择 **Up**
4. **Prescaler**: 0
5. **Counter Period**: 65535

**步骤3：配置编码器引脚**
1. **Channel1** 下拉菜单选择 **Encoder Mode TI1**
2. **Channel2** 下拉菜单选择 **Encoder Mode TI2**
3. 回到主界面，设置引脚标签：
   - **PA6** → ENCODER_A3
   - **PA7** → ENCODER_B3

### 6.4 编码器4配置（TIM4）

**步骤1：打开TIM4配置**
1. 在CubeMX主界面左侧选择 **Timers** → **TIM4**
2. **Clock Source** 选择 **Internal Clock**
3. **Mode** 选择 **Encoder Mode**

**步骤2：配置编码器模式参数**
1. 进入 **Parameter Settings** 标签页
2. **Encoder Mode**: 选择 **Encoder Mode TI1 and TI2**
3. **Counter Mode**: 选择 **Up**
4. **Prescaler**: 0
5. **Counter Period**: 65535

**步骤3：配置编码器引脚**
1. **Channel1** 下拉菜单选择 **Encoder Mode TI1**
2. **Channel2** 下拉菜单选择 **Encoder Mode TI2**
3. 回到主界面，设置引脚标签：
   - **PB6** → ENCODER_A4
   - **PB7** → ENCODER_B4

### 6.5 编码器参数表

| 编码器 | 定时器 | A相引脚 | B相引脚 | 模式 |
|--------|--------|---------|---------|------|
| 编码器1 | TIM1 | PE9 | PE11 | Encoder Mode |
| 编码器2 | TIM2 | PA0 | PA1 | Encoder Mode |
| 编码器3 | TIM3 | PA6 | PA7 | Encoder Mode |
| 编码器4 | TIM4 | PB6 | PB7 | Encoder Mode |

### 6.6 注意事项

1. **中断配置**：如果需要使用编码器中断，在对应定时器的NVIC配置中开启中断
2. **计数方向**：可以通过交换A相和B相的连接来改变计数方向
3. **计数范围**：使用16位计数器，最大值为65535，需要注意溢出处理
4. **PWM使用**：TIM8继续用于PWM输出，控制电机速度

## 七、中断配置

### 7.1 编码器中断配置

**步骤1：打开NVIC配置页面**
1. 在CubeMX主界面左侧选择 **System Core** → **NVIC**
2. 进入 **NVIC Configuration** 标签页

**步骤2：启用编码器定时器中断**
1. 在 **NVIC Interrupt Table** 中找到以下中断并勾选 **Enabled** 复选框：
   - **TIM1 update interrupt and TIM9 global interrupt**（编码器1）
   - **TIM2 global interrupt**（编码器2）
   - **TIM3 global interrupt**（编码器3）
   - **TIM4 global interrupt**（编码器4）

**步骤3：设置中断优先级**
1. 对于上述四个中断，设置：
   - **Preemption Priority**：5
   - **Sub Priority**：0
2. 确保 **Uses FreeRTOS functions** 列被勾选

### 7.2 串口中断配置

**步骤1：启用串口中断**
1. 在 **NVIC Interrupt Table** 中找到以下中断并勾选 **Enabled** 复选框：
   - **USART2 global interrupt**
   - **USART3 global interrupt**

**步骤2：设置串口中断优先级**
1. 对于上述两个中断，设置：
   - **Preemption Priority**：5
   - **Sub Priority**：0
2. 确保 **Uses FreeRTOS functions** 列被勾选

### 7.3 中断配置参数表

| 中断名称 | 使能 | 抢占优先级 | 子优先级 | 用途 |
|----------|------|------------|----------|------|
| TIM1 global interrupt | ✓ | 5 | 0 | 编码器1中断 |
| TIM2 global interrupt | ✓ | 5 | 0 | 编码器2中断 |
| TIM3 global interrupt | ✓ | 5 | 0 | 编码器3中断 |
| TIM4 global interrupt | ✓ | 5 | 0 | 编码器4中断 |
| USART2 global interrupt | ✓ | 5 | 0 | 串口2中断（摄像头） |
| USART3 global interrupt | ✓ | 5 | 0 | 串口3中断（调试） |

### 7.4 注意事项

1. **中断处理**：中断处理函数应尽量简短，避免在中断中执行耗时操作
2. **优先级设置**：编码器和串口中断优先级相同，避免优先级反转
3. **FreeRTOS兼容**：确保所有中断都勾选了 **Uses FreeRTOS functions**，以支持FreeRTOS的中断管理
4. **中断向量**：CubeMX会自动生成中断处理函数的框架，需要在用户代码区域添加具体实现

## 八、生成代码配置

### 8.1 Project Manager 设置
| 参数 | 值 |
|------|-----|
| Project Name | Trash_can |
| Project Location | 你的工程路径 |
| Toolchain / IDE | STM32CubeIDE |
| Application Structure | Basic |

### 8.2 Code Generator 设置
- **Generate peripheral initialization as a pair of '.c/.h' files**: 勾选
- **Keep User Code when re-generating**: 勾选（重要！）
- **Delete previously generated files when not re-generated**: 不勾选

---

## 九、用户代码区域说明

CubeMX生成的代码中，以下注释标记的区域为用户代码保护区，重新生成代码时不会被覆盖：

```c
/* USER CODE BEGIN xxx */
// 你的代码写在这里
/* USER CODE END xxx */
```

### 常用用户代码区域
| 区域 | 位置 | 用途 |
|------|------|------|
| Includes | main.c | 添加头文件包含 |
| PTD | main.c | 私有类型定义 |
| PD | main.c | 私有宏定义 |
| PV | main.c | 私有变量定义 |
| PFP | main.c | 私有函数声明 |
| 0 | main.c | 用户代码区（初始化前） |
| 1 | main.c | 用户代码区（初始化后） |
| WHILE | main.c | 主循环代码 |
| 3 | main.c | 主循环后代码 |

---

## 十、重新生成代码注意事项

1. **永远不要**修改 `/* USER CODE BEGIN xxx */` 和 `/* USER CODE END xxx */` 之外的代码
2. **BSP层代码**放在 `Drivers/BSP/` 目录下，不会被CubeMX覆盖
3. **重新生成前**确保已保存所有修改
4. **生成后**检查用户代码区域是否完整

---

## 十一、调试建议

1. 先测试 **LED闪烁** 验证基础配置
2. 再测试 **PWM输出** 验证电机驱动
3. 然后测试 **串口通信** 验证数据收发
4. 最后集成 **FreeRTOS** 进行多任务调试

## 十二、ADC配置

### 12.1 ADC1配置

**步骤1：打开ADC配置**
1. 在CubeMX主界面左侧选择 **Analog** → **ADC1**

**步骤2：配置ADC通道**
1. 在 **ADC1 Mode and Configuration** 页面中，找到 **Mode** 部分
2. 勾选 **IN11** 通道（对应PC1引脚）

**步骤3：配置ADC参数**
1. 进入 **Parameter Settings** 标签页
2. **Resolution**：选择 **12 Bits**
3. **Data Alignment**：选择 **Right alignment**
4. **Scan Conversion Mode**：选择 **Disabled**
5. **Continuous Conversion Mode**：选择 **Disabled**
6. **Discontinuous Conversion Mode**：选择 **Disabled**
7. **Number Of Conversion**：设置为 **1**
8. **External Trigger Conversion Source**：选择 **Regular Conversion launched by software**
9. **External Trigger Conversion Edge**：选择 **None**

**步骤4：配置通道采样时间**
1. 在 **Rank** 部分，找到 **Channel 11**
2. **Sampling Time** 选择 **3 Cycles**（或根据实际需求调整）

**步骤5：配置注入通道（可选）**
1. 在 **ADC_Injected_ConversionMode** 部分
2. **Number Of Conversions**：设置为 **1**
3. **External Trigger Source**：选择 **Injected Conversion launched by software**
4. **External Trigger Edge**：选择 **None**
5. **Injected Conversion Mode**：选择 **None**
6. 在 **Injected Rank** 部分，找到 **Channel 11**
7. **Sampling Time** 选择 **15 Cycles**（或根据实际需求调整）
8. **Injected Offset**：设置为 **0**

**步骤6：配置看门狗（可选）**
1. 在 **WatchDog** 部分
2. **Enable Analog WatchDog Mode**：保持未勾选状态

**步骤7：配置引脚**
1. 回到主界面，找到 **PC1** 引脚
2. 在右侧配置面板中：
   - **GPIO mode**：选择 **Analog**
   - **User Label**：设置为 **ADC1_IN11**

### 12.2 ADC配置参数表

| 引脚 | 模式 | 标签 | 说明 |
|------|------|------|------|
| PC1 | Analog | ADC1_IN11 | ADC输入通道11 |

### 12.3 注意事项

1. **采样时间**：根据实际应用需求调整采样时间，采样时间越长，精度越高，但转换速度越慢
2. **参考电压**：默认使用VDD作为参考电压（3.3V）
3. **校准**：ADC启动前建议进行校准，以提高转换精度
4. **DMA配置**：如果需要连续采样，可以配置DMA传输，减少CPU开销
