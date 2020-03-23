//Teteris and Gyroscopes
//Daren Liu

        .syntax		unified
		.cpu		cortex-m4
		.text
	
	// BOOL GetBit(uint16_t *bits, uint32_t row, uint32_t col);
		.global		GetBit
		.thumb_func
		.set		BITBANDING, 1		// Comment out if not using bit-banding
		.ifdef		BITBANDING
	GetBit:
		// TODO: Write your Assembly code for the 'GetBit' function (with bit-banding!).
		SUB R0, R0, 0x20000000 //R0 = Bit-Band Region offset
		LSL R0, 5 //R0 = R0 * 32
		LDR R3 ,= 0x22000000
		ADD R0, R3, R0 //offset + R3
		ADD R1, R2, R1, LSL 2 //Get bit number
		LSL R1, 2 //bit number * 4
		ADD R0, R0, R1 //R0 = Bit Band address
		LDR R0, [R0] //Load bit band address into R0
		BX LR
		.else
	
	GetBit:
		// TODO: Write your Assembly code for the 'GetBit' function (without bit-banding!).
		ADD R1, R2, R1, LSL 2 //Gets the position of the bit 
		LDRH R0, [R0] //R0 = bits
		LSR R0, R0, R1 //Shift right R1 times
		UBFX R0, R0, 0, 1 //Row 0, col 0 should be bit of interest, extract the first bit
		BX LR
		.endif
	
	// void PutBit(BOOL value, uint16_t *bits, uint32_t row, uint32_t col);
		.global		PutBit
		.thumb_func
		.ifdef		BITBANDING
	PutBit:
		// TODO: Write your Assembly code for the 'PutBit' function (with bit-banding!).
		SUB R1, R1, 0x20000000
		LSL R1, 5 //r1 = offset
		LDR R12 ,= 0x22000000
		ADD R1, R1, R12 //0x22000000 + offset
		ADD R2, R3, R2, LSL 2 //position of bit
		LSL R2, 2 //4 times bit position
		ADD R1, R1, R2 //R1 = Bit band address
		STR R0, [R1] //Store the value into the bit band address
		.else
	
	PutBit:
		// TODO: Write your Assembly code for the 'PutBit' function (without bit-banding!).
		ADD R2, R3, R2, LSL 2 //Gets position of the bit
		LDR R3, [R1]  //R3 = bits
		ROR R3, R3, R2 //Rotate R3 R2 times
		BFI R3, R0, 0, 1 //Set the first bit to R0 (value)
		RSB R2, R2, 32 //R2 = 32 - R2
		ROR R3, R3, R2 //Rotate 32 - R2 times
		STR R3, [R1] //Store *bits back into R1
		BX LR
		.endif
		
		.end
