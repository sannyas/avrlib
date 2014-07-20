# bmp085 calculate method
-------------------------
## Calculate temperation

	X1 = ( UT - AC6  ) * AC5 / 2^15
	X2 = MC * 2^11 / ( X1 + MD )
	B5 = X1 + X2
	T = ( B5 + 8 ) / 2^4

## Calculate presure

	B6 = B5 - 4000
	X1 = ( B2*(B6*B6/2^12))/2^11
	X2 = AC2*B6/2^11
	X3 = X1 + X2
	B3 = ((AC1*4+X3)<<oss + 2 )/4
	X1 = AC3 * B6 / 2^13
	X2 = ( B1 *( B6*B6/2^12))/2^16
	X3 = ((X1+X2)+2)/2^2
	B4 = AC4*(unsigned long)( X3+32768 ) /2^15
	B7 = ( (unsigned long)UP - B3 ) * ( 50000>>oss )
	if( B7 < 0x80000000 ){
		p = ( B7*2 )/B4
	}
	else{
		p = ( B7/B4 )*2
	}

	X1 = ( p/2^8 ) * ( p/2^8 )
	X1 = ( X1 * 3038 )/2^16
	X2 = ( -7375*p )/2^16
	p = p + ( X1+X2+3791 )/2^4 
