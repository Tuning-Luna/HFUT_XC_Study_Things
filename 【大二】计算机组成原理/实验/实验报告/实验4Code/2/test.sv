module seven_seg_display_tb;

    reg clk;
    reg rst;
    reg [3:0] data3, data2, data1, data0;
    wire [7:0] Bit;
    wire [7:0] SEG;

    seven_seg_display uut (
        .clk(clk),
        .rst(rst),
        .data3(data3),
        .data2(data2),
        .data1(data1),
        .data0(data0),
        .Bit(Bit),
        .SEG(SEG)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        rst = 0;
        data3 = 4'd1;
        data2 = 4'd2;
        data1 = 4'd3;
        data0 = 4'd4;
        #35; 
        rst = 1;
        #1000000;
        $finish;
    end

    initial begin
        $dumpfile("seven_seg_display.vcd");
        $dumpvars(0, seven_seg_display_tb);
    end

endmodule