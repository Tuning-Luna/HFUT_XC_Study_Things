// ============================================================================
// 数据存储器模块
// ============================================================================
// 功能：存储和读取数据
// 输入：clk - 时钟信号
//       mem_write - 写使能信号
//       mem_read - 读使能信号
//       addr - 访问地址
//       write_data - 写入数据
// 输出：read_data - 读出数据
// 说明：支持 256 个字（1KB）的数据存储
// ============================================================================

`timescale 1ns/1ps

module data_memory(
    input wire clk,                   // 时钟信号
    input wire mem_write,             // 写使能
    input wire mem_read,              // 读使能
    input wire [31:0] addr,           // 地址
    input wire [31:0] write_data,     // 写入数据
    output wire [31:0] read_data      // 读出数据
);

    // 数据存储器（256 个字）
    reg [31:0] mem [0:255];
    
    // 读操作（组合逻辑）
    assign read_data = mem_read ? mem[addr[9:2]] : 32'h00000000;
    
    // 写操作（时序逻辑）
    always @(posedge clk) begin
        if (mem_write) begin
            mem[addr[9:2]] <= write_data;
        end
    end
    
    // 初始化存储器
    integer i;
    initial begin
        for (i = 0; i < 256; i = i + 1) begin
            mem[i] = 32'h00000000;
        end
    end

endmodule
