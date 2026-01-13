// ============================================================================
// 控制单元模块
// ============================================================================
// 功能：根据指令的操作码生成各种控制信号
// 输入：opcode - 6 位操作码
// 输出：各种控制信号
// 支持指令：R-type, lw, sw, beq, addi, j
// ============================================================================

`timescale 1ns/1ps

module control_unit(
    input wire [5:0] opcode,      // 操作码
    output reg reg_dst,           // 1: rd, 0: rt
    output reg jump,              // 跳转信号
    output reg branch,            // 分支信号
    output reg mem_read,          // 内存读信号
    output reg mem_to_reg,        // 1: 内存数据写入寄存器, 0: ALU 结果写入
    output reg [1:0] alu_op,      // ALU 操作类型
    output reg mem_write,         // 内存写信号
    output reg alu_src,           // 1: 立即数, 0: 寄存器
    output reg reg_write          // 寄存器写使能
);

    // 操作码定义
    localparam OP_RTYPE = 6'b000000;  // R-type 指令
    localparam OP_LW    = 6'b100011;  // lw 指令
    localparam OP_SW    = 6'b101011;  // sw 指令
    localparam OP_BEQ   = 6'b000100;  // beq 指令
    localparam OP_ADDI  = 6'b001000;  // addi 指令
    localparam OP_JUMP  = 6'b000010;  // j 指令
    
    // 根据操作码生成控制信号
    always @(*) begin
        // 默认值（避免 latch）
        reg_dst = 0;
        jump = 0;
        branch = 0;
        mem_read = 0;
        mem_to_reg = 0;
        alu_op = 2'b00;
        mem_write = 0;
        alu_src = 0;
        reg_write = 0;
        
        case (opcode)
            OP_RTYPE: begin  // R-type 指令 (add, sub, and, or, slt)
                reg_dst = 1;
                alu_op = 2'b10;
                reg_write = 1;
            end
            
            OP_LW: begin     // lw 指令
                alu_src = 1;
                mem_to_reg = 1;
                reg_write = 1;
                mem_read = 1;
                alu_op = 2'b00;
            end
            
            OP_SW: begin     // sw 指令
                alu_src = 1;
                mem_write = 1;
                alu_op = 2'b00;
            end
            
            OP_BEQ: begin    // beq 指令
                branch = 1;
                alu_op = 2'b01;
            end
            
            OP_ADDI: begin   // addi 指令
                alu_src = 1;
                reg_write = 1;
                alu_op = 2'b00;
            end
            
            OP_JUMP: begin   // j 指令
                jump = 1;
            end
            
            default: begin
                // NOP 或未定义指令
            end
        endcase
    end

endmodule
