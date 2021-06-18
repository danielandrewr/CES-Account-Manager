int strlen(const unsigned char *string){
	register int i;
	for(i = 0; string[i] != '\0'; i++);
	return i;
}

void strcpy(unsigned char *destination, unsigned char *source){ //parameter ketiga beda 
	register int i, size_source = strlen(source);
	
	//#pragma omp parallel for
	for (i = 0; i<size_source; i++){
		destination[i] = source[i];
	}
	destination[size_source + 1] = '\0';
}

void strcat(unsigned char *destination, unsigned char *source){
	register int i;
	register int size_source = strlen(source);
	register int size_destination = strlen(destination);
	register int size_keduanya = size_source + size_destination;

	//#pragma omp parallel for
	for (i = size_destination + 1; i<size_keduanya; i++){
		destination[i] = source[i-size_destination-1];
	}
	destination[size_keduanya + 1] = '\0';
}

int strcmp(const unsigned char *str1,const  unsigned char *str2){
	register int i,flag = 1;
	register int len_str1 = strlen(str1);
	
	//#pragma omp parallel for shared(flag)
	for(i = 0; i<len_str1; i++){ 
		if(str1[i] != str2[i]){
			flag = 0;
		}
		if(!flag){
			break;
		}
	}
	return !flag;
}
int strcasestr(const unsigned char * string, const unsigned char * toFind) {
  register int slen = strlen(string);
  register int tFlen = strlen(toFind);
  register int found = 0, s,t;
  

  if (slen >= tFlen) {
    for ( s = 0, t = 0; s < slen; s++) {
      do {

        if (string[s] == toFind[t] || string[s] == toFind[t] + 32 || string[s] + 32 == toFind[t]) {
          if (++found == tFlen) return 0;
          s++;
          t++;
        } else {
          s -= found;
          found = 0;
          t = 0;
        }

      } while (found);
    }
    return 1;
  } else return -1;
}