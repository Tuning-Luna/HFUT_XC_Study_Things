module decoder3to8 (
    input  wire A2, 
    input  wire A1, 
    input  wire A0, 
    output wire Y0, 
    output wire Y1, 
    output wire Y2, 
    output wire Y3, 
    output wire Y4, 
    output wire Y5, 
    output wire Y6, 
    output wire Y7  
);
  assign Y0 = ( ~A2 & ~A1 & ~A0 ) ? 1'b0 : 1'b1;
  assign Y1 = ( ~A2 & ~A1 &  A0 ) ? 1'b0 : 1'b1;
  assign Y2 = ( ~A2 &  A1 & ~A0 ) ? 1'b0 : 1'b1;
  assign Y3 = ( ~A2 &  A1 &  A0 ) ? 1'b0 : 1'b1;
  assign Y4 = (  A2 & ~A1 & ~A0 ) ? 1'b0 : 1'b1;
  assign Y5 = (  A2 & ~A1 &  A0 ) ? 1'b0 : 1'b1;
  assign Y6 = (  A2 &  A1 & ~A0 ) ? 1'b0 : 1'b1;
  assign Y7 = (  A2 &  A1 &  A0 ) ? 1'b0 : 1'b1;
  
endmodule
