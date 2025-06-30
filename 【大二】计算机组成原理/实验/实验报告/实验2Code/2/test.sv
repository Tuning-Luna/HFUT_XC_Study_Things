`timescale 1ns/1ps
module tb_mux2to1_4;
  reg [3:0] A, B;
  reg       S;
  wire [3:0] F;
  mux2to1_4 uut (
    .A(A),
    .B(B),
    .S(S),
    .F(F)
  );
  
  initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_mux2to1_4);
    $monitor("Time=%0t: A=%b, B=%b, S=%b, F=%b", $time, A, B, S, F);
    A = 4'b0001; B = 4'b0011; S = 1'b0; #7;
    A = 4'b0001; B = 4'b0011; S = 1'b1; #2;
    A = 4'b0010; B = 4'b0110; S = 1'b0; #7;
    A = 4'b0010; B = 4'b0110; S = 1'b1; #3;
    A = 4'b1001; B = 4'b0101; S = 1'b0; #7;
    A = 4'b1001; B = 4'b0101; S = 1'b1; #2;
    A = 4'b1011; B = 4'b0111; S = 1'b0; #7;
    A = 4'b1011; B = 4'b0111; S = 1'b1; #3;
    
    $finish;
  end
  
endmodule
