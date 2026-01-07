		.text
main:
		divf f2, f5, f6
		divf f1, f2, f6
		divf f3, f1, f5
		divf f0, f4, f7
		addf f14, f0, f6
		addf f15, f5, f7
		multf f20, f4, f6
		multf f21, f5, f7
		halt