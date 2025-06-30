module encoder8to3 (
    input  wire X0,  // 输入X0～X7均为高电平（1）时无编码效果，
    input  wire X1,
    input  wire X2,
    input  wire X3,
    input  wire X4,
    input  wire X5,
    input  wire X6,
    input  wire X7,
    output wire P2,  // 输出P2 P1 P0对应的二进制编码
    output wire P1,
    output wire P0
);
  assign {P2, P1, P0} = (!X0) ? 3'b000 :
                          (!X1) ? 3'b001 :
                          (!X2) ? 3'b010 :
                          (!X3) ? 3'b011 :
                          (!X4) ? 3'b100 :
                          (!X5) ? 3'b101 :
                          (!X6) ? 3'b110 :
                          (!X7) ? 3'b111 : 3'bxxx;
  
endmodule
