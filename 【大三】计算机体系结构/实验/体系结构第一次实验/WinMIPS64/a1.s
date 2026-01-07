.text
        add.d f0,f0,2
main:
loop: 	
        ld 		f2,8(r1)		 
        ld 		f4,0(r1)		 
        mul.d 	f6,f4,f0
        sub.d	f8,f2,f6
        sd		f8,0(r2)

        ld 		f2,16(r1)		 
        ld 		f4,8(r1)		 
        mul.d 	f6,f4,f0
        sub.d	f8,f2,f6
        sd		f8,8(r2)

        ld 		f2,24(r1)		 
        ld 		f4,16(r1)		 
        mul.d 	f6,f4,f0
        sub.d	f8,f2,f6
        sd		f8,16(r2)

        dsub 		r1, 	r1,	#8	
        dsub		r2,	r2,	#8
        bnez		r2,  	loop
		halt