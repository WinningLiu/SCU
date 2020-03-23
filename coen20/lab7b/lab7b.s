//Making Change
//Daren Liu

		.syntax		unified
		.cpu		cortex-m4
		.text
	
	// void Bills(uint32_t dollars, BILLS *paper);
		.global		Bills
		.thumb_func
	Bills:
		// TODO: Write your Assembly code for the 'Bills' function.
		//Divide by 20
		LDR R2 ,= 0xCCCCCCCD
		UMULL R3, R2, R2, R0
		LSR R3, R2, 4
		STR R3, [R1] // R3 = quotient
		LSL R12, R3, 4 //R12 = 16R3
		ADD R3, R12, R3, LSL 2 // R3 = 16R3 + 4 R3
		SUB R0, R0, R3 //remainder = dollars - (20 * R3)
		
		//Divide by 10
		LDR R2 ,= 0xCCCCCCCD
		UMULL R3, R2, R2, R0
		LSR R3, R2, 3
		STR R3, [R1, 4] // R3 = quotient
		LSL R12, R3, 3 //R12 = 8R3
		ADD R3, R12, R3, LSL 1 //R3 = 8R3 + 2R3
		SUB R0, R0, R3 //remainder = dollars - (10 * R3)
		
		//Divide by 5
		LDR R2 ,= 0xCCCCCCCD
		UMULL R3, R2, R2, R0
		LSR R3, R2, 2
		STR R3, [R1, 8] //R3 = quotient
		LSL R12, R3, 2 //R12 = 4R3
		ADD R3, R12, R3 // R3 = 5R3
		SUB R0, R0, R3 //remainder = dollars - (5*R3)
		
		//Divide by 1
		STR R0, [R1, 12]
		BX LR
		
	// void Coins(uint32_t cents, COINS *coins);
		.global		Coins
		.thumb_func
	Coins:
		// TODO: Write your Assembly code for the 'Coins' function.
		//Divide by 25
		LDR R2 ,= 0x51EB851F
		UMULL R3, R2, R2, R0
		LSR R3, R2, 3
		STR R3, [R1] //R3 = quotient
		LSL R12, R3, 4 //R12 = 16*R3
		ADD R12, R12, R3, LSL 3 //R12 = 16R3 + 8R3 = 24R3
		ADD R3, R12, R3 //R3 = 25R3
		SUB R0, R0, R3 //R0 = dollars - (25R3)
		
		//Divide by 10
		LDR R2 ,= 0xCCCCCCCD
		UMULL R3, R2, R2, R0
		LSR R3, R2, 3
		STR R3, [R1, 4] R3 = quotient
		LSL R12, R3, 3 //R12 = 8R3
		ADD R3, R12, R3, LSL 1 //R3 = 8R3 + 2R3
		SUB R0, R0, R3 //remainder = dollars - (10 * R3)
		
		//Divide by 5
		LDR R2 ,= 0xCCCCCCCD
		UMULL R3, R2, R2, R0
		LSR R3, R2, 2
		STR R3, [R1, 8] //R3 = quotient
		LSL R12, R3, 2 //R12 = 4R3
		ADD R3, R12, R3 // R3 = 5R3
		SUB R0, R0, R3 //remainder = dollars - (5*R3)
		
		//Divide by 1
		STR R0, [R1, 12]
		BX LR
		.end
