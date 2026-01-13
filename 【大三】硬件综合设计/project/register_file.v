// ============================================================================
// 寄存器文件模块
// ============================================================================
// 功能：32 个 32 位通用寄存器，支持 2 读 1 写
// 输入：clk - 时钟信号
//       rst - 复位信号
//       reg_write - 写使能信号
//       read_reg1, read_reg2 - 读寄存器编号
//       write_reg - 写寄存器编号
//       write_data - 写入数据
// 输出：read_data1, read_data2 - 读出数据
// 说明：$0 寄存器恒为 0
// ============================================================================

`timescale 1ns/1ps

module register_file(
    input wire clk,                    // 时钟信号
    input wire rst,                    // 复位信号
    input wire reg_write,              // 写使能
    input wire [4:0] read_reg1,        // 读寄存器 1 编号
    input wire [4:0] read_reg2,        // 读寄存器 2 编号
    input wire [4:0] write_reg,        // 写寄存器编号
    input wire [31:0] write_data,      // 写入数据
    output wire [31:0] read_data1,     // 读出数据 1
    output wire [31:0] read_data2      // 读出数据 2
);

    // 32 个 32 位寄存器
    reg [31:0] registers [0:31];
    
    // 读操作（组合逻辑）
    assign read_data1 = (read_reg1 == 5'b00000) ? 32'h00000000 : registers[read_reg1];
    assign read_data2 = (read_reg2 == 5'b00000) ? 32'h00000000 : registers[read_reg2];
    
    // 写操作（时序逻辑）
    integer i;
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            // 复位时清零所有寄存器
            for (i = 0; i < 32; i = i + 1) begin
                registers[i] <= 32'h00000000;
            end
        end else if (reg_write && write_reg != 5'b00000) begin
            // 写使能且不是 $0 寄存器时写入
            registers[write_reg] <= write_data;
        end
    end

endmodule
