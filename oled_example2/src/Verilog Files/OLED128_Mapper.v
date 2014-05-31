`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// OLED128_Mapper wire GPIO register bits to external pins for OLED128 display
//////////////////////////////////////////////////////////////////////////////////


module OLED128_Mapper(
    input clk,
    input [31:0] gpio_oled_control,
    output [31:0] gpio_oled_status,
    input oled_sd_cd,
    output oled_rst,
    output oled_dc
    );
    

    (* mark_debug = "true" *) reg [31:0] dbg_oled_status;
    (* mark_debug = "true" *) reg [31:0] dbg_oled_control;

    //    Y16 --> JB1_DNC
    //    Y17 --> JB2_MISO
    //    W14 --> JB3_MOSI
    //    Y14 --> JB4_SCK
    //    T16 --> JB7_RST
    //    U17 --> JB8_OLED_CS
    //    V15 --> JB9_SD_CS
    //    W15 --> JB10_SD_CD

    assign oled_dc = gpio_oled_control[0];
    assign oled_rst = gpio_oled_control[1];

    wire [31:0] thingy;
    assign thingy[31:4] = 'hDEAD000;
    assign thingy[3] = 0;
    assign thingy[2] = oled_rst;
    assign thingy[1] = oled_dc;
    assign thingy[0] = oled_sd_cd;

    assign gpio_oled_status = thingy;

    always @(posedge clk)
    begin
        // assign data to local reg, then local reg to port
        
        dbg_oled_status <= thingy;
        dbg_oled_control <= gpio_oled_control;
    end
 

endmodule
