int strlcpy(char *dst, const char *src, int size){
	int i = 0;
	for(; i<(size-1); ++i){
		if(src[i] == 0)// end of string
			break; 
		dst[i] = src[i];
	}
	dst[i]=0; // null terminate
	return i;
}

#define LONG long // allows easy change of int type
#define PRECISION	1.0e9	// 9 digits after decimal, fits in 32-bit int

int itostr(char *dst, LONG x, int size){
	char buf[12];
	char neg = 0;
	if(size < 2)
		return 0;
	if(x == 0){
		return strlcpy(dst, "0", size);
	}else if(x < 0){
		neg = 1;
		x = -x; // Will fail if x is max negative number
		if( x < 0 )
			--x;	// Force to max val via 2's compliment
	}
	int i;
	for(i = 0; x; x = x/10){
		buf[i++] = x%10 + '0';
	}
	int j=0;
	if(neg){
		dst[j++] = '-';
	}
	for(; (i)&&(j<(size-1)); ++j){
		dst[j] = buf[--i];
	}
	dst[j] = 0; // null terminate
	return j;
}

int ftostr(char *dst, double x, int size){
	int len = 0;
	if(size <= 4)
		return 0; // not enough space to do anything
	if( x == 0.0 ){
		return strlcpy(dst, "0.0", size);
	} else if(x < 0){
		*dst = '-';
		++len;
		x = -x;
	}
	int exp=0;
	char exp_string[6];
	LONG y = x + 1;
	if(x > y){
		// Too large, use scientific notation
		for(exp=0; x>1000.; exp+=3){
			x *= 0.001;
		}
		exp = 1+itostr(exp_string, exp, sizeof(exp_string));
	} else if( x < 0.1) {
		// Too small, use scientific notation
		for(exp=0; x<1.; exp-=3){
			x *= 1000.;
		}
		exp = itostr(exp_string, exp, sizeof(exp_string));
	}
	// Reasonable range. Use moving decimal format
	y = x;
	len += itostr(dst+len, x, size-len-exp);
	dst[len++] = '.';
	x -= y;
	y = x * PRECISION;
	len += itostr(dst+len, y, size-len-exp);
	if( exp ){
		dst[len++]='E';
		len += strlcpy(dst+len, exp_string, sizeof(exp_string));
	}
	return len; 
}
