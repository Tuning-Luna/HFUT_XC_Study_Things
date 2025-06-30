`timescale 1ns/1ps

module vga_tb;
  logic clk = 0;
  logic rst = 1;
  logic hsync, vsync;
  logic [3:0] r, g, b;

  vga_top uut (
    .clk_100mhz(clk),
    .rst(rst),
    .hsync(hsync),
    .vsync(vsync),
    .vga_r(r),
    .vga_g(g),
    .vga_b(b)
  );

  always #5 clk = ~clk;

  initial begin
    $dumpfile("vga.vcd");
    $dumpvars(0, uut);

    #100 rst = 0;

    #20000 $finish;
  end
endmodule
