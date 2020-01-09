#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

typedef struct matrix {
	char name; //이름
	int** elements; //행렬 배열
	int row; //행
	int column; //열
}matrix;


FILE* file_pointer; //입력 파일 포인터
matrix* Matrix; //파일에서 불러온 행렬 구조체
int matrix_num;
char str[60]; //연산 문자열
char** divide_str; //'+', '-'에 따라 연산 문자열을 나눠 저장하는 문자형 이차원 배열
int cnt; //'+', '-' 개수를 나타내는 정수
int flag; //연산 가능 불가능 여부 판단 정수

void printMatrix(matrix M); //행렬 출력 함수

void fetchMatrix(); //파일 패치 함수
void searchFile(); //입력한 파일이 있는지 여부를 확인, 동작하는 함수
void checkMatrixNum(); //파일 안의 행렬의 개수를 세는 함수

int getMinimumDet(matrix smallMatrix); //2x2 행렬의 det값을 계산하는 함수
int calculateDet(matrix major_matrix, int size); //전체 행렬의 det값을 cofactor를 통해 계산하는 함수

void main() {
	int i;
	matrix_num = 0;

	searchFile();

	for (i = 0; i < matrix_num; i++) {
		printf("det(%c) = ", Matrix[i].name);

		if (Matrix[i].row == Matrix[i].column) { //square matrix 여부 확인
			printf("%d\n", calculateDet(Matrix[i], Matrix[i].row));
		}
		else {
			printf("계산할 수 없습니다.\n");
		}
	}

	return;
}

int getMinimumDet(matrix smallMatrix) {
	int det;

	det = smallMatrix.elements[0][0] * smallMatrix.elements[1][1] - smallMatrix.elements[0][1] * smallMatrix.elements[1][0];

	return det;
}
int calculateDet(matrix major_matrix, int size) {
	matrix* minor_matrix;
	int* det; //cofactor
	int D = 0; //전체 det 값
	int i, j, k;

	if (size == 1) { //자기 자신이 det
		return major_matrix.elements[0][0];
	}
	else if (size == 2) {
		return getMinimumDet(major_matrix);
	}

	minor_matrix = (matrix*)malloc(sizeof(matrix) * size);
	det = (int*)malloc(sizeof(int) * size);

	for (i = 0; i < size; i++) { //minor_matrix의 크기 설정
		minor_matrix[i].elements = (int**)malloc(sizeof(int*) * (size - 1)); //row
		for (j = 0; j < size - 1; j++) {
			minor_matrix[i].elements[j] = (int*)malloc(sizeof(int) * (size - 1)); //column
		}
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < size - 1; j++) {
			for (k = 0; k < size - 1; k++) {
				if (i == 0) {
					minor_matrix[i].elements[j][k] = major_matrix.elements[j + 1][k + 1];
				}
				else if (i > 0) {
					if (i > k) {
						minor_matrix[i].elements[j][k] = major_matrix.elements[j + 1][k];
					}
					else if (i <= k) {
						minor_matrix[i].elements[j][k] = major_matrix.elements[j + 1][k + 1];
					}
				}
			}
		}
	}

	for (i = 0; i < size; i++) { //minor_matrix의 det값 계산
		det[i] = calculateDet(minor_matrix[i], size - 1);
	}

	for (i = 0; i < size; i++) { //부호 설정해서 전체 det값 계산
		if (i % 2 == 0) {
			D += det[i] * major_matrix.elements[0][i];
		}
		else {
			D -= det[i] * major_matrix.elements[0][i];
		}
	}

	return D;
}

void printMatrix(matrix M) {
	int i, j;

	for (i = 0; i < M.row; i++) {
		for (j = 0; j < M.column; j++) {
			printf("%d ", M.elements[i][j]);
		}
		printf("\n");
	}

	return;
}

void checkMatrixNum() {
	char find_alphabet;

	while (!feof(file_pointer)) {
		fscanf(file_pointer, "%c", &find_alphabet);

		if (find_alphabet >= 'A' && find_alphabet <= 'Z') {
			matrix_num++;
		}
	}
	return;
}
void fetchMatrix() {
	int i, j, k;
	int tmp;
	char nothing;

	for (i = 0; i < matrix_num; i++) {
		fscanf(file_pointer, "%c", &Matrix[i].name);
		fscanf(file_pointer, "%c", &nothing);
		fscanf(file_pointer, "%d", &Matrix[i].row);
		fscanf(file_pointer, "%c", &nothing);
		fscanf(file_pointer, "%d", &Matrix[i].column);

		Matrix[i].elements = (int**)malloc(sizeof(int*) * Matrix[i].row);
		for (j = 0; j < Matrix[i].row; j++) {
			Matrix[i].elements[j] = (int*)malloc(sizeof(int) * Matrix[i].column);
		}

		for (j = 0; j < Matrix[i].row; j++) {
			for (k = 0; k < Matrix[i].column; k++) {
				fscanf(file_pointer, "%d", &tmp);

				Matrix[i].elements[j][k] = tmp;

				fscanf(file_pointer, "%c", &nothing);
			}
		}

		printf("행렬 %c = \n", Matrix[i].name);
		printMatrix(Matrix[i]);
		printf("--------------\n");
	}

	return;
}
void searchFile() {
	char file_root[10] = "\0";
	char file_path[100] = "\0";
	char file_name[20] = "\0";
	int result;

	strcat(file_root, "./");

	while (1) {
		printf("확장자명(.txt)을 제외한 파일 이름을 입력하세요: ");
		scanf("%s", file_name);

		strcat(file_name, ".txt");
		strcat(file_path, file_root);
		strcat(file_path, file_name);

		result = access(file_path, 0);

		if (result == 0) {
			printf("해당 파일이 존재하므로 행렬을 불러옵니다.\n");
			break;
		}
		else {
			printf("해당 파일이 존재하지 않습니다.\n");
			file_path[0] = '\0';
		}
	}

	file_pointer = fopen(file_path, "r");
	checkMatrixNum();
	fclose(file_pointer);

	Matrix = (matrix*)malloc(sizeof(matrix) * matrix_num);

	file_pointer = fopen(file_path, "r");
	fetchMatrix();
	fclose(file_pointer);

	return;
}
