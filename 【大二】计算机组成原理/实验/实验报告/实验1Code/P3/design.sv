module circuit3 (
    input  M,
    input  N,
    input  P,
    input  Q,
    output Z
);
    assign Z = (M & N) | (M & P) | (M & Q) |
               (N & P) | (N & Q) |
               (P & Q);

endmodule