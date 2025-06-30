`timescale 1ns/1ps

module tb_dff();

reg clk;
reg cp;
reg d;
wire q;
wire qn;

dff uut (
  .clk(clk),
  .cp(cp),
  .d(d),
  .q(q),
  .qn(qn)
);

initial begin
  clk = 0;
  forever #5 clk = ~clk;
end

initial begin
  cp = 0;
  d = 0;

  #(7*10) clk_action(0, 0);
  #(2*10) clk_action(0, 1);
  #(7*10) clk_action(1, 0);
  #(3*10) clk_action(1, 1);
  #(7*10) clk_action(1, 0);
  #(2*10) clk_action(0, 1);
  #(7*10) clk_action(0, 0);
  #(3*10) clk_action(1, 1);

  #100 $finish;
end

task clk_action(input reg clk_val, input reg d_val);
  begin
    cp = 1;
    d = d_val;
    @(posedge clk);
    cp = 0;
  end
endtask

initial begin
  $dumpfile("waveform.vcd");
  $dumpvars(0, tb_dff);
end

endmodule
