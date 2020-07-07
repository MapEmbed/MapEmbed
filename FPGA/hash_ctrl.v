`timescale 1ns / 1ps
//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	hash_ctrl
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================


module hash_ctrl(

	input 						Sys_clk,
	input 						Rst_n,

	input 						Hash_valid,
	//----------------------------------------
	input 	[11:0]				Hash_add1,	

	output 	[3:0]				Hash_rdata1,
	output						Hash_rdata1_wr,

	input						enb_1,
	input						web_1,
	input	[11:0]				addrb_1,
	input 	[3:0]				dib_1,
	output 						dob_1,
	output 						dob_valid_1,
	//-------------------------------------------
	input 	[10:0]				Hash_add2,	
	output 	[3:0]				Hash_rdata2,
	output						Hash_rdata2_wr,

	input						enb_2,
	input						web_2,
	input	[22:0]				addrb_2,
	input 	[3:0]				dib_2,
	output 						dob_2,
	output 						dob_valid_2,

	//--------------------------------------------
	input 	[9:0]				Hash_add3,	
	output 	[3:0]				Hash_rdata3,
	output						Hash_rdata3_wr,

	input						enb_3,
	input						web_3,
	input	[22:0]				addrb_3,
	input 	[3:0]				dib_3,
	output 						dob_3,
	output 						dob_valid_3

    );
	
	//--------------------------------------------------------------------
	//the signal to read ram1 by a port
	wire 				ena_1;
	wire 				wea_1;
	wire 	[11:0] 		addra_1;

	assign ena_1 		= (Hash_valid == 1'b1) ? 1'b1:1'b0;
	assign wea_1 		= 1'b0;
	assign addra_1		= Hash_add1;

	//--------------------------------------------------------------------
	//the signal to read ram2 by a port
	wire 				ena_2;
	wire 				wea_2;
	wire 	[10:0] 		addra_2;

	assign ena_2 		= (Hash_valid == 1'b1) ? 1'b1:1'b0;
	assign wea_2 		= 1'b0;
	assign addra_2		= Hash_add2;

	//--------------------------------------------------------------------
	//the signal to read ram3 by a port
	wire 				ena_3;
	wire 				wea_3;
	wire 	[9:0] 		addra_3;

	assign ena_3 		= (Hash_valid == 1'b1) ? 1'b1:1'b0;
	assign wea_3 		= 1'b0;
	assign addra_3		= Hash_add3;
	
    //hash table 1
	Dualport_RAM_4_2140 Dualport_RAM_4_2140_00(
	.clka 				(Sys_clk		),
	.clkb 				(Sys_clk		),
	.rst_n 				(rst_n 			),
	.ena 				(ena_1 			),
	.enb 				(enb_1 			),
	.wea 				(wea_1 			),
	.web 				(web_1 			),
	.addra 				(addra_1 		),
	.addrb 				(addrb_1 		),
	.dia 				(4'b0 			),
	.dib 				(dib_1 			),
	.doa 				(Hash_rdata1 	),
	.doa_valid 			(Hash_rdata1_wr	),
	.dob 				(dob_1 			),
	.dob_valid 			(dob_valid_1	));
	
    //hash table 2
	Dualport_RAM_4_1070 Dualport_RAM_4_1070_00(
	.clka 				(Sys_clk 		),
	.clkb 				(Sys_clk 		),
	.rst_n 				(Rst_n 			),
	.ena 				(ena_2 			),
	.enb 				(enb_2 			),
	.wea 				(wea_2 			),
	.web 				(web_2 			),
	.addra 				(addra_2 		),
	.addrb 				(addrb_2 		),
	.dia 				(4'b0 			),
	.dib 				(dib_2 			),
	.doa 				(Hash_rdata2 	),
	.doa_valid 			(Hash_rdata2_wr ),
	.dob 				(dob_2 			),
	.dob_valid 			(dob_valid_2 	));

	//hash table 3
	Dualport_RAM_4_535 Dualport_RAM_4_53500(
	.clka 				(Sys_clk  		),
	.clkb 				(Sys_clk 		),
	.rst_n 				(Rst_n 			),
	.ena 				(ena_3 			),
	.enb 				(enb_3 			),
	.wea 				(wea_3 			),
	.web 				(web_3 			),
	.addra 				(addra_3 		),
	.addrb 				(addrb_3 		),
	.dia 				(4'b0 			),
	.dib 				(dib_3 			),
	.doa 				(Hash_rdata3 	),
	.doa_valid 			(Hash_rdata3_wr	),
	.dob 				(dob_3 			),
	.dob_valid 			(dob_valid_3 	));


endmodule
