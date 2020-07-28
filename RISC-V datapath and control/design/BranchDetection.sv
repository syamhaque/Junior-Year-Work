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


module BranchDetection#
    (parameter WIDTH = 8)
    (input logic Branch,BranchMEM, BMEM,
	output logic branchhazard,reset);

	always_comb
	begin
	// If Data Hazard, then Hazard==0, else Hazard = 1.
	if(Branch && BranchMEM == 0)
		begin
		branchhazard = 0;
		reset = 0;
		end
	else if(Branch && BranchMEM && BMEM)
		begin
			branchhazard = 1;
			reset = 1;
		end
	else
		begin
		branchhazard = 1;
		reset = 0;
		end
	end
endmodule

