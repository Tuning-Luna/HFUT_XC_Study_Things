module dff_async_reset (
  input clk,
  input cp,
  input d,
  input r,    
  output reg q,
  output reg qn
);

always @(posedge clk or posedge r) begin
  if (r == 1'b1) begin
    q <= 1'b0;
    qn <= 1'b1;
  end else if (cp == 1'b1) begin
    if (d == 1'b1) begin
      q <= 1'b1;
      qn <= 1'b0;
    end else begin
      q <= 1'b0;
      qn <= 1'b1;
    end
  end
end

endmodule
