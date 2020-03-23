//HW Floating-Point Quadratics
//Daren Liu

        .syntax		unified
		.cpu		cortex-m4
		.text
		
		.global		Discriminant
		.thumb_func
	// float Discriminant(float a, float b, float c);
	Discriminant:
		
		.global		Quadratic
		.thumb_func
		VMUL.F32 S1, S1, S1 //S1 = B^2
		VMOV S3, 4.0 //S3 = 4.0
		VMUL.F32 S0, S0, S2 //S0 = AC
		VMUL.F32 S0, S0, S3 //S0 = 4AC
		VSUB.F32 S0, S1, S0 //S0 = B^2 - 4AC
		BX LR
	// float Quadratic(float x, float a, float b, float c);
	Quadratic:
		
		.global		Root1
		.thumb_func
		
		VMUL.F32 S2, S2, S0 //S2 = BX
		VADD.F32 S2, S2, S3 //S2 = BX + C
		VMUL.F32 S3, S0, S0 //S3 = X^2
		VMUL.F32 S3, S3, S1 //S3 = AX^2
		VADD.F32 S0, S3, S2 //S0 = AX^2 + BX + C
		BX LR
	// float Root1(float a, float b, float c);
	Root1:
		
		.global		Root2
		.thumb_func
		VMOV S3, S0 //S3 = A
		VMOV S4, S1 //S4 = B
		BL Discriminant //S0 = Discriminant
		VSQRT.F32 S0, S0 //S0 = sqrt(Discriminant)
		VNEG.F32 S4, S4 //S4 = -B
		VADD.F32 S0, S4, S0 //S0 = -B + sqrt(Discriminant)
		VMOV S6, 2.0
		VMUL.F32 S3, S3, S6 //S3 = 2A
		VDIV.F32 S0, S0, S3 //S0 = -B + sqrt(Discriminant) / 2A
		BX LR
	// float Root2(float a, float b, float c);
	Root2:
		VMOV S3, S0 //S3 = A
		VMOV S4, S1 //S4 = B
		BL Discriminant //S0 = Discriminant
		VSQRT.F32 S0, S0 //S0 = sqrt(Discriminant)
		VNEG.F32 S4, S4 //S4 = -B
		VSUB.F32 S0, S4, S0 //S0 = -B - sqrt(Discriminant)
		VMOV S6, 2.0
		VMUL.F32 S3, S3, S6 //S3 = 2A
		VDIV.F32 S0, S0, S3 //S0 = -B - sqrt(Discriminant) / 2A
		BX LR
		.end
