`timescale 1ns/1ps

module tb_counter10();

reg clk, r, x;
wire [3:0] q;

counter10 uut (
  .clk(clk),
  .r(r),
  .x(x),
  .q(q)
);

initial begin
  clk = 0;
  forever #1 clk = ~clk;
end

initial begin
  r = 0;
  #(20*7);
  r = 1;

  x = 0;
  #(20*2);

  x = 1;
  #(20*7);

  #(20*3);

  #100 $finish;
end

initial begin
  $dumpfile("waveform_counter.vcd");
  $dumpvars(0, tb_counter10);
end

endmodule
