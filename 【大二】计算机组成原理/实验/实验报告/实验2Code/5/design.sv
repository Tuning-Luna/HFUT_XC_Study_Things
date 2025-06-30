module full_adder (
    input  wire A,      // 加数A
    input  wire B,      // 加数B
    input  wire Cin,    // 进位输入
    output wire Sum,    // 和
    output wire Cout    // 进位输出
);
  assign Sum  = A ^ B ^ Cin;
  assign Cout = (A & B) | (B & Cin) | (A & Cin);
endmodule
module adder8bit (
    input  wire [7:0] A,    // 8位加数A
    input  wire [7:0] B,    // 8位加数B
    input  wire       Cin,  // 初始进位
    output wire [7:0] Sum,  // 8位和
    output wire       Cout  // 最终进位
);
  wire [6:0] carry;  // 内部进位信号
  
  full_adder FA0 (
    .A   (A[0]),
    .B   (B[0]),
    .Cin (Cin),
    .Sum (Sum[0]),
    .Cout(carry[0])
  );
  
  full_adder FA1 (
    .A   (A[1]),
    .B   (B[1]),
    .Cin (carry[0]),
    .Sum (Sum[1]),
    .Cout(carry[1])
  );
  
  full_adder FA2 (
    .A   (A[2]),
    .B   (B[2]),
    .Cin (carry[1]),
    .Sum (Sum[2]),
    .Cout(carry[2])
  );
  
  full_adder FA3 (
    .A   (A[3]),
    .B   (B[3]),
    .Cin (carry[2]),
    .Sum (Sum[3]),
    .Cout(carry[3])
  );
  
  full_adder FA4 (
    .A   (A[4]),
    .B   (B[4]),
    .Cin (carry[3]),
    .Sum (Sum[4]),
    .Cout(carry[4])
  );
  
  full_adder FA5 (
    .A   (A[5]),
    .B   (B[5]),
    .Cin (carry[4]),
    .Sum (Sum[5]),
    .Cout(carry[5])
  );
  
  full_adder FA6 (
    .A   (A[6]),
    .B   (B[6]),
    .Cin (carry[5]),
    .Sum (Sum[6]),
    .Cout(carry[6])
  );
  
  full_adder FA7 (
    .A   (A[7]),
    .B   (B[7]),
    .Cin (carry[6]),
    .Sum (Sum[7]),
    .Cout(Cout)
  );
endmodule
