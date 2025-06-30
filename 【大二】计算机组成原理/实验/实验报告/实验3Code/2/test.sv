`timescale 1ns/1ps

module tb_dff_async_reset();

reg clk, cp, d, r;
wire q, qn;

dff_async_reset uut (
  .clk(clk),
  .cp(cp),
  .d(d),
  .r(r),
  .q(q),
  .qn(qn)
);

initial begin
  clk = 0;
  forever #5 clk = ~clk;
end

initial begin
  r = 0; cp = 0; d = 0;

  #(7*10) do_input(0, 0, 0); // R=0, clk=0, D=0
  #(2*10) do_input(0, 0, 1); // R=0, clk=0, D=1
  #(7*10) do_input(0, 1, 0); // R=0, clk=1, D=0
  #(3*10) do_input(0, 1, 1); // R=0, clk=1, D=1
  #(7*10) do_input(1, 0, 0); // R=1, clk=0, D=0
  #(2*10) do_input(1, 0, 1); // R=1, clk=0, D=1
  #(7*10) do_input(1, 1, 0); // R=1, clk=1, D=0
  #(3*10) do_input(1, 1, 1); // R=1, clk=1, D=1

  #100 $finish;
end

task do_input(input reg r_val, input reg cp_val, input reg d_val);
  begin
    r = r_val;
    cp = cp_val;
    d = d_val;
    @(posedge clk);
  end
endtask

initial begin
  $dumpfile("waveform_async.vcd");
  $dumpvars(0, tb_dff_async_reset);
end

endmodule
