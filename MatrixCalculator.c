#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
//Declaring funcs
void UI(int matSize);
void printMenu();
int getSize();
void addNewMatrix(int size);
void deleteMatrix(int size);
void incrementMatrix(int size);
void decrementMatrix(int size);
void dupMatrix(int size);
void powerMatrix(int size);
void powAssist(int size, int index, double** tempMatrix);
void printMatrix(int size);
//GLOBAL VARS
double*** matrixArr;
double** matrix;
int countMatrix = 0;
char name[20][3] = { 0 };//---> Assuming there's no more than 19 matrices..(Changeable to any number of course)
//Main function
void main() {
	int i;
	//Gets the size n*n for the matrix
	int matSize = getSize();
	//calls for the UI (Menu for the program)
	UI(matSize);
	if (countMatrix != 0) {
		//free the main array
		free(matrixArr);
		//free the 2d array
		for (i = 0; i < matSize; i++) {
			free(matrix[i]);
		}
		free(matrix);
	}
}
//UI - Menu
void UI(int matSize) {
	int choice = 0;
	do {
		//Prints menu
		printMenu();
		//Gets choice from user
		while (scanf_s("%d", &choice) != 1) {
			scanf_s("%*s");
			printf("\tHey no chars ! enter a number !\n");
		}
		switch (choice) {
		case 1: addNewMatrix(matSize);
			break;
		case 2: deleteMatrix(matSize);
			break;
		case 3: incrementMatrix(matSize);
			break;
		case 4: decrementMatrix(matSize);
			break;
		case 5: dupMatrix(matSize);
			break;
		case 6: powerMatrix(matSize);
			break;
		case 7: printMatrix(matSize);
			break;
		case 8:
			puts("\n\t\t\t\t----------------------------------------");
			puts("\t\t\t\t|| Hope you had fun ! See you Later ! ||");
			puts("\t\t\t\t----------------------------------------\n\n");
			break;
		default: puts("Invalid input !");
			break;
		}
	} while (choice != 8);

}
//Prints the menu
void printMenu() {
	puts("\n\n--------------------\n||\tMenu\t  ||\n--------------------");
	puts("1. Add A New Matrix");
	puts("2. Delete A Matrix");
	puts("3. Increment Matrices");
	puts("4. Decrement Matrices");
	puts("5. Matrix Duplication");
	puts("6. Power Matrix");
	puts("7. Print Matrix");
	puts("8. End Program");
	printf("Your choice ------>  ");
}
//Gets size n*n of square matrix
int getSize() {
	int size;
	puts("\t\t\t\t---------------------------------------");
	puts("\t\t\t\t Welcome to Yarin's Matrix Calculator !");
	puts("\t\t\t\t---------------------------------------\n");
	printf("Please enter a size for the SQUARE matrix : \n");
	while (scanf_s("%d", &size) != 1) { //Gets size from user, and protects from char input
		scanf_s("%*s");
		printf("Invalid Input !\nTry Again.\n");
	}
	while (size < 1) {//Below that, it won't be a matrix (;
		puts("Invalid Input !\nTry Again.");
		while (scanf_s("%d", &size) != 1) { //Gets size from user, and protects from char input
			scanf_s("%*s");
			printf("Invalid Input !\nTry Again.\n");
		}
	}
	return size;
}
//Add a Matrix
void addNewMatrix(int size) {
	int i, j, c;
	char tempName[255];
	puts("Please enter a name for your matrix : ");
	scanf("%s", tempName);
	while ((c = getchar()) != '\n' && c != EOF) {}//CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(tempName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(tempName[1])) || tempName[1] == '\0' || tempName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists already, if it does, kicks you to menu
	for (i = 0; i < 20; i++) {
		if (strcmp(tempName, name[i]) == 0) {
			puts("\nThat matrix already exists !\nReturning to menu.");
			return;
		}
	}
	//After all the checks, puts the name in the names array and clears the temp name array
	strcpy(name[countMatrix], tempName);
	tempName[0] = '\0';
	//Allocating size for the square matrix
	matrix = (double**)malloc(size * sizeof(double*));
	for (i = 0; i < size; i++) {
		matrix[i] = (double*)malloc(size * sizeof(double));
	}
	//-----------GETS MATRIX NUMBERS(WITH INPUT VALIDATION)--------------
	for (i = 0; i < size; i++) {
		printf("Enter numbers for row number : %d\n", (i + 1));
		for (j = 0; j < size; j++) {
			//checks return value of scanf, if it scanned anything but numbers it breaks the loop.
			while (scanf("%lf", &matrix[i][j]) != 1) {
				scanf_s("%*s");
				printf("\nNo chars allowed !\nReturning to menu.");
				for (i = 0; i < size; i++) {
					free(matrix[i]);
				}
				free(matrix);
				name[countMatrix][0] = '\0';
				return;
			}
		}
		//This condition protects from too many numbers in a line
		c = getc(stdin);
		if (c != '\n') {
			for (i = 0; i < size; i++) {
				free(matrix[i]);
			}
			free(matrix);
			name[countMatrix][0] = '\0';
			puts("Too many numbers in line.\n Returning to menu.");
			while ((c = getchar()) != '\n' && c != EOF) {}//CLEAR BUFFER
			return;
		}
	}
	//Allocating space in the pointer to matrix array(malloc doesn't work here)
	matrixArr = (double***)realloc(matrixArr, (countMatrix + 1) * sizeof(double**));
	matrixArr[countMatrix] = matrix;
	countMatrix++;// Counts how many Matrices have been added (global var)
	puts("\nThe Matrix Has Been Succesfully Added !");
}
//Delete a Matrix
void deleteMatrix(int size) {
	int c, index, i, flag = 0, j;
	char matName[255] = { 0 };
	puts("Please choose which matrix to delete : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does, gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nA Matrix by that name wasn't created !\nReturning to Menu");
		return;
	}
	//frees the matrix
	for (j = 0; j < size; j++) {
		free(matrixArr[index][j]);
	}
	free(matrixArr[index]);
	//swap places in both matrix and names arrays (to avoid holes in the memory)
	if (countMatrix != 0) {
		matrixArr[index] = matrixArr[countMatrix - 1];
		matrixArr[countMatrix - 1] = NULL;
		strcpy(name[index], name[countMatrix - 1]);
		name[countMatrix - 1][0] = '\0';
		countMatrix--;
		printf("\nThe Matrix %s Has Been Deleted.\nReturning to Menu.\n", matName);
	}

}
//Increment a Matrix
void incrementMatrix(int size) {
	int c, index, index1, i, flag = 0, j;
	char matName[255] = { 0 };
	char matName1[255] = { 0 };
	puts("Please choose 1st matrix to add : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nThere's no such Matrix !\nReturning to Menu");
		return;
	}
	puts("Please choose 2nd matrix to add : ");
	scanf("%s", matName1);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName1[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName1[1])) || matName1[1] == '\0' || matName1[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName1, name[i]) == 0) {
			index1 = i;
			flag = 1;
			break;
		}
		//if the name doesn't exist the flag turns and returns to menu
		if (flag != 1) {
			puts("\nThere's no such Matrix !\nReturning to Menu");
			return;
		}
	}
	//ADDS THE 2 MATRICES
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++) {
			matrixArr[index1][i][j] = matrixArr[index][i][j] + matrixArr[index1][i][j];
		}
	puts("\nYour Action Was Successful !");
}
//Decrement a Matrix
void decrementMatrix(int size) {
	int c, index, index1, i, flag = 0, j;
	char matName[255] = { 0 };
	char matName1[255] = { 0 };
	puts("Please choose 1st matrix to decrease : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nThere's no such Matrix !\nReturning to Menu");
		return;
	}
	puts("Please choose 2nd matrix to decrease : ");
	scanf("%s", matName1);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName1[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName1[1])) || matName1[1] == '\0' || matName1[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName1, name[i]) == 0) {
			index1 = i;
			flag = 1;
			break;
		}
		//if the name doesn't exist the flag turns and returns to menu
		if (flag != 1) {
			puts("\nThere's no such Matrix !\nReturning to Menu");
			return;
		}
	}
	//DECREASE THE 2 MATRICES
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++) {
			matrixArr[index1][i][j] = matrixArr[index][i][j] - matrixArr[index1][i][j];
		}
	puts("\nYour Action Was Successful !");
}
//Duplicate a Matrix
void dupMatrix(int size) {
	//Declare vars and arrays for local func use
	int c, index, index1, i, flag = 0, j, k;
	char matName[255] = { 0 };
	char matName1[255] = { 0 };
	puts("Please choose 1st matrix to add : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nThere's no such Matrix !\nReturning to Menu");
		return;
	}
	puts("Please choose 2nd matrix to add : ");
	scanf("%s", matName1);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName1[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName1[1])) || matName1[1] == '\0' || matName1[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName1, name[i]) == 0) {
			index1 = i;
			flag = 1;
			break;
		}
		//if the name doesn't exist the flag turns and returns to menu
		if (flag != 1) {
			puts("\nThere's no such Matrix !\nReturning to Menu");
			return;
		}
	}
	//Make the temporary matrix
	double** tempMatrix = (double**)malloc(sizeof(double*) * size);
	for (int i = 0; i < size; i++) {
		tempMatrix[i] = (double*)calloc(size, sizeof(double));
	}
	//Duplicate the Matrices into a temp matrix
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				tempMatrix[i][j] += matrixArr[index][i][k] * matrixArr[index1][k][j];
			}
		}
	}
	//Assigning the temp matrix into the one we'd like to change
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrixArr[index1][i][j] = tempMatrix[i][j];
		}
	}
	//Free temp matrix
	for (j = 0; j < size; j++) {
		free(tempMatrix[j]);
	}
	free(tempMatrix);
	puts("\nYour Action Was Successful !");
}
//Power a Matrix
void powerMatrix(int size) {
	int i, flag = 0, index, c, power, j;
	char matName[255] = { 0 };
	puts("Please choose which matrix to raise by power : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//The if-else is for checking valid input
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nThere's no such Matrix !\nReturning to Menu");
		return;
	}
	//Gets the power input from user
	puts("Please choose the power :");
	while (scanf_s("%d", &power) != 1) {
		scanf_s("%*s");
		printf("\nHey no chars ! enter a number !\n");
	}
	//Valid power is between 2-10 as said (0,1 are not powers to a matrix)
	if (power > 10 || power < 2) {
		puts("\nYour number is OUT OF RANGE !\nReturning to Menu");
		return;
	}
	//Make the temporary matrix
	double** tempMatrix = (double**)malloc(sizeof(double*) * size);
	for (i = 0; i < size; i++) {
		tempMatrix[i] = (double*)calloc(size, sizeof(double));
	}
	//Makes an identity matrix (I)
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (i == j)
				tempMatrix[i][j] = 1;
			else
				tempMatrix[i][j] = 0;
		}
	}
	//Iterates n times (power times) - calls an assisting function
	for (i = 0; i < power; i++) {
		powAssist(size, index, tempMatrix);
	}
	//Puts the temp matrix in the regular matrix
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrixArr[index][i][j] = tempMatrix[i][j];
		}
	}
	//Free temp matrix
	for (i = 0; i < size; i++) {
		free(tempMatrix[i]);
	}
	free(tempMatrix);

	puts("\nYour Action Was Successful !");
}
//Assist to PowerMatrix
void powAssist(int size, int index, double** tempMatrix) {
	//Makes a local temp matrix
	double** temp1 = (double**)malloc(sizeof(double*) * size);
	for (int i = 0; i < size; i++) {
		temp1[i] = (double*)calloc(size, sizeof(double));
	}
	int i, j, k;
	//This loop is for multiplication...
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			temp1[i][j] = 0;
			for (k = 0;k < size;k++) {
				temp1[i][j] += matrixArr[index][i][k] * tempMatrix[k][j];
			}
		}
	}
	//Puts the local temp matrix in the temp that goes back
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			tempMatrix[i][j] = temp1[i][j];
		}
	}
	//Free local temp matrix
	for (i = 0; i < size; i++) {
		free(temp1[i]);
	}
	free(temp1);
}
//Print a Matrix
void printMatrix(int size) {
	int i, flag = 0, index, c, j;
	char matName[255] = { 0 };
	puts("Please choose which matrix to print : ");
	scanf("%s", matName);
	while ((c = getchar()) != '\n' && c != EOF) {} //CLEAR BUFFER
	//THE IF-ELSE IS FOR CHECKING VALID INPUT
	if (!isupper(matName[0])) {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	else if ((!isdigit(matName[1])) || matName[1] == '\0' || matName[2] != '\0') {
		puts("\nInvalid Input !\nReturning to Menu");
		return;
	}
	//Checks if the matrix's name exists, if it does gets the index in the name array
	for (i = 0; i < 20; i++) {
		if (strcmp(matName, name[i]) == 0) {
			index = i;
			flag = 1;
			break;
		}
	}
	//if the name doesn't exist the flag turns and returns to menu
	if (flag != 1) {
		puts("\nThere's no such Matrix !\nReturning to Menu");
		return;
	}
	//prints the matrix by the name array's index (name points to matrix)
	puts("\n Your Matrix Is \n");
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			printf("  %0.3lf", matrixArr[index][i][j]);
		}
		printf("\n");
	}
}