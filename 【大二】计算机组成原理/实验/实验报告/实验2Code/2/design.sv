module mux2to1_4 (
    input  wire [3:0] A, 
    input  wire [3:0] B, 
    input  wire       S, 
    output wire [3:0] F  
);
  assign F = (S == 1'b0) ? A : B;
  
endmodule
