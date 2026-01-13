# 单周期 MIPS32 CPU 设计说明文档

## 一、项目概述

本项目实现了一个基于 MIPS32 精简指令集的单周期 CPU，支持 10 条以上的指令，能够在 FPGA 上运行并执行斐波那契数列计算程序。

### 1.1 设计特点

- **架构**：单周期 CPU（每条指令在一个时钟周期内完成）
- **指令集**：MIPS32 精简指令集
- **支持指令数**：10 条
- **数据通路**：经典五级数据通路（取指、译码、执行、访存、写回）
- **测试程序**：斐波那契数列计算（前 10 项）

### 1.2 支持的指令列表

| 序号 | 指令 | 类型   | 操作码             | 功能描述     |
| ---- | ---- | ------ | ------------------ | ------------ |
| 1    | add  | R-type | 0x00 (funct: 0x20) | 寄存器加法   |
| 2    | sub  | R-type | 0x00 (funct: 0x22) | 寄存器减法   |
| 3    | and  | R-type | 0x00 (funct: 0x24) | 按位与       |
| 4    | or   | R-type | 0x00 (funct: 0x25) | 按位或       |
| 5    | slt  | R-type | 0x00 (funct: 0x2A) | 小于则置位   |
| 6    | lw   | I-type | 0x23               | 从内存加载字 |
| 7    | sw   | I-type | 0x2B               | 向内存存储字 |
| 8    | beq  | I-type | 0x04               | 相等则分支   |
| 9    | addi | I-type | 0x08               | 立即数加法   |
| 10   | j    | J-type | 0x02               | 无条件跳转   |

## 二、模块功能划分与引脚定义

### 2.1 顶层模块 (cpu_top.v)

**功能**：整合所有子模块，实现完整的单周期 CPU 功能。

**引脚定义**：

```verilog
module cpu_top(
    input wire clk,              // 时钟信号
    input wire rst,              // 复位信号（高电平有效）
    output wire [31:0] pc_out,   // 当前 PC 值（调试用）
    output wire [31:0] alu_result // ALU 结果（调试用）
);
```

**内部连接**：

- 连接 PC 寄存器、指令存储器、控制单元、寄存器文件、ALU、数据存储器
- 实现数据通路的多路选择器逻辑
- 处理分支和跳转逻辑

---

### 2.2 PC 寄存器模块 (pc_register.v)

**功能**：存储程序计数器（PC），每个时钟周期更新。

**引脚定义**：

```verilog
module pc_register(
    input wire clk,                  // 时钟信号
    input wire rst,                  // 复位信号
    input wire [31:0] pc_next,       // 下一个 PC 值
    output reg [31:0] pc_current     // 当前 PC 值
);
```

**工作原理**：

- 复位时：PC = 0x00000000
- 正常运行：在时钟上升沿更新 PC 为 pc_next

---

### 2.3 指令存储器模块 (instruction_memory.v)

**功能**：存储程序指令，根据 PC 值读取指令。

**引脚定义**：

```verilog
module instruction_memory(
    input wire [31:0] addr,          // 指令地址（PC 值）
    output wire [31:0] instruction   // 输出的 32 位指令
);
```

**特点**：

- 容量：128 条指令（512 字节）
- 访问方式：组合逻辑读取（字对齐）
- 预装载：包含斐波那契数列计算程序

---

### 2.4 控制单元模块 (control_unit.v)

**功能**：根据指令操作码生成各种控制信号。

**引脚定义**：

```verilog
module control_unit(
    input wire [5:0] opcode,         // 6 位操作码
    output reg reg_dst,              // 寄存器目标选择
    output reg jump,                 // 跳转信号
    output reg branch,               // 分支信号
    output reg mem_read,             // 内存读信号
    output reg mem_to_reg,           // 内存到寄存器
    output reg [1:0] alu_op,         // ALU 操作类型
    output reg mem_write,            // 内存写信号
    output reg alu_src,              // ALU 源选择
    output reg reg_write             // 寄存器写使能
);
```

**控制信号说明**：

- **reg_dst**：选择写入的目标寄存器（0=rt, 1=rd）
- **jump**：是否执行跳转
- **branch**：是否可能分支
- **mem_read**：是否从内存读取
- **mem_to_reg**：写回数据来源（0=ALU, 1=内存）
- **alu_op**：ALU 操作类型（00=加法, 01=减法, 10=根据 funct）
- **mem_write**：是否写入内存
- **alu_src**：ALU 第二个操作数来源（0=寄存器, 1=立即数）
- **reg_write**：是否写入寄存器

---

### 2.5 寄存器文件模块 (register_file.v)

**功能**：32 个 32 位通用寄存器，支持同时 2 读 1 写。

**引脚定义**：

