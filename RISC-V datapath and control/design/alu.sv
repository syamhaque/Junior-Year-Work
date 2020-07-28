`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/07/2018 10:23:43 PM
// Design Name: 
// Module Name: alu
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


module alu#(
        parameter DATA_WIDTH = 32,
        parameter OPCODE_LENGTH = 4
        )(
        input logic [DATA_WIDTH-1:0]    SrcA,
        input logic [DATA_WIDTH-1:0]    SrcB,

        input logic [OPCODE_LENGTH-1:0]    Operation,
	output logic Zero,
        output logic[DATA_WIDTH-1:0] ALUResult
        );
    
        always_comb
        begin
            case(Operation)
            4'b0000:        // AND
			begin
                    ALUResult = SrcA & SrcB;
		    		Zero = 0;
			end
            4'b0001:        //OR
			begin
                    ALUResult = SrcA | SrcB;
		    		Zero = 1'b0;
			end
            4'b0010:        //ADD
		    begin
                    ALUResult = SrcA + SrcB;
		     		Zero = 1'b1;
		    end
	    4'b0011:        //XOR
			begin
	            ALUResult=SrcA^SrcB;
		    		Zero = 1'b0;
			end
	    4'b0100:
			begin
		    		ALUResult = SrcB;
		    		Zero = 1'b0;
			end
            4'b0110:        //Subtract
			begin
                    ALUResult = $signed(SrcA) - $signed(SrcB);
		    		Zero = 1'b0;
			end
	    4'b0111:
			begin
		    ALUResult = (SrcA == SrcB);
			Zero = ALUResult[0]; //(SrcA == SrcB);
			end
	    4'b1000:
			begin
		    
		    ALUResult = (SrcA != SrcB);
			Zero = ALUResult[0]; //(SrcA != SrcB);
			end
	    4'b1001:
			begin
		    ALUResult = ($signed(SrcA)<$signed(SrcB));
		    Zero = ALUResult[0];//($signed(SrcA)<$signed(SrcB));
			end
	    4'b1010:
			begin
		    		ALUResult = ($signed(SrcA)>=$signed(SrcB));
		    		Zero = ALUResult[0];//($signed(SrcA)>=$signed(SrcB));
			end
	    4'b1011:
			begin
		    		ALUResult = (SrcA<SrcB);
		    		Zero = ALUResult[0];
			end
	    4'b1100:
			begin
	    			ALUResult = (SrcA>=SrcB);
		    		Zero = ALUResult[0];
			end
	    4'b1101:
		    ALUResult = SrcA << SrcB[4:0];
		    //Zero = 0; 
	    4'b1110:
		    ALUResult = SrcA >> SrcB[4:0];
		    //Zero = 0;
	    4'b1111:
		    ALUResult = $signed(SrcA) >>> SrcB[4:0];
		    //Zero = 0;
		default:
			ALUResult = ALUResult;
            endcase
        end
endmodule

