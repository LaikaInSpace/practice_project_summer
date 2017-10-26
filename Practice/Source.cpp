#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
const int N = 1440;// размер матрицы в нормальном представлении N*N
int d1 = 1;//Размер блока
int d2 = 1;//Размер блока d1xd2
int COUNT_BLOCKS = N / d1; //Количество блоков- в строке
int COUNT_OF_DEVISOR = 36;//Количество делителей
const string NAME_FILE_DEVISOR = "devisor.txt"; // Имя файла с делителями 
const string NAME_FILE_RESULT = "result.txt"; // Имя файла вывода(результат)

int addr(int i, int j) {	//Функция вычесления смещения по индексам элемента матрицы // (i,j) где i-строка j-столбец i,j=0..N				
	return ((i / d2)*d1*d2) + ((j / d1)*(N / d2)*d2*d1) + ((i%d1)*d2) + (j%d2);
}

int* create_squad_matrix(int *matrix) {	//Функция создания квадратной матрицы				
	for (int i = 0; i<N; i++) {
		for (int j = 0; j < N; j++) {
			*(matrix + addr(i, j)) = rand() % 200 + 5;
		}
	}
	return matrix;
}
int* create_triangle_matrix(int *matrix) { // Функция создания верхне-треугольной //матрицы
	for (int i = 0; i<N; i++) {
		for (int j = 0; j < N; j++) {
			if (i <= j)*(matrix + addr(i, j)) = rand() % 200 + 5;
			else *(matrix + addr(i, j)) = 0;
		}
	}
	return matrix;
}

void print_matrix(int *matrix) { //Печать матрицы 
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << *(matrix + addr(i, j)) << "		";
		}
		cout << endl;
		cout << endl;
	}
}
void Block_Mult(int *BlockA, int *BlockB, int *BlockC) { //Процедура умножения блоков //матриц A*B=C Где BlockA,BlockB- адреса перемножаемых блоков BlockC -адрес //результирующего блока
	for (int i = 0; i < d1; i++) {
		for (int j = 0; j < d1; j++) {
			for (int r = 0; r < d1; r++) {
				*(BlockC + d1*i + j%d1) += *(BlockA + d1*i + r%d1)*(*(BlockB + d1*r + j%d1));
			}
		}
	}
}
void Block_Matrix_Mult(int *A, int *B, int *C) { //Процедура умножения матриц
	int *BlockA, *BlockB, *BlockC;
	for (int i = 0; i <COUNT_BLOCKS; i++) { // C_ij=Сумма(A_is*B_sj)
		for (int j = 0; j < COUNT_BLOCKS; j++) {
			for (int k = 0; k < COUNT_BLOCKS; k++) {
				BlockA = A + d1*d2*i + d1*d2*COUNT_BLOCKS*k; //  Нахождения //очередного адреса блока матрицы А
				BlockB = B + d1*d2*k + d1*d2*COUNT_BLOCKS*j; //	Нахождения //очередного адреса блока матрицы B
				BlockC = C + d1*d2*i + d1*d2*COUNT_BLOCKS*j; //Нахождения //очередного адреса блока матрицы C
				Block_Mult(BlockA, BlockB, BlockC);
			}
		}
	}
}

int main() {
	int *matrix_squad = new int[N*N];
	int *matrix_triangle = new int[N*N];
	int *matrix_result = new int[N*N];
	ifstream file_devisor(NAME_FILE_DEVISOR); //Определения файлового потока для //чтения делителей
	ofstream file_result(NAME_FILE_RESULT);//Опередение файлового потока для //записи результатов
	for (int i = 0; i < N*N; i++)matrix_result[i] = 0;// обнуление результирующей //матрицы
	create_squad_matrix(matrix_squad);
	create_triangle_matrix(matrix_triangle);
	//print_matrix(matrix_squad); 
	//print_matrix(matrix_triangle);
	cout << endl;
	double search_time;
	for (int i = 0; i < COUNT_OF_DEVISOR; i++) {
		int temp;
		file_devisor >> temp; //Читаем из файла очередной делитель
		d1 = d2 = temp; //Устанавливаем размер блока
		COUNT_BLOCKS = N / d1;//Находим количество блоков в строке
		unsigned int start_time = clock();
		Block_Matrix_Mult(matrix_squad, matrix_triangle, matrix_result);
		unsigned int end_time = clock();
		search_time = end_time - start_time;
		file_result << search_time << " "; //Записываем результат в файл, можно //выбрать любой символ-разделитель
	}
	cout << "Search time =" << search_time << endl;
	//print_matrix(matrix_result);
	file_result.close(); //закрытие файлов
	file_devisor.close();
	delete[] matrix_squad;//чистка памяти
	delete[] matrix_triangle;
	delete[] matrix_result;
	system("pause");
}
