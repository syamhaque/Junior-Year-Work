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


module MEMWBFlop#
    (parameter WIDTH = 8)
    (input logic clk, reset,enable,
	input logic [31:0] ALUMux,ReadData,
	input logic RegWriteMEM,MemtoRegMEM,
	input logic [4:0] WriteRegAddrMEM,
	output logic [31:0] ALUMuxWB,ReadDataWB,
	output logic RegWriteWB,MemtoRegWB,
	output logic [4:0] WriteRegAddrWB
);

always_ff @(posedge clk, posedge reset)
    if (reset) begin
		ALUMuxWB = 'b0;
		ReadDataWB = 'b0;
		RegWriteWB = 'b0;
		MemtoRegWB = 'b0;
		WriteRegAddrWB = 'b0;
		end
    else if(enable)
		begin
		ALUMuxWB = ALUMux;
		ReadDataWB = ReadData;
		RegWriteWB = RegWriteMEM;
		MemtoRegWB = MemtoRegMEM;
		WriteRegAddrWB = WriteRegAddrMEM;
		end
    
endmodule
