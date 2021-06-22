/*
Header ini merupakan kumpulan fungsi scanf yang diberikan error handling.
Berisi scanf khusus integer dan scanf khusus string yang diberi batas bawah
dan batas atas.
*/
#ifndef GET_INPUT
#define GET_INPUT

#include <stdio.h>
#include <stdlib.h>

//Fungsi yang berguna untuk membersihan STDIN.
int clean_stdin() {
  while (getchar() != '\n');
  return 1;
}

//Fungsi khusus integer yang akan mendeteksi jika diinput huruf.
void inputInteger(int * variabel) {
  unsigned char dummy;
  while ((scanf("%d%c", variabel, & dummy) != 2 || dummy != '\n') && clean_stdin()) {
    puts("(* - *)! Hanya boleh memasukkan angka. Silakan masukkan angka");
  }
}

//Fungsi yang mendeteksi besarnya input integer di atas.
void inputAngka(int * angka, const int batas_bawah, const int batas_atas) {
  do {
    inputInteger(angka);
    if ( * angka < batas_bawah || * angka > batas_atas) {
      puts("(> o <)! Masukkan angka dengan benar!");
    }
  } while ( * angka < batas_bawah || * angka > batas_atas);
}

//Fungsi khusus untuk mendapatkan input string dengan batas bawah dan batas atas.
void inputString(unsigned char * string, const unsigned int batas_bawah, const unsigned int batas_atas) {
  register unsigned short int i, count = 0;
  unsigned char *input = malloc(256*sizeof(unsigned char));
  do {
    scanf(" %255[^\n]hhu", input);
	fflush(stdin);
	//ntar pakein pragma omp for reduction bisa ga ni
    count = my_strlen(input);
    if (count < batas_bawah){
			printf("Input yang diberikan minimal %d karakter. Silakan input lagi\n\n", batas_bawah);
		}else if (count > batas_atas){
			printf("Input yang diberikan maksimal %d karakter. Silakan input lagi\n\n", batas_atas);
		}else{
			my_strcpy(string,input);
		}
  } while (count < batas_bawah || count > batas_atas);
	free(input);
}
#endif //GET_INPUT