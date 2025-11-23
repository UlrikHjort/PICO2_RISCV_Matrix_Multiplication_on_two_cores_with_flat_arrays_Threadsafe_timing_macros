# PICO 2 Multicore matrix multiplication using flat arrays

Embarrassingly parallel matrix multiplication using the PICO2's two RISCV cores
(should work on the M33 cores as well, but not tested there). The rows are split
50-50 on the two cores. flat arrays are used.

The timing macros detect which core they are called on and keep a seperate counter
for eeach core. 

NOTE: The matrix multiplication here is not fully efficient across two cores because both cores share the same system bus and SRAM. 
During the heavy memory access here , the bus arbitration logic  causing contention and stalls. 
This reduces effective memory bandwidth and limits the expected speed-up.



	   
