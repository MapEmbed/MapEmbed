# FPGA MapEmbed


## Introduction

We implement the lookup module of MapEmbed on an FPGA network experimental platform (Virtex-7 VC709). The FPGA integrated with the platform is xc7vx690tffg1761-2 with 433200 Slice LUTs, 866400 Slice Register, and 850 Block RAM Tile (i.e., 30.6Mb on-chip memory). The implementation mainly consists of three hardware modules: calculating hash values (calculating), searching the table (searching), and writing the matched results (writing). FPGA-based MapEmbed is fully pipelined, which can input one key in every clock, and output one result after four clocks. 
According to the synthesis report, the clock frequency of our implementation in FPGA is 367 MHz, meaning the throughput of the system can be 367 Mips. Moreover, the logic resource usage is 0.98%, and memory resource usage is 0.3%.


## Overall Design

  			hash_algorithm	---->  hash_ctrl ----> MUX

- **hash\_algorithm**: Recieve the key (32bit) and calculate the hash using three different hash algorithms, the widths of outptut hash are 12bit, 11bit and 10bit, respectively.
- **hash\_ctrl**: The top module of the lookup table logic, including Dualport\_RAM\_4\_535, Dualport\_RAM\_4\_1070, and Dualport\_RAM\_4\_2140 modules. Revcieve three different hash from hash_algorithm. Then we use 12bit hash as the address to query Dualport\_RAM\_4\_2140, use 11bit hash as the address to query Dualport\_RAM\_4\_2140, use 10bit hash as the address to query Dualport\_RAM\_4\_535. Then, we send the results of the table lookup to the mux module. 
- **Dualport\_RAM\_4\_1070**: The dual port ram is achieved by the register in FPGA, which width is 4bit and depth is 535. To support higher processing bandwidth, the Ram supports full pipeline query.
- **Dualport\_RAM\_4\_1070**: The dual port ram is achieved by the register in FPGA, which width is 4bit and depth is 1070. To support higher processing bandwidth, the Ram supports full pipeline query. 
- **Dualport\_RAM\_4\_2140**: The dual port ram is achieved by the register in FPGA,which width is 4bit and depth is 2140. To support higher processing bandwidth,the Ram supports full pipeline query.
- **Mux**: Recieve the results of the table lookup and choose one result as the output according some rule.
- **Sketch\_top**: The top-level module of the project.
- **Sketch\_tb**: The testbench of the project.


## Our experiments setups

- compile: `Vivado 2017.04`
- language: `Verilog`
- FPGA version: `virtes-7 xc7vx690tffg1761-2`
- frequency: `Fmax is 367.36Mhz.`

