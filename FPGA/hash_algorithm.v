//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	hash_algorithm
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================


module hash_algorithm(

	input 						sys_clk,		//system clock,125Mhz
	input						rst_n,			//reset signal,reset low

	input 		[31:0]			Key,			//key
	input 						Key_in,			//key_in

	output reg	[11:0]			Hash_add1,		//Hash Address1
	output reg	[10:0]			Hash_add2,		//Hash Address2
	output reg	[9:0]			Hash_add3,		//Hash Address3
	output reg 					Hash_valid		//Hash valid
    );

	
	always @(posedge sys_clk or negedge rst_n) begin 
		if(~rst_n) begin
			Hash_add1		<=	12'b0;
			Hash_add2		<=	11'b0;
			Hash_add3		<=	10'b0;
			Hash_valid 		<= 	1'b0;
		end else begin
			if (Key_in == 1'b1) begin
			 	Hash_add1		<=	( Key[31:16] & Key[15:0] ) % 2140; //hash algorithm 1
				Hash_add2		<=	( Key[31:12] & Key[19:0] ) % 1070; //hash algorithm 2
				Hash_add3		<=	( Key[31:8]  & Key[23:0] ) % 535; //hash algorithm 3
				Hash_valid 		<= 	1'b1; 
			end else begin
			 	Hash_add1		<=	12'b0;
				Hash_add2		<=	11'b0;
				Hash_add3		<=	10'b0;
				Hash_valid 		<= 	1'b0; 
			end
		end
	end

endmodule
