module shift_register (
  input logic clk,
  input logic [1:0] s,         
  input logic dsl, dsr,        
  input logic [3:0] din,       
  output logic [3:0] q         
);

always_ff @(posedge clk) begin
  case (s)
    2'b00: q <= q;             
    2'b01: q <= {q[2:0], dsr}; 
    2'b10: q <= {dsl, q[3:1]}; 
    2'b11: q <= din;           
  endcase
end

endmodule
