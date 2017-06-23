#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define READ_BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " \t\r\n\a"

/* 
Core functions
*/
void sh_loop();

char* sh_read_line();
char** sh_parse_line(char *line);
int sh_execute(char **tokens);


/* 
Built-in commands
*/
int sh_cd(char **cmd);


void print_tokens(char **tokens);

char* sh_read_line(){
	char *line = NULL;
	size_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;

}

char** sh_parse_line(char *line){
	int bufsize = TOKEN_BUFFER_SIZE;
	int token_position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr,"tsh: allocation error\n");
		exit(EXIT_FAILURE);
	}


	token = strtok(line,TOKEN_DELIM);

	while (token != NULL){
		tokens[token_position] = token;
		token_position++;

		if(token_position >= bufsize){
			bufsize += TOKEN_BUFFER_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if (!tokens){
				fprintf(stderr, "tsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL,TOKEN_DELIM);
	}
	tokens[token_position] = NULL;
	return tokens;
}

int sh_execute(char **tokens){
	if(tokens[0] == NULL){
		return 0;
	}

	return 0;
}


int sh_cd(char **cmd){
	if(cmd[0] == NULL){
		fprintf(stderr, "tsh: empty arguments for \"cd\"\n");
	} else{
		if(chdir(cmd[1]) != 0){
			perror("tsh");
		}
	}
	return 1;
}

void print_tokens(char **tokens){
	int pos = 0;
	printf("Tokens: [");
	while(tokens[pos] != NULL){
		if(tokens[pos + 1] != NULL){
			printf("%s, ",tokens[pos]);
		}else{
			printf("%s",tokens[pos]);
		}
		
		pos++;
	}
	printf("]\n");
}


void sh_loop(){
	int status = 0;
	char *line;
	char **tokens;
	
	while(status == 0){
		//do stuff here
		line = sh_read_line();
		printf("line: %s",line);
		tokens = sh_parse_line(line);
		print_tokens(tokens);
		status = sh_execute(tokens);

		free(line);
		free(tokens);
	}	
}

int main(int argc, char **argv){
	printf("Tiny Shell opened, type help for instructions\n");
	sh_loop();
	return EXIT_SUCCESS;
}