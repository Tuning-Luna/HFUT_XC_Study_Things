`timescale 1ns/1ps
module tb3();
    reg M;
    reg N;
    reg P;
    reg Q;
    wire Z;

    circuit3 uut (
        .M(M),
        .N(N),
        .P(P),
        .Q(Q),
        .Z(Z)
);

    initial begin
        // 添加VCD文件用于波形查看
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb3);



        M=0; N=0; P=0; Q=0; #7;
        M=0; N=0; P=0; Q=1; #2;
        M=0; N=0; P=1; Q=0; #9;
        M=0; N=0; P=1; Q=1; #4;
        M=0; N=1; P=0; Q=0; #7;
        M=0; N=1; P=0; Q=1; #2;
        M=0; N=1; P=1; Q=0; #9;
        M=0; N=1; P=1; Q=1; #4;
        M=1; N=0; P=0; Q=0; #7;
        M=1; N=0; P=0; Q=1; #2;
        M=1; N=0; P=1; Q=0; #9;
        M=1; N=0; P=1; Q=1; #4;
        M=1; N=1; P=0; Q=0; #7;
        M=1; N=1; P=0; Q=1; #2;
        M=1; N=1; P=1; Q=0; #9;
        M=1; N=1; P=1; Q=1; #4;

        $finish;
    end

endmodule