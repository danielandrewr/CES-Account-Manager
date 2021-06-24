/* 
header ini berisi fungsi - fungsi file handling untuk menyimpan dan membuka akun
dalam suatu file database. 
*/
#ifndef FILEHANDLER
#define FILEHANDLER

#include <stdio.h>
#include <stdlib.h>
#include "parallel_string.h"
const unsigned char penanda[] = "Z6@z#6iHx@4U%ncK^m$^";
//fungsi mencetak nilai hex dari string
void fprints(FILE *fptr,const unsigned char *string){
	register int i;
   for (i=0;string[i] != '\0';i++) {
      fprintf(fptr,"%2.2x", string[i]);
   }
}
//fungsi untuk mengubah nilai hex dari file txt menjadi unsigned char dan memasukkannya ke pointer string
unsigned char *Fgets(FILE *fptr, unsigned char *string){
	char *temp = (char*) malloc(EMAIL_MAX*(sizeof(char)));
	unsigned char *temp2 = (unsigned char*)malloc(3*sizeof(unsigned char));
	temp2[2] = '\0';
	fgets(temp,EMAIL_MAX,fptr);
	register int i, j, len = my_strlen((unsigned char*)temp);
	for (i = 0, j = 0; i<len; i +=2,j++){
		temp2[0] = temp[i];
		temp2[1] = temp[i+1];
		string[j] = (unsigned char)strtol((char*)temp2, NULL, 16);
	}
	free(temp);
	free(temp2);
	temp = NULL;
	temp2 = NULL;
	return string;
}
// fungsi untuk mengecek apakah filenya memiliki isi
bool ifFileNotNull(FILE *fptr) {
	bool fileNotNull = false;

	fptr = fopen(fileName, "r");
	if (fptr != NULL) {
		fseek(fptr, 0, SEEK_END);
		int fsize = ftell(fptr);
		if (fsize != 0)
			fileNotNull = true;

	} else printf("\nUps, Something went wrong!\n");

	fclose(fptr);
	return fileNotNull;
}


//function untuk menciptakan file baru untuk disimpan, dan akan digunakan pada run kedepannya
bool createFileWithMasterAccount(struct MasterAccount * head) {
    struct MasterAccount * ptr = NULL;
    struct SlaveAccount * ptr_slave = NULL;
    FILE * fptr;
    fptr = fopen(fileName, "r");
    if (fptr == NULL) { //jika file baru
        fptr = fopen(fileName, "w");
    }

    if (fptr != NULL) {
        if (ifFileNotNull(fptr)) {
            int choice;
            printf("File yang ingin dioverride tidak kosong, Apakah Anda ingin melanjutkan?\n 1). Yes\n 2). No\n");
            inputAngka( & choice, 1, 2);

            if (choice == 2) {
                fclose(fptr);
                return false;
            }
        } 
        fptr = fopen(fileName, "w");
        for (ptr = head; ptr != NULL; ptr = ptr -> next) {
            fprints(fptr,ptr -> username);
            fprintf(fptr,"\n");
            fprints(fptr,ptr -> md5_auth);
            fprintf(fptr,"\n");
            for (ptr_slave = ptr -> slave; ptr_slave != NULL; ptr_slave = ptr_slave -> next) {
                fprints(fptr, ptr_slave -> website);
                fprintf(fptr,"\n");
                fprints(fptr, ptr_slave -> email);
                fprintf(fptr,"\n");
                fprints(fptr, ptr_slave -> password);
                fprintf(fptr,"\n");
            }
            fprints(fptr, penanda);
			if (ptr->next != NULL){
				fprintf(fptr,"\n");
			}
        }
        printf("\nFile berhasil dibuat!\n");
        
    }
    fclose(fptr);
	return true;
}

//function untuk membaca file yang sebelumnya sudah disimpan
void readFile(struct MasterAccount ** head) {  
    struct MasterAccount * temp = NULL, * ptr = NULL;
    struct SlaveAccount * temp_slave = NULL, * ptr_slave = NULL;
    unsigned char *checker = (unsigned char*) malloc(EMAIL_MAX*sizeof(char));
	
  	FILE *fptr;
    fptr = fopen(fileName, "r");
    if (fptr != NULL) {
        if (ifFileNotNull(fptr)) {
            int access = 0, choice;
            printf("File ditemukan!\n");
			fptr = fopen(fileName, "r");
            while (!feof(fptr)) {
                temp =(struct MasterAccount*) malloc(sizeof(struct MasterAccount));
				temp -> next = NULL;
                temp -> slave = NULL;
				
				Fgets(fptr,temp->username);
				Fgets(fptr,temp->md5_auth);
             
                while (my_strcmp(Fgets(fptr, checker), penanda) != 0) {
                    temp_slave = (struct SlaveAccount*)malloc(sizeof(struct SlaveAccount));
					my_strcpy(temp_slave -> website, checker);
                    Fgets(fptr,temp_slave->email);
                    Fgets(fptr,temp_slave->password);
                    temp_slave -> next = NULL;
                    if (temp -> slave == NULL) {
                        temp -> slave = temp_slave;
                        ptr_slave = temp_slave;
                    } else {
                        ptr_slave -> next = temp_slave;
                        ptr_slave = ptr_slave -> next;
                    }
                }
				
                if (*head == NULL) {
                    *head = temp;
                    ptr = temp;
                } else {
                    ptr -> next = temp;
                    ptr = ptr -> next;
                }
            }
		fclose(fptr);
        }
    } else {
        printf("File Gagal Ditemukan!\n");
		printf("Tekan ENTER untuk melanjutkan\n");
		getchar();
		system("cls");
    }
	
	free(checker);
	checker = NULL;
}

#endif //FILEHANDLER