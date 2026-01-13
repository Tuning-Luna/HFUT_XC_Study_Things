// ============================================================================
// Single-Cycle CPU Testbench
// ============================================================================
// Function: Test CPU running 6-instruction test program
// Note: Run for 100 clock cycles, generate waveform for analysis
// ============================================================================
// 单周期CPU测试平台，用于验证6条指令功能，运行100个时钟周期并生成波形文件
// 测试指令：add, sub, and, or, slt, addi

`timescale 1ns/1ps

module testbench;

    // Test signals  // 测试信号
    reg clk;
    reg rst;
    wire [31:0] pc_out;
    wire [31:0] alu_result;
    
    // Instantiate CPU  // 实例化CPU顶层
    cpu_top uut (
        .clk(clk),
        .rst(rst),
        .pc_out(pc_out),
        .alu_result(alu_result)
    );
    
    // Generate waveform file  // 生成波形文件和关键信号转储
    initial begin
        $dumpfile("cpu_test.vcd");
        $dumpvars(0, testbench);
        
        // Dump registers in register file // 转储寄存器文件中的寄存器
        $dumpvars(0, uut.u_regfile.registers[0]);
        $dumpvars(0, uut.u_regfile.registers[8]);   // $t0 = 5
        $dumpvars(0, uut.u_regfile.registers[9]);   // $t1 = 3
        $dumpvars(0, uut.u_regfile.registers[10]);  // $t2 (add结果)
        $dumpvars(0, uut.u_regfile.registers[11]);  // $t3 (sub结果)
        $dumpvars(0, uut.u_regfile.registers[12]);  // $t4 (and结果)
        $dumpvars(0, uut.u_regfile.registers[13]);  // $t5 (or结果)
        $dumpvars(0, uut.u_regfile.registers[14]);  // $t6 (slt结果)
        $dumpvars(0, uut.u_regfile.registers[15]);  // $t7 (addi结果)
        $dumpvars(0, uut.u_regfile.registers[16]);  // $s0 = 0x100
        $dumpvars(0, uut.u_regfile.registers[24]);  // $t8 (lw验证用)
        $dumpvars(0, uut.u_regfile.registers[25]);  // $t9 (beq验证用)
        
        // Dump key data memory locations // 转储数据内存相关位置（6条指令的测试结果）
        $dumpvars(0, uut.u_dmem.mem[64]);   // address 0x100 - add结果 = 8
        $dumpvars(0, uut.u_dmem.mem[65]);   // address 0x104 - sub结果 = 2
        $dumpvars(0, uut.u_dmem.mem[66]);   // address 0x108 - and结果 = 1
        $dumpvars(0, uut.u_dmem.mem[67]);   // address 0x10C - or结果 = 7
        $dumpvars(0, uut.u_dmem.mem[68]);   // address 0x110 - slt结果 = 1
        $dumpvars(0, uut.u_dmem.mem[69]);   // address 0x114 - addi结果 = 15
    end
    
    // Generate clock signal (10ns period, 100MHz) // 生成时钟信号（周期10ns，100MHz）
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    // Reset and test sequence // 复位及测试主流程
    initial begin
        // Initialize // 初始化
        rst = 1;
        #15;  // Hold reset for 15ns // 复位保持15ns
        rst = 0;
        
        // Run for 100 clock cycles // 运行100个时钟周期
        #1000;
        
        // Show result // 显示运算结果
        $display("========================================");
        $display("6-Instruction Test Program Finished."); // 6条指令测试程序完成
        $display("========================================");
        $display("Test Results stored in memory:"); // 存储在内存中的测试结果
        $display("Test 1 - ADD:  mem[0x100] = %d (expected: 8)", uut.u_dmem.mem[64]);
        $display("Test 2 - SUB:  mem[0x104] = %d (expected: 2)", uut.u_dmem.mem[65]);
        $display("Test 3 - AND:  mem[0x108] = %d (expected: 1)", uut.u_dmem.mem[66]);
        $display("Test 4 - OR:   mem[0x10C] = %d (expected: 7)", uut.u_dmem.mem[67]);
        $display("Test 5 - SLT:  mem[0x110] = %d (expected: 1)", uut.u_dmem.mem[68]);
        $display("Test 6 - ADDI: mem[0x114] = %d (expected: 15)", uut.u_dmem.mem[69]);
        $display("========================================");
        $display("Register Values:"); // 寄存器值
        $display("$t0 = %d, $t1 = %d", uut.u_regfile.registers[8], uut.u_regfile.registers[9]);
        $display("$t2 = %d (add), $t3 = %d (sub)", uut.u_regfile.registers[10], uut.u_regfile.registers[11]);
        $display("$t4 = %d (and), $t5 = %d (or)", uut.u_regfile.registers[12], uut.u_regfile.registers[13]);
        $display("$t6 = %d (slt), $t7 = %d (addi)", uut.u_regfile.registers[14], uut.u_regfile.registers[15]);
        $display("========================================");
        $display("Final PC value: 0x%h", pc_out); // 最终PC值
        $display("========================================");
        
        // Check result // 验证6条指令测试结果正确性
        if (uut.u_dmem.mem[64] == 8 &&   // add: 5 + 3 = 8
            uut.u_dmem.mem[65] == 2 &&    // sub: 5 - 3 = 2
            uut.u_dmem.mem[66] == 1 &&    // and: 5 & 3 = 1
            uut.u_dmem.mem[67] == 7 &&    // or:  5 | 3 = 7
            uut.u_dmem.mem[68] == 1 &&    // slt: (3 < 5) ? 1 : 0 = 1
            uut.u_dmem.mem[69] == 15) begin // addi: 5 + 10 = 15
            $display("✓ All 6 instructions test PASSED!"); // 所有6条指令测试通过
        end else begin
            $display("✗ Test FAILED! Some instruction result is incorrect."); // 测试失败
            if (uut.u_dmem.mem[64] != 8) $display("  - ADD test failed: got %d, expected 8", uut.u_dmem.mem[64]);
            if (uut.u_dmem.mem[65] != 2) $display("  - SUB test failed: got %d, expected 2", uut.u_dmem.mem[65]);
            if (uut.u_dmem.mem[66] != 1) $display("  - AND test failed: got %d, expected 1", uut.u_dmem.mem[66]);
            if (uut.u_dmem.mem[67] != 7) $display("  - OR test failed: got %d, expected 7", uut.u_dmem.mem[67]);
            if (uut.u_dmem.mem[68] != 1) $display("  - SLT test failed: got %d, expected 1", uut.u_dmem.mem[68]);
            if (uut.u_dmem.mem[69] != 15) $display("  - ADDI test failed: got %d, expected 15", uut.u_dmem.mem[69]);
        end
        $display("========================================");
        
        $finish;
    end
    
    // Monitor key signals // 监控关键信号变化
    initial begin
        $monitor("Time=%0t | PC=0x%h | Instr=0x%h | ALU_result=%d", 
                $time, pc_out, uut.instruction, alu_result); // 时间、PC、指令、ALU输出
    end

endmodule
