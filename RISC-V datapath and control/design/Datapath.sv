`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/07/2018 10:10:33 PM
// Design Name: 
// Module Name: Datapath
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

module Datapath #(
    parameter PC_W = 9, // Program Counter
    parameter INS_W = 32, // Instruction Width
    parameter RF_ADDRESS = 5, // Register File Address
    parameter DATA_W = 32, // Data WriteData
    parameter DM_ADDRESS = 9, // Data Memory Address
    parameter ALU_CC_W = 4 // ALU Control Code Width
    )(
    input logic clk , reset , // global clock
    output logic [DATA_W-1:0] WB_Data //ALU_Result
    );

logic [PC_W-1:0] PC,PCID,PCEX, PCPlus4,PCPlus4ID,PCPlus4EX, PCPlusB,PCPlusBMEM,PCPlusTemp, PCPlus, ExtShift;
logic [INS_W-1:0] Instr,InstrID,InstrID1,InstrEX;
logic [DATA_W-1:0] Result;
logic [DATA_W-1:0] Reg1, Reg2,Reg1EX,Reg1EX2,Reg1EX3,Reg2EX,Reg2EX2,Reg2EX3, Reg2MEM;
logic [DATA_W-1:0] ReadData;
logic [DATA_W-1:0] SrcA, SrcB, ALUResult, ALUMux,ALUMuxMEM, ALUMuxWB, ReadDataWB;
logic [DATA_W-1:0] ExtImm,ExtImmEX, JalrMux,JalrMuxMEM;
logic Zero;
logic B, hazard;
//logic RegWriteID, MemtoRegID, ALUSrcID, MemWriteID,MemReadID,BranchID,JALID, JALRID, AUIPCID;
//logic [3:0] ALU_CCID;
logic RegWriteID1, MemtoRegID1, ALUSrcID1, MemWriteID1,MemReadID1,BranchID1,JALID1, JALRID1, AUIPCID1;
logic [3:0] ALU_CCID1,ALU_CCEX;
logic RegWriteEX, MemtoRegEX, ALUSrcEX, MemWriteEX,MemReadEX,BranchEX,JALEX, JALREX, AUIPCEX;
logic [1:0]  ForwardA, ForwardB;
logic [4:0] WriteRegAddrEX, Reg1AddrEX, Reg2AddrEX;
logic [2:0] Funct3EX, Funct3MEM;
logic RegWriteMEM,MemtoRegMEM, MemWriteMEM,MemReadMEM,BMEM,JALRMEM;
logic [4:0] WriteRegAddrMEM,WriteRegAddrWB;
logic RegWriteWB,MemtoRegWB,bhazard,breset;


    logic [6:0] opcode;
    logic [6:0] Funct7;
    logic [2:0] Funct3;

                           // reset , sets the PC to zero
    logic RegWrite , MemtoReg ,     // Register file writing enable   // Memory or ALU MUX
    ALUSrc , MemWrite ,       // Register file or Immediate MUX // Memroy Writing Enable
    MemRead ,                 // Memroy Reading Enable
	Branch, JAL, JALR, AUIPC; 		// Branch instuction mux selection
    logic [ ALU_CC_W -1:0] ALU_CC; // ALU Control Code ( input of the ALU )
	logic [1:0] ALUop;



 //Instruction memory
    instructionmemory instr_mem (PC, Instr);
    
IFIDFlop ifidflop(clk,(reset || BMEM || JALRMEM), (hazard),PC,PCPlus4,Instr, PCID, PCPlus4ID,InstrID);

    assign opcode = InstrID[6:0];
    assign Funct7 = InstrID[31:25];
    assign Funct3 = InstrID[14:12];
      
	Controller c(opcode, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, JAL, JALR, AUIPC, ALUop);
    
    ALUController ac(ALUop, Funct7, Funct3, ALU_CC);


// //Register File
    RegFile rf(clk, reset, RegWriteWB, WriteRegAddrWB, InstrID[19:15], InstrID[24:20],
            Result, Reg1, Reg2);

	
//Hazard Detection Start

HazardDetection hazdetect(MemReadEX,RegWrite, MemtoReg, ALUSrc, MemWrite,MemRead,Branch,JAL, JALR, AUIPC,ALU_CC,InstrID, InstrID[19:15], InstrID[24:20], WriteRegAddrEX, hazard,RegWriteID1, MemtoRegID1, ALUSrcID1, MemWriteID1,MemReadID1,BranchID1,JALID1, JALRID1, AUIPCID1,ALU_CCID1,InstrID1 );

//Hazard Detection End

//Branch Detection Start

BranchDetection brancdetect(BranchID1, BranchMEM, BMEM,bhazard,breset);

//Branch Detection End

    //Instr[11:7] -> WriteRegAddr
	// Instr[19:15]->Reg1Addr
	// Instr[24:20] ->Reg2Addr
IDEXFlop idexflop(clk, (reset || BMEM || JALRMEM), 1'b1, PCID, PCPlus4ID,RegWriteID1, MemtoRegID1, ALUSrcID1, MemWriteID1,MemReadID1,BranchID1,JALID1, JALRID1, AUIPCID1,ALU_CCID1, InstrID1[11:7], InstrID1[19:15], InstrID1[24:20],Reg1,Reg2,InstrID1, Funct3, PCEX, PCPlus4EX,RegWriteEX, MemtoRegEX, ALUSrcEX, MemWriteEX,MemReadEX,BranchEX,JALEX, JALREX, AUIPCEX,ALU_CCEX, WriteRegAddrEX, Reg1AddrEX, Reg2AddrEX,Reg1EX,Reg2EX,InstrEX,Funct3EX);    

	mux2 #(32) forward1a(Reg1EX,Result,ForwardA[0],Reg1EX2);
	mux2 #(32) forward2a(Reg1EX2,ALUMuxMEM, ForwardA[1],Reg1EX3);

	mux2 #(32) pcsrcmux(Reg1EX3,{23'b0,PC},AUIPCEX,SrcA);

	mux2 #(32) forward1b(Reg2EX,Result,ForwardB[0],Reg2EX2);
	mux2 #(32) forward2b(Reg2EX2,ALUMuxMEM, ForwardB[1],Reg2EX3);
//// sign extend
    imm_Gen Ext_Imm (InstrEX,ExtImmEX);


    mux2 #(32) srcbmux(Reg2EX3, ExtImmEX, ALUSrcEX, SrcB);
           

//// ALU
    
    alu alu_module(SrcA, SrcB, ALU_CCEX, Zero, ALUResult);
    mux2 #(32) jalmux(ALUResult,{23'b0,PCPlus4EX},JALEX,ALUMux);

	assign ExtShift = (ExtImmEX[7:0] << 1);
	adder #(9) badd(PCEX,ExtShift, PCPlusB);
	assign B = BranchEX & Zero;

	assign JalrMux = (ALUResult & 32'hFFFFFFFE);

//Forwarding Unit Start
	
Forwarding forward(RegWriteMEM,RegWriteWB,Reg1AddrEX,Reg2AddrEX,WriteRegAddrMEM,WriteRegAddrWB,ForwardA,ForwardB);


//Forwarding Unit End


EXMEMFlop exmemflop(clk, reset, 1'b1,PCPlusB, RegWriteEX,MemtoRegEX, MemWriteEX,MemReadEX,B,BranchEX,JALREX,WriteRegAddrEX,Reg2EX3,JalrMux,ALUMux,Funct3EX,  PCPlusBMEM, RegWriteMEM,MemtoRegMEM, MemWriteMEM,MemReadMEM,BMEM,BranchMEM,JALRMEM,WriteRegAddrMEM,Reg2MEM,JalrMuxMEM, ALUMuxMEM, Funct3MEM);

	

// next PC (MEM)
    adder #(9) pcadd (PC, 9'b100, PCPlus4);
    mux2 #(9) branchmux(JalrMuxMEM[8:0], PCPlusBMEM, BMEM, PCPlusTemp);
    mux2 #(9) jalrmux(PCPlus4,PCPlusTemp, JALRMEM, PCPlus);

	//mux2 #(9) branchmux(PCPlus4, PCPlusBMEM, BMEM, PCPlusTemp);
    //mux2 #(9) jalrmux(PCPlusTemp,JalrMuxMEM[8:0], JALRMEM, PCPlus);

    flopr #(9) pcreg(clk, reset, (hazard), PCPlus, PC);

    
////// Data memory 
	datamemory data_mem(clk, MemReadMEM, MemWriteMEM, ALUMuxMEM[DM_ADDRESS-1:0], Funct3MEM, Reg2MEM, ReadData);

MEMWBFlop memwbflop(clk, reset, 1'b1, ALUMuxMEM,ReadData,RegWriteMEM,MemtoRegMEM,WriteRegAddrMEM, ALUMuxWB,ReadDataWB,RegWriteWB,MemtoRegWB,WriteRegAddrWB );
	mux2 #(32) resmux(ALUMuxWB, ReadDataWB, MemtoRegWB, Result);
    
    assign WB_Data = Result;
     
endmodule
