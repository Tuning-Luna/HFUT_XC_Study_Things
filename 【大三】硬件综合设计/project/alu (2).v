// ============================================================================
// ALU (算术逻辑单元) 模块
// ============================================================================
// 功能：执行算术和逻辑运算
// 输入：input1, input2 - 32 位操作数
//       alu_control - 4 位控制信号
// 输出：result - 32 位运算结果
//       zero - 零标志（结果为 0 时为 1）
// 支持操作：ADD, SUB, AND, OR, SLT, NOR
// ============================================================================

`timescale 1ns/1ps

module alu(
    input wire [31:0] input1,         // 操作数 1
    input wire [31:0] input2,         // 操作数 2
    input wire [3:0] alu_control,     // ALU 控制信号
    output reg [31:0] result,         // 运算结果
    output wire zero                  // 零标志
);

    // ALU 控制信号定义
    localparam ALU_AND = 4'b0000;
    localparam ALU_OR  = 4'b0001;
    localparam ALU_ADD = 4'b0010;
    localparam ALU_SUB = 4'b0110;
    localparam ALU_SLT = 4'b0111;
    localparam ALU_NOR = 4'b1100;
    
    // 零标志：结果为 0 时置 1
    assign zero = (result == 32'h00000000);
    
    // ALU 运算
    always @(*) begin
        case (alu_control)
            ALU_AND: result = input1 & input2;           // AND
            ALU_OR:  result = input1 | input2;           // OR
            ALU_ADD: result = input1 + input2;           // 加法
            ALU_SUB: result = input1 - input2;           // 减法
            ALU_SLT: result = ($signed(input1) < $signed(input2)) ? 32'h00000001 : 32'h00000000;  // 小于则置位
            ALU_NOR: result = ~(input1 | input2);        // NOR
            default: result = 32'h00000000;
        endcase
    end

endmodule
