int clean_stdin() {
  while (getchar() != '\n');
  return 1;
}

void inputInteger(int * variabel) {
  unsigned char dummy;
  while ((scanf("%d%c", variabel, & dummy) != 2 || dummy != '\n') && clean_stdin()) {
    puts("(* - *)! Hanya boleh memasukkan angka. Silakan masukkan angka");
  }
}

void inputAngka(int * angka, int batas_bawah, int batas_atas) {
  do {
    inputInteger(angka);
    if ( * angka < batas_bawah || * angka > batas_atas) {
      puts("(> o <)! Masukkan angka dengan benar!");
    }
  } while ( * angka < batas_bawah || * angka > batas_atas);
}

void inputString(unsigned char * string, const unsigned int batas_bawah, const unsigned int batas_atas) {
  unsigned short int i, count = 0;
  unsigned char *input = malloc(256*sizeof(unsigned char));
  do {
    scanf(" %255[^\n]hhu", input);
		fflush(stdin);
		//ntar pakein pragma omp for reduction bisa ga ni
    for (i = 0; input[i] != '\0'; ++i) {
      count++;
    }
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