
`timescale 1ns/1ps
module tb_adder8bit;
  reg  [7:0] A, B;
  reg        Cin;
  wire [7:0] Sum;
  wire       Cout;
  adder8bit uut (
    .A(A),
    .B(B),
    .Cin(Cin),
    .Sum(Sum),
    .Cout(Cout)
  );
  
  initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_adder8bit);
    $monitor("Time=%0t: A=%b, B=%b, Cin=%b | Sum=%b, Cout=%b", 
            $time, A, B, Cin, Sum, Cout);
    A   = 8'b00010010;
    B   = 8'b10011010;
    Cin = 1'b0; #7;
    A   = 8'b00011110;
    B   = 8'b11001101;
    Cin = 1'b0; #2;
    A   = 8'b01011010;
    B   = 8'b10111110;
    Cin = 1'b0; #7;
    A   = 8'b10010011;
    B   = 8'b01011010;
    Cin = 1'b0; #3;
    A   = 8'b01010110;
    B   = 8'b10111011;
    Cin = 1'b0; #7;
    A   = 8'b11010010;
    B   = 8'b01011010;
    Cin = 1'b0; #2;
    A   = 8'b00010010;
    B   = 8'b10011010;
    Cin = 1'b0; #7;
    A   = 8'b00010010;
    B   = 8'b10011010;
    Cin = 1'b0; #3;
    
    $finish;
  end
endmodule
