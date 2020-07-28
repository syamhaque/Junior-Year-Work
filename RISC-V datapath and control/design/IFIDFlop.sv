`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/07/2018 10:19:34 PM
// Design Name: 
// Module Name: flopr
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module IFIDFlop#
    (parameter WIDTH = 8)
    (input logic clk, reset,enable,
	input logic [8:0] PC,PCPlus4,
	input logic [31:0] Instr,
	output logic [8:0] PCID, PCPlus4ID,
	output logic [31:0] InstrID
);

always_ff @(posedge clk, posedge reset)
    if (reset)
		begin
			PCID = 'b0;
			PCPlus4ID = 'b0;
			InstrID = 'b0;
		end
    else if(enable)
		begin
		PCID = PC;
		PCPlus4ID = PCPlus4;
		InstrID = Instr;
		end
    
endmodule
