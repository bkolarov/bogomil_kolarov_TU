#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define MAX_FILE_NAME_LENGTH 20
#define READ_LINE_BUFFER_SIZE 128

#define MODE_READ_ONLY "r"

int read_file(char* file_name);
// Function: void chomp(char* string);
// Description: Removes the '\n', '\r', '\t' symbols from the end of a string
// Parameters: char* string - the string we are working on
void chomp(char* line);

int main() {
	char file_name[MAX_FILE_NAME_LENGTH];
	char* check;

	printf("Enter the name of the file: ");
	
	while ((check = fgets(file_name, MAX_FILE_NAME_LENGTH, stdin)) == NULL) {
		if (feof(stdin)) {
			break;
		}
		printf("error");
	}

	read_file(file_name);

	system("pause");
	return 0;
}

int read_file(char* file_name) {
	char buffer[READ_LINE_BUFFER_SIZE];
	char* fgets_check;
	FILE* file;

	chomp(file_name);
	file = fopen(file_name, MODE_READ_ONLY);

	if (file == NULL) {
		perror("Error");
		return -1;
	}

	while ((fgets_check = fgets(buffer, READ_LINE_BUFFER_SIZE, file)) != NULL) {
		printf("%s", buffer);
	}

	fclose(file);
	printf("\n");

	return 0;
}

void chomp(char* string) {
	int index;
	for (index = 0; string[index] != '\0'; index++) {
		if (string[index] == '\n' || string[index] == '\r' || string[index] == '\t') {
			string[index] = '\0';
		}
	}
}
