#include "tessbox.h"
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
char encrypt(unsigned char *M, unsigned char *key, int *flag){
	int maksKey,maksM;
	maksKey = strlen(key);
	maksM = strlen(M);
	#pragma omp parallel shared(maksKey,maksM)
	{
		int from,to,i,j;
		#pragma omp for
		for (i=0;i<maksM;i+=maksKey+1){
			from =i;
			to = i + maksKey;
			#pragma omp task
			{
				for (j=from;j<=to;j++){
					M[j]=mod((M[j]+key[j%maksKey]),256);
					sbox(M[j]);
					if(M[j]<=31||M[j]==127){ //error handler for ascii code 0-31 (usually generate error)
						flag[j]=1;
						M[j]+=31;
					}
				}
			}
		}
	}
	return 0;
}
char decrypt(unsigned char *M, unsigned char *key,int *flag){
	int maksKey,maksM;
	maksKey = strlen(key);
	maksM = strlen(M);
	#pragma omp parallel shared(maksKey,maksM,M,key)
	{
		int from,to,i,j;
		#pragma omp for
		for (i=0;i<maksM;i+=maksKey+1){
			from =i;
			to = i + maksKey;
			#pragma omp task
			{
				for (j=from;j<=to;j++){
					if(flag[j]==1){ //error handler for ascii code 0-31 (usually generate error)
						flag[j]=0;
						M[j]-=31;
					}
					rsbox(M[j]); //inverse subbox
					M[j]=mod((M[j]-key[j%maksKey]),256); //vigenere function
				}
			}
		}
	}
	return 0;
}