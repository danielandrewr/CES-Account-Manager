/*#include <stdio.h>
#include <stdlib.h>

void swapRow(int row, int column, double M[row][column], int i, int r){
	double temp[column];
	int j;
	
	for (j = 0; j<column; j++){
		temp[j] = M[i][j];
		M[i][j] = M[r][j];
		M[r][j] = temp[j];
	}
	
}
void printMatrix(int row, int column, double M[row][column]){
	int i, j;
	for (i = 0; i<row; i++){
		for (j = 0; j<column; j++){
			printf("%f ",M[i][j]);
		}
		puts("");
	}
	puts("");
}

//ini fungsi rref cmn beda algoritma
void rref2(int row, int column, double mat[row][column]) {
    int lead = 0;

    while (lead < row) {
    	
    	if (lead == row-1) 
    		printf("Result");
    	
        int k, l;
        double divisor, multiplier;

        for (k = 0; k < row; ++k) {
            divisor = mat[lead][lead];
            multiplier = mat[k][lead] / mat[lead][lead];

            for (l = 0; l < column; ++l) {
                if (k == lead) {
                    mat[k][l] = mat[k][l] / divisor;
                } else mat[k][l] -= mat[lead][l] * multiplier;
            }
        }

        lead++;
        printMatrix(row, column, mat);
    }
}

// ini dari wikipedia dengan sedikit modifikasi
int rref(int row, int column, double M[row][column]){
	int lead = 0;
	int r, i;
	double pivot;
	int k;
	
	for (r = 0; r<row; r++){
		if (column <= lead) {return 0;}
		i = r;
		while (M[i][lead] == 0){
			i++;
			if (row == i){
				i = r;
				lead++;
				if (column == lead){return 0;}
			}
		}
		if (i != r){swapRow(row,column,M,i,r);}
		//printMatrix(row,column,M); //debugging
		if (M[r][lead] != 1){
			pivot = M[r][lead];
			for (k = 0; k<column; k++){
				M[r][k] /= pivot;
				if (M[r][k] == 0.0){M[r][k] = 0.0;}
				}
		}
		//printMatrix(row,column,M); //debugging
		for (i = 0; i<row; i++){
			pivot = M[i][lead];
			if ( i!=r ){
				for (k = 0; k<column;k++){
					M[i][k] -= pivot * M[r][k];
				}
			}
			
		}
		lead++;
		//printMatrix(row,column,M); debugging
	}
	return 0;
}


void ref_only(int row, int column, double M[row][column]){
	double x;
	int r, allZeros, c;
	for (r = 0; r<row; r++){
		allZeros = 1;
		for (c = 0; c<column; c++){
			if (M[r][c] != 0){
				allZeros = 0;
				break;
			}
		}
		if (allZeros == 1){
			swapRow(row,column,M,r,row);
			row--;
		}
	}
	int p = 0;
	while (p < row && p <column){
		nextPivot:
			r = 1;
			while(M[p][p] == 0){
				if (p+r <= row){
					p++;
					goto nextPivot;
				}
				swapRow(row,column,M,p,p+r);
				r++;
			}
			for (r = 1; r< row-p; r++){
				if (M[p+r][p] != 0){
					x = -1.0 * M[p+r][p] / M[p][p];
					for (c = p; c<column; c++){M[p+r][c] = M[p][c] * x + M[p+r][c];}
				}
			}
			p++;
	}
}





int main(void){
	int row, column, i,j;
	printf("Masukkan jumlah baris matriks: ");
	scanf("%d", &row);
	printf("Masukkan jumlah kolom matriks: ");
	scanf("%d", &column);

	double M[row][column];
	printf("Masukkan isi matriks\n");
	for (i = 0; i<row; i++){
		for (j = 0; j<column; j++){
			scanf("%lf", &M[i][j]);
		}
	}
	rref(row,column,M);

	printf("Hasil Reduced Row Echelon Form nya adalah: \n");
	printMatrix(row,column,M);
	
	
	
	
	return 0;
}
*/