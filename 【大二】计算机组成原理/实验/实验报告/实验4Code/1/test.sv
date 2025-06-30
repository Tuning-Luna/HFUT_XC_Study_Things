module seq_detector_tb;

    reg clk;
    reg rst;
    reg din;
    wire dout;

    seq_detector uut (
        .clk(clk),
        .rst(rst),
        .din(din),
        .dout(dout)
    );

    initial begin
        clk = 0;
        forever #2000 clk = ~clk; // 2 μs 切换，周期 4 μs
    end

    initial begin
        rst = 0;
        din = 0;
        #35000; // 延迟 7 单位（7 * 5 μs = 35 μs = 35000 ns）
        rst = 1;

        // 输入序列："1101101101000101101110111101000101010110101"
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 0;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 0;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 din = 0;
        #4000 din = 1;
        #4000 $finish;
    end

    // 导出波形
    initial begin
        $dumpfile("seq_detector.vcd");
        $dumpvars(0, seq_detector_tb);
    end

endmodule