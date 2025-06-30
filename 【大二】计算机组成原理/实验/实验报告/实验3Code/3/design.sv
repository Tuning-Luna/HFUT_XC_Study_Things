module counter10 (
  input clk,
  input r,
  input x,
  output reg [3:0] q
);

always @(posedge clk or negedge r) begin
  if (!r) begin
    q <= 4'd0;
  end else begin
    if (x == 1'b0) begin
      q <= (q == 4'd9) ? 4'd0 : q + 1;
    end else begin
      q <= (q == 4'd0) ? 4'd9 : q - 1;
    end
  end
end

endmodule
