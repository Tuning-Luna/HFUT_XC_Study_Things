module logic_circuit(
    input wire A,
    input wire B,
    input wire C,
    output wire F
);

    wire xor_result;
    assign xor_result = A ^ B;
    assign F = xor_result & C;
endmodule
