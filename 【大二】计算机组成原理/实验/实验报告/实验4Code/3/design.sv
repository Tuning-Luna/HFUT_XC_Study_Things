// design.sv

module clk_div (
  input  logic clk_in,
  input  logic rst,
  output logic clk_out
);
  logic [1:0] cnt;
  always_ff @(posedge clk_in or posedge rst) begin
    if (rst) begin
      cnt <= 0;
      clk_out <= 0;
    end else begin
      cnt <= cnt + 1;
      if (cnt == 1)
        clk_out <= ~clk_out;
    end
  end
endmodule

module vga_sync (
  input  logic clk,
  input  logic rst,
  output logic hsync,
  output logic vsync,
  output logic h_valid,
  output logic v_valid,
  output logic [9:0] x,
  output logic [9:0] y
);
  parameter H_SYNC  = 96,
            H_BACK  = 48,
            H_DISP  = 640,
            H_FRONT = 16,
            H_TOTAL = 800;
  parameter V_SYNC  = 2,
            V_BACK  = 33,
            V_DISP  = 480,
            V_FRONT = 10,
            V_TOTAL = 525;

  logic [9:0] h_cnt = 0, v_cnt = 0;

  always_ff @(posedge clk or posedge rst) begin
    if (rst) begin
      h_cnt <= 0;
      v_cnt <= 0;
    end else begin
      if (h_cnt < H_TOTAL - 1)
        h_cnt <= h_cnt + 1;
      else begin
        h_cnt <= 0;
        if (v_cnt < V_TOTAL - 1)
          v_cnt <= v_cnt + 1;
        else
          v_cnt <= 0;
      end
    end
  end

  always_ff @(posedge clk) begin
    hsync <= (h_cnt < H_SYNC) ? 0 : 1;
    vsync <= (v_cnt < V_SYNC) ? 0 : 1;

    h_valid <= (h_cnt >= (H_SYNC + H_BACK)) &&
               (h_cnt <  (H_SYNC + H_BACK + H_DISP));
    v_valid <= (v_cnt >= (V_SYNC + V_BACK)) &&
               (v_cnt <  (V_SYNC + V_BACK + V_DISP));

    x <= h_valid ? h_cnt - (H_SYNC + H_BACK) : 0;
    y <= v_valid ? v_cnt - (V_SYNC + V_BACK) : 0;
  end
endmodule

module color_bar (
  input  logic [9:0] x,
  input  logic [9:0] y,
  input  logic h_valid,
  input  logic v_valid,
  output logic [3:0] r,
  output logic [3:0] g,
  output logic [3:0] b
);
  always_comb begin
    if (h_valid && v_valid) begin
      case (x[9:6])
        4'd2: {r, g, b} = {4'hF, 4'h0, 4'h0}; // Red
        4'd3: {r, g, b} = {4'h0, 4'hF, 4'h0}; // Green
        4'd4: {r, g, b} = {4'h0, 4'h0, 4'hF}; // Blue
        4'd5: {r, g, b} = {4'hF, 4'hF, 4'h0}; // Yellow
        4'd6: {r, g, b} = {4'hF, 4'h0, 4'hF}; // Magenta
        4'd7: {r, g, b} = {4'h0, 4'hF, 4'hF}; // Cyan
        default: {r, g, b} = {4'hF, 4'hF, 4'hF}; // White
      endcase
    end else begin
      {r, g, b} = 12'h000; // Black outside
    end
  end
endmodule

module vga_top (
  input  logic clk_100mhz,
  input  logic rst,
  output logic hsync,
  output logic vsync,
  output logic [3:0] vga_r,
  output logic [3:0] vga_g,
  output logic [3:0] vga_b
);
  logic clk_25mhz;
  logic h_valid, v_valid;
  logic [9:0] x, y;

  clk_div u_clk (.clk_in(clk_100mhz), .rst(rst), .clk_out(clk_25mhz));
  vga_sync u_sync (
    .clk(clk_25mhz), .rst(rst),
    .hsync(hsync), .vsync(vsync),
    .h_valid(h_valid), .v_valid(v_valid),
    .x(x), .y(y)
  );
  color_bar u_bar (
    .x(x), .y(y),
    .h_valid(h_valid), .v_valid(v_valid),
    .r(vga_r), .g(vga_g), .b(vga_b)
  );
endmodule
