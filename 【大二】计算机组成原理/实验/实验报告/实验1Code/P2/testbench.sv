`timescale 1ns/1ps

module tb2;

    reg A, B, C;
    wire Y1, Y2;

    logic_circuit uut (
        .A(A),
        .B(B),
        .C(C),
        .Y1(Y1),
        .Y2(Y2)
    );

    initial begin
        $monitor("Time=%0t: A=%b B=%b C=%b => Y1=%b Y2=%b", $time, A, B, C, Y1, Y2);

        $dumpfile("waveform.vcd");
        $dumpvars(0, tb2);

        // 初始化
        {A, B, C} = 3'b000; #7;
        {A, B, C} = 3'b001; #2;
        {A, B, C} = 3'b010; #9;
        {A, B, C} = 3'b011; #4;
        {A, B, C} = 3'b100; #7;
        {A, B, C} = 3'b101; #2;
        {A, B, C} = 3'b110; #9;
        {A, B, C} = 3'b111; #4;

        $finish;
    end

endmodule