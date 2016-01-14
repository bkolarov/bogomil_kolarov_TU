// Course Work Bogomil Kolarov
/*
*/
#include <visual_2012.h>
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
#define SPACE_STR = " "

#define TERMINATING_CHARACTER '\0'

#define IDENTIFIERS_TXT "identifiers.txt"
#define INPUT_PROGRAM_FILE_NAME "input.c"
#define MODE_READ_ONLY "r"
#define MODE_READ_AND_APPEND "a+"

const short True = 1; //
const short False = 0;

void chomp(char* line);
void process(char* file_name, FILE* result_output);
// Finds the first occurence of the new line symbol and cheks if it is escaped.

// Returns 1 if the new line symbol is escaped, 0 if it is not, -1 if no new line symbol was found.\
//\
//
short is_new_line_escaped(char* string);
short is_symbol_escaped(char* string, char symbol, int index_symbol);
short is_c_type(char* word);
short is_c_non_type_keyword(char* word);
short is_identifier_already_counted(char* identifier);

void remove_spaces(char* source);
void normalize_identifier(char* identifier);

int count_identifiers_with_exceptions(char* src, int comment_start, int comment_end, int string_start, int string_end, int* count);
int count_identifiers_except_bounds(char* src, int start, int end, int* count);
int count_identifiers(char* string, int* count);

struct Quote has_string(char* string);
/*
//
*/

struct Quote {
	short has_string;
	int starts_at;
	int ends_at;
};

struct MultilineComment {
	int starts_at;
	int ends_at;
};

void clear_stdin() {
	int ch;
	while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
}

void read_filename(char* file_name) {
	char* check;

	check = fgets(file_name, MAX_FILE_NAME_LENGTH, stdin);

	if (check == NULL) {
		if (!feof(stdin)) {
			perror("Read error ocurred: ");
		}
	}
}

short is_c_file(char* name) {
	if (strlen(name) < 3) {
		return False;
	}

	if (name[strlen(name) - 2] != '.' && name[strlen(name) - 1] != 'c') {
		return False;
	}
	else {
		return True;
	}
}

void check_file_opening(FILE* file) {
	if (file == NULL) {
		perror(ERROR);
		system("Pause");
		exit(0);
	}
}

void write_program_to_file(char* file_name) {
	FILE* file = fopen(file_name, "w");
	char buffer_line[READ_LINE_BUFFER_SIZE];
	check_file_opening(file);

	printf("write your awesome code here: \n");

	while (fgets(buffer_line, READ_LINE_BUFFER_SIZE, stdin) != NULL) {
		fprintf(file, "%s", buffer_line);
		if (buffer_line[strlen(buffer_line) - 1] != NEW_LINE) {
			fprintf(file, "%c", '\n');
		}
	}

	fclose(file);
}

int main() {
	char input_file_name[MAX_FILE_NAME_LENGTH];
	char output_file_name[MAX_FILE_NAME_LENGTH];
	char* check;
	int mode = 0;

	FILE* output_file;

	SetConsoleOutputCP(1251);

	fclose(fopen(IDENTIFIERS_TXT, "w"));

	printf("Изберете опция: \n");
	printf("Опция 1 :  read from file and write to file\n");
	printf("Опция 2 :  read from file and write to screen\n");
	printf("Опция 3 :  read from user and write to file\n");
	printf("Опция 4 :  read from user and write to screen\n");
	

	while (mode < 1 || mode > 4) {
		printf("Enter mode: ");
		scanf("%d", &mode);
		clear_stdin();

		switch (mode) {
		case 1:
			printf("Enter input file name: ");
			read_filename(input_file_name);
			

			printf("Enter outut file name: ");
			read_filename(output_file_name);

			chomp(input_file_name);

			if (!is_c_file(input_file_name)) {
				printf("invalid input filename\n");
				mode = 0;
			}
			else {
				chomp(output_file_name);
				output_file = fopen(output_file_name, "w");

				check_file_opening(output_file);

				process(input_file_name, output_file);
				fclose(output_file);
			}

			break;
		case 2:
			printf("Enter input file name: ");
			read_filename(input_file_name);

			if (!is_c_file(input_file_name)) {
				printf("invalid input filename\n");
				mode = 0;
			}
			else {
				process(input_file_name, stdout);
			}

			break;
		case 3:
			printf("Enter outut file name: ");
			read_filename(output_file_name);

			chomp(output_file_name);
			output_file = fopen(output_file_name, "w");

			write_program_to_file(INPUT_PROGRAM_FILE_NAME);
			process(INPUT_PROGRAM_FILE_NAME, output_file);
			break;
		case 4:
			write_program_to_file(INPUT_PROGRAM_FILE_NAME);
			process(INPUT_PROGRAM_FILE_NAME, stdout);
			break;
		}
	}

	system("pause");
	return 0;
}

