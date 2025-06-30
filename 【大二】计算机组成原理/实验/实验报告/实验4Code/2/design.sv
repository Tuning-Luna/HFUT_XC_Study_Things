module seven_seg_display (
    input clk,
    input rst,
    input [3:0] data3, data2, data1, data0,
    output reg [7:0] Bit,
    output reg [7:0] SEG
);

    reg [19:0] counter; 
    reg [3:0] display_data [7:0];
    reg [2:0] digit_select;
    reg [26:0] second_counter;

    always @(posedge clk) begin
        if (~rst) counter <= 0;
        else counter <= counter + 1;
        if (counter == 999999) counter <= 0;
    end

    always @(posedge clk) begin
        if (~rst) digit_select <= 0;
        else if (counter == 999999) digit_select <= digit_select + 1;
    end

    // Frequency division for clk2 (1s period)
    always @(posedge clk) begin
        if (~rst) second_counter <= 0;
        else second_counter <= second_counter + 1;
        if (second_counter == 99999999) second_counter <= 0; 
    end

    always @(posedge clk) begin
        if (~rst) begin
            display_data[0] <= data0;
            display_data[1] <= data1;
            display_data[2] <= data2;
            display_data[3] <= data3;
            display_data[4] <= 0;
            display_data[5] <= 0;
            display_data[6] <= 0;
            display_data[7] <= 0;
        end else if (second_counter == 99999999) begin
            display_data[0] <= (display_data[0] + 1) % 10;
            display_data[1] <= (display_data[1] + 1) % 10;
            display_data[2] <= (display_data[2] + 1) % 10;
            display_data[3] <= (display_data[3] + 1) % 10;
            display_data[4] <= (display_data[4] + 1) % 10;
            display_data[5] <= (display_data[5] + 1) % 10;
            display_data[6] <= (display_data[6] + 1) % 10;
            display_data[7] <= (display_data[7] + 1) % 10;
        end
    end

    always @* begin
        Bit <= 8'hFF;
        Bit[digit_select] <= 1'b0; 
        case (display_data[digit_select])
            4'd0: SEG <= 8'b11000000;
            4'd1: SEG <= 8'b11111001;
            4'd2: SEG <= 8'b10100100;
            4'd3: SEG <= 8'b10110000;
            4'd4: SEG <= 8'b10011001;
            4'd5: SEG <= 8'b10010010;
            4'd6: SEG <= 8'b10000010;
            4'd7: SEG <= 8'b11111000;
            4'd8: SEG <= 8'b10000000;
            4'd9: SEG <= 8'b10010000;
            default: SEG <= 8'b11111111;
        endcase
    end

endmodule