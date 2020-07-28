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


module IDEXFlop#
    (parameter WIDTH = 8)
    (input logic clk, reset,enable,
	input logic [8:0] PCID,PCPlus4ID,
	input logic RegWriteID, MemtoRegID, ALUSrcID, MemWriteID,MemReadID,BranchID,JALID, JALRID, AUIPCID,
	input logic [3:0] ALU_CCID,
	input logic [4:0] WriteRegAddr, Reg1Addr, Reg2Addr,
	input logic [31:0] Reg1, Reg2, ExtImm,
	input logic [2:0] Funct3, 
	output logic [8:0] PCEX,PCPlus4EX,
	output logic RegWriteEX, MemtoRegEX, ALUSrcEX, MemWriteEX,MemReadEX,BranchEX,JALEX, JALREX, AUIPCEX,
	output logic [3:0] ALU_CCEX,
	output logic [4:0] WriteRegAddrEX, Reg1AddrEX, Reg2AddrEX,
	output logic [31:0] Reg1EX, Reg2EX, ExtImmEX,
	output logic [2:0] Funct3EX 
);

always_ff @(posedge clk, posedge reset)
    if (reset)
	begin
		PCEX = PCID;
                PCPlus4EX = 'b0;
                RegWriteEX = 'b0;
                MemtoRegEX = 'b0;
                ALUSrcEX = 'b0;
                MemWriteEX = 'b0;
                MemReadEX = 'b0;
                BranchEX = 'b0;
                JALEX    = 'b0;
                JALREX = 'b0;
                AUIPCEX = 'b0;
                ALU_CCEX = 'b0;
                WriteRegAddrEX = 'b0;
                Reg1AddrEX = 'b0;
                Reg2AddrEX = 'b0;
                Reg1EX = 'b0;
                Reg2EX = 'b0;
                ExtImmEX = 'b0;
                Funct3EX = 'b0;

	end
    else if(enable)
		begin
		PCEX = PCID;
		PCPlus4EX = PCPlus4ID;
		RegWriteEX = RegWriteID;
		MemtoRegEX = MemtoRegID;
		ALUSrcEX = ALUSrcID;
		MemWriteEX = MemWriteID;
		MemReadEX = MemReadID;
		BranchEX = BranchID;
		JALEX	 = JALID;
		JALREX = JALRID;
		AUIPCEX = AUIPCID;
		ALU_CCEX = ALU_CCID;
		WriteRegAddrEX = WriteRegAddr;
		Reg1AddrEX = Reg1Addr;
		Reg2AddrEX = Reg2Addr;
		Reg1EX = Reg1;
		Reg2EX = Reg2;
		ExtImmEX = ExtImm;
		Funct3EX = Funct3;
		end
    
endmodule
