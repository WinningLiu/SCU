//Sliding 15-Puzzle
//Daren Liu

        .syntax		unified
		.cpu		cortex-m4
		.text
	
	// void	CopyCell(RGB_PXL *dst, RGB_PXL *src);
		.global		CopyCell
		.thumb_func
	CopyCell:
		// TODO: Write your Assembly code for the 'CopyCell' function.
		CopyCell: // r0 = dst, r1 = src 
		ldr r2 ,= 0 //r2 = row = 0
	FirstLoop:
		ldr r3 ,= 0 // r3 = col = 0
		cmp r2, 60 
		BHS End 
	SecondLoop: 
		cmp r3, 60
		BHS Then
		ldr r12, [r1, r3, lsl 2] //r12 <- src[col]
		str r12, [r0, r3, lsl 2] // dst[col] = src[col]
		add r3, r3, 1 //col++
		B SecondLoop
	Then:
		add r0, r0, 960 //dst += 240
		add r1, r1, 960 //src += 240
		add r2, r2, 1 //row++
		B FirstLoop
	End:
		BX LR
	
	// void FillCell(RGB_PXL *dst, uint32_t color);
		.global		FillCell
		.thumb_func
	FillCell:
		// TODO: Write your Assembly code for the 'FillCell' function.
	ldr r2 ,= 0 //r2 = row = 0
	FirstLoop1:
		ldr r3 ,= 0 //r3 = col = 0 
		cmp r2, 60
		BHS End1
	SecondLoop1:
		cmp r3, 60
		BHS Then1
		str r1, [r0, r3, lsl 2] //dst[col] = pixel
		add r3, r3, 1 //col++
		B SecondLoop1
	Then1:
		add r0, r0, 960 //dst += 240
		add r2, r2, 1 //row++
		B FirstLoop1
	End1:
		BX LR
		.end
