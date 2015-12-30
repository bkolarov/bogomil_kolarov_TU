#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define MAX_FILE_NAME_LENGTH 20
#define READ_LINE_BUFFER_SIZE 128

#define CODE_SINGLE_LINE_COMMENT 1
#define CODE_SINGLE_LINE_COMMENT_WITH_NEW_LINE 2
#define CODE_MULTI_LINE_COMMENT 3

#define SINGLE_LINE_COMMENT "//"
#define MULTILINE_COMMENT_BEGIN "/*"
#define MULTILINE_COMMENT_END "*/"

#define MODE_READ_ONLY "r"

// Function: void chomp(char* string);
// Description: Removes the '\n', '\r', '\t' symbols from the end of a string
// Parameters: char* string - the string we are working on
void chomp(char* line);
void process(char* file_name);
int has_comment(char* text);

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

	process(file_name);

	system("pause");
	return 0;
}

void process(char* file_name) {
	FILE* file;
	char text_chunk[READ_LINE_BUFFER_SIZE];
	short comment_type = 0;
	short is_in_comment = 0, is_in_quotes = 0;

	chomp(file_name);
	file = fopen(file_name, MODE_READ_ONLY);

	if (file == NULL) {
		perror("Error");
		return NULL;
	}

	while (fgets(text_chunk, READ_LINE_BUFFER_SIZE, file) != NULL) {
		if ((comment_type = has_comment(text_chunk))) {
			switch (comment_type) {
			case CODE_SINGLE_LINE_COMMENT: 
				printf("%s", text_chunk);
				printf("single line comment\n\n");
				break;
			case CODE_SINGLE_LINE_COMMENT_WITH_NEW_LINE:

				break;
			case CODE_MULTI_LINE_COMMENT:
				printf("%s", text_chunk);
				printf("multi line comment\n\n");
				break;
			}
		}
	}
	// empty comment
	fclose(file);
}

int has_comment(char* text) {
	char* singleline_strstr = strstr(text, SINGLE_LINE_COMMENT);
	char* multiline_strstr = strstr(text, MULTILINE_COMMENT_BEGIN);

	int strlen_single_line_strstr = 0;
	int strlen_multiline_strstr = 0;

	if (singleline_strstr != NULL) {
		strlen_single_line_strstr = strlen(singleline_strstr);
	}

	if (multiline_strstr != NULL) {
		strlen_multiline_strstr = strlen(multiline_strstr);
	}

	if (strlen_single_line_strstr > strlen_multiline_strstr) {
		return CODE_SINGLE_LINE_COMMENT;
	}
	else if (strlen_single_line_strstr < strlen_multiline_strstr) {
		return CODE_MULTI_LINE_COMMENT;
	}

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
