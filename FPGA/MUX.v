//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	MUX
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================


module MUX(
	//clock and reset signal 
	input 					Sys_clk,			//system clock,125Mhz
	input 					Rst_n,				//reset signal,reset low

	//Look-up results of hash table 1
	input 		[3:0]		Hash_rdata1,		//result data
	input					Hash_rdata1_wr,		//result enable
	//Look-up results of hash table 2	
	input 		[3:0]		Hash_rdata2,		//result data
	input					Hash_rdata2_wr,		//result enable
	//Look-up results of hash table 3
	input 		[3:0]		Hash_rdata3,		//result data
	input					Hash_rdata3_wr,		//result enable

	//the data to next module
	output 	reg	[3:0]		Hash_rdata_MUX_o,	//the choosed result data
	output 	reg				Hash_rdata_MUX_o_wr	//data enable

    );
	
	wire 					If_hashdata1_f;
	wire 					If_hashdata2_f;
	wire 					If_hashdata3_f;
	assign  If_hashdata1_f 	= (Hash_rdata1 == 4'd15) ? 1'b0 :1'b1;
	assign  If_hashdata2_f  = (Hash_rdata2 == 4'd15) ? 1'b0 :1'b1;
	assign  If_hashdata3_f  = (Hash_rdata3 == 4'd15) ? 1'b0 :1'b1;

	always @(posedge Sys_clk or negedge Rst_n) begin
		if(~Rst_n) begin
			 Hash_rdata_MUX_o 			<= 4'b0;
			 Hash_rdata_MUX_o_wr 		<= 1'b0;
		end 
		else begin
			 if (Hash_rdata1_wr == 1'b1) begin
			 	if (If_hashdata1_f == 1'b0) begin
			 		Hash_rdata_MUX_o 	<= Hash_rdata1;
			 		Hash_rdata_MUX_o_wr	<= 1'b1;
			 	end
			 	else if (If_hashdata2_f == 1'b0) begin
			 		Hash_rdata_MUX_o 	<= Hash_rdata2;
			 		Hash_rdata_MUX_o_wr	<= 1'b1;
			 	end
			 	else begin
			 		Hash_rdata_MUX_o 	<= Hash_rdata3;
			 		Hash_rdata_MUX_o_wr	<= 1'b1;
			 	end
			 end
			 else begin
			 	Hash_rdata_MUX_o 		<= 4'b0;
			 	Hash_rdata_MUX_o_wr		<= 1'b0;
			 end
		end
	end
	
endmodule
