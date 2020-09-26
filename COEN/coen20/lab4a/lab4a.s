//Solving Quadratics
//Daren Liu

        .syntax		unified
		.cpu		cortex-m4
		.text
	
	// int32_t Discriminant(int32_t a, int32_t b, int32_t c);
		.global		Discriminant
		.thumb_func
	Discriminant:
		// TODO: Write your Assembly code for the 'Discriminant' function.
		mul r1, r1, r1 //r1 = b^2
		lsl r0, r0, 2 //r0 = 4a
		mls r0, r0, r2, r1 // r0 = b^2 - 4ac
		bx lr
	
	// int32_t Root1(int32_t a, int32_t b, int32_t c);
		.global		Root1
		.thumb_func
	Root1:
		// TODO: Write your Assembly code for the 'Root1' function.
		push {r4, r5, r6, lr}
		mov r4, r0 //r4 = a 
		mov r5, r1 //r5 = b 
		ldr r6 ,= -1 //r6 = -1
		bl Discriminant
		bl SquareRoot //r0 = SquareRoot(Discriminant())
		mul r1, r5, r6  //r1 = -b 
		add r1, r1, r0 //r1 = -b + squareroot(Discriminant)
		ldr r6 ,= 2 // r6 = 2
		mul r0, r4, r6 //r0 = 2a 
		sdiv r0, r1, r0 //r0 = r1/r0 
		pop {r4, r5, r6, pc}
	
	// int32_t Root2(int32_t a, int32_t b, int32_t c);
		.global		Root2
		.thumb_func
	Root2:
		// TODO: Write your Assembly code for the 'Root2' function.
		push {r4, r5, r6, lr}
		mov r4, r0 //r4 = a 
		mov r5, r1 //r5 = b 
		ldr r6 ,= -1 //r6 = -1
		bl Discriminant
		bl SquareRoot //r0 = SquareRoot(Discriminant())
		mul r1, r5, r6  //r1 = -b 
		sub r1, r1, r0 //r1 = -b - squareroot(Discriminant)
		ldr r6 ,= 2 //r6 = 2
		mul r0, r4, r6 //r0 = 2a 
		sdiv r0, r1, r0 //r0 = r1/r0 
		pop {r4, r5, r6, pc}
	
	// int32_t Quadratic(int32_t x, int32_t a, int32_t b, int32_t c);
		.global		Quadratic
		.thumb_func
	Quadratic:
		// TODO: Write your Assembly code for the 'Quadratic' function.
		mov r4, r0 //r4 = x 
		mov r5, r1 //r5 = a 
		mul r2, r2, r0 //r2 = bx
		add r3, r2, r3 //r3 = bx + c 
		mul r0, r0, r0 //r0 = x^2
		mul r0, r1, r0 //r0 = ax^2
		add r0, r0, r3 //r0 = r0 + r3
		bx lr 
		.end
