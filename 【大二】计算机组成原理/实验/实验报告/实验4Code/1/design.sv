module seq_detector (
    input clk,
    input rst,
    input din,
    output reg dout
);

    reg [2:0] state, next_state;
    parameter IDLE = 3'b000;
    parameter GOT_1 = 3'b001;
    parameter GOT_10 = 3'b010;
    parameter GOT_101 = 3'b011;
    parameter DETECTED = 3'b100;

    always @* begin
        case (state)
            IDLE:
                if (din == 1'b1) next_state = GOT_1;
                else next_state = IDLE;
            GOT_1:
                if (din == 1'b0) next_state = GOT_10;
                else next_state = GOT_1;
            GOT_10:
                if (din == 1'b1) next_state = GOT_101;
                else next_state = IDLE;
            GOT_101:
                if (din == 1'b1) next_state = DETECTED;
                else next_state = GOT_10;
            DETECTED:
                if (din == 1'b1) next_state = GOT_1;
                else next_state = GOT_10;
            default: next_state = IDLE;
        endcase
    end

    always @* begin
        dout = (state == DETECTED) ? 1'b1 : 1'b0;
    end

    always @(posedge clk) begin
        if (rst == 1'b0) state <= IDLE;
        else state <= next_state;
    end

endmodule