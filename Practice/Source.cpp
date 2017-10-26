#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
const int N = 1440;// ������ ������� � ���������� ������������� N*N
int d1 = 1;//������ �����
int d2 = 1;//������ ����� d1xd2
int COUNT_BLOCKS = N / d1; //���������� ������- � ������
int COUNT_OF_DEVISOR = 36;//���������� ���������
const string NAME_FILE_DEVISOR = "devisor.txt"; // ��� ����� � ���������� 
const string NAME_FILE_RESULT = "result.txt"; // ��� ����� ������(���������)

int addr(int i, int j) {	//������� ���������� �������� �� �������� �������� ������� // (i,j) ��� i-������ j-������� i,j=0..N				
	return ((i / d2)*d1*d2) + ((j / d1)*(N / d2)*d2*d1) + ((i%d1)*d2) + (j%d2);
}

int* create_squad_matrix(int *matrix) {	//������� �������� ���������� �������				
	for (int i = 0; i<N; i++) {
		for (int j = 0; j < N; j++) {
			*(matrix + addr(i, j)) = rand() % 200 + 5;
		}
	}
	return matrix;
}
int* create_triangle_matrix(int *matrix) { // ������� �������� ������-����������� //�������
	for (int i = 0; i<N; i++) {
		for (int j = 0; j < N; j++) {
			if (i <= j)*(matrix + addr(i, j)) = rand() % 200 + 5;
			else *(matrix + addr(i, j)) = 0;
		}
	}
	return matrix;
}

void print_matrix(int *matrix) { //������ ������� 
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << *(matrix + addr(i, j)) << "		";
		}
		cout << endl;
		cout << endl;
	}
}
void Block_Mult(int *BlockA, int *BlockB, int *BlockC) { //��������� ��������� ������ //������ A*B=C ��� BlockA,BlockB- ������ ������������� ������ BlockC -����� //��������������� �����
	for (int i = 0; i < d1; i++) {
		for (int j = 0; j < d1; j++) {
			for (int r = 0; r < d1; r++) {
				*(BlockC + d1*i + j%d1) += *(BlockA + d1*i + r%d1)*(*(BlockB + d1*r + j%d1));
			}
		}
	}
}
void Block_Matrix_Mult(int *A, int *B, int *C) { //��������� ��������� ������
	int *BlockA, *BlockB, *BlockC;
	for (int i = 0; i <COUNT_BLOCKS; i++) { // C_ij=�����(A_is*B_sj)
		for (int j = 0; j < COUNT_BLOCKS; j++) {
			for (int k = 0; k < COUNT_BLOCKS; k++) {
				BlockA = A + d1*d2*i + d1*d2*COUNT_BLOCKS*k; //  ���������� //���������� ������ ����� ������� �
				BlockB = B + d1*d2*k + d1*d2*COUNT_BLOCKS*j; //	���������� //���������� ������ ����� ������� B
				BlockC = C + d1*d2*i + d1*d2*COUNT_BLOCKS*j; //���������� //���������� ������ ����� ������� C
				Block_Mult(BlockA, BlockB, BlockC);
			}
		}
	}
}

int main() {
	int *matrix_squad = new int[N*N];
	int *matrix_triangle = new int[N*N];
	int *matrix_result = new int[N*N];
	ifstream file_devisor(NAME_FILE_DEVISOR); //����������� ��������� ������ ��� //������ ���������
	ofstream file_result(NAME_FILE_RESULT);//���������� ��������� ������ ��� //������ �����������
	for (int i = 0; i < N*N; i++)matrix_result[i] = 0;// ��������� �������������� //�������
	create_squad_matrix(matrix_squad);
	create_triangle_matrix(matrix_triangle);
	//print_matrix(matrix_squad); 
	//print_matrix(matrix_triangle);
	cout << endl;
	double search_time;
	for (int i = 0; i < COUNT_OF_DEVISOR; i++) {
		int temp;
		file_devisor >> temp; //������ �� ����� ��������� ��������
		d1 = d2 = temp; //������������� ������ �����
		COUNT_BLOCKS = N / d1;//������� ���������� ������ � ������
		unsigned int start_time = clock();
		Block_Matrix_Mult(matrix_squad, matrix_triangle, matrix_result);
		unsigned int end_time = clock();
		search_time = end_time - start_time;
		file_result << search_time << " "; //���������� ��������� � ����, ����� //������� ����� ������-�����������
	}
	cout << "Search time =" << search_time << endl;
	//print_matrix(matrix_result);
	file_result.close(); //�������� ������
	file_devisor.close();
	delete[] matrix_squad;//������ ������
	delete[] matrix_triangle;
	delete[] matrix_result;
	system("pause");
}
