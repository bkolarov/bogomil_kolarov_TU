// Couse Work Bogomil Kolarov\
//
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#define MAX_FILE_NAME_LENGTH 20
#define READ_LINE_BUFFER_SIZE 128

#define ERROR "Error"

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
//\
//
short is_new_line_escaped(char* string);
short is_symbol_escaped(char* string, char symbol, int index);
short is_symbol_escaped(char* string, char symbol, int index_symbol);

struct Quote has_string(char* string);
/*
*/

struct Quote {
	short has_string;
	int starts_at;
	int ends_at;
};

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

	chomp(file_name);
	process(file_name);


	system("pause");
	return 0;
}

void process(char* file_name) {
	FILE* file;
	char line[READ_LINE_BUFFER_SIZE];

	int comment_count = 0;

	file = fopen(file_name, MODE_READ_ONLY);

	if (file == NULL) {
		perror(ERROR);
		return;
	}

	short skip_line = False;
	short is_in_multiline_comment = False;
	
	while (fgets(line, READ_LINE_BUFFER_SIZE, file) != NULL) {
		if (skip_line) {
			if (is_new_line_escaped(line) > False) {
				skip_line = True;
			}
			else {
				skip_line = False;
			}

			continue;
		}

		struct Quote string_bounds = has_string(line);

		int index;
		char c;
		int singleline_comment_index, multiline_comment_index, quote_index;
		singleline_comment_index = multiline_comment_index = quote_index = READ_LINE_BUFFER_SIZE;
 		for (index = 0; index < strlen(line) - 1; index++) {
			c = line[index];

			if (!is_in_multiline_comment) {
				if (strlen(line) > 1) {
					if (c == '/') {
						if (line[index + 1] == '/') {
							singleline_comment_index = index;
							break;
						}
						else if (line[index + 1] == '*') {
							multiline_comment_index = index;
							break;
						}
					}
					else if (c == QUOTE) {
						quote_index = index;	
					}
				}
			}
			else {
				if (strlen(line) > 1) {
					if (c == '*' && line[index + 1] == '/') {
						is_in_multiline_comment = False;
					}
				}
			}
		}

		if (!is_in_multiline_comment) {
			if ((singleline_comment_index + multiline_comment_index) < 2 * (READ_LINE_BUFFER_SIZE)) {

				if ((singleline_comment_index < multiline_comment_index)
					&& (string_bounds.has_string 
						&& (singleline_comment_index < string_bounds.starts_at || singleline_comment_index > string_bounds.ends_at))
							|| !string_bounds.has_string) {

					comment_count++;
					printf("%s", line);
					if (is_new_line_escaped(line)) {
						skip_line = True;
						continue;
					}
				}
				else if ((multiline_comment_index < singleline_comment_index)
					&& (string_bounds.has_string 
						&& (multiline_comment_index < string_bounds.starts_at || multiline_comment_index > string_bounds.ends_at))
							|| !string_bounds.has_string) {

					comment_count++;
					printf("%s", line);
					is_in_multiline_comment = True;
				}
			}
		}
	}

	printf("comments count: %d\n", comment_count);

	fclose(file);
}

struct Quote has_string(char* string) {
	struct Quote result;

	result.has_string = False;

	int index;
	for (index = 0; index < strlen(string); index++) {
		char c = string[index];
		if (!result.has_string) {
			if (string[index] == QUOTE) {
				result.has_string = True;
				result.starts_at = index;
			}
		}
		else {
			if (string[index] == QUOTE) {
				short symbol_escaped = is_symbol_escaped(string, QUOTE, index);
				
				if (symbol_escaped < 0) {
					symbol_escaped = False;
				}
				
				if (!symbol_escaped) {
					result.ends_at = index;
					break;
				}
			}
		}
	}

	return result;
}

short is_symbol_escaped(char* string, char symbol, int index_symbol) {
	int index;
	int escape_symbol_count = -1;

	if (index_symbol >= 1) {
		for (index = index_symbol - 1; index > 0; index--) {
			if (string[index] != ESCAPE_SYMBOL) {
				break;
			}
			else {
				if (escape_symbol_count == -1) {
					escape_symbol_count = 0;
				}

				escape_symbol_count++;
			}
		}

		if (escape_symbol_count > 0 && escape_symbol_count % 2 != 0) {
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
	
	short is_escaped = is_symbol_escaped(string, NEW_LINE, new_line_position);
	
	return is_escaped;
}

void chomp(char* string) {
	int index;
	for (index = 0; string[index] != '\0'; index++) {
		if (string[index] == '\n' || string[index] == '\r' || string[index] == '\t') {
			string[index] = '\0';
		}
	}
}
