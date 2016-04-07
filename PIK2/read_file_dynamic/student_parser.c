#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIVIDER "|"

#define CHUNK_SIZE 16

#define ID 0
#define NAME 1
#define	FAK_NO 2 // :DD read that loud
#define UNI 3
#define GRADE 4

int main() {

	FILE* fp;

	fp = fopen("workfile", "rb");

	int buffer_elements_count = CHUNK_SIZE;
	char* buffer = (char*) calloc(buffer_elements_count, sizeof(char));
	char* p_buffer = buffer;

	while (fgets(p_buffer, CHUNK_SIZE, fp) != NULL) {
		int buffer_len = strlen(buffer);
		
		if (buffer[buffer_len-1] != '\n' && !feof(fp)) {

			do {
				buffer_elements_count += CHUNK_SIZE;
			
				buffer = (char*) realloc(buffer, buffer_elements_count);
				p_buffer = &buffer[buffer_len];
				
				fgets(p_buffer, CHUNK_SIZE, fp);

				buffer_len = strlen(buffer);
			} while(buffer[buffer_len-1] != '\n' && !feof(fp));
			
			if (buffer[buffer_len-1] != '\n') {
				if (buffer_len + 1 == buffer_elements_count) {
					buffer = (char*) realloc(buffer, buffer_elements_count + 1);
					buffer_len = strlen(buffer);
				}
				
					buffer[buffer_len] = '\n';
					buffer[buffer_len + 1] = '\0';
			}			
			
		}
		
		printf("%s", buffer);

		char* token = strtok(buffer, DIVIDER);
		int token_number;
		for (token_number = 0; token != NULL; token_number++) {
			switch (token_number) {
			
			case ID:

				printf("id: %d\n", atoi(token));

				break;
			case NAME:

				printf("name: %s\n", token);

				break;
			case FAK_NO: // read it again :D
				
				printf("faculty number: %ld\n", atol(token));
				int last_number_of_fak_no = token[strlen(token) - 1] - '0';
				
				if (last_number_of_fak_no % 2 != 0)  {
					printf("odd faculty number: %s", buffer);
				}

				break;
			case UNI:

				printf("the waste of time place (if bulgarian): %s\n", token);

				break;

			case GRADE:

				printf("grade: %d\n", atoi(token));

				break;

			}

			token = strtok(NULL, DIVIDER);
		}

		printf("\n=================\n");

		free(buffer);
		buffer_elements_count = CHUNK_SIZE;
		buffer = (char*) calloc(buffer_elements_count, sizeof(char));
		p_buffer = buffer;		
	}

	fclose(fp);

	return 0;
}
