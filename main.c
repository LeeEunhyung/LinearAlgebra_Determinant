#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

typedef struct matrix {
	char name; //�̸�
	int** elements; //��� �迭
	int row; //��
	int column; //��
}matrix;


FILE* file_pointer; //�Է� ���� ������
matrix* Matrix; //���Ͽ��� �ҷ��� ��� ����ü
int matrix_num;
char str[60]; //���� ���ڿ�
char** divide_str; //'+', '-'�� ���� ���� ���ڿ��� ���� �����ϴ� ������ ������ �迭
int cnt; //'+', '-' ������ ��Ÿ���� ����
int flag; //���� ���� �Ұ��� ���� �Ǵ� ����

void printMatrix(matrix M); //��� ��� �Լ�

void fetchMatrix(); //���� ��ġ �Լ�
void searchFile(); //�Է��� ������ �ִ��� ���θ� Ȯ��, �����ϴ� �Լ�
void checkMatrixNum(); //���� ���� ����� ������ ���� �Լ�

int getMinimumDet(matrix smallMatrix); //2x2 ����� det���� ����ϴ� �Լ�
int calculateDet(matrix major_matrix, int size); //��ü ����� det���� cofactor�� ���� ����ϴ� �Լ�

void main() {
	int i;
	matrix_num = 0;

	searchFile();

	for (i = 0; i < matrix_num; i++) {
		printf("det(%c) = ", Matrix[i].name);

		if (Matrix[i].row == Matrix[i].column) { //square matrix ���� Ȯ��
			printf("%d\n", calculateDet(Matrix[i], Matrix[i].row));
		}
		else {
			printf("����� �� �����ϴ�.\n");
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
	int D = 0; //��ü det ��
	int i, j, k;

	if (size == 1) { //�ڱ� �ڽ��� det
		return major_matrix.elements[0][0];
	}
	else if (size == 2) {
		return getMinimumDet(major_matrix);
	}

	minor_matrix = (matrix*)malloc(sizeof(matrix) * size);
	det = (int*)malloc(sizeof(int) * size);

	for (i = 0; i < size; i++) { //minor_matrix�� ũ�� ����
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

	for (i = 0; i < size; i++) { //minor_matrix�� det�� ���
		det[i] = calculateDet(minor_matrix[i], size - 1);
	}

	for (i = 0; i < size; i++) { //��ȣ �����ؼ� ��ü det�� ���
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

		printf("��� %c = \n", Matrix[i].name);
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
		printf("Ȯ���ڸ�(.txt)�� ������ ���� �̸��� �Է��ϼ���: ");
		scanf("%s", file_name);

		strcat(file_name, ".txt");
		strcat(file_path, file_root);
		strcat(file_path, file_name);

		result = access(file_path, 0);

		if (result == 0) {
			printf("�ش� ������ �����ϹǷ� ����� �ҷ��ɴϴ�.\n");
			break;
		}
		else {
			printf("�ش� ������ �������� �ʽ��ϴ�.\n");
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
