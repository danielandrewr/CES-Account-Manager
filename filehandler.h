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

void createFileWithMasterAccount(FILE *fptr, char fileName[50], struct MasterAccount *head) {
  struct MasterAccount *data;
	fptr = fopen("account.txt", "w");
	if (fptr != NULL) {
		if (ifFileNotNull(fptr, fileName)) {
			do{
				fprintf("MasterAccount 1\n");//kayaknya ini gausa, kan encrypted text, langsung aja user hash
				fprintf("\tUsername: ");
				fprintf("\tmd5_auth: ");
        do{
          fprintf("inislave");
          data->slave = data->slave->next;
        }while(data->slave != NULL);
        data = data->next;
			} while (data->next != NULL);
			printf("File %s berhasil dibuat!\n", fileName);
		} else printf("Ups, something went wrong");
	} else printf("Ups, something went wrong!\n");

	fclose(fptr); //wah ini pusing gw bacanya
}

void addAccountToFile(struct MasterAccount *head){
  fptr = fopen("account.txt","w");
  if (fptr != NULL){
    ?????
  }
}