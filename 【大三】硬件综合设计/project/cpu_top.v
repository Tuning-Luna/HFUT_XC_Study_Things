// ============================================================================
// 单周期 MIPS32 CPU 顶层模块
// ============================================================================
// 功能：整合所有子模块，实现单周期 CPU 的完整功能
// 作者：课程设计
// 日期：2025-12-07
// ============================================================================

`timescale 1ns/1ps

module cpu_top(
    input wire clk,           // 时钟信号
    input wire rst,           // 复位信号（高电平有效）
    output wire [31:0] pc_out,    // 当前 PC 值（用于调试）
    output wire [31:0] alu_result // ALU 结果（用于调试）
);

    // ========== 内部信号定义 ==========
    
    // PC 相关信号
    wire [31:0] pc_current;      // 当前 PC
    wire [31:0] pc_next;         // 下一个 PC
    wire [31:0] pc_plus4;        // PC + 4
    wire [31:0] pc_branch;       // 分支目标地址
    
    // 指令存储器信号
    wire [31:0] instruction;     // 当前指令
    
    // 指令字段
    wire [5:0] opcode;           // 操作码
    wire [4:0] rs;               // 源寄存器1
    wire [4:0] rt;               // 源寄存器2
    wire [4:0] rd;               // 目标寄存器
    wire [4:0] shamt;            // 移位量
    wire [5:0] funct;            // 功能码
    wire [15:0] immediate;       // 立即数
    wire [25:0] jump_addr;       // 跳转地址
    
    // 控制信号
    wire reg_dst;                // 寄存器目标选择
    wire jump;                   // 跳转信号
    wire branch;                 // 分支信号
    wire mem_read;               // 内存读信号
    wire mem_to_reg;             // 内存到寄存器
    wire [1:0] alu_op;           // ALU 操作类型
    wire mem_write;              // 内存写信号
    wire alu_src;                // ALU 源操作数选择
    wire reg_write;              // 寄存器写使能
    
    // 寄存器文件信号
    wire [4:0] write_reg;        // 写入的寄存器编号
    wire [31:0] write_data;      // 写入寄存器的数据
    wire [31:0] read_data1;      // 读出的数据1
    wire [31:0] read_data2;      // 读出的数据2
    
    // ALU 信号
    wire [31:0] alu_input2;      // ALU 第二个输入
    wire [31:0] alu_out;         // ALU 输出
    wire alu_zero;               // ALU 零标志
    wire [3:0] alu_control;      // ALU 控制信号
    
    // 数据存储器信号
    wire [31:0] mem_data;        // 从内存读出的数据
    
    // 立即数扩展
    wire [31:0] sign_extended;   // 符号扩展后的立即数
    
    // 分支控制
    wire pc_src;                 // PC 源选择
    
    // ========== 指令字段解析 ==========
    assign opcode = instruction[31:26];
    assign rs = instruction[25:21];
    assign rt = instruction[20:16];
    assign rd = instruction[15:11];
    assign shamt = instruction[10:6];
    assign funct = instruction[5:0];
    assign immediate = instruction[15:0];
    assign jump_addr = instruction[25:0];
    
    // ========== 输出调试信号 ==========
    assign pc_out = pc_current;
    assign alu_result = alu_out;
    
    // ========== 模块实例化 ==========
    
    // PC 寄存器
    pc_register u_pc(
        .clk(clk),
        .rst(rst),
        .pc_next(pc_next),
        .pc_current(pc_current)
    );
    
    // 指令存储器
    instruction_memory u_imem(
        .addr(pc_current),
        .instruction(instruction)
    );
    
    // 控制单元
    control_unit u_control(
        .opcode(opcode),
        .reg_dst(reg_dst),
        .jump(jump),
        .branch(branch),
        .mem_read(mem_read),
        .mem_to_reg(mem_to_reg),
        .alu_op(alu_op),
        .mem_write(mem_write),
        .alu_src(alu_src),
        .reg_write(reg_write)
    );
    
    // 寄存器文件
    register_file u_regfile(
        .clk(clk),
        .rst(rst),
        .reg_write(reg_write),
        .read_reg1(rs),
        .read_reg2(rt),
        .write_reg(write_reg),
        .write_data(write_data),
        .read_data1(read_data1),
        .read_data2(read_data2)
    );
    
    // ALU 控制单元
    alu_control u_alu_control(
        .alu_op(alu_op),
        .funct(funct),
        .alu_control(alu_control)
    );
    
    // ALU
    alu u_alu(
        .input1(read_data1),
        .input2(alu_input2),
        .alu_control(alu_control),
        .result(alu_out),
        .zero(alu_zero)
    );
    
    // 数据存储器
    data_memory u_dmem(
        .clk(clk),
        .mem_write(mem_write),
        .mem_read(mem_read),
        .addr(alu_out),
        .write_data(read_data2),
        .read_data(mem_data)
    );
    
    // ========== 数据通路连接 ==========
    
    // 符号扩展
    assign sign_extended = {{16{immediate[15]}}, immediate};
    
    // ALU 第二个输入选择（立即数或寄存器）
    assign alu_input2 = alu_src ? sign_extended : read_data2;
    
    // 写入寄存器选择（rt 或 rd）
    assign write_reg = reg_dst ? rd : rt;
    
    // 写入数据选择（ALU 结果或内存数据）
    assign write_data = mem_to_reg ? mem_data : alu_out;
    
    // PC + 4
    assign pc_plus4 = pc_current + 4;
    
    // 分支目标地址
    assign pc_branch = pc_plus4 + (sign_extended << 2);
    
    // 分支控制
    assign pc_src = branch & alu_zero;
    
    // 下一个 PC 选择
    assign pc_next = jump ? {pc_plus4[31:28], jump_addr, 2'b00} :
                     (pc_src ? pc_branch : pc_plus4);

endmodule
