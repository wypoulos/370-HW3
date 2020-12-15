Wyatt Poulos

Hello and welcome to my ted talk about HW3

**** Set UP ****
Unpack the .zip file where you would like to use it and open the command prompt in that directory

**** COMPILING ****
For best results please use on the linux machines at CSU
step 1: type make in the command terminal an press enter
Done!

**** RUNNING ****
Step 1: type coordinator into the command terminal followed by a space
Step 2: enter the divisor followed by a space 
Step 3: enter each dividend followed by a space until your done entering dividends
Step 4: press enter
Done!

**** More ****
For more information on how the code works each file has discriptions of what the different chunks of code does

Unlike homework 2 this home work works by using pipes and shared memory. The first step is to fork and create a pipe next is to create a shared
memory space and pipe it to the child process. The child process then puts a 1 or 0 in to the shared memory and coordinator then outputs
the result. 