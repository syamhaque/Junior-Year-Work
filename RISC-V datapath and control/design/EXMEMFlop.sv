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


module EXMEMFlop#
    (parameter WIDTH = 8)
    (input logic clk, reset,enable,
	input logic [8:0] PCPlusB,
	input logic RegWriteEX, MemtoRegEX, MemWriteEX,MemReadEX,B,BranchEX,JALREX,
	input logic [4:0] WriteRegAddrEX,
	input logic [31:0] Reg2EX, JalrMux, ALUMux,
	input logic [2:0] Funct3EX,
	output logic [8:0] PCPlusBMEM,
	output logic RegWriteMEM, MemtoRegMEM, MemWriteMEM,MemReadMEM,BMEM,BranchMEM,JALRMEM,
	output logic [4:0] WriteRegAddrMEM,
	output logic [31:0] Reg2MEM, JalrMuxMEM, ALUMuxMEM,
	output logic [2:0] Funct3MEM
);

always_ff @(posedge clk, posedge reset)
    if (reset) begin
		PCPlusBMEM = 'b0;
		RegWriteMEM = 'b0;
		MemtoRegMEM = 'b0;
		MemWriteMEM = 'b0;
		MemReadMEM = 'b0;
		BMEM = 'b0;
		JALRMEM = 'b0;
		WriteRegAddrMEM = 'b0;
		Reg2MEM = 'b0;
		JalrMuxMEM = 'b0;
		ALUMuxMEM = 'b0;
		Funct3MEM = 'b0;
		BranchMEM = 'b0;
		end
    else if(enable)
		begin
		PCPlusBMEM = PCPlusB;
		RegWriteMEM = RegWriteEX;
		MemtoRegMEM = MemtoRegEX;
		MemWriteMEM = MemWriteEX;
		MemReadMEM = MemReadEX;
		BMEM = B;
		JALRMEM = JALREX;
		WriteRegAddrMEM = WriteRegAddrEX;
		Reg2MEM = Reg2EX;
		JalrMuxMEM = JalrMux;
		ALUMuxMEM = ALUMux;
		Funct3MEM = Funct3EX;
		BranchMEM = BranchEX;
		end
    
endmodule
