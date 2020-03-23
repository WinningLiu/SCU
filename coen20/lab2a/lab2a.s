    //Lab 2a: Functions and Parameters
	//Daren Liu

        .syntax     unified
        .cpu        cortex-m4
        .text
   
    // int32_t Add(int32_t a, int32_t b);
        .global     Add
        .thumb_func
Add:
        //code here
		//R0 = a, R1 = b
	ADD R0, R0, R1 //Adds a and b to R0
	BX LR
   
    // int32_t Less1(int32_t a);
        .global     Less1
        .thumb_func
Less1:
        //code here
		//R0 = a
	SUB R0, R0, 1 //Subtracts a and 1 to R0
	BX LR
   
    // int32_t Square2x(int32_t x);
    .global     Square2x
    .thumb_func
Square2x:
        //code here
		//R0 = x
	ADD R0, R0, R0 //Adds x and x to R0
	B Square //Returns Square(x+x)
   
    // int32_t Last(int32_t x);
    .global     Last
    .thumb_func
Last:
    //code here
	//R0 = x
	PUSH {R4, LR} 
	MOV R4, R0 //Copies x to R4
	BL SquareRoot //Calls SquareRoot
	ADD R0, R0, R4 // Adds x and SquareRoot(x)
	POP	{R4, PC}
		
    .end