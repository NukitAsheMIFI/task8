#include <time.h>
#include <s21_matrix.h> //библиотека для работы с матрицами
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define SIZE 3

volatile sig_atomic_t got_sigint = 0;

void handler(int sig) {
	got_sigint = 1;
	(void)sig;
}

int main(){
	srand(time(NULL));
	matrix_t A, B, C;
	s21_create_matrix(SIZE, SIZE, &A); //создание матрицы
	s21_create_matrix(SIZE, SIZE, &B); 
	s21_create_matrix(SIZE, SIZE, &C);
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			A.matrix[i][j] = rand() % 10; //заполнение матрицы случайными числами
			B.matrix[i][j] = rand() % 10;
		}
	}

	signal(SIGINT, handler);
		
	//s21_mult_matrix(&A, &B, &C);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
        	C.matrix[i][j] = 0;
            if (got_sigint) {
            	printf("\n");
                printf("i=%d j=%d\n", i, j);
               	signal(SIGINT, SIG_DFL);
                got_sigint = 0;
            }
            
            for (int k = 0; k < SIZE; k++){
                C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
        	}
        	usleep(500000);
    	}
	}

	printf("Program came to the end\n");
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			printf("%.0f ", C.matrix[i][j]);
			
		}
		printf("\n");
	}	
	s21_remove_matrix(&A); //удаление матриц
	s21_remove_matrix(&B);
	s21_remove_matrix(&C);
	return 0;
}
