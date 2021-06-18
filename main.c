#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include "md5.h"
#include "parallel_string.h"
#define fileName "account.txt"

#define USERNAME_MIN 5
#define USERNAME_MAX 50
#define EMAIL_MIN 10
#define EMAIL_MAX 254
#define PASSWORD_MIN 6
#define PASSWORD_MAX 100
#define MD5_SIZE 16



struct SlaveAccount{
	unsigned char website[USERNAME_MAX];
	unsigned char email[EMAIL_MAX];
	unsigned char password[PASSWORD_MAX];
	struct SlaveAccount *next;
};
struct MasterAccount {
	unsigned char username[USERNAME_MAX];
	unsigned char md5_auth[MD5_SIZE];
	struct SlaveAccount *slave;
	struct MasterAccount *next;
};

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

// fungsi ngecek filenya ada isi
/*bool ifFileNotNull(FILE *fptr) {
	bool fileNotNull = false;

	fptr = fopen(fileName, "r");
	if (fptr != NULL) {
		fseek(fptr, 0, SEEK_END);
		int fsize = ftell(fptr);
		if (fsize != 0)
			fileNotNull = true;  

	} else 
		printf("\nUps, Something went wrong!\n");
		
	return fileNotNull;
}
*/
void inputString(unsigned char * string, const unsigned int batas_bawah, const unsigned int batas_atas) {
  unsigned short int i, count = 0;
  unsigned char *input = malloc(256*sizeof(unsigned char));
  do {
    scanf(" %255[^\n]s", & input);
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
			strcpy(string,input);
		}
  } while (count < batas_bawah || count > batas_atas);
	free(input);
}
void Registrasi(struct MasterAccount *head){
	unsigned char *temp_email = malloc(355*sizeof(unsigned char));
	unsigned char *temp_password = malloc(101*sizeof(unsigned char));
	int yakin;
	struct MasterAccount *ptr = head;
	while (ptr->next != NULL){
		ptr = ptr->next;
	}
	do{
		printf("\nMasukkan username akun master: ");
		inputString(ptr->username,USERNAME_MIN,USERNAME_MAX); // username length minimal 5, max 50
		printf("Masukkan email akun master: ");
		inputString(&temp_email,EMAIL_MIN,EMAIL_MAX); // email length minimal 10, max 254
		printf("Masukkan password akun master: ");
		inputString(&temp_password,PASSWORD_MIN,PASSWORD_MAX); //password length minimal 6, max 101

		printf("Apakah anda yakin dengan data yang telah diinput? (1. Ya 2. Tidak)\n");
		inputAngka(&yakin,1,2);
		printf("\033[0;0H\033[2J"); //clear console di repl
	}while (yakin == 2);
	encrypt(ptr->username,temp_password); //enkripsi usernamenya

	strcat(temp_email,temp_password);
	
	md5(temp_email,strlen(temp_email)+strlen(temp_password),ptr->md5_auth);
	


	free(temp_email);
	free(temp_password);
	ptr = NULL;
	

	/*
	int len = strlen(temp);
	printf("%d", len);
	
	FILE *fptr;
	fptr = fopen(fileName, "w");

	if (ifFileNotNull(fptr) != false) {
		// make new account
	} else {
		// write new master account
	}
	*/
}

void Login(struct MasterAccount *head) {
	bool authenticated = false;

	unsigned char *temp_email = malloc(355*sizeof(unsigned char));
	unsigned char *temp_password = malloc(101*sizeof(unsigned char));

	do {
		printf("\n\nMasukkan email\t: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX);
		printf("\n\nMasukkan password\t: ");
		inputString(temp_password,PASSWORD_MIN,PASSWORD_MAX);
		
		strcat(temp_email,temp_)

		// Autentikasi email dan password // coba liat args terakhir hash functionya
		if ((strcmp(head->username, email) != 0) && 
			strcmp((md5(head->md5_auth, strlen(head->md5_auth), /*uint8_t *digest*/), hash) != 0)) {
			printf("\nUps, Something went wrong!\n");
		} else {
			printf("\nAuthenticated!\n");
			authenticated = true;
			break;
		}
	} while (!authenticated);
}

int main(void) {
	struct MasterAccount *head = malloc(sizeof(struct MasterAccount));
	head->next = NULL;
	head->slave = malloc(sizeof(struct SlaveAccount));
	head->slave ->next = NULL;

	int menu;
	printf("Selamat datang di Proglan Account Manager!");
	do{
		printf("Menu\n1. Register\n2. Login\n3. Exit\n");
		printf("Masukkan pilihan: ");
		inputAngka(&menu,1,2);
		switch (menu){
			case 1: Registrasi(head);
				
				break;
			case 2: //Login(head);
				break;
			default: break;
		}
		
	} while (menu != 3);

	printf("Terima kasih karena telah menggunakan aplikasi Account Manager!");
	return 0;
}
