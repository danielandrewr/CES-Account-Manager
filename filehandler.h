#ifndef FILEHANDLER
#define FILEHANDLER

#include <stdio.h>
#include <stdlib.h>
const unsigned char penanda[] = "Z6@z#6iHx@4U%ncK^m$^";
//fungsi mencetak nilai hex dari string
void fprints(FILE *fptr,const unsigned char *string){
	register int i;
   for (i=0;string[i] != '\0';i++) {
      fprintf(fptr,"%2.2x", string[i]);
   }
   fprintf(fptr,"\n");
}
unsigned char *Fgets(FILE *fptr, unsigned char *string){
	char *temp = malloc(EMAIL_MAX*(sizeof(char)));
	unsigned char *temp2 = malloc(3*sizeof(unsigned char));
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
	return string;
}
// fungsi ngecek filenya ada isi
bool ifFileNotNull(FILE *fptr) {
	bool fileNotNull = false;

	fptr = fopen(fileName, "r");
	if (fptr != NULL) {
		fseek(fptr, 0, SEEK_END);
		int fsize = ftell(fptr);
		// printf("%d", fsize);
		if (fsize != 0)
			fileNotNull = true;

	} else printf("\nUps, Something went wrong!\n");

	fclose(fptr);
	return fileNotNull;
}


//function untuk menciptakn file baru untuk disimpah, dan akan digunakan pada run kedepannya
bool createFileWithMasterAccount(struct MasterAccount * head) {
    struct MasterAccount * ptr = NULL;
    struct SlaveAccount * ptr_slave = NULL;
    FILE * fptr;
    fptr = fopen(fileName, "r");
    if (fptr == NULL) { //kalo file baru
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
        } //sama aja gasi
        fptr = fopen(fileName, "w");
        for (ptr = head; ptr != NULL; ptr = ptr -> next) {
            fprints(fptr,ptr -> username);
            fprints(fptr,ptr -> md5_auth);
            for (ptr_slave = ptr -> slave; ptr_slave != NULL; ptr_slave = ptr_slave -> next) {
                fprints(fptr, ptr_slave -> website);
                fprints(fptr, ptr_slave -> email);
                fprints(fptr, ptr_slave -> password);
            }
            
			if (ptr->next == NULL){ //kalo dia ini masteraccount terakhir
				fprints(fptr, penanda);
			}else{//ngga perlu kan?
				fprints(fptr, penanda);//sori yang ini blm ngeh bedanya apa sama yang diatas harusnya \n
			}

        }
        printf("\nFile berhasil dibuat!\n");
        // klo dua ga lanjutin apa'
    }
    fclose(fptr);
	return true;
}

//function untuk membaca file yang sebelumnya sudah disimpan
void readFile(struct MasterAccount ** head) {  
    struct MasterAccount * temp = NULL, * ptr = NULL;
    struct SlaveAccount * temp_slave = NULL, * ptr_slave = NULL;
    unsigned char *checker = malloc(EMAIL_MAX*sizeof(char));
	
  	FILE *fptr;
    fptr = fopen(fileName, "r");
	//apa jangan jangan segmentation faultnya yang di 33
    if (fptr != NULL) {
        if (ifFileNotNull(fptr)) {
            int access = 0, choice;
            printf("File ditemukan!\n");
			fptr = fopen(fileName, "r");
            while (!feof(fptr)) {
                temp = malloc(sizeof(struct MasterAccount));
				temp -> next = NULL;
                temp -> slave = NULL;
				//fscanf(fptr,"%[^\n]hhu", temp->username);
				Fgets(fptr,temp->username);
              // fgets((char*)temp -> username, USERNAME_MAX, fptr);
				//printf("%s\n",temp->username);
				Fgets(fptr,temp->md5_auth);
             //  fgets((char*)temp -> md5_auth, MD5_SIZE, fptr);
			   //fscanf(fptr,"%[^\n]hhu", temp->md5_auth);
				//printf("%s\n",temp->md5_auth);
				//fgets(checker, EMAIL_MAX, fptr);
				//puts(checker);
                
				// iya coba aj
				//printf("%s\n",temp->username); //gw coba debug
				//printf("%s\n",temp->md5_auth);
				//getchar();
                while (my_strcmp(Fgets(fptr, checker), penanda) != 0) {
					//printf("%s\n",checker);
                    temp_slave = malloc(sizeof(struct SlaveAccount));
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
				printf("Debugging\n"); // disini dibaca
				
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
		printf("\033[0;0H\033[2J"); //clear console replit	
    }
	
	free(checker);
}

#endif //FILEHANDLER