void process(char* file_name, FILE* result_output) {
	FILE* file;
	char line[READ_LINE_BUFFER_SIZE];

	int comment_count = 0;
	int identifiers_count = 0;

	short skip_line = False;
	short is_in_multiline_comment = False;

	struct Quote string_bounds;
	struct MultilineComment multiline_comment;

	int index;
	char c;
	int singleline_comment_index, multiline_comment_index, quote_index;
	int i;

	file = fopen(file_name, MODE_READ_ONLY);

	check_file_opening(file);
	
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

		string_bounds = has_string(line);
		
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
						multiline_comment.ends_at = index + 1;

						if (string_bounds.has_string) {
							identifiers_count += count_identifiers_with_exceptions(line, multiline_comment.starts_at, multiline_comment.ends_at, string_bounds.starts_at, string_bounds.ends_at, &identifiers_count);
						}
						else {
							identifiers_count += count_identifiers_with_exceptions(line, multiline_comment.starts_at, multiline_comment.ends_at, -1, -1, &identifiers_count);
						}
						

						is_in_multiline_comment = False;
						multiline_comment.starts_at = -1;
						multiline_comment.ends_at = -1;
					}
				}
			}
		}

		if (!is_in_multiline_comment) {
			if ((singleline_comment_index + multiline_comment_index) < 2 * (READ_LINE_BUFFER_SIZE)) {

				if ((singleline_comment_index < multiline_comment_index) 
					&& ((string_bounds.has_string && (singleline_comment_index < string_bounds.starts_at || singleline_comment_index > string_bounds.ends_at)) 
						|| !string_bounds.has_string)) {
					
					if (string_bounds.has_string) {
						identifiers_count += count_identifiers_with_exceptions(line, singleline_comment_index, -1, string_bounds.starts_at, string_bounds.ends_at, &identifiers_count);
					}
					else {
						identifiers_count += count_identifiers_with_exceptions(line, singleline_comment_index, -1, -1, -1, &identifiers_count);
					}

					comment_count++;

					if (is_new_line_escaped(line)) {
						skip_line = True;
						continue;
					}
				}
				else if ((multiline_comment_index < singleline_comment_index)
					&& (string_bounds.has_string 
						&& (multiline_comment_index < string_bounds.starts_at || multiline_comment_index > string_bounds.ends_at))
							|| !string_bounds.has_string) {

					multiline_comment.starts_at = multiline_comment_index;
					comment_count++;
					
					for (i = 0; i < strlen(line) - 1; i++) {
						if (line[i] == '*' && line[i + 1] == '/') {
							is_in_multiline_comment = False;
							break;
						}
						else {
							is_in_multiline_comment = True;
						}
					}
				}
			}
			else {
				if (string_bounds.has_string) {
					count_identifiers_with_exceptions(line, string_bounds.starts_at, string_bounds.ends_at, -1, -1, &identifiers_count);
				}
				else {
					count_identifiers(line, &identifiers_count);
				}
			}
		}
	}

	fprintf(result_output, "comments count: %d\n", comment_count);
	fprintf(result_output, "identifiers char count: %d\n", identifiers_count);

	fclose(file);
}

