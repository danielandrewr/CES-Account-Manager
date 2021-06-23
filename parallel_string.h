/*
Header ini adalah kumpulan fungsi string.h yang dibutuhkan
oleh kelompok kami dan  beberapa dimodifikasi menjadi versi paralel
jika memungkinkan.
*/
#ifndef PARALLEL_STRING
#define PARALLEL_STRING
//Fungsi strlen. Tidak bisa diparalelkan.
int my_strlen(const unsigned char *string){
	#pragma omp single
	register int i;
	for(i = 0; string[i] != '\0'; i++);
	return i;
}

//Fungsi strcpy yang diubah menjadi paralel
void my_strcpy(unsigned char *destination, const unsigned char *source){ //parameter ketiga beda 
	register int i, size_source = my_strlen(source);
	
	#pragma omp parallel for
	for (i = 0; i<size_source; i++){
		destination[i] = source[i];
	}
	destination[size_source] = '\0';
}

//Fungsi strcat yang diubah menjadi paralel
void my_strcat(unsigned char *destination, const unsigned char *source){
	register int i;
	register int size_source = my_strlen(source);
	register int size_destination = my_strlen(destination);
	register int size_keduanya = size_source + size_destination;

	#pragma omp parallel for
	for (i = size_destination; i<size_keduanya; i++){
		destination[i] = source[i-size_destination];
	}
	destination[size_keduanya] = '\0';
}
//Fungsi strcmp yang diubah menjadi paralel
int my_strcmp(const unsigned char *str1,const  unsigned char *str2){
	register int i,flag = 1;
	register int len_str1 = my_strlen(str1);
	
	#pragma omp parallel for shared(flag)
	for(i = 0; i<len_str1; i++){ 
		if(str1[i] != str2[i]){
			#pragma omp critical
			flag = 0;
		}
		if(!flag){
			break;
		}
	}
	return !flag;
}

/*
Fungsi strcasecmp, tetapi dapat digunakan jika kata yang dicari tidak persis.
Contohnya, ada "facebook.com", tetapi dengan mencari "Book" dapat ditemukan.
*/
int my_strcasestr(const unsigned char * string, const unsigned char * toFind) {
  register int slen = my_strlen(string);
  register int tFlen = my_strlen(toFind);
  register int found = 0, s,t;
  
#pragma omp single
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
#endif //PARALLEL_STRING