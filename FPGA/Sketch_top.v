`timescale 1ns / 1ps
//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	Sketch
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================


module Sketch_top(
	//clock and reset signal
	input 						sys_clk,		//system clock
	input						rst_n,			//reset low	

	//the input key
	input 	[31:0]				Key,			//key
	input 						Key_in,			//key valid

	//ram configuration interface
	input 						enb_1,			//ram1 config data enable
	input 						web_1,			//ram1 operation type :0 read; 1 wirite
	input 	[11:0]				addrb_1,		//ram1 address
	input 	[3:0]				dib_1,			//ram1 input data
	output	[3:0]				dob_1,			//ram1 output data
	output 		 				dob_valid_1,	//ram1 output data valid

	input 						enb_2,			//ram2 config data enable
	input 						web_2,			//ram2 operation type :0 read; 1 wirite
	input 	[11:0]				addrb_2,		//ram2 address
	input 	[3:0]				dib_2,			//ram2 input data
	output	[3:0]				dob_2,			//ram2 output data
	output 		 				dob_valid_2,	//ram2 output data valid

	input 						enb_3,			//ram3 config data enable
	input 						web_3,			//ram3 operation type :0 read; 1 wirite
	input 	[11:0]				addrb_3,		//ram3 address
	input 	[3:0]				dib_3,			//ram3 input data
	output	[3:0]				dob_3,			//ram3 output data
	output 		 				dob_valid_3,	//ram3 output data valid

	//the result of the algorithm
	output 	[3:0]				Hash_rdata, 	//result
	output 						Hash_rdata_wr 	//result valid 
	
    );

	//the signal between hash_algorithm module and hash_ctrl module
	wire	[11:0]				Hash_add1;		//Hash Address1
	wire	[10:0]				Hash_add2;		//Hash Address2
	wire	[9:0]				Hash_add3;		//Hash Address3
	wire						Hash_valid; 	//Hash valid

	//the signal between hash_ctrl and MUX
	wire	[3:0]				Hash_rdata1;	//Table lookup results of hash table 1
	wire						Hash_rdata1_wr;	//result valid
	wire	[3:0]				Hash_rdata2;	//Table lookup results of hash table 1
	wire						Hash_rdata2_wr;	//result valid
	wire	[3:0]				Hash_rdata3;	//Table lookup results of hash table 1
	wire						Hash_rdata3_wr; //result valid

	hash_algorithm hash_algorithm00(
	.sys_clk 					(sys_clk 	),	
	.rst_n 						(rst_n  	),
	//-------------------------------------------	
	.Key 						(Key 		),	
	.Key_in 					(Key_in 	),	
	//-------------------------------------------
	.Hash_add1 					(Hash_add1 	),	
	.Hash_add2 					(Hash_add2 	),	
	.Hash_add3 					(Hash_add3 	),	
	.Hash_valid 				(Hash_valid	)	
    );

	hash_ctrl hash_ctrl00(
	.Sys_clk 					(sys_clk 	),
	.Rst_n 						(rst_n 		),

	.Hash_valid					(Hash_valid	),
	//-------------------------------------------
	.Hash_add1 					(Hash_add1	),
	.Hash_rdata1 				(Hash_rdata1),
	.Hash_rdata1_wr 			(Hash_rdata1_wr),	
	.enb_1 						(enb_1 		),
	.web_1 						(web_1 		),
	.addrb_1 					(addrb_1 	),
	.dib_1 						(dib_1 		),
	.dob_1 						(dob_1 		),
	.dob_valid_1 				(dob_valid_1),
	//-------------------------------------------
	.Hash_add2 					(Hash_add2 	),	
	.Hash_rdata2 				(Hash_rdata2),
	.Hash_rdata2_wr 			(Hash_rdata2_wr),
	.enb_2 						(enb_2 		),
	.web_2 						(web_2 		),
	.addrb_2 					(addrb_2 	),
	.dib_2 						(dib_2 		),
	.dob_2 						(dob_2 		),
	.dob_valid_2 				(dob_valid_2),
	//------------------------------------------
	.Hash_add3 					(Hash_add3 	),	
	.Hash_rdata3 				(Hash_rdata3),
	.Hash_rdata3_wr 			(Hash_rdata3_wr),
	.enb_3 						(enb_3 		),
	.web_3 						(web_3 		),
	.addrb_3 					(addrb_3 	),
	.dib_3 						(dib_3 		),
	.dob_3 						(dob_3 		),
	.dob_valid_3 				(dob_valid_3)
	);

	MUX MUX00(
	.Sys_clk 					(sys_clk 	),
	.Rst_n 						(rst_n 		),
	//-------------------------------------------
	.Hash_rdata1 				(Hash_rdata1),
	.Hash_rdata1_wr 			(Hash_rdata1_wr),
	//-------------------------------------------
	.Hash_rdata2 				(Hash_rdata2),
	.Hash_rdata2_wr 			(Hash_rdata2_wr),
	//-------------------------------------------
	.Hash_rdata3 				(Hash_rdata3),
	.Hash_rdata3_wr 			(Hash_rdata3_wr),
	//-------------------------------------------
	.Hash_rdata_MUX_o 			(Hash_rdata ),
	.Hash_rdata_MUX_o_wr 		(Hash_rdata_wr)

    );
endmodule
