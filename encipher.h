#ifndef ENCIPHER
#define ENCIPHER

#include "subbytes.h"

int special(int a, int b){ //function for negative numbers of modulo, recursively
	if(a>=0){
		return a;
	}
	else{
		return special(a+b,b);
	}
}
int mod(int a,int b){ //function for modulo, since C can't moduled negative numbers
	if (a<0){
		return special(a+b,b);
	}
	else{
		return a%b;
	}
}
char encrypt(unsigned char * M, const unsigned char * key) {
    int maksKey, maksM;
    //#pragma omp single
    //{
    maksKey = strlen(key);
    maksM = strlen(M);
    //}
    //#pragma omp parallel shared(maksKey,maksM)
    //{
    int from, to, i, j;
    //#pragma omp for
    for (i = 0; i < maksM; i += maksKey + 1) {
        from = i;
        to = i + maksKey;
        //#pragma omp task
        //{
        for (j = from; j <= to; j++) {
            M[j] = mod((M[j] + key[j % maksKey]), 256);
            M[j] = sbox(M[j]);
        }
        //}
        }
    //}
    return 0;
}
char decrypt(unsigned char * M, const unsigned char * key) {
    int maksKey, maksM;
    //#pragma omp single
    //{
    maksKey = strlen(key);
    maksM = strlen(M);
    //}
    //#pragma omp parallel shared(maksKey,maksM,M,key)
    //{
    int from, to, i, j;
    //#pragma omp for
    for (i = 0; i < maksM; i += maksKey + 1) {
        from = i;
        to = i + maksKey;
        //#pragma omp taskwait
        //{
        for (j = from; j <= to; j++) {
            M[j] = rsbox(M[j]); //inverse subbox
            M[j] = mod((M[j] - key[j % maksKey]), 256); //vigenere function
        }
        //}
    }
    //}
    return 0;
}
#endif //ENCIPHER