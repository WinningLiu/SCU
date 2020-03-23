//Copying Data Quickly
//Daren Liu

        .syntax		unified
		.cpu		cortex-m4
		.text
		
		.global		UseLDRB
		.thumb_func
	// void UseLDRB(void *dst, void *src);
	UseLDRB:
		// TODO: Write your Assembly code for the 'UseLDRB' function.
		.rept 512 //Loops 512 times for 512 bytes
		LDRB R2, [R1], 1 //Stores r1 in r2, increments address by 1
		STRB R2, [R0], 1 //stores r2 in r0, increments address by 1
		.endr 
		BX LR
		.global		UseLDRH
		.thumb_func
	// void UseLDRH(void *dst, void *src);
	UseLDRH:
		// TODO: Write your Assembly code for the 'UseLDRH' function.
		.rept 256 //512/2 = 256, loops 256 times
		LDRH R2, [R1], 2 //Stores r1 in r2, increments address by 2
		STRH R2, [R0], 2 //Stores r2 in r0, increments address by 2
		.endr
		BX LR
		.global		UseLDR
		.thumb_func
	// void UseLDR(void *dst, void *src);
	UseLDR:
		// TODO: Write your Assembly code for the 'UseLDR' function.
		.rept 128 //512/4 = 128, loops 128 times
		LDR R2, [R1], 4 //Stores r1 in r2, increments address by 4
		STR R2, [R0], 4 //Stores r2 in r0, increments address by 4
		.endr
		BX LR
		.global		UseLDRD
		.thumb_func
	// void UseLDRD(void *dst, void *src);
	UseLDRD:
		// TODO: Write your Assembly code for the 'UseLDRD' function.
		.rept 64 //512/8 = 64, loops 64 times
		LDRD R2, R3, [R1], 8 //Stores r1 in r2 and r3, increases address by 8 
		STRD R2, R3, [R0], 8 //Stores r2 amd r3 in r0, increase address by 8
		.endr
		BX LR
		.global		UseLDM
		.thumb_func
	// void UseLDM(void *dst, void *src);
	UseLDM:
		// TODO: Write your Assembly code for the 'UseLDM' function.
		PUSH {R4-R12} //Extra 8 registers
		.rept 16 //Loops 16 times
		LDMIA R1!, {R4-R12} //Stores r1 in r4-r12
		STMIA R0!, {R4-R12} //Stores r4-r12 in r0
		.endr
		POP {R4-R12}
		BX LR
		.end
