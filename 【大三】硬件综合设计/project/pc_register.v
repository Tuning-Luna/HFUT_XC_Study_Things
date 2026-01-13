// ============================================================================
// PC 寄存器模块
// ============================================================================
// 功能：存储当前程序计数器（PC）的值，每个时钟周期更新
// 输入：clk - 时钟信号
//       rst - 复位信号（高电平有效）
//       pc_next - 下一个 PC 值
// 输出：pc_current - 当前 PC 值
// ============================================================================

`timescale 1ns/1ps

module pc_register(
    input wire clk,              // 时钟信号
    input wire rst,              // 复位信号
    input wire [31:0] pc_next,   // 下一个 PC 值
    output reg [31:0] pc_current // 当前 PC 值
);

    // 在时钟上升沿或复位时更新 PC
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            pc_current <= 32'h00000000;  // 复位时 PC 归零
        end else begin
            pc_current <= pc_next;        // 正常情况下更新为下一个 PC
        end
    end

endmodule
