#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parallel_string.h"
#include "md5.h"
#include "get_input.h"
#include "encipher.h"
#define fileName "account.txt"

#define USERNAME_MIN 5
#define USERNAME_MAX 50
#define EMAIL_MIN 10
#define EMAIL_MAX 254
#define PASSWORD_MIN 6
#define PASSWORD_MAX 100
#define MD5_SIZE 16

const unsigned char GARBAGE_STRING[] = "!@#$%^&*()";

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


void Registrasi(struct MasterAccount **head){
	unsigned char *temp_email = malloc(355*sizeof(unsigned char));
	unsigned char *temp_password = malloc(101*sizeof(unsigned char));
	int yakin;
	struct MasterAccount *ptr = NULL;
	
	do{
		printf("\nMasukkan username akun master: ");
		inputString(ptr->username,USERNAME_MIN,USERNAME_MAX); // username length minimal 5, max 50
		printf("Masukkan email akun master: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX); // email length minimal 10, max 254
		printf("Masukkan password akun master: ");
		inputString(temp_password,PASSWORD_MIN,PASSWORD_MAX); //password length minimal 6, max 101

		printf("Apakah anda yakin dengan data yang telah diinput? (1. Ya 2. Tidak)\n");
		inputAngka(&yakin,1,2);
		printf("\033[0;0H\033[2J"); //clear console di repl
	}while (yakin == 2);

	if (!my_strcmp((**head).username,GARBAGE_STRING)){ //jika barusan mengisi
		ptr = *head; //ptr adalah head nya
		ptr->next = NULL;
	}else{ //jika database ada
		ptr = malloc(sizeof(struct MasterAccount)); //maka ptr menjadi data pertama
		ptr->next = *head;
		*head = ptr; //head nya berubah menjadi data yang barusan dimasukkan
	}
	encrypt(ptr->username,temp_password); //enkripsi usernamenya

	my_strcat(temp_email,temp_password);
	
	md5(temp_email,my_strlen(temp_email)+my_strlen(temp_password),ptr->md5_auth);
	


	free(temp_email);
	free(temp_password);
	ptr = NULL;
}
void Add_Slave(struct SlaveAccount **head_slave, unsigned char *password){
	struct SlaveAccount *ptr = NULL;
	
	int yakin;
	unsigned char *temp_website = NULL;
	unsigned char *temp_email = NULL;
	unsigned char *temp_password = NULL;

	

	do{
		temp_website = malloc(USERNAME_MAX*sizeof(unsigned char));
		temp_email = malloc(EMAIL_MAX*sizeof(unsigned char));
		temp_password = malloc(PASSWORD_MAX*sizeof(unsigned char));

		
		printf("Website baru: ");
		inputString(temp_website,USERNAME_MIN,USERNAME_MAX);
		printf("Email baru: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX);
		printf("Password baru: ");
		inputString(temp_password,USERNAME_MIN,USERNAME_MAX);

		printf("Apakah anda yakin dengan perubahan ini?\n1. Yakin\n2. Ulangi input 3. Tidak jadi input\n");
		inputAngka(&yakin,1,3);
		switch (yakin){
			case 1: if (!my_strcmp((**head_slave).website,GARBAGE_STRING)){ //jika pertama kali punya akun
				ptr = *head_slave;
				ptr -> next = NULL; //maka head nya yg diedit
			}else{ //jika sudah ada database
				ptr = malloc(sizeof(struct SlaveAccount));
				ptr->next = *head_slave; //maka ptr menjadi data yang pertama
				*head_slave = ptr;
			}
							encrypt(temp_website,password);
							encrypt(temp_email,password);
							encrypt(temp_password,password);

							my_strcpy(ptr->website,temp_website);
							my_strcpy(ptr->email,temp_email);
							my_strcpy(ptr->password,temp_password);
							break;
			case 2: break;
			default: break;
		}
		free(temp_website);
		free(temp_email);
		free(temp_password);
	} while(yakin == 2);
	
	ptr = NULL;

}
void Cari_Slave(struct SlaveAccount *head, unsigned char *password){
	struct SlaveAccount *ptr = head;
	unsigned char *pencarian = NULL;
	unsigned char *decrypted_website = NULL;
	unsigned char *decrypted_email = NULL;
	unsigned char *decrypted_password = NULL;
	bool printed = false;
	int again;
	
	do{
		pencarian = malloc(USERNAME_MAX*sizeof(unsigned char));
		decrypted_website = malloc(USERNAME_MAX*sizeof(unsigned char));
		decrypted_email = malloc(EMAIL_MAX*sizeof(unsigned char));
		decrypted_password = malloc(PASSWORD_MAX*sizeof(unsigned char));
		printf("Masukkan akun website yang ingin dicari: ");
		inputString(pencarian,1,USERNAME_MAX);
		while (ptr != NULL){
			my_strcpy(decrypted_website,ptr->website);
			decrypt(decrypted_website,password);
			if (!my_strcasestr(decrypted_website,pencarian)){
				my_strcpy(decrypted_email,ptr->email);
				my_strcpy(decrypted_password,ptr->password);
				
				decrypt(decrypted_email,password);
				decrypt(decrypted_password,password);
				
				printf("\nWebsite: %s\n",decrypted_website);
				printf("Email: %s\n",decrypted_email);
				printf("Password: %s\n\n",decrypted_password);
				
				printed = true;
			}
			ptr = ptr->next;
		}
		if(!printed){
			printf("\nAkun tidak ditemukan!\n");
		}
		printf("Ingin mencari lagi?\n1. Ya\n2. Tidak\n");
		inputAngka(&again,1,2);
		printf("\033[0;0H\033[2J"); //clear console di repl
		
		free(decrypted_website);
		free(decrypted_email);
		free(decrypted_password);
		free(pencarian);
	}while(again != 1);	
	ptr = NULL;	
}
void Login_Success(struct MasterAccount *head, unsigned char *password){
	unsigned char *decrypted_username = malloc(USERNAME_MAX*sizeof(unsigned char));
	my_strcpy(decrypted_username, head->username);
	decrypt(decrypted_username, password);

	int pilihan;

	do{
		printf("Selamat datang, %s!\n",decrypted_username);
		printf("Menu\n1. Tambah akun\n2. Cari akun\n3. Delete akun\n4. Logout");
		inputAngka(&pilihan, 1,4);
		switch (pilihan){
			case 1: Add_Slave(&head->slave,password);
							break;
			case 2: Cari_Slave(head->slave,password);
							break;
			case 3: Delete_Slave(head,password);
							break;

		}
	}while (pilihan != 4);
	free(decrypted_username);
}

void Login(struct MasterAccount *head) {
	bool authenticated = false;
	int exit = 2;

	struct MasterAccount *ptr = NULL;

	unsigned char *temp_email = NULL;
	unsigned char *temp_password = NULL;
	unsigned char *temp_md5 = NULL;

	do {
		ptr = head;
		temp_email = malloc(355*sizeof(unsigned char));
		temp_password = malloc(101*sizeof(unsigned char));
		temp_md5 = malloc(16*sizeof(unsigned char));

		printf("\n\nMasukkan email\t: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX);
		printf("\n\nMasukkan password\t: ");
		inputString(temp_password,PASSWORD_MIN,PASSWORD_MAX);
		
		my_strcat(temp_email,temp_password);

		md5(temp_email,my_strlen(temp_email)+my_strlen(temp_password),temp_md5);
		free(temp_email);

		while(ptr != NULL){
			if (!my_strcmp(ptr->md5_auth,temp_md5)){
				authenticated = true;
				printf("Telah berhasil login!");
				Login_Success(ptr, temp_password);
				break;
			}else{
				ptr = ptr->next;
			}
		}
		if (ptr!= NULL){
			printf("Email dan password salah!\n1. Input ulang email dan password\n2. Exit\n");
			inputAngka(&exit, 1, 2);
		}
		
		free(temp_password);
		free(temp_md5);
	} while (!authenticated && exit == 1);
}

int main(void) {
	struct MasterAccount *head = malloc(sizeof(struct MasterAccount));
	my_strcpy(head->username,GARBAGE_STRING);
	head->next = NULL;
	head->slave = malloc(sizeof(struct SlaveAccount));
	my_strcpy(head->slave->website,GARBAGE_STRING);
	head->slave ->next = NULL;

	int menu;
	printf("Selamat datang di Proglan Account Manager!\n");
  printf("Silahkan pilih salah satu menu\n");
	do{
		printf("Menu\n1. Register\n2. Login\n3. Exit\n");
		printf("Masukkan pilihan: ");
		inputAngka(&menu,1,2);
		switch (menu){
			case 1: Registrasi(&head);
				break;
			case 2: //Login(head);
				break;
      		case 3:
				printf("Terima kasih karena telah menggunakan aplikasi Account Manager!");
				break;
				default: 
      			break;
		}		
	} while ((menu >= 1) && (menu <= 2));
	return 0;
}

