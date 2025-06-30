`timescale 1ns/1ps
module tb_decoder3to8;
  reg A2, A1, A0;
  wire Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7;
  decoder3to8 uut (
    .A2(A2),
    .A1(A1),
    .A0(A0),
    .Y0(Y0),
    .Y1(Y1),
    .Y2(Y2),
    .Y3(Y3),
    .Y4(Y4),
    .Y5(Y5),
    .Y6(Y6),
    .Y7(Y7)
  );
  
  initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, tb_decoder3to8);
    $monitor("Time=%0t: A2A1A0=%b%b%b | Y0=%b Y1=%b Y2=%b Y3=%b Y4=%b Y5=%b Y6=%b Y7=%b",
            $time, A2, A1, A0, Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7);
    {A2, A1, A0} = 3'b000; #7;
    {A2, A1, A0} = 3'b001; #2;
    {A2, A1, A0} = 3'b010; #7;
    {A2, A1, A0} = 3'b011; #3;
    {A2, A1, A0} = 3'b100; #7;
    {A2, A1, A0} = 3'b101; #2;
    {A2, A1, A0} = 3'b110; #7;
    {A2, A1, A0} = 3'b111; #3;
    
    $finish;
  end
  
endmodule