int count_identifiers(char* src, int* count) {
	int identifiers_char_count = 0;
	char string[READ_LINE_BUFFER_SIZE];
	

	short  is_type = False;
	char* token;

	strcpy(string, src);

	token = strtok(string, " ");
	chomp(token);
	while (token != NULL) {
		remove_spaces(token);
		normalize_identifier(token);

		is_type =/* COMMENTED_IDENTIFIER */ is_c_type(token);

		if (!is_type && !is_c_non_type_keyword(token)) {
			if (!is_identifier_already_counted(token)) {
				*count += strlen(token);
			}
		}

		token = strtok(NULL, " ");
	}
	
	return identifiers_char_count;
}

void normalize_identifier(char* identifier) {
	int index;

	for (index = 0; index < strlen(identifier); index++) {
		if (identifier[index] != '_') {
			if (index == 0) {
				if (!isalpha(identifier[index])) {
					identifier[index] = TERMINATING_CHARACTER;
					return;
				}
			}
			else {
				if (!isalpha(identifier[index]) && !isdigit(identifier[index])) {
					identifier[index] = TERMINATING_CHARACTER;
				}
			}
		}
	}
}

int count_identifiers_with_exceptions(char* src, int comment_start, int comment_end, int string_start, int string_end, int* count) {
	if (comment_start > -1) {
		if (string_start > -1) {
			if (comment_start < string_start) {
				return count_identifiers_except_bounds(src, comment_start, comment_end, count);
			}
			else {
				return count_identifiers_except_bounds(src, string_start, string_end, count);
			}
		}
		else {
			return count_identifiers_except_bounds(src, comment_start, comment_end, count);
		}
	}
	else if (string_start > -1) {
		return count_identifiers_except_bounds(src, string_start, string_end, count);
	}
}

int count_identifiers_except_bounds(char* src, int start, int end, int* count) {
	int identifiers_count = 0;
	char src_cpy[READ_LINE_BUFFER_SIZE];
	char* p_src_cpy = src_cpy;

	strcpy(src_cpy, src);

	if (start > 0) {
		src_cpy[start] = '\0';
		identifiers_count += count_identifiers(p_src_cpy, count);
		if (end > -1) {
			strcpy(src_cpy, src);
			p_src_cpy = &src_cpy[end + 1];
			identifiers_count += count_identifiers(p_src_cpy, count);
		}
	}
	
	return identifiers_count;
}

short is_identifier_already_counted(char* identifier) {
	FILE* file;
	char line[80];
	file = fopen(IDENTIFIERS_TXT, MODE_READ_AND_APPEND);

	if (file == NULL) {
		printf("Error when trying to open file identifiers.txt\n");
		perror("Message: ");
		return False;
	}

	while (fgets(line, 80, file) != NULL) {
		chomp(line);
		if (strcmp(line, identifier) == 0) {
			fclose(file);
			return True;
		}
	}

	fprintf(file, "%s", identifier);
	fprintf(file, "\n");
	fclose(file);

	return False;
}

void remove_spaces(char* source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (*i != ' ' && *i != '\t')
			i++;
	}
	*i = 0;
}

struct Quote has_string(char* string) {
	struct Quote result;
	int index;
	char c;

	result.has_string = False;

	for (index = 0; index < strlen(string); index++) {
		c = string[index];
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

short is_c_non_type_keyword(char* word) {
	const char* keywords[] = { "auto", "break", "case",
		"const", "continue", "default", "do",
		"else", "enum", "extern", "for", 
		"goto", "if", "include", "register", 
		"return", "sizeof", "static",
		"struct", "switch", "typedef",
		"union", "volatile", "while" };
	int counter;

	for (counter = 0; counter < 24; counter++) {
		if (strcmp(keywords[counter], word) == 0) {
			return True;
		}
	}

	return False;
}

short is_c_type(char* word) {
	const char* keywords[] = {
		"char", "double", "define",
		"float", "int", "long","short",
		"signed", "unsigned", "void" };
	int counter;

	for (counter = 0; counter < 10; counter++) {
		if (strcmp(keywords[counter], word) == 0) {
			return True;
		}
	}

	return False;
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
	short is_escaped;

	for (index = 0; index < strlen(string); index++) {
		if (string[index] == NEW_LINE) {
			new_line_position = index;
			break;
		}
	}
	
	is_escaped = is_symbol_escaped(string, NEW_LINE, new_line_position);
	
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
