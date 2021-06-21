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

// void createFileWithMasterAccount(FILE *fptr, char fileName[50], struct MasterAccount *head) {
//     struct MasterAccount *data;
// 	fptr = fopen("account.txt", "w");
// 	if (fptr != NULL) {
// 		if (ifFileNotNull(fptr, fileName)) {
// 			do{
// 				fprintf("%s\n", data->username);//masukin username master
// 				fprintf("%s\n", data->md5_auth);//md5 authorization
// 				do {
// 					fprintf("%s\n", data->slave->website);//masukin website
// 					fprintf("%s\n", data->slave->email);//masukin email
// 					fprintf("%s\n", data->slave->password);//masukin password
// 					data->slave = data->slave->next;
// 				} while(data != NULL);
//         	fprintf("***\n");//penanda slave terakhir
//         	data = data->next;//nextnya dipindah dulu ke data, nah berarti yang perlu dicek apakah datanya kosong ato ngga
// 			} while (data != NULL);
// 			printf("File %s berhasil dibuat!\n", fileName);
// 		} else printf("Ups, something went wrong");
// 	} else printf("Ups, something went wrong!\n");

// 	fclose(fptr); //wah ini pusing gw bacanya
// }

void createFileWithMasterAccount(FILE *fptr, struct MasterAccount *head, char fileName[50]) {
	struct MasterAccount *ptr = NULL;
	struct SlaveAccount *ptr_slave = NULL;

	int choice;
	
	fptr = fopen(fileName, "w");

	if (fptr != NULL) {
		if (!ifFileNotNull(fptr, fileName)) {

			int access = 0, choice;
			printf("File yang ingin dioverride tidak kosong, Apakah Anda ingin melanjutkan?\n 1). Yes\n 2).No\n");
			inputAngka(&choice,1,2);
			do {
				if (choice == 1) {
					for (ptr = head; ptr->>next != NULL; ptr = ptr->next){
						fprintf("%s\n", ptr->username);
						fprintf("%s\n", ptr->md5_auth);
						for (ptr_slave = head->slave; ptr_slave != NULL; ptr_slave = ptr_slave -> next){
							fprintf("%s\n", ptr_slave->website);
							fprintf("%s\n", ptr_slave->email);
							fprintf("%s\n", ptr_slave->password);
						}
						fprintf("***\n");
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