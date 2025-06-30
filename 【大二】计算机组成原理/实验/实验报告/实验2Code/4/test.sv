`timescale 1ns/1ps
module tb_encoder8to3;
  reg X0, X1, X2, X3, X4, X5, X6, X7;
  wire P2, P1, P0;
  encoder8to3 uut (
    .X0(X0),
    .X1(X1),
    .X2(X2),
    .X3(X3),
    .X4(X4),
    .X5(X5),
    .X6(X6),
    .X7(X7),
    .P2(P2),
    .P1(P1),
    .P0(P0)
  );
  
  initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_encoder8to3);
    $monitor("Time=%0t: X7X6X5X4X3X2X1X0=%b%b%b%b%b%b%b%b | P2P1P0=%b%b%b", 
            $time, X7, X6, X5, X4, X3, X2, X1, X0, P2, P1, P0);
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11111110; #7;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11111101; #2;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11111011; #7;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11110111; #3;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11101111; #7;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b11011111; #2;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b10111111; #7;
    {X7, X6, X5, X4, X3, X2, X1, X0} = 8'b01111111; #3;
    
    $finish;
  end
  
endmodule
