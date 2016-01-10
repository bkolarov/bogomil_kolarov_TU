#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define MAX_FILE_NAME_LENGTH 20
#define READ_LINE_BUFFER_SIZE 128

#define CODE_SINGLE_LINE_COMMENT 1
#define CODE_SINGLE_LINE_COMMENT_WITH_NEW_LINE 2
#define CODE_MULTILINE_COMMENT_START 3

#define SINGLE_LINE_COMMENT "//"
#define MULTILINE_COMMENT_BEGIN "/*"
#define MULTILINE_COMMENT_END "*/"
#define ESCAPE_SYMBOL '\\'
#define QUOTE '\"'
#define NEW_LINE '\n'
#define TERMINATING_CHARACTER '\0'

#define MODE_READ_ONLY "r"

#define True 1;
#define False 0;

void chomp(char* line);
void process(char* file_name);
short is_new_line_escaped(char* string);

int main() {
	char file_name[MAX_FILE_NAME_LENGTH];
	char* check;

	printf("%d\n", is_new_line_escaped("\\bla\tbla\\\n"));

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
	char line[READ_LINE_BUFFER_SIZE];
	char escape_and_symbol[3];
	
	int comment_count = 0;

	chomp(file_name);
	file = fopen(file_name, MODE_READ_ONLY);

	if (file == NULL) {
		perror("Error");
		return;
	}

	int index;
	escape_and_symbol[2] = '\0';
	short skip_line = False;
	while (fgets(line, READ_LINE_BUFFER_SIZE, file) != NULL) {
		if (skip_line) {
			if (is_new_line_escaped) {
				skip_line = True;
			}
			else {
				skip_line = False;
			}

			continue;
		}

		for (index = 0; index < strlen(line) - 1; index++) {
			escape_and_symbol[0] = line[index];
			escape_and_symbol[1] = line[index + 1];
			
			if (escape_and_symbol[0] == '\\' && escape_and_symbol[1] != NEW_LINE) {
				index += 1;
				continue;
			}

			if (!strcmp(escape_and_symbol, SINGLE_LINE_COMMENT)) {
				comment_count++;
				if (is_new_line_escaped(line)) {
					skip_line = True;
				}
			}

		}


	}

	fclose(file);
}

short is_new_line_escaped(char* string) {
	char c;
	int index;
	int new_line_position = 0;
	int escape_symbol_count = 0;
	
	
	for (index = 0; index < strlen(string); index++) {
		if (string[index] == NEW_LINE) {
			new_line_position = index;
			break;
		}
	}
	
	if (new_line_position >= 1) {
		for (index = new_line_position - 1; index > 0; index--) {
			if (string[index] != ESCAPE_SYMBOL) {
				break;
			}
			else {
				escape_symbol_count++;
			}
		}

		if (escape_symbol_count % 2 != 0) {
			return True;
		}
		else {
			return False;
		}
	}
	else {
		return -1;
	}

}

void chomp(char* string) {
	int index;
	for (index = 0; string[index] != '\0'; index++) {
		if (string[index] == '\n' || string[index] == '\r' || string[index] == '\t') {
			string[index] = '\0';
		}
	}
}
