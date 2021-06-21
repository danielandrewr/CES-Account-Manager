// fungsi ngecek filenya ada isi
bool ifFileNotNull(FILE *fptr, char fileName[50]) {
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

void createFileWithMasterAccount(FILE *fptr, struct MasterAccount *head, char fileName[50]) {
	struct MasterAccount *ptr = NULL;
	struct SlaveAccount *ptr_slave = NULL;
	unsigned char *temp_md5 = NULL;
	unsigned char *temp_concat = NULL;
	int choice;
	
	fptr = fopen(fileName, "w");

	if (fptr != NULL) {
		if (!ifFileNotNull(fptr, fileName)) {

			int access = 0, choice;
			printf("File yang ingin dioverride tidak kosong, Apakah Anda ingin melanjutkan?\n 1). Yes\n 2).No\n");
			inputAngka(&choice,1,2);
			do {
				if (choice == 1) {
					for (ptr = head; ptr->next != NULL; ptr = ptr->next){
						fprintf("%s\n", ptr->username);
						fprintf("%s\n", ptr->md5_auth);
						for (ptr_slave = ptr->slave; ptr_slave != NULL; ptr_slave = ptr_slave -> next){
							fprintf("%s\n", ptr_slave->website);
							fprintf("%s\n", ptr_slave->email);
							fprintf("%s\n", ptr_slave->password);
						}
						temp_md5 = malloc(MD5_MAX*sizeof(unsigned char));
						temp_concat = malloc((USERNAME_MAX+MD5_MAX)*sizeof(unsigned char));
						my_strcpy(temp_concat,ptr->username);
						my_strcat(temp_concat,ptr->md5_auth);
						md5(temp_concat,strlen(temp_concat),temp_md5);
						free(temp_concat);
						fprintf("%s\n",temp_md5);
						free(temp_md5);
					}
				} else if (choice == 2) {
					access = -1;
					break;
				} else {
					printf("Tolong masukkan input yang valid!\n");
				}
			} while (access != -1);
		}
	}
}

void readFile(FILE * fptr, struct MasterAccount * head, char fileName[50]) {
    struct MasterAccount * temp, * ptr;
    struct SlaveAccount * temp_slave, * ptr_slave;
    unsigned char *checker = malloc(EMAIL_MAX*sizeof(unsigned char));
	unsigned char *temp_md5 = NULL;
	unsigned char *temp_concat = NULL;
    fptr = fopen(fileName, "r");
	
    if (fptr != NULL) {
        if (!ifFileNotNull(fptr, fileName)) {
            int access = 0, choice;
            printf("File ditemukan!");
            ptr = head;
            while (fptr != EOF) {
                temp = malloc(sizeof(struct MasterAccount));
                fgets(temp -> username, USERNAME_MAX, fptr);
                fgets(temp -> md5_auth, MD5_SIZE, fptr);
                temp -> next = NULL;
                temp -> slave = NULL;

				temp_md5 = malloc(MD5_MAX*sizeof(unsigned char));
				temp_concat = malloc((USERNAME_MAX+MD5_MAX)*sizeof(unsigned char));
				my_strcpy(temp_concat,ptr->username);
				my_strcat(temp_concat,ptr->md5_auth);
				md5(temp_concat,strlen(temp_concat),temp_md5);
				free(temp_concat);
                while (my_strcmp(fgets(checker, EMAIL_MAX, fptr), temp_md5)!=0) {
                    temp_slave = malloc(sizeof(struct SlaveAccount));
					strcpy(temp_slave -> website, checker);
                    fgets(temp_slave -> email, EMAIL_MAX, fptr);
                    fgets(temp_slave -> password, PASSWORD_MAX, fptr);
                    temp_slave -> next = NULL;
                    if (temp -> slave == NULL) {
                        temp -> slave = temp_slave;
                        ptr_slave = temp_slave;
                    } else {
                        ptr_slave -> next = temp_slave;
                        ptr_slave = ptr_slave -> next;
                    }
                }
                if (head == NULL) {
                    head = temp;
                    ptr = temp;
                } else {
                    ptr -> next = temp;
                    ptr = ptr -> next;
                }
				free(temp_md5);
            }
        }
    } else {
        printf("File Gagal Ditemukan!");
    }
	free(checker);
}