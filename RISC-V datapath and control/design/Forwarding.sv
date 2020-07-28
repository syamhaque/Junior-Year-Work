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


module Forwarding#
    (parameter WIDTH = 8)
    (
	input logic MEMRegWrite, WBRegWrite,
	input logic [4:0] Addr1, Addr2, MEMrd, WBrd,
	output logic [1:0] ForwardA, ForwardB );


	always_comb
	begin
	ForwardA = 2'b0;
	ForwardB = 2'b0;
	if(MEMRegWrite)
		begin
		if(MEMrd!='b0 && MEMrd == Addr1)
			begin
			ForwardA[1] = 1'b1;
			//ForwardB = 2'b00;
			end
		if(MEMrd!='b0 && MEMrd == Addr2)
			begin
			//ForwardA = 2'b00;
			ForwardB[1] = 1'b1;
			end
		end
	if(WBRegWrite)
		begin
		if(WBrd!='b0 && WBrd == Addr1)
			begin
			ForwardA[0] = 1'b1;
			//ForwardB = 2'b00;
			end
		if(WBrd!='b0 && WBrd == Addr2)
			begin
			//ForwardA = 2'b00;
			ForwardB[0] = 1'b1;
			end
		end
	else
		begin
			ForwardA = 'b0;
			ForwardB = 'b0;
		end
    	end
endmodule
