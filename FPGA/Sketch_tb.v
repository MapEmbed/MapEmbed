`timescale 1ns / 1ps
//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	Sketch_tb
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================


module Sketch_tb();

	//-----------------------------------------
	//the clock signal, Sys_clk:125Mhz
	reg 		Sys_clk;
	parameter 	ClocksysPeriod = 8 ;

	initial begin
   		Sys_clk = 0;
  		forever begin
  			# (ClocksysPeriod/2) Sys_clk = ~Sys_clk ;
  		end
    	
 	end

 	//-----------------------------------------
 	//the reset signal ,active low
	reg rst_n;
	initial
   		begin
     		rst_n = 1 ;
     		#100 ;
    		rst_n = 0;
    		#600 ;
    		rst_n = 1;
   end

    //-----------------------------------------
   	//the data to next module
	wire [3:0] 			Hash_rdata_MUX_o;
	wire  				Hash_rdata_MUX_o_wr;

	//-----------------------------------------
   	//the input key
	reg [31:0] 			Key;
	reg 				Key_in;

	integer i;
	initial begin
		Key 					= 	32'd0;
		Key_in 					= 	1'b0;	
		#150004
		Key 			=	$random;
		Key_in 			= 	1'b1;
		#8
		Key 			=	$random;
		Key_in 			= 	1'b1;
		#8
		Key 			=	$random;
		Key_in 			= 	1'b1;
		#8
		Key 			=	$random;
		Key_in 			= 	1'b1;
		#8
		Key 			=	$random;
		Key_in 			= 	1'b1;
		#8
		Key 					= 	32'd0;
		Key_in 					= 	1'b0;
	end
   
	//-----------------------------------------------------
   	//intilize the ram1 
	reg					enb_1;
	reg					web_1;
	reg		[11:0]		addrb_1;
	reg		[3:0]		dib_1;
	wire 	[3:0]		dob_1;
	wire 				dob_valid_1;
	//intilize the ram2 
	reg 				enb_2;
	reg 				web_2;
	reg 	[10:0]		addrb_2;
	reg 	[3:0]		dib_2;
	wire 	[3:0]		dob_2;
	wire 				dob_valid_2;
	//intilize the ram3
	reg 				enb_3;
	reg 				web_3;
	reg 	[9:0]		addrb_3;
	reg 	[3:0]		dib_3;
	wire 	[3:0]		dob_3;
	wire 				dob_valid_3;


	//the counter to start to control the process
	reg		[15:0] 		counter;
	always @(posedge Sys_clk or negedge rst_n) begin
		if (!rst_n) begin
			counter 		<= 16'd0;		
		end
		else begin
			counter 		<= counter + 16'd1;		
		end
	end

	//the logic to initiliaze the ram1
	reg	[11:0]			Addressb_1_reg;

	localparam 			idle_s 		= 4'd0,
						initial_s 	= 4'd1;

	reg	[3:0]			cunrrent_state;

   always @(posedge Sys_clk or negedge rst_n) begin
   	if (!rst_n) begin
   		enb_1 			<= 1'b0;
		web_1 			<= 1'b0;
		addrb_1 		<= 12'b0;
		dib_1 			<= 4'b0;	
		Addressb_1_reg	<= 12'd0;	
		cunrrent_state	<= idle_s;
   		
   	end
   	else begin
   		case(cunrrent_state)
   			idle_s:begin
   				if (counter < 16'd10) begin //wait
   					enb_1 			<= 1'b0;
					web_1 			<= 1'b0;
					addrb_1 		<= 12'b0;
					dib_1 			<= 4'b0;
					Addressb_1_reg	<= 12'd0;
					cunrrent_state	<= idle_s;
   				end else begin
   					if (Addressb_1_reg <= 16'd2139) begin
   						enb_1 			<= 1'b1;
						web_1 			<=  1'b1;
						addrb_1 		<= Addressb_1_reg;
						Addressb_1_reg 	<= Addressb_1_reg + 16'd1;
						dib_1 			<= $random % 15;
						cunrrent_state	<= initial_s;
   					end else begin
   						enb_1 			<= 1'b0;
						web_1 			<=  1'b0;
						addrb_1 		<= 12'b0;
						Addressb_1_reg 	<= Addressb_1_reg;
						dib_1 			<= 4'b0;
						cunrrent_state	<= idle_s;
   					end
   				end   				
   			end
   			initial_s:begin
   				if (dob_valid_1 == 1'b1) begin //the write complete
   					enb_1 			<= 1'b0;
					web_1 			<= 1'b0;
					addrb_1 		<= 16'd0;
					dib_1 			<= 4'd0;
					cunrrent_state	<= idle_s;
   				end else begin
   					cunrrent_state	<= initial_s;
   				end
   				
   			end
   			default:begin
   				enb_1 			<= 1'b0;
				web_1 			<= 1'b0;
				addrb_1 		<= 12'b0;
				dib_1 			<= 4'b0;
				Addressb_1_reg	<= 12'd0;
				cunrrent_state	<= idle_s;
   			end
   		endcase 		
   	end
   end


//the logic to initiliaze the ram2
	reg	[10:0]					Addressb_2_reg;
	localparam 		idle_2_s 		= 4'd0,
					initial_2_s 	= 4'd1;

	reg		[3:0]	cunrrent_state_2;

    always @(posedge Sys_clk or negedge rst_n) begin
   	if (!rst_n) begin
   		enb_2 			<= 1'b0;
		web_2 			<= 1'b0;
		addrb_2 		<= 11'b0;
		dib_2 			<= 4'b0;	
		Addressb_2_reg	<= 11'd0;	
		cunrrent_state_2	<= idle_2_s;
   		
   	end
   	else begin
   		case(cunrrent_state_2)
   			idle_2_s:begin
   				if (counter < 16'd10) begin //wait
   					enb_2 			<= 1'b0;
					web_2 			<= 1'b0;
					addrb_2 		<= 11'b0;
					dib_2 			<= 4'b0;
					Addressb_2_reg	<= 11'd0;
					cunrrent_state_2	<= idle_2_s;
   				end else begin
   					if (Addressb_2_reg <= 16'd1069) begin
   						enb_2 			<= 1'b1;
						web_2 			<=  1'b1;
						addrb_2 		<= Addressb_2_reg;
						Addressb_2_reg 	<= Addressb_2_reg + 16'd1;
						dib_2 			<= $random % 15;
						cunrrent_state_2	<= initial_2_s;
   					end else begin
   						enb_2 			<= 1'b0;
						web_2 			<=  1'b0;
						addrb_2 		<= 11'b0;
						Addressb_2_reg 	<= Addressb_2_reg;
						dib_2 			<= 4'b0;
						cunrrent_state_2	<= idle_2_s;
   					end
   				end   				
   			end
   			initial_2_s:begin
   				if (dob_valid_2 == 1'b1) begin //the write complete
   					enb_2 			<= 1'b0;
					web_2 			<= 1'b0;
					addrb_2 		<= 16'd0;
					dib_2 			<= 4'd0;
					cunrrent_state_2	<= idle_2_s;
   				end else begin
   					cunrrent_state_2	<= initial_2_s;
   				end
   				
   			end
   			default:begin
   				enb_2 			<= 1'b0;
				web_2 			<= 1'b0;
				addrb_2 		<= 11'b0;
				dib_2 			<= 4'b0;
				Addressb_2_reg	<= 11'd0;
				cunrrent_state_2	<= idle_2_s;
   			end
   		endcase 		
   	end
   end
   //the logic to initiliaze the ram2
	reg	[9:0]					Addressb_3_reg;
	localparam 		idle_3_s 		= 4'd0,
					initial_3_s 	= 4'd1;

	reg		[3:0]	cunrrent_state_3;

   always @(posedge Sys_clk or negedge rst_n) begin
   	if (!rst_n) begin
   		enb_3 			<= 1'b0;
		web_3 			<= 1'b0;
		addrb_3 		<= 11'b0;
		dib_3 			<= 4'b0;	
		Addressb_3_reg	<= 11'd0;	
		cunrrent_state_3	<= idle_3_s;
   		
   	end
   	else begin
   		case(cunrrent_state_3)
   			idle_3_s:begin
   				if (counter < 16'd10) begin //wait
   					enb_3 			<= 1'b0;
					web_3 			<= 1'b0;
					addrb_3 		<= 11'b0;
					dib_3 			<= 4'b0;
					Addressb_3_reg	<= 11'd0;
					cunrrent_state_3	<= idle_3_s;
   				end else begin
   					if (Addressb_3_reg <= 16'd534) begin
   						enb_3 			<= 1'b1;
						web_3 			<=  1'b1;
						addrb_3 		<= Addressb_3_reg;
						Addressb_3_reg 	<= Addressb_3_reg + 16'd1;
						dib_3 			<= $random % 15;
						cunrrent_state_3	<= initial_3_s;
   					end else begin
   						enb_3 			<= 1'b0;
						web_3 			<=  1'b0;
						addrb_3 		<= 11'b0;
						Addressb_3_reg 	<= Addressb_3_reg;
						dib_3 			<= 4'b0;
						cunrrent_state_3	<= idle_3_s;
   					end
   				end   				
   			end
   			initial_3_s:begin
   				if (dob_valid_3 == 1'b1) begin //the write complete
   					enb_3 			<= 1'b0;
					web_3 			<= 1'b0;
					addrb_3 		<= 16'd0;
					dib_3 			<= 4'd0;
					cunrrent_state_3	<= idle_3_s;
   				end else begin
   					cunrrent_state_3	<= initial_3_s;
   				end
   				
   			end
   			default:begin
   				enb_3 			<= 1'b0;
				web_3 			<= 1'b0;
				addrb_3 		<= 11'b0;
				dib_3 			<= 4'b0;
				Addressb_3_reg	<= 11'd0;
				cunrrent_state_3	<= idle_3_s;
   			end
   		endcase 		
   	end
   end

   //instance the Sketch_top
	Sketch_top Sketch00(
	.sys_clk 		(Sys_clk 		),
	.rst_n 			(rst_n 			),

	.Key 			(Key 			),
	.Key_in 		(Key_in 		),

	//the ram configuration interface
	.enb_1 			(enb_1 			),
	.web_1 			(web_1 			),
	.addrb_1 		(addrb_1 		),
	.dib_1 			(dib_1 			),
	.dob_1 			(dob_1 			),
	.dob_valid_1 	(dob_valid_1 	),

	.enb_2 			(enb_2 			),
	.web_2 			(web_2 			),
	.addrb_2 		(addrb_2 		),
	.dib_2 			(dib_2 			),
	.dob_2 			(dob_2 			),
	.dob_valid_2	(dob_valid_2 	),

	.enb_3 			(enb_3 			),
	.web_3 			(web_3 			),
	.addrb_3 		(addrb_3 		),
	.dib_3 			(dib_3 			),
	.dob_3 			(dob_3 			),
	.dob_valid_3 	(dob_valid_3 	),

	//the data to next module
	.Hash_rdata 	(Hash_rdata_MUX_o),
	.Hash_rdata_wr 	(Hash_rdata_MUX_o_wr)
    );

endmodule
