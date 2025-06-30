module dff (
  input clk,
  input cp,
  input d,
  output reg q,
  output reg qn
);

always @(posedge clk) begin
  if (cp == 1'b0) begin
  end else begin
    if (d == 1'b0) begin
      q <= 1'b0;
      qn <= 1'b1;
    end else begin
      q <= 1'b1;
      qn <= 1'b0;
    end
  end
end

endmodule
