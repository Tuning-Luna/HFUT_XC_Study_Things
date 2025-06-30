`timescale 1ns/1ps

module tb_shift_register();

logic clk;
logic [1:0] s;
logic dsl, dsr;
logic [3:0] din;
wire [3:0] q;

shift_register uut (
  .clk(clk),
  .s(s),
  .dsl(dsl),
  .dsr(dsr),
  .din(din),
  .q(q)
);

// 时钟
initial begin
  clk = 0;
  forever #1 clk = ~clk;
end

initial begin
  din = 4'b1101;
  dsl = 1;
  dsr = 1;

  s = 2'b11;  #3;  
  s = 2'b10;  #7;  
  s = 2'b11;  #3;  
  s = 2'b01;  #2;  
  s = 2'b11;  #3;  
  s = 2'b00;  #7;  

  //  7 2 7 3 

  #10 $finish;
end

initial begin
  $dumpfile("waveform_shift.vcd");
  $dumpvars(0, tb_shift_register);
end

endmodule
