`timescale 1ns / 1ps
//`define __SIM__

module datamemory#(
    parameter DM_ADDRESS = 9 ,
    parameter DATA_W = 32
    )(
    input logic clk,
	input logic MemRead , // comes from control unit
    input logic MemWrite , // Comes from control unit
    input logic [ DM_ADDRESS -1:0] a , // Read / Write address - 9 LSB bits of the ALU output
    input logic [2:0] funct3, // Funct3 to choose between (LW,LH,LB,LHU,LBU,SW,SH,SB)
    input logic [ DATA_W -1:0] wd , // Write Data
    output logic [ DATA_W -1:0] rd // Read Data
    );

`ifdef __SIM__
    
    logic [DATA_W-1:0] mem [(2**(DM_ADDRESS-2))-1:0];
    
    always_comb 
    begin
       if(MemRead)
		begin
		if(funct3 == 3'b000) //LB
			begin
				if(a[1:0] == 2'b0)
					rd = $signed({mem[a[DM_ADDRESS-1:2]][7]? 24'hffffff:24'b0,mem[a[DM_ADDRESS-1:2]][7:0]});
				else if(a[1:0]==2'b1)
					rd = $signed({mem[a[DM_ADDRESS-1:2]][15]? 16'hffff:16'b0,mem[a[DM_ADDRESS-1:2]][15:8],8'b0});
				else if(a[1:0]==2'b10)
					rd = $signed({mem[a[DM_ADDRESS-1:2]][23]? 8'hff:8'b0,mem[a[DM_ADDRESS-1:2]][23:16],16'b0});
				else
					rd = $signed({mem[a[DM_ADDRESS-1:2]][31:24],24'b0});
			end
		else if(funct3 == 3'b001) //LH
			begin
				if(a[1] == 1'b0)
					rd = $signed({mem[a[DM_ADDRESS-1:2]][15]? 16'hffff:16'b0,mem[a[DM_ADDRESS-1:2]][15:0]});
				else if(a[1]==1'b1)
					rd = $signed({mem[a[DM_ADDRESS-1:2]][31:16],16'b0});
			end
		else if(funct3 == 3'b010) //LW
			begin
				rd = $signed(mem[a[DM_ADDRESS-1:2]]);
				if(rd[0]===1'bx)
					rd[7:0] = 8'b0;
				if(rd[8]===1'bx)
					begin
					if(rd[7] == 1'b1)
						rd[15:8] = 8'hFF;
					else
						rd[15:8] = 8'b0;
					end
				if(rd[16]===1'bx)
					begin
					if(rd[15] == 1'b1)
						rd[23:16] = 8'hFF;
					else
						rd[23:16] = 8'b0;
					end
				if(rd[24]===1'bx)
					begin
					if(rd[23] == 1'b1)
						rd[31:24] = 8'hFF;
					else
						rd[31:24] = 8'b0;
					end
			end
		else if(funct3 == 3'b100) //LBU
			begin
				if(a[1:0] == 2'b0)
					rd = {24'b0,mem[a[DM_ADDRESS-1:2]][7:0]};
				else if(a[1:0]==2'b1)
					rd = {16'b0,mem[a[DM_ADDRESS-1:2]][15:8],8'b0};
				else if(a[1:0]==2'b10)
					rd = {8'b0,mem[a[DM_ADDRESS-1:2]][23:16],16'b0};
				else
					rd = {mem[a[DM_ADDRESS-1:2]][31:24],24'b0};
			end
		else if(funct3 == 3'b101) //LHU
			begin
				if(a[1] == 1'b0)
					rd = {16'b0,mem[a[DM_ADDRESS-1:2]][15:0]};
				else if(a[1]==1'b1)
					rd = {mem[a[DM_ADDRESS-1:2]][31:16],16'b0};
			end
		else
			rd = 'b0;
		end
	end
    
    always @(posedge clk) begin
       if (MemWrite)
		begin
		if(funct3 == 3'b000)                                         //SB
			begin
                if(a[1:0] == 2'b0)
                    mem[a[DM_ADDRESS-1:2]][7:0] = wd[7:0];
                else if(a[1:0] == 2'b1)
                    mem[a[DM_ADDRESS-1:2]][15:8] = wd[15:8];
                else if(a[1:0] == 2'b10)
                    mem[a[DM_ADDRESS-1:2]][23:16] = wd[23:16];
                else
                    mem[a[DM_ADDRESS-1:2]][31:24] = wd[31:24];
			end
            else if(funct3 == 3'b001)                                         //SH
			begin
                if(a[1] == 1'b0)
                    mem[a[DM_ADDRESS-1:2]][15:0] = wd[15:0];
                else
                    mem[a[DM_ADDRESS-1:2]][31:16] = wd[31:16];
			end
            else if(funct3 == 3'b010)                                         //SW
                mem[a[DM_ADDRESS-1:2]] = wd;
		else
			mem[a[DM_ADDRESS-1:2]] = mem[a[DM_ADDRESS-1:2]];
           end
    end

`else

  logic we;

    assign we = MemWrite;

  

   SRAM1RW512x32 RAM (

         .A       ( a[8:0] ),

         .CE      ( 1'b1   ),

         .WEB     ( ~we    ),

         .OEB     ( we     ),

         .CSB     ( 1'b0   ),

         .I       ( wd    ),

         .O       ( rd    )

         );
		 


`endif
    
endmodule
