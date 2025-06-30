`timescale 1ns / 1ps
module tb4;
    reg A, B, C, D;
    wire Y2, Y1, Y0;
    
    circuit4 uut (
        .A(A),
        .B(B),
        .C(C),
        .D(D),
        .Y2(Y2),
        .Y1(Y1),
        .Y0(Y0)
    );

    initial begin
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb4);

        A = 0; B = 0; C = 0; D = 0; #7;
        A = 0; B = 0; C = 0; D = 1; #2;
        A = 0; B = 0; C = 1; D = 0; #9;
        A = 0; B = 0; C = 1; D = 1; #4;
        A = 0; B = 1; C = 0; D = 0; #7;
        A = 0; B = 1; C = 0; D = 1; #2;
        A = 0; B = 1; C = 1; D = 0; #9;
        A = 0; B = 1; C = 1; D = 1; #4;
        A = 1; B = 0; C = 0; D = 0; #7;
        A = 1; B = 0; C = 0; D = 1; #2;
        A = 1; B = 0; C = 1; D = 0; #9;
        A = 1; B = 0; C = 1; D = 1; #4;
        A = 1; B = 1; C = 0; D = 0; #7;
        A = 1; B = 1; C = 0; D = 1; #2;
        A = 1; B = 1; C = 1; D = 0; #9;
        A = 1; B = 1; C = 1; D = 1; #4;
        $finish;
    end
endmodule