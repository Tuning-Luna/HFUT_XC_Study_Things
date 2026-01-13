# 单周期 MIPS32 CPU - Vivado 工程建立与模块设计说明

## 📋 目录

1. [Vivado 工程建立步骤](#vivado-工程建立步骤)
2. [模块功能划分与引脚定义](#模块功能划分与引脚定义)
3. [代码功能描述](#代码功能描述)
4. [设计流程与代码编写思路](#设计流程与代码编写思路)
5. [文件依赖关系](#文件依赖关系)
6. [预期结果与测试方法](#预期结果与测试方法)
7. [测试代码执行位置](#测试代码执行位置)

---

## 1. Vivado 工程建立步骤

### 1.1 创建新工程

1. **启动 Vivado**

   - 打开 Vivado Design Suite
   - 选择 "Create Project" 创建新工程

2. **工程设置**

   - **Project Name**: `single_cycle_cpu`
   - **Project Location**: 选择项目目录
   - **Project Type**: 选择 "RTL Project"
   - **Do not specify sources at this time**: 取消勾选（我们需要添加源文件）

3. **添加源文件**

   - 点击 "Add Sources" → "Add or create design sources"
   - 添加以下 **8 个 Verilog 文件**（**不要添加 testbench.v**）：
     ```
     pc_register.v          # PC 寄存器模块
     instruction_memory.v   # 指令存储器模块
     control_unit.v         # 控制单元模块
     register_file.v        # 寄存器文件模块
     alu_control.v         # ALU 控制单元模块
     alu.v                 # 算术逻辑单元模块
     data_memory.v         # 数据存储器模块
     cpu_top.v             # CPU 顶层模块（设置为顶层）
     ```

4. **设置顶层模块**

   - 在 Sources 窗口中，右键点击 `cpu_top.v`
   - 选择 "Set as Top"

5. **添加约束文件（可选，用于 FPGA 下载）**
   - 点击 "Add Sources" → "Add or create constraints"
   - 创建或添加 `.xdc` 约束文件
   - 定义时钟频率、引脚分配等

### 1.2 工程配置

1. **综合设置**

   - 在 Flow Navigator 中点击 "Run Synthesis"
   - 等待综合完成

2. **实现设置**

   - 综合完成后，点击 "Run Implementation"
   - 等待实现完成

3. **生成比特流（可选）**
   - 实现完成后，点击 "Generate Bitstream"
   - 用于下载到 FPGA 开发板

### 1.3 注意事项

⚠️ **重要提示**：

- ❌ **不要**将 `testbench.v` 添加到 Vivado 工程中（仅用于仿真）
- ✅ **必须**将 `cpu_top.v` 设置为顶层模块
- ✅ 所有 `.v` 文件必须在同一目录下
- ✅ 确保所有模块文件名与代码中的模块名一致

---

## 2. 模块功能划分与引脚定义

### 2.1 模块层次结构

```
cpu_top (顶层模块)
├── pc_register          # PC 寄存器
├── instruction_memory   # 指令存储器
├── control_unit         # 控制单元
├── register_file        # 寄存器文件
├── alu_control          # ALU 控制单元
├── alu                  # 算术逻辑单元
└── data_memory          # 数据存储器
```

### 2.2 各模块详细说明

#### 2.2.1 `cpu_top.v` - CPU 顶层模块

**功能**：整合所有子模块，实现单周期 CPU 的完整数据通路

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `clk` | input | 1 | 时钟信号 |
| `rst` | input | 1 | 复位信号（高电平有效） |
| `pc_out` | output | 32 | 当前 PC 值（用于调试） |
| `alu_result` | output | 32 | ALU 运算结果（用于调试） |

**关键功能**：

- 连接所有子模块
- 实现数据通路（指令获取、译码、执行、访存、写回）
- 处理分支和跳转逻辑
- 实现多路选择器（MUX）功能

---

#### 2.2.2 `pc_register.v` - PC 寄存器模块

**功能**：存储程序计数器（PC），每个时钟周期更新

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `clk` | input | 1 | 时钟信号 |
| `rst` | input | 1 | 复位信号 |
| `pc_next` | input | 32 | 下一个 PC 值 |
| `pc_current` | output | 32 | 当前 PC 值 |

**关键功能**：

- 在时钟上升沿更新 PC
- 复位时 PC 归零
- 支持顺序执行、分支和跳转

---

#### 2.2.3 `instruction_memory.v` - 指令存储器模块

**功能**：存储程序指令，根据 PC 地址读取指令

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `addr` | input | 32 | 指令地址（PC 值） |
| `instruction` | output | 32 | 32 位指令 |

**关键功能**：

- 128 条指令容量（512 字节）
- 预装载斐波那契数列计算程序
- 字对齐访问（忽略地址低 2 位）

---

#### 2.2.4 `control_unit.v` - 控制单元模块

**功能**：根据指令操作码生成所有控制信号

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `opcode` | input | 6 | 指令操作码 |
| `reg_dst` | output | 1 | 寄存器目标选择（1:rd, 0:rt） |
| `jump` | output | 1 | 跳转信号 |
| `branch` | output | 1 | 分支信号 |
| `mem_read` | output | 1 | 内存读信号 |
| `mem_to_reg` | output | 1 | 数据源选择（1:内存, 0:ALU） |
| `alu_op` | output | 2 | ALU 操作类型 |
| `mem_write` | output | 1 | 内存写信号 |
| `alu_src` | output | 1 | ALU 源选择（1:立即数, 0:寄存器） |
| `reg_write` | output | 1 | 寄存器写使能 |

**支持的指令类型**：

- R-type: `add`, `sub`, `and`, `or`, `slt`
- I-type: `lw`, `sw`, `beq`, `addi`
- J-type: `j`

---

#### 2.2.5 `register_file.v` - 寄存器文件模块

**功能**：32 个 32 位通用寄存器，支持 2 读 1 写

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `clk` | input | 1 | 时钟信号 |
| `rst` | input | 1 | 复位信号 |
| `reg_write` | input | 1 | 写使能 |
| `read_reg1` | input | 5 | 读寄存器 1 编号 |
| `read_reg2` | input | 5 | 读寄存器 2 编号 |
| `write_reg` | input | 5 | 写寄存器编号 |
| `write_data` | input | 32 | 写入数据 |
| `read_data1` | output | 32 | 读出数据 1 |
| `read_data2` | output | 32 | 读出数据 2 |

**关键功能**：

- 32 个 32 位寄存器（$0-$31）
- $0 寄存器恒为 0（硬件实现）
- 读操作是组合逻辑（立即输出）
- 写操作是时序逻辑（时钟上升沿）

---

#### 2.2.6 `alu_control.v` - ALU 控制单元模块

**功能**：根据 ALU_OP 和功能码生成 ALU 控制信号

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `alu_op` | input | 2 | ALU 操作类型（来自控制单元） |
| `funct` | input | 6 | R-type 指令功能码 |
| `alu_control` | output | 4 | ALU 控制信号 |

**ALU 操作类型映射**：

- `alu_op = 2'b00`: lw/sw/addi → ADD
- `alu_op = 2'b01`: beq → SUB（用于比较）
- `alu_op = 2'b10`: R-type → 根据 funct 决定

---

#### 2.2.7 `alu.v` - 算术逻辑单元模块

**功能**：执行算术和逻辑运算

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `input1` | input | 32 | 操作数 1 |
| `input2` | input | 32 | 操作数 2 |
| `alu_control` | input | 4 | ALU 控制信号 |
| `result` | output | 32 | 运算结果 |
| `zero` | output | 1 | 零标志（结果为 0 时为 1） |

**支持的运算**：

- `ADD` (4'b0010): 加法
- `SUB` (4'b0110): 减法
- `AND` (4'b0000): 按位与
- `OR` (4'b0001): 按位或
- `SLT` (4'b0111): 小于则置位
- `NOR` (4'b1100): 或非

---

#### 2.2.8 `data_memory.v` - 数据存储器模块

**功能**：存储和读取数据

**引脚定义**：
| 引脚名 | 方向 | 位宽 | 说明 |
|--------|------|------|------|
| `clk` | input | 1 | 时钟信号 |
| `mem_write` | input | 1 | 写使能 |
| `mem_read` | input | 1 | 读使能 |
| `addr` | input | 32 | 访问地址 |
| `write_data` | input | 32 | 写入数据 |
| `read_data` | output | 32 | 读出数据 |

**关键功能**：

- 256 个字容量（1KB）
- 字对齐访问（忽略地址低 2 位）
- 读操作是组合逻辑
- 写操作是时序逻辑（时钟上升沿）

---

## 3. 代码功能描述

### 3.1 单周期 CPU 数据通路

单周期 CPU 在一个时钟周期内完成一条指令的所有阶段：

```
┌─────────┐
│   PC    │ ──┐
└─────────┘   │
     │        │
     ▼        │
┌─────────────┴──┐
│ Instruction    │
│ Memory         │ ──► 指令
└────────────────┘
     │
     ▼
┌─────────────┐
│   Control   │ ──► 控制信号
│   Unit      │
└─────────────┘
     │
     ▼
┌─────────────┐     ┌─────────────┐
│  Register   │ ──► │     ALU     │
│   File      │     │             │
└─────────────┘     └─────────────┘
     │                    │
     │                    ▼
     │              ┌─────────────┐
     │              │    Data     │
     └─────────────►│   Memory    │
                    └─────────────┘
```

### 3.2 指令执行流程

#### R-type 指令（如 `add $t2, $t0, $t1`）

1. **取指（IF）**：从指令存储器读取指令
2. **译码（ID）**：
   - 控制单元识别为 R-type 指令
   - 从寄存器文件读取 `$t0` 和 `$t1` 的值
3. **执行（EX）**：
   - ALU 执行加法运算
   - 结果存储在 ALU 输出
4. **写回（WB）**：
   - 将 ALU 结果写回寄存器 `$t2`

#### I-type 指令（如 `lw $t0, 0($s0)`）

1. **取指（IF）**：读取指令
2. **译码（ID）**：
   - 控制单元识别为 `lw` 指令
   - 从寄存器文件读取 `$s0` 的值
3. **执行（EX）**：
   - ALU 计算地址：`$s0 + 0`
4. **访存（MEM）**：
   - 从数据存储器读取数据
5. **写回（WB）**：
   - 将内存数据写回寄存器 `$t0`

#### 分支指令（如 `beq $t0, $t1, label`）

1. **取指（IF）**：读取指令
2. **译码（ID）**：
   - 从寄存器文件读取 `$t0` 和 `$t1`
3. **执行（EX）**：
   - ALU 执行减法：`$t0 - $t1`
   - 如果结果为 0，`zero` 标志置 1
4. **分支判断**：
   - 如果 `branch & zero = 1`，PC 跳转到分支目标地址
   - 否则 PC = PC + 4

### 3.3 关键代码片段说明

#### 3.3.1 指令字段解析（cpu_top.v）

```verilog
// 从 32 位指令中提取各个字段
assign opcode = instruction[31:26];    // 操作码（6 位）
assign rs = instruction[25:21];       // 源寄存器 1（5 位）
assign rt = instruction[20:16];       // 源寄存器 2（5 位）
assign rd = instruction[15:11];       // 目标寄存器（5 位）
assign funct = instruction[5:0];       // 功能码（6 位）
assign immediate = instruction[15:0]; // 立即数（16 位）
```

#### 3.3.2 符号扩展（cpu_top.v）

```verilog
// 将 16 位立即数符号扩展为 32 位
assign sign_extended = {{16{immediate[15]}}, immediate};
// {16{immediate[15]}} 表示将最高位重复 16 次
```

#### 3.3.3 多路选择器（MUX）实现

```verilog
// ALU 第二个输入选择（立即数或寄存器）
assign alu_input2 = alu_src ? sign_extended : read_data2;

// 写入寄存器选择（rt 或 rd）
assign write_reg = reg_dst ? rd : rt;

// 写入数据选择（ALU 结果或内存数据）
assign write_data = mem_to_reg ? mem_data : alu_out;

// PC 选择（顺序、分支或跳转）
assign pc_next = jump ? {pc_plus4[31:28], jump_addr, 2'b00} :
                 (pc_src ? pc_branch : pc_plus4);
```

#### 3.3.4 控制信号生成（control_unit.v）

```verilog
case (opcode)
    OP_RTYPE: begin  // R-type 指令
        reg_dst = 1;      // 目标寄存器是 rd
        alu_op = 2'b10;   // ALU 操作由功能码决定
        reg_write = 1;    // 需要写寄存器
    end

    OP_LW: begin     // lw 指令
        alu_src = 1;      // ALU 第二个输入是立即数
        mem_to_reg = 1;   // 写入数据来自内存
        reg_write = 1;    // 需要写寄存器
        mem_read = 1;     // 需要读内存
        alu_op = 2'b00;   // ALU 执行加法（计算地址）
    end
    // ... 其他指令类型
endcase
```

---

## 4. 设计流程与代码编写思路

### 4.1 设计流程

#### 阶段 1：需求分析

1. **确定指令集**：选择要支持的 MIPS32 指令

   - R-type: `add`, `sub`, `and`, `or`, `slt`
   - I-type: `lw`, `sw`, `beq`, `addi`
   - J-type: `j`

2. **确定 CPU 架构**：单周期 CPU
   - 一个时钟周期完成一条指令的所有阶段

#### 阶段 2：模块划分

按照功能将 CPU 划分为以下模块：

```
1. PC 寄存器        → 存储程序计数器
2. 指令存储器       → 存储程序
3. 控制单元         → 生成控制信号
4. 寄存器文件       → 存储通用寄存器
5. ALU 控制单元     → 生成 ALU 控制信号
6. ALU              → 执行运算
7. 数据存储器       → 存储数据
8. CPU 顶层         → 连接所有模块
```

#### 阶段 3：数据通路设计

设计指令执行的数据通路：

1. **取指阶段**：

   - PC → 指令存储器 → 指令

2. **译码阶段**：

   - 指令字段解析
   - 控制单元生成控制信号
   - 寄存器文件读取数据

3. **执行阶段**：

   - ALU 执行运算
   - 计算分支目标地址

4. **访存阶段**（仅 lw/sw）：

   - 数据存储器读写

5. **写回阶段**：
   - 将结果写回寄存器

#### 阶段 4：代码实现

按照模块逐个实现：

1. **基础模块**（无依赖）：

   - `pc_register.v`
   - `instruction_memory.v`
   - `control_unit.v`
   - `register_file.v`
   - `alu_control.v`
   - `alu.v`
   - `data_memory.v`

2. **顶层模块**（依赖所有基础模块）：

   - `cpu_top.v`：连接所有模块，实现数据通路

3. **测试模块**（用于仿真）：
   - `testbench.v`：生成时钟、复位信号，验证功能

### 4.2 代码编写思路

#### 4.2.1 模块化设计

每个模块独立实现，通过接口（引脚）连接：

```verilog
// 模块定义
module module_name(
    input wire signal1,
    output wire signal2
);
    // 内部实现
endmodule

// 模块实例化
module_name instance_name(
    .signal1(input_signal),
    .signal2(output_signal)
);
```

#### 4.2.2 组合逻辑 vs 时序逻辑

**组合逻辑**（无时钟）：

- 用于立即响应的操作
- 例如：指令字段解析、ALU 运算、多路选择器

```verilog
// 组合逻辑：使用 assign 或 always @(*)
assign result = input1 + input2;
```

**时序逻辑**（有时钟）：

- 用于需要同步的操作
- 例如：PC 更新、寄存器写入、内存写入

```verilog
// 时序逻辑：使用 always @(posedge clk)
always @(posedge clk) begin
    if (enable)
        register <= new_value;
end
```

#### 4.2.3 控制信号设计

控制信号决定数据通路的流向：

1. **主控制单元**（control_unit）：

   - 根据 `opcode` 生成主要控制信号
   - 例如：`reg_write`, `mem_read`, `mem_write`, `branch`, `jump`

2. **ALU 控制单元**（alu_control）：
   - 根据 `alu_op` 和 `funct` 生成 ALU 控制信号
   - 决定 ALU 执行哪种运算

#### 4.2.4 数据通路连接

在顶层模块中连接所有模块：

```verilog
// 1. 实例化所有子模块
pc_register u_pc(...);
instruction_memory u_imem(...);
control_unit u_control(...);
// ...

// 2. 连接数据通路
assign pc_plus4 = pc_current + 4;
assign alu_input2 = alu_src ? sign_extended : read_data2;
assign write_reg = reg_dst ? rd : rt;
// ...

// 3. 处理分支和跳转
assign pc_src = branch & alu_zero;
assign pc_next = jump ? jump_address : (pc_src ? pc_branch : pc_plus4);
```

---

## 5. 文件依赖关系

### 5.1 文件依赖图

```
testbench.v (测试平台)
    │
    └──► cpu_top.v (顶层模块)
            │
            ├──► pc_register.v
            ├──► instruction_memory.v
            ├──► control_unit.v
            ├──► register_file.v
            ├──► alu_control.v
            │       │
            │       └──► (依赖 control_unit 的 alu_op 输出)
            ├──► alu.v
            │       │
            │       └──► (依赖 alu_control 的 alu_control 输出)
            └──► data_memory.v
```

### 5.2 详细依赖关系说明

#### 5.2.1 `testbench.v` → `cpu_top.v`

**依赖关系**：

- `testbench.v` 实例化 `cpu_top` 模块
- 提供时钟和复位信号
- 监控 CPU 的输出信号

**代码位置**：

```verilog
// testbench.v 第 20-25 行
cpu_top uut (
    .clk(clk),
    .rst(rst),
    .pc_out(pc_out),
    .alu_result(alu_result)
);
```

#### 5.2.2 `cpu_top.v` → 所有子模块

**依赖关系**：

- `cpu_top.v` 实例化并连接所有 7 个子模块
- 实现数据通路和控制逻辑

**代码位置**：

```verilog
// cpu_top.v 第 88-152 行
pc_register u_pc(...);
instruction_memory u_imem(...);
control_unit u_control(...);
register_file u_regfile(...);
alu_control u_alu_control(...);
alu u_alu(...);
data_memory u_dmem(...);
```

#### 5.2.3 `control_unit.v` → `alu_control.v`

**依赖关系**：

- `control_unit` 输出 `alu_op` 信号
- `alu_control` 接收 `alu_op` 和 `funct`，生成 `alu_control` 信号

**数据流**：

```
control_unit.alu_op → alu_control.alu_op
instruction.funct → alu_control.funct
alu_control.alu_control → alu.alu_control
```

#### 5.2.4 `alu_control.v` → `alu.v`

**依赖关系**：

- `alu_control` 输出 `alu_control` 信号
- `alu` 接收该信号，执行相应的运算

**数据流**：

```
alu_control.alu_control → alu.alu_control
register_file.read_data1 → alu.input1
(register_file.read_data2 或 sign_extended) → alu.input2
alu.result → (写回寄存器 或 数据存储器地址)
```

### 5.3 编译顺序

**正确的编译顺序**（iverilog 会自动处理）：

1. **基础模块**（无依赖）：

   - `pc_register.v`
   - `instruction_memory.v`
   - `control_unit.v`
   - `register_file.v`
   - `alu_control.v`
   - `alu.v`
   - `data_memory.v`

2. **顶层模块**：

   - `cpu_top.v`（依赖所有基础模块）

3. **测试模块**：
   - `testbench.v`（依赖 `cpu_top.v`）

**注意**：在 `run.bat` 中，文件顺序不重要，iverilog 会自动解析依赖关系。

---

## 6. 预期结果与测试方法

### 6.1 测试程序：斐波那契数列

**程序功能**：计算前 10 个斐波那契数并存储到内存

**预期结果**：

```
fib[0] = 0   (存储在地址 0x100)
fib[1] = 1   (存储在地址 0x104)
fib[2] = 1   (存储在地址 0x108)
fib[3] = 2   (存储在地址 0x10C)
fib[4] = 3   (存储在地址 0x110)
fib[5] = 5   (存储在地址 0x114)
fib[6] = 8   (存储在地址 0x118)
fib[7] = 13  (存储在地址 0x11C)
fib[8] = 21  (存储在地址 0x120)
fib[9] = 34  (存储在地址 0x124)
```

### 6.2 测试方法

#### 方法 1：使用 iverilog + gtkwave（推荐）

1. **运行仿真**：

   ```bash
   # Windows
   run.bat

   # Linux/Mac
   ./run.sh
   ```

2. **查看控制台输出**：

   - 仿真完成后，控制台会显示内存中的斐波那契数列
   - 如果结果正确，会显示 "✓ Test passed!"

3. **查看波形文件**：

   ```bash
   gtkwave cpu_test.vcd
   ```

   在 GTKWave 中观察：

   - PC 值的变化
   - 寄存器文件中的值（$t0-$t5, $s0）
   - 数据存储器中的值（地址 0x100-0x124）
   - 控制信号的变化

#### 方法 2：使用 Vivado 仿真

1. **添加测试平台**：

   - 在 Vivado 中添加 `testbench.v` 作为仿真源文件
   - 设置 `testbench` 为顶层模块（仅用于仿真）

2. **运行仿真**：

   - 在 Flow Navigator 中选择 "Run Simulation" → "Run Behavioral Simulation"
   - 等待仿真完成

3. **查看波形**：
   - 在 Vivado 波形窗口中添加信号
   - 观察信号变化

### 6.3 验证检查清单

#### 6.3.1 功能验证

- [ ] PC 从 0x00000000 开始，逐步递增
- [ ] 指令正确从指令存储器读取
- [ ] 控制信号根据指令类型正确生成
- [ ] 寄存器文件正确读写
- [ ] ALU 正确执行运算
- [ ] 数据存储器正确存储斐波那契数列
- [ ] 分支指令正确跳转
- [ ] 跳转指令正确执行

#### 6.3.2 数据验证

- [ ] `$t0` 寄存器：初始为 0，后续更新
- [ ] `$t1` 寄存器：初始为 1，后续更新
- [ ] `$t2` 寄存器：存储当前计算的斐波那契数
- [ ] `$t3` 寄存器：循环计数器，从 2 递增到 10
- [ ] `$s0` 寄存器：数据存储基地址 0x100
- [ ] 内存地址 0x100-0x124：存储 10 个斐波那契数

#### 6.3.3 时序验证

- [ ] 每个时钟周期执行一条指令
- [ ] 复位后 PC 归零
- [ ] 寄存器写入在时钟上升沿完成
- [ ] 内存写入在时钟上升沿完成

---

## 7. 测试代码执行位置

### 7.1 测试代码在哪里？

#### 7.1.1 测试程序位置

**测试程序存储在 `instruction_memory.v` 中**：

```verilog
// instruction_memory.v 第 25-106 行
initial begin
    // 初始化所有指令为 NOP
    for (i = 0; i < 128; i = i + 1) begin
        mem[i] = 32'h00000000;
    end

    // 预装载斐波那契数列程序
    mem[0] = 32'h20080000;  // addi $t0, $0, 0
    mem[1] = 32'h20090001;  // addi $t1, $0, 1
    // ... 更多指令
end
```

**说明**：

- 测试程序在 `instruction_memory` 模块的 `initial` 块中
- 程序在仿真开始时自动加载到指令存储器
- 程序计算斐波那契数列并存储到数据存储器

#### 7.1.2 测试平台位置

**测试平台在 `testbench.v` 中**：

```verilog
// testbench.v 第 11-116 行
module testbench;
    // 生成时钟和复位信号
    // 实例化 CPU
    // 生成波形文件
    // 验证结果
endmodule
```

**功能**：

- 生成时钟信号（100MHz，周期 10ns）
- 生成复位信号
- 实例化 CPU 顶层模块
- 生成波形文件（`cpu_test.vcd`）
- 在仿真结束时显示结果并验证正确性

### 7.2 测试代码执行流程

#### 7.2.1 仿真启动流程

```
1. 运行 run.bat
   │
   ├─► iverilog 编译所有 .v 文件
   │   ├─► 编译基础模块
   │   ├─► 编译 cpu_top.v
   │   └─► 编译 testbench.v
   │
   ├─► 生成 cpu_sim.vvp（可执行文件）
   │
   └─► vvp cpu_sim.vvp（运行仿真）
       │
       ├─► testbench 开始执行
       │   ├─► 初始化时钟和复位
       │   └─► 实例化 cpu_top
       │
       ├─► instruction_memory 的 initial 块执行
       │   └─► 加载测试程序到指令存储器
       │
       ├─► data_memory 的 initial 块执行
       │   └─► 初始化数据存储器为 0
       │
       ├─► CPU 开始执行程序
       │   ├─► PC = 0x00000000
       │   ├─► 读取第一条指令
       │   ├─► 执行指令
       │   └─► 更新 PC，继续下一条指令
       │
       └─► 1000ns 后（100 个时钟周期）
           ├─► 显示内存中的斐波那契数列
           ├─► 验证结果正确性
           └─► 生成波形文件 cpu_test.vcd
```

#### 7.2.2 指令执行流程

```
时钟周期 1：
  PC = 0x00000000
  ├─► 读取指令：addi $t0, $0, 0
  ├─► 控制单元识别为 addi 指令
  ├─► 寄存器文件读取 $0（值为 0）
  ├─► ALU 执行：0 + 0 = 0
  └─► 写回：$t0 = 0

时钟周期 2：
  PC = 0x00000004
  ├─► 读取指令：addi $t1, $0, 1
  ├─► 控制单元识别为 addi 指令
  ├─► 寄存器文件读取 $0（值为 0）
  ├─► ALU 执行：0 + 1 = 1
  └─► 写回：$t1 = 1

时钟周期 3：
  PC = 0x00000008
  ├─► 读取指令：addi $t3, $0, 2
  └─► ...（继续执行）

...（循环执行，计算斐波那契数列）

时钟周期 N：
  PC = 0x00000050
  ├─► 读取指令：j 0x50（无限循环）
  └─► 程序结束
```

### 7.3 如何查看预期结果？

#### 7.3.1 控制台输出

运行 `run.bat` 后，控制台会显示：

```
========================================
Fibonacci calculation finished.
========================================
Fibonacci numbers stored in memory:
fib[0] = 0 (address 0x100)
fib[1] = 1 (address 0x104)
fib[2] = 1 (address 0x108)
fib[3] = 2 (address 0x10C)
fib[4] = 3 (address 0x110)
fib[5] = 5 (address 0x114)
fib[6] = 8 (address 0x118)
fib[7] = 13 (address 0x11C)
fib[8] = 21 (address 0x120)
fib[9] = 34 (address 0x124)
========================================
Final PC value: 0x00000050
========================================
✓ Test passed! Fibonacci calculation is correct.
========================================
```

#### 7.3.2 波形文件

使用 GTKWave 打开 `cpu_test.vcd`：

1. **添加信号到波形窗口**：

   - `testbench.clk`：时钟信号
   - `testbench.rst`：复位信号
   - `testbench.pc_out`：PC 值
   - `testbench.uut.instruction`：当前指令
   - `testbench.uut.u_regfile.registers[8]`：$t0
   - `testbench.uut.u_regfile.registers[9]`：$t1
   - `testbench.uut.u_regfile.registers[10]`：$t2
   - `testbench.uut.u_dmem.mem[64]`：fib[0]（地址 0x100）
   - `testbench.uut.u_dmem.mem[65]`：fib[1]（地址 0x104）
   - ...（更多信号）

2. **观察信号变化**：
   - PC 从 0x00000000 开始递增
   - 寄存器值按预期变化
   - 内存中的斐波那契数列逐步计算并存储

#### 7.3.3 验证代码位置

**验证代码在 `testbench.v` 第 90-104 行**：

```verilog
// 验证斐波那契数列正确性
if (uut.u_dmem.mem[64] == 0 &&
    uut.u_dmem.mem[65] == 1 &&
    uut.u_dmem.mem[66] == 1 &&
    uut.u_dmem.mem[67] == 2 &&
    uut.u_dmem.mem[68] == 3 &&
    uut.u_dmem.mem[69] == 5 &&
    uut.u_dmem.mem[70] == 8 &&
    uut.u_dmem.mem[71] == 13 &&
    uut.u_dmem.mem[72] == 21 &&
    uut.u_dmem.mem[73] == 34) begin
    $display("✓ Test passed! Fibonacci calculation is correct.");
end else begin
    $display("✗ Test failed! The result is incorrect.");
end
```

---

## 8. 总结

### 8.1 项目结构总结

```
项目根目录/
├── 设计文件（8个）
│   ├── cpu_top.v              # 顶层模块（Vivado 顶层）
│   ├── pc_register.v          # PC 寄存器
│   ├── instruction_memory.v   # 指令存储器（包含测试程序）
│   ├── control_unit.v         # 控制单元
│   ├── register_file.v        # 寄存器文件
│   ├── alu_control.v          # ALU 控制单元
│   ├── alu.v                  # ALU
│   └── data_memory.v          # 数据存储器
│
├── 测试文件（1个）
│   └── testbench.v            # 测试平台（仅用于仿真）
│
├── 脚本文件（1个）
│   └── run.bat                # 运行脚本
│
└── 文档文件（多个）
    ├── README.md
    ├── CPU设计说明文档.md
    ├── 斐波那契数列运行指标说明.md
    └── Vivado工程建立与模块设计说明.md（本文档）
```

### 8.2 关键要点

1. **Vivado 工程**：

   - 添加 8 个设计文件（不包括 testbench.v）
   - 设置 `cpu_top.v` 为顶层模块

2. **模块依赖**：

   - `cpu_top.v` 依赖所有基础模块
   - `testbench.v` 依赖 `cpu_top.v`
   - `alu_control.v` 依赖 `control_unit.v` 的输出

3. **测试程序**：

   - 存储在 `instruction_memory.v` 的 `initial` 块中
   - 计算斐波那契数列并存储到数据存储器

4. **预期结果**：

   - 控制台输出显示计算结果
   - 波形文件显示信号变化
   - 验证代码自动检查结果正确性

5. **执行位置**：
   - 运行 `run.bat` 执行仿真
   - 使用 GTKWave 查看波形
   - 在 Vivado 中也可以运行仿真

---

## 9. 常见问题解答

### Q1: 为什么不能将 testbench.v 添加到 Vivado 工程？

**A**: `testbench.v` 仅用于仿真验证，不是实际硬件设计的一部分。Vivado 综合和实现阶段只需要实际的设计文件。

### Q2: 如何修改测试程序？

**A**: 修改 `instruction_memory.v` 中的 `initial` 块，将新的指令编码写入 `mem[]` 数组。

### Q3: 如何添加新的指令支持？

**A**:

1. 在 `control_unit.v` 中添加新指令的 `opcode` 识别和控制信号生成
2. 如果是 R-type 指令，在 `alu_control.v` 中添加对应的 `funct` 处理
3. 如果 ALU 需要新运算，在 `alu.v` 中添加新的运算类型

### Q4: 仿真时间不够怎么办？

**A**: 修改 `testbench.v` 第 69 行的 `#1000;`，增加仿真时间（单位：ns）。

### Q5: 如何查看特定寄存器的值？

**A**: 在 GTKWave 中添加信号路径：`testbench.uut.u_regfile.registers[寄存器编号]`

---

**文档版本**: 1.0  
**最后更新**: 2025-12-07  
**作者**: 课程设计项目组
