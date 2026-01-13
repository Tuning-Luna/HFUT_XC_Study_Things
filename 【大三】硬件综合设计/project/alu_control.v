// ============================================================================
// ALU 控制单元模块
// ============================================================================
// 功能：根据 ALU_OP 和功能码生成 ALU 控制信号
// 输入：alu_op - 来自主控制单元的 ALU 操作类型
//       funct - R-type 指令的功能码
// 输出：alu_control - 4 位 ALU 控制信号
// ============================================================================

`timescale 1ns/1ps

module alu_control(
    input wire [1:0] alu_op,          // ALU 操作类型
    input wire [5:0] funct,           // 功能码
    output reg [3:0] alu_control      // ALU 控制信号
);

    // ALU 控制信号定义
    localparam ALU_AND = 4'b0000;  // AND 操作
    localparam ALU_OR  = 4'b0001;  // OR 操作
    localparam ALU_ADD = 4'b0010;  // 加法 
    localparam ALU_SUB = 4'b0110;  // 减法
    localparam ALU_SLT = 4'b0111;  // 小于则置位
    localparam ALU_NOR = 4'b1100;  // NOR 操作
    
    // 功能码定义（R-type 指令）
    localparam FUNCT_ADD = 6'b100000;  // add
    localparam FUNCT_SUB = 6'b100010;  // sub
    localparam FUNCT_AND = 6'b100100;  // and
    localparam FUNCT_OR  = 6'b100101;  // or
    localparam FUNCT_SLT = 6'b101010;  // slt
    
    always @(*) begin
        case (alu_op)
            2'b00: begin  // lw, sw, addi - 加法
                alu_control = ALU_ADD;
            end
            
            2'b01: begin  // beq - 减法（用于比较）
                alu_control = ALU_SUB;
            end
            
            2'b10: begin  // R-type - 根据功能码决定
                case (funct)
                    FUNCT_ADD: alu_control = ALU_ADD;
                    FUNCT_SUB: alu_control = ALU_SUB;
                    FUNCT_AND: alu_control = ALU_AND;
                    FUNCT_OR:  alu_control = ALU_OR;
                    FUNCT_SLT: alu_control = ALU_SLT;
                    default:   alu_control = ALU_ADD;
                endcase
            end
            
            default: begin
                alu_control = ALU_ADD;
            end
        endcase
    end

endmodule
