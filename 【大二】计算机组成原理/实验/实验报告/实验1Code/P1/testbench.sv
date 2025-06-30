`timescale 1ns/1ps

module tb1;

    reg A, B, C;
    wire F;

    // 实例化设计模块
    logic_circuit uut (
        .A(A),
        .B(B),
        .C(C),
        .F(F)
    );

    initial begin
        $dumpfile("waveform.vcd");  
        $dumpvars(0, tb1);          

        A = 0; B = 0; C = 0;
        
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

    // 实时打印
    initial begin
        $monitor("Time=%0t: A=%b B=%b C=%b => F=%b", $time, A, B, C, F);
    end

endmodule
