int mod(register int a,register int b){ //function for modulo, since C can't moduled negative numbers
	if (a<0){
		return special(a+b,b);
	}
	else{
		return a%b;
	}
}
int special(register int a, register int b){ //function for negative numbers of modulo, recursively
	if(a>=0){
		return a;
	}
	else{
		return special(a+b,b);
	}
}
void encrypt(unsigned char *M, unsigned char *key) {
	register int maksKey, maksM;
  maksKey = strlen(key);
  maksM = strlen(M);
#pragma omp parallel shared (maksM,maksKey)
{
  register int from, to, i, j;
  #pragma omp for
  for (i = 0; i < maksM; i += maksKey + 1) {
    from = i;
    to = i + maksKey;
    #pragma omp task{
      for (j = from; j <= to; j++) {
       M[j] = mod((M[j] + key[j % maksKey]), 256);
      }
    }
  }
}

}
void decrypt(unsigned char *M, unsigned char *key) {
  register int maksKey, maksM;
  maksKey = strlen(key);
  maksM = strlen(M);
#pragma omp parallel shared (maksKey,maksM,M,key)
{
  register int from, to, i, j;
  #pragma omp for
  for (i = 0; i < maksM; i += maksKey + 1) {
    from = i;
    to = i + maksKey;
    #pragma omp task{
      for (j = from; j <= to; j++) {
        M[j] = mod((M[j] - key[j % maksKey]), 256);
      }
    }
  }
}
}
