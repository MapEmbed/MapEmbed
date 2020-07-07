//====================================================
// Create Date		: 	2020/04/05 22:02:13
// Design Name		: 	hanyuhao
// Module Name		: 	Dualport_RAM_4_2140
// Project Name		: 	sketch
// Target Devices	: 	xc7vx690tffg1761-2
// Revision 		:	0.01
//====================================================

 
module Dualport_RAM_4_2140(

	input 						clka,			//port a clock	
	input 						clkb, 			//port b clock
	input 						rst_n,			//reset signal,reset low
	input 						ena, 			//port a input data enable
	input 						enb,			//port b input data enable
	input 						wea,			//port a operation type 0: read 1:write	
	input 						web,			//port b operation type 0: read 1:write	
	input 		[11:0]			addra,			//port a operation address
	input 		[11:0]			addrb,			//port b operation address
	input 		[3:0]			dia,			//port a input data
	input 		[3:0]			dib, 			//port b input data
	output 	reg [3:0]			doa, 			//port a output data
	output 	reg 				doa_valid, 		//port a output data valid
	output	reg [3:0]			dob,			//port b output data
	output 	reg 				dob_valid); 	//port b output data valid
 

	reg 		[3:0] 			ram [2139:0];

	reg 		[3:0]			reg_a_1[8:0];
	reg 		[3:0]			reg_a_2[143:0];

	reg 		[3:0]			reg_b_1[8:0];
	reg 		[3:0]			reg_b_2[143:0];

	integer 					i;				//the ram register index
	integer 					j;				//the port a cache1 register index
	integer 					k;				//the port a cache2 register index
	integer 					m;				//the port b cache1 register index
	integer 					n;				//the port b cache2 register index

	reg 						reg_a_1_flag;
	wire 						reg_a_1_flag_wire;
	reg 		[11:0]			reg_addra_1;
	wire 		[11:0] 			reg_addra_1_wire;

	assign  	reg_a_1_flag_wire 	= reg_a_1_flag;
	assign  	reg_addra_1_wire 	= reg_addra_1;

	reg 						reg_a_2_flag;
	wire 						reg_a_2_flag_wire;
	reg 		[11:0]			reg_addra_2;
	wire 		[11:0] 			reg_addra_2_wire;
	assign  	reg_a_2_flag_wire 	= reg_a_2_flag;
	assign  	reg_addra_2_wire 	= reg_addra_2;

	//------------------------------------------------------------
	//the a port can only used to read the register
	always @(posedge clka or posedge rst_n) begin
		if (!rst_n) begin
			//initialize the ram and the cache register
			for(i=0;i<2140;i=i+1)
				ram [i] 				<=	4'b0;
			for(j=0;j<9;j=j+1)
				reg_a_1[j]				<=	4'b0;
			for(k=0;k<144;k=k+1)
				reg_a_2[k]				<=	4'b0;
			reg_a_2[addra[11:4]] 		<= 4'b0;
			reg_a_2_flag 				<= 1'b0;
			reg_addra_2 				<= 12'd0;
		end else begin
			if (ena == 1'b1) begin //read request
				reg_a_2[addra[11:4]] 	<= ram[addra[11:0]];
				reg_a_2_flag 			<= 1'b1;
				reg_addra_2 			<= addra;
			end else begin
				reg_a_2[addra[11:4]] 	<= 4'b0;
				reg_a_2_flag 			<= 1'b0;
				reg_addra_2 			<= 12'd0;
			end
		end
	end

	always @(posedge clka or negedge rst_n) begin 
		if(!rst_n) begin
			reg_a_1[reg_addra_2_wire[11:8]]	<= 4'b0;
			reg_a_1_flag 				<= 1'b0;
			reg_addra_1 				<= 12'd0;
		end else begin
			if (reg_a_2_flag_wire == 1'b1) begin
				reg_a_1[reg_addra_2_wire[11:8]]	<= reg_a_2[reg_addra_2_wire[11:4]];
				reg_a_1_flag 			<= 1'b1;
				reg_addra_1 			<= reg_addra_2;
			end else begin
				reg_a_1[reg_addra_2_wire[11:8]]	<= 4'b0;
				reg_a_1_flag 			<= 1'b0;
				reg_addra_1 			<= 12'd0;
			end
		end
	end

	always @(posedge clka or negedge rst_n) begin 
		if(!rst_n) begin
			doa 						<= 4'b0;
			doa_valid 					<= 1'b0;
		end else begin
			if (reg_a_1_flag_wire == 1'b1) begin
				doa 					<= reg_a_1[reg_addra_1_wire[11:8]];
				doa_valid 				<= 1'b1;
			end else begin
				doa 					<= 4'b0;
				doa_valid 				<= 1'b0;
			end				
		end
	end

	//-----------------------------------------------------------------
	//the b port can be used to read and write the register
	localparam 	b_idle_s 			= 4'd0,		//idle state
				b_read_1c_s 		= 4'd1,		//read the register step1
				b_read_2c_s 		= 4'd2,		//read the register step2
				b_write_1c_s		= 4'd3,		//write the register step1
				b_write_2c_s		= 4'd4,		//write the register step2
				b_wait_s			= 4'd5; 	//wait 1 cycle

	reg [3:0]	current_state_b;

	always @(posedge clkb or posedge rst_n) begin
		if (!rst_n) begin						//initialize the port b cache1 and cache2 register
			dob						<= 	4'd0;
			dob_valid				<=	1'b0;
			for(m=0;m<9;m=m+1)
				reg_b_1[m]			<=	4'b0;
			for(n=0;n<144;n=n+1)
				reg_b_2[n]			<=	4'b0;
			current_state_b			<= 	b_idle_s;	
		end
		else  begin
			case(current_state_b)
				b_idle_s:begin
					if (enb == 1'b1 && web == 1'b0) begin 			//read the register
						reg_b_2[addrb[11:4]] 	<= ram[addrb[11:0]];//get the ram data to cache 2
						dob 					<= 4'b0;
						dob_valid				<= 1'b0;
						current_state_b			<= b_read_1c_s;
					end else if (enb == 1'b1 && web == 1'b1) begin 	//write the  register
						reg_b_1[addrb[11:8]] 	<= dib;				//put the input data to cache 1
						dob 					<= 4'b0;	
						dob_valid				<= 1'b0;
						current_state_b			<= b_write_1c_s;
					end else begin									//no operation request
						dob 					<= 4'b0;
						dob_valid				<= 1'b0;
						current_state_b			<= b_idle_s;
					end
				end
				b_read_1c_s:begin 									//get the ram data to cache 1
					reg_b_1[addrb[11:8]]		<= reg_b_2[addrb[11:4]];
					current_state_b 			<= b_read_2c_s;
				end
				b_read_2c_s:begin									//get the ram data to output
					dob 						<= reg_b_1[addrb[11:8]];
					dob_valid					<= 1'b1;
					current_state_b 			<= b_wait_s;
				end
				b_write_1c_s:begin									//put the input data to cache 2
					reg_b_2[addrb[11:4]] 		<= reg_b_1[addrb[11:8]];
					dob 						<= 4'b0;
					dob_valid					<= 1'b0;
					current_state_b				<= b_write_2c_s;
				end
				b_write_2c_s:begin									//put the input data to ram
					ram[addrb[11:0]]			<= reg_b_2[addrb[11:4]];
					dob_valid					<= 1'b1;
					current_state_b				<= b_wait_s;
				end
				b_wait_s:begin										//wait 1 cycle
					dob 						<= 4'b0;
					dob_valid					<= 1'b0;
					current_state_b				<= b_idle_s;
				end
				default:begin
					dob 						<= 4'b0;
					dob_valid					<= 1'b0;
					current_state_b				<= b_idle_s;
				end
			endcase		
		end
	end

endmodule
