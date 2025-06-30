`timescale 1ns/1ps
module tb_mux2to1;
  reg A, B, S;
  wire F;
  
  mux2to1 uut (
    .A(A),
    .B(B),
    .S(S),
    .F(F)
  );
  
  initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_mux2to1);
    $monitor("Time=%0t: A=%b, B=%b, S=%b, F=%b", $time, A, B, S, F);
    
    
    {A, B, S} = 3'b000;
    #7;
    
    {A, B, S} = 3'b001;
    #2;
    
    {A, B, S} = 3'b010;
    #7;
    
    {A, B, S} = 3'b011;
    #3;
    
    {A, B, S} = 3'b100;
    #7;
    
    {A, B, S} = 3'b101;
    #2;
    
    {A, B, S} = 3'b110;
    #7;
    
    {A, B, S} = 3'b111;
    #3;
    
    $finish;
  end
  
endmodule
