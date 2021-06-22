#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define USERNAME_MIN 5
#define USERNAME_MAX 50
#define EMAIL_MIN 10
#define EMAIL_MAX 254
#define PASSWORD_MIN 6
#define PASSWORD_MAX 100
#define MD5_SIZE 16

const char *fileName = "Encrypted Database.txt";


struct SlaveAccount{    //struct untuk slave akun dimana akan terhubung dengan master 
	unsigned char website[USERNAME_MAX];
	unsigned char email[EMAIL_MAX];
	unsigned char password[PASSWORD_MAX];
	struct SlaveAccount *next;
};
struct MasterAccount {  //struct untuk master akun dimana akan terhubung dengan slave
	unsigned char username[USERNAME_MAX];
	unsigned char md5_auth[MD5_SIZE];
	struct SlaveAccount *slave;
	struct MasterAccount *next;
};

#include "parallel_string.h"
#include "md5.h"
#include "get_input.h"
#include "encipher.h"
#include "filehandler.h"

// function untuk registrasi akun master
void Registrasi(struct MasterAccount **head){   
	unsigned char *temp_email = NULL;
	unsigned char *temp_password = NULL;
	unsigned char *temp_username = NULL;
	int yakin;
	struct MasterAccount *ptr = NULL;
	
	do{
		temp_email = malloc((EMAIL_MAX+PASSWORD_MAX)*sizeof(unsigned char));
		temp_password = malloc(PASSWORD_MAX*sizeof(unsigned char));
		temp_username = malloc(USERNAME_MAX*sizeof(unsigned char));
		ptr = malloc(sizeof(struct MasterAccount));

		printf("\nMasukkan username akun master: ");
		inputString(ptr->username,USERNAME_MIN,USERNAME_MAX); // username length minimal 5, max 50
		printf("Masukkan email akun master: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX); // email length minimal 10, max 254
		printf("Masukkan password akun master: ");
		inputString(temp_password,PASSWORD_MIN,PASSWORD_MAX); //password length minimal 6, max 101

    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Apakah anda yakin dengan data yang telah diinput? (1. Ya 2. Ulangi 3. Tidak jadi registrasi)\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		inputAngka(&yakin,1,3);
		switch (yakin){
			case 1: 
					encrypt(ptr->username,temp_password); //enkripsi usernamenya

					my_strcat(temp_email,temp_password); //konkatenasi email dan password
								
					md5(temp_email,my_strlen(temp_email),ptr->md5_auth); //dijadikan MD5 authentication
					if(*head == NULL){
						ptr->next = NULL;
					}else{
						ptr->next = *head;
					}
					*head = ptr; //LIFO agar algoritma penambahannya time complexity nya O(1)
					break;
			case 2: break;
			default: break;
		}
		free(temp_email);
		free(temp_password);
		ptr = NULL;

		printf("\033[0;0H\033[2J"); //clear console di repl
	}while (yakin == 2);
}
//funtion untuk menambahkan akun slave yang langsung berhubung dengan akun master
void Add_Slave(struct SlaveAccount **head_slave, const unsigned char *password){  
	struct SlaveAccount *ptr = NULL;
	
	int yakin;
	unsigned char *temp_website = NULL;
	unsigned char *temp_email = NULL;
	unsigned char *temp_password = NULL;

	do{
		temp_website = malloc(USERNAME_MAX*sizeof(unsigned char));
		temp_email = malloc(EMAIL_MAX*sizeof(unsigned char));
		temp_password = malloc(PASSWORD_MAX*sizeof(unsigned char));

		
		printf("Website baru: ");     //input nama website baru
		inputString(temp_website,USERNAME_MIN,USERNAME_MAX);
		printf("Email baru: ");       //input nama email baru
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX);
		printf("Password baru: ");    //input password baru
		inputString(temp_password,USERNAME_MIN,USERNAME_MAX);

    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Apakah anda yakin dengan perubahan ini?\n1. Yakin\n2. Ulangi input 3. Tidak jadi input\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		inputAngka(&yakin,1,3);
		switch (yakin){
			case 1: ptr = malloc(sizeof(struct SlaveAccount));
					encrypt(temp_website,password);
					encrypt(temp_email,password);
					encrypt(temp_password,password);

					my_strcpy(ptr->website,temp_website);
					my_strcpy(ptr->email,temp_email);
					my_strcpy(ptr->password,temp_password);
					if (*head_slave == NULL){
						ptr->next = NULL;
					}else{
						ptr->next = *head_slave;
					}
					*head_slave = ptr;
						
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
//function untuk mencari akun slave
bool Cari_Slave(struct SlaveAccount *head_slave, const unsigned char *password){  
	if (head_slave == NULL){
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("\nTidak ada akun. Silakan tambahkan akun terlebih dahulu!\n");  //memberitahu bahwa tidak ada akun yang tersimpan
		printf("Tekan ENTER untuk kembali\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		getchar();
		return true;
	}
	struct SlaveAccount *ptr = NULL;
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
		printf("Masukkan akun website yang ingin dicari: ");    //input website untuk dicari
		inputString(pencarian,1,USERNAME_MAX);
		for (ptr = head_slave; ptr != NULL; ptr = ptr->next){
			my_strcpy(decrypted_website,ptr->website);
			decrypt(decrypted_website,password);
			if (!my_strcasestr(decrypted_website,pencarian)){
				my_strcpy(decrypted_email,ptr->email);
				my_strcpy(decrypted_password,ptr->password);
				
				decrypt(decrypted_email,password);
				decrypt(decrypted_password,password);
				
				printf("\nWebsite: %s\n",decrypted_website);    //menunjukan akun pada website yang telah dicari
				printf("Email: %s\n",decrypted_email);
				printf("Password: %s\n\n",decrypted_password);
				
				printed = true;
			}
		}
		if(!printed){
			printf("\nAkun tidak ditemukan!\n");  //akun tidak ditemukan pada website yang dicari
		}
		printf("Ingin mencari lagi?\n1. Ya\n2. Tidak\n");
		inputAngka(&again,1,2);
		printf("\033[0;0H\033[2J"); //clear console di repl
		
		free(decrypted_website);
		free(decrypted_email);
		free(decrypted_password);
		free(pencarian);
	}while(again == 1);	
	ptr = NULL;	

	return false;
}
//function untuk menghapus akun slave berdasarkan yang dipilih
bool Delete_Slave(struct SlaveAccount **head_slave, const unsigned char *password){ 
	if (head_slave == NULL){
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("\nTidak ada akun. Silakan tambahkan akun terlebih dahulu!\n");
		printf("Tekan ENTER untuk kembali\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		getchar();
		return true;
	}
	struct SlaveAccount *ptr = NULL, *prev = NULL;
	unsigned char *decrypted_website = NULL;
	unsigned char *pencarian = NULL;
	int again;

	do{
		decrypted_website = malloc(USERNAME_MAX*sizeof(unsigned char));
		pencarian = malloc(USERNAME_MAX*sizeof(unsigned char));
		printf("Masukkan nama website dari akun yang ingin dihapus\n");
		inputString(pencarian,1,USERNAME_MAX);
		
		for(ptr = *head_slave; ptr != NULL; prev = ptr, ptr = ptr->next){
			my_strcpy(decrypted_website,ptr->website);
			decrypt(decrypted_website,password);
			if(!my_strcasestr(decrypted_website, pencarian)){
				if(!my_strcmp(ptr->website,(**head_slave).website)){
					*head_slave = ptr->next;
				}else if(ptr->next == NULL){
					prev->next = NULL;
				}else{
					prev->next = ptr->next;
				}
				free(ptr);
        printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
				printf("\nAkun website %s telah terhapus!\n", decrypted_website);
				printf("Tekan ENTER untuk kembali\n");
        printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
				getchar();
				break; 
			}
		}
	
		if (ptr == NULL){
			printf("Tidak ada website dengan dengan nama \"%s\" pada akun. Silakan coba lagi!\n\n",pencarian);
		}
		
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Ingin menghapus akun lagi? (1. Ya 2. Tidak)\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		inputAngka(&again,1,2);
	}while(again == 1);

	free(decrypted_website);
	free(pencarian);
	ptr = NULL;
	prev = NULL;

	return false;
}
//function untuk bisa menunjukan semua akun slave
bool Show_All_Slave(struct SlaveAccount *head_slave, const unsigned char *password){  
	if (head_slave == NULL){
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Tidak ada akun. Silakan tambahkan akun terlebih dahulu!\n");
		printf("Tekan ENTER untuk kembali\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		getchar();
		return true;
	}
	struct SlaveAccount *ptr = NULL;
	unsigned char *decrypted_website = malloc(USERNAME_MAX*sizeof(unsigned char));
	unsigned char *decrypted_email = malloc(USERNAME_MAX*sizeof(unsigned char));
	unsigned char *decrypted_password = malloc(USERNAME_MAX*sizeof(unsigned char));

	for (ptr = head_slave; ptr != NULL; ptr = ptr->next){
		my_strcpy(decrypted_website,ptr->website);
		my_strcpy(decrypted_email,ptr->email);
		my_strcpy(decrypted_password,ptr->password);

		decrypt(decrypted_website,password);
		decrypt(decrypted_email,password);
		decrypt(decrypted_password,password);

		printf("\nWebsite: %s\n",decrypted_website);
		printf("Email: %s\n",decrypted_email);
		printf("Password: %s\n\n",decrypted_password);
	}
	free(decrypted_website);
	free(decrypted_email);
	free(decrypted_password);

	printf("Tekan ENTER untuk kembali\n");
	getchar();
	printf("\033[0;0H\033[2J"); //clear console replit

	return false;
}
//function untuk menghapus akun master yang juga akan menghapus akun slave yang berhubungan dengan akun master tersebut
bool Delete_Account(struct SlaveAccount *head){ 
	int yakin;
	struct SlaveAccount *ptr = NULL;

  printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
	printf("Apakah anda yakin untuk menghapus akun master seluruhnya?\n1. Yakin\n2. Tidak jadi\n");
  printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
	printf("Masukkan pilihan: ");
	inputAngka(&yakin,1,2);
	if(yakin == 1){
		for(ptr = head; ptr != NULL; head = ptr){
			ptr = ptr->next;
			free(head);
		}
		printf("Akun telah terhapus!\n");
		printf("Tekan ENTER untuk kembali\n");
		getchar();
		return true;
	}else{
		return false;
	}
}
//function untuk mengecek login sudah benar atau belum
bool Login_Success(struct MasterAccount *head, unsigned char *password){  
	unsigned char *decrypted_username = malloc(USERNAME_MAX*sizeof(unsigned char));
	my_strcpy(decrypted_username, head->username);
	decrypt(decrypted_username, password);
	bool deleted = false;

	int pilihan;

	do{
		printf("\033[0;0H\033[2J"); //clear console replit
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Selamat datang, %s!\n",decrypted_username);
		printf("Menu\n1. Lampirkan semua akun\n2. Tambah akun\n3. Cari akun\n4. Delete akun\n5. Delete akun master\n6. Logout\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Masukkan pilihan: ");
		inputAngka(&pilihan, 1,6);
		switch (pilihan){
			case 1: Show_All_Slave(head->slave,password);
					break;
			case 2: Add_Slave(&head->slave,password);
					break;
			case 3: Cari_Slave(head->slave,password);
					break;
			case 4: Delete_Slave(&head->slave,password);
					break;
			case 5: deleted = Delete_Account(head->slave);
					break;
			case 6: break;
		}
		 
	}while ( pilihan < 5 ) ;
	free(decrypted_username);
	
	return deleted;
}

bool Login(struct MasterAccount **head) { //function untuk mengecek login
	if (head == NULL){
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Database kosong! Silakan registrasi terlebih dahulu!\n");
		printf("Tekan ENTER untuk kembali\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		getchar();
		return true;
	}
	bool authenticated = false;
	int exit = 2;

	struct MasterAccount *ptr = NULL, *prev = NULL;

	unsigned char *temp_email = NULL;
	unsigned char *temp_password = NULL;
	unsigned char *temp_md5 = NULL;

	do {
		temp_email = malloc(355*sizeof(unsigned char));
		temp_password = malloc(101*sizeof(unsigned char));
		temp_md5 = malloc(16*sizeof(unsigned char));

    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("\n\nMasukkan email\t: ");
		inputString(temp_email,EMAIL_MIN,EMAIL_MAX);
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("\n\nMasukkan password\t: ");
		inputString(temp_password,PASSWORD_MIN,PASSWORD_MAX);
		
		my_strcat(temp_email,temp_password);

		md5(temp_email,my_strlen(temp_email),temp_md5);
		free(temp_email);

		for (ptr = *head; ptr != NULL; prev = ptr, ptr = ptr->next){
			if (!my_strcmp(ptr->md5_auth,temp_md5)){
				authenticated = true;
				printf("Telah berhasil login!");
				if(Login_Success(ptr, temp_password)){ //jika delete akun
					if(ptr == *head){
						*head = ptr->next;
					}else if(ptr->next == NULL){
						prev->next = NULL;
					}else{
						prev->next = ptr->next;
					}
					free(ptr);
				}
				break;
			}
		}

		if (ptr == NULL && !authenticated){
      printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
			printf("Email dan password salah!\n1. Input ulang email dan password\n2. Exit\n");
			inputAngka(&exit, 1, 2);
		}
		
		free(temp_password);
		free(temp_md5);
		ptr = NULL;
	} while (!authenticated && exit == 1);

	return false;
}

int main(void) {    //main function
	
	struct MasterAccount *head = NULL;
	int menu;
	readFile(&head);
	do{
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Selamat datang di Proglan Account Manager!\n");
 		printf("Silahkan pilih salah satu menu\n");
		printf("Menu\n1. Register\n2. Login\n3. Exit\n");
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
		printf("Masukkan pilihan: ");
		inputAngka(&menu,1,3);
		switch (menu){
			case 1: Registrasi(&head);
				break;
			case 2: Login(&head);
				break;
      case 3:
        printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
				printf("Terima kasih karena telah menggunakan aplikasi Account Manager!");
				printf("Tekan ENTER untuk kembali\n");
        printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n");
				getchar();
				break;
			default: 
      	break;
		}
		printf("\033[0;0H\033[2J"); //clear console replit		
	} while (menu != 3);
	
    createFileWithMasterAccount(head);
	return 0;
}