```verilog
module register_file(
    input wire clk,                      // 时钟信号
    input wire rst,                      // 复位信号
    input wire reg_write,                // 写使能
    input wire [4:0] read_reg1,          // 读寄存器 1 编号
    input wire [4:0] read_reg2,          // 读寄存器 2 编号
    input wire [4:0] write_reg,          // 写寄存器编号
    input wire [31:0] write_data,        // 写入数据
    output wire [31:0] read_data1,       // 读出数据 1
    output wire [31:0] read_data2        // 读出数据 2
);
```

**特点**：

- $0 寄存器恒为 0（硬件实现）
- 读操作：组合逻辑，立即输出
- 写操作：时序逻辑，时钟上升沿写入

---

### 2.6 ALU 控制单元模块 (alu_control.v)

**功能**：根据 ALU_OP 和功能码生成具体的 ALU 控制信号。

**引脚定义**：

```verilog
module alu_control(
    input wire [1:0] alu_op,             // ALU 操作类型
    input wire [5:0] funct,              // 功能码
    output reg [3:0] alu_control         // 4 位 ALU 控制信号
);
```

**ALU 控制码**：

- 0000: AND
- 0001: OR
- 0010: ADD（加法）
- 0110: SUB（减法）
- 0111: SLT（小于则置位）
- 1100: NOR

---

### 2.7 ALU 模块 (alu.v)

**功能**：执行算术和逻辑运算。

**引脚定义**：

```verilog
module alu(
    input wire [31:0] input1,            // 操作数 1
    input wire [31:0] input2,            // 操作数 2
    input wire [3:0] alu_control,        // ALU 控制信号
    output reg [31:0] result,            // 运算结果
    output wire zero                     // 零标志
);
```

**支持操作**：

- 算术运算：加法、减法、小于比较
- 逻辑运算：AND、OR、NOR

---

### 2.8 数据存储器模块 (data_memory.v)

**功能**：存储和读取数据。

**引脚定义**：

```verilog
module data_memory(
    input wire clk,                      // 时钟信号
    input wire mem_write,                // 写使能
    input wire mem_read,                 // 读使能
    input wire [31:0] addr,              // 访问地址
    input wire [31:0] write_data,        // 写入数据
    output wire [31:0] read_data         // 读出数据
);
```

**特点**：

- 容量：256 个字（1KB）
- 读操作：组合逻辑
- 写操作：时钟上升沿写入

## 三、数据通路设计

### 3.1 单周期数据通路图

```
                    +-------------+
                    |  PC 寄存器   |
                    +-------------+
                           |
                           v
                    +-------------+
                    | 指令存储器   |
                    +-------------+
                           |
            +-------------+-------------+
            |                           |
            v                           v
    +-------------+            +-------------+
    |  控制单元    |            | 寄存器文件   |
    +-------------+            +-------------+
            |                           |
            v                           v
    +-------------+              +-------+
    | ALU 控制    |              |  ALU  |
    +-------------+              +-------+
                                      |
                                      v
                              +-------------+
                              | 数据存储器   |
                              +-------------+
```

### 3.2 关键数据通路

1. **取指（IF）**：PC → 指令存储器 → 指令
2. **译码（ID）**：指令 → 控制单元 + 寄存器文件
3. **执行（EX）**：寄存器数据/立即数 → ALU → 结果
4. **访存（MEM）**：ALU 结果 → 数据存储器 → 数据
5. **写回（WB）**：ALU 结果/内存数据 → 寄存器文件

## 四、斐波那契数列程序说明

### 4.1 程序逻辑

```
初始化：
  fib[0] = 0
  fib[1] = 1
  counter = 2

循环（counter < 10）：
  fib[counter] = fib[counter-1] + fib[counter-2]
  存储到内存
  counter++

结束：
  无限循环
```

### 4.2 寄存器使用

- **$t0 ($8)**：fib[n-2]（前前一个数）
- **$t1 ($9)**：fib[n-1]（前一个数）
- **$t2 ($10)**：fib[n]（当前数）
- **$t3 ($11)**：循环计数器
- **$t4 ($12)**：循环上限（10）
- **$t5 ($13)**：内存地址偏移
- **$s0 ($16)**：数据存储基地址（0x100）

### 4.3 内存布局

| 地址  | 内容 | 说明   |
| ----- | ---- | ------ |
| 0x100 | 0    | fib[0] |
| 0x104 | 1    | fib[1] |
| 0x108 | 1    | fib[2] |
| 0x10C | 2    | fib[3] |
| 0x110 | 3    | fib[4] |
| 0x114 | 5    | fib[5] |
| 0x118 | 8    | fib[6] |
| 0x11C | 13   | fib[7] |
| 0x120 | 21   | fib[8] |
| 0x124 | 34   | fib[9] |

