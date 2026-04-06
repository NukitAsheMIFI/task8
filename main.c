#include <time.h>
#include <s21_matrix.h> //библиотека для работы с матрицами
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 3

volatile sig_atomic_t got_sigint = 0;

void my_handler(int sig) { //обработка ctrl-c
	got_sigint = 1;
	(void)sig;
}

void interruptor(int use_sa, void (*handler)(int)){ //функция разделяющая системные вызовы signal и sigaction
	if (use_sa == 1) {
		struct sigaction sa; //объявление структуры sigaction
		sigemptyset(&sa.sa_mask); //очищение маски
		sa.sa_handler = my_handler; 
		sa.sa_flags = 0; //обнуление флагов
		sigaction(SIGINT, &sa, NULL); 
	}
	else {
		signal(SIGINT, handler);
	}
}

int main(int argc, char **argv){
	if (argc < 2){
		printf("Недостаточно аргументов\n");
		return 1;
	}
	char *mode = argv[1];
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
	int use_sa;
	if (strcmp(mode, "--signal") == 0){
		use_sa = 0; //использование signal
	}
	else if (strcmp(mode, "--sigaction") == 0){
		use_sa = 1; //использование sigaction
	}
	else {
		printf("неизвестный системный вызов");
		return 1;
	}
	interruptor(use_sa, my_handler); //передача функции, принимающей int
	//s21_mult_matrix(&A, &B, &C);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
        	C.matrix[i][j] = 0;
            if (got_sigint) { //обнаружен ctrl-C
            	printf("\n");
                printf("i=%d j=%d\n", i, j);
               	interruptor(use_sa, SIG_DFL); //возвращение к стандартному поведению
                got_sigint = 0; 
            }
            for (int k = 0; k < SIZE; k++){
                C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
        	}
        	usleep(500000); //задержка полсекунды
    	}
	}

	printf("Program came to the end\n");
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			printf("%.0f ", C.matrix[i][j]); //вывод элементов матрицы
			
		}
		printf("\n");
	}	
	s21_remove_matrix(&A); //удаление матриц
	s21_remove_matrix(&B);
	s21_remove_matrix(&C);
	return 0;
}
