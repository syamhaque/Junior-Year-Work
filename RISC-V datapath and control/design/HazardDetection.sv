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


module HazardDetection#
    (parameter WIDTH = 8)
    (input logic EXMemRead,RegWriteID, MemtoRegID, ALUSrcID, MemWriteID,MemReadID,BranchID,JALID, JALRID, AUIPCID,
	input logic [3:0] ALU_CCID,
	input logic [31:0] InstrID, 
	input logic [4:0] Addr1, Addr2,WriteAddr,
	output logic hazard,RegWriteID1, MemtoRegID1, ALUSrcID1, MemWriteID1,MemReadID1,BranchID1,JALID1, JALRID1, AUIPCID1,
	output logic [3:0] ALU_CCID1,
	output logic [31:0] InstrID1);

	always_comb
	begin
	// If Data Hazard, then Hazard==0, else Hazard = 1.
	if(EXMemRead && (Addr1 == WriteAddr || Addr2 == WriteAddr))
		begin
		hazard = 0;
		RegWriteID1 = 0;
		MemtoRegID1 = 0;
		ALUSrcID1 = 0;
		MemWriteID1 = 0;
		MemReadID1 = 0;
		BranchID1 = 0;
		JALID1 = 0;
		JALRID1 = 0;
		AUIPCID1 = 0;
		ALU_CCID1 = 'b0;
		InstrID1 = 'b0;
		end
	else
		begin
			hazard = 1;
			RegWriteID1 = RegWriteID;
			MemtoRegID1 = MemtoRegID;
			ALUSrcID1 = ALUSrcID;
			MemWriteID1 = MemWriteID;
			MemReadID1 = MemReadID;
			BranchID1 = BranchID;
			JALID1 = JALID;
			JALRID1 = JALRID;
			AUIPCID1 = AUIPCID;
			ALU_CCID1 = ALU_CCID;
			InstrID1 = InstrID;
		end
	end
endmodule
