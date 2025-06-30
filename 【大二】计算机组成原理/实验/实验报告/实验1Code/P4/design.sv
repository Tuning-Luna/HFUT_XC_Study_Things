module circuit4(
    input A, B, C, D,
    output Y2, Y1, Y0
);
    assign Y2 = (~A & ~B & C) | (~A & B & ~C);
    assign Y1 = (~A & ~C & D) | (~A & B & C);
    assign Y0 = (~A & B & D) | (~A & C & D) | (A & ~B & ~C & ~D);
endmodule