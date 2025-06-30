module logic_circuit (
    input wire A,
    input wire B,
    input wire C,
    output wire Y1,
    output wire Y2
);

    wire G1,G2,G3,G4,G5;

    assign G1 = A & B;
    assign G3 = A ^ B;
    assign G2 = C & G3;
    assign G4 = C ^ G3;
    assign G5 = ~(G1 | G2);

    assign Y2 = G4;
    assign Y1 = ~G5;
endmodule
