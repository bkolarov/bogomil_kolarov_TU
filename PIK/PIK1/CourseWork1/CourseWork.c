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

const short True = 1;
const short False = 0;

void chomp(char* line);
void process(char* file_name);
// Finds the first occurence of the new line symbol and cheks if it is escaped.
// Returns 1 if the new line symbol is escaped, 0 if it is not, -1 if no new line symbol was found.\
//
short is_new_line_escaped(char* string);
/**/
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
	char line[READ_LINE_BUFFER_SIZE];
	char escape_and_symbol[3];
	escape_and_symbol[2] = '\0';

	int comment_count = 0;

	chomp(file_name);
	file = fopen(file_name, MODE_READ_ONLY);

	if (file == NULL) {
		perror("Error");
		return;
	}

	short skip_line = False;
	short is_in_multiline_comment = False;
	while (fgets(line, READ_LINE_BUFFER_SIZE, file) != NULL) {
		if (skip_line) {
			if (is_new_line_escaped > False) {
				skip_line = True;
			}
			else {
				skip_line = False;
			}

			continue;
		}
		
		int index;
		for (index = 0; index < strlen(line) - 1; index++) {
			escape_and_symbol[0] = line[index];
			escape_and_symbol[1] = line[index + 1];

			if (is_in_multiline_comment) {
				if (!strcmp(escape_and_symbol, MULTILINE_COMMENT_END)) {
					is_in_multiline_comment = False;
				}
			}

			if (!is_in_multiline_comment) {
				if (!strcmp(escape_and_symbol, SINGLE_LINE_COMMENT)) {
					comment_count++;
					if (is_new_line_escaped(line) > False) {
						skip_line = True;
					}

					continue;
				}
				else if (!strcmp(escape_and_symbol, MULTILINE_COMMENT_BEGIN)) {
					comment_count++;
					is_in_multiline_comment = True;
				}
			}

		}
	}

	printf("comments count: %d\n", comment_count);

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